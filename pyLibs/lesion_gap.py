import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from scipy.stats import norm
from sklearn.preprocessing import StandardScaler
from scipy import stats
from mpl_toolkits.mplot3d import Axes3D
from sklearn.model_selection import train_test_split
import warnings
import math
from numpy import linalg as LA
from numpy import random, nanmax, argmax, unravel_index
from scipy.spatial.distance import pdist, squareform

# lesion library 
from lesion.core import LesionSet
from lesion.viz import PolarPlotter

warnings.filterwarnings('ignore')

# load the lesion intensity set 
#l =  LesionSet.LesionSet('../python_scripts/data/encirclement/point_lesion/DP_RPV_7NN.csv')
l =  LesionSet.LesionSet('../python_scripts/data/encirclement/control/CF_LPV_7NN.csv')

# the intensity above which lesion is defined 
t_scar = 4.5   # DP - 40, CF - 4.5


# classify lesion set data based on t_scar 
l.prepareset(t_scar)

# Now for some visualisations 
plotter = PolarPlotter.PolarPlotter(l.getData())
plotter.CircularPiePlot(t_scar, 'test.csv')
#plotter.CircularPiePlotScarWidth(t_scar)

