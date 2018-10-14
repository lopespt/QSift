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

#include <VideoSift.hpp>


#define VIDEO_SRC "/Users/wachs/Dropbox/SIFT/Videos/MundoVisto-de-Cima/ScreenCaptureProject108.mp4"
#define IMG_SRC "/Users/wachs/Dropbox/Tchu/29.jpg"
#define CutThreshold 200000

void outMat(CvMat *mat){
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++) {
                printf("%.8f\t", cvGet2D(mat,i,j).val[0] );
            }
            puts("");
        }
}

int main(int argc, char *argv[]) {

    CvMat *mm = cvLoadImageM(IMG_SRC, CV_LOAD_IMAGE_GRAYSCALE);
    CvMat *m = cvCreateMat(mm->rows, mm->cols, CV_8UC1);
    cvResize(mm,m);
    CvMat *m2 = cvCloneMat(m);
    CvMat *m3 = cvCloneMat(m);
    CvMat *comp = cvCloneMat(m);

    vector< tuple<float,float,float> > examples;
    examples.push_back({0.5,0.5,0.5});
    examples.push_back({0.99,0.5,0.5});
    examples.push_back({0.5,0.5,0.8});
    examples.push_back({0.9999,0.5,0.5});

    CvMat *k;
    for(auto i : examples) {
        k = createQGaussianKernel( std::get<0>(i),std::get<1>(i),std::get<2>(i) );
        outMat(k);
        puts("\n");
        cvReleaseMat(&k);
    }
    return 0;
    k = createQGaussianKernel(0.5, 0.5, 0.5);


    cvFilter2D(m, m2, k);
    cvSmooth(m, m3, CV_GAUSSIAN, 5, 5, 1.25, 1.25);

    cvCmp(m2,m3,comp,CV_CMP_EQ);
    double s = cvSum(comp).val[0];
    printf("sum = %f\n", s);

    cvShowImage("img orig", m);
    cvShowImage("img minha", m2);
    cvShowImage("img opencv", m3);
    cvWaitKey(0);

    cvReleaseMat(&mm);
    cvReleaseMat(&m);
    cvReleaseMat(&m2);
    cvReleaseMat(&m3);

    return 0;
}