import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator
import numpy as np

N = 10
x = np.arange(0, 50*N, 1)

fig = plt.figure()
ax = fig.add_subplot(projection='3d')

with open("output.txt", "r") as file:
    while True:
        data = file.readline()
        if not data:
            break
        t = float(data)
        data = file.readline()
        if t * 10 % 10 != 0: 
            continue
        y = [float(i) for i in data.split()]
        ax.plot(xs=x, ys=[t]*len(x), zs=y, color="b", marker=".")
        
# ax.set_xlabel('x')
ax.set_ylabel('t')
ax.set_zlabel('u(x, t)')
  
plt.show()