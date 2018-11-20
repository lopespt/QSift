import json

from matplotlib import pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import json
from scipy import interpolate



fig = plt.figure()
ax = fig.gca(projection='3d')

# Make data.
data = []
with open("marple17.json") as f:
    data = json.load(f)


data = [ ( round(x["parameters"][1],2), round(x["parameters"][3],2), x["result"]["quality"] ) for x in data]

x = sorted(list(set([x[0] for x in data])))
y = sorted(list(set([x[1] for x in data])))

xmin, xmax = x[0], x[-1]
ymin, ymax = y[0], y[-1]


cx = {}
for i in x:
    if i in cx:
        cx[i]+=1
    else:
        cx[i]=1


cy = {}
for i in cy:
    if i in cy:
        cy[i]+=1
    else:
        cy[i]=1

print(cx)
print(cy)

data = {(x[0], x[1]): x[2] for x in data}
print(data)

X = x
Y = y
X, Y = np.meshgrid(X, Y)
print(X)

Z = [ data[(a,b)] if (a,b) in data else -100 for a,b in zip(X.ravel(), Y.ravel())]

Z = np.array(Z)
Z = Z.reshape( np.shape(X) )





xnew, ynew = np.mgrid[xmin:xmax:500j, ymin:ymax:500j]
tck = interpolate.bisplrep(X, Y, Z, s=0)
znew = interpolate.bisplev(xnew[:,0], ynew[0,:], tck)


# Plot the surface.
surf = ax.plot_surface(X, Y, Z, cmap=plt.cm.coolwarm,
                       linewidth=0, antialiased=False)

# Add a color bar which maps values to colors.
fig.colorbar(surf, shrink=0.5, aspect=5)

plt.show()