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
	prop_rcs = CreatePropellantResource(1000);
	//THROTTLE INPUT
	thr_input = CreateThruster(_V(0,0,0),_V(0,1,0),0,prop_main,0);
	CreateThrusterGroup(&thr_input,1,THGROUP_MAIN);

	//MAIN THRUSTER
	thr_main = CreateThruster(_V(0,-7,0),_V(0,1,0),1.2e6,prop_main,3700);
	thg_usr = CreateThrusterGroup(&thr_main,1,THGROUP_USER);

	PARTICLESTREAMSPEC mainex = {
		/*flags*/0,/*size*/1.5,/*rate*/400,/*speed*/10,/*spread*/0.1,/*life*/1,/*growthrate*/2,/*atm slowdown*/1,
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_SQRT,0,1,
		PARTICLESTREAMSPEC::ATM_FLAT,1
	};
	AddExhaustStream(thr_main,&mainex);

	SetupRCS(true);
}

void DSLander::SetupRCS(bool state)
{
	if(state)
	{
		PARTICLESTREAMSPEC rcsex=
		{
			/*flags*/0,/*size*/0.2,/*rate*/600,/*speed*/10,/*spread*/0.1,/*life*/0.175,/*growthrate*/2,/*atm slowdown*/1,
			PARTICLESTREAMSPEC::EMISSIVE,
			PARTICLESTREAMSPEC::LVL_SQRT,0,1,
			PARTICLESTREAMSPEC::ATM_FLAT,1
		};

		double rcsthrust=2000;
		double rcsisp=3032;
		//Fore/back left/right exhaust direction
		thr_rcs[ 0]=CreateThruster(_V(-3.836, 0.228,-4.190),_V( 0, 0, 1),rcsthrust,prop_rcs,rcsisp); //Back left backward
		thr_rcs[ 1]=CreateThruster(_V( 3.836, 0.228,-4.190),_V( 0, 0, 1),rcsthrust,prop_rcs,rcsisp); //Back right backward
		thr_rcs[ 2]=CreateThruster(_V(-3.836,-0.446,-3.600),_V( 0, 1, 0),rcsthrust,prop_rcs,rcsisp); //Back left downward
		thr_rcs[ 3]=CreateThruster(_V( 3.836,-0.446,-3.600),_V( 0, 1, 0),rcsthrust,prop_rcs,rcsisp); //back right downward
		thr_rcs[ 4]=CreateThruster(_V(-3.836, 0.904,-3.600),_V( 0,-1, 0),rcsthrust,prop_rcs,rcsisp); //back left upward
		thr_rcs[ 5]=CreateThruster(_V( 3.836, 0.904,-3.600),_V( 0,-1, 0),rcsthrust,prop_rcs,rcsisp); //back right upward
		thr_rcs[ 6]=CreateThruster(_V(-3.836, 0.228, 4.190),_V( 0, 0,-1),rcsthrust,prop_rcs,rcsisp); //Front left forward
		thr_rcs[ 7]=CreateThruster(_V( 3.836, 0.228, 4.190),_V( 0, 0,-1),rcsthrust,prop_rcs,rcsisp); //front right forward
		thr_rcs[ 8]=CreateThruster(_V(-3.836,-0.446, 3.600),_V( 0, 1, 0),rcsthrust,prop_rcs,rcsisp); //front left downward
		thr_rcs[ 9]=CreateThruster(_V( 3.836,-0.446, 3.600),_V( 0, 1, 0),rcsthrust,prop_rcs,rcsisp); //front right downward
		thr_rcs[10]=CreateThruster(_V(-3.836, 0.904, 3.600),_V( 0,-1, 0),rcsthrust,prop_rcs,rcsisp); //front left upward
		thr_rcs[11]=CreateThruster(_V( 3.836, 0.904, 3.600),_V( 0,-1, 0),rcsthrust,prop_rcs,rcsisp); //front right upward
		thr_rcs[12]=CreateThruster(_V(-4.345, 0.228,-3.600),_V( 1, 0, 0),rcsthrust,prop_rcs,rcsisp); //back left outward
		thr_rcs[13]=CreateThruster(_V( 4.345, 0.228,-3.600),_V(-1, 0, 0),rcsthrust,prop_rcs,rcsisp); //back right outward
		thr_rcs[14]=CreateThruster(_V(-4.345, 0.228, 3.600),_V( 1, 0, 0),rcsthrust,prop_rcs,rcsisp); //front left outward
		thr_rcs[15]=CreateThruster(_V( 4.345, 0.228, 3.600),_V(-1, 0, 0),rcsthrust,prop_rcs,rcsisp); //front right outward
		//thr_rcs[16]=CreateThruster(_V( 0.000, 0.000, 0.000),_V( 0, 0, 0),rcsthrust,prop_rcs,rcsisp); //Superfluous
		//thr_rcs[17]=CreateThruster(_V( 0.000, 0.000, 0.000),_V( 0, 0, 0),rcsthrust,prop_rcs,rcsisp); //
		//thr_rcs[18]=CreateThruster(_V( 0.000, 0.000, 0.000),_V( 0, 0, 0),rcsthrust,prop_rcs,rcsisp); //
		//thr_rcs[19]=CreateThruster(_V( 0.000, 0.000, 0.000),_V( 0, 0, 0),rcsthrust,prop_rcs,rcsisp); //

		for(int i=0;i<16;i++)
		{
			AddExhaustStream(thr_rcs[i],&rcsex);
		}

		thr_group[0]=thr_rcs[4];
		thr_group[1]=thr_rcs[5];
		thr_group[2]=thr_rcs[8];
		thr_group[3]=thr_rcs[9];
		CreateThrusterGroup(thr_group,4,THGROUP_ATT_PITCHUP);

		thr_group[0]=thr_rcs[2];
		thr_group[1]=thr_rcs[3];
		thr_group[2]=thr_rcs[10];
		thr_group[3]=thr_rcs[11];
		CreateThrusterGroup(thr_group,4,THGROUP_ATT_PITCHDOWN);

		thr_group[0]=thr_rcs[15];
		thr_group[1]=thr_rcs[12];
		CreateThrusterGroup(thr_group,2,THGROUP_ATT_YAWLEFT);

		thr_group[0]=thr_rcs[13];
		thr_group[1]=thr_rcs[14];
		CreateThrusterGroup(thr_group,2,THGROUP_ATT_YAWRIGHT);

		thr_group[0]=thr_rcs[10];
		thr_group[1]=thr_rcs[9];
		CreateThrusterGroup(thr_group,2,THGROUP_ATT_BANKLEFT);

		thr_group[0]=thr_rcs[8];
		thr_group[1]=thr_rcs[11];
		CreateThrusterGroup(thr_group,2,THGROUP_ATT_BANKRIGHT);

		thr_group[0]=thr_rcs[2];
		thr_group[1]=thr_rcs[3];
		thr_group[2]=thr_rcs[8];
		thr_group[3]=thr_rcs[9];
		CreateThrusterGroup(thr_group,4,THGROUP_ATT_UP);

		thr_group[0]=thr_rcs[4];
		thr_group[1]=thr_rcs[5];
		thr_group[2]=thr_rcs[10];
		thr_group[3]=thr_rcs[11];
		CreateThrusterGroup(thr_group,4,THGROUP_ATT_DOWN);

		thr_group[0]=thr_rcs[13];
		thr_group[1]=thr_rcs[15];
		CreateThrusterGroup(thr_group,2,THGROUP_ATT_LEFT);

		thr_group[0]=thr_rcs[12];
		thr_group[1]=thr_rcs[14];
		CreateThrusterGroup(thr_group,2,THGROUP_ATT_RIGHT);

		thr_group[0]=thr_rcs[0];
		thr_group[1]=thr_rcs[1];
		CreateThrusterGroup(thr_group,2,THGROUP_ATT_FORWARD);

		thr_group[0]=thr_rcs[6];
		thr_group[1]=thr_rcs[7];
		CreateThrusterGroup(thr_group,2,THGROUP_ATT_BACK);
	}
	else
	{
		for(int i=0;i<16;i++)
			DelThruster(thr_rcs[i]);
		for(int i=0;i<4;i++)
			DelThruster(thr_group[i]);
	}
}