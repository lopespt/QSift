//
// Created by Guilherme Wachs on 08/03/16.
//

#include <sift.h>

#include <highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <imgfeatures.h>
#include <utils.h>
#include <opencv2/imgproc.hpp>
#include <VideoSift.hpp>
#include <imgfeatures.h>

#define VIDEO_SRC "/home/miller/Desktop/OneDrive - DePaul University/qSIFT/Videos/MundoVisto-de-Cima/ScreenCaptureProject108.mp4"

IplImage *resizeIpl(IplImage *img, float factor){
    CvSize s = cvGetSize(img);
    s.height *= factor;
    s.width *= factor;

    IplImage *ret = cvCreateImage( s, img->depth, img->nChannels);
    cvResize(img, ret);
    return ret;
}

using namespace std;
using namespace cv;


int main(int argc, char *argv[]) {
    VideoSift vid(VIDEO_SRC);
    vector<vector<feature>> feats;
    for(int i=200; i<220; i+=2){
        feats.push_back(vid.extractSiftFeatures(i));
    }

    vector<double> dists;
    for(int i=0; i<feats.size(); i++){
        dists.push_back(descr_dist_sq(feats[0].data(), feats[i].data()));
    }

    for(auto k : dists){
        printf("%.2f\n", k);
    }
    //waitKey(0);
}

int main2(int argc, char *argv[]) {

    VideoCapture capture(VIDEO_SRC);
    Mat img;
    int frame=0;
    capture.set(CV_CAP_PROP_POS_FRAMES, 250);

    while(capture.read(img)){
        double f = capture.get(CV_CAP_PROP_POS_FRAMES);
        putText(img, String("Frame ") + to_string(f), cv::Point2i(10,50), cv::HersheyFonts::FONT_HERSHEY_DUPLEX, 2, Scalar(0,0,255,0));
        imshow("out", img);

        int z = waitKey(0);
        if(z == '/')
            break;
        if(z == 'b')
            capture.set(CV_CAP_PROP_POS_FRAMES, 2500);

        frame++;
    }

    return 0;
}

