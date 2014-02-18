#if 0

#ifndef TRAPEZOIDAL_H
#define TRAPEZOIDAL_H

#include "WPILib.h"
#include <iostream>
#include <fstream>
#include <Math.h>
#include "PID.h"

class Trapezoidal
{
public:
        Trapezoidal( Encoder& encoder, int displacement, Talon& talon, float velocity );
        ~Trapezoidal ();
        void setConstants( double* constants );
        void reset( int displacement, float velocity );
        const float getMotorOutput ();
        
private:
        Encoder& m_encoder;
        int m_targetDisplacement;
        Talon& m_talon;
        float m_targetVelocity;
        
        double m_constants[9];
        PID m_PID0, m_PID1, m_PID2;
        PID& m_trapPID;
        
        int m_phase;
        void changePhase ();

        double m_previousTime;
        
        
        const int getCurrentDisplacement () const;
        const float getCurrentVelocity () const;
        const double getCurrentTime () const;
        
};


#endif

#endif
