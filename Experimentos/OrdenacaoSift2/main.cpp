//
// Created by Guilherme Wachs on 23/03/16.
//


#include <stdio.h>

#include <VideoSift.hpp>
#include <random>
#include <tools.hpp>
#include <QString>
//#include <engine.h>
#include <QThreadPool>
#include <QtConcurrent>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

//#define VIDEO_SRC "/Users/wachs/Dropbox/SIFT/Videos/MundoVisto-de-Cima/ScreenCaptureProject108.mp4"
#define CutThreshold 200000
#define startFrame 8041
#define endFrame 8346

//#define startFrame 9092
//#define endFrame 9650

using namespace std;

struct FrameInfo {
    vector<feature> features;
    int frame_num;

    void freeMemory() {
        for (int i = 0; i < features.size(); i++) {
            free(features[i].feature_data);
        }
    }
};

bool operator==(const FrameInfo &a, const FrameInfo &b) {
    return a.frame_num == b.frame_num;
}

vector<int> geraAleatorio(int start, int end, int step) {
    vector<int> ret;
    for (int i = start; i <= end; i+= step) {
        ret.push_back(i);
    }

    shuffle(ret.begin() + 1, ret.end(), default_random_engine(time(0)));
    return ret;
}


vector<FrameInfo> ordena(vector<FrameInfo> list) {
    typedef vector<FrameInfo>::iterator frameIt;
    typedef pair<double, FrameInfo> FrameGrade;
    vector<FrameInfo> ret;

    /*if (list[0].features.size() == 0) {
        return list;
    }*/

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


        /*auto mitem = min_element(list.begin(), list.end(), [last](const FrameInfo &a, const FrameInfo &b) {
            return PDM(last.features, a.features) < PDM(last.features, b.features);
        });*/

        QVector<FrameInfo> qv = QVector<FrameInfo>::fromStdVector(list);

        QVector<FrameGrade> dists = QtConcurrent::blockingMapped<QVector<FrameGrade>>(qv, function<FrameGrade(
                const FrameInfo &)>([last](const FrameInfo &a) {
            return FrameGrade(PDM(last.features, a.features), a);
        }));

        auto mitem = min_element(dists.begin(), dists.end(), [](const FrameGrade &a, const FrameGrade &b) {
            return a.first < b.first;
        });

        //printf("%d\n", mitem->frame_num);

        ret.push_back(mitem->second);
        list.erase(find(list.begin(), list.end(), mitem->second));
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

float avalia2(vector<FrameInfo> result) {
    float res = 0;
    if(result[0].features.size() == 0)
        return 0;

    for (int i = 2; i < result.size() && result[i-1].features.size()>0 && result[i].features.size()>0; i++) {
        res +=  result[i - 1].frame_num < result[i].frame_num ? 1 : -1;
    }
    return res;
}

vector<FrameInfo> createVectorSpace2(const char *source, int sFrame, int eFrame, int step, float *q = NULL, float *b = NULL) {


    VideoSift video(source);
    QVector<int> order = QVector<int>::fromStdVector(geraAleatorio(sFrame, eFrame, step));


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

vector<FrameInfo> createVectorSpace(const char *source, int sFrame, int eFrame, int step, float *q = NULL, float *b = NULL) {


    VideoSift video(source, sFrame, eFrame);
    QVector<int> order = QVector<int>::fromStdVector(geraAleatorio(sFrame, eFrame, step));


    QVector<FrameInfo> res;
    if (!q || (q && *q == 1)) {
        res = QtConcurrent::blockingMapped<QVector<FrameInfo>>(order, function<FrameInfo(const int &)>(
                [&](const int &i) { return FrameInfo{video.extractSiftFeatures(i, 100), i}; }));
    } else {
        res = QtConcurrent::blockingMapped<QVector<FrameInfo>>(order, function<FrameInfo(const int &)>(
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

    QVector<double> v2 = QtConcurrent::blockingMapped<QVector<double>>(v, function<double(const int &)>(
            [](const int &i) { return 2.0 * i; }));

    for (auto i: v2) {
        printf("%f\n", i);
    }
    return 0;
}

tuple<int,int, QString> discoverFramesInFolder(QString folder){
    QFileInfo ff(folder);
    QFileInfoList imageFiles = ff.dir().entryInfoList();
    QVector<int> indexes;
    QRegularExpression re("-(\\d*)\\.\\w*$");
    QString basename;
    for(auto fi : imageFiles){
        QRegularExpressionMatch m = re.match(fi.fileName());
        if(m.hasMatch()){
            indexes.append(m.captured(1).toInt());
            basename = fi.fileName().replace(m.captured(1)+".png","");
        }
    }
    qSort(indexes);

    return tuple<int,int,QString>(indexes.first(),indexes.last(),basename);
};

bool checaPontosFiduciais(const vector<FrameInfo> &ordered){

    bool ok=true;
    int i=0;
    while(i < ordered.size() && ordered[i].features.size() > 0){
        i++;
    }

    for(;i<ordered.size() && ok; i++){
        ok = ok && (ordered[i].features.size()==0);
    }

    if(!ok){
        for(i=0;i<ordered.size();i++){
            printf("%lu ", ordered[i].features.size());
        }
        puts("");
    }

    return ok;
}


float countZeroFeatures(const vector<FrameInfo> &ordered){
    int r=0;
    for(int i=0; i < ordered.size(); i++){
        if(ordered[i].features.size()==0)
            r++;
    }
    return r*1./ordered.size();
}

int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("folder", "Images Folder");
    parser.addPositionalArgument("q", "q Parameter (0 <= q < 3)");
    parser.addPositionalArgument("b", "q Parameter (b > 0)");
    parser.addOption(QCommandLineOption(QStringList{"from","f"},"Starts at frame f", "from"));
    parser.addOption(QCommandLineOption(QStringList{"to","t"},"Ends at frame t","to"));
    parser.addOption(QCommandLineOption(QStringList{"step","s"},"Frame steps","step"));
    parser.process(app);

    float q = parser.positionalArguments().value(1).toFloat();
    float b = parser.positionalArguments().value(2).toFloat();
    int from, to, step=1;
    QString basename;
    tie(from, to, basename) = discoverFramesInFolder(parser.positionalArguments().value(0));
    basename = parser.positionalArguments().value(0) + basename;

    if(parser.isSet("from"))
        from = parser.value("from").toInt();
    if(parser.isSet("to"))
        to = parser.value("to").toInt();
    if(parser.isSet("step"))
        step = parser.value("step").toInt();

    //printf("%d %d\n",from, to);

    //printf("%f\n", q);
    if(q==1){

        //vector<FrameInfo> captured = createVectorSpace(basename.toStdString().c_str(), from, to, step);
        vector<FrameInfo> captured = createVectorSpace(basename.toStdString().c_str(), from, to, step);
        vector<FrameInfo> ordered = ordena(captured);
        float aval2 = avalia2(ordered);
        //printf("%d\t%.2f\t%.0f\n", step, countZeroFeatures(ordered), aval2);
        printf("%.2f\t%.2f\t%d\t%.2f\t%.0f\n", 1.0, -1., step, countZeroFeatures(ordered), aval2);

        for (FrameInfo f : captured) {
            f.freeMemory();
        }
    }else {
        vector<FrameInfo> captured = createVectorSpace(basename.toStdString().c_str(), from, to, step, &q, &b);
        vector<FrameInfo> ordered = ordena(captured);
        float aval2 = avalia2(ordered);

        printf("%.2f\t%.2f\t%d\t%.2f\t%.0f\n", q, b, step, countZeroFeatures(ordered), aval2);

        for (FrameInfo f : captured) {
            f.freeMemory();
        }
    }
    fflush(stdout);

    return 0;
}