//
// Created by Guilherme Wachs on 09/03/16.
//

#ifndef SIMPLE_EXAMPLE_VIDEOSIFT_HPP
#define SIMPLE_EXAMPLE_VIDEOSIFT_HPP


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <sift.h>
#include <imgfeatures.h>
#include <vector>
#include <string>
#include <mutex>


using namespace std;
using namespace cv;

class VideoSift {

private:
    VideoCapture capture;
    Mat actualFrame;
    mutex x;
    int from;
    int to;
    string basename;
    Mat readFrame(int);

public:
    VideoSift(string path);
    VideoSift(string basename,int from, int to);
    vector<feature> extractSiftFeatures(int frameNum, int maxPoints=-1);
    vector<feature> extractQSiftFeatures(int frameNum, int maxPoints=-1, double q=1.5, double b=1);
    void play(const char* name, unsigned int start, unsigned int finish) ;

    unsigned int totalFrames() const;

};


#endif //SIMPLE_EXAMPLE_VIDEOSIFT_HPP
