
import sys
import os
import subprocess
from pathlib import Path
import multiprocessing as mp
import functools
import itertools
import tqdm

def extract_frames(video, start, end, dest_folder):
    p = subprocess.Popen(["../CPP/qsift/build/Conversor", video,
                          str(start), str(end), dest_folder], stdout=subprocess.PIPE)
    #out = p.communicate()[0]
    while(True):
        x = p.stdout.readline()
        if b"" == x:
            break
        print(x.decode("utf-8", end=""))


def qsift(pasta_imagens, q_val, b_val, steps=1, start=-1, end=-1):
    def parse_qsift_output(saida):
        return dict(zip(["q","b","step","zero_features","quality"], saida.split()))
    start = ["--from", str(start)] if start != -1 else []
    end = ["--to", str(end)] if end != -1 else []
    steps = ["--step", str(steps)] if steps != -1 else []

    p = subprocess.Popen(["../CPP/qsift/build/OrdenacaoSift2", pasta_imagens, str(q_val), str(b_val),
                          *start, *end, *steps], stdout=subprocess.PIPE)
    saida = ""
    while(True):
        x = p.stdout.readline()
        if b"" == x:
            break
        x = x.decode("utf-8")
        x = x.replace("\n","").replace("\r","")
        saida += x
    print(saida)
    return parse_qsift_output(saida)


q_s = [0.1, 0.2, 0.4, 0.5, 0.7, 1.1,1.2,1.3,1.4]
b_s = [2*x for x in q_s]
step = range(1,11)

parameters = itertools.product(q_s,b_s,step)
pool = mp.Pool(16)
x = pool.starmap(functools.partial(qsift,"/home/lopespt/Downloads/v1/"), parameters)
print(x)
#for x in range(1, 10):
#    print(qsift("/home/lopespt/Downloads/v1/", 0.5, 2, x))
