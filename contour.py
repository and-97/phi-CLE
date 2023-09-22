#load the data in countour.dat and plot the contour in the complex plane

import numpy as np
import matplotlib.pyplot as plt

#load the data
data = np.loadtxt('contour.dat')
x = data[:,0]
y = data[:,1]

#plot the contour
plt.plot(x,y, 'Xr-', ms = 8, mec = 'k', mfc = 'k')
plt.xlabel('Re(z)') 
plt.ylabel('Im(z)')
#show the real and imaginary axis
plt.axhline(y=0, color='k')
plt.axvline(x=0, color='k')

plt.title('Contour in the complex plane')
plt.show()
