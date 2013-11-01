#include "APU.h"

APU::APU()
{
	parent=NULL;
	active=0;
	rpm=0;
	fuel_consumption=0;
	temp=0;
	preheat=0;
	cooling=0;
	turbine=0;
	starts=8;
}

APU::APU(VESSEL3 *v)
{
	parent = v;
	active=0;
	rpm=0;
	fuel_consumption=0;
	preheat=0;
	cooling=0;
	turbine=0;
	starts=8;
}

bool APU::EnablePreheating(bool state)
{
	if(active)
	{
		if(state)
			preheat=true;
		else
			preheat=false;
	}
	else
		preheat=false;
	return preheat;
}

bool APU::Activate(bool state)
{
	if(state)
		active = true;
	else
		active = false;
	return active;
}

bool APU::EnableTurbine(bool state)
{
	if(active && temp>=320) //~50 C
	{
		if(state)
			turbine=true;
		else
			turbine=false;
	}
	else
		turbine=false;
	return turbine;
}

bool APU::CheckReadiness()
{
	if(active && turbine && rpm>=3990)
		return true;
	else
		return false;
}

void APU::Iterate(double SimDT,PROPELLANT_HANDLE ph)
{
	dtemp = 0;
	if(active)
	{
		if(preheat)
		{
			if(temp<323)
			{
				dtemp=dtemp+12.3;
			}
		}
		else
		{

		}
		if(turbine)
		{
			if(temp>320)
			{
				if(parent->GetPropellantMass(ph)>100)
				{
					if(rpm<4000)
						rpm=rpm+73*SimDT;
					else if(rpm>4000)
						rpm=4000;
					fuel_consumption=rpm*0.000001;
					dtemp=dtemp+rpm*0.001;
				}
				else
				{
					EnableTurbine(false);
					fuel_consumption=rpm*0.000001;
					dtemp=dtemp+rpm*0.001;
				}
			}
			else
			{
				EnableTurbine(false);
				fuel_consumption=rpm*0.000001;
				dtemp=dtemp+rpm*0.001;
			}
		}
		else //If turbine disabled
		{
			if(rpm>0)
				rpm=rpm-73*SimDT;
			else if(rpm<0)
				rpm=0;
			fuel_consumption=rpm*0.000001;
			dtemp=dtemp+rpm*0.001;
		}
		if(cooling)
		{
			if(turbine)
			{
				if(temp>723)
				{
					dtemp=dtemp-5;
				}
			}
			else if(temp>323)
				dtemp=dtemp-5;
		}
		else
		{

		}

	}
	else //If main power disabled
	{
		if(preheat)
			EnablePreheating(false);
		if(turbine)
			EnableTurbine(false);
	}
	temp=temp+dtemp*SimDT;
	//rpm=rpm*SimDT;
	if(temp>820)
		EnableTurbine(false);
}