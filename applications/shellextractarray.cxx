#define HAS_VTK 1

#include "LaShellExtractArray.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*
*
*/
int main(int argc, char * argv[])
{
	char* input_f1, *output_f, *input_f2; 
	
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;
	
	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-i") {
					input_f1 = argv[i + 1];
					foundArgs1 = true;
				}
				if (string(argv[i]) == "-a") {
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

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\n(Mandatory)\n\t-i <source mesh>\n\t-a <array name>\n\t-o <output vtk>" << endl; 
			

		exit(1);
	}
	else
	{
		LaShell* source = new LaShell(input_f1);
		LaShell* la_out = new LaShell();
		
		LaShellExtractArray* wt = new LaShellExtractArray();
		wt->SetInputData(source); 
		wt->SetArrayName(input_f2);

		wt->Update(); 

		la_out = wt->GetOutput(); 

		la_out->ExportVTK(output_f);
	}

}