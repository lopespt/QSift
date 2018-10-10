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
import plotly.graph_objs as go
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


try:
    results_path = sys.argv[1]
except:
    logmsg('Parameter missing: Results Path')
    exit()

# try:
#     output_path = sys.argv[2]
# except:
#     logmsg('Parameter missing: Output Path')
#     exit()

try:
    frames_file = sys.argv[2]
except:
    logmsg('Parameter missing: Frame Info File')
    exit()

try:
    cat_lim = int(sys.argv[3])

    if cat_lim < 1:
        logmsg('{} categories is not valid!'.format(cat_lim))
        exit()
except:
    try:
        out = sys.argv[3]
        logmsg('Invalid parameter: Limit of Categories - ' +
               '{} is invalid.'.format(out))
        exit()
    except:
        logmsg('Parameter missing: Limit of Categories')
        exit()

# try:
#     output_path = sys.argv[4]
# except:
#     logmsg('Parameter missing: Output Path')
#     exit()

try:
    sub = bool(sys.argv[4])
except:
    logmsg('Parameter missing or invalid: Use sub-categories?')
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

cat_list = {}
cat_types = {}
cat_sub_types = {}

for d in data:
    d = ''.join(d.split()).split(';')
    # video = d[0]
    scene = d[1]
    ff = int(d[2])
    tf = int(d[3])
    l = int(d[4])

    cat_list[scene] = []

    for i in range(5, min(5 + cat_lim, len(d))):
        if d[i] != '':
            c = d[i].split('-')
            cat_list[scene].append(c)
            if c[0] in cat_types.keys():
                cat_types[c[0]] += 1
            else:
                cat_types[c[0]] = 1
            if (c[0], c[1]) in cat_sub_types.keys():
                cat_sub_types[(c[0], c[1])] += 1
            else:
                cat_sub_types[(c[0], c[1])] = 1

    # print scene, cat_list[scene]

    frange[scene] = (ff, tf, l)

# print cat_list
# print ""
#
# for key in sorted(cat_types.keys()):
#     print key, cat_types[key]
#
# print ""
#
# for key in sorted(cat_sub_types.keys()):
#     print key, cat_sub_types[key]

ssf_dict_file = {}
ssf_surf_file = {}

for folder in r_folder:
    ssf_dict_file[folder] = os.path.join(results_path,
                                         folder,
                                         'results.cpkl')

    ssf_surf_file[folder] = os.path.join(results_path,
                                         folder,
                                         'resultsSurf.cpkl')
# ssf_dict_file = sys.argv[1]

f_name = '{}_{}'

title = ('nssf Cumulative Regression | Max of Categories: {}' +
         '| Category: {} | Number of Scenes: {}')

ssf_dict = {}
ssf_surf = {}

q_values_sup = np.arange(0.1, 1.0, 0.1)
q_values_sub = np.arange(1.1, 2.0, 0.1)
h_values = range(200, 401, 100)
s_values = range(1, 102, 4)
s_values = s_values + range(150, 251, 50)

SURF_LEN = len(s_values) * len(h_values)
SIFT_LEN = len(s_values) * (len(q_values_sup) +
                            len(q_values_sub) + 1)

to_remove = []

for folder in r_folder:
    f = open(ssf_dict_file[folder], 'rb')
    ssf_dict[folder] = cpkl.load(f)
    f.close()

    f = open(ssf_surf_file[folder], 'rb')
    ssf_surf[folder] = cpkl.load(f)
    f.close()

    if (len(ssf_dict[folder].keys()) < SIFT_LEN or
                len(ssf_surf[folder].keys()) < SURF_LEN):
        to_remove.append(folder)
        logmsg('{} removed due to lack of results.'.format(folder))

for folder in to_remove:
    r_folder.remove(folder)

# print r_folder

for i in range(len(q_values_sup)):
    q_values_sup[i] = round(q_values_sup[i], 2)

for i in range(len(q_values_sub)):
    q_values_sub[i] = round(q_values_sub[i], 2)

# for i in range(len(s_values)):
#     s_values[i] = round(s_values[i], 2)

sift_list = {}
q = 0.99

if sub == True:
    p_keys = cat_types.keys() + cat_sub_types.keys()
else:
    p_keys = cat_types.keys()

for cat in p_keys:
    sift_list[cat] = []

for s in sorted(s_values):
    for cat in p_keys:
        sift_list[cat].append(0.0)

    for folder in r_folder:
        ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
        frames = float((frange[folder][2] - 1) // s)
        nssf = (frames + ssf) / (2.0 * frames)

        for c in cat_list[folder]:
            sift_list[c[0]][-1] += nssf
            sift_list[(c[0], c[1])][-1] += nssf

    for c in cat_types.keys():
        sift_list[c][-1] /= cat_types[c]

    for c in cat_sub_types.keys():
        sift_list[c][-1] /= cat_sub_types[c]

surf_list = {}

for cat in p_keys:
    surf_list[cat] = []

for s in sorted(s_values):
    for cat in p_keys:
        surf_list[cat].append(0.0)

    for folder in r_folder:
        ssf = 0.0

        for h in h_values:
            temp_ssf = float(ssf_surf[folder][s, h].split()[3].replace(',', '.'))

            if temp_ssf > ssf:
                ssf = temp_ssf

        frames = float((frange[folder][2] - 1) // s)
        nssf = (frames + ssf) / (2.0 * frames)

        for c in cat_list[folder]:
            surf_list[c[0]][-1] += nssf
            surf_list[(c[0], c[1])][-1] += nssf

    for c in cat_types.keys():
        surf_list[c][-1] /= cat_types[c]

    for c in cat_sub_types.keys():
        surf_list[c][-1] /= cat_sub_types[c]


qsift_sup_list = {}

for cat in p_keys:
    qsift_sup_list[cat] = []

for s in sorted(s_values):
    for cat in p_keys:
        qsift_sup_list[cat].append(0.0)

    for folder in r_folder:
        ssf = 0.0

        for q in q_values_sup:
            temp_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))

            if temp_ssf > ssf:
                ssf = temp_ssf

        frames = float((frange[folder][2] - 1) // s)
        nssf = (frames + ssf) / (2.0 * frames)

        for c in cat_list[folder]:
            qsift_sup_list[c[0]][-1] += nssf
            qsift_sup_list[(c[0], c[1])][-1] += nssf

    for c in cat_types.keys():
        qsift_sup_list[c][-1] /= cat_types[c]

    for c in cat_sub_types.keys():
        qsift_sup_list[c][-1] /= cat_sub_types[c]

qsift_sub_list = {}

for cat in p_keys:
    qsift_sub_list[cat] = []

for s in sorted(s_values):
    for cat in p_keys:
        qsift_sub_list[cat].append(0.0)

    for folder in r_folder:
        ssf = 0.0

        for q in q_values_sub:
            temp_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))

            if temp_ssf > ssf:
                ssf = temp_ssf

        frames = float((frange[folder][2] - 1) // s)
        nssf = (frames + ssf) / (2.0 * frames)

        for c in cat_list[folder]:
            qsift_sub_list[c[0]][-1] += nssf
            qsift_sub_list[(c[0], c[1])][-1] += nssf

    for c in cat_types.keys():
        qsift_sub_list[c][-1] /= cat_types[c]

    for c in cat_sub_types.keys():
        qsift_sub_list[c][-1] /= cat_sub_types[c]

x_plot = range(0, 260, 10)
x_plot = np.array(x_plot)

s_values = np.array(s_values)
X = s_values[:, np.newaxis]
x_plot = x_plot[:, np.newaxis]

sift_trace = {}

for c in p_keys:
    # print c
    model = make_pipeline(PolynomialFeatures(4), Ridge())
    model.fit(X, sift_list[c])

    y_plot = model.predict(x_plot)

    sift_trace[c] = {
        "x": x_plot,
        "y": y_plot,
        "mode": "lines+markers",
        "name": "SIFT",
        "type": "scatter"
    }

# model = make_pipeline(PolynomialFeatures(4), Ridge())
# model.fit(X, best_qsift_list)
# y_plot = model.predict(x_plot)
#
# best_qsift_trace = {
#     "x": x_plot,
#     "y": y_plot,
#     "mode": "lines+markers",
#     "name": "qSIFT q=0.6",
#     "type": "scatter"
# }

surf_trace = {}

# for c in p_keys:
for c in cat_types.keys():
    model = make_pipeline(PolynomialFeatures(4), Ridge())
    model.fit(X, surf_list[c])
    y_plot = model.predict(x_plot)

    surf_trace[c] = {
        "x": x_plot,
        "y": y_plot,
        "mode": "lines+markers",
        "name": "SURF",
        "type": "scatter"
    }

qsift_sup_trace = {}

# for c in p_keys:
for c in cat_types.keys():
    model = make_pipeline(PolynomialFeatures(4), Ridge())
    model.fit(X, qsift_sup_list[c])
    y_plot = model.predict(x_plot)

    qsift_sup_trace[c] = {
        "x": x_plot,
        "y": y_plot,
        "mode": "lines+markers",
        "name": "Super-Extensive qSIFT",
        "type": "scatter"
    }

qsift_sub_trace = {}

for c in p_keys:
    model = make_pipeline(PolynomialFeatures(4), Ridge())
    model.fit(X, qsift_sub_list[c])
    y_plot = model.predict(x_plot)

    qsift_sub_trace[c] = {
        "x": x_plot,
        "y": y_plot,
        "mode": "lines+markers",
        "name": "Sub-Extensive qSIFT",
        "type": "scatter"
    }

for c in p_keys:
    data = Data([sift_trace[c], surf_trace[c], qsift_sup_trace[c],
                 qsift_sub_trace[c]])

    if type(c) == type(""):
        c_out = c
        scene_count = cat_types[c]
    else:
        c_out = '-'.join(c)
        scene_count = cat_sub_types[c]

    layout = {
        "hovermode": "closest",
        "legend": {
            "x": 0.625,
            "y": 0.2,
            "font": {"size": 14}
        },
        "xaxis": {
            "autorange": False,
            "nticks": 0,
            "range": [-5, 255],
            "showline": False,
            "showticklabels": True,
            "tickmode": "auto",
            "title": "Interval Between Consecutive Frames",
            "type": "linear"
        },
        "yaxis": {
            "autorange": False,
            "range": [-0.05, 1.05],
            "showline": False,
            "title": "nssf",
            "type": "linear"
        }
    }

    fig = Figure(data=data, layout=layout)

    # # You must sign in into plotly.com
    # py.sign_in('username', 'api_key')
    py.plot(fig, filename=f_name.format(c_out, cat_lim))

    # out_file = os.path.join(output_path,
    #                         f_name.format(cat_lim, c_out))
    #
    # py.image.save_as(fig, filename=out_file)
