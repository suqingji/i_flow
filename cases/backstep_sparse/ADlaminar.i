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
[]

[Materials]
  [./const]
    type = ADGenericConstantMaterial
    prop_names = 'rho mu cp k'
    prop_values = '1  1e-6  1  1'
  [../]
  [ins_mat]
    type = INSADTauMaterial
    velocity = velocity
    pressure = p
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  nl_rel_tol = 1e-4
  nl_max_its = 600
  l_tol = 1e-3
  l_max_its = 600
  start_time = 0.0
  end_time = 100
  petsc_options_iname = '-pc_type -sub_pc_type -sub_pc_factor_shift_type -ksp_gmres_restart'
  petsc_options_value = 'lu   asm           NONZERO                   200'
  line_search = none
  [TimeStepper]
    type = RatioTimeStepper
    dt = 0.001
    ratio = 1.3
    step = 10
    max_dt = 1
  []
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]

#[Dampers]
#  [./const_damp]
#    type = ConstantDamper
#    damping = 0.1
#  [../]
#[]
