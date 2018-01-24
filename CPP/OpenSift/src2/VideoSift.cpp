//
// Created by Guilherme Wachs on 09/03/16.
//

#include "VideoSift.hpp"
#include <memory>
#include <mutex>
using namespace std;


VideoSift::VideoSift(string path) : capture(path), from(0), to(0) {

}

VideoSift::VideoSift(string basename, int from, int to) : basename(basename), from(from), to(to) {
}

Mat VideoSift::readFrame(int frameNum){
    Mat m;
    if(from==0 && to==0) {
        x.lock();
        capture.set(CV_CAP_PROP_POS_FRAMES, frameNum - 1);
        capture.read(m);
        x.unlock();
    }else {
        //puts( (basename + std::to_string(frameNum) + ".png\n").c_str());
        string r = basename + std::to_string(frameNum) + ".png";
        m = imread(r);
    }
    return m;
}

unsigned int VideoSift::totalFrames() const {
    return (unsigned int) (capture.get(CV_CAP_PROP_FRAME_COUNT));
}


void VideoSift::play(const char *name, unsigned int start, unsigned int finish) {
    namedWindow(name);
    Mat temp;
    for (int i = start; i <= finish; i++) {
        capture.set(CV_CAP_PROP_POS_FRAMES, i - 1);
        capture.read(temp);
        imshow(name, temp);
        waitKey(30);
    }
}

vector<feature> VideoSift::extractQSiftFeatures(int frameNum, int maxPoints, double q, double b) {
    Mat m = readFrame(frameNum);
    resize(m, m, Size(), 0.25, 0.25);
    //putText(actualFrame, String("Frame ") + to_string(frameNum), cv::Point2i(10,50), cv::HersheyFonts::FONT_HERSHEY_DUPLEX, 2, Scalar(0,0,255,0));
    //imshow(String("actual") + to_string(frameNum), this->actualFrame);
    //puts("teste");
    //waitKey(0);

    feature *ptr;
    unique_ptr<IplImage> img(new IplImage(m));
    int n = qsift_features(img.get(), &ptr, &q, &b);

//    // Checking if the features are sorted
//    for(int i = 0; i < n; i++){
//        printf("%d: %lf\n", i, ptr[i].scl);
//    }
//    exit(0);

    if (maxPoints > 0) {
        n = MIN(maxPoints, n);
    }

    vector<feature> ret(ptr, ptr + n);
    free(ptr);

    return ret;
}

vector<feature> VideoSift::extractSiftFeatures(int frameNum, int maxPoints) {
    Mat m = readFrame(frameNum);
    resize(m, m, Size(), 0.25, 0.25);
    //putText(actualFrame, String("Frame ") + to_string(frameNum), cv::Point2i(10,50), cv::HersheyFonts::FONT_HERSHEY_DUPLEX, 2, Scalar(0,0,255,0));
    //imshow(String("actual") + to_string(frameNum), this->actualFrame);
    //waitKey(0);
    //printf("opa\n");

    feature *ptr;
    unique_ptr<IplImage> img(new IplImage(m));

    int n = sift_features(img.get(), &ptr);
    if (maxPoints > 0) {
        n = MIN(maxPoints, n);
    }
    vector<feature> ret(ptr, ptr + n);
    free(ptr);


    return ret;
}


