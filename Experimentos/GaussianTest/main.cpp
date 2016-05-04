//
// Created by Guilherme Wachs on 16/03/16.
//

#include <VideoSift.hpp>


#define VIDEO_SRC "/Users/wachs/Dropbox/SIFT/Videos/MundoVisto-de-Cima/ScreenCaptureProject108.mp4"
#define IMG_SRC "/Users/wachs/Dropbox/Tchu/Viagens/Socorro/P7230168.JPG"
#define CutThreshold 200000

int main(int argc, char *argv[]) {

    CvMat *mm = cvLoadImageM(IMG_SRC, CV_LOAD_IMAGE_GRAYSCALE);
    CvMat *m = cvCreateMat(mm->rows/10, mm->cols/10, CV_8UC1);
    cvResize(mm,m);
    CvMat *m2 = cvCloneMat(m);
    CvMat *m3 = cvCloneMat(m);
    CvMat *comp = cvCloneMat(m);

    CvMat *k = createQGaussianKernel(1,1);

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