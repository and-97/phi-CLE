#load numpy and matplotlib
import numpy as np
import matplotlib.pyplot as plt

#load data
data = np.loadtxt('output.dat')

t = data[:,0]
Re_phi = data[:,1::2]
Im_phi = data[:,2::2]

#plot the field trajectories in separate files
for i in range(0, len(Re_phi[0,:])):
    plt.plot(Re_phi[:,i], Im_phi[:,i], label = 'phi_%d' %i)
    plt.xlabel('t')
    plt.ylabel('phi')
    plt.title('Field trajectories')
    plt.legend(loc = 'best')
    plt.savefig('figs/field_trajectories_%d.png' %i)
    plt.clf()

