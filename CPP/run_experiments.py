import sys
import subprocess
import numpy as np
import datetime
import cPickle as cpkl
import os


def logmsg(*args):
    print '[', datetime.datetime.now(), ']', ' '.join([str(a) for a in args])


# output_path = "/scratch-global/paulo.sergio/miller"
# MY_PATH = "/home/paulo.sergio/miller/qSIFT/CPP/OpenSift"
output_path = sys.argv[3]
MY_PATH = sys.argv[2]

if not os.path.exists(output_path):
    os.makedirs(output_path)

q_values = np.arange(0.1, 2.0, 0.1)
h_values = range(200, 401, 100)
s_values = range(1, 102, 4)
s_values = s_values + range(150, 251, 50)
s_values.reverse()

for i in range(len(q_values)):
    q_values[i] = round(q_values[i], 1)

if q_values[9] == 1.0:
    q_values[9] = 0.99

b = 0.5

video_path = sys.argv[1]

print h_values
print s_values

for subfolder in os.listdir(video_path):
    scene_path = os.path.join(video_path, subfolder, '')
    logmsg('scene path: {}'.format(scene_path))

    try:
        f = open(os.path.join(output_path, subfolder,
                              'results.cpkl'), 'rb')
        results = cpkl.load(f)
        f.close()
    except:
        results = {}

    try:
        f = open(os.path.join(output_path, subfolder,
                              'resultsSurf.cpkl'), 'rb')
        resultsSurf = cpkl.load(f)
        f.close()
    except:
        resultsSurf = {}

    if not os.path.exists(os.path.join(output_path, subfolder)):
        os.makedirs(os.path.join(output_path, subfolder))

    for s in s_values:
        for h in h_values:
            logmsg("s = {} | h = {}".format(s, h))
            if not (s, h) in resultsSurf.keys():
                # if q == 0.99:
                out = subprocess.check_output(
                    [os.path.join(MY_PATH, "SURF", "build",
                                  "./OrdenacaoSurf"),
                     scene_path, "{}".format(h), "-s{}".format(s)])
                print "{} {} {}".format(s, h, out)

                resultsSurf[s, h] = out

                f = open(os.path.join(output_path, subfolder,
                                      'resultsSurf.cpkl'),
                         'wb')
                cpkl.dump(resultsSurf, f)
                f.close()
            else:
                logmsg("Computed already")

        for q in q_values:
            logmsg("s = {} | q = {}".format(s, q))
            if not (s, q) in results.keys():
                out = subprocess.check_output(
                    [os.path.join(MY_PATH, "OpenSift", "build",
                                  "./OrdenacaoSift2"),
                     scene_path, "{}".format(q), "{}".format(b),
                     "-s{}".format(s)])
                print "{} {} {}".format(s, q, out)

                results[s, q] = out

                f = open(os.path.join(output_path, subfolder,
                                      'results.cpkl'), 'wb')
                cpkl.dump(results, f)
                f.close()
            else:
                logmsg("Computed already")
