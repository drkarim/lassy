# Lassy

Lassy is an experimental C++ library for encapsulating and analysing data generated in cardiac eletrophysiology research. Lassy currently incorporates models only for the left venricle and atrium. 

It is being currently developed for computing the complexity of scar in [Re-entrant ventricular tachycardia research](https://wwwhomes.doc.ic.ac.uk/~rkarim/mediawiki/index.php?title=Scar_complexity)

## Dependencies 

This library has some dependencies as it requires to perform 3D medical image processing and process 3D surface meshes. For these purposes, it depends on the [Image Registration Toolkit (ITK)](https://itk.org/) and the [Visulisation Toolkit](https://www.vtk.org/). Both are open-source and written in C++. VTK is released under the 3-Clause BSD license and ITK was released under the Apache 2.0 license. 


## Usage
It contains a tool that can be used to interrogate late gadolinium enhancement CMR images of the ventricular myocardium. 

It takes input parameters such as:  

```-i``` Ventricular myocardial shell in VTK format 


<img src="https://wwwhomes.doc.ic.ac.uk/~rkarim/mediawiki/images/3/36/Lassy_preview.PNG" width="50">

## Author 

```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
