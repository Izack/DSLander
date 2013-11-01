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

//Define as many as you like, just make sure to increment maxhuds
#define HUDMODE_GENERAL 0
#define HUDMODE_ENGINE 1
#define HUDMODE_APU 2
#define HUDMODE_DEBUG 3
#define HUDMODE_CONSOLE 4
const int maxhuds = 4;

class DSLander: public VESSEL3
{
public:
	DSLander(OBJHANDLE hVessel, int fmodel);
	~DSLander();
	
	void DefineAnimations();
	UINT anim_door;
	double door_rot_angle;
	int doorstate; //0 = closed, 1 = open
	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkPostCreation(void);
	void clbkPreStep(double SimT, double SimDT, double mjd);
	bool clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp);
	int clbkConsumeBufferedKey(DWORD key, bool down, char *kstate);
	void clbkLoadStateEx(FILEHANDLE scn, void *status);
	void SetupRCS(bool state);

	DOCKHANDLE dock_main;
	PROPELLANT_HANDLE prop_main, prop_apu, prop_rcs;
	THRUSTER_HANDLE thr_input, thr_main, thr_rcs[16], thr_group[4]; //20 total thrusters, 4 unusable
	THGROUP_HANDLE thg_main,thg_usr;

	//OrbiterSound
	int OSID;

	char c_input[64];
private:
	RD445 eng1;
	APU apu1;

	int hudmode_l;
	int hudmode_r;
	void SwitchHUD(int side, int hud_mode); //0 left, 1 right

	void InitPropulsion();
};