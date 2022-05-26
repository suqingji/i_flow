[Mesh]
  [gen]
    type = FileMeshGenerator
    file = back_quad_sparse.e
  []
[]

[Variables]
  [./velocity]
    family = LAGRANGE_VEC
  [../]
  [./p]
  [../]
  [kinetic]
    [InitialCondition]
      type = ConstantIC
      value = 0.001
    []
  []
  [omega]
    [InitialCondition]
      type = ConstantIC
      value = 1.0
    []
  []
[]

[ICs]
  [velocity]
    type = VectorConstantIC
    x_value = 1
    y_value = 1e-15
    variable = velocity
  []
[]

[Kernels]
  [./mass]
    type = INSADMass
    variable = p
  [../]
  [./mass_pspg]
    type = INSADMassPSPG
    variable = p
  [../]

  [./momentum_time]
    type = INSADMomentumTimeDerivative
    variable = velocity
  [../]

  [./momentum_convection]
    type = INSADMomentumAdvection
    variable = velocity
  [../]

  [./momentum_viscous]
    type = INSADMomentumViscous
    variable = velocity
  [../]

  [./momentum_pressure]
    type = INSADMomentumPressure
    variable = velocity
    pressure = p
    integrate_p_by_parts = true
  [../]

  [./momentum_supg]
    type = INSADMomentumSUPG
    variable = velocity
    velocity = velocity
  [../]

  [./kinetic_time]
    type = INSADKineticTimeDerivative
    variable = kinetic
  [../]

  [./kinetic_convection]
    type = INSADKineticAdvection
    variable = kinetic
  [../]

  [./kinetic_viscous]
    type = INSADKineticDiffusion
    variable = kinetic
  [../]

  [./kinetic_supg]
    type = INSADKineticSUPG
    variable = kinetic
    velocity = velocity
  [../]

  [./omega_time]
    type = INSADOmegaTimeDerivative
    variable = omega
  [../]

  [./omega_convection]
    type = INSADOmegaAdvection
    variable = omega
  [../]

  [./omega_viscous]
    type = INSADOmegaDiffusion
    variable = omega
  [../]

  [./omega_supg]
    type = INSADOmegaSUPG
    variable = omega
    velocity = velocity
  [../]
[]

[BCs]
  [./no_slip]
    type = VectorDirichletBC
    variable = velocity
    boundary = 'bottom top'
    values = '0 0 0'
  [../]

  [./in]
    type = VectorDirichletBC
    variable = velocity
    boundary = 'in'
    values = '1 0 0'
  [../]

  [./pressure_pin]
    type = DirichletBC
    variable = p
    boundary = 'out'
    value = 0
  [../]

  [./kin_wall]
    type = DirichletBC
    variable = kinetic
    boundary = 'bottom top'
    value = 0
  [../]

  [./omega_wall]
    type = FunctionDirichletBC
    variable = omega
    boundary = 'top bottom'
    function = 'wall_omega'
  [../]
  
  [./k_in]
    type = DirichletBC
    variable = kinetic
    boundary = 'in'
    value = 0.001
  [../]

  [./ome_in]
    type = DirichletBC
    variable = omega
    boundary = 'in'
    value = 1
  [../]
[]

[Materials]
  [./const]
    type = ADGenericConstantMaterial
    prop_names = 'rho mu_l cp k'
    prop_values = '1  1e-5  1  1e-6'
  [../]
  [ins_mat]
    type = INSADKOTurbStabilized
    velocity = velocity
    pressure = p
    kinetic = kinetic
    omega = omega
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  nl_rel_tol = 1e-2
  nl_max_its = 1000
  l_tol = 1e-3
  l_max_its = 60
  start_time = 0.0
  end_time = 50
  petsc_options_iname = '-pc_type -sub_pc_type -sub_pc_factor_shift_type -ksp_gmres_restart'
  petsc_options_value = 'lu   asm           NONZERO                   200'
  line_search = none
  [TimeStepper]
    type = RatioTimeStepper
    dt = 1e-5
    ratio = 1.3
    step = 2
    max_dt = 1
  []
[]
[Functions]
  [wall_omega]
    type = ParsedFunction
    value = 'max(100 + 40 * (t - 10), 100)'
    #value = 100
  []
[]
[Outputs]
  exodus = true
#  [./my_checkpoint]
#    type = Checkpoint
#    num_files = 4
#    interval = 10
#    execute_on = 'final'
#  [../]
#  [./my_tecplot]
#    type = Tecplot
#    execute_on = 'final'
#  [../]
[]
[Dampers]
  [./diffusion_damp]
    type = ConstantDamper
    damping = 0.1
  [../]
[]
#[Debug]
#  show_var_residual_norms = true
#[]