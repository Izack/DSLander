/*
	Deepstar Airless Lander Complex Edition
	
	Visuals by 80mileshigh
	Parameters by Axel
	Code by Izack
*/


#include "Orbitersdk.h"
#include "OrbiterAPI.h"
#include "VesselAPI.h"
#include "RD445.h"
#include "APU.h"

#include "OrbiterSoundSDK40.h"

#define ORBITER_MODULE

class DSLander: public VESSEL3
{
public:
	DSLander(OBJHANDLE hVessel, int fmodel);
	~DSLander();
	
	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkPostCreation(void);
	void clbkPreStep(double SimT, double SimDT, double mjd);
	bool clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp);

	DOCKHANDLE dock_main;
	PROPELLANT_HANDLE prop_main, prop_apu, prop_rcs;
	THRUSTER_HANDLE thr_input, thr_main, thr_rcs[20];
	THGROUP_HANDLE thg_main,thg_usr;

	//OrbiterSound
	int OSID;
private:
	//States
	bool state_engine;
	bool state_pressurant;

	RD445 eng1;
	APU apu1;

	int hudmode;

	void InitPropulsion();
};