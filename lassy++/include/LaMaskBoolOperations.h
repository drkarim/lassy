/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaMaskOperations performs simple boolean operations on two mask
*
*/
#pragma once
#define BOOL_OR 1 
#define BOOL_AND 2 
#define BOOL_XOR 3
#define BOOL_ANOTB 4 

#include "LaImageAlgorithms.h"




class LaMaskBoolOperations : public LaImageAlgorithms {

private: 
	LaImage* _mask_img1; 
	LaImage* _mask_img2;
	LaImage* _output_img;
	int _which_operation; 

public:
	// Constructor with default values for data members
	/*static LaShellAlgorithms *New();*/
	
	void SetInputData(LaImage* image); 
	void SetInputData2(LaImage* image);

	void Update(); 

	LaImage* GetOutput();
	
	void SetBooleanOperationToOR();
	void SetBooleanOperationToAND();
	void SetBooleanOperationToXOR();
	void SetBooleanOperationToANOTB();


	LaMaskBoolOperations();
	~LaMaskBoolOperations();
	
}; 