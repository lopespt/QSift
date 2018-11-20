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
#include <opencv2/core.hpp>
#include <QString>
#include <QThreadPool>
#include <QtConcurrent>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <vector>
#include <random>
#include <cmath>
#include "VideoSurf.hpp"

#include <algorithm>

using namespace std;
using namespace cv;

const int N_DIMS = 128;

struct FrameInfo {
    Mat features;
    int frame_num;

    void freeMemory() {
        features.release();
    }
};

bool operator == (const FrameInfo &a, const FrameInfo &b){
    return a.frame_num == b.frame_num;
}

vector<int> geraAleatorio(const int &start, const int &end, const int &step) {
    vector<int> ret;
    for (int i = start; i <= end; i+= step) {
        ret.push_back(i);
    }

    shuffle(ret.begin() + 1, ret.end(), default_random_engine(time(0)));
    return ret;
}

vector<FrameInfo> createVectorSpace(const char * const source, const unsigned int &sFrame,
                                    const unsigned int &eFrame, const unsigned int &step,
                                    const float &h = 200) {
    VideoSurf video(source, sFrame, eFrame);
    vector<int> v = geraAleatorio(sFrame, eFrame, step);
    QVector<int> order = QVector<int>::fromStdVector(v);

    vector<FrameInfo> res;
    /*res = QtConcurrent::blockingMapped<QVector<FrameInfo>>(order, function<FrameInfo(const int &)>(
            [&](const int &i) { return FrameInfo{video.extractSurfFeatures(i, 100, h), i}; }));
*/
    auto rit = back_inserter(res);
    transform(order.begin(), order.end(), rit, function<FrameInfo(const int &)>(
            [&](const int &i) { return FrameInfo{video.extractSurfFeatures(i, 100, h), i}; }));

    return res;
}

float descr_dist_sq(const Mat &m1, const Mat &m2){
    float dist = 0;

    if(m1.cols != m2.cols){
        printf("DIFFERENT SIZED DESCRIPTORS!\n");
        exit(2);
    }

    for(int i = 0; i < m1.cols; i++){
        float temp = m1.at<float>(i) - m2.at<float>(i);
        dist += temp * temp;
    }

//    return sqrt(dist);
    return dist;
}

float PDM(Mat fset1, Mat fset2) {
    float dist = 0;

    for (int i = 0; i < fset1.rows; i++) {
//        vector<float> dists(fset2.rows);
//        std::transform(fset2.begin<Mat>(), fset2.end<Mat>(), dists.begin() , [&](Mat f){
//            return descr_dist_sq(f, fset1.row(i));
//        });
//        if(dists.size() == 0) {
//            return DBL_MAX;
//        }else{
//            dist += *std::min_element(dists.begin(), dists.end());
//        }

        float min_dist = FLT_MAX;
        for(int j = 0; j < fset2.rows; j++){
            float temp_dist = descr_dist_sq(fset2.row(j), fset1.row(i));

            min_dist = min(min_dist, temp_dist);
        }
        dist += min_dist;
    }
    for (int i = 0; i < fset2.rows; i++) {
//        vector<float> dists(fset1.rows);
//        std::transform(fset1.begin<float *>(), fset1.end<float *>(),dists.begin() , [&](float * f){
//            return descr_dist_sq(f, fset2.row(i));
//        });
//        if(dists.size() == 0){
//            return DBL_MAX;
//        }else {
//            dist += *std::min_element(dists.begin(), dists.end());
//        }

        float min_dist = FLT_MAX;
        for(int j = 0; j < fset1.rows; j++){
            float temp_dist = descr_dist_sq(fset1.row(j), fset2.row(i));

            min_dist = min(min_dist, temp_dist);
        }
        dist += min_dist;
    }

    return dist / (fset1.rows + fset2.rows);
}

vector<FrameInfo> ordena(vector<FrameInfo> list) {
    typedef vector<FrameInfo>::iterator frameIt;
    typedef pair<float, FrameInfo> FrameGrade;
    vector<FrameInfo> ret;

    ret.push_back(list.front());
    list.erase(list.begin());


    while (!list.empty()) {
        FrameInfo last = ret.back();

        QVector<FrameInfo> qv = QVector<FrameInfo>::fromStdVector(list);

        QVector<FrameGrade> dists = QtConcurrent::blockingMapped<QVector<FrameGrade>>(qv, function<FrameGrade(
        const FrameInfo &)>([last](const FrameInfo &a) {
            return FrameGrade(PDM(last.features, a.features), a);
        }));

        auto mitem = min_element(dists.begin(), dists.end(), [](const FrameGrade &a, const FrameGrade &b) {
            return a.first < b.first;
        });

        ret.push_back(mitem->second);
        list.erase(find(list.begin(), list.end(), mitem->second));
    }
    return ret;
}

float avalia2(vector<FrameInfo> result) {
    float res = 0;
    if(result[0].features.rows == 0 || result.size() == 0)
        return 0;

//    printf("%d %d\n", result[0].features.rows, result.size());

    for (int i = 2; i < result.size() && result[i-1].features.rows>0 && result[i].features.rows>0; i++) {
        res +=  result[i - 1].frame_num < result[i].frame_num ? 1 : -1;
    }
    //return res;
    return (res + float(result.size() - 1)) / (2.0 * float(result.size()) - 2.0);
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

float countZeroFeatures(const vector<FrameInfo> &ordered){
    int r=0;
    for(int i=0; i < ordered.size(); i++){
        if(ordered[i].features.rows==0)
            r++;
    }
    return r*1./ordered.size();
}

int main(int argc, char * argv[]){
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("folder", "Images Folder");
    parser.addPositionalArgument("h", "Hessian Threshold");
    parser.addOption(QCommandLineOption(QStringList{"from","f"},"Starts at frame f", "from"));
    parser.addOption(QCommandLineOption(QStringList{"to","t"},"Ends at frame t","to"));
    parser.addOption(QCommandLineOption(QStringList{"step","s"},"Frame steps","step"));
    parser.process(app);

    float h = parser.positionalArguments().value(1).toFloat();
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

    vector<FrameInfo> captured = createVectorSpace(basename.toStdString().c_str(), from, to, step, h);

    vector<FrameInfo> ordered = ordena(captured);

    float aval2 = avalia2(ordered);

    printf("%.2f\t%d\t%.2f\t%.4f\n", h, step, countZeroFeatures(ordered), aval2);

    for (FrameInfo f : captured) {
        f.freeMemory();
    }

    fflush(stdout);

    return 0;
}