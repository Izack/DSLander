#include "RD445.h"

RD445::RD445()
{
	oxypump1_rpm = 0;
	oxypump1_press = 0;

	oxypump2_rpm = 0;
	oxypump2_press = 0;

	propump1_rpm = 0;
	propump1_press = 0;

	propump2_rpm = 0;
	propump2_press = 0;

	output = 0;
}

/*RD445::~RD445()
{

}*/

double RD445::Propagate(double SimT, double SimDT, double MJD, double throttle)
{
	//Placeholder
	

	if(throttle==0)
	{
		if(output>=0.01)
			output=output-0.1*SimDT;
		else
			output=0;
	}
	else if(throttle==1)
	{
		if(output<=9.99)
			output=output+0.1*SimDT;
		else
			output=1;
	}
	else if(throttle>output)
		output=output+0.1*SimDT;
	else if(throttle<output)
		output=output-0.1*SimDT;
	return output;
	
	if (s_main)
	{
		//Oxidiser system
		if (v_oxy_inlet && v_oxy_pre && v_oxy_main)
		{
			if (oxypump1_rpm < 5000)
				oxypump1_rpm = oxypump1_rpm + 17 * SimDT;
		}
	}
}