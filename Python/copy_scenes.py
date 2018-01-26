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


from shutil import copy2
import os
import sys
import datetime


def logmsg(*args):
    print '[{}] {}'.format(datetime.datetime.now(),
                           ' '.join([str(a) for a in args]))


try:
    csv_file = sys.argv[1]
except:
    logmsg("Argument missing: CSV file")
    exit()

try:
    data_path = sys.argv[2]
except:
    logmsg("Argument missing: Video dataset path")
    exit()

try:
    output_path = sys.argv[3]
except:
    logmsg("Argument missing: Output path")
    exit()

f = open(csv_file, 'r')
f.readline()  # Removing header
video_info = f.readlines()
f.close()

for info in video_info:
    info = ''.join(info.split()).split(';')
    video_folder = info[0]
    scene = info[1]
    ff = int(info[2])
    tf = int(info[3])
    l = int(info[4])

    ftemp = os.path.join(data_path, video_folder,
                         video_folder + '-{}.png')
    out_folder = os.path.join(output_path, scene)

    if not os.path.exists(out_folder):
        os.makedirs(out_folder)

    fcopied = 0
    ftocopy = range(ff, tf+1)

    for i in range(l):
        out_file = os.path.join(out_folder,
                                scene + '-{}.png'.format(i))
        copied_file = ftemp.format(ftocopy[i])

        if not os.path.exists(out_file):
            copy2(copied_file, out_file)

        fcopied += 1

        if fcopied % 50 == 0:
            logmsg('{} - {} of {} frames copied'.format(scene,
                                                        fcopied,
                                                        l))

