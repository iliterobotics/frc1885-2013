/********************************************************************************
 *  Project   		: FIRST Motor Controller
 *  File Name  		: PCVideoServer.h
 *  Contributors   	: ELF
 *  Creation Date 	: November 29, 2008
 *  Revision History	: Source code & revision history maintained at sourceforge.WPI.edu
 *  File Description	: C++ Axis camera control for the FIRST Vision API
 */
/*----------------------------------------------------------------------------*/
/*        Copyright (c) FIRST 2008.  All Rights Reserved.                     */
/*  Open Source Software - may be modified and shared by FRC teams. The code  */
/*  must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib. */
/*----------------------------------------------------------------------------*/

#ifndef __PCVIDEOSERVER_H__
#define __PCVIDEOSERVER_H__

#include "Task.h"
#include "bhs_CameraInf.h"

/**
 * Class the serves images to the PC.
 * StartPCFunction can only be called once.
 */
class PCVideoServer1885 : public ErrorBase
{
    
  public:
    PCVideoServer1885(bhs_CameraInf *camera);
    PCVideoServer1885(bhs_CameraInf *camera, int port);
    ~PCVideoServer1885();

    int ImageToPCServer();
    bool ShouldStopImageToPCServer();
    void StopImageToPCServer();
    unsigned int Release();

    int initVideoServer();

  private:
    int mnVideoPCPort;
    bool mbStopImageToPCServer;
    int StartPcTask();
    bool mbStarted;

    bhs_CameraInf* mpCameraServer;
};

#endif

