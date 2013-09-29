/*
	Deepstar Airless Lander Complex Edition
	
	Visuals by 80mileshigh
	Parameters by Axel
	Code by Izack
*/

#include "DSLander.h"

void DSLander::InitPropulsion()
{
	prop_main = CreatePropellantResource(80000);
	prop_apu = CreatePropellantResource(450);
	//THROTTLE INPUT
	thr_input = CreateThruster(_V(0,0,0),_V(0,1,0),0,prop_main,0);
	CreateThrusterGroup(&thr_input,1,THGROUP_MAIN);

	//MAIN THRUSTER
	thr_main = CreateThruster(_V(0,-7,0),_V(0,1,0),1.2e6,prop_main,3700);
	thg_usr = CreateThrusterGroup(&thr_main,1,THGROUP_USER);

	PARTICLESTREAMSPEC mainex = {
		/*flags*/0,/*size*/1.5,/*rate*/60000,/*speed*/25,/*spread*/0.1,/*life*/0.25,/*growthrate*/5,/*atm slowdown*/1,
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_SQRT,0,1,
		PARTICLESTREAMSPEC::ATM_FLAT,1
	};
	AddExhaustStream(thr_main,&mainex);
}