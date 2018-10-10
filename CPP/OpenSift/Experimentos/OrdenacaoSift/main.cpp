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


#include <stdio.h>
#include <VideoSift.hpp>
#include <random>
#include <tools.hpp>
//#include <engine.h>


#define VIDEO_SRC "/Users/wachs/Dropbox/SIFT/Videos/MundoVisto-de-Cima/ScreenCaptureProject108.mp4"
#define IMG_SRC "/Users/wachs/Dropbox/Tchu/Viagens/Socorro/P7230168.JPG"
#define CutThreshold 200000
#define startFrame 8041
#define endFrame 8346


using namespace std;

struct FrameInfo {
    vector<feature> features;
    int frame_num;

};

bool operator==(const FrameInfo &a, const FrameInfo &b) {
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
/*
void plotMatlab(Engine *e, vector<double> v, vector<double> x = vector<double>()) {
    mxArray *ret = mxCreateDoubleMatrix(v.size(), 1, mxREAL);
    double *retv = (double *) mxGetData(ret);
    memcpy(retv, v.data(), sizeof(double) * v.size());
    engPutVariable(e, "plMatlab", ret);
    engEvalString(e, "plot(plMatlab);");

    if (x.size() != 0) {
        mxArray *ret2 = mxCreateDoubleMatrix(v.size(), 1, mxREAL);
        double *retv2 = (double *) mxGetData(ret2);
        memcpy(retv2, x.data(), sizeof(double) * x.size());
        engPutVariable(e, "plMatlab2", ret2);
        engEvalString(e, "plot(plMatlab2, plMatlab);");
        getchar();
        engEvalString(e, "delete plMatab2;");
        mxDestroyArray(ret2);
    } else {
        engEvalString(e, "plot(plMatlab);");
        getchar();
    }

    engEvalString(e, "delete plMatab;");
    mxDestroyArray(ret);
}
*/

int main(int argc, char *argv[]) {

    srand((unsigned int) time(0));
    int i = 10;
    //vector<FrameInfo> list = createVectorSpace(VIDEO_SRC, startFrame, startFrame + 10);
    //vector<FrameInfo> ordenados = ordena(list);

    IplImage *img = cvLoadImage(IMG_SRC, CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *img2 = cvCreateImage(cvSize(img->width / 10, img->height / 10), img->depth, img->nChannels);
    cvResize(img, img2);
    IplImage *img3 = create_init_img(img2, 0, SIFT_SIGMA);
    double qs[] = {2, 0.75,1.1};
    double b = 1;
    IplImage ***octave = build_gauss_pyr(img3, 3, 3, SIFT_SIGMA, 1, qs, &b);
    cvReleaseImage(&img);
    cvReleaseImage(&img2);

    char name[40];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sprintf(name, "%d %d", i, j);
            cvShowImage(name, octave[i][j]);
        }
    }


    cvWaitKey(0);
    release_pyr(&octave, 3, 3);


    cvReleaseImage(&img3);


    return 0;
}