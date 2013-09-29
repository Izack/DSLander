#include "APU.h"

APU::APU()
{
	parent=NULL;
	active=0;
	rpm=0;
	fuel_consumption=0;
}

APU::APU(VESSEL3 *v)
{
	parent = v;
	active=0;
	rpm=0;
	fuel_consumption=0;
}

bool APU::Activate(bool state)
{
	if(state)
		active = true;
	else
		active = false;
	return active;
}

double APU::Propagate(double SimDT, PROPELLANT_HANDLE ph)
{
	fuel_consumption = 0;
	if(active && parent->GetPropellantMass(ph)> 10)
	{
		if(rpm<4000)
			rpm=rpm+73*SimDT;
		else if(rpm>4000)
			rpm=4000;
	}
	else if(parent->GetPropellantMass(ph)<=0)
		rpm=0;
	else
	{
		if(rpm>0)
			rpm=rpm-137*SimDT;
		else if(rpm<0)
			rpm=0;
	}
	fuel_consumption=rpm*0.000001*SimDT;
	//Note: SS APU fuel consumption is given as about 5 pounds/minute, so ~0.03kg/s

	//double d = parent->GetPropellantMass(ph);
	//sprintf(oapiDebugString(),"%.3f",d);
	//parent->SetPropellantMass(ph,(parent->GetPropellantMass(ph))-(fuel_consumption*SimDT));
	//parent->SetPropellantMass(ph,100); //Cannot set propellant mass
	return fuel_consumption;
}