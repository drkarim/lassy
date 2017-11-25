/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellShellArithmetic is performs simple arithmetic operations on scalars contained within two VTK meshes 
*	
*
*/
#pragma once

#define DO_STANDARD_STATS 1 


#include "LaShellAlgorithms.h"
#include "LaShell.h"

#include "MathBox.h"
#include <string>
#include <map>

using namespace std; 

class LaShellStatistics : public LaShellAlgorithms {

	LaShell* _source_la;
	
	int _operation; 
    map<string, double> _stats;

public:
		
	void SetInputData(LaShell* shell);
	void SetOperationToStandardStats();
	
    void Update();

	double GetOutputValue(const char* label);


	LaShellStatistics();
	~LaShellStatistics();
	
}; 