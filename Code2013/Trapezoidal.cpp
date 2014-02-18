#if 0

#include "Trapezoidal.h"

extern ofstream outfile;

Trapezoidal :: Trapezoidal ( const Encoder& encoder, const int displacement, const Talon& talon, const float velocity ):
                                                        m_encoder(encoder),
                                                        m_talon(talon),
                                                        m_PID0(),
                                                        m_PID1(),
                                                        m_PID2(),
                                                        m_trapPID( m_PID0 )
{
	m_encoder.Reset();
    m_encoder.Start();
    m_targetDisplacement = displacement;
    m_targetVelocity = velocity;
       
    m_phase = 0;

    m_previousTime = getCurrentTime();
}

void Trapezoidal :: setConstants ( const double* const constants )
{
        for ( int i=9; i != 0; --i )
                m_constants[i] = constants[i];
        
        m_PID0.setConstants( m_constants[0], m_constants[1], m_constants[2] );
        m_PID1.setConstants( m_constants[3], m_constants[4], m_constants[5] );
        m_PID2.setConstants( m_constants[6], m_constants[7], m_constants[8] );  
}

void Trapezoidal :: reset ( const int displacement, const float velocity )
{
        m_targetDisplacement = displacement;
        m_targetVelocity = velocity;
        
        m_trapPID = m_PID0;
                
        m_phase = 0;
        
        m_previousTime = getCurrentTime();
}

const float Trapezoidal :: getMotorOutput ()
{
        const float currentVelocity = getCurrentVelocity();
        const double currentTime = getCurrentTime();
        const int currentDisplacement = getCurrentDisplacement();
                
        if ( m_phase == 0 && currentDisplacement >= m_targetDisplacement/4 ||
			 m_phase == 1 && currentDisplacement >= m_targetDisplacement * 0.75 )
                changePhase();

        const double pid = m_trapPID.getPID( currentVelocity, m_targetVelocity, currentTime - m_previousTime );
        
        outfile << m_phase << ", " << currentVelocity << ", " << m_targetVelocity << ", " << pid 
                        << ", " << currentDisplacement << ", ";
        
#if 1
                printf( "constants: %lf, %lf, %lf\n", m_constants[0], m_constants[1], m_constants[2] );
                printf( "phase: %d\ncurrentVelocity: %f\ntargetVelocity: %f\npid: %lf\n", m_phase, currentVelocity, m_targetVelocity, pid );
                printf( "currentTime: %lf previousTime: %lf dt: %lf\n", currentTime, m_previousTime, (currentTime - m_previousTime ) );
                printf( "currentDisplacement: %d targetDisplacement: %d\n", currentDisplacement, m_targetDisplacement);
#endif
                
        m_previousTime = currentTime;
        
        return pid;
}




void Trapezoidal :: changePhase ()
{
        if ( m_phase )
        {
				m_phase = 2;
                m_targetVelocity = 0;
                m_trapPID = m_PID2;
                printf("**CHANGE**\n");
        }
        else
        {
                m_phase = 1;
                m_trapPID = m_PID1;
                printf("**CHANGE**\n");
        }
}

const int Trapezoidal :: getCurrentDisplacement () const
{
        return m_encoder.Get();
}

const float Trapezoidal :: getCurrentVelocity () const
{
        return m_talon.Get();
}

const double Trapezoidal :: getCurrentTime () const
{
        return Timer::GetFPGATimestamp ();
        // return Timer::GetPPCTimestamp ();
}

#endif
