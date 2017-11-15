/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaImageAlgorithms is a generalisation for algorithms that work on LaImage objects, i.e. meshes 
*	For more information: https://www.vtk.org/doc/nightly/html/classvtkPolyData.html
*
*/
#pragma once


#include "LaImage.h"



class LaImageAlgorithms {


public:
	// Constructor with default values for data members
	/*static LaShellAlgorithms *New();*/
	
	virtual void SetInputData(LaImage* image); 
	virtual void SetInputData2(LaImage* shell);

	virtual void Update(); 

	virtual LaImage* GetOutput();
	
	

protected:
	LaImageAlgorithms();
	~LaImageAlgorithms();
	
}; 