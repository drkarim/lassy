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

warnings.filterwarnings('ignore')

class PolarPlotter(object):

    def __init__(self, lesion_df=None): 
        self.lesion_df = lesion_df 

    # t_nogap is the cut-off where a value above t_nogap means no gap 
    def CircularPiePlot(self, t_nogap, write_csv_file=None):
        count = len( self.lesion_df.as_matrix())
        #scalars = df_data_with_theta.as_matrix(columns=df_data_with_theta.columns[7:8])
        scalars =  self.lesion_df['MeshScalar'].as_matrix()
        num_prim_groups = 4 #  Do not change as pie plot is hard-coded to accept four primary groups
        num_sub_groups = 4  #  Do not change as pie plot is hard-coded to accept four primary groups
        total_groups = num_prim_groups*num_sub_groups
        sub_group_size = count/total_groups

        # Divide the data into 12 groups, 4 regions x 3 sub-regions, regions = post sup, post inf, etc. 
        sub_group_array = np.zeros(shape=(total_groups, 1))

        n=0
        sb = np.array_split(scalars, total_groups)
        for i in sb: 
            sub_group_array[n] = np.mean(i)
            n += 1
            
        
        # binary classify sub_groups into gaps and no-gaps 
        bin_subgrouparray = sub_group_array > t_nogap
        bin_subgrouparray = bin_subgrouparray * 1    # converts the false true array to 0 1 
        
        # Now prepare data for circular plot 
        
        # First decide color sub-groups based on binary classification of sub_groups (i.e. t_nogaps)
        a, b, c=[plt.cm.Blues, plt.cm.Reds, plt.cm.Greens]
        color_subgroups = list() 
        for x in bin_subgrouparray: 
            if x == 1:
                color_subgroups.append(b(0.5))
            else:
                color_subgroups.append(c(0.5))
        
        # Decide color of the primary groups (n=4) based on number of sub_groups classified as gaps/no-gaps 
        color_primgroups = list()
        prim_group_array = np.zeros(shape=(num_prim_groups, 1))
            
        # classify primary groups 
        prim_group_array = np.split(bin_subgrouparray, num_prim_groups)
        n=0
        for i in prim_group_array: 
            prim_group_array[n] = np.sum(i)
            n += 1
            
        for x in prim_group_array: 
            if x==1:
                color_primgroups.append('green')
            elif x==2: 
                color_primgroups.append('yellow')
            elif x==3:
                color_primgroups.append('gold')
            elif x==4:
                color_primgroups.append('red')
            else:
                color_primgroups.append('green')   # x == 0
            
            
            
        # Make data: I have 3 groups and 7 subgroups
        group_names=['Post-Sup (A)', 'Ant-Sup (B)', 'Ant-Inf (C)', 'Post-Inf (D)']
        group_size=[1,1,1,1]
        subgroup_names=['A1', 'A2', 'A3', 'A4','B1', 'B2', 'B3', 'B4', 'C1', 'C2', 'C3', 'C4', 'D1', 'D2', 'D3', 'D4']
        subgroup_size=[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
        

        if not write_csv_file: 
            # First Ring (outside)
            fig, ax = plt.subplots()
            ax.axis('equal')
            mypie, _ = ax.pie(group_size, radius=1.3, labels=group_names, colors=color_primgroups )
            plt.setp( mypie, width=0.3, edgecolor='white')

            # Second Ring (Inside)
            mypie2, _ = ax.pie(subgroup_size, radius=1.3-0.3, labels=subgroup_names, labeldistance=0.7, colors=color_subgroups)
            plt.setp( mypie2, width=0.4, edgecolor='white')
            plt.margins(0,0)

            # show it
            plt.show()
        else:
            # write to CSV file 
            
            
            i=0; 
            entries=[]
            

            for x in bin_subgrouparray:
                #df_write.loc[len(df_write)] = [0, math.ceil(i/4), i, x, 0, 0]
                entries.append([ 0, math.ceil(i/4), i, x[0], 0, 0])
                i+=1 
            
            #print entries 
            df_write = pd.DataFrame(data=entries, index=range(1, 1+len(entries)), columns=['case', 'quadrant', 'segment', 'gap', 'width', 'point_lesion'])
            df_write.to_csv(write_csv_file)

    
    # t_nogap is the cut-off where a value above t_nogap means no gap 
    def CircularPiePlotScarWidth(self, t_scar):
        count = len(self.lesion_df.as_matrix())
        #scalars = df_data_with_theta.as_matrix(columns=df_data_with_theta.columns[7:8])
        scalars = self.lesion_df['MeshScalar'].as_matrix()
        num_prim_groups = 4 #  Do not change as pie plot is hard-coded to accept four primary groups
        num_sub_groups = 4  #  Do not change as pie plot is hard-coded to accept four primary groups
        total_groups = num_prim_groups*num_sub_groups
        sub_group_size = count/total_groups
        
       
        for i, row in self.lesion_df.iterrows():
            #row['polar_group'] = math.ceil(index / sub_group_size )
            self.lesion_df.at[i, 'polar_group'] = math.ceil(i / sub_group_size )
        
        # See https://stackoverflow.com/questions/31667070/max-distance-between-2-points-in-a-data-set-and-identifying-the-points
        total_width_in_each_subgrp = []
        scar_width_in_each_subgrp = []
        for i in range(0, total_groups):
            df_temp = self.lesion_df[self.lesion_df['polar_group']==i]
            xyz = df_temp.as_matrix(columns=['X', 'Y', 'Z'])
            D = pdist(xyz) # pair-wise distances 
            D = squareform(D)
            N, [I_row, I_col] = nanmax(D), unravel_index( argmax(D), D.shape )
            total_width_in_each_subgrp.append(round(N,2))
            
            # now select rows that was classified as scar
            df_temp2 = df_temp[df_temp['scar_bin'] == 1]
            xyz = df_temp2.as_matrix(columns=['X', 'Y', 'Z'])
            D = pdist(xyz) # pair-wise distances 
            D = squareform(D)
            N, [I_row, I_col] = nanmax(D), unravel_index( argmax(D), D.shape )
            scar_width_in_each_subgrp.append(round(N,2))
        
        # calculate scar width percentage 
        scar_width_percentage = []
        i=0
        while i < len(scar_width_in_each_subgrp):
            total = total_width_in_each_subgrp[i]
            this_scar = scar_width_in_each_subgrp[i]
            scar_width_percentage.append(this_scar/total)
            i+=1
        scar_width_percentage = np.asarray(scar_width_percentage)
        # Now prepare data for circular plot 
        
        # First decide color sub-groups based on binary classification of sub_groups (i.e. t_nogaps)
        a, b, c=[plt.cm.Blues, plt.cm.Reds, plt.cm.Greens]
        color_subgroups = list() 
        for x in scar_width_percentage: 
            if x >= 0.75:
                color_subgroups.append('red')
            elif x >= 0.5 and x < 0.75:
                color_subgroups.append('gold')
            elif x >= 0.25 and x < 0.5: 
                color_subgroups.append('yellow')
            elif x >= 0 and x < 0.25: 
                color_subgroups.append('green')
        
                
        # Decide color of the primary groups (n=4) based on number of sub_groups classified as gaps/no-gaps 
        color_primgroups = list()
        prim_group_array = np.zeros(shape=(num_prim_groups, 1))
            
        # classify primary groups 
        prim_group_array = np.split(scar_width_percentage, num_prim_groups)
        n=0
        for i in prim_group_array: 
            prim_group_array[n] = np.mean(i)
            n += 1
            
        for x in prim_group_array: 
            if x >= 0.75:
                color_primgroups.append('red')
            elif x >= 0.5 and x < 0.75:
                color_primgroups.append('gold')
            elif x >= 0.25 and x < 0.5: 
                color_primgroups.append('yellow')
            elif x > 0 and x < 0.25: 
                color_primgroups.append('green')
            else:
                color_primgroups.append('green')   # x == 0
            
            
            
        # Make data: I have 3 groups and 7 subgroups
        group_names=['Post-Sup (A)', 'Ant-Sup (B)', 'Ant-Inf (C)', 'Post-Inf (D)']
        group_size=[1,1,1,1]
        subgroup_names=['A1', 'A2', 'A3', 'A4','B1', 'B2', 'B3', 'B4', 'C1', 'C2', 'C3', 'C4', 'D1', 'D2', 'D3', 'D4']
        subgroup_size=[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
        
        # First Ring (outside)
        fig, ax = plt.subplots()
        ax.axis('equal')
        mypie, _ = ax.pie(group_size, radius=1.3, labels=group_names, colors=color_primgroups )
        plt.setp( mypie, width=0.3, edgecolor='white')

        # Second Ring (Inside)
        mypie2, _ = ax.pie(subgroup_size, radius=1.3-0.3, labels=subgroup_names, labeldistance=0.7, colors=color_subgroups)
        plt.setp( mypie2, width=0.4, edgecolor='white')
        plt.margins(0,0)

        # show it
        plt.show()
        
        print ("Complete widths in each sub-segment", total_width_in_each_subgrp) 
        print ("Scar widths in each sub-segment", scar_width_in_each_subgrp) 
            