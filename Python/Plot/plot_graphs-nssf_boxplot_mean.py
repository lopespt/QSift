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


results_path = 0

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

f_name = 'nssf_box2'

title = "nssf Cumulative Regression"

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

surf_list = []
surf_x = []

for s in sorted(s_values):
    for folder in r_folder:
        nssf = 0.0
        frames = float((frange[folder][2] - 1) // s)

        for h in h_values:
            ssf = float(ssf_surf[folder][s, h].split()[3].replace(',', '.'))
            nssf += (frames + ssf) / (2.0 * frames)

        nssf /= len(h_values)
        surf_list.append(nssf)
        surf_x.append(s)

qsift_sup_list = []
qsift_sup_x = []

for s in sorted(s_values):
    for folder in r_folder:
        nssf = 0.0
        frames = float((frange[folder][2] - 1) // s)

        for q in q_values_sup:
            ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
            nssf += (frames + ssf) / (2.0 * frames)

        nssf /= len(q_values_sup)
        qsift_sup_list.append(nssf)
        qsift_sup_x.append(s)

qsift_sub_list = []
qsift_sub_x = []

for s in sorted(s_values):
    for folder in r_folder:
        nssf = 0.0
        frames = float((frange[folder][2] - 1) // s)

        for q in q_values_sub:
            ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
            nssf += (frames + ssf) / (2.0 * frames)

        nssf /= len(q_values_sub)
        qsift_sub_list.append(nssf)
        qsift_sub_x.append(s)

surf_trace = go.Box(
    y=surf_list,
    x=surf_x,
    name='SURF',
    marker=dict(color='#1F77B4')
)

qsift_sup_trace = go.Box(
    y=qsift_sup_list,
    x=qsift_sup_x,
    name='Super-Extensive q-SIFT',
    marker=dict(color='#FF7F0E')
)

qsift_sub_trace = go.Box(
    y=qsift_sub_list,
    x=qsift_sub_x,
    name='Sub-Extensive q-SIFT',
    marker=dict(color='#2CA02C')
)

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

data = Data([qsift_sub_trace])
fig = go.Figure(data=data, layout=layout)

py.sign_in(username='miller.horvath',
           api_key='F1vaWxL8j2CFqLpJNmCQ')
py.plot(fig, filename=f_name+'_qsift_sub')

data = Data([qsift_sup_trace])
fig = go.Figure(data=data, layout=layout)

py.sign_in(username='miller.horvath',
           api_key='F1vaWxL8j2CFqLpJNmCQ')
py.plot(fig, filename=f_name+'_qsift_sup')

data = Data([surf_trace])
fig = go.Figure(data=data, layout=layout)

py.sign_in(username='miller.horvath',
           api_key='F1vaWxL8j2CFqLpJNmCQ')
py.plot(fig, filename=f_name+'_surf')
