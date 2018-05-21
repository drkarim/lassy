import numpy as np
import SimpleITK as sitk
import sys

# lesion library 
from lesion.core import ImageLib

imLib = ImageLib.ImageLib()
img = imLib.loadImage(sys.argv[1])

corner = (1,1,1)
dims = (300,300,50)
img2 = imLib.cropImage(img, corner, dims)

img2 = sitk.GetImageFromArray(img2)
imLib.writeImage(img2, sys.argv[2])





