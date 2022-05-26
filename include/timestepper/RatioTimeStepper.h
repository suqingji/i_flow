#pragma once

#include "TimeStepper.h"

class RatioTimeStepper;

template <>
InputParameters validParams<RatioTimeStepper>();

/**
 * This class cuts the timestep in half at every iteration
 * until it reaches a user-specified minimum value.
 */
class RatioTimeStepper : public TimeStepper
{
public:
	RatioTimeStepper(const InputParameters &parameters);

protected:
	virtual Real computeInitialDT();

	virtual Real computeDT();

private:
	Real _ratio;
	Real _max_dt;
	int _step;
};
