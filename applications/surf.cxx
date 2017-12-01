#define HAS_VTK 1

#include "LaImageSurfaceNormalAnalysis.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application interrogates the intensities along the vertex normals of an object of interest (OFI)
*	   The OFI is provided by the user as a binary mask image (0 outside/1 inside). 
*	   The intensity image is also provided as a 3D image
*	   There optional parameters such as mean and standard deviation, so the interrogated intensty can be z-scored 
*	   The interrogated intensities are aggregated as either maximum intensity or integral sum (AIT)
*	   The output mesh is the surface of the OFI with the AIT computed for each vertex
*
*/
int main(int argc, char * argv[])
{
	char* input_f1, *input_f2, *input_f3,  *output_f;
	int direction = 1; 
	double mean, std, step_size=4.0; 
	int aggregate_method=1; 
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3=false, foundArgs4=false, foundArgs5=false, foundArgs6=false, foundArgs7=false;
	
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
				else if (string(argv[i]) == "-i3") {
					input_f3 = argv[i + 1];
					foundArgs3 = true;
				}
				else if (string(argv[i]) == "-o") {
					output_f = argv[i + 1];
					foundArgs4 = true; 
				}
				else if (string(argv[i]) == "-m") {
					mean = atof(argv[i + 1]);
					foundArgs5 = true;
				}
				else if (string(argv[i]) == "-s") {
					std = atof(argv[i + 1]);
					foundArgs6 = true;
				}
				else if (string(argv[i]) == "-t") {
					aggregate_method = atoi(argv[i + 1]);
					foundArgs7 = true;
				}
				else if (string(argv[i]) == "-p") {
					step_size = atof(argv[i + 1]);
					
				}

			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs4))
	{
		cerr << "Performs surface normal analysis\nCheeck your parameters\n\nUsage:"
			"\n(Mandatory)\n\t-i1 <intensity image for intterogation> \n\t-i2 <binary image for surface>" 
			"\n\t-o <output_vtk>"
			"\n\n(Optional)"
			"\n\t-i3 <surface depth map for normal extent>"
			"\n\t-m <mean for z-scoring> \n\t-s <std for z-scoring>"
			"\n\t-t <1-max, 2-integral>"
			"\n\t-p <size of normal>\n" << endl; 
			
		exit(1);
	}
	else
	{
		LaImage* image = new LaImage(input_f1);
		LaImage* bin_image = new LaImage(input_f2);
		
		LaShell* thickness_map;

		LaShell* mesh_out = new LaShell();

		LaImageSurfaceNormalAnalysis* algorithm = new LaImageSurfaceNormalAnalysis();
		algorithm->SetInputDataImage(image);
		algorithm->SetInputDataBinary(bin_image); 
				
		if (foundArgs3) {
			cout << "Note: Using wall thickness map for traversing normals .." << endl; 
			thickness_map = new LaShell(input_f3);
			algorithm->SetInputNormalStepShell(thickness_map);
		}
		else {
			cout << "\nUsing a constant normal size of: "  << step_size << endl;
			algorithm->SetStepSize(step_size);
		}

		if (aggregate_method == 1)
		{
			cout << "\nAggregate method: Max" << endl;
			algorithm->SetAggregationMethodToMax();
		}
		else if (aggregate_method == 2)	
		{
			cout << "\nAggregate method: Integral" << endl;
			algorithm->SetAggregationMethodToIntegral();
		}

		if (foundArgs5 && foundArgs6)
		{
			algorithm->SetZScoreMean(mean);
			algorithm->SetZScoreStd(std);
		}

		algorithm->Update();
		mesh_out = algorithm->GetOutput();
		mesh_out->ExportVTK(output_f);
	}

}