import plotly.plotly as py
from plotly.graph_objs import *
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

f_name = 'best_nssf'

title = "b_nssf Cumulative Regression"

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

surf_list = []

best_h_acc = [0 for i in range(3)]
worst_h_acc = [0 for i in range(3)]
best_h_rmse = np.array([0.0 for i in range(3)])
best_h_mae = np.array([0.0 for i in range(3)])
surf_list_best = []
surf_list_worst = []
surf_worst_and_best_are_equal = 0.0

for s in sorted(s_values):
    surf_list_best.append(0.0)
    surf_list_worst.append(0.0)

    for folder in r_folder:
        b_ssf = -100000.0
        w_ssf = 100000.0

        for h in h_values:
            temp_ssf = float(ssf_surf[folder][s, h].split()[3].replace(',', '.'))

            if temp_ssf > b_ssf:
                b_ssf = temp_ssf

            if temp_ssf < w_ssf:
                w_ssf = temp_ssf

        frames = float((frange[folder][2] - 1) // s)
        b_nssf = (frames + b_ssf) / (2.0 * frames)
        w_nssf = (frames + w_ssf) / (2.0 * frames)

        for h in h_values:
            temp_ssf = float(ssf_surf[folder][s, h].split()[3].replace(',', '.'))
            temp_nssf = (frames + temp_ssf) / (2.0 * frames)
            idx = (h - 200) / 100

            best_h_rmse[idx] += (b_nssf - temp_nssf) * (b_nssf - temp_nssf)
            best_h_mae[idx] += abs(b_nssf - temp_nssf)

        if w_ssf == b_ssf:
            surf_worst_and_best_are_equal += 1.0
        else:
            for h in h_values:
                temp_ssf = float(ssf_surf[folder][s, h].split()[3].replace(',', '.'))
                idx = (h - 200) / 100

                if temp_ssf == w_ssf:
                    worst_h_acc[idx] += 1
                if temp_ssf == b_ssf:
                    best_h_acc[idx] += 1

        surf_list_best[-1] += b_nssf
        surf_list_worst[-1] += w_nssf
    surf_list_best[-1] /= len(r_folder)
    surf_list_worst[-1] /= len(r_folder)

for s in sorted(s_values):
    surf_list.append(0.0)

    for folder in r_folder:
        b_ssf = -100000.0

        for h in h_values:
            temp_ssf = float(ssf_surf[folder][s, h].split()[3].replace(',', '.'))

            if temp_ssf > b_ssf:
                b_ssf = temp_ssf

        frames = float((frange[folder][2] - 1) // s)
        b_nssf = (frames + b_ssf) / (2.0 * frames)
        surf_list[-1] += b_nssf
    surf_list[-1] /= len(r_folder)

best_q_acc = [0 for i in range(19)]
worst_q_acc = [0 for i in range(19)]
best_q_rmse = np.array([0.0 for i in range(19)])
best_q_mae = np.array([0.0 for i in range(19)])
qsift_sup_list_best = []
qsift_sup_list_worst = []
sup_worst_and_best_are_equal = 0.0

for s in sorted(s_values):
    qsift_sup_list_best.append(0.0)
    qsift_sup_list_worst.append(0.0)

    for folder in r_folder:
        b_ssf = -100000.0
        w_ssf = 100000.0

        for q in q_values_sup:
            temp_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))

            if temp_ssf > b_ssf:
                b_ssf = temp_ssf

            if temp_ssf < w_ssf:
                w_ssf = temp_ssf

        frames = float((frange[folder][2] - 1) // s)
        b_nssf = (frames + b_ssf) / (2.0 * frames)
        w_nssf = (frames + w_ssf) / (2.0 * frames)

        for q in q_values_sup:
            temp_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
            temp_nssf = (frames + temp_ssf) / (2.0 * frames)
            idx = int(q * 10) - 1

            best_q_rmse[idx] += (b_nssf - temp_nssf) * (b_nssf - temp_nssf)
            best_q_mae[idx] += abs(b_nssf - temp_nssf)

        if w_ssf == b_ssf:
            sup_worst_and_best_are_equal += 1.0
        else:
            for q in q_values_sup:
                temp_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
                idx = int(q * 10) - 1

                if temp_ssf == w_ssf:
                    worst_q_acc[idx] += 1
                if temp_ssf == b_ssf:
                    best_q_acc[idx] += 1

        qsift_sup_list_best[-1] += b_nssf
        qsift_sup_list_worst[-1] += w_nssf
    qsift_sup_list_best[-1] /= len(r_folder)
    qsift_sup_list_worst[-1] /= len(r_folder)

qsift_sub_list_best = []
qsift_sub_list_worst = []
sub_worst_and_best_are_equal = 0.0

for s in sorted(s_values):
    qsift_sub_list_best.append(0.0)
    qsift_sub_list_worst.append(0.0)

    for folder in r_folder:
        b_ssf = -100000.0
        w_ssf = 100000.0
        frames = float((frange[folder][2] - 1) // s)

        for q in q_values_sub:
            temp_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))

            if temp_ssf > b_ssf:
                b_ssf = temp_ssf

            if temp_ssf < w_ssf:
                w_ssf = temp_ssf

        b_nssf = (frames + b_ssf) / (2.0 * frames)
        w_nssf = (frames + w_ssf) / (2.0 * frames)

        for q in q_values_sub:
            temp_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
            temp_nssf = (frames + temp_ssf) / (2.0 * frames)
            idx = int(q * 10) - 1

            best_q_rmse[idx] += (b_nssf - temp_nssf) * (b_nssf - temp_nssf)
            best_q_mae[idx] += abs(b_nssf - temp_nssf)

        if w_ssf == b_ssf:
            sub_worst_and_best_are_equal += 1.0
        else:
            for q in q_values_sub:
                temp_ssf = float(ssf_dict[folder][s, q].split()[4].replace(',', '.'))
                idx = int(q * 10) - 1

                if temp_ssf == b_ssf:
                    best_q_acc[idx] += 1

                if temp_ssf == w_ssf:
                    worst_q_acc[idx] += 1

        qsift_sub_list_best[-1] += b_nssf
        qsift_sub_list_worst[-1] += w_nssf
    qsift_sub_list_best[-1] /= len(r_folder)
    qsift_sub_list_worst[-1] /= len(r_folder)

er_size = len(s_values) * len(r_folder)

best_q_mae = best_q_mae / er_size
best_q_rmse = np.sqrt(best_q_rmse / er_size)
best_h_mae = best_h_mae / er_size
best_h_rmse = np.sqrt(best_h_rmse / er_size)
# print best_q_mae
# print best_q_rmse
# exit()

# print best_q_acc
# exit()

# out_label = '{} & \\centering {} & \\centering {} & {} & {} \\\\'
out_label = '{} & {} & {} & {} & {} \\\\'

for i in range(len(q_values_sup)):
    idx = (float(i) + 1) / 10
    best_size = er_size - sup_worst_and_best_are_equal
    percent_best = float(best_q_acc[i]) / float(best_size)
    percent_worst = float(worst_q_acc[i]) / float(best_size)
    print out_label.format(idx,
                           round(percent_best, 3),
                           round(percent_worst, 3),
                           round(best_q_mae[i], 3),
                           round(best_q_rmse[i], 3))

for i in range(10, 10 + len(q_values_sub)):
    idx = (float(i) + 1) / 10
    best_size = er_size - sub_worst_and_best_are_equal
    percent_best = float(best_q_acc[i]) / float(best_size)
    percent_worst = float(worst_q_acc[i]) / float(best_size)
    print out_label.format(idx,
                           round(percent_best, 3),
                           round(percent_worst, 3),
                           round(best_q_mae[i], 3),
                           round(best_q_rmse[i], 3))

for i in range(len(h_values)):
    idx = 200 + i * 100
    best_size = er_size - surf_worst_and_best_are_equal
    percent_best = float(best_h_acc[i]) / float(best_size)
    percent_worst = float(worst_h_acc[i]) / float(best_size)
    print out_label.format(idx,
                           round(percent_best, 3),
                           round(percent_worst, 3),
                           round(best_h_mae[i], 3),
                           round(best_h_rmse[i], 3))

print round(sup_worst_and_best_are_equal / float(er_size), 3)
print round(sub_worst_and_best_are_equal / float(er_size), 3)
print round(surf_worst_and_best_are_equal / float(er_size), 3)
exit()

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

model = make_pipeline(PolynomialFeatures(4), Ridge())
model.fit(X, surf_list)
y_plot = model.predict(x_plot)

surf_trace = {
    "x": x_plot,
    "y": y_plot,
    "line": {"shape": "spline"},
    "mode": "lines+markers",
    "name": "SURF",
    "type": "scatter",
    "marker": {
        "symbol": "triangle-down-open",
        "size": 12
    }
}

model = make_pipeline(PolynomialFeatures(4), Ridge())
model.fit(X, qsift_sup_list_best)
y_plot = model.predict(x_plot)

qsift_sup_trace = {
    "x": x_plot,
    "y": y_plot,
    "line": {"shape": "spline"},
    "mode": "lines+markers",
    "name": "Super-Extensive qSIFT",
    "type": "scatter",
    "marker": {
        "symbol": "diamond-open",
        "size": 12
    }
}

model = make_pipeline(PolynomialFeatures(4), Ridge())
model.fit(X, qsift_sub_list_best)
y_plot = model.predict(x_plot)

qsift_sub_trace = {
    "x": x_plot,
    "y": y_plot,
    "line": {"shape": "spline"},
    "mode": "lines+markers",
    "name": "Sub-Extensive qSIFT",
    "type": "scatter",
    "marker": {
        "symbol": "star",
        "size": 12
    }
}

data = Data([surf_trace, qsift_sup_trace,
             qsift_sub_trace, sift_trace])

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
        "title": "b_nssf",
        "titlefont": {"size": 20},
        "type": "linear"
    }
}

fig = Figure(data=data, layout=layout)

# py.sign_in('username', 'api_key')
py.sign_in(username='miller.horvath',
           api_key='F1vaWxL8j2CFqLpJNmCQ')
py.plot(fig, filename=f_name)
