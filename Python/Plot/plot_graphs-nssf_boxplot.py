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
import datetime
import sys


def logmsg(*args):
    print '[', datetime.datetime.now(), ']', ' '.join([str(a) for a in args])


results_path = 0

try:
    results_path = sys.argv[1]
except:
    logmsg('Parameter missing: Results Path')
    exit()

frames_file = 0
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

f_name = 'nssf_box'

title = "nssf Cumulative Regression"

ssf_dict = {}
ssf_surf = {}

q_values_sup =[0.3]
# q_values_sup = np.arange(0.1, 1.0, 0.1)
q_values_sub = [1.1]
# q_values_sub = np.arange(1.1, 2.0, 0.1)
h_values = [300]
# h_values = range(200, 401, 100)
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

    if len(ssf_dict[folder].keys()) < SIFT_LEN or \
                    len(ssf_surf[folder].keys()) < SURF_LEN:
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

surf_list = {}
surf_x = {}

for h in h_values:
    surf_list[h] = []
    surf_x[h] = []

    for s in sorted(s_values):
        for folder in r_folder:
            frames = float((frange[folder][2] - 1) // s)

            ssf = float(ssf_surf[folder][s, h].split()[3].replace(',', '.'))
            surf_list[h].append((frames + ssf) / (2.0 * frames))
            surf_x[h].append('{}'.format(s))

qsift_sup_list = {}
qsift_sup_x = {}

for q in q_values_sup:
    qsift_sup_list[q] = []
    qsift_sup_x[q] = []

    for s in sorted(s_values):
        for folder in r_folder:
            frames = float((frange[folder][2] - 1) // s)

            ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
            qsift_sup_list[q].append((frames + ssf) / (2.0 * frames))
            qsift_sup_x[q].append('{}'.format(s))

qsift_sub_list = {}
qsift_sub_x = {}

for q in q_values_sub:
    qsift_sub_list[q] = []
    qsift_sub_x[q] = []

    for s in sorted(s_values):
        for folder in r_folder:
            frames = float((frange[folder][2] - 1) // s)

            ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
            qsift_sub_list[q].append((frames + ssf) / (2.0 * frames))
            qsift_sub_x[q].append('{}'.format(s))

sift_list = {}
sift_x = {}

for q in [0.99]:
    sift_list[q] = []
    sift_x[q] = []

    for s in sorted(s_values):
        for folder in r_folder:
            frames = float((frange[folder][2] - 1) // s)

            ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
            sift_list[q].append((frames + ssf) / (2.0 * frames))
            sift_x[q].append('{}'.format(s))

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
        "autorange": True,
        # "fixedrange": True,
        # "nticks": 0,
        # "range": [-5, 255],
        # "showline": False,
        # "showticklabels": True,
        # "tickmode": "auto",
        "title": "s",
        "titlefont": {"size": 20},
        "type": "category",
        # "categoryorder": "category ascending"
    },
    "yaxis": {
        "autorange": False,
        "fixedrange": True,
        "range": [-0.01, 1.01],
        "showline": False,
        "title": "nssf",
        "titlefont": {"size": 20},
        "type": "linear"
    }
}


# # You must sign in into plotly.com
# py.sign_in('username', 'api_key')

for h in h_values:
    surf_trace = {
        "y": surf_list[h],
        "x": surf_x[h],
        "fillcolor": "rgba(31, 119, 180, 0.2)",
        "line": {
            "color": "rgba(31, 119, 180)",
            "width": 2
        },
        "marker": {"color": "#1F77B4"},
        "name": "SURF",
        "type": "box",
    }

    data = Data([surf_trace])
    fig = go.Figure(data=data, layout=layout)
    py.plot(fig, filename=f_name + '_surf')

for q in q_values_sup:
    qsift_sup_trace = {
        "y": qsift_sup_list[q],
        "x": qsift_sup_x[q],
        "fillcolor": "rgba(255, 127, 14, 0.2)",
        "line": {
            "color": "rgba(255, 127, 14)",
            "width": 2
        },
        "marker": {"color": "#FF7F0E"},
        "name": "Super-Extensive q-SIFT",
        "type": "box",
    }

    data = Data([qsift_sup_trace])
    fig = go.Figure(data=data, layout=layout)
    py.plot(fig, filename=f_name+'_qsift_sup')

for q in q_values_sub:
    qsift_sub_trace = {
        "y": qsift_sub_list[q],
        "x": qsift_sub_x[q],
        "fillcolor": "rgba(44, 160, 44, 0.2)",
        "line": {
            "color": "rgba(44, 160, 44)",
            "width": 2
        },
        "marker": {"color": "#2CA02C"},
        "name": "Sub-Extensive q-SIFT",
        "type": "box",
    }

    data = Data([qsift_sub_trace])
    fig = go.Figure(data=data, layout=layout)
    py.plot(fig, filename=f_name+'_qsift_sub')

for q in [0.99]:
    sift_trace = {
        "y": sift_list[q],
        "x": sift_x[q],
        "fillcolor": "#rgba(214, 39, 40, 0.2)",
        "line": {
            "color": "#rgba(214, 39, 40)",
            "width": 2
        },
        "marker": {"color": "#D62728"},
        "name": "SIFT",
        "type": "box",
    }

    data = Data([sift_trace])
    fig = go.Figure(data=data, layout=layout)
    py.plot(fig, filename=f_name+'_sift')


