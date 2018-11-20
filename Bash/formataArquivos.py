import sys
import os
from PIL import Image
import re

def formatFilename(s):
    r = re.findall("(.*?)([0-9]*)(\..*$)", s,re.DOTALL)[0]
    return r[0]+ r[1].lstrip("0")+ ".png"

    
afrom = sys.argv[1]
ato = formatFilename(sys.argv[1])
print("Convertendo %s em %s" %(afrom, ato))
im = Image.open(afrom)

im.save(ato)