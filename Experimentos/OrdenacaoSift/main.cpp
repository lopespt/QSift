//
// Created by Guilherme Wachs on 23/03/16.
//


#include <stdio.h>
#include <VideoSift.hpp>
#include <random>
#include <tools.hpp>
#include <engine.h>
#include <sift.h>


#define VIDEO_SRC "/Users/wachs/Dropbox/SIFT/Videos/MundoVisto-de-Cima/ScreenCaptureProject108.mp4"
#define CutThreshold 200000
#define startFrame 8041
#define endFrame 8346


using namespace std;

struct FrameInfo {
    vector<feature> features;
    int frame_num;

};
bool operator ==(const FrameInfo&a, const FrameInfo& b){
    return a.frame_num == b.frame_num;
}

vector<int> geraAleatorio(int start, int end) {
    vector<int> ret;
    for (int i = start; i <= end; i++) {
        ret.push_back(i);
    }

    shuffle(ret.begin() + 1, ret.end(), default_random_engine(time(0)));
    return ret;
}

vector<FrameInfo> ordena(vector<FrameInfo> list) {
    typedef vector<FrameInfo>::iterator frameIt;
    vector<FrameInfo> ret;

    ret.push_back(list.front());
    list.erase(list.begin());
    while (!list.empty()) {

        //printf("%d\n", ret.back().frame_num);

        FrameInfo last = ret.back();

        for (auto el:list) {
            //printf("%-10d\t", el.frame_num);
        }
        //printf("\n");
        for (auto el:list) {
            //printf("%-10.2f\t", PDM(last.features,el.features));
        }
        auto mitem = min_element(list.begin(), list.end(), [last](const FrameInfo &a, const FrameInfo &b) {
            return PDM(last.features, a.features) < PDM(last.features, b.features);
        });
        //printf("%d\n", mitem->frame_num);

        ret.push_back(*mitem);
        list.erase(mitem);
        //printf("\n");
    }
    return ret;
}


vector<FrameInfo> createVectorSpace(char *source, int sFrame, int eFrame) {
    vector<FrameInfo> list;
    VideoSift video(source);
    vector<int> order = geraAleatorio(sFrame, eFrame + 10);

    for (auto i : order) {
        list.push_back(
                FrameInfo{
                        video.extractSiftFeatures(i, 400),
                        i
                });
    }
    return list;
}

void plotMatlab(Engine* e, vector<double> v, vector<double> x = vector<double>()){
    mxArray *ret = mxCreateDoubleMatrix(v.size(),1,mxREAL);
    double *retv = (double*)mxGetData(ret);
    memcpy(retv,v.data(),sizeof(double)*v.size());
    engPutVariable(e,"plMatlab",ret);
    engEvalString(e,"plot(plMatlab);");

    if(x.size()!=0){
        mxArray *ret2 = mxCreateDoubleMatrix(v.size(),1,mxREAL);
        double *retv2 = (double*)mxGetData(ret2);
        memcpy(retv2,x.data(),sizeof(double)*x.size());
        engPutVariable(e,"plMatlab2",ret2);
        engEvalString(e,"plot(plMatlab2, plMatlab);");
        getchar();
        engEvalString(e,"delete plMatab2;");
        mxDestroyArray(ret2);
    }else{
        engEvalString(e,"plot(plMatlab);");
        getchar();
    }

    engEvalString(e,"delete plMatab;");
    mxDestroyArray(ret);
}


int main(int argc, char *argv[]) {
    srand((unsigned int) time(0));
    int i = 10;
    //vector<FrameInfo> list = createVectorSpace(VIDEO_SRC, startFrame, startFrame + 10);
    //vector<FrameInfo> ordenados = ordena(list);



    return 0;
}