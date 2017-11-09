/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellAlgorithms is a generalisation for algorithms that work on LaShell objects, i.e. meshes 
*	For more information: https://www.vtk.org/doc/nightly/html/classvtkPolyData.html
*
*/
#pragma once


#include "LaShell.h"
#include "LaImage.h"
#include "LaImageShellAlgorithms.h"


class LaImageSurfaceNormalAnalysis : public LaImageShellAlgorithms {

private: 
	LaShell* _la_shell;
	LaImage* _la_binary;
	LaImage* _la_image; 
	LaImage* _mask_image;
	bool _vtk_logging; 

	vtkSmartPointer<vtkPolyData> _mesh_3d; 
	
public:
	// Constructor with default values for data members
	/*static LaShellAlgorithms *New();*/
	
	void SetInputDataShell(LaShell* shell); 
	void SetInputDataImage(LaImage* img);
	void SetInputDataBinary(LaImage* bin);
	void SetInputDataImageMask(LaImage* mask);

	void SetVTKLogging();

	void Update(); 

	LaShell* GetOutput(); 
	void SurfaceProjection(bool doLogging = false); 


	LaImageSurfaceNormalAnalysis();
	~LaImageSurfaceNormalAnalysis();
	
}; 