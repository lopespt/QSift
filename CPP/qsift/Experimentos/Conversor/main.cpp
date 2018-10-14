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

#include <QFileInfo>
#include <QDir>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <sstream>

using namespace cv;
using namespace std;

string formatFilename(QString folder,QString filename, unsigned int frame){
    stringstream s;
    QDir d = QDir(folder);
    if(d.isRelative()){
        s << "./";
    }
    s << folder.toStdString() << "/" << filename.toStdString() << "-" << frame << ".png";
    cout << s.str() << endl;
    return s.str();
}

int main(int argc, char *argv[]) {

    VideoCapture cap;
    if (!cap.open(argv[1])) {
        puts("Nao consegui abrir o arquivo");
        exit(0);
    }
    unsigned int start = std::stoi(argv[2]);
    unsigned int end = std::stoi(argv[3]);

    if (start==-1)
        start = 0;
    if(end == -1)
        end = (int) (cap.get(CAP_PROP_FRAME_COUNT) - 1);

    QString folder=QFileInfo(argv[1]).absolutePath();
    QString filename = QFileInfo(argv[1]).fileName();
    if(argc > 3){
        folder = QString(argv[4]);
    }

    Mat current;
    
    unsigned int i;
    #pragma omp parallel for private(i)
    for (i = start; i <= end; i++) {
        cap.set(CAP_PROP_POS_FRAMES, i);
        cap.read(current);
        
        imwrite( formatFilename(folder, filename, i), current, vector<int>{CV_IMWRITE_PNG_COMPRESSION});
    }

    return 0;
}

