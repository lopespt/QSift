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


import plotly.plotly as py
from plotly.graph_objs import *
import os
import cPickle as cpkl
import numpy as np

from sklearn.linear_model import Ridge
from sklearn.preprocessing import PolynomialFeatures
from sklearn.pipeline import make_pipeline

r_folder = 'Cambridge'
# r_folder = 'NYC'
# r_folder = 'Italy_f4382_t5280'
# r_folder = 'Italy_f11853_t13074'
# r_folder = 'SDakota'

ssf_dict_file = os.path.join('..', 'Results',
                             r_folder,
                             'results.cpkl')  # Test Path

ssf_surf_file = os.path.join('..', 'Results',
                             r_folder,
                             'resultsSurf.cpkl')  # Test Path
# ssf_dict_file = sys.argv[1]

f_name = r_folder + '-MaxTable.csv'
f_path = os.path.join('..', 'Results', 'Tables')

f = 315
t = 1897
# f = int(sys.argv[2])
# t = int(sys.argv[3])

frames_num = t - f + 1

f = open(ssf_dict_file, 'rb')
ssf_dict = cpkl.load(f)
f.close()

f = open(ssf_surf_file, 'rb')
ssf_surf = cpkl.load(f)
f.close()

q_values_sub = np.arange(0.1, 1.0, 0.1)
q_values_sup = np.arange(1.1, 2.0, 0.1)
h_values = range(200, 401, 100)
s_values = range(1, 102, 4)
s_values = s_values + range(150, 251, 50)

for i in range(len(q_values_sub)):
    q_values_sub[i] = round(q_values_sub[i], 2)

for i in range(len(q_values_sup)):
    q_values_sup[i] = round(q_values_sup[i], 2)

# for i in range(len(s_values)):
#     s_values[i] = round(s_values[i], 2)

sift_list = []
q = 0.99

for s in sorted(s_values):
    ssf = float(ssf_dict[s, q].split()[4].replace(',', '.'))
    frames = float((frames_num - 1) // s)
    nssf = (frames + ssf) / (2.0 * frames)

    sift_list.append(nssf)

surf_list = []
for s in sorted(s_values):
    nssf = 0.0
    best_h = 0

    for h in sorted(h_values):
        ssf = float(ssf_surf[s, h].split()[3].replace(',', '.'))
        frames = float((frames_num - 1) // s)
        temp_nssf = (frames + ssf) / (2.0 * frames)

        if temp_nssf > nssf:
            nssf = temp_nssf
            best_h = h

    surf_list.append((nssf, best_h))

qsift_sub_list = []
for s in sorted(s_values):
    nssf = 0.0
    best_q = 0.0

    for q in sorted(q_values_sub):
        ssf = float(ssf_dict[s, q].split()[4].replace(',', '.'))
        frames = float((frames_num - 1) // s)
        temp_nssf = (frames + ssf) / (2.0 * frames)

        if temp_nssf > nssf:
            nssf = temp_nssf
            best_q = q

    qsift_sub_list.append((nssf, best_q))

qsift_sup_list = []
for s in sorted(s_values):
    nssf = 0.0
    best_q = 0.0

    for q in sorted(q_values_sup):
        ssf = float(ssf_dict[s, q].split()[4].replace(',', '.'))
        frames = float((frames_num - 1) // s)
        temp_nssf = (frames + ssf) / (2.0 * frames)

        if temp_nssf > nssf:
            nssf = temp_nssf
            best_q = q

    qsift_sup_list.append((nssf, best_q))

header = "s;SURF nssf;H;SIFT nssf;Sub-Extensive qSIFT nssf;q;Super-Extensive qSIFT nssf;q\n"

if not os.path.exists(f_path):
    os.makedirs(f_path)

f = open(os.path.join(f_path, f_name), 'w')
f.write(header)

l_temp = '{};{};{};{};{};{};{};{}\n'
i = 0
for s in sorted(s_values):
    f.write(l_temp.format(s,
                          surf_list[i][0], surf_list[i][1],
                          sift_list[i],
                          qsift_sub_list[i][0], qsift_sub_list[i][1],
                          qsift_sup_list[i][0], qsift_sup_list[i][1]))
    i += 1

f.close()
