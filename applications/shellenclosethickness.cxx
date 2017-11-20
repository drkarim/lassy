#define HAS_VTK 1

#include "LaShellEnclosureDistance.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application computes the thickness at each vertex of an enclosed mesh. 
*	   The output is a mesh with the computed thickness at each vertex
*/
int main(int argc, char * argv[])
{
	char* input_f1, *output_f;
	
	bool foundArgs1 = false, foundArgs2 = false;
	
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

	if (!(foundArgs1 || foundArgs2 ))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nCalculates the distance within the shell enclosure"
			"\nThe final enclosed thickness is mapped to the same shell structure"
			"\n(Mandatory)\n\t-i <input_mesh_vtk> \n\t-o <output_vtk>" << endl; 
			
		exit(1);
	}
	else
	{
		LaShell* source = new LaShell(input_f1);
		LaShell* la_out = new LaShell();

		LaShellEnclosureDistance* algorithm = new LaShellEnclosureDistance();
		algorithm->SetLoggingToTrue();
		algorithm->SetInputData(source); 
		
		algorithm->Update();

		la_out = algorithm->GetOutput();

		la_out->ExportVTK(output_f);
	}

}