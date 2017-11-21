#define HAS_VTK 1

#define DO_MEAN 1
#define DO_MEDIAN 2

#include "LaShellShellDisplacement.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application computes the median/mean displacement at each vertex between a source and a list of target shells  
*/
int main(int argc, char * argv[])
{
	char* input_f1, *output_f, *input_f2;
	
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

				else if (string(argv[i]) == "-t") {
					input_f2 = argv[i + 1];
					foundArgs3 = true;
				}

				else if (string(argv[i]) == "-m") {
					method = atoi(argv[i + 1]);
				
				}

			}
			
		}
	}

	if (!(foundArgs1 || foundArgs2 || foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nCalculates the displacement between a source and target shells. "
			"\nNote that multiple target shells can be specified with their filenames as a list within a txt file" 
			"\nNote that by defalt the median displacement is computed\n"
			"\n(Mandatory)\n\t-i <source_mesh_vtk> \n\t-t <target mesh filenames as list txt>\n\t-o <output file>\n== Optional ==\n\t-m (1=mean, 2=median)" << endl; 
			
		exit(1);
	}
	else
	{
		LaShell* source = new LaShell(input_f1);
		LaShell* la_out = new LaShell();

		LaShellShellDisplacement* algorithm = new LaShellShellDisplacement();
		algorithm->SetInputData(source); 
		algorithm->SetInputMultipleTargets(input_f2); 

		algorithm->SetAggregateMethodToMedian(); 

		algorithm->Update();

		la_out = algorithm->GetOutput();

		la_out->ExportVTK(output_f);
	}

}