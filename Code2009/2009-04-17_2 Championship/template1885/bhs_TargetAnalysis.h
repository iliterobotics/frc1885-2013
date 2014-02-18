#ifndef __TARGET_H__
#define __TARGET_H__

#include "WPILib.h"
#include "RobotGlobalData.h"
#include "bhs_CameraInf.h"
#include "bhs_Constants.h"
#include "VisionAPI.h"
#include "TrackAPI.h"
#include "DistanceCalculator.h"
#include "DriverStation.h"
#include "CameraTurret.h"
#include <set>
/*  Constants */
/* 
 * If you want to change these settings, test with 
 * GREEN in several different light settings. Remember
 * there may be more glare on the field which could
 * cause failure to recognize the whole target.
 */
/** In order to report a color particle it must have this
 * number of pixels or more. Because the color parameters
 * are set fairly wide to ensure recognition in different
 * light settings, it is common to pick up random pixels
 * of similar colors in the enviornment. This parameter
 * reduces the number of very small particles processed.
 */
#define FRC_MINIMUM_PIXELS_FOR_TARGET 5

/** Scaling factor for determining alignment of target colors 
 * to ensure one target is exactly above the other, use a
 * smaller number. However, light shining directly on the
 * target causes significant variation, so this parameter
 * is best left fairly high. */
#define FRC_ALIGNMENT_SCALE 3.0

/** Tolerance for two-color target color separation (# pixels) 
 * this is loose enough for any image size but could be tightened 
 * (reduced) to reduce false alarms
 */
#define FRC_MAX_IMAGE_SEPARATION 20

/** Tolerance for two-color target particle size (factor) 
 * keep this relatively large so that differences in color or light
 * do not affect target recognition */
#define FRC_SIZE_FACTOR 3

/** Maximum number of color particles to find */
#define FRC_MAX_HITS 10

/** Image quality requirement: particle must be this % of pixels
 * For instance, a 320x240 image has 76800 pixels. With this 
 * tolerance at .001, the image must be 77 pixels.
 * Uses a percentage instead of a fixed # of pixels so different
 * image sizes will work the same way */
#define FRC_COLOR_TO_IMAGE_PERCENT 0.001

struct bhs_TargetData
{
    ParticleAnalysisReport mnParticleReport;
    DriverStation::Alliance meTeamColor; //the color of the team the target is from
};

struct bhs_TargetReport
{
    int reportNumber;
    int numberOfValidTargetReports;
    //array of 5 bhs_TargetData  mnReport[5];
    bhs_TargetData mnReport[bhs_Constants::cnMAX_TARGET_REPORTS];

    bhs_TargetReport()
    {
      static int lastReportNumber = 0;
      
      numberOfValidTargetReports = 0;
      reportNumber = lastReportNumber;
      lastReportNumber++;
    }
};

typedef struct ImageHits_struct
{
    int numberOfHits;
    int indices[FRC_MAX_HITS];
    double areas[FRC_MAX_HITS];
    ParticleAnalysisReport pars[FRC_MAX_HITS];
} ImageHits;

class bhs_ParticleReportKey
{
  public:
    bhs_ParticleReportKey(Rect aParArea)
    {
      parArea = aParArea;
    }
    friend bool operator==(const bhs_ParticleReportKey& acFirst,
                           const bhs_ParticleReportKey& acSecond);
    friend bool operator>(const bhs_ParticleReportKey& acFirst,
                         const bhs_ParticleReportKey& acSecond);
    friend bool operator<(const bhs_ParticleReportKey& acFirst,
                          const bhs_ParticleReportKey& acSecond);

  private:
    Rect parArea;
};

/*  Enumerated Types */
/** Position of the second color related to the first color */
enum SecondColorPosition
{ ABOVE = 0x0001, BELOW = 0x0002, RIGHT = 0x0004, LEFT = 0x0008};

class bhs_TargetAnalysis
{
  public:
    bhs_TargetAnalysis();
    ~bhs_TargetAnalysis();
    static void CameraTrackingTask(int anThis);
    void configureCamera(bhs_CameraInf *apCamera, RobotGlobalData *apData, DriverStation *apDS);
    void getDecodedImage();
    int initTargetAnalysis(SEM_ID acTimingSem); //Spawn a task to process the image
    bhs_TargetReport FindTwoColors(TrackingThreshold &td1, TrackingThreshold &td2,
                                   int position, int numberHits, Image* imageToPass);
    int FindColorHits(ColorMode mode, const Range* plane1Range,
                      const Range* plane2Range, const Range* plane3Range,
                      ImageHits* hitReport, Rect rect, int numberHitsRequested,
                      Image* imageToPass);
    bool Aligned(int center1, int center2, int dimension1, int dimension2);
    bool Adjacent(int value1, int value2);
    bool SizesRelative(double area1, double area2);
    int GetLargestParticles(Image* binaryImage, ImageHits *hitReport, Rect rect,
                            int numberHitsRequested);
    bool TempInArea(Image* binaryImage, int particleIndex, Rect rect);
  private:
    void getNextServoAngle();
    bool servoInPosition();
    
    enum ServoAngle { kServoAngleFirst, kServoAngleCenter, kServoAngleLast };
    ServoAngle meServoAngle;
    enum ServoState { kSetPanAngle, kGoToAngle, kThrowAwayFrame, kAnalyzeFrame };
    ServoState meServoState;
    bool mbPanForward;
    
    float mnPreviousPan;
    ServoAngle meLastAngle;
    
    CameraTurret mcCameraTurret;
    std::set<bhs_ProcessedTarget> msForwardSweepTargets;
    std::set<bhs_ProcessedTarget> msBackwardSweepTargets;
    
    SEM_ID mcTimingSem;
    
    DistanceCalculator maDistanceCalc;
    bhs_CameraInf *mpCamera;
    RobotGlobalData *mpData;
    DriverStation *mpDS;
    TrackingThreshold mtd1, mtd2; // color thresholds
};
#endif
