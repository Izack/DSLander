#include "Orbitersdk.h"

class APU
{
public:
	APU();
	APU(VESSEL3 *v);
	VESSEL3 *parent;

	void Iterate(double SimDT,PROPELLANT_HANDLE ph);
	bool preheat;
	bool cooling;
	int starts;
	bool Activate(bool state);
	bool EnablePreheating(bool state);
	bool EnableTurbine(bool state);
	bool CheckReadiness(); //Check if APU is operating nominally
	bool active; //main power
	bool turbine; //running
	double rpm; //ideal 4000;
	double fuel_consumption;
	double temp; //kelvin
	double dtemp;
};