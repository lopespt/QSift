#include <stdio.h>

#include <VideoSift.hpp>
#include <random>
#include <tools.hpp>
#include <QString>
#include <QThreadPool>
#include <QtConcurrent>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <boost/lexical_cast.hpp>

using namespace std;

struct FrameInfo {
    vector<feature> features;
    int frame_num;
    Mat img;

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

typedef pair<FrameMatch, FrameInfo> FrameGrade;

vector<FrameGrade> ordena(vector<FrameInfo> list) {
    typedef vector<FrameInfo>::iterator frameIt;
    vector<FrameInfo> ret;
    vector<FrameGrade> ret2;

//    ret.push_back(FrameMatch(0.0, vector<feature>(), vector<feature>()), list.front());
    ret.push_back(list.front());
    ret2.push_back(FrameGrade(FrameMatch(), list.front()));
    list.erase(list.begin());

    while (!list.empty()) {
        FrameInfo last = ret.back();

        QVector<FrameInfo> qv = QVector<FrameInfo>::fromStdVector(list);

        QVector<FrameGrade> dists = QtConcurrent::blockingMapped<QVector<FrameGrade>>(qv, function<FrameGrade(
                const FrameInfo &)>([last](const FrameInfo &a) {
            return FrameGrade(PDM_Matches(last.features, a.features), a);
        }));

        auto mitem = min_element(dists.begin(), dists.end(), [](const FrameGrade &a, const FrameGrade &b) {
            return get<0>(a.first) < get<0>(b.first);
        });

        ret2.push_back(FrameGrade(mitem->first, mitem->second));
        ret.push_back(mitem->second);
        list.erase(find(list.begin(), list.end(), mitem->second));
    }

    return ret2;
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
    if(result[0].features.size() == 0 || result.size() == 0)
        return 0;

    for (int i = 2; i < result.size() && result[i-1].features.size()>0 && result[i].features.size()>0; i++) {
        res +=  result[i - 1].frame_num < result[i].frame_num ? 1 : -1;
    }
    return res;
}

vector<FrameInfo> createVectorSpace(const char *source, int sFrame, int eFrame, int step, float *q = NULL, float *b = NULL) {
    VideoSift video(source, sFrame, eFrame);
    QVector<int> order = QVector<int>::fromStdVector(geraAleatorio(sFrame, eFrame, step));

    QVector<FrameInfo> res;
    if (!q || (q && *q == 1)) {
        res = QtConcurrent::blockingMapped<QVector<FrameInfo>>(order, function<FrameInfo(const int &)>(
                [&](const int &i) { return FrameInfo{video.extractSiftFeatures(i, 10000), i, video.readFrame(i)}; }));
    } else {
        res = QtConcurrent::blockingMapped<QVector<FrameInfo>>(order, function<FrameInfo(const int &)>(
                [&](const int &i) { return FrameInfo{video.extractQSiftFeatures(i, 10000, *q, *b), i, video.readFrame(i)}; }));
    }

    return res.toStdVector();
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
    parser.addPositionalArgument("b", "b Parameter (b > 0)");
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

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0);

    if(q==1) {
        vector <FrameInfo> captured = createVectorSpace(basename.toStdString().c_str(), from, to, step);
        vector <FrameGrade> ordered = ordena(captured);
//        float aval2 = avalia2(ordered);

//        printf("%.2f\t%.2f\t%d\t%.2f\t%.4f\n", 1.0, -1., step, countZeroFeatures(ordered), aval2);

        string name = "Image_";

        for(int i = 0; i < ordered.size(); i++){
            String out = name + boost::lexical_cast<std::string>(i) + ".png";
            for(int j = 0; j < ordered[i].second.features.size(); j++){
                ordered[i].second.features[j].x *= 4;
                ordered[i].second.features[j].y *= 4;
            }
            draw_features(new IplImage(ordered[i].second.img), &ordered[i].second.features[0], ordered[i].second.features.size());

//            for(int j = 0; j < ordered[i].second.features.size(); j++){
//                printf("%d: (%.2lf, %.2lf)\n", j, ordered[i].second.features[j].x, ordered[i].second.features[j].y);
//            }

//            printf("Got Here!\n");
            imwrite(out, ordered[i].second.img, compression_params);
//            printf("And Here!\n");
//            imshow(out, image);
//            cvNamedWindow(out.c_str(), CV_WINDOW_AUTOSIZE );
//            cvShowImage(out.c_str(), captured[i].img);
//            cvWaitKey(0);
        }

        for (FrameInfo f : captured) {
            f.freeMemory();
        }
    }else {
        vector <FrameInfo> captured = createVectorSpace(basename.toStdString().c_str(), from, to, step, &q, &b);
        vector <FrameGrade> ordered = ordena(captured);
//        float aval2 = avalia2(ordered);

//        printf("%.2f\t%.2f\t%d\t%.2f\t%.4f\n", q, b, step, countZeroFeatures(ordered), aval2);

        string name = "Image_";

        for(int i = 0; i < ordered.size(); i++){
            String out = name + boost::lexical_cast<std::string>(i) + ".png";
            for(int j = 0; j < ordered[i].second.features.size(); j++){
                ordered[i].second.features[j].x *= 4;
                ordered[i].second.features[j].y *= 4;
            }
            draw_features(new IplImage(ordered[i].second.img), &ordered[i].second.features[0], ordered[i].second.features.size());

//            for(int j = 0; j < ordered[i].second.features.size(); j++){
//                printf("%d: (%.2lf, %.2lf)\n", j, ordered[i].second.features[j].x, ordered[i].second.features[j].y);
//            }

//            printf("Got Here!\n");
            imwrite(out, ordered[i].second.img, compression_params);
//            printf("And Here!\n");
//            imshow(out, image);
//            cvNamedWindow(out.c_str(), CV_WINDOW_AUTOSIZE );
//            cvShowImage(out.c_str(), captured[i].img);
//            cvWaitKey(0);
        }

        for (FrameInfo f : captured) {
            f.freeMemory();
        }
    }
    fflush(stdout);

    return 0;
}
