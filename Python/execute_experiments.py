
import sys
import os
import subprocess
from pathlib import Path


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
    start = ["--from", str(start)] if start != -1 else []
    end = ["--to", str(end)] if end != -1 else []
    steps = ["--step", str(steps)] if steps != -1 else []

    p = subprocess.Popen(["../CPP/qsift/build/OrdenacaoSift2", pasta_imagens, str(q_val), str(b_val),
                          *start, *end, *steps], stdout=subprocess.PIPE)
    saida = b""
    while(True):
        x = p.stdout.readline()
        if b"" == x:
            break
        print(x.decode("utf-8"), end="")


for x in range(1, 10):
    qsift("/home/lopespt/Downloads/v1/", 0.5, 2, x)
