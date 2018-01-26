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
#include <VideoSift.hpp>
#include <tools.hpp>


#define VIDEO_SRC "/Users/wachs/Dropbox/SIFT/Videos/MundoVisto-de-Cima/ScreenCaptureProject108.mp4"
#define CutThreshold 200000

int main(int argc, char *argv[]) {
    VideoSift video(VIDEO_SRC);

    FILE *out = fopen("diferencas.txt", "w");

    unsigned int total = video.totalFrames();

    unsigned int startFrame = 0;
    unsigned int lastFrame = total;

    auto last = video.extractSiftFeatures(startFrame, 10);
    unsigned int lastIdx = startFrame;
    for (unsigned int i = startFrame + 1; i < total && i < lastFrame; i++) {
        auto current = video.extractSiftFeatures(i, 10);
        auto z = PDM(last, current);
        fprintf(out, "%.3f\n", z);
        fflush(out);
        //printf("%u %f\n", i, z);
        if (z > CutThreshold) {
            printf("%d %d\n", lastIdx, i - 1);
            fflush(stdout);
            //printf("%f\n", z);
            //video.play("video", lastIdx, i);
            //waitKey(0);
            lastIdx = i;
        }

        last = current;
    }

    fclose(out);

    return 0;
}