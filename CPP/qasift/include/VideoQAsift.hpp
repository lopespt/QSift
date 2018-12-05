// ------------------------------------------------------------------------
// Project: q-SIFT
// Authors:  Guilherme Wachs-Lopes <gwachs@fei.edu.br>
//           Miller Horvath <mhorvath@fei.edu.br>
//
// This is an open source code created for research and academic purposes.
// If you have used this code in you research, please cite:
//
//   P. S. Rodrigues, G. A. Wachs-Lopes, G. Antonio Giraldi and M. Horvath,
//   in press. q-SIFT: A Strategy Based on Non-Extensive Statistic to
//   Improve SIFT Algorithm Under Severe Conditions. Pattern Recognition.
//
// ------------------------------------------------------------------------

#ifndef CPP_VideoQASift_H
#define CPP_VideoQASift_H

#include <demo_lib_sift.h>
#include "io_png/io_png.h"
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "compute_asift_keypoints.h"
#include "compute_asift_matches.h"
#include "fproj.h"
#include "frot.h"
#include "library.h"

//#include <mutex>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

struct FrameInfo {
  int h;
  int w;
  vector<vector<keypointslist>> keypoints;
};
class VideoQASift {
private:
  unsigned int from;
  unsigned int to;
  //    mutex x;
  //    VideoCapture capture;
  string basename;
  float q;
  float b;
  Mat readFrame(const int &frameNum);
  siftPar siftparameters;

public:
  //    VideoQASift(string path);
  VideoQASift(string basename, const unsigned int &from, const unsigned int &to, float q, float b);
  FrameInfo extractQASiftFeatures(const unsigned int &frameNum);
  FrameInfo extractQASiftFeaturesMemoized(const unsigned int &frameNum);
};

#endif // CPP_VideoQASift_H
