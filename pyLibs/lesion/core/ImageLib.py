import SimpleITK as sitk
import numpy as np
import copy


class ImageLib:
    

    def __init__(self):
        pass


    def loadImage(self, pathName):
        im = []
        im = sitk.GetArrayFromImage(sitk.ReadImage(pathName))
        return im

    def loadImages(self, pathNames):
        im_out = []
        for p in pathNames:
            im = sitk.GetArrayFromImage(sitk.ReadImage())
            im_out.append(im)

        return im_out


    def loadImageSpacing(self, pathName):
        return sitk.ReadImage(pathName).GetSpacing()
    

    def cropImage(self, I, corner, dims):
        c = copy.copy(corner)
        for i in range(3):
            if c[i] < 0:
                pad_elem = [(0, 0), (0, 0), (0, 0)]
                pad_elem[i] = (-c[i], dims[i] + c[i] - I.shape[i])
                pad_elem = tuple(pad_elem)
                I = np.pad(I, pad_elem, 'constant', constant_values=0)
                c[i] = 0

        d, h, w = dims
        z, y, x = c
        return I[z:z+d, y:y+h, x:x+w]

    def writeImage(self, I, pathName):
        sitk.WriteImage(I, pathName)
