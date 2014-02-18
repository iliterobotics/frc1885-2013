// Dear Buddy, be not 2clever.
// Dear Anon, wat.
#include "bhs_SSD.h"

#include "GlobalData.h"
#include "Processing.h"

#include <map>
#include <vector>
#include <list>
#include <memory>
#include <sstream>

#include "bhs_Hopper.h"
#include "bhs_Hang.h"
#include "bhs_Launcher.h"
#include "bhs_Spatula.h"

#define DT_BEARING_TOLERANCE 5.0

namespace bhs_SSD
{
    typedef int (*ssm_processor)(GlobalData*);
    
    namespace subsystem
    {
        #define SSD_ALL_SUBSYSTEMS \
            Launcher, \
            Spatula, \
            Hopper, \
            Hang
        
        enum Subsystem
        {
            SSD_ALL_SUBSYSTEMS
        };
        
        const Subsystem ssm_arr[] =
        {
            SSD_ALL_SUBSYSTEMS
        };
        
        typedef std::map<Subsystem, bool> ssm_map;
        
        template <int Size>
        void ssm_map_init(ssm_map& mp, const Subsystem (&arr)[Size])
        {
            for(int i = 0; i < Size; ++i)
            {
                mp[arr[i]] = false;
            }
        }
        
        // Not thread safe. Do not call more than once.
        ssm_map& subsystemsMap()
        {
            // Static so we only allocate once.
            static ssm_map datmap;
            
            // Still reinit each time to guarantee all values are false.
            ssm_map_init(datmap, ssm_arr);
            
            return datmap;
        }
        
        bool resourceIsTaken(const ssm_map& mp, Subsystem ssm)
        {
            ssm_map::const_iterator iter = mp.find(ssm);
            
            // If it doesn't exist, we want to pretend it's taken.
            // Otherwise, if it's true, it's taken. False? not taken.
            return iter == mp.end() || iter->second;
        }
        
        void takeResource(ssm_map& mp, Subsystem ssm)
        {
            mp[ssm] = true;
        }
        
        template <int Size>
        bool takeAllOrNothing(ssm_map& mp, const Subsystem (&ssms)[Size])
        {
            for(int i = 0; i < Size; ++i)
            {
                if(resourceIsTaken(mp, ssms[i]))
                {
                    return false;
                }
            }
            
            for(int i = 0; i < Size; ++i)
            {
                takeResource(mp, ssms[i]);
            }
            
            return true;
        }

#if 0
        bool takeAllOrNothing(ssm_map& mp, const Subsystem ssms[], int size)
        {
            for(int i = 0; i < size; ++i)
            {
                if(resourceIsTaken(mp, ssms[i]))
                {
                    return false;
                }
            }
            
            for(int i = 0; i < size; ++i)
            {
                takeResource(mp, ssms[i]);
            }
            
            return true;
        }
        
        template <template <class, class> class T, typename A>
        bool takeAllOrNothing(ssm_map& mp, const T<Subsystem, A> ssms)
        {
            for(typename T<Subsystem, A>::const_iterator it = ssms.begin(); it != ssms.end(); ++it)
            {
                if(resourceIsTaken(mp, *it))
                {
                    return false;
                }
            }
            
            for(typename T<Subsystem, A>::const_iterator it = ssms.begin(); it != ssms.end(); ++it)
            {
                takeResource(mp, *it);
            }
            
            return true;
        }
#endif
        
        template <typename T, int Sz>
		int arrSize(T (&)[Sz])
		{
			return Sz;
		}
        
        void printAvailability(const std::string& str, ssm_map& ssmap)
        {
        	std::ostringstream res;
        	
			for(int i = 0; i < arrSize(subsystem::ssm_arr); ++i)
			{
				res << ssm_arr[i] << ": ";
				if(resourceIsTaken(ssmap, ssm_arr[i]))
				{
					res << " T ;; ";
				}
				else
				{
					res << " A ;; ";
				}
			}
			
			printf("%s %s\n", str.c_str(), res.str().c_str());
        }
    }
    
    namespace manual
    {
        int handleLauncher(GlobalData* gd)
        {  /*      	
        	printf( "bhs_ssd, manual::handleLauncher()\n");
        	//NOT checking ready to launch, only state of Manual Kick button.
        	if(gd->d_manualKick)
			{
			gd->s_launcher->launch(true);
			}
			else
			{
				//gd->s_launcher->forceStopLaunch();
			}
        	
        	gd->s_launcher->updateSelf();
        	*/
            return 0;
        }
        
        int handleHang(GlobalData *gd) {
        	return 0;
        }
        
        int handleHopper(GlobalData* gd)
        {
        	if(gd->d_hopperSwitch)
        	{
        		gd->s_hopper->putUp();
        	}
        	else
        	{
        		gd->s_hopper->putDown();
        	}
        	
            return 0;
        }
        
        int handleSpatula(GlobalData* gd)
        {
        	if (gd->d_spatulaDown)
        	{
        		gd->s_spatula->moveDown();
        	}
        	else if (gd->d_spatulaUp)
        	{
        		gd->s_spatula->moveUp();
        	} 
        	else if (gd->s_spatula->isUp() && !gd->d_spatulaUp)
        	{
        		gd->s_spatula->setIntakeOff();
        	}
        	        	
        	return 0;
        }
    }

    namespace util
    {
        bool wantToHook(const GlobalData* gd)
        {
            return !gd->d_hangSafetySwitch &&
            	(gd->d_hangExtended || gd->d_hangRetracted);
        }
        
        bool wantToShoot(const GlobalData* gd)
        {
        	return gd->d_launcherFireSequence || gd->d_manualKick;
        }

        bool wantToAutoPositionHopper(const GlobalData* gd)
        {
        	// Manual positioning is just one switch. 
        	// So auto positioning shouldn't exist
          
        	// So apparently we now always want to do this
        	return true; 
        }
        
        bool wantToAutoPositionShooter(const GlobalData* gd)
        {
        	return (!gd->d_autoEnabled && !gd->d_launcherFireSequence) || 
        			(gd->d_autoEnabled && gd->d_launcherFireSequence);
        }
      
        
        bool wantToAutoPositionSpatula(const GlobalData* gd)
        {
            return gd->d_spatulaDown || gd->d_spatulaUp;
        }
        
        template <typename T, int Sz>
        int arrSize(T (&)[Sz])
        {
            return Sz;
        }
        
        bool isReadyToLaunch()
        {
        	GlobalData* gd = GlobalData::GetInstance();
        	
        	bool launcherAdjusted = gd->s_launcher->isAtDesiredAngle() && gd->s_launcher->isAtDesiredSpeed();
        	if ( gd->d_autoEnabled )
        	{
        		bool atBearing = fabs(gd->i_gyroAngle-gd->i_launcherVisionHorizontalAngle) < DT_BEARING_TOLERANCE;
        		if ( atBearing )
        			gd->d_turnSwitch = GlobalData::NOT_TURNING;
        		launcherAdjusted = launcherAdjusted && atBearing;
        	}
        	printf( "hopper_up %d, launcher_adjusted: %d\n", gd->s_hopper->isUp(), launcherAdjusted );
        	return gd->s_hopper->isUp() && launcherAdjusted;// && !gd->s_launcher->getKickerState();
        		   
        }
        
        void ensureReadyToLaunch()
        {
        	GlobalData* gs = GlobalData::GetInstance();
        	
        	gs->s_hopper->putUp();
        	
        	// Ensure none of the driver inputs are acted on. Period.
        	gs->d_spatulaUp = true;
        	
        	gs->s_launcher->moveToSpeed( gs->i_launcherSpeed );
        	gs->s_launcher->moveToAngle(gs->i_launcherDesiredTilt, Processing::launcherPotToAngle(gs->i_launcherTiltPotentiometer));
        	printf("Angle in ensureReadyToLaunch:\t%f\n",gs->i_launcherDesiredTilt);

        	if ( gs->d_autoEnabled )
        	{
        		gs->d_turnSwitch = GlobalData::WANT_TO_TURN;
        		gs->d_turn = gs->i_launcherVisionHorizontalAngle;
        	}
        	
        }
        
        bool isReadyToLoad()
        {
        	GlobalData* gs = GlobalData::GetInstance();
        	
        	return gs->s_hopper->isDown();
        	// No spatula check; but they can only move it down (not back up)
        }
        
        bool ensureReadyToLoad()
        {
			bool ready = isReadyToLoad();
			
			GlobalData* gs = GlobalData::GetInstance();
			
			gs->s_hopper->putDown();
			
			// Guarantee no spatula-up happens.
			gs->d_spatulaUp = false;
			manual::handleSpatula(gs);
			
			return ready;
        }
    }
    
    namespace automagic
    {
        int handleLauncher(GlobalData* gd)
        {
        	// The previous line is placed above this line because
        	// 	it gives hardware more time to get into position.
        	bool readyToLaunch = util::isReadyToLaunch();
        	util::ensureReadyToLaunch();
        	
        	
        	if(readyToLaunch || gd->d_manualKick)
        	{
        		gd->s_launcher->launch(true);
        	}
        	else
        	{
        		gd->s_launcher->launch(false);
        	}
        	        	
            return 0;
        }
        
        int handlePositionForLaunch( GlobalData* gd )
        {						
        	printf( "handlePositionForLaunch()\n" );
        	
        	
        	if ( !gd->s_launcher->isAtDesiredSpeed())
        		gd->s_launcher->moveToSpeed( gd->i_launcherSpeed );
        	if ( !gd->s_launcher->isAtDesiredAngle())
        		gd->s_launcher->moveToAngle(gd->i_launcherDesiredTilt, Processing::launcherPotToAngle(gd->i_launcherTiltPotentiometer));
        	else {
        		// if within tolerance, indicate success.
        		// FIXME: may need state like other turning code
        		gd->i_launcherDesiredTilt = gd->s_launcher->getAngle();
        		gd->o_launcherTilt = 0;
        	}
        	
        	printf("Angle in handlePositionForLaunch:\t%f\t%f\n",gd->i_launcherDesiredTilt,gd->o_launcherTilt);
        	if ( gd->d_autoEnabled && gd->d_turnSwitch == GlobalData::NOT_TURNING )
        	{
        		gd->d_turnSwitch = GlobalData::WANT_TO_TURN;
        		gd->d_turn = gd->i_launcherVisionHorizontalAngle;
        	}
        	
        	return 0;
        }
        
        int handleSpatula(GlobalData* gd)
        {
        	if(util::isReadyToLoad() && gd->d_spatulaDown)
        	{
        		gd->s_spatula->moveDown();
        	}
        	else
        	{
        		gd->s_spatula->moveUp();
        	}
        	
            return 0;
        }
        
        int handleHang(GlobalData* gd)
        {
        	if(gd->d_hangExtended)
        	{
        		bool readyToLoad = util::isReadyToLoad();
        		bool launcherAtDefault = gd->s_launcher->getAngle() == bhs_Launcher::AngleDefault;
        		
        		gd->s_launcher->moveToAngle(gd->i_launcherDesiredTilt, PRESET1_ANGLE);
        		printf("Angle in handleHang:\t%f\n",gd->i_launcherDesiredTilt);

        		util::ensureReadyToLoad();
        		
        		if(readyToLoad && launcherAtDefault)
        		{
        			gd->s_hang->extend();
        		}
        	} 
        	else if(gd->d_hangRetracted)
        	{
        		bool readyToLoad = util::isReadyToLoad();
        		bool spatulaUp = gd->s_spatula->isUp();
        		
        		gd->s_spatula->moveUp();
        		//util::ensureReadyToLoad();		// FIXME
        		
        		if(readyToLoad && spatulaUp)
        		{
        			gd->s_hang->retract();
        		}
        	}
        	
            return 0;
            
        }        
    }
    
    namespace
    {
    	// Dependencies lists for things like being ready to load, etc.
    
#		define DEPS_READY_TO_LOAD \
			subsystem::Hopper, \
			subsystem::Spatula
    
        int processAutoInputs(std::vector<ssm_processor>& processor_stack, subsystem::ssm_map& avail_ssms)
        {
        	GlobalData* gd = GlobalData::GetInstance();
        	//printf("Shoot: %d\tAutoShoot: %d\n", 
        	//			util::wantToShoot(gd), 
        	//			util::wantToAutoPositionShooter(gd) );
        	
            if(util::wantToHook(gd))
            {
            	// These needed_ssms arrays are only used for takeAllOrNothing
            	// and nothing after.
                const subsystem::Subsystem needed_ssms[] = { 
                		DEPS_READY_TO_LOAD, subsystem::Hang, subsystem::Launcher };
                
                if(subsystem::takeAllOrNothing(avail_ssms, needed_ssms))
                {
                    processor_stack.push_back(&automagic::handleHang);
                } else {
                	//subsystem::printAvailability("fail ", avail_ssms);
                }
            }
            
            if(util::wantToShoot(gd))
            {
                const subsystem::Subsystem needed_ssms[] = { 
                		subsystem::Launcher, subsystem::Hopper };
                
                if(subsystem::takeAllOrNothing(avail_ssms, needed_ssms))
                {
                    processor_stack.push_back(&automagic::handleLauncher);
                }
            }
            
            if(util::wantToAutoPositionSpatula(gd))
            {            	
            	const subsystem::Subsystem needed_ssms[] = { subsystem::Spatula };

            	if(subsystem::takeAllOrNothing(avail_ssms, needed_ssms))
            	{						             
            		processor_stack.push_back(&automagic::handleSpatula);
            	}
            }  
            
            if(util::wantToAutoPositionShooter(gd))
            {
                const subsystem::Subsystem needed_ssms[] = { subsystem::Launcher };
                
                if(subsystem::takeAllOrNothing(avail_ssms, needed_ssms))
                {
                    processor_stack.push_back(&automagic::handlePositionForLaunch);	
                }
            }
            
            return 0;
        }
        
        struct ManualControl
        {
            subsystem::Subsystem system;
            ssm_processor processor;
        };
        
        int processManualInputs(std::vector<ssm_processor>& processor_stack, const subsystem::ssm_map& avail_ssms)
        {
            static std::map<subsystem::Subsystem, ssm_processor> ssm_map;
            
            if(ssm_map.empty()) 
            {
                const ManualControl mc_arr[] =
                {
                    { subsystem::Launcher, &manual::handleLauncher },
                    { subsystem::Spatula, &manual::handleSpatula },
                    { subsystem::Hang, &manual::handleHang },
                    { subsystem::Hopper, &manual::handleHopper }
                };
                
                for(int i = 0; i < util::arrSize(mc_arr); ++i)
                {
                    ssm_map.insert(std::pair<subsystem::Subsystem, ssm_processor>(mc_arr[i].system, mc_arr[i].processor));
                }
            }
            
            for(std::map<subsystem::Subsystem, ssm_processor>::const_iterator it = ssm_map.begin();
                it != ssm_map.end();
                ++it)
            {
                // If the resource has yet to be taken, add the "manual" method on the end of the stack.
                if(!subsystem::resourceIsTaken(avail_ssms, it->first))
                {
                    processor_stack.push_back(it->second);
                }
            }
            
            return 0;
        }
        
        int executeProcessors(std::vector<ssm_processor>& processor_stack)
        {
            GlobalData* gd = GlobalData::GetInstance();
            
            for(std::vector<ssm_processor>::reverse_iterator it = processor_stack.rbegin();
                it != processor_stack.rend();
                ++it)
            {
                ssm_processor proc = *it;
                
                if(proc == NULL || proc(gd))
                {
                	printf("Premature exit on procedure %p\n", proc);
                    return 1;
                }
            }
            
            return 0;
        }
    }
    
    int processInputs()
    {
        // This is static because one alloc ever and we're done. 10 is magic number.
        // I don't think we'll need more than 5 or 6 here, but 10 is only an extra 20
        // bytes (than 5), and it guarantees no reallocs. So I think it's worth it.
        static std::vector<ssm_processor> processor_stack(10);
        
        // Clear, in most implementations, just destructs everything in it in-place,
        // and does not actually shrink storage.
        processor_stack.clear();
        
        // Grab by reference is safe because subsystemsMap returns a ref to a static 
        // variable
        subsystem::ssm_map& avail_ssms = subsystem::subsystemsMap();
        
        // If this failed, fail out. Everything is reset automatically on each call.
        if(processAutoInputs(processor_stack, avail_ssms))
        {
            return 1;
        }
        
        if(processManualInputs(processor_stack, avail_ssms))
        {
            return 1;
        }
        
        return executeProcessors(processor_stack);
    }
}
