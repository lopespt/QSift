
import sys
import os
import subprocess

p = subprocess.Popen(["ls", "-la"], stdout=subprocess.PIPE)
out = p.communicate()[0]
print(out.decode("utf-8") )