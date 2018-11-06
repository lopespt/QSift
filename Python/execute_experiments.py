import itertools
import json
import multiprocessing as mp
import operator as op
import os
import subprocess
import sys
from functools import partial
from pathlib import Path

import tqdm

from experiments import asift, extract_frames, qsift, surf


def star(func, args):
    return func(*args)


def partialAsift(args):
    return asift("/home/lopespt/Downloads/v1/", *args)


def runner(func, k):
    return {"result": func(*k), "parameters": k}


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
        if x not in done:

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


def execute_asift_experiments():
    folder = "/home/lopespt/Downloads/v1/"
    outfile = "../Results/asift.json"

    step = range(1, 50, 2)
    parameters = itertools.product([folder], step, [0], [100])
    parameters = list(parameters)
    execute(outfile, asift, parameters)


def execute_qsift_experiments():
    folders = ["/home/lopespt/Downloads/v1/"]
    q_s = [0.1, 0.3, 0.4, 0.5, 0.7, 1.1, 1.2, 1.3, 1.4]
    # q_s = [0.1, 0.3]
    b_s = [2 * x for x in q_s]
    step = range(1, 10, 2)
    step = [1]
    outfile = "../Results/qsift.json"

    parameters = list(itertools.product(folders, q_s, b_s, step, [0], [10]))
    parameters.extend(itertools.product(folders, [1], [1], step, [0], [10]))
    parameters = list(parameters)

    execute(outfile, qsift, parameters)


def execute_surf_experiments():
    folders = ["/home/lopespt/Downloads/v1/"]
    # q_s = [0.1, 0.3, 0.4, 0.5, 0.7, 1.1, 1.2, 1.3, 1.4]
    # q_s = [0.1, 0.3]
    hessian = range(100, 500, 100)
    step = range(1, 10, 2)
    outfile = "../Results/surf.json"

    parameters = list(itertools.product(folders, hessian, step, [0], [100]))
    parameters = list(parameters)

    execute(outfile, surf, parameters)


# execute_qsift_experiments()
execute_asift_experiments()
# execute_surf_experiments()
