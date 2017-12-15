#define HAS_VTK 1

#define DO_MEAN 1
#define DO_MEDIAN 2

#include "LaShellDeformationWithField.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application deforms a mesh based on +ve and -ve distances moved by each vertex in input mesh. 
*	   These distances are exected to be stored in the scalar field array of the input mesh
*/
int main(int argc, char * argv[])
{
	char* input_f1, *output_f;
	
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;

	int method = DO_MEDIAN; 
	
	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-i") {
					input_f1 = argv[i + 1];
					foundArgs1 = true;
				}
				
				else if (string(argv[i]) == "-o") {
					output_f = argv[i + 1];
					foundArgs2 = true; 
				}


			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nDeforms a mesh with deformation field stored as scalars in input mesh "
			"\n(Mandatory)\n\t-i <input_mesh_with_deform_field> \n\t-o <output vtk filename>" << endl; 
			
		exit(1);
	}
	else
	{
		LaShell* source = new LaShell(input_f1);
		LaShell* la_out = new LaShell();

		LaShellDeformationWithField* algorithm = new LaShellDeformationWithField();
		algorithm->SetInputData(source); 
		


		algorithm->Update();

		la_out = algorithm->GetOutput();

		la_out->ExportVTK(output_f);
	}

}