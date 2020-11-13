import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator
import numpy as np

N = 10
x = np.arange(0, 50*N, 1)

fig = plt.figure(1)
ax = fig.add_subplot(projection='3d')

with open("output.txt", "r") as file:
    while True:
        data = file.readline()
        if not data:
            break
        t = float(data)
        if t > 0.005:
            break
        data = file.readline()
        y = [float(i) for i in data.split()]
        ax.scatter(xs=x, ys=t, zs=y)
  
plt.show()