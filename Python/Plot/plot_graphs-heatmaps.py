from plotly.graph_objs import *
import plotly.graph_objs as go
import plotly.plotly as py
import cPickle as cpkl
import os
import numpy as np

py.sign_in(username='miller.horvath',
           api_key='F1vaWxL8j2CFqLpJNmCQ')

r_folder = ['NYC',
            'Italy_f4382_t5280',
            'Italy_f11853_t13074',
            'SDakota']

out_f_name = ['NYC',
              'Italy1',
              'Italy2',
              'SDakota']

ssf_dict_file = []
ssf_surf_file = []

for folder in r_folder:
    ssf_dict_file.append(os.path.join('..', 'nssf_results',
                                      folder,
                                      'results.cpkl'))

    ssf_surf_file.append(os.path.join('..', 'nssf_results',
                                      folder,
                                      'resultsSurf.cpkl'))

f_name = []

for fn in out_f_name:
    f_name.append('{}_Heatmap'.format(fn))

frange = [(2017, 2517), (4382, 5280),
          (11853, 13074), (43370, 44350)]

title = []
t_temp = "{} | Frames from {} to {}"

for i in range(len(out_f_name)):
    f = frange[i][0]
    t = frange[i][1]
    title.append(t_temp.format(out_f_name[i], f, t))

frames_num = []

for r in frange:
    frames_num.append(r[1] - r[0] + 1)

ssf_dict = []

for fn in ssf_dict_file:
    f = open(fn, 'rb')
    ssf_dict.append(cpkl.load(f))
    f.close()

ssf_surf = []

for fn in ssf_surf_file:
    f = open(fn, 'rb')
    ssf_surf.append(cpkl.load(f))
    f.close()

q_values = np.arange(0.1, 2.0, 0.1)
s_values = range(1, 102, 4)
s_values = s_values + range(150, 251, 50)
h_values = range(200, 401, 100)

for i in range(len(q_values)):
    q_values[i] = round(q_values[i], 2)

for i in range(len(s_values)):
    s_values[i] = round(s_values[i], 2)

q_values[9] = 0.99

for v_idx in range(len(r_folder)):
    image = np.zeros((22, 29))
    data_list = []

    i = 4
    for q in sorted(q_values):
        if q != 0.99:
            j = 0

            for s in sorted(s_values):
                ssf = float(ssf_dict[v_idx][s, q].split()[4].replace(',', '.'))
                frames = float((frames_num[v_idx] - 1) // s)
                nssf = (frames + ssf) / (2.0 * frames)

                image[i, j] = nssf

                j += 1
            i += 1

    i = 0
    for h in sorted(h_values):
        j = 0
        for s in sorted(s_values):
            # print ssf_surf[v_idx][s, h]
            ssf = float(ssf_surf[v_idx][s, h].split()[3].replace(',', '.'))
            frames = float((frames_num[v_idx] - 1) // s)
            nssf = (frames + ssf) / (2.0 * frames)
            # print ssf

            image[i, j] = nssf
            j += 1
        i += 1

    i = 3
    q = 0.99
    j = 0

    for s in sorted(s_values):
        ssf = float(ssf_dict[v_idx][s, q].split()[4].replace(',', '.'))
        frames = float((frames_num[v_idx] - 1) // s)
        nssf = (frames + ssf) / (2.0 * frames)

        image[i, j] = nssf

        j += 1

    x_label = []

    for s in sorted(s_values):
        x_label.append(s)

    y_label = []

    for h in sorted(h_values):
        y_label.append('SURF | H={}'.format(h))

    y_label.append('SIFT')

    for q in sorted(q_values):
        if q != 0.99:
            y_label.append('qSIFT | q={}'.format(q))

    # Trace Settings
    colorbar = {"title": "nssf"}
    colorscale = [[0, "rgb(5,10,172)"], [0.35, "rgb(106,137,247)"],
                  [0.5, "rgb(190,190,190)"], [0.6, "rgb(220,170,132)"],
                  [0.7, "rgb(230,145,90)"], [1, "rgb(178,10,28)"]]
    name = title[v_idx]
    reversescale = True
    showscale = True
    xgap = 1
    ygap = 1
    zauto = True
    zmax = 1.0
    zmin = 0.0

    trace = go.Heatmap(z=image, x=x_label, y=y_label,
                       colorbar=colorbar,
                       colorscale=colorscale,
                       name=name,
                       reversescale=reversescale,
                       showscale=showscale,
                       xgap=xgap, ygap=ygap,
                       zauto=zauto,
                       zmax=zmax, zmin=zmin)
    data = [trace]

    layout = {
        "autosize": True,
        "font": {"size": 12},
        "hovermode": False,
        "margin": {
            "t": 80,
            "l": 110,
            "pad": 0
        },
        "title": name,
        "titlefont": {"size": 17},
        "xaxis": {
            "autorange": True,
            "range": [-1, 275],
            "title": "Interval Between Consecutive Frames",
            "type": "linear"
        },
        "yaxis": {
            "autorange": True,
            "range": [-0.5, 18.5],
            "title": "Algorithm Parameters",
            "type": "category"
        }
    }

    fig = Figure(data=data, layout=layout)
    py.plot(fig, filename=f_name[v_idx])
