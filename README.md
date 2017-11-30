# Lassy
<p>
Lassy is a C++ library useful for interrogating the image around an object of interest using surface normal analysis. This is performed using point or vertex normal profiles of the surface mesh.
  
In a typical use case, a surface of the 3D object of interest is generated. The surface normal intensity profiles are then used for interrogating the 3D image around the object of interest. 

A simple example application of this library would be to interrograte the image around an organ or anatomical structure in a medical 3D image. There are special applications in cardiac imaging for which this library was built, especially to interrogate the image around chambers of the heart. 

<img align="right" src="http://catlikecoding.com/unity/tutorials/noise-derivatives/05-mesh-normals.png" width="250">

The project is undergoing continuous iteration and this means that the library has now several more functionalities apart from interrogating intensities around an object an interest. The interrogating normals (INTERN) can be used in other applications such as finding point distances to an adjacent object, copying data from an adjacent oject, finding thickness of a convex object. 

The 3D images are expected to be in the medical image formats: NifTII, NRRD, Analyze and GIPL. The meshes can be in [VTK](https://www.vtk.org/) format. 

It is being currently developed for visualising pixel enhancments in MRI imaging in [cardiac imaging research](https://wwwhomes.doc.ic.ac.uk/~rkarim/mediawiki/index.php?title=Research_Wiki)
</p>

## Dependencies 
This library has some dependencies as it requires to perform 3D medical image processing and process 3D surface meshes. For these purposes, it depends on the [Image Registration Toolkit (ITK)](https://itk.org/) and the [Visulisation Toolkit](https://www.vtk.org/). Both are open-source and written in C++. VTK is released under the 3-Clause BSD license and ITK was released under the Apache 2.0 license. 

## Installation 
<p>
The library currently has two dependencies: [Image Registration Toolkit (ITK)](https://itk.org/) and the [VTK](https://www.vtk.org/). These need to be built from source first using [CMake](https://cmake.org/). There are Github repositories for [ITK](https://github.com/InsightSoftwareConsortium/ITK) and [VTK](https://github.com/Kitware/VTK)
</p>

<p>
Once the ITK and VTK toolkits are built, this library can be built from source using CMake. Windows binaries will also be available soon and can be requested. 
</p>


## Usage
<p>
<img align="right" src="https://wwwhomes.doc.ic.ac.uk/~rkarim/mediawiki/images/3/36/Lassy_preview.PNG" width="250">

The ```applications``` folder contains several examples of command-line tools that utilise this library. 
</p>
<p>
The list of applications continue to grow: 
  
  1. Image intensity interrogation around object of interest
  2. Distance to an adjacent object using interrogating normals (INTERN)
  3. Distance within a convex object using INTERNs 
  4. Copying data from an adjcent object using INTERNs
 
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
