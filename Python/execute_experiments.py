
import functools
import itertools
import json
import multiprocessing as mp
import os
import subprocess
import sys
from pathlib import Path

import tqdm

from experiments import extract_frames, qsift


def execute_qsift_experiments():
    q_s = [0.1, 0.2, 0.4, 0.5, 0.7, 1.1, 1.2, 1.3, 1.4]
    b_s = [2*x for x in q_s]
    step = range(1, 10, 2)
    outfile = "../Results/qsift.json"
    x = []
    if os.path.exists(outfile):
        with open(outfile, "r") as f:
            x = json.load(f)
    parameters = list(itertools.product(q_s, b_s, step))
    parameters.extend(itertools.product([1], [1], step))

    done = [(i["q"], i["b"], i["step"]) for i in x]

    rest = set(parameters) - set(done)
    rest = list(rest)

    pool = mp.Pool(6)

    x.extend(pool.starmap(functools.partial(
        qsift, "/home/lopespt/Downloads/v1/"), rest))
    with open(outfile, "w") as f:
        json.dump(x, f, indent=4)

    # for x in range(1, 10):
    #    print(qsift("/home/lopespt/Downloads/v1/", 0.5, 2, x))


execute_qsift_experiments()
