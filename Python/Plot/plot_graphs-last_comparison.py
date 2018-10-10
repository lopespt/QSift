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
# import plotly.graph_objs as go
import os
import cPickle as cpkl
import numpy as np
import datetime
import sys
import math

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
ssf_surf_file = {}

for folder in r_folder:
    ssf_dict_file[folder] = os.path.join(results_path,
                                         folder,
                                         'results.cpkl')

    ssf_surf_file[folder] = os.path.join(results_path,
                                         folder,
                                         'resultsSurf.cpkl')
# ssf_dict_file = sys.argv[1]

f_name = 'last_nssf'

# title = "nssf Cumulative Regression"

ssf_dict = {}
ssf_surf = {}

for folder in r_folder:
    f = open(ssf_dict_file[folder], 'rb')
    ssf_dict[folder] = cpkl.load(f)
    f.close()

    f = open(ssf_surf_file[folder], 'rb')
    ssf_surf[folder] = cpkl.load(f)
    f.close()

# q_values = [0.1, 0.3, 1.1]
q_values = [0.3, 1.1]
h_values = [300]
s_values = range(1, 102, 4)
s_values = s_values + range(150, 251, 50)

sift_list = []
q = 0.99

for s in sorted(s_values):
    sift_list.append(0.0)

    for folder in r_folder:
        b_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
        frames = float((frange[folder][2] - 1) // s)
        b_nssf = (frames + b_ssf) / (2.0 * frames)

        sift_list[-1] += b_nssf
    sift_list[-1] /= len(r_folder)

surf_list = {}

for h in h_values:
    surf_list[h] = []

for s in sorted(s_values):
    for h in h_values:
        surf_list[h].append(0.0)

        for folder in r_folder:
            ssf = float(ssf_surf[folder][s, h].split()[3].replace(',', '.'))
            frames = float((frange[folder][2] - 1) // s)
            nssf = (frames + ssf) / (2.0 * frames)

            surf_list[h][-1] += nssf
        surf_list[h][-1] /= len(r_folder)

q_sift_list = {}

for q in q_values:
    q_sift_list[q] = []

for s in sorted(s_values):
    for q in q_values:
        q_sift_list[q].append(0.0)

        for folder in r_folder:
            ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
            frames = float((frange[folder][2] - 1) // s)
            nssf = (frames + ssf) / (2.0 * frames)

            q_sift_list[q][-1] += nssf
        q_sift_list[q][-1] /= len(r_folder)

x_plot = range(0, 260, 10)
x_plot = np.array(x_plot)

s_values = np.array(s_values)
X = s_values[:, np.newaxis]
x_plot = x_plot[:, np.newaxis]

model = make_pipeline(PolynomialFeatures(4), Ridge())
model.fit(X, sift_list)
y_plot = model.predict(x_plot)

sift_trace = {
    "x": x_plot,
    "y": y_plot,
    "line": {"shape": "spline"},
    "mode": "lines+markers",
    "name": "SIFT",
    "type": "scatter",
    "marker": {
        "symbol": "circle-open",
        "size": 12
    }
}

qsift_trace = []

for q in q_values:
    model = make_pipeline(PolynomialFeatures(4), Ridge())
    model.fit(X, q_sift_list[q])
    y_plot = model.predict(x_plot)

    qsift_trace.append({
        "x": x_plot,
        "y": y_plot,
        "line": {"shape": "spline"},
        "mode": "lines+markers",
        "name": "qSIFT (q = {})".format(q),
        "type": "scatter",
        "marker": {
            "symbol": "diamond-open",
            "size": 12
        }
    })

surf_trace = []

for h in h_values:
    model = make_pipeline(PolynomialFeatures(4), Ridge())
    model.fit(X, surf_list[h])
    y_plot = model.predict(x_plot)

    surf_trace.append({
        "x": x_plot,
        "y": y_plot,
        "line": {"shape": "spline"},
        "mode": "lines+markers",
        "name": "SURF (H = {})".format(h),
        "type": "scatter",
        "marker": {
            "symbol": "triangle-down-open",
            "size": 12
        }
    })

data = Data(surf_trace + qsift_trace + [sift_trace])
# data = Data(qsift_trace)

layout = {
    "dragmode": "zoom",
    "font": {"size": 16},
    "hovermode": False,
    "legend": {
        "x": 0.6,
        "y": 0.95,
        "font": {"size": 20}
    },
    "margin": {
        "r": 0,
        "t": 0,
        "b": 50,
        "l": 60
    },
    "xaxis": {
        "autorange": False,
        "fixedrange": True,
        "nticks": 0,
        "range": [-5, 255],
        "showline": False,
        "showticklabels": True,
        "tickmode": "auto",
        "title": "s",
        "titlefont": {"size": 20},
        "type": "linear"
    },
    "yaxis": {
        "autorange": True,
        "showline": False,
        "title": "nssf",
        "titlefont": {"size": 20},
        "type": "linear"
    }
}

fig = Figure(data=data, layout=layout)

# # You must sign in into plotly.com
# py.sign_in('username', 'api_key')
py.plot(fig, filename=f_name)

layout = {
    "dragmode": "zoom",
    "font": {"size": 16},
    "hovermode": False,
    "boxmode": "group",
    "margin": {
        "r": 0,
        "t": 0,
        "b": 50,
        "l": 60
    },
    "xaxis": {
        "autorange": False,
        "fixedrange": True,
        "nticks": 0,
        "range": [-5, 255],
        "showline": False,
        "showticklabels": True,
        "tickmode": "auto",
        "title": "s",
        "titlefont": {"size": 20},
        "type": "linear"
    },
    "yaxis": {
        "autorange": True,
        "showline": False,
        "title": "nssf",
        "titlefont": {"size": 20},
        "type": "linear"
    }
}