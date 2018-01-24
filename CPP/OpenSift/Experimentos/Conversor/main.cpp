//
// Created by Guilherme Wachs on 04/05/16.
//

#include <QFileInfo>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

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

    if (start==-1)
        start = 0;
    if(end == -1)
        end = (int) (cap.get(CAP_PROP_FRAME_COUNT) - 1);

    Mat current;
    char nome[100];
    strcpy(nome, argv[1]);
    *strrchr(nome, '.') = '\0';
    int i;
    #pragma omp parallel for private(i)
    for (i = start; i <= end; i++) {
        cap.set(CAP_PROP_POS_FRAMES, i);
        cap.read(current);
        strcpy(nome, argv[1]);
        *strrchr(nome, '.') = '\0';
        sprintf(nome, "%s-%d.png", nome, i);
        QFileInfo f(nome);
        printf("%s\n", f.fileName().toStdString().c_str());
        imwrite(f.fileName().toStdString(), current, vector<int>{CV_IMWRITE_PNG_COMPRESSION});
    }

    return 0;
}

