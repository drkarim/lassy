# Lassy
<p>
Lassy is a C++ library useful for mining imaging data, especially around an object of interest. It is compatible with VTK meshes and NIFTII or NRRD images. 
  
Image data mining is performed using surface normal analysis. The image intensity of surface normal profiles is returned. The library is also capable of mining data from surface meshes. Please see full list of applications, or get in touch if you need a functionality which this library can provide. 
  
This library was built for mining medical imaging data. A simple example application would be to interrograte the image around an organ or anatomical structure in a medical 3D image. Regions around an organ's medical image are usually interesting. In cardiac imaging, the region around the heart's chamber could show areas of damage sustained from cardiac arrest. 

<img align="right" src="http://catlikecoding.com/unity/tutorials/noise-derivatives/05-mesh-normals.png" width="250">

The project is undergoing continuous iteration with new functionalities added from time to time. If you would like to see a new functionality, please get in touch. The growing list of functionalities can be found below. 

There are unfortunately some pre-requisities for the images this library can process. The 3D images are expected to be in the medical image formats: NifTII, NRRD, Analyze and GIPL. The meshes can be in [VTK](https://www.vtk.org/) format. 

If you use this for research, we would appreciate if you could reference our work [Karim et al.](http://ieeexplore.ieee.org/document/6774956/?reload=true&tp=&arnumber=6774956&searchWithin%3Dp_First_Names:rashed%26searchWithin%3Dp_Last_Names:karim%26matchBoolean%3Dtrue%26queryText%3D(p_Authors:karim,%20rashed)) and [Knowles et al.](http://ieeexplore.ieee.org/document/5415610/). Also, visit my wiki to read further on our [cardiac imaging research projects](https://wwwhomes.doc.ic.ac.uk/~rkarim/mediawiki/index.php?title=Research_Wiki)
</p>

## Dependencies 
This library has some dependencies as it requires to perform 3D medical image processing and process 3D surface meshes. For these purposes, it depends on the [Image Registration Toolkit (ITK)](https://itk.org/) and the [Visulisation Toolkit (VTK)](https://www.vtk.org/). Both are open-source and written in C++. VTK is released under the 3-Clause BSD license and ITK was released under the Apache 2.0 license. 

## Installation 
The library currently has two dependencies: [ITK](https://itk.org/) and [VTK](https://www.vtk.org/). These need to be built from source first using [CMake](https://cmake.org/). There are Github repositories for [ITK](https://github.com/InsightSoftwareConsortium/ITK) and [VTK](https://github.com/Kitware/VTK)

Once the ITK and VTK toolkits are built, this library can be built from source by running [CMake](https://cmake.org/) on the source files. Windows binaries will also be available soon and can be requested. 



## Usage
<p>
<img align="right" src="https://wwwhomes.doc.ic.ac.uk/~rkarim/mediawiki/images/3/36/Lassy_preview.PNG" width="250">

The ```applications``` folder contains several examples of command-line tools that utilise this library. 
</p>
<p>
The list of applications continue to grow: 
  
  1. Image intensity interrogation around object of interest ```applications\surf.cxx```
  2. Distance to an adjacent mesh using interrogating normals (INTERN) ```applications\wallthickness.cxx```
  3. Distance within a convex object mesh using INTERNs ```applications\shellenclosethickness.cxx```
  4. Copying data from an adjcent mesh using INTERNs ```applications\target2source.cxx```
  5. Ordinary statistics (Median and mean) on scalars of mesh vertices  ```applications\shellstats.cxx```
  6. Boolean operations on scalars of two meshes ```applications\boolmask.cxx```
  7. Compute vertex displacements to a deformed mesh ```applications\displacement.cxx```
  8. Mining image data along a user-defined trajectory on a mesh ```applications\encirclement.cxx```
  9. Extracting image features as CSV for on-ward pixel classification  ```applications\imagefeature.cxx```
  10. Aggregate scalars inside a mask ```applications\maskshell.cxx```
  11. Reads pixel values as CSV file to update an image ```applications\createbinaryimage.cxx```
  12. Reads 3D points and scalars from CSV and maps them to the mesh ```applications\csvshell.cxx```
  13. Scalar union of two geometrically exact meshes ```applications\combshell.cxx```
 
</p>

## Platforms 
Lassy has been tested on Windows 7 on Visual Studio 2015 and MacOS Sierra 10.12 running native g++ compilers. 

## License 
The software has been released under the [Apache 2.0](https://en.wikipedia.org/wiki/Apache_License#Version_2.0) licensing terms. 

## Author 

```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
