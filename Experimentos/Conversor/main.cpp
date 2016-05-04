//
// Created by Guilherme Wachs on 04/05/16.
//

#include <QtConcurrent>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <QFileInfo>
using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {

    VideoCapture cap;
    if (!cap.open(argv[1])) {
        puts("Nao consegui abrir o arquivo");
        exit(0);
    }
    int start = std::stoi(argv[2]);
    int end = std::stoi(argv[3]);

    Mat current;
    char nome[100];
    strcpy(nome, argv[1]);
    *strrchr(nome, '.') = '\0';
    for (int i = start; i <= end; i++) {
        cap.set(CAP_PROP_POS_FRAMES, i);
        cap.read(current);
        strcpy(nome, argv[1]);
        *strrchr(nome, '.') = '\0';
        sprintf(nome, "%s-%d.png", nome, i);
        QFileInfo f(nome);
        printf("%s\n", f.fileName().toStdString().c_str());
        imwrite(f.fileName().toStdString().c_str(), current, vector<int>{CV_IMWRITE_PNG_COMPRESSION});
    }

    return 0;
}

