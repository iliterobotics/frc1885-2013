#ifndef PID_H
#define PID_H

#include "GlobalData.h"

class PID
{

public:

        PID ();
        ~PID ();
        void setConstants ( double P, double I, double D );
        double getPID ( double pv, double sp, double dt );

        
private:

        double getP () const;
        double getI ( double dt );
        double getD () const;

        double m_error, m_previousError;
        double m_integral;

        GlobalData* m_globals;
};

#endif
