#define HAS_VTK 1

#include "LaShellWallThickness.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/
int main(int argc, char * argv[])
{
	char* input_f1, *input_f2,  *output_f;
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3=false; 
	
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

			}
		}
	}

	if (!(foundArgs1 || foundArgs2 || foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\n(Mandatory)\n\t-i1 <source_mesh_vtk> \n\t-i2 <target_mesh_vtk> \n\t-o <output_vtk>" << endl; 
			

		exit(1);
	}
	else
	{
		LaShell* la1 = new LaShell(input_f1);
		LaShell* la2 = new LaShell(input_f2);
		LaShell* la_out = new LaShell(input_f2);

		LaShellWallThickness* wt = new LaShellWallThickness();
		wt->SetInputData(la1); 
		wt->SetInputData2(la2); 
		wt->Update(); 

		la_out = wt->GetOutput(); 

		la_out->ExportVTK(output_f);
	}

}