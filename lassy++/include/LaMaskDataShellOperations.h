/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaMaskDataShellOperations takes two mesh, the cumulative scalar of the shell covered by the mask 
*   is returned
*/
#pragma once

#define MEAN 1 
#define MEDIAN 2 
#define STDEV 3

#include "LaShellAlgorithms.h"
#include "vtkPointLocator.h"
#include "LaShell.h"
#include "MathBox.h"



class LaMaskDataShellOperations : public LaShellAlgorithms {

private: 
	LaShell* _data_shell; 
	LaShell* _mask_shell;
	
	LaShell* _output_la;
	int _which_operation; 
	double _scalar_aggregate; 

public:
	// Constructor with default values for data members
	/*static LaShellAlgorithms *New();*/
	
	void SetInputData(LaShell* data); 
	void SetInputData2(LaShell* mask);

	void Update(); 

	double GetOutputValue();
	LaShell* GetOutput();
	
	void SetOperationToMean();
	void SetOperationToMedian();
	void SetOperationToStdev();
	

	LaMaskDataShellOperations();
	~LaMaskDataShellOperations();
	
}; 