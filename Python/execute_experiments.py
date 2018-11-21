"""
Módulo utilizado para rodar os experimentos
"""

import itertools
import json
import multiprocessing as mp
import os
from pathlib import Path

import numpy as np
import tqdm

from experiments import asift, qsift, surf


def star(func, args):
    return func(*args)


def partialAsift(args):
    return asift("/home/lopespt/Downloads/v1/", *args)


def runner(func, k):
    x = dict()
    x["parameters"] = [func.__name__, *k]
    x["result"] = func(*k)

    return x


def execute(fout, func, parameters):
    x = []

    if os.path.exists(fout):
        with open(fout, "r") as f:
            x = json.load(f)

    done = [tuple(i["parameters"]) for i in x]

    proc = mp.Pool(2)

    runs = x
    parameters = list(parameters)
    tq = tqdm.tqdm(total=len(parameters))

    for x in parameters:
        if tuple([func.__name__, *x]) not in done:

            def callback(result):
                tq.update(1)
                with open(fout, "w") as f:
                    runs.append(result)
                    json.dump(runs, f, indent=4)

            proc.apply_async(runner, (func, x), callback=callback)
        else:
            tq.update(1)

    proc.close()
    proc.join()
    tq.close()


def execute_asift_experiments(folders, steps, outfile):
    # folder = "/home/lopespt/Downloads/v1/"
    # outfile = "../Results/asift.json"

    # step = range(1, 50, 2)
    parameters = itertools.product(folders, steps, [-1], [-1])
    parameters = list(set(parameters))
    execute(outfile, asift, parameters)


def execute_qsift_experiments(folders, q_s, b_s, steps, outfile):
    # folders = ["/home/lopespt/Downloads/v1/"]
    # q_s = [0.1, 0.3, 0.4, 0.5, 0.7, 1.1, 1.2, 1.3, 1.4]
    # b_s = [2 * x for x in q_s]
    # step = range(1, 20, 2)
    # outfile = "../Results/qsift.json"

    parameters = list(itertools.product(folders, q_s, b_s, steps, [-1], [-1]))
    parameters.extend(itertools.product(folders, [1], [1], steps, [-1], [-1]))
    parameters = list(set(parameters))

    execute(outfile, qsift, parameters)


def execute_surf_experiments(folders, hessian, steps, outfile):
    # folders = ["/home/lopespt/Downloads/v1/"]
    # hessian = range(100, 500, 100)
    # step = range(1, 20, 2)
    # outfile = "../Results/surf.json"

    # Para cada pasta, rodar o experimento com todas as variações considerando todo o vídeo
    parameters = list(itertools.product(folders, hessian, steps, [-1], [-1]))
    parameters = list(set(parameters))
    execute(outfile, surf, parameters)


pastas = [
    str(Path(a).resolve()) + "/"
    for (a, _, c) in os.walk("../RunningExperiment/", followlinks=True)
    if len(c) > 0
]

# arquivos = [p.split("/")[-2]+".json" for p in pastas]

# print(pastas)
# print(arquivos)


def arange(start, end, step):
    return [round(i, 2) for i in np.arange(start, end, step).tolist()]


# execute_qsift_experiments(pastas, arange(0.2, 1.8, 0.2), arange(0.2, 2, 0.4),
                          # arange(1, 200, 5), "qsift.json")

# execute_qsift_experiments(pastas, [0.5], [0.5], [2], arquivos)

# execute_surf_experiments(pastas, arange(100, 300, 50), arange(1, 200, 5),
#                          "surf.json")

# execute_surf_experiments()
execute_asift_experiments(pastas, arange(1, 200, 5), "asift.json")
