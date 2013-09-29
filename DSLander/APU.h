#include "Orbitersdk.h"

class APU
{
public:
	APU(VESSEL3 *v);
	VESSEL3 *parent;

	double Propagate(double SimDT,PROPELLANT_HANDLE ph);
	bool Activate(bool state = true);
	bool active; //on/off
	double rpm; //ideal 4000;
	double fuel_consumption;
	double voltage;
	double current;
}