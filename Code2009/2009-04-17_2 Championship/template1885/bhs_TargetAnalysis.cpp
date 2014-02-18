#include "WPILib.h"
#include "bhs_TargetAnalysis.h"
#include "RobotGlobalData.h"
#include "task.h"
#include "TrackAPI.h"
#include "string.h"
#include "vxWorks.h" 

#include "AxisCamera.h" 
#include "FrcError.h"
#include "Utility.h"

#include <vector>
#include <map>
#include <math.h>

int Target_debugFlag = 0;
#define DPRINTF if(Target_debugFlag)dprintf

const float bhs_Constants::cnPAN_ANGLES[3] =
{ 115, 75, 40 };
const float bhs_Constants::cnACTUAL_ANGLES[3] =
{ 126, 90, 51 };

// for 160x120, 50 pixels = .38 %
#define MIN_PARTICLE_TO_IMAGE_PERCENT 0.25		// target is too small
#define MAX_PARTICLE_TO_IMAGE_PERCENT 20.0		// target is too close
bool operator==(const bhs_ParticleReportKey& acFirst,
                const bhs_ParticleReportKey& acSecond)
{
  bool lbReturnVal = false;
  
  if (acFirst.parArea.height == acSecond.parArea.height && acFirst.parArea.left
      == acSecond.parArea.left && acFirst.parArea.top == acSecond.parArea.top
      && acFirst.parArea.width == acSecond.parArea.width)
  {
    lbReturnVal = true;
  }
  return lbReturnVal;
}

bool operator<(const bhs_ParticleReportKey &acFirst,
               const bhs_ParticleReportKey &acSecond)
{
  double lnFirstHash = acFirst.parArea.height + acFirst.parArea.left * (1000)
      + acFirst.parArea.top * (1000000) + acFirst.parArea.width * (1000000000);
  
  double lnSecondHash = acSecond.parArea.height + acSecond.parArea.left * (1000)
      + acSecond.parArea.top * (1000000) + acSecond.parArea.width * (1000000000);
  
  bool lbReturnVal = false;
  
  if (lnFirstHash < lnSecondHash)
  {
    lbReturnVal = true;
  }
  return lbReturnVal;
}

bool operator>(const bhs_ParticleReportKey& acFirst,
              const bhs_ParticleReportKey& acSecond)
{
  double lnFirstHash = acFirst.parArea.height + acFirst.parArea.left * (1000)
      + acFirst.parArea.top * (1000000) + acFirst.parArea.width * (1000000000);
  
  double lnSecondHash = acSecond.parArea.height + acSecond.parArea.left * (1000)
      + acSecond.parArea.top * (1000000) + acSecond.parArea.width * (1000000000);
  
  bool lbReturnVal = false;
  
  if (lnFirstHash > lnSecondHash)
  {
    lbReturnVal = true;
  }
  return lbReturnVal;
}

struct HitNode
{
    int nodeIndex;
    double nodeArea;
    HitNode* next;
    HitNode(int newIndex = -1, double newArea = 0)
    {
      nodeIndex = newIndex;
      nodeArea = newArea;
      next = NULL;
    }
    void printNode()
    {
      DPRINTF(LOG_DEBUG, "~!~ index=%i ~!~ nodeArea=%g ", nodeIndex, nodeArea);
    }
};
struct HitList
{
    HitNode *head;
    HitList()
    {
      head = NULL;
    }
    ~HitList()
    {
      if (head == NULL)
        return;
      if (head->next == NULL)
      {
        delete head;
        return;
      }
      HitNode *toDelete = head;
      HitNode *temp = head->next;
      while (temp != NULL)
      {
        delete toDelete;
        toDelete = temp;
        temp = temp->next;
      }
      delete toDelete;
      return;
    }
    int AddNode(int index, double area)
    {
      if (head == NULL)
      {
        head = new HitNode(index, area);
      }
      else
      {
        //create the new node now
        HitNode *insert = new HitNode(index,area);
        //check to see if this should go before head
        if (area > head->nodeArea)
        {
          insert->next = head;
          head = insert;
          return 1;
        }
        //a temporary pointer to keep position
        HitNode *current = head;
        //loop until we find where we need to insert new node
        while (current->nodeArea > area)
        {
          //reached end of list
          if (current->next == NULL)
          {
            current->next = insert;
            return 1;
          }
          //found position to insert the new node
          else if (current->next->nodeArea < area)
          {
            insert->next = current->next;
            current->next = insert;
            return 1;
          }
          //did not find position, advance the pointer
          else
          {
            current = current->next;
          }
        }
      }
      return 0;
    }
    void debugDump()
    {
      if (head == NULL)
      {
        DPRINTF(LOG_DEBUG, "~~no entries in HitList");
      }
      else
      {
        DPRINTF(LOG_DEBUG, "~~HitList:");
        HitNode *toPrint = head;
        //HitNode *temp = head;
        while (toPrint != NULL)
        {
          toPrint->printNode();
          toPrint = toPrint->next;
        }
        DPRINTF(LOG_DEBUG, "~~end of HitList");
      }
    }
};

bhs_TargetAnalysis::bhs_TargetAnalysis()
{
  mpCamera = NULL;
  /* image data for tracking - override default parameters if needed
   * recommend making PINK the first color because GREEN is more 
   * subsceptible to hue variations due to lighting type so may
   * result in false positives */

  // PINK
  sprintf(mtd1.name, "PINK");
  mtd1.hue.minValue = 220;
  mtd1.hue.maxValue = 255;
  mtd1.saturation.minValue = 30;  //previous value: 75
  mtd1.saturation.maxValue = 255;
  mtd1.luminance.minValue = 85;
  mtd1.luminance.maxValue = 255;
  // GREEN
  sprintf(mtd2.name, "GREEN");
  mtd2.hue.minValue = 55;
  mtd2.hue.maxValue = 125;
  mtd2.saturation.minValue = 35;  //previous value: 58
  mtd2.saturation.maxValue = 255;
  mtd2.luminance.minValue = 92;
  mtd2.luminance.maxValue = 255;
  
  meServoAngle = kServoAngleFirst;
  mbPanForward = true;
  meServoState = kSetPanAngle;
  mnPreviousPan = 0;
  meLastAngle = kServoAngleFirst;
}
bhs_TargetAnalysis::~bhs_TargetAnalysis()
{
}

void bhs_TargetAnalysis::CameraTrackingTask(int anThis)
{
  bhs_TargetAnalysis* lpThis = (bhs_TargetAnalysis*)anThis;
  
  lpThis->getDecodedImage();
}

void bhs_TargetAnalysis::configureCamera(bhs_CameraInf *apCamera,
                                         RobotGlobalData *apData,
                                         DriverStation *apDS)
{
  mpCamera = apCamera;
  mpData = apData;
  mpDS = apDS;
}

void bhs_TargetAnalysis::getDecodedImage()
{
  char funcName[]="getDecodedImage";
  double lnTimeStamp;
  Image* lpCurrentImage;
  //Image* lpAnotherImage;
  if (mpCamera==NULL)
  {
    return;
  }
  int cnt = 0;
  while (1)
  {
    semTake(mcTimingSem, WAIT_FOREVER);
    switch (meServoState)
    {
      case kSetPanAngle:
        getNextServoAngle();
        meServoState = kGoToAngle;
        break;
      case kGoToAngle:
        if (servoInPosition())
        {
          meServoState = kThrowAwayFrame;
        }
        break;
      case kThrowAwayFrame:
        bhs_CameraTurretData lcCameraTurretData;
        mpData->getCameraTurretData(lcCameraTurretData);
        if(mnPreviousPan != lcCameraTurretData.mnCameraTurretPanDegree)
        {
          //Throw away some frames if the camera moved, since it is probably
          //initially blurry
          mpCamera->ThrowAwayFrames(7);
          //printf("killed frames\n");
        }
        mnPreviousPan = lcCameraTurretData.mnCameraTurretPanDegree;
        meLastAngle = meServoAngle;
        meServoState = kAnalyzeFrame;
        break;
      case kAnalyzeFrame:
        if (mpCamera->GetDecodedImageBlocking(lpCurrentImage, &lnTimeStamp) == OK)
        {
          getNextServoAngle();
          /*lpAnotherImage = frcCreateImage(IMAQ_IMAGE_HSL);
           frcCopyImage(lpAnotherImage, lpCurrentImage);*/
          bhs_TargetReport lsTargets = FindTwoColors(mtd1, mtd2, ABOVE | BELOW, 5,
                                                     lpCurrentImage);
          
          DistanceData lsDistanceAngleData;
          
          float lnCurrentTargetDistance, lnCurrentTargetAngle;
          
          for (int i=0; i<bhs_Constants::cnMAX_TARGET_REPORTS; i++)
          {
            if (i<lsTargets.numberOfValidTargetReports)
            {
              bhs_ProcessedTarget lcTempTarget;
              lsDistanceAngleData
                  = maDistanceCalc.calcDistance(&lsTargets.mnReport[i].mnParticleReport,
                                                bhs_Constants::cnACTUAL_ANGLES[meLastAngle],
                                                bhs_Constants::cnACTUAL_FIXED_TILT_ANGLE);
              lcTempTarget.mnAngle=lsDistanceAngleData.mnAngle;
              lcTempTarget.mnDistance=lsDistanceAngleData.mnDistance;
              lcTempTarget.meTeam=lsTargets.mnReport[i].meTeamColor;
              lcTempTarget.mbValid=true;
              
              if(meLastAngle == kServoAngleCenter)
              {
                if(mbPanForward)
                {
                  msForwardSweepTargets.insert(lcTempTarget);
                }
                else
                {
                  msBackwardSweepTargets.insert(lcTempTarget);
                }
              }
              else
              {
                msForwardSweepTargets.insert(lcTempTarget);
                msBackwardSweepTargets.insert(lcTempTarget);
              }
              /*printf("angle: %f distance %f\n", lsDistanceAngleData.mnAngle,
                     lsDistanceAngleData.mnDistance);*/
            }
          }
          
          //printf("analyzed stuff\n");
          meServoState = kGoToAngle;
          frcDispose(funcName, lpCurrentImage, NULL);
          
          bhs_TargetingData lcTargetingData;
          
          mpData->getTargetingData(lcTargetingData);
          
          std::set<bhs_ProcessedTarget> *lpSweepTargetOutput;
          if (meLastAngle == kServoAngleFirst)
          {
            lpSweepTargetOutput = &msBackwardSweepTargets;
          }
          else if(meLastAngle == kServoAngleLast)
          {
            lpSweepTargetOutput = &msForwardSweepTargets;
          }
          else if(meLastAngle == kServoAngleCenter && mbPanForward &&
                  !lcTargetingData.mbCameraZone3Active)
          {
            lpSweepTargetOutput = &msForwardSweepTargets;
          }
          else if(meLastAngle == kServoAngleCenter && !mbPanForward &&
                  !lcTargetingData.mbCameraZone1Active)
          {
            lpSweepTargetOutput = &msBackwardSweepTargets;
          }
          else
          {
            continue;
          }
          
          float lnShortestDistance = 100000000;
          float lnShortestDistanceAngle = -1;
          
          std::set<bhs_ProcessedTarget>::const_iterator pos;
          
          int j=1;
          for (pos = lpSweepTargetOutput->begin(); pos != lpSweepTargetOutput->end(); pos++)
          {
            if (pos->mbValid &&
                (mpDS->GetAlliance() == DriverStation::kInvalid ||
                    (pos->meTeam != mpDS->GetAlliance() && pos->meTeam != DriverStation::kInvalid)) &&
                pos->mnDistance < lnShortestDistance &&
                pos->mnDistance >= 0)
            {
              lnShortestDistance = pos->mnDistance;
              lnShortestDistanceAngle = pos->mnAngle;
            }
            printf("    Target %d: dist: %f angle: %f\n", j, pos->mnDistance, pos->mnAngle);
            j++;
          }
          
          if (lnShortestDistanceAngle != -1)
          {
            lnCurrentTargetAngle = lnShortestDistanceAngle;
            lnCurrentTargetDistance = lnShortestDistance;
          }
          else
          {
            lnCurrentTargetAngle = -1;
            lnCurrentTargetDistance = -1;
          }
          
          lcTargetingData.mmTargets = *lpSweepTargetOutput;
          lcTargetingData.mnCurrentTargetDistance = lnCurrentTargetDistance;
          lcTargetingData.mnCurrentTargetAngle = lnCurrentTargetAngle;
          
          mpData->setTargetingData(lcTargetingData);
          
          lpSweepTargetOutput->clear();

          /*printf("bhs_TargetAnalysis %d image = %p targets = %d\n", cnt, lpCurrentImage,
           lsTargets.numberOfValidTargetReports);*/
          
          /*for (int i=0; i<lsTargets.numberOfValidTargetReports; i++)
           {
           Rect tmpRec = lsTargets.mnReport[i].mnParticleReport.boundingRect;
           printf("    Target %d: top: %d left: %d height: %d width: %d\n", i+1,
           tmpRec.top, tmpRec.left, tmpRec.height, tmpRec.width);
           }*/
          //frcDispose(funcName,lpAnotherImage,NULL);
          cnt++;
        }
        else
        {
          printf(" bhs_TargetAnalysis bad image %d\n", cnt);
        }
        break;
    }
  }
}

void bhs_TargetAnalysis::getNextServoAngle()
{
  float lnCameraTurretPanDegree = mnPreviousPan;
  
  bhs_TargetingData lcTargetingData;
  mpData->getTargetingData(lcTargetingData);
  
  switch (meServoAngle)
  {
    case kServoAngleFirst:
      if(lcTargetingData.mbCameraZone2Active)
      {
        lnCameraTurretPanDegree = bhs_Constants::cnPAN_ANGLES[kServoAngleCenter];
        meServoAngle = kServoAngleCenter;
        mbPanForward = true;
      }
      else if(lcTargetingData.mbCameraZone3Active)
      {
        lnCameraTurretPanDegree = bhs_Constants::cnPAN_ANGLES[kServoAngleLast];
        meServoAngle = kServoAngleLast;
        mbPanForward = true;
      }
      else
      {
        mbPanForward = false;
      }
      break;
    case kServoAngleCenter:
      if (mbPanForward)
      {
        if(lcTargetingData.mbCameraZone3Active)
        {
          lnCameraTurretPanDegree = bhs_Constants::cnPAN_ANGLES[kServoAngleLast];
          meServoAngle = kServoAngleLast;
        }
        else if(lcTargetingData.mbCameraZone1Active)
        {
          lnCameraTurretPanDegree = bhs_Constants::cnPAN_ANGLES[kServoAngleFirst];
          meServoAngle = kServoAngleFirst;
          mbPanForward = false;
        }
      }
      else
      {
        if(lcTargetingData.mbCameraZone1Active)
        {
          lnCameraTurretPanDegree = bhs_Constants::cnPAN_ANGLES[kServoAngleFirst];
          meServoAngle = kServoAngleFirst;
        }
        else if(lcTargetingData.mbCameraZone3Active)
        {
          lnCameraTurretPanDegree = bhs_Constants::cnPAN_ANGLES[kServoAngleLast];
          meServoAngle = kServoAngleLast;
          mbPanForward = true;
        }
      }
      break;
    case kServoAngleLast:
      if(lcTargetingData.mbCameraZone2Active)
      {
        lnCameraTurretPanDegree = bhs_Constants::cnPAN_ANGLES[kServoAngleCenter];
        meServoAngle = kServoAngleCenter;
        mbPanForward = false;
      }
      else if(lcTargetingData.mbCameraZone1Active)
      {
        lnCameraTurretPanDegree = bhs_Constants::cnPAN_ANGLES[kServoAngleFirst];
        meServoAngle = kServoAngleFirst;
        mbPanForward = false;
      }
      else
      {
        mbPanForward = true;
      }
      break;
  }
  
  mcCameraTurret.setPanAngle(mpData, lnCameraTurretPanDegree);
  mcCameraTurret.setTiltAngle(mpData, bhs_Constants::cnFIXED_TILT_ANGLE);
  //printf("setting pan angle %f\n", lnCameraTurretPanDegree);
}

bool bhs_TargetAnalysis::servoInPosition()
{
  bhs_CameraTurretData lcCameraTurretData;
  
  mpData->getCameraTurretData(lcCameraTurretData);
  
  if (fabs(lcCameraTurretData.mnCameraTurretPanDegree - lcCameraTurretData.mnCameraTurretActualPanDegree) < 2)
  {
    return true;
  }
  //printf("waiting...\n");
  return false;
}

int bhs_TargetAnalysis::initTargetAnalysis(SEM_ID acTimingSem)
{
  mcTimingSem = acTimingSem;
  int m_taskID = 0;
  //TODO: Define priority, stack size?
  m_taskID = taskSpawn("getDecodedImage", // name
                        101, // priority
                       VX_FP_TASK, // options
                        64000, // stack size
                        (FUNCPTR)CameraTrackingTask, // function to start
                        (int)this, 0, 0, 0, 0, 0, 0, 0, 0, 0);// additional unused parameters
  return m_taskID;
}

//position = ABOVE or BELOW
//I think numberHits = 3
bhs_TargetReport bhs_TargetAnalysis::FindTwoColors(TrackingThreshold &td1,
                                                   TrackingThreshold &td2,
                                                   int position, int numberHits,
                                                   Image* imageToPass)
{
  //printf("FindTwoColors Begin\n");
  bhs_TargetReport lsReportReturn;
  //char funcName[]="FindTwoColors";
  int success;
  
  // create list of color hits
  ImageHits color1Hits; // define return object
  memset(&color1Hits, 0, sizeof(ImageHits));
  
  // search for the largest particles of the first color
  // get several particles in case the first one turns out not to be a target
  success = FindColorHits(IMAQ_HSL, &td1.hue, &td1.saturation, &td1.luminance,
                           &color1Hits, IMAQ_NO_RECT, numberHits, imageToPass);
  if (!success)
    return lsReportReturn;
  //PrintReport(&color1Hits);
  //printf(">>>>>>>>>>>>>>> found first color: %s particles: %i\n", td1.name, color1Hits.numberOfHits);

  ImageHits color2Hits; // define return object
  memset(&color2Hits, 0, sizeof(ImageHits));
  success = FindColorHits(IMAQ_HSL, &td2.hue, &td2.saturation, &td2.luminance,
                           &color2Hits, IMAQ_NO_RECT, numberHits, imageToPass);
  if (!success)
    return lsReportReturn;
  //PrintReport(&color2Hits);
  //printf(">>>>>>>>>>>>>>> found second color: %s particles: %i\n", td2.name, color2Hits.numberOfHits);

  //Maps are used to eliminate duplicate reports.  Duplicate reports have the
  //same rectangular area.
  std::map <bhs_ParticleReportKey, ParticleAnalysisReport*> lmFirstTrackReportRemoval;
  std::map <bhs_ParticleReportKey, ParticleAnalysisReport*>
      lmSecondTrackReportRemoval;
  
  for (int i=0; i<color1Hits.numberOfHits; i++)
  {
    bhs_ParticleReportKey tmpKey(color1Hits.pars[i].boundingRect);
    lmFirstTrackReportRemoval[tmpKey] = &color1Hits.pars[i];
  }
  for (int i=0; i<color2Hits.numberOfHits; i++)
  {
    bhs_ParticleReportKey tmpKey(color2Hits.pars[i].boundingRect);
    lmSecondTrackReportRemoval[tmpKey] = &color2Hits.pars[i];
  }
  
  //Vectors are used to eliminate matched reports from color2hits
  std::vector <ParticleAnalysisReport*> lvFirstTrackReports;
  std::vector <ParticleAnalysisReport*> lvSecondTrackReports;
  
  std::map<bhs_ParticleReportKey, ParticleAnalysisReport*>::const_iterator iter;
  
  for (iter=lmFirstTrackReportRemoval.begin(); iter!=lmFirstTrackReportRemoval.end(); ++iter)
  {
    lvFirstTrackReports.push_back(iter->second);
  }
  for (iter=lmSecondTrackReportRemoval.begin(); iter
      !=lmSecondTrackReportRemoval.end(); ++iter)
  {
    lvSecondTrackReports.push_back(iter->second);
  }
  
  ParticleAnalysisReport *firstTrackReport;
  ParticleAnalysisReport *secondTrackReport;
  
  // check all 1st color begining with the largest
  double sizeInImage;
  for (UINT i=0; i<lvFirstTrackReports.size(); ++i)
  {
    firstTrackReport = lvFirstTrackReports.at(i);
    sizeInImage = firstTrackReport->particleToImagePercent;
    if (sizeInImage < FRC_COLOR_TO_IMAGE_PERCENT)
    {
      //printf("First target too small: %g\n", sizeInImage);
      continue;
    }
    // check all 2nd color begining with the largest
    //printf("BEGIN CHECK\n");
    for (UINT32 j=0; j<lvSecondTrackReports.size(); ++j)
    {
      secondTrackReport = lvSecondTrackReports.at(j);
      sizeInImage = secondTrackReport->particleToImagePercent;
      if (secondTrackReport->particleToImagePercent < FRC_COLOR_TO_IMAGE_PERCENT)
      {
        //printf("Second target too small: %g\n", sizeInImage);
        continue;
      }
      // found both colors. check position.

      if (position & ABOVE) //is second color above first color?
      {
        if (secondTrackReport->center_mass_y < firstTrackReport->center_mass_y)
        {
          // add in the SizesRelative call if needed -
          // so far it does not seem necessary
          if (Aligned(firstTrackReport->center_mass_x,
                      secondTrackReport->center_mass_x,
                      firstTrackReport->boundingRect.width,
                      secondTrackReport->boundingRect.width)
              && Adjacent(firstTrackReport->boundingRect.top,
                           (secondTrackReport->boundingRect.top
                              + secondTrackReport->boundingRect.height))
              && SizesRelative(firstTrackReport->particleArea,
                               secondTrackReport->particleArea) )
          {
            
            //The second track report is on the bottom
            memcpy(
                    &lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].mnParticleReport,
                   secondTrackReport,
                    sizeof(lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].mnParticleReport));
            
            //The first track report is on top and pink, so the team color is red
            lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].meTeamColor
                = DriverStation::kRed;
            
            lvSecondTrackReports.erase(lvSecondTrackReports.begin()+j);
            
            lsReportReturn.numberOfValidTargetReports++;
            if (lsReportReturn.numberOfValidTargetReports
                >=bhs_Constants::cnMAX_TARGET_REPORTS)
              return lsReportReturn;
            
            /*printf("Above Triggered, Target Number: %d\n",
             lsReportReturn.numberOfValidTargetReports);*/

            //There should only be one match between the first block and all of the
            //second ones.
            break;
          }
        }
      }
      if (position & BELOW) //is second color below first color?
      {
        if (secondTrackReport->center_mass_y > firstTrackReport->center_mass_y)
        {
          if (Aligned(firstTrackReport->center_mass_x,
                      secondTrackReport->center_mass_x,
                      firstTrackReport->boundingRect.width,
                      secondTrackReport->boundingRect.width)
              && Adjacent( (firstTrackReport->boundingRect.top
                  + firstTrackReport->boundingRect.height),
                          secondTrackReport->boundingRect.top) )
          {
            
            //The first track report is on the bottom
            memcpy(
                    &lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].mnParticleReport,
                   firstTrackReport,
                    sizeof(lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].mnParticleReport));
            
            //The second track report is on top and green, so the team color is blue
            lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].meTeamColor
                = DriverStation::kBlue;
            
            lvSecondTrackReports.erase(lvSecondTrackReports.begin()+j);
            
            lsReportReturn.numberOfValidTargetReports++;
            if (lsReportReturn.numberOfValidTargetReports
                >=bhs_Constants::cnMAX_TARGET_REPORTS)
              return lsReportReturn;
            /*printf("Below Triggered, Target Number: %d\n",
             lsReportReturn.numberOfValidTargetReports);*/

            //There should only be one match between the first block and all of the
            //second ones.
            break;
          }
        }
      }
      if (position & RIGHT) //is second color to the right of first color?
      {
        if (secondTrackReport->center_mass_x > firstTrackReport->center_mass_x)
        {
          if (Aligned(firstTrackReport->center_mass_y,
                      secondTrackReport->center_mass_y,
                      firstTrackReport->boundingRect.width,
                      secondTrackReport->boundingRect.width)
              && Adjacent( (firstTrackReport->boundingRect.left
                  + secondTrackReport->boundingRect.width),
                          secondTrackReport->boundingRect.left) )
          {
            //memcpy(trackReport, firstTrackReport, sizeof(ParticleAnalysisReport));
            //return true;
            memcpy(
                    &lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].mnParticleReport,
                   secondTrackReport,
                    sizeof(lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].mnParticleReport));
            
            //All sideways targets are on unknown teams
            lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].meTeamColor
                = DriverStation::kInvalid;
            
            lvSecondTrackReports.erase(lvSecondTrackReports.begin()+j);
            
            lsReportReturn.numberOfValidTargetReports++;
            if (lsReportReturn.numberOfValidTargetReports
                >=bhs_Constants::cnMAX_TARGET_REPORTS)
              return lsReportReturn;
            /*printf("Right Triggered, Target Number: %d\n",
             lsReportReturn.numberOfValidTargetReports);*/

            //There should only be one match between the first block and all of the
            //second ones.
            break;
          }
        }
      }
      if (position & LEFT) //is second color to the left of first color?
      {
        if (secondTrackReport->center_mass_x < firstTrackReport->center_mass_x)
        {
          if (Aligned(firstTrackReport->center_mass_y,
                      secondTrackReport->center_mass_y,
                      firstTrackReport->boundingRect.width,
                      secondTrackReport->boundingRect.width)
              && Adjacent(firstTrackReport->boundingRect.left,
                           (secondTrackReport->boundingRect.left
                              + secondTrackReport->boundingRect.width)) )
          {
            //memcpy(trackReport, firstTrackReport, sizeof(ParticleAnalysisReport));
            //return true;
            memcpy(
                    &lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].mnParticleReport,
                   firstTrackReport,
                    sizeof(lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].mnParticleReport));
            
            //All sideways targets are on unknown teams
            lsReportReturn.mnReport[lsReportReturn.numberOfValidTargetReports].meTeamColor
                = DriverStation::kInvalid;
            
            lvSecondTrackReports.erase(lvSecondTrackReports.begin()+j);
            
            lsReportReturn.numberOfValidTargetReports++;
            if (lsReportReturn.numberOfValidTargetReports
                >=bhs_Constants::cnMAX_TARGET_REPORTS)
              return lsReportReturn;
            /*printf("Left Triggered, Target Number: %d\n",
             lsReportReturn.numberOfValidTargetReports);*/

            //There should only be one match between the first block and all of the
            //second ones.
            break;
          }
        }
      }
    }
  }
  
  return lsReportReturn;
}

/**
 * @brief Search for color particles. Supports IMAQ_IMAGE_HSL and IMAQ_IMAGE_RGB. 
 * @param mode Color mode, either IMAQ_HSL or IMAQ_RGB
 * @param plane1Range The range for the first plane (hue or red)
 * @param plane2Range The range for the second plane (saturation or green)
 * @param plane3Range The range for the third plane (luminance or blue)
 * @param hitReport Contains array of numberHits largest targets
 * @param rect Rectangle to confine search to
 * @param numberHitsRequested maximum number to return
 * @return 0 = error
 */
int bhs_TargetAnalysis::FindColorHits(ColorMode mode, const Range* plane1Range,
                                      const Range* plane2Range,
                                      const Range* plane3Range,
                                      ImageHits* hitReport, Rect rect,
                                      int numberHitsRequested, Image* imageToPass)
{
  char funcName[]="FindColorHits";
  int errorCode = 0;
  int success = 0;
  /* fixed limit to number of hits processed
   * larger # takes more processing time but can eliminate failure due to 
   * interference for instance, a red or green area in the stands or 
   * closer targets of the wrong alliance
   */
  if (numberHitsRequested> FRC_MAX_HITS)
    numberHitsRequested = FRC_MAX_HITS;
  
  /* create an image object */
  Image* cameraImage = frcCreateImage(IMAQ_IMAGE_U8);
  if (!cameraImage)
  {
    printf("ERROR: Failed to create temporary image in FindColorHits\n");
    return success;
  }
  
  /* get image from camera - if the camera has not finished initializing,
   * this will fail */
  if (imageToPass!=NULL)
    success = 1;
  if (!success)
  {
    printf("No camera Image available Error = %i %s", errorCode,
           GetVisionErrorText(errorCode));
    frcDispose(cameraImage);
    imaqSetError(errorCode, funcName); //reset error code for the caller	
    return success;
  }
  
  /* Color threshold the image */
  success = frcColorThreshold(cameraImage, imageToPass, mode, plane1Range,
                              plane2Range, plane3Range);
  if ( !success)
  {
    errorCode = GetLastVisionError();
    printf("Error = %i  %s ", errorCode, GetVisionErrorText(errorCode));
    frcDispose(funcName, cameraImage, NULL);
    return success;
  }
  /* get largest particles that meet criteria */
  /* GetLargestParticles fills out only the indices in the hitReport */
  success = GetLargestParticles(cameraImage, hitReport, rect, numberHitsRequested);
  if ( !success)
  {
    errorCode = GetLastVisionError();
    printf("Error after GetLargestParticles=%i  %s ", errorCode,
           GetVisionErrorText(errorCode));
    frcDispose(funcName, cameraImage, NULL);
    imaqSetError(ERR_COLOR_NOT_FOUND, funcName);
    return success;
  }
  if (hitReport->numberOfHits==0)
  {
    imaqSetError(ERR_COLOR_NOT_FOUND, funcName);
    frcDispose(funcName, cameraImage, NULL);
    return 0;
  }
  //DPRINTF(LOG_INFO, "number hits found = %i\n", hitReport->numberOfHits);	

  /* Particles were found --
   * Fill in report information for the largest particles 
   */
  double imageTime = GetTime();
  for (int i=0; i<hitReport->numberOfHits; ++i)
  {
    hitReport->pars[i].imageTimestamp = imageTime;
    success = frcParticleAnalysis(cameraImage, hitReport->indices[i],
                                   &hitReport->pars[i]);
  }
  /* clean up */
  frcDispose(funcName, cameraImage, NULL);
  
  //printf("FindColorHits successful\n");

  return success;
}

/**
 * @brief Check if two pixels are vertically or horizontally aligned 
 * @param center1 Center pixel of first partical (x for vertical alignment, y for horizontal)
 * @param center2 Center pixel of second partical (x for vertical alignment, y for horizontal)
 * @param dimension1 of first partical (width for vertical alignment, height for horizontal)
 * @param dimension2 of second partical (width for vertical alignment, height for horizontal)
 * @return true if aligned vertically or horizontally
 */
bool bhs_TargetAnalysis::Aligned(int center1, int center2, int dimension1,
                                 int dimension2)
{
  //char funcName[]="Aligned";
  float averageWidth = (dimension1 + dimension2) / 2.0;
  //scale down width
  averageWidth *= FRC_ALIGNMENT_SCALE;
  int centerDiff = abs(center1 - center2);
  if (centerDiff < averageWidth)
  {
    //DPRINTF(LOG_DEBUG,"Aligned Centers: %i %i Widths: %i  %i", center1, center2, dimension1, dimension2);
    return true;
  }
  //dimensions (widths or heights) should be nearly the same. If they are 
  //different, most likely there is glare or incorrect color specification
  //DPRINTF(LOG_DEBUG,"Not Aligned Centers: %i %i Widths: %i  %i", center1, center2, dimension1, dimension2);
  return false;
}
/**
 * @brief Check if two values are approximately adjacent
 * @param value1 Center pixel of first partical (x for vertical alignment, y for horizontal)
 * @param value2 Center pixel of second partical (x for vertical alignment, y for horizontal)
 * @return true if particles are within a 5 pixel buffer
 */
bool bhs_TargetAnalysis::Adjacent(int value1, int value2)
{
  //char funcName[]="Adjacent";
  if (abs(value1 - value2) <= FRC_MAX_IMAGE_SEPARATION)
    return true;
  //DPRINTF(LOG_DEBUG,"Not Adjacent  %i  %i", value1, value2);
  return false;
}

/**
 * @brief Check if two values are approximately the same size, 
 * Make the check loose to allow for differences in light & color recognition
 * @param area1 First area
 * @param area2 Second area
 * @return true if particles size differences are small enough
 */
bool bhs_TargetAnalysis::SizesRelative(double area1, double area2)
{
  //char funcName[]="SizesRelative";
  if ( (area2 < (area1 * FRC_SIZE_FACTOR)) && (area1 < (area2 * FRC_SIZE_FACTOR)))
  {
    return true;
  }
  DPRINTF(LOG_DEBUG,"Targets too different in size %g  %g", area1, area2);
  return false;
}

/**
 * @brief Find the largest particles that meet a criteria 
 * @param binaryImage Image to inspect
 * @param hitReport structure containing arry of hits - first hit is largest 
 * @param rect area to search
 * @param numParticles Number of particles in array
 * @return 0 = error
 */
int bhs_TargetAnalysis::GetLargestParticles(Image* binaryImage,
                                            ImageHits *hitReport, Rect rect,
                                            int numberHitsRequested)
{
  //char funcName[]="GetLargestParticles";
  HitList *hitsInArea = new HitList(); // list to contain all particles within area sorted by size
  if (hitsInArea == NULL)
  {
    printf("Memory go boom... :(\n");
    return 0;
  }
  int i;
  
  /* determine number of particles in thresholded image */
  int numParticles = -1;
  int success = frcCountParticles(binaryImage, &numParticles);
  if ( !success)
  {
    delete hitsInArea;
    return success;
  }
  //DPRINTF(LOG_DEBUG, "particles requested = %i ; particles found in image = %i", numberHitsRequested, numParticles);	

  /* if no particles found we can quit here */
  if (numParticles <= 0)
  {
    delete hitsInArea;
    return 1;
  } // successful, but zero particles found

  /* get areas of each particle and insert into list */
  double particleArea;
  int count = 0;
  for (i = 0; i < numParticles; ++i)
  {
    success = imaqMeasureParticle(binaryImage, i, 0, IMAQ_MT_AREA, &particleArea);
    if ( !success)
    {
      delete hitsInArea;
      return success;
    }
    //DPRINTF (LOG_INFO, "size of particle %i = %g", i, particleArea);
    // see if is in the right area and large enough to be considered a possible target

    //TODO: Call InArea & delete TempInArea when new WPILib is released
    //if (InArea(binaryImage, i, rect)) {
    //if ( InArea(binaryImage, i, rect) && (particleArea >= FRC_MINIMUM_PIXELS_FOR_TARGET) ) {

    //We want to search the entire area of the image, so no need to check
    if (/*TempInArea(binaryImage, i, rect) &&*/(particleArea
        >= FRC_MINIMUM_PIXELS_FOR_TARGET))
    {
      hitsInArea->AddNode(i, particleArea);
      count++;
    }
  }
  // see what's in the list 
  hitsInArea->debugDump();
  
  // fill in return structure - number of hits
  if (numParticles < numberHitsRequested)
  {
    hitReport->numberOfHits = numParticles;
  }
  else
  {
    hitReport->numberOfHits = numberHitsRequested;
  }
  
  // fill in return structure - indices & areas of largest hits
  HitNode *hitPtr = hitsInArea->head;
  for (i = 0; i < hitReport->numberOfHits; ++i)
  {
    if (hitPtr == NULL)
    {
      break;
    }
    hitReport->indices[i] = hitPtr->nodeIndex;
    hitReport->areas[i] = hitPtr->nodeArea;
    //DPRINTF (LOG_INFO, "put index %i in hitReport %i", hitPtr->nodeIndex, hitReport->indices[i]);
    hitPtr = hitPtr->next;
  }
  // dispose of HitList 
  delete hitsInArea;
  return 1; // success
}

/**
 * NOTE: This is included temporarily until the updated WPILib 
 * is released with the proper headers and immplmentation for 
 * this method. It will be in TrackAPI.cpp.
 * 
 * @brief Find the largest particle that meets a criteria 
 * @param binaryImage Image to inspect
 * @param rect area to search
 * @return 0 = error
 */
bool bhs_TargetAnalysis::TempInArea(Image* binaryImage, int particleIndex, Rect rect)
{
  //char funcName[]="InArea";
  double position;
  
  imaqMeasureParticle(binaryImage, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_LEFT,
                       &position);
  if (position < (rect.left ))
    return false; // outside left of rectangle?

  imaqMeasureParticle(binaryImage, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_TOP,
                       &position);
  if (position < (rect.top ))
    return false; // outside top of rectangle ?

  imaqMeasureParticle(binaryImage, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_RIGHT,
                       &position);
  if (position > (rect.left + rect.width))
    return false; // outside right of rectangle ?

  imaqMeasureParticle(binaryImage, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_BOTTOM,
                       &position);
  if (position > (rect.top + rect.height))
    return false; // outside bottom of rectangle ?	

  DPRINTF(LOG_INFO, "particle %i is in (%i %i) height %i width %i\n",
      particleIndex, rect.left, rect.top, rect.height, rect.width);
  return true;
}
