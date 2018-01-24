import plotly.plotly as py
from plotly.graph_objs import *
import os
import cPickle as cpkl
import numpy as np
import datetime
import sys

from sklearn.linear_model import Ridge
from sklearn.preprocessing import PolynomialFeatures
from sklearn.pipeline import make_pipeline

def logmsg(*args):
    print '[', datetime.datetime.now(), ']', ' '.join([str(a) for a in args])

# py.sign_in('username', 'api_key')
# py.sign_in(username='miller.horvath',
#            api_key='F1vaWxL8j2CFqLpJNmCQ')

try:
    results_path = sys.argv[1]
except:
    logmsg('Parameter missing: Results Path')
    exit()

try:
    output_path = sys.argv[2]
except:
    logmsg('Parameter missing: Output Path')
    exit()

try:
    frames_file = sys.argv[3]
except:
    logmsg('Parameter missing: Frame Info File')
    exit()

r_folder = []
for subfolder in os.listdir(results_path):
    if os.path.isdir(os.path.join(results_path, subfolder)):
        r_folder.append(subfolder)

frange = {}

f = open(frames_file, 'r')
f.readline()
data = f.readlines()
f.close()

for d in data:
    d = ''.join(d.split()).split(';')
    # video = d[0]
    scene = d[1]
    ff = int(d[2])
    tf = int(d[3])
    l = int(d[4])

    frange[scene] = (ff, tf, l)

ssf_dict_file = {}

for folder in r_folder:
    ssf_dict_file[folder] = os.path.join(results_path,
                                         folder,
                                         'results.cpkl')
# ssf_dict_file = sys.argv[1]

q_values_sup = np.arange(0.1, 1.0, 0.1)
q_values_sub = np.arange(1.1, 2.0, 0.1)
s_values = range(1, 102, 4)
s_values = s_values + range(150, 251, 50)

SIFT_LEN = len(s_values) * (len(q_values_sup) +
           len(q_values_sub) + 1)

ssf_dict = {}

to_remove = []

for folder in r_folder:
    f = open(ssf_dict_file[folder], 'rb')
    ssf_dict[folder] = cpkl.load(f)
    f.close()

    if len(ssf_dict[folder].keys()) < SIFT_LEN:
        to_remove.append(folder)
        logmsg('{} removed due to lack of results.'.format(folder))

for folder in to_remove:
    r_folder.remove(folder)

# for i in range(len(q_values_sup)):
#     q_values_sup[i] = round(q_values_sup[i], 2)
#
# for i in range(len(q_values_sub)):
#     q_values_sub[i] = round(q_values_sub[i], 2)

qsift_sup_q_matrix = []

for folder in r_folder:
    qsift_sup_q_matrix.append([])

    for s in sorted(s_values):
        ssf = -3000.0
        best_q = []

        for q in q_values_sup:
            q = round(q, 2)
            temp_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))

            if temp_ssf > ssf:
                ssf = temp_ssf
                best_q = [q]
            elif temp_ssf == ssf:
                best_q.append(q)

        qsift_sup_q_matrix[-1].append(best_q)

qsift_sub_q_matrix = []

for folder in r_folder:
    qsift_sub_q_matrix.append([])

    for s in sorted(s_values):
        ssf = -3000.0
        best_q = []

        for q in q_values_sub:
            q = round(q, 2)
            temp_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))

            if temp_ssf > ssf:
                ssf = temp_ssf
                best_q = [q]
            elif temp_ssf == ssf:
                best_q.append(q)

        qsift_sub_q_matrix[-1].append(best_q)

f = open(os.path.join(output_path, 'best_q_sup.csv'), 'w')

f.write('scene')

for s in s_values:
    f.write(';{}'.format(s))
f.write('\n')

for i in range(len(r_folder)):
    f.write('{}'.format(r_folder[i]))
    for j in range(len(s_values)):
        f.write(';{}'.format(qsift_sup_q_matrix[i][j]))
    f.write('\n')

f.close()

f = open(os.path.join(output_path, 'best_q_sub.csv'), 'w')

f.write('scene')

for s in s_values:
    f.write(';{}'.format(s))
f.write('\n')

for i in range(len(r_folder)):
    f.write('{}'.format(r_folder[i]))
    for j in range(len(s_values)):
        f.write(';{}'.format(qsift_sub_q_matrix[i][j]))
    f.write('\n')

f.close()
