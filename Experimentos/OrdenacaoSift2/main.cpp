//
// Created by Guilherme Wachs on 23/03/16.
//


#include <stdio.h>

#include <VideoSift.hpp>
#include <random>
#include <tools.hpp>
//#include <engine.h>
#include <QThreadPool>
#include <QtConcurrent>

//#define VIDEO_SRC "/Users/wachs/Dropbox/SIFT/Videos/MundoVisto-de-Cima/ScreenCaptureProject108.mp4"
#define VIDEO_SRC "/home/ribeiro.pinheiro/qsift/ScreenCaptureProject108.mp4"
#define IMG_SRC "/Users/wachs/Dropbox/Tchu/Viagens/Socorro/P7230168.JPG"
#define CutThreshold 200000
#define startFrame 8041
#define endFrame 8346

//#define startFrame 9092
//#define endFrame 9650

using namespace std;

struct FrameInfo {
    vector<feature> features;
    int frame_num;

    ~FrameInfo() {
        for (int i = 0; i < features.size(); i++) {
            free(features[i].feature_data);
        }
    }
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


vector<vector<FrameInfo>> generateMatrix(vector<FrameInfo> v){
    vector<vector<FrameInfo>> ret(v.size(), v);
    return ret;
}

vector<FrameInfo> ordena(vector<FrameInfo> list) {
    typedef vector<FrameInfo>::iterator frameIt;
    vector<FrameInfo> ret;

    if (list[0].features.size() == 0) {
        return list;
    }

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

float avalia(vector<FrameInfo> result) {
    float res = 0;
    for (int i = 1; i < result.size(); i++) {
        res += abs(result[i - 1].frame_num - result[i].frame_num);
    }
    return res / (result.size() - 1);
}

vector<FrameInfo> createVectorSpace2(const char *source, int sFrame, int eFrame, float *q = NULL, float *b = NULL) {

    QThreadPool pool;


    VideoSift video(source);
    QVector<int> order = QVector<int>::fromStdVector(geraAleatorio(sFrame, eFrame));


    vector<FrameInfo> list;
    if (!q) {
        for (auto i : order) {
            list.push_back(
                    FrameInfo{
                            video.extractSiftFeatures(i, 400),
                            i
                    });
        }
    } else {
        for (auto i : order) {
            list.push_back(
                    FrameInfo{
                            video.extractQSiftFeatures(i, 400, *q, *b),
                            i
                    });
        }
    }


    return list;
}

vector<FrameInfo> createVectorSpace(const char *source, int sFrame, int eFrame, float *q = NULL, float *b = NULL) {

    QThreadPool pool;


    VideoSift video(source, sFrame, eFrame);
    QVector<int> order = QVector<int>::fromStdVector(geraAleatorio(sFrame, eFrame));


    QVector<FrameInfo> res;
    if (!q) {
        res = QtConcurrent::blockingMapped(order, function<FrameInfo(const int &)>(
                [&](const int &i) { return FrameInfo{video.extractSiftFeatures(i, 100), i}; }));
    } else {
        res = QtConcurrent::blockingMapped(order, function<FrameInfo(const int &)>(
                [&](const int &i) { return FrameInfo{video.extractQSiftFeatures(i, 100, *q, *b), i}; }));
    }


    return res.toStdVector();
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
        engEvalString(e, "clear plMatab2;");
        mxDestroyArray(ret2);
    } else {
        engEvalString(e, "plot(plMatlab);");
        getchar();
    }

    engEvalString(e, "clear plMatab;");
    mxDestroyArray(ret);
}

void plotMesh(Engine *e, CvMat *f) {
    mxArray *ret = mxCreateDoubleMatrix(f->rows, f->cols, mxREAL);
    double *retv = (double *) mxGetData(ret);
    for (int i = 0; i < f->rows * f->cols; i++)
        retv[i] = (double) (f->data.fl[i]);

    engPutVariable(e, "plMatlab", ret);
    engEvalString(e, "surf(plMatlab)");
    getchar();
    engEvalString(e, "clear plMatlab;");
    mxDestroyArray(ret);
}

int main2(int argc, char *argv[]) {
    vector<double> y;
    vector<double> x;
    Engine *m = engOpen(0);

    CvMat *filt = createQGaussianKernel(2.5, 2);
    printMat(filt);
    plotMesh(m, filt);
    cvReleaseMat(&filt);

    vector<double> y2;
    vector<double> x2;

    for (int i = -700; i < 700; i++) {
        y2.push_back(Gaussian(i / 100., 1.6));
        x2.push_back(i / 100.);
    }

    while (true) {
        y.clear();
        x.clear();
        double q, b;
        scanf("%lf", &q);
        scanf("%lf", &b);
        engEvalString(m, "close all;");
        for (int i = -700; i < 700; i++) {
            if (i == 0)
                printf("%f\n", qGaussian(0, q, b));
            y.push_back(qGaussian(i / 100., q, b));
            x.push_back(i / 100.);
        }
        printf("Ploting q=%lf   b=%lf\n", q, b);
        plotMatlab(m, y, x);
        engEvalString(m, "figure;");
        plotMatlab(m, y2, x2);
    }
    engClose(m);
    return 0;
}

*/

int main2(int argc, char *argv[]) {
    QVector<int> v;
    for (int i = 0; i < 10000; i++) {
        v.push_back(i);
    }

    QVector<double> v2 = QtConcurrent::blockingMapped(v, function<double(const int &)>(
            [](const int &i) { return 2 * i; }));

    for (auto i: v2) {
        printf("%f\n", i);
    }
    return 0;

}

int main(int argc, char *argv[]) {


    float q;
    float b;
    for (b = 1; b < 3; b += 0.5) {
        for (q = 0; q < 3; q += 0.1) {
            vector<FrameInfo> captured = createVectorSpace(argv[1], startFrame, endFrame, &q,
            &b);
            if (captured[0].features.size() == 0) {
                fprintf(stderr, "Pulando config q=%f  b=%f\n", q, b);
                continue;
            }

            vector<FrameInfo> ordered = ordena(captured);

            float aval = avalia(ordered);
            //printf("q=%.2f\tb=%.2f   a=%.4f\n", q, b, aval);
            printf("%.2f\t%.2f\t%.4f\n", q, b, aval);
            fflush(stdout);
        }
    }


    return 0;
}