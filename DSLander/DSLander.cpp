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
	APU* apu1 = new APU(this);
}

DSLander::~DSLander()
{

}

void DSLander::clbkSetClassCaps(FILEHANDLE cfg)
{
	SetSize(5);
	SetEmptyMass(19000);

	InitPropulsion();
	
	SetTouchdownPoints(_V(0,-8.359,4.935),_V(-4.926,-8.359,-4.935),_V(4.926,-8.359,-4.935));

	InitNavRadios(6);
	SetTransponderChannel(460); //131.00
	EnableTransponder(true);

	dock_main = CreateDock(_V(0,2.697,0),_V(0,1,0),_V(0,1,0));

	AddMesh("Deepstar2.1\\Deepstar2.1Lander",0);
	SetMeshVisibilityMode(0,MESHVIS_EXTERNAL);
}

void DSLander::clbkPostCreation(void)
{
	//apu1.Activate(true);

	OSID = ConnectToOrbiterSoundDLL(GetHandle());

	SoundOptionOnOff(OSID,PLAYMAINTHRUST,0);
}

void DSLander::clbkPreStep(double SimT, double SimDT, double MJD)
{
	SetThrusterLevel(thr_main,eng1.Propagate(SimT, SimDT, MJD, GetThrusterLevel(thr_input)));

	//apu1.Activate(true);
	
	SetPropellantMass(prop_apu,GetPropellantMass(prop_apu)-apu1.Propagate(SimDT,prop_apu));
	if(GetPropellantMass(prop_apu)<=0)
		SetPropellantMass(prop_apu,0);
	sprintf(oapiDebugString(),"%.3f",GetPropellantMass(prop_apu));

	if(SimT >= 20)
		apu1.Activate(true);
	apu1.Propagate(SimDT,prop_apu);
	//sprintf(oapiDebugString(),"%.3f",GetThrusterGroupLevel(thg_usr));
}

bool DSLander::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp)
{
	char c[64];
	int line = hps->H/60;

	VESSEL3::clbkDrawHUD(mode, hps, skp);

	sprintf(c,"ENGINE PARAMETERS");
	skp->Text(hps->W/16,line*15,c,17);

	sprintf(c,"Oxygen Pump 1 pressure: %.3f", eng1.oxypump1_rpm);
	skp->Text(hps->W/16,line*16,c,29);

	if(apu1.active)
		sprintf(c,"APU Status:   ACTIVE");
	else
		sprintf(c,"APU Status: INACTIVE");
	skp->Text(hps->W/16,line*18,c,20);

	sprintf(c,"APU RPM: %04.0f",apu1.rpm);
	skp->Text(hps->W/16,line*19,c,13); //%.3f takes 5 chars

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