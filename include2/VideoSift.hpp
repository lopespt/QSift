//
// Created by Guilherme Wachs on 09/03/16.
//

#ifndef SIMPLE_EXAMPLE_VIDEOSIFT_HPP
#define SIMPLE_EXAMPLE_VIDEOSIFT_HPP


#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <sift.h>
#include <imgfeatures.h>
#include <vector>

using namespace std;
using namespace cv;

class VideoSift {

private:
    VideoCapture capture;
    Mat actualFrame;

public:
    VideoSift(string path);
    vector<feature> extractSiftFeatures(int frameNum, int maxPoints=-1);
    void play(const char* name, unsigned int start, unsigned int finish) ;

    unsigned int totalFrames() const;

};


#endif //SIMPLE_EXAMPLE_VIDEOSIFT_HPP
