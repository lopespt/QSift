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


#ifndef CPP_VIDEOSURF_H
#define CPP_VIDEOSURF_H

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
//#include <mutex>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

class VideoSurf {
private:
    int from;
    int to;
//    mutex x;
//    VideoCapture capture;
    string basename;
    Mat readFrame(const int &frameNum);

public:
//    VideoSurf(string path);
    VideoSurf(string basename, const unsigned int &from, const unsigned int &to);
    Mat extractSurfFeatures(const unsigned int &frameNum, const int &maxPoints,
                            const unsigned int &h);
};

#endif //CPP_VIDEOSURF_H
