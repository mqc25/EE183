from math import cos
from math import sin
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from math import pi


def T_matrix(q):
    T = [[cos(q[3]), -sin(q[3]), 0, q[0]],
    [sin(q[3])*cos(q[1]), cos(q[3]) * cos(q[1]), -sin(q[1]), -sin(q[1]) * q[2]],
    [sin(q[3])*sin(q[1]), cos(q[3]) * sin(q[1]), cos(q[1]), cos(q[1]) * q[2]],
    [0, 0, 0, 1]]

    return np.array(T)

def plotBody(a):
    x = []
    y = []
    z = []
    for i in a:
        x.append(i[0])
        y.append(i[1])
        z.append(i[2])

    max_range = np.array([max(x) - min(x), max(y) - min(y), max(z) - min(z)]).max() / 2.0

    fig = plt.figure()
    ax = Axes3D(fig)

    ax.set_xlabel('X axis')
    ax.set_ylabel('Y axis')
    ax.set_zlabel('Z axis')
    ax.auto_scale_xyz([0,80], [-40, 40], [-80, 0])

    color = ['black','red','blue','orange','violet','green']
    ax.plot(x, y, z)
    ax.scatter(x, y, z,c=color)
    ax.axis('equal')

def FK(q):
    p = [[0, pi/2, 0, q[0]], [2, -pi/2, 0, q[1]], [35, pi/2, 0, q[2]], [0, pi/2, 2, q[3]], [20, 0, 35, 0]]

    T = np.array([T_matrix(p[0]), T_matrix(p[1]), T_matrix(p[2]), T_matrix(p[3]), T_matrix(p[4])])
    T0 = [T[0]]
    for i in range(4):
        T0.append(np.dot(T0[i], T[i + 1]))

    a = [np.array([0, 0, 0, 1])]
    for i in range(5):
        a.append(np.dot(T0[i], a[0]))

    return T0,a




def iterateQ(q,x,goal):
    step = 0.0001
    b = q[:]
    test = np.identity(len(q))*step
    a = []
    for i in range(4):
        temp, hold = FK(b+test[i])
        a.append(hold[5])

    dx = []
    for i in range(4):
        temp = (a[i][0] - x[0])/step
        dx.append(temp)

    dy = []
    for i in range(4):
        temp = (a[i][1] - x[1])/step
        dy.append(temp)

    dz = []
    for i in range(4):
        temp = (a[i][2] - x[2])/step
        dz.append(temp)

    J = [dx,dy,dz,np.zeros(4)]
    Jinv= np.linalg.pinv(np.array(J))

    delX = np.sign(goal[0] - x[0])*step
    delY = np.sign(goal[1] - x[1])*step
    delZ = np.sign(goal[2] - x[2]) * step
    dq = np.dot(Jinv,[delX,delY,delZ,0])
    q += dq
    x[0] += delX
    x[1] += delY
    x[2] += delZ
    return q,x

def main():
    q = [-pi / 2, 0, pi / 2,0]
    T0,a = FK(q)
    print a[5]
    plotBody(a)
    plt.show()

    x = a[5]
    goal = [37, 0, -17, 1]
    while(np.linalg.norm(goal-x) > 0.0001):
        q,x = iterateQ(q,x,goal)

    print x
    print q
    plotBody(a)
    plt.show()


if __name__== "__main__":
    main()