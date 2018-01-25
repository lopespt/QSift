//
// Created by mille on 21/09/2017.
//

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
