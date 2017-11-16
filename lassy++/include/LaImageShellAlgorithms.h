/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaImgaeShellAlgorithms is a generalisation for algorithms that work on LaShell and LaImage objects
*	For more information: https://www.vtk.org/doc/nightly/html/classvtkPolyData.html
*
*/
#pragma once


#define ADD 1
#define SUBTRACT 2
#define MULTIPLY 3
#define DIVIDE 4


#include "LaShell.h"
#include "LaImage.h"


class LaImageShellAlgorithms {


	
public:
	
	virtual void SetInputDataShell(LaShell* shell); 
	virtual void SetInputDataImage(LaImage* shell);

	virtual void Update(); 

	virtual LaShell* GetOutput(); 

protected:
	LaImageShellAlgorithms();
	~LaImageShellAlgorithms();
	
}; 