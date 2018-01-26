import cv2
# ------------------------------------------------------------------------
# Project: q-SIFT
# Authors:  Guilherme Wachs-Lopes <gwachs@fei.edu.br>
#           Miller Horvath <mhorvath@fei.edu.br>
#
# This is an open source code created for research and academic purposes.
# If you have used this code in you research, please cite:
#
#   P. S. Rodrigues, G. A. Wachs-Lopes, G. Antonio Giraldi and M. Horvath,
#   in press. q-SIFT: A Strategy Based on Non-Extensive Statistic to
#   Improve SIFT Algorithm Under Severe Conditions. Pattern Recognition.
#
# ------------------------------------------------------------------------


import sys
import os
import datetime
from joblib import Parallel, delayed


def logmsg(*args):
    print '[{}] {}'.format(datetime.datetime.now(), ' '.join([str(a) for a in args]))


try:
    video_path = sys.argv[1]
except:
    logmsg("Argument missing: Video file path")
    exit()

try:
    output_path = sys.argv[2]
except:
    logmsg("Argument missing: Output path")
    exit()

if not os.path.exists(output_path):
    print output_path, "doesn't exist"
    answer = raw_input('Would you like to create it? [y/n]: ')

    if answer == 'y':
        os.makedirs(output_path)
    else:
        exit()

file_list = os.listdir(video_path)

for path in file_list:
    video_file = os.path.join(video_path, path)
    if os.path.isfile(video_file):
        name = os.path.splitext(os.path.basename(path))[0]
        cap = cv2.VideoCapture(video_file)
        start = 0
        end = int(cap.get(cv2.CAP_PROP_FRAME_COUNT) - 1)

        if not os.path.exists(os.path.join(output_path, name)):
            os.makedirs(os.path.join(output_path, name))

        out_temp = os.path.join(output_path, name, "{}-{}.png")
        # print 'test 1'
        for i in range(start, end+1):
            # print 'test 2'
            if not os.path.exists(out_temp.format(name, i)):
                cap.set(cv2.CAP_PROP_POS_FRAMES, i);
                # print 'test 3'
                success, image = cap.read()
                # print 'test 4'
                cv2.imwrite(out_temp.format(name, i), image)
                # print 'test 5'
            try:
                if (i - start) % ((end - start) // 20) == 0:
                    logmsg("{} - {} of {} frames converted into images.".format(name, i - start, end - start))
            except:
                pass
            # exit()
