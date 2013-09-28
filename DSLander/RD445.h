#include "Orbitersdk.h"
//#include "DSLander.h"

class RD445
{
public:
	RD445();
	//~RD445();

	//Functions
	double Propagate(double SimT, double SimDT, double MJD, double throttle);
	double output;

	int test;

	//Switch states
	bool s_main; //main power
	bool s_oxypump1;
	bool s_oxypump2;
	bool s_propump1;
	bool s_propump2;

	//Valves
	bool v_oxy_inlet; //Main LOX inlet
	double v_oxy_inlet_press;

	bool v_oxy_pre; //LOX preburner valve
	double v_oxy_pre_press;

	bool v_oxy_main; //Main LOX valve
	double v_oxy_main_press;

	//Parameters
	double oxypump1_rpm; //5000
	double oxypump1_press; //2.9MPa
	double oxypump2_rpm; //20000
	double oxypump2_press; //30MPa
	double propump1_rpm; //5000
	double propump1_press; //1.9MPa
	double propump2_rpm; //25000
	double propump2_press; //45MPa

	double mcc_temp; //main combustion chamber - ideal 3200 C
	double noz_temp; //Engine bell -ideal 3140
};