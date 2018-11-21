import json
import numpy as np
from matplotlib import rc,pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.interpolate import griddata


font = {'family' : 'normal',
        'size'   : 20}
rc('font', **font)

def plot_qsift():
    res = None
    with open("./qsift.json") as f:
        res = json.load(f)

    videos = np.unique([x["parameters"][1] for x in res])

    for video in videos:
        q,step,quality = zip(*[ (x["result"]["q"], x["result"]["step"], x["result"]["quality"]) for x in res if  x["parameters"][1] == video]  )

        q,step,quality = zip(*sorted(zip(q,step,quality), key=lambda x: x[1]))
        min_q, max_q = min(q), max(q)
        min_step, max_step = min(step), max(step)
        rx,ry = np.linspace(min_q, max_q, 10), np.linspace(min_step, max_step, 10)
        X,Y = np.meshgrid(rx,ry)
        print(X, Y)
        Z = griddata( [[a,b] for a,b in zip(q,step)], quality, (X,Y) ,method='cubic',fill_value=0)

        #fig = plt.figure()
        #ax = Axes3D(fig)
        #ax.plot_surface(X,Y,Z)
        plt.figure()
        plt.title(video)
        plt.contourf(X,Y,Z,100)
        plt.colorbar()
        plt.xlabel("q")
        plt.ylabel("step")
        plt.show()


    #fig = plt.figure()
    #ax = Axes3D(fig)
    #ax.plot_trisurf(q,step,quality)
    #plt.xlabel("q")
    #plt.ylabel("step")
    #ax.set_label("quality")
    #plt.show()
    
    
plot_qsift()