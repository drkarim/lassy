#define HAS_VTK 1


#define DO_MEDIAN 1 
#define DO_MEAN 2 


#include "LaShellProbabilisticAtlas.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application constructs a probabilistic atlas from a list of target shells, preferably with the same topology 
*	   The application requires two inputs, a source shell and a list of target shells. The target shells are used to 
*	   construct the atlas based on the toplogy of the source shell. The aggregated data (median/mean) from target is used
*	   to compute the value at each vertex of the atlas. 
*/
int main(int argc, char * argv[])
{
	char* input_f1, *output_f, *input_f2;

	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;

	int method = DO_MEDIAN, is_topology_equal = USE_DIRECT_COPY; 

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
				else if (string(argv[i]) == "-t") {
					is_topology_equal = atoi(argv[i + 1]);

				}

			}

		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nCalculates the atlas on source from a list of target shells, preferably same topology "
			"\nNote that multiple target shells can be specified with their filenames as a list within a txt file"
			"\nNote that by defalt the median displacement is computed\n"
			"\n(Mandatory)\n\t-i <source_mesh_vtk> \n\t-t <target mesh filenames as list txt>\n\t-o <output file>\n== Optional ==\n\t-m (1=mean, 2=median)"
			"\n-t (target shell topology: 1 - equal, 2 - not equal" << endl;

		exit(1);
	}
	else
	{
		LaShell* source = new LaShell(input_f1);
		LaShell* la_out = new LaShell();

		LaShellProbabilisticAtlas* algorithm = new LaShellProbabilisticAtlas();
		algorithm->SetInputData(source);
		algorithm->SetInputMultipleTargets(input_f2);

		switch (method)
		{
			case DO_MEDIAN:
				algorithm->SetAggregateMethodToMedian();
				break;

			case DO_MEAN:
				algorithm->SetAggregateMethodToMean();
				break;
		}

		switch (is_topology_equal)
		{
			case USE_DIRECT_COPY:
				algorithm->SetAtlasConstructionToUseDirectCopy();
			break;

			case USE_CLOSEST_POINT:
				algorithm->SetAtlasConstructionToUseClosestPoint();
			break;
		}

		algorithm->Update();

		la_out = algorithm->GetOutput();

		la_out->ExportVTK(output_f);
	}

}