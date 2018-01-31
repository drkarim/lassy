#define HAS_VTK 1

#define DO_MEAN 1
#define DO_MEDIAN 2

#include "LaShellGapsInBinary.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application computes the gaps within a user-defined path on a binarised shell
*/
int main(int argc, char * argv[])
{
	char* input_f1, *output_f;
	double fill_threshold = 0.5; 

	bool foundArgs1 = false, foundArgs2 = false;
	
	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-i") {
					input_f1 = argv[i + 1];
					foundArgs1 = true;
				}
				
				
				else if (string(argv[i]) == "-t") {
					fill_threshold = atof(argv[i + 1]);
					foundArgs2 = true;
				}

				

			}
			
		}
	}

	if (!(foundArgs1))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nCalculates the coverage of data or encirclement along a user-defined path on a shell "
			"\n(Mandatory)\n\t-i <source_mesh_vtk>"
			"\n\n(optional)\n\t-t <the threshold value for determining filling>" << endl; 
			
		exit(1);
	}
	else
	{
		LaShell* source = new LaShell(input_f1);
		
		LaShellGapsInBinary* application = new LaShellGapsInBinary();
		application->SetInputData(source); 
		

		application->Run();
		


	}

}