import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

csv_fn1 = "../../cases/20170918_025/nifti_3d/intensity_logs/scar1_intensity_log.csv"
csv_fn2 = "../../cases/20170918_025/nifti_3d/intensity_logs/scar2_intensity_log.csv"

csv = np.genfromtxt (csv_fn1, delimiter=",", skiprows=1)

bp = 293 
sd = 70
i = csv[:,5] 
i_z = (i - 293) / 70
#print(i_z)
#plt.plot(csv[:,0], i_z, 'ro')
#plt.axis([20000, 30000, 0, 2])
#plt.show()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(csv[:,1], csv[:,0],  i_z)
ax.set_xlabel('Normal direction')
ax.set_ylabel('Cell-id')
ax.set_zlabel('Standardise intensity')

plt.show()