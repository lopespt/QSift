"""
Plot all graphics from experiments
"""

import json
import math
import pandas as pd

import numpy as np
from matplotlib import pyplot as plt
from matplotlib import rc
from scipy.interpolate import griddata

font = {"family": "Dejavu Sans", "size": 20}
rc("font", **font)


def getFittedData(x, y, z, step_x=10, step_y=10, x_range=None, y_range=None):
    x, y, z = zip(*sorted(zip(x, y, z), key=lambda x: x[0]))
    if x_range is not None:
        min_x, max_x = x_range
    else:
        min_x, max_x = min(x), max(x)

    if y_range is not None:
        min_y, max_y = y_range
    else:
        min_y, max_y = min(y), max(y)

    rx, ry = np.linspace(min_x, max_x, step_x), np.linspace(
        min_y, max_y, step_y)

    # rx, ry = np.linspace(0.2, 1.8, 10), np.linspace(
    #    1, 200, 10)
    X, Y = np.meshgrid(rx, ry)
    Z = griddata([[a, b] for a, b in zip(x, y)],
                 z, (X, Y),
                 method="cubic",
                 fill_value=0)
    return (X, Y, Z)


def plotData(X, Y, Z, title, xlabel, ylabel, saveFileName=None):
    plt.figure(figsize=(15, 10))
    plt.title(title)
    plt.contourf(X, Y, Z, 100)
    plt.colorbar()
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)

    if saveFileName is None:
        plt.show()

    plt.savefig(saveFileName)


def plot_qsift():
    res = None
    with open("./results_servidor/qsift_arrumado.json") as f:
        res = json.load(f)

    videos = np.unique([x["parameters"][1] for x in res])
    zAcum = None
    zTotal = 0

    for video in videos:
        q, step, quality = zip(*[(x["result"]["q"], x["result"]["step"],
                                  x["result"]["quality"])
                                 for x in res if x["parameters"][1] == video])

        X, Y, Z = getFittedData(q, step, quality)
        zAcum = zAcum + Z if zAcum is not None else Z
        zTotal += 1

        plotData(X, Y, Z, video, "q", "step", video.replace("/", "_") + ".png")

    # Plot Média
    plt.close("all")
    zMedio = zAcum / zTotal
    plotData(X, Y, zMedio, "Media", "q", "step", "media.png")

    plt.close('all')
    print(zMedio)
    melhorPorStep = np.max(zMedio, axis=1)
    x = (Y[:, 0]).flatten()
    y = melhorPorStep
    plt.plot(x, y)
    plt.xlabel("Step")
    plt.ylabel("Nota")
    plt.title("Notas dos Melhores q(s)")
    plt.savefig("qsift_melhoresq.png")
    print(melhorPorStep)
    return (x, y)


def plot_asift():
    res = None
    with open("./results_servidor/asift.json") as f:
        res = json.load(f)

    videos = np.unique([x["parameters"][1] for x in res])
    media = {}

    for video in videos:
        step, quality = zip(*[(x["result"]["step"], x["result"]["quality"])
                              for x in res if x["parameters"][1] == video
                              and not math.isnan(x["result"]["quality"])])
        step, quality = zip(*sorted(zip(step, quality), key=lambda x: x[0]))
        if len(step) < 4:
            continue

        # media.append(quality)
        for x, y in zip(step, quality):
            if x not in media:
                media[x] = []
            media[x].append(y)

    plt.close("all")
    print(media.keys())
    mediaValues = [np.mean(media[i]) for i in sorted(media.keys())]
    print(mediaValues)
    plt.title("Nota média ASIFT")
    plt.xlabel("step")
    plt.ylabel("nota")
    x = sorted(media.keys())
    y = mediaValues
    plt.plot(x, y)
    plt.savefig("asift.png")
    return x, y



def plot_qsift_asift(x, y, x2, y2):
    plt.close("all")
    plt.plot(x,y,x2,y2)
    plt.xlabel("step")
    plt.ylabel("nota")
    plt.legend(["q-Sift", "a-Sift"])
    plt.savefig("qsift_asift.png")

def plot_qasift():
    res = None
    with open("./results_servidor/qasift.json") as f:
        res = json.load(f)

    videos = np.unique([x["parameters"][1] for x in res])
    media = {}

    for video in videos:
        step, quality = zip(*[(x["result"]["step"], x["result"]["quality"])
                              for x in res if x["parameters"][1] == video
                              and not math.isnan(x["result"]["quality"])])
        step, quality = zip(*sorted(zip(step, quality), key=lambda x: x[0]))
        if len(step) < 4:
            continue

        # media.append(quality)
        for x, y in zip(step, quality):
            if x not in media:
                media[x] = []
            media[x].append(y)

    plt.close("all")
    print(media.keys())
    mediaValues = [np.mean(media[i]) for i in sorted(media.keys())]
    print(mediaValues)
    plt.title("Nota média ASIFT")
    plt.xlabel("step")
    plt.ylabel("nota")
    x = sorted(media.keys())
    y = mediaValues
    plt.plot(x, y)
    plt.savefig("asift.png")
    return x, y


# x, y = plot_qsift()
# x2, y2 = plot_asift()
# plot_qsift_asift(x, y, x2, y2)

# plot_qasift()



