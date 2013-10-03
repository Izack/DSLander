#include "APU.h"

APU::APU()
{
	parent=NULL;
	active=0;
	rpm=0;
	fuel_consumption=0;
	temp=0;
	preheating=0;
}

APU::APU(VESSEL3 *v)
{
	parent = v;
	active=0;
	rpm=0;
	fuel_consumption=0;
	preheating=0;
}

void APU::Preheat()
{
	if(preheating)
		preheating=false;
	else
		preheating=true;
}

bool APU::Activate(bool state)
{
	if(temp>=323) //~50 C
	{
		if(state)
			active = true;
		else
			active = false;
		return active;
	}
}

void APU::Iterate(double SimDT, PROPELLANT_HANDLE ph)
{
	if(preheating)
	{
		if(temp<=323)
			temp=temp+12.3*SimDT;
		if(temp>323)
		{
			temp=323;
			preheating=false;
		}
	}

	//fuel_consumption = 0;
	else if(active && parent->GetPropellantMass(ph)> 75)
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

	//return fuel_consumption;
}