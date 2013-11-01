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
	door_rot_angle=0;
	hudmode_l=HUDMODE_DEBUG;//debug
	hudmode_r=HUDMODE_APU;
	RD445* eng1 = new RD445();
	APU* apu1 = new APU(this);
	APU* apu2 = new APU(this);
}

DSLander::~DSLander()
{

}

void DSLander::DefineAnimations()
{
	static UINT group_door[1] = {9};

	static MGROUP_ROTATE an_d
	(
		0,
		group_door,1,
		_V(0.55,0,-4.81),
		_V(0,-1,0),
		(float)(PI/1.6)
	);

	anim_door = CreateAnimation(0);
	AddAnimationComponent(anim_door,0,1,&an_d);
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

	dock_main = CreateDock(_V(0,2.697,0),_V(0,1,0),_V(0,0,1));

	AddMesh("Deepstar2.1\\Deepstar2.1Lander",0);
	SetMeshVisibilityMode(0,MESHVIS_EXTERNAL);
	DefineAnimations();
}

void DSLander::clbkLoadStateEx(FILEHANDLE scn, void *status)
{
	int apu_active,apu_preheating,apu_turbine,apu_cooling;

	char *line;
	while(oapiReadScenario_nextline(scn,line))
	{
		if(!_strnicmp(line,"LHUD",4))
			sscanf(line+4,"%d",&hudmode_l);
		if(!_strnicmp(line,"RHUD",4))
			sscanf(line+4,"%d",&hudmode_r);

		if(!_strnicmp(line,"APU_ACTIVE",10))
			sscanf(line+10,"%d",apu1.active);
		//if(!_strnicmp(line,"APU
		else
			ParseScenarioLineEx(line,status);
	}
}

void DSLander::clbkPostCreation(void)
{
	//apu1.Activate(true);

	OSID = ConnectToOrbiterSoundDLL(GetHandle());

	SoundOptionOnOff(OSID,PLAYMAINTHRUST,0);
}

void DSLander::clbkPreStep(double SimT, double SimDT, double MJD)
{
	//Animation
	if(doorstate==0)
	{
		if(door_rot_angle>0)
			door_rot_angle -= SimDT*0.2;
		SetAnimation(anim_door,door_rot_angle);
	}
	else if(doorstate==1)
	{
		if(door_rot_angle<1)
			door_rot_angle += SimDT*0.2;
		SetAnimation(anim_door,door_rot_angle);
	}
	sprintf(oapiDebugString(),"%f",door_rot_angle);

	SetThrusterLevel(thr_main,eng1.Iterate(SimT, SimDT, MJD, GetThrusterLevel(thr_input)));

	//apu1.Activate(true);
	
	apu1.Iterate(SimDT,prop_apu);
	SetPropellantMass(prop_apu,GetPropellantMass(prop_apu)-apu1.fuel_consumption);
	if(GetPropellantMass(prop_apu)<=0)
		SetPropellantMass(prop_apu,0);
	//sprintf(oapiDebugString(),"%f",apu1.dtemp);

	apu1.Iterate(SimDT,prop_apu);
	//sprintf(oapiDebugString(),"%.3f",GetThrusterGroupLevel(thg_usr));
}

void DSLander::SwitchHUD(int side, int hud_mode)
{
	switch(side)
	{
	case 0:
		hudmode_l=hud_mode;
	case 1:
		hudmode_r=hud_mode;
	}
}

bool DSLander::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp)
{
	char c[64];
	int column = hps->W/16;
	int line = hps->H/60;

	int column_l=column*1;
	int column_r=column*11;

	VESSEL3::clbkDrawHUD(mode, hps, skp);

	switch(hudmode_l)
	{
	case HUDMODE_GENERAL:
		sprintf(c,"GENERAL");
		skp->Text(column_l,line*16,c,7);
		break;
	case HUDMODE_ENGINE:
		sprintf(c,"ENGINE DISPLAY");
		skp->Text(column_l,line*16,c,6);

		//if(eng1.active);

		break;
	case HUDMODE_APU:
		sprintf(c,"APU DISPLAY");
		skp->Text(column_l,line*16,c,11);
		
		if(apu1.active)
			sprintf(c,"MAIN POWER:  ON");
		else
			sprintf(c,"MAIN POWER: OFF");
		skp->Text(column_l,line*18,c,15);

		if(apu1.preheat)
			sprintf(c,"PREHEAT:  ON");
		else
			sprintf(c,"PREHEAT: OFF");
		skp->Text(column_l,line*19,c,12);

		if(apu1.turbine)
			sprintf(c,"TURBINE:  ON");
		else
			sprintf(c,"TURBINE: OFF");
		skp->Text(column_l,line*20,c,12);

		if(apu1.cooling)
			sprintf(c,"COOLING:  ON");
		else
			sprintf(c,"COOLING: OFF");
		skp->Text(column_l,line*21,c,12);

		sprintf(c,"APU RPM: %04.0f",apu1.rpm);
		skp->Text(column_l,line*23,c,13); //%.3f takes 5 chars

		sprintf(c,"APU TEMP: %.3f",apu1.temp);
		skp->Text(column_l,line*24,c,15);
		break;
	case HUDMODE_DEBUG:
		sprintf(c,"DEBUG DISPLAY");
		skp->Text(column_l,line*16,c,13);

		//sprintf(c,"Oxygen Pump 1 pressure: %.3f", eng1.oxypump1_rpm);
		//skp->Text(column_l,line*16,c,29);

		sprintf(c,"RCS prop: %.3f",GetPropellantMass(prop_rcs));
		skp->Text(column_l,line*17,c,15);

		if(apu1.active)
			sprintf(c,"APU Status:   ACTIVE");
		else
			sprintf(c,"APU Status: INACTIVE");
		skp->Text(column_l,line*18,c,20);

		sprintf(c,"APU RPM: %04.0f",apu1.rpm);
		skp->Text(column_l,line*19,c,13); //%.3f takes 5 chars

		sprintf(c,"APU TEMP: %.3f",apu1.temp);
		skp->Text(column_l,line*20,c,15);

		if(apu1.preheat)
			sprintf(c,"PREHEAT  ON");
		else
			sprintf(c,"PREHEAT OFF");
		skp->Text(column_l,line*21,c,11);

		if(apu1.turbine)
			sprintf(c,"TURBINE  ON");
		else
			sprintf(c,"TURBINE OFF");
		skp->Text(column_l,line*22,c,11);

		if(apu1.cooling)
			sprintf(c,"COOLING  ON");
		else
			sprintf(c,"COOLING OFF");
		skp->Text(column_l,line*24,c,11);
		break;
	case HUDMODE_CONSOLE:
		sprintf(c,"COMMAND INPUT");
		skp->Text(column_l,line*16,c,13);

		sprintf(c,c_input);
		skp->Text(column_l,line*18,c,64);
	}

	switch(hudmode_r)
	{
	case HUDMODE_GENERAL:
		sprintf(c,"GENERAL");
		skp->Text(column_r,line*15,c,7);
		break;
	case HUDMODE_ENGINE:
		sprintf(c,"ENGINE");
		skp->Text(column_r,line*15,c,6);
		break;
	case HUDMODE_APU:
		sprintf(c,"APU DISPLAY");
		skp->Text(column_r,line*16,c,11);
		
		if(apu1.active)
			sprintf(c,"MAIN POWER:  ON");
		else
			sprintf(c,"MAIN POWER: OFF");
		skp->Text(column_r,line*18,c,15);

		if(apu1.preheat)
			sprintf(c,"PREHEAT:  ON");
		else
			sprintf(c,"PREHEAT: OFF");
		skp->Text(column_r,line*19,c,12);

		if(apu1.turbine)
			sprintf(c,"TURBINE:  ON");
		else
			sprintf(c,"TURBINE: OFF");
		skp->Text(column_r,line*20,c,12);

		if(apu1.cooling)
			sprintf(c,"COOLING:  ON");
		else
			sprintf(c,"COOLING: OFF");
		skp->Text(column_r,line*21,c,12);

		sprintf(c,"APU RPM: %04.0f",apu1.rpm);
		skp->Text(column_r,line*23,c,13); //%.3f takes 5 chars

		sprintf(c,"APU TEMP: %.3f",apu1.temp);
		skp->Text(column_r,line*24,c,15);
		break;
	case HUDMODE_DEBUG:
		sprintf(c,"DEBUG DISPLAY");
		skp->Text(column_r,line*15,c,13);

		sprintf(c,"Oxygen Pump 1 pressure: %.3f", eng1.oxypump1_rpm);
		skp->Text(column_r,line*16,c,29);

		if(apu1.active)
			sprintf(c,"APU Status:   ACTIVE");
		else
			sprintf(c,"APU Status: INACTIVE");
		skp->Text(column_r,line*18,c,20);

		sprintf(c,"APU RPM: %04.0f",apu1.rpm);
		skp->Text(column_r,line*19,c,13); //%.3f takes 5 chars

		sprintf(c,"APU TEMP: %.3f",apu1.temp);
		skp->Text(column_r,line*20,c,15);

		if(apu1.preheat)
			sprintf(c,"PREHEAT  ON");
		else
			sprintf(c,"PREHEAT OFF");
		skp->Text(column_r,line*21,c,11);

		if(apu1.turbine)
			sprintf(c,"TURBINE  ON");
		else
			sprintf(c,"TURBINE OFF");
		skp->Text(column_r,line*22,c,11);

		if(apu1.cooling)
			sprintf(c,"COOLING  ON");
		else
			sprintf(c,"COOLING OFF");
		skp->Text(column_r,line*24,c,11);
		break;
	}

	return true;
}

//Test
bool InterpretCommand(void *id, char *str, void *usrdata)
{
	DSLander* parent = (DSLander*)usrdata;
	sprintf(parent->c_input,str);

	return true;
}

int DSLander::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate)
{
	if(!down)
		return 0;

	if(!KEYMOD_CONTROL(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) //CLEAN
	{
		switch (key)
		{
		case OAPI_KEY_P:
			if(!apu1.preheat)
				apu1.EnablePreheating(true);
			else
				apu1.EnablePreheating(false);
			return 1;
		case OAPI_KEY_U:
			if(!apu1.active)
				apu1.Activate(true);
			else
				apu1.Activate(false);
			return 1;
		case OAPI_KEY_O:
			if(!apu1.turbine)
				apu1.EnableTurbine(true);
			else
				apu1.EnableTurbine(false);
			return 1;
		case OAPI_KEY_C:
			if(!apu1.cooling)
				apu1.cooling=true;
			else
				apu1.cooling=false;
			return 1;
		case OAPI_KEY_Y:
			oapiOpenInputBox("COMMAND",InterpretCommand,0,64,(void*)this);
			return 1;
		case OAPI_KEY_G:
			if(doorstate==0)
				doorstate=1;
			else
				doorstate=0;
		}
	}
	else if (KEYMOD_CONTROL(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) //CTRL
	{
		switch(key)
		{
		case OAPI_KEY_A: //Cycle hud
			if(hudmode_l<maxhuds)
				hudmode_l++;
			else
				hudmode_l=0;
			return 1;
		}
	}
	else if (!KEYMOD_CONTROL(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) //SHIFT
	{
		
	}
	else if (!KEYMOD_CONTROL(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_ALT(kstate)) //ALT
	{
		switch(key)
		{
		case OAPI_KEY_A: //Cycle hud
			if(hudmode_r<maxhuds)
				hudmode_r++;
			else
				hudmode_r=0;
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