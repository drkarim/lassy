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


#include "LaShellAlgorithms.h"



class LaShellEnclosureDistance : public LaShellAlgorithms {


	
public:
	// Constructor with default values for data members
	/*static LaShellAlgorithms *New();*/
	
	virtual void SetInputData(LaShell* shell); 
	virtual void SetInputData2(LaShell* shell);

	virtual void Update(); 

	virtual LaShell* GetOutput(); 

	LaShellEnclosureDistance();
	~LaShellEnclosureDistance();
	
}; 