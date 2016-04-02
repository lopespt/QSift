//
// Created by Guilherme Wachs on 09/03/16.
//

#include <opencv2/imgproc.hpp>
#include "VideoSift.hpp"


VideoSift::VideoSift(string path) : capture(path) {

}

vector<feature> VideoSift::extractSiftFeatures(int frameNum, int maxPoints) {
    capture.set(CV_CAP_PROP_POS_FRAMES, frameNum-1);
    capture.read(this->actualFrame);
    resize(this->actualFrame, this->actualFrame, Size(), 0.25,0.25);
    putText(actualFrame, String("Frame ") + to_string(frameNum), cv::Point2i(10,50), cv::HersheyFonts::FONT_HERSHEY_DUPLEX, 2, Scalar(0,0,255,0));
    //imshow(String("actual") + to_string(frameNum), this->actualFrame);

    feature *ptr;
    unique_ptr<IplImage> img(new IplImage(actualFrame));
    int n=sift_features(img.get(), &ptr);
    if(maxPoints>0){
        n=MIN(maxPoints, n);
    }

    vector<feature> ret(ptr, ptr+n);
    free(ptr);

    return ret;
}

unsigned int VideoSift::totalFrames() const {
    return (unsigned int) (capture.get(CV_CAP_PROP_FRAME_COUNT));
}


void VideoSift::play(const char *name, unsigned int start, unsigned int finish) {
    namedWindow(name);
    Mat temp;
    for(int i=start; i<=finish; i++) {
        capture.set(CV_CAP_PROP_POS_FRAMES, i-1);
        capture.read(temp);
        imshow(name, temp);
        waitKey(30);
    }
}
