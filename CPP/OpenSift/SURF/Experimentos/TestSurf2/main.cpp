//
// Created by mille on 22/09/2017.
//

#include <QString>
#include <QThreadPool>
#include <QtConcurrent>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <iostream>
#include <cstdio>
#include "VideoSurf.hpp"

using namespace std;

struct FrameInfo {
    Mat features;
    int frame_num;

    void freeMemory() {
        features.release();
    }
};

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

    printf("Checkpoint %d\n", 1);
    VideoSurf video(basename.toStdString().c_str(), from, to);
    printf("Checkpoint %d\n", 2);
    FrameInfo fi{video.extractSurfFeatures(from, 100, h), from};
    printf("Checkpoint %d\n", 3);

    float maglast = 0.0;

    Mat feat = fi.features;
    for(int i = 0; i < feat.row(0).cols; i++){
        maglast += feat.row(0).at<float>(i) * feat.row(0).at<float>(i);
    }

    int count = 0;

    for(int j = 1; j < feat.rows; j++){
        float mag = 0;
        for(int i = 0; i < feat.row(0).cols; i++){
            mag += feat.row(j).at<float>(i) * feat.row(j).at<float>(i);
        }

//        printf("%d: %f %f\n", j, mag, maglast);

        if(mag > maglast){
            count++;
        }
        maglast = mag;
    }

    cout << count << " of " << feat.rows - 1 << endl;

    return 0;
}