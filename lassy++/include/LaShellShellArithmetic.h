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

#define ADD 1 
#define SUBTRACT 2 
#define MULTIPLY 3 
#define DIVIDE 4


#include "LaShellAlgorithms.h"
#include "LaShell.h"


class LaShellShellArithmetic : public LaShellAlgorithms {

	LaShell* _source_la_1;
	LaShell* _source_la_2;
	LaShell* _output_la;

	int _operation; 
	double _factor;

public:
		
	void SetInputData(LaShell* shell);
	void SetInputData2(LaShell* shell);


	void SetArithmetricOperationToAdd();
	void SetArithmetricOperationToSubtract();
	void SetArithmetricOperationToMultiply();
	void SetArithmetricOperationToDivide();
	void SetFactor(double factor); 

	void Update();

	LaShell* GetOutput();


	LaShellShellArithmetic();
	~LaShellShellArithmetic();
	
}; 