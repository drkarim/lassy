/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellExtractArray enables extraction of arrays in multi-labelled point data scalars 
*
*/
#pragma once


#include "LaShellAlgorithms.h"



class LaShellExtractArray : public LaShellAlgorithms {

private: 
	LaShell* _source_la;
	LaShell* _output_la; 
	
	string _array_name;
	
public:
	// Constructor with default values for data members
	/*static LaShellAlgorithms *New();*/
	
	void SetInputData(LaShell* shell); 
	
	void SetArrayName(const char* array_name);
	void Update(); 

	LaShell* GetOutput(); 

	LaShellExtractArray();
	~LaShellExtractArray();
	
}; 