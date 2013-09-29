#include "APU.h"

APU::APU(VESSEL3 *v)
{
	parent = v;
}

bool APU::Activate(bool state = true)
{
	if(state)
		active == true;
	else
		active == false;
	return active;
}

double APU::Propagate(double SimDT, PROPELLANT_HANDLE ph)
{
	fuel_consumption = 0;
	if(active)
	{
		if(rpm<4000)
			rpm=rpm+5*SimDT;
		else if(rpm>4000)
			rpm=4000;
		fuel_consumption=rpm*0.01;
	}

	parent->SetPropellantMass(ph,parent->GetPropellantMass(ph)-fuel_consumption*SimDT);
	return fuel_consumption*SimDT;
}