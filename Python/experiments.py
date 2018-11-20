import functools
import itertools
import json
import multiprocessing as mp
import os
import subprocess
import sys
from pathlib import Path

import tqdm


def extract_frames(video, start, end, dest_folder):
    p = subprocess.Popen(
        ["../CPP/qsift/build/Conversor", video, str(start), str(end), dest_folder],
        stdout=subprocess.PIPE,
    )
    # out = p.communicate()[0]
    while True:
        x = p.stdout.readline()
        if b"" == x:
            break
        print(x.decode("utf-8", end=""))


def qsift(*args, **kwargs):
    pasta_imagens, q_val, b_val, steps, start, end = args

    # print("qsift(q_val: %s, b_val %s, steps, %s)" % (float(q_val), float(b_val), int(steps)))

    def parse_qsift_output(saida,):
        d = dict(zip(["q", "b", "step", "zero_features", "quality"], saida.split()))
        d["q"] = float(d["q"])
        d["b"] = float(d["b"])
        d["step"] = int(d["step"])
        d["zero_features"] = float(d["zero_features"])
        d["quality"] = float(d["quality"])
        return d

    sstart = ["--from", str(start)] if start != -1 else []
    send = ["--to", str(end)] if end != -1 else []
    ssteps = ["--step", str(steps)] if steps != -1 else []
    
    p = subprocess.Popen(
        [
            "optirun",
            "../CPP/qsift/build/OrdenacaoSift2",
            pasta_imagens,
            str(q_val),
            str(b_val),
            *sstart,
            *send,
            *ssteps,
        ],
        stdout=subprocess.PIPE,
    )
    saida = ""
    while True:
        x = p.stdout.readline()
        if b"" == x:
            break
        x = x.decode("utf-8")
        x = x.replace("\n", "").replace("\r", "")
        x = x.replace(",", ".")
        saida += x
    return parse_qsift_output(saida)


def surf(pasta_imagens, hessian_thr, steps=1, start=-1, end=-1):
    # print("qsift(q_val: %s, b_val %s, steps, %s)" % (float(q_val), float(b_val), int(steps)))

    def parse_surf_output(saida):
        d = dict(zip(["h", "step", "zero_features", "quality"], saida.split()))
        d["h"] = float(d["h"])
        d["step"] = int(d["step"])
        d["zero_features"] = float(d["zero_features"])
        d["quality"] = float(d["quality"])
        return d

    start = ["--from", str(start)] if start != -1 else []
    end = ["--to", str(end)] if end != -1 else []
    steps = ["--step", str(steps)] if steps != -1 else []

    p = subprocess.Popen(
        [
            "optirun",
            "../CPP/SURF/build/OrdenacaoSurf",
            pasta_imagens,
            str(hessian_thr),
            *start,
            *end,
            *steps,
        ],
        stdout=subprocess.PIPE,
    )
    saida = ""
    while True:
        x = p.stdout.readline()
        if b"" == x:
            break
        x = x.decode("utf-8")
        x = x.replace("\n", "").replace("\r", "")
        x = x.replace(",", ".")
        saida += x
    return parse_surf_output(saida)


def asift(*args, **kwargs):
    pasta_imagens, steps, start, end = args
    # print("qsift(q_val: %s, b_val %s, steps, %s)" % (float(q_val), float(b_val), int(steps)))

    def parse_asift_output(saida):
        d = dict(zip(["step", "quality"], saida.split()))
        d["step"] = int(d["step"])
        d["quality"] = float(d["quality"])
        d["folder"] = pasta_imagens
        return d

    start = ["--from", str(start)] if start != -1 else []
    end = ["--to", str(end)] if end != -1 else []
    steps = ["--step", str(steps)] if steps != -1 else []

    p = subprocess.Popen(
        [
            "optirun",
            "../CPP/asift/build/OrdenacaoASift", 
            pasta_imagens, 
            *start, 
            *end, 
            *steps],
        stdout=subprocess.PIPE,
    )
    saida = ""
    while True:
        x = p.stdout.readline()
        if b"" == x:
            break
        x = x.decode("utf-8")
        x = x.replace("\n", "").replace("\r", "")
        x = x.replace(",", ".")
        saida += x
    return parse_asift_output(saida)


# print(surf("/home/lopespt/Downloads/v1/", 200))
# extract_frames("")
