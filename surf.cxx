#define HAS_VTK 1

#include "LaImageSurfaceNormalAnalysis.h"
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
	int direction = 1; 
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
			"\n(Mandatory)\n\t-i1 <3d image> \n\t-i2 <bin mask image> \n\t-o <output_vtk>" << endl; 
			

		exit(1);
	}
	else
	{
		LaImage* image = new LaImage(input_f1);
		LaImage* bin_image = new LaImage(input_f2);
		
		LaShell* mesh_out = new LaShell();

		LaImageSurfaceNormalAnalysis* algorithm = new LaImageSurfaceNormalAnalysis();
		algorithm->SetInputDataImage(image);
		algorithm->SetInputDataBinary(bin_image); 
		algorithm->SetStepSize(4.0);
		
		algorithm->Update();
		
		mesh_out = algorithm->GetOutput();
		mesh_out->ConvertToPointData();
		mesh_out->ExportVTK(output_f);
	}

}