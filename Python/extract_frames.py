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
    video_file = sys.argv[1]
except:
    logmsg("Argument missing: Video file path")

try:
    cap = cv2.VideoCapture(video_file)
except:
    logmsg("couldn't open {}".format(video_file))

try:
    output_path = sys.argv[2]
except:
    logmsg("Argument missing: Output path")

if not os.path.exists(output_path):
    print output_path, "doesn't exist"
    answer = raw_input('Would you like to create it? [y/n]: ')

    if answer == 'y':
        os.makedirs(output_path)
    else:
        exit()

out_temp = os.path.join(output_path, "{}-{}.png")

name = os.path.splitext(os.path.basename(video_file))[0]

try:
    start = sys.argv[3]
except:
    start = 0

try:
    end = sys.argv[3]
except:
    end = int(cap.get(cv2.CAP_PROP_FRAME_COUNT) - 1)

for i in range(start, end+1):
    cap.set(cv2.CAP_PROP_POS_FRAMES, i);
    success, image = cap.read()

    cv2.imwrite(out_temp.format(name, i), image)

    if (i - start) % ((end - start) // 20) == 0:
        # logmsg("{}% of frames converted into images.".format(
        #     round(float(i - start) / float(end - start) * 100)))
        try:
            logmsg("{} of {} frames converted into images.".format(i - start, end - start))
        except:
            pass

# def extract_frame(cap, i, name):
#     cap.set(cv2.CAP_PROP_POS_FRAMES, i);
#     success, image = cap.read()
#
#     cv2.imwrite(out_temp.format(name, i), image)
#
#     if (i - start) % ((end - start) // 20) == 0:
#         logmsg("Approximately {}% of frames converted into images.".format(
#             round(float(i - start) / float(end - start) * 100)))
#         # logmsg("{} of {} frames converted into images.".format(i - start, end - start))
#
#     return None
#
#
# Parallel(n_jobs=-1, verbose=0, backend="threading") \
#     (delayed(extract_frame)(cap, i, name)
#      for i in range(start, end + 1))
