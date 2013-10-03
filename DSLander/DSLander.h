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
#define HUDMODE_GENERAL 0
#define HUDMODE_ENGINE 1
#define HUDMODE_APU 2
#define HUDMODE_DEBUG 3

class DSLander: public VESSEL3
{
public:
	DSLander(OBJHANDLE hVessel, int fmodel);
	~DSLander();
	
	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkPostCreation(void);
	void clbkPreStep(double SimT, double SimDT, double mjd);
	bool clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp);
	int clbkConsumeBufferedKey(DWORD key, bool down, char *kstate);

	DOCKHANDLE dock_main;
	PROPELLANT_HANDLE prop_main, prop_apu, prop_rcs;
	THRUSTER_HANDLE thr_input, thr_main, thr_rcs[20];
	THGROUP_HANDLE thg_main,thg_usr;

	//OrbiterSound
	int OSID;
private:
	RD445 eng1;
	APU apu1;

	int hudmode;
	void SwitchHUD(int hud_mode);

	void InitPropulsion();
};