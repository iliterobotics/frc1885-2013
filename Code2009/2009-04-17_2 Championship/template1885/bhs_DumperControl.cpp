///*
// * bhs_DumperControl.cpp
// *
// *  Created on: Feb 13, 2009
// *      Author: ITRobo
// */
//
//#include "bhs_DumperControl.h"
//#include "bhs_Constants.h"
//#include "RobotGlobalData.h"
//#include "DriverStation.h"
//
//struct DumpingPower
//{
//  static const float maDistance[];
//  static const float maPower[];
//  static const int mnNumEntries = 5;
//};
//
//const float DumpingPower::maDistance[] = {0, 24, 47, 52, 75};
//const float DumpingPower::maPower[] = {0, 0.242188, 0.5, 0.765625, 1};
//
///*
// * Default constructor
// */
//bhs_DumperControl::bhs_DumperControl()
//  /*:mcUpperLimit(bhs_Constants::cnDIGITAL_TURRET_UPPER_LIMIT),
//  mcLowerLimit(bhs_Constants::cnDIGITAL_TURRET_LOWER_LIMIT)*/
//{
//	/*mcRotateTurret.setGains(bhs_Constants::cnTURRET_ROTATION_P,
//	                        bhs_Constants::cnTURRET_ROTATION_I,
//	                        bhs_Constants::cnTURRET_ROTATION_D);*/
//}
//
///*
// * Default destructor
// */
//bhs_DumperControl::~bhs_DumperControl()
//{
//	
//}
//
///*void bhs_TurretControl::controlTurret(RobotGlobalData *apData)
//{
//  //controlAngle(apData);
//  controlDumper(apData);
//}*/
//
////void bhs_TurretControl::controlAngle(RobotGlobalData *apData)
////{
////  static DriverStation* lpDS = DriverStation::GetInstance();
////  
////  if(apData->meShooterState == RobotGlobalData::kShooterAutomatic)
////  {
////    bhs_TargetingData lcTargetingData;
////    
////    apData->getTargetingData(lcTargetingData);
////    
////    apData->mnDesiredShootingAngle = lcTargetingData.mnCurrentTargetAngle;
////  }
////  else
////  {
////    /*if(apData->meShooterState == RobotGlobalData::kShooterManualActive)
////    {*/
////      apData->mnDesiredShootingAngle = apData->mnUserDesiredShootingAngle;
////    //}
////  }
////  
////  if(apData->mbTurretLockOn)
////  {
////    apData->mnTurretPower = 0;
////    apData->mnDesiredShootingAngle = apData->mnActualShootingAngle;
////  }
////  else if(apData->mbDebugTurret)
////  {
////    apData->mnTurretPower = lpDS->GetStickAxis(4, 1);
////    
////    printf("Expected angle: %f POT Value: %f\n", apData->mnActualShootingAngle,
////           apData->mnPOTRawValue);
////  }
////  else if(apData->mnDesiredShootingAngle >= 0)
////  {
////    apData->mnTurretPower = mcRotateTurret.rotateToAngle(apData->mnActualShootingAngle, apData->mnDesiredShootingAngle).mnPower;
////    //apData->mnTurretPower = 0;
////  }
////  else  //a negative desired angle means that the automatic targeting sees nothing
////  {
////    apData->mnTurretPower = 0;
////  }
////  
////  //apData->mnTurretPower = -1*lpDS->GetStickAxis(2, 1);
////  
////  //Make sure the turret does not try to go too far
////  if (mcUpperLimit.Get() == bhs_Constants::cnDIGITAL_LIMIT_CLOSED
////      && apData->mnTurretPower < 0)
////  {
////    apData->mnTurretPower = 0;
////  }
////  else if (mcLowerLimit.Get() == bhs_Constants::cnDIGITAL_LIMIT_CLOSED
////      && apData->mnTurretPower > 0)
////  {
////    apData->mnTurretPower = 0;
////  }
////  //printf("turret power %f desired %f\n", apData->mnTurretPower, apData->mnDesiredShootingAngle);
////}
//
//void bhs_DumperControl::controlDumper(RobotGlobalData *apData)
//{
//  static DriverStation* lpDS = DriverStation::GetInstance();
//  
//  if(apData->meShooterState == RobotGlobalData::kShooterAutomatic)
//  {
//    bhs_TargetingData lcTargetingData;
//    
//    apData->getTargetingData(lcTargetingData);
//    
//    apData->mnDesiredShootingDistance = lcTargetingData.mnCurrentTargetDistance;
//  }
//  else
//  {
//    if(apData->meShooterState == RobotGlobalData::kShooterManualActive)
//    {
//      apData->mnDesiredShootingDistance = apData->mnUserDesiredShootingDistance;
//    }
//  }
//  
//  if(apData->mbDebugDumper)
//  {
//    if(lpDS->GetStickButtons(4) & 0x0001)
//    {
//      apData->mnDumperPower = -1*lpDS->GetStickAxis(4, 2);
//      printf("New Dumper speed: %f\n", apData->mnDumperPower);
//    }
//  }
//  else if(apData->mbDumperSpinBackwards)
//  {
//    apData->mnDumperPower = bhs_Constants::cnDUMPER_BACKWARDS_POWER;
//    apData->mnDesiredShootingAngle = (bhs_Constants::cnDUMPER_ANGLE_MIN +
//                                      bhs_Constants::cnDUMPER_ANGLE_MAX)/2;
//    apData->mnDesiredShootingDistance = 0;
//  }
//  /*else
//  {
//    apData->mnDumperPower = 0;
//  }*/
//  else if(apData->mnDesiredShootingDistance < 0)
//  {
//    //do nothing, maintain previous power
//    //the only time it should be negative is if the automatic targeting sees nothing
//  }
//  else if(apData->mnDesiredShootingDistance > bhs_Constants::cnDUMPER_DIST_MAX)
//  {
//    apData->mnDumperPower = 1.0;
//  }
//  else if(apData->mnDesiredShootingDistance < bhs_Constants::cnDUMPER_DIST_MIN)
//  {
//    apData->mnDumperPower = 0;
//  }
//  else
//  {
//    for(int i=0; i<DumpingPower::mnNumEntries-1; i++)
//    {
//      if (apData->mnDesiredShootingDistance == DumpingPower::maDistance[i])
//      {
//        apData->mnDumperPower =  DumpingPower::maPower[i];
//        return;
//      }
//      else if(apData->mnDesiredShootingDistance > DumpingPower::maDistance[i] &&
//              apData->mnDesiredShootingDistance < DumpingPower::maDistance[i+1])
//      {
//        double lnPowerPerInch  = ((double)DumpingPower::maDistance[i+1] - DumpingPower::maDistance[i])
//                                  /((double)DumpingPower::maPower[i+1] - DumpingPower::maPower[i]);
//        double lnPowerInRange  = (apData->mnDesiredShootingDistance-DumpingPower::maDistance[i])/lnPowerPerInch;
//        double lnFinalPower    = lnPowerInRange + DumpingPower::maPower[i];
//        apData->mnDumperPower = lnFinalPower;
//        return;
//      }
//      else if (apData->mnDesiredShootingDistance == DumpingPower::maDistance[i+1])
//      {
//        apData->mnDumperPower = DumpingPower::maPower[i+1];
//        return;
//      }
//    }
//    //if for some reason the desired distance is greater than the last entry in the
//    //array but not greater than the maximum allowed distance
//    printf("ERROR: check the dumping power arrays... %f\n", apData->mnDesiredShootingDistance);
//    apData->mnDumperPower = DumpingPower::maPower[DumpingPower::mnNumEntries-1];
//  }
//}
