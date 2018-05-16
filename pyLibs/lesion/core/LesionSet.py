import pandas as pd


class LesionSet(object):

    def __init__(self, filename=None):
        self.lesion_df_data = pd.read_csv(filename)

    def createlesion(self, t_scar):
        self.lesion_df_data['scar_bin'] = self.lesion_df_data['MeshScalar'] > t_scar 
        self.lesion_df_data['scar_bin'] = self.lesion_df_data['scar_bin'] * 1 

    def removeduplicates(self): 
        self.lesion_df_data = self.lesion_df_data[self.lesion_df_data.VertexID.duplicated(keep=False)]
        


    def prepareset(self, t_scar):
        self.createlesion(t_scar) 
        self.removeduplicates()

    def getData(self):
        return self.lesion_df_data


    
    
