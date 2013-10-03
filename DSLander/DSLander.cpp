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
	APU* apu2 = new APU(this);
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
	SetThrusterLevel(thr_main,eng1.Iterate(SimT, SimDT, MJD, GetThrusterLevel(thr_input)));

	//apu1.Activate(true);
	
	apu1.Iterate(SimDT,prop_apu);
	SetPropellantMass(prop_apu,GetPropellantMass(prop_apu)-apu1.fuel_consumption);
	if(GetPropellantMass(prop_apu)<=0)
		SetPropellantMass(prop_apu,0);
	sprintf(oapiDebugString(),"%.3f %.3f",apu1.fuel_consumption,GetPropellantMass(prop_apu));

	apu1.Iterate(SimDT,prop_apu);
	//sprintf(oapiDebugString(),"%.3f",GetThrusterGroupLevel(thg_usr));
}

void DSLander::SwitchHUD(int hud_mode)
{
	hudmode=hud_mode;
}

bool DSLander::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp)
{
	char c[64];
	int line = hps->H/60;

	VESSEL3::clbkDrawHUD(mode, hps, skp);

	switch(hudmode)
	{
	case HUDMODE_DEBUG:
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

		sprintf(c,"APU TEMP: %.3f",apu1.temp);
		skp->Text(hps->W/16,line*20,c,15);
	}

	return true;
}

int DSLander::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate)
{
	if(!down)
		return 0;

	if(!KEYMOD_CONTROL(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate))
	{
		switch (key)
		{
		case OAPI_KEY_P:
			apu1.Preheat();
			return 1;
		case OAPI_KEY_U:
			apu1.Activate(true);
		}
	}
	else if (KEYMOD_CONTROL(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) //CTRL
	{
		
	}
	else if (!KEYMOD_CONTROL(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) //SHIFT
	{
		
	}
	else if (!KEYMOD_CONTROL(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_ALT(kstate)) //ALT
	{
		switch(key)
		{
		case OAPI_KEY_H: //Cycle hud
			if(hudmode<3)
				hudmode++;
			else
				hudmode=0;
			return 1;
		}
	}
	return 0;
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