#define __BLOB_DETECTOR_V2_HPP++
#include "opencv2/core/core.hpp"

using namespace cv;

class BlobDetector
{
   public:
      static vector<vector<Point> > findBlobs(Mat &binaryImage, int pMinArea, double pEpsilon);
      static Vec3f getValidDepth(Point &pPnt, Mat &pCloud, int step=1);
};
