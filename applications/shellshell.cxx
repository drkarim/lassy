#define HAS_VTK 1

#include "LaShellShellArithmetic.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application performs simple arithmetic operation on the vertex scalars of two meshes 
*	   The input are two meshes with the same topology or at least the exact same number of points 
*	   The arthmetic operations are pre-defined as addition, subtraction, multiplication and division 
*	   The output is a mesh with the resulting scalars from the pre-defined arithmetic operation
*/
int main(int argc, char * argv[])
{
	char* input_f1, *input_f2, *output_f;
	int operation = ADD;
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false, foundArgs4=false;


	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-i1") {
					input_f1 = argv[i + 1];
					foundArgs1 = true;
				}
				else if (string(argv[i]) == "-i2") {
					input_f2 = argv[i + 1];
					foundArgs2 = true;
				}

				else if (string(argv[i]) == "-o") {
					output_f = argv[i + 1];
					foundArgs3 = true;
				}
				else if (string(argv[i]) == "-p") {
					operation = atoi(argv[i + 1]);
					foundArgs4 = true;
				}

			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Performs simple arithmetic operations on scalars contained within two VTK meshes\nCheck your parameters\n\nUsage:"
			"\n(Mandatory)\n\t-i1 <1st Mesh in VTK> \n\t-i2 <2nd Mesh in VTK>"
			"\n\t-o <output_vtk>"
			"\n\n(Optional)"
			"\n\t-p <which operation: 1-ADD, 2-SUBTRACT, 3-MULTIPLY, 4-DIVIDE>\n" << endl;


		exit(1);
	}
	else
	{
		LaShell* source1 = new LaShell(input_f1);
		LaShell* source2 = new LaShell(input_f2);
		LaShell* la_out = new LaShell(input_f2);

		LaShellShellArithmetic* algorithm = new LaShellShellArithmetic();
		algorithm->SetInputData(source1);
		algorithm->SetInputData2(source2);
		
		switch (operation)
		{
		case ADD:
			algorithm->SetArithmetricOperationToAdd();
			break;
		case SUBTRACT:
			algorithm->SetArithmetricOperationToSubtract();
			break;
		case MULTIPLY:
			algorithm->SetArithmetricOperationToMultiply();
			break;
		case DIVIDE:
			algorithm->SetArithmetricOperationToDivide();
			break;

		}

		
		algorithm->Update();

		la_out = algorithm->GetOutput();

		la_out->ExportVTK(output_f);
	}

}