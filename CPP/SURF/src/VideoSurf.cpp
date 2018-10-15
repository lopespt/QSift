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


#include "VideoSurf.hpp"
#include <vector>
#include <iostream>

using namespace std;

//VideoSurf::VideoSurf(string path) : capture(path), from(0), to(0){
//
//}

VideoSurf::VideoSurf(string basename, const unsigned int &from,
const unsigned int &to) : basename(basename), from(from), to(to){

}

Mat VideoSurf::readFrame(const int &frameNum){
    Mat m;
    string r = basename + std::to_string(frameNum) + ".png";
    m = imread(r);

    return m;
}


class SortMat{
private:
    vector< tuple<double, int> > mag;

    static bool my_comp(const tuple<double, int> &a,
                        const tuple<double, int> &b){
        return get<0>(a) > get<0>(b);
    }

public:
    ~SortMat(){
        mag.clear();
    }

    SortMat(const vector<KeyPoint> &kp){
        mag.resize(kp.size());
        for(int i = 0; i < kp.size(); i++){
            mag[i] = tuple<double,int>(kp[i].size, i);
        }

        sort(mag.begin(), mag.end(), this->my_comp);
    }

    int getId(const int &i) const{
        return get<1>(mag[i]);
    }
};

Mat VideoSurf::extractSurfFeatures(const unsigned int &frameNum, const int &maxPoints = -1,
                                   const unsigned int &h = 200){
    cout << "frame: " << frameNum << endl;
    Mat img = readFrame(frameNum);

    Ptr<SURF> surf = SURF::create();
    Ptr<Feature2D> detector = surf;
    surf->setHessianThreshold(h);
    surf->setExtended(true);

    vector<KeyPoint> keypoints;
    Mat descriptors;

    detector->detectAndCompute(img, Mat(), keypoints, descriptors);

//    int count = 0;
//    for(int i = 1; i < keypoints.size(); i++){
//        if(keypoints[i].size > keypoints[i-1].size){
//            count++;
//        }
//    }

//    printf("%d OUT OF %ld", count, keypoints.size());

    img.release();

    if (maxPoints > 0) {
//        printf("Checkpoint %d\n", 1);
        int n = min(maxPoints, descriptors.rows);

        SortMat * smat = new SortMat(keypoints);

        Mat ret;

        for(unsigned int i = 0; i < n; i++){
            ret.push_back(descriptors.row(smat->getId(i)).clone());
//            printf("%d: %lf\n", i, keypoints[smat->getId(i)].size);
        }
//        for(unsigned int i = n; i < keypoints.size(); i++){
//            printf("%d: %lf\n", i, keypoints[smat->getId(i)].size);
//        }

        keypoints.clear();
        descriptors.release();
        delete smat;

//        printf("Checkpoint %d\n", 2);
        return ret;
    }
    else{
        keypoints.clear();
        return descriptors;
    }
//    return Mat(descriptors, Range(0, n), Range::all());
}