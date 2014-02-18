#include "Main.h"
#include "bhs_Spatula.h"
#include "sched.h"

GlobalData* Main::m_globals = GlobalData::GetInstance();
PID* Main::m_turnPID;

Main :: Main ()
{
	printf("Robot build %s %s\n", __TIME__, __DATE__);
	m_autonomous = new bhs_Autonomous();
	m_started = false;
	
	netDriver= new NetDriver();
	netVision= new NetVision();
	Wait(0.5);//FIXME: Note 0.5 sec at startup...
}

//Bits 0 and 1: hold the number of driver send/recv errors
#define ERRORMASK_DRIVER 0x03
//Bits 2 and 3: hold the number of vision send/recv errors
#define ERRORMASK_VISION 0x0C
//Adds 1 to change bits 0 and 1
#define INCREMENT_DRIVER 0x01
//Adds 4 to change bits 2 and 3
#define INCREMENT_VISION 0x04

//For re-establishing connection to server, bit 4
#define FLAG_RECONNECT_VISION 0x10
//For NetVision, bit 5
#define FLAG_DID_SENDSIGNAL 0x20

//For re-establishing connection to server, bit 6
#define FLAG_RECONNECT_DRIVER 0x40
#if 0
//For NetDriver, bit 7
#define FLAG_DID_TASKSPAWN 0x80
#endif

void Main::netDriverConnect(Main* const my_this, NetDriver* const my_netDriver, GlobalData* const my_m_globals)
{
	printf("\nMain: t_netDriverConnect(): started\n");
	unsigned char result = 0;
	while(true)
	{
		if(my_netDriver->open())
			result|= FLAG_RECONNECT_DRIVER;
		
		while(!(result & FLAG_RECONNECT_DRIVER))//Only checks reconnect bit
		{
			if(my_netDriver->send())
			{
				result+= INCREMENT_DRIVER;//Adds one to error count (held in bits 0 and 1)
				if((result & ERRORMASK_DRIVER) == ERRORMASK_DRIVER)//Breaks if there are 3 failed sends
					result|= FLAG_RECONNECT_DRIVER;//Signals error: need to reconnect
				//printf("\nRETRY: Main: netDriverConnect(): %d", result & ERRORMASK_DRIVER);
			}
#if 0
			else if(!(result & FLAG_DID_TASKSPAWN))//Only checks didTaskSpawn bit
			{
				result|= FLAG_DID_TASKSPAWN;//Only checks didTaskSpawn bit
				taskSpawn("t_netDriverRecv", 90, 0, 10000, (FUNCPTR)netDriverRecv(netDriver), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			}
#endif
			//Wait(m_globals->i_dsDataTransmitRate/10);//FIXME: Find magic wait time
			Wait(m_globals->i_dsDataTransmitRate);//DWB - The rate is now the actual wait time
		}
		result&= ~(ERRORMASK_DRIVER|FLAG_RECONNECT_DRIVER);//Clears bits that hold number of errors and the reconnect flag
		//perror("\nERROR: Main: driverConnect(): Reconnect");
		Wait(0.5);//FIXME: Find magic wait time
	}
}
#if 0
int Main::netDriverRecv(const NetDriver& netDriver)
{
	printf("\nMain: t_netDriverRecv(): started\n");
	while(true)
		while(!(result & FLAG_RECONNECT_DRIVER))//Only checks reconnect bit
			if(netDriver.recv())
			{
				result+= INCREMENT_DRIVER;//Adds one to error count (held in bits 0 and 1)
				if((result & ERRORMASK_DRIVER) == ERRORMASK_DRIVER)//Breaks if there are 3 failed recvs
					result|= FLAG_RECONNECT_DRIVER;//Signals error: need to reconnect
				printf("\nRETRY: Main: netDriverRecv(): %d", result & ERRORMASK_DRIVER);
			}
	return 0;
}
#endif
void Main::netVisionConnect(Main* const my_this, NetVision* const my_netVision, GlobalData* const my_m_globals)
{
	printf("\nMain: t_netVisionConnect(): started\n");
	unsigned char result = 0;
	while(true)
	{
		if(my_netVision->open())
			result|= FLAG_RECONNECT_VISION;
		
		while(!(result & FLAG_RECONNECT_VISION))//Only checks reconnect bit
		{
			if(my_m_globals->i_requestForVision)//my_m_globals->d_autoEnabled && my_m_globals->d_launcherFireSequence)
			{
				printf( "fire button pressed (result %x)\n", result);
				if(!(result & FLAG_DID_SENDSIGNAL) && my_netVision->send())//Only sends if I did not already send a signal
				{
					printf( "signal sent\n");
					result+= INCREMENT_VISION;//Adds one to error count (held in bits 2 and 3)
					if((result & ERRORMASK_VISION) == ERRORMASK_VISION)//Breaks if t1here are 3 failed sends
						result|= FLAG_RECONNECT_VISION;//Signals error: need to reconnect
					//printf("\nRETRY: Main: netDriverSend(): %d", (result & ERRORMASK_VISION)>>2);//To print 1-3 instead of 4-12
				}
				else if(my_netVision->recv())
				{
					result|= FLAG_DID_SENDSIGNAL;//Flags that I did signal vision to send values and did not yet recieve said values.
					result+= INCREMENT_VISION;//Adds one to error count (held in bits 2 and 3)
					if((result & ERRORMASK_VISION) == ERRORMASK_VISION)//Breaks if there are 3 failed recvs
						result|= FLAG_RECONNECT_VISION;//Signals error: need to reconnect
					//printf("\nRETRY: Main: netDriverRecv(): %d", (result & ERRORMASK_VISION)>>2);//To print 1-3 instead of 4-12
				}
			}
//			else
//				sched_yield();
			Wait(0.5);//FIXME: Find magic wait time // DBoyd changed to 0.5 to help DS sends.
		}
		
		result&= ~(ERRORMASK_VISION|FLAG_RECONNECT_VISION);//Clears bits that hold number of errors and the reconnect flag
		perror("\nERROR: Main: netVisionConnect(): Reconnect");

		Wait(0.5);
	}
}
Main :: ~Main ()
{
	delete m_globals;
}

void Main :: Autonomous ()
{	
	m_globals->o_matchState= 1;
	while( IsEnabled() && IsAutonomous() ){
		if(!m_globals->g_comp->Enabled())
			m_globals->g_comp->Start();
		m_globals->resetOutputs();
		m_globals->resetDS();
		//	printf( "\n\nMain::Autonomous():\t%f\n", m_globals->i_turn );
		Input::GetInputs();
		if(!m_globals->i_ds->GetDigitalIn(1) && !m_globals->i_ds->GetDigitalIn(2))
			m_autonomous->MiddleThreeDiscs();
		else if (!m_globals->i_ds->GetDigitalIn(1) && m_globals->i_ds->GetDigitalIn(2))
			m_autonomous->ThreeDiscReverse();
		else if (m_globals->i_ds->GetDigitalIn(1) && !m_globals->i_ds->GetDigitalIn(2))
			m_autonomous->ThreeDiscReverse180();
		else if (m_globals->i_ds->GetDigitalIn(1) && m_globals->i_ds->GetDigitalIn(2))
			//m_autonomous->EncoderTest(1000 + (int)m_globals->i_dsEncoderDistanceOffset);
			m_autonomous->EncoderTest(90.0);
#if 0
		else if (m_globals->i_ds->GetDigitalIn(3))
			m_autonomous->SevenAutonomous();
		else if (m_globals->i_ds->GetDigitalIn(4))
			m_autonomous->MidFieldAutonomous(false);
		else if (m_globals->i_ds->GetDigitalIn(5))
			m_autonomous->MidFieldAutonomous(true);
		else if (m_globals->i_ds->GetDigitalIn(6))
			m_autonomous->FiveAutonomous();
#endif
		
		Processing::process();
		m_globals->o_launcherTilt = 0.0;
		Output::OutputToActuators();
	}
}

void Main :: OperatorControl ()
{
	m_autonomous->m_state = bhs_Autonomous::k_spinUp;
	if (!m_started)
	{
		printf("\nMain: OperatorControl: started\n");
		taskSpawn("t_netDriverConnect", 150, VX_FP_TASK, 10000, (FUNCPTR)netDriverConnect, (int)this, (int)netDriver, (int)m_globals, 0, 0, 0, 0, 0, 0, 0);
		taskSpawn("t_netVisionConnect", 150, VX_FP_TASK, 10000, (FUNCPTR)netVisionConnect, (int)this, (int)netVision, (int)m_globals, 0, 0, 0, 0, 0, 0, 0);
		Wait(0.01);
		m_started = true;
	}
	
	m_globals->o_matchState= 2;
	

	m_globals->resetOutputs();
	m_globals->resetDS();

#if 1
	while ( IsEnabled() && IsOperatorControl() )
	{
		if(!m_globals->g_comp->Enabled())
			m_globals->g_comp->Start();

		Input::GetInputs();
		Processing::process();
		Output::OutputToActuators();
		taskDelay(sysClkRateGet() / 25);
	}
#endif
}

void Main :: Disabled ()
{
	m_globals->o_matchState= 0;
}

void Main :: Test ()
{
	m_globals->o_matchState= 3;
}

START_ROBOT_CLASS( Main );
