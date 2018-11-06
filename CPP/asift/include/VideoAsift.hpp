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


#ifndef CPP_VideoASift_H
#define CPP_VideoASift_H

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include "demo_lib_sift.h"
#include "io_png/io_png.h"

#include "library.h"
#include "frot.h"
#include "fproj.h"
#include "compute_asift_keypoints.h"
#include "compute_asift_matches.h"

//#include <mutex>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

struct FrameInfo{
    int h;
    int w;
    vector<vector< keypointslist> > keypoints;
};

class VideoASift {
private:
    unsigned int from;
    unsigned int to;
//    mutex x;
//    VideoCapture capture;
    string basename;
    Mat readFrame(const int &frameNum);
    siftPar siftparameters;	
	
public:
//    VideoASift(string path);
    VideoASift(string basename, const unsigned int &from, const unsigned int &to);
    FrameInfo extractASiftFeatures(const unsigned int &frameNum);
};

#endif //CPP_VideoASift_H
