/*
	Deepstar Airless Lander Complex Edition
	
	Visuals by 80mileshigh
	Parameters by Axel
	Code by Izack
*/

#include "DSLander.h"

DSLander::DSLander(OBJHANDLE hVessel, int fmodel)
: VESSEL3(hVessel, fmodel)
{
	RD445* eng1 = new RD445();
	APU* apu1 = new APU();
}

DSLander::~DSLander()
{

}

void DSLander::clbkSetClassCaps(FILEHANDLE cfg)
{
	SetSize(5);
	SetEmptyMass(19000);

	InitPropulsion();
	
	AddMesh("Deepstar2.1\\Deepstar2.1Lander",0);
	SetMeshVisibilityMode(0,MESHVIS_EXTERNAL);
}

void DSLander::clbkPostCreation(void)
{
	OSID = ConnectToOrbiterSoundDLL(GetHandle());

	SoundOptionOnOff(OSID,PLAYMAINTHRUST,0);
}

void DSLander::clbkPreStep(double SimT, double SimDT, double MJD)
{
	SetThrusterLevel(thr_main,eng1.Propagate(SimT, SimDT, MJD, GetThrusterLevel(thr_input)));

	apu1.Activate(true);
	S

	sprintf(oapiDebugString(),"%.3f",GetThrusterGroupLevel(thg_usr));
}

bool DSLander::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp)
{
	char c[64];

	VESSEL3::clbkDrawHUD(mode, hps, skp);

	sprintf(c,"ENGINE PARAMETERS");
	skp->Text(hps->W/16,hps->H/60*15,c,17);

	sprintf(c,"Oxygen Pump 1 pressure: %.3f", eng1.oxypump1_rpm);
	skp->Text(hps->W/16,hps->H/60*16,c,29);

	return true;
}

DLLCLBK VESSEL *ovcInit(OBJHANDLE hVessel, int fmodel)
{
	return new DSLander(hVessel, fmodel);
}

DLLCLBK void ovcExit(VESSEL *vessel)
{
	if(vessel)
		delete (DSLander*)vessel;
}