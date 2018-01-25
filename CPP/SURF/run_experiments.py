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
output_path = sys.argv[5]
MY_PATH = sys.argv[4]

if not os.path.exists(output_path):
    os.makedirs(output_path)

h_values = range(200, 401, 100)
s_values = range(1, 102, 4)
s_values = s_values + range(150, 251, 50)
s_values.reverse()

# video_path = "../../../Videos/scene2/"
video_path = sys.argv[1]
ff = int(sys.argv[2])
t = int(sys.argv[3])

print h_values
print s_values

try:
    f = open(os.path.join(output_path, 'resultsSurf.cpkl'), 'rb')
    results = cpkl.load(f)
    f.close()
except:
    results = {}

for s in s_values:
    for h in h_values:
        logmsg("s = {} | h = {}".format(s, h))
        if not (s, h) in results.keys():
            # if q == 0.99:
            out = subprocess.check_output(
                [os.path.join(MY_PATH, "build", "./OrdenacaoSurf"), video_path,
                 "{}".format(h), "-f{}".format(ff), "-t{}".format(t),
                 "-s{}".format(s)])
            print "{} {} {}".format(s, h, out)

            results[s, h] = out

            f = open(os.path.join(output_path, 'resultsSurf.cpkl'), 'wb')
            cpkl.dump(results, f)
            f.close()
        else:
            logmsg("Computed already")
