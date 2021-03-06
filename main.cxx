#define HAS_VTK 1

#include "LaShell.h"
#include "VizBox.h"
#include "ShellEntropy.h"
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
	char* input_f, *input_f2, *input_f3, *output_f, *output_poly_fn, *output_txt_fn;
	bool foundArgs1=false, foundArgs2=false; 
	int method=0; 
	float i_value=0;

	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-i") {
					input_f = argv[i + 1];
					foundArgs1 = true;
				}
				else if (string(argv[i]) == "-i2") {
					input_f2 = argv[i + 1];
					
				}
				else if (string(argv[i]) == "-i3") {
					input_f3 = argv[i + 1];

				}
				else if (string(argv[i]) == "-m")
				{
					method = atoi(argv[i + 1]); 

				}
				else if (string(argv[i]) == "-v")
				{
					i_value = atof(argv[i + 1]);

				}
				else if (string(argv[i]) == "-o") {
					output_f = argv[i + 1];
					foundArgs2 = true;
				}

			}
		}
	}                                                                            

	if (!(foundArgs1 ))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\n(Mandatory)\n\t-i <file_with_filenmes.txt> \n\t-m <which method>"
			"\n\n(Optional)\n\t-i2 <second input>\n\t-i3 <third input>"
			"\n\t-o <output file> \n\t-m <which method>" << endl;

		cerr << "\nAvailable methods\n=================\n"
			"\n1. Compute mean of mesh vertex scalars (m=1)\n\tinputs: \n\t-i - Mesh vtk"
			"\n\n2. Pixel value at all image locations (m=2)\n\tinputs: \n\t-i - Input 3D image in NIFTII/NRRD/GIPL format\n\t-o - Output text file"
			"\n\n3. Surface normal interrogation (scar3D) (m=3)\n\tinputs: \n\t-i - Input 3D binary mask\n\t-i2 - Input 3D MRI/CT image\n\t-o - Output VTK mesh"
			"\n\n4. Surface normal interrogation within mask (m=4)\n\tinputs: \n\t-i - Input 3D binary mask\n\t-i2 - Input 3D MRI/CT image\n\t-i3 - Scar 3D mask image\n\t-o - Output VTK mesh" 
			"\n\n5. Visualising a mesh and image from file (m=5)\n\tinputs: \n\t-i - VTK mesh\n\t-i2 - Input 3D MRI/CT image" 
			"\n\n6. Entropy around a point (m=6)\n\tinputs: \n\t-i - VTK mesh\n\t-v - Point ID" 
			"\n\n7. Display contours on mesh (m=7)\n\tinputs: \n\t-i - VTK mesh" << endl;
		exit(1);
	}


	if (method == 1)
	{
		LaShell* la = new LaShell(input_f);
		vector<double> v = la->GetMeshVertexValues(); 

		float average = accumulate(v.begin(), v.end(), 0.0) / v.size();
		cout << "Mean vertex scalar = " << average << endl; 
		

	}
	else if (method == 2)
	{
		LaImage *la = new LaImage(input_f);
		ofstream out; 
		out.open(output_f); 
		short pixelValue; 

		//la->PixelToFile(output_f);
		int maxX, maxY, maxZ; 
		la->GetImageSize(maxX, maxY, maxZ); 
		for (int i = 0; i < maxX; i++)
		{
			for (int j = 0; j < maxY; j++) {
				for (int k = 0; k < maxZ; k++)
				{
					la->GetIntensityAt(i, j, k, pixelValue); 
					//out << pixelValue << ",";
				}
				out << endl; 
			}
			out << endl; 
		}
		out.close(); 

	}

	else if (method == 3) // Mesh normal interrogating of image
	{
		
		LaImage *la_img = new LaImage(input_f);
		LaImage *lge_img = new LaImage(input_f2); 
			
		LaImageSurfaceNormalAnalysis* algorithm = new LaImageSurfaceNormalAnalysis();
		algorithm->SetInputDataImage(lge_img);
		algorithm->SetInputDataBinary(la_img); 
		algorithm->Update();
		
		LaShell* mesh_out = new LaShell();	
		mesh_out = algorithm->GetOutput();
		mesh_out->ConvertToPointData();
		mesh_out->ExportVTK(output_f);
	}
	else if (method == 4)			// Mesh normal interrogating of image, with mask constraint 
	{
		LaImage *bp_mask_img = new LaImage(input_f);
		LaImage *lge_img = new LaImage(input_f2);
		LaImage *scar_mask_img = new LaImage(input_f3);
		
		LaImageSurfaceNormalAnalysis* algorithm = new LaImageSurfaceNormalAnalysis();
		algorithm->SetInputDataImage(lge_img);
		algorithm->SetInputDataBinary(bp_mask_img); 
		algorithm->SetInputDataImageMask(scar_mask_img);
		algorithm->Update();

		LaShell* mesh_out = new LaShell();
		mesh_out = algorithm->GetOutput();
		mesh_out->ConvertToPointData();
		mesh_out->ExportVTK(output_f);
	}
	else if (method == 5)
	{
		cout << "\nMethod = " << method << ", Visualising a 3D mesh and its corresponding 3D image slices, from file" << endl;
		//LaImage *bp_mask_img = new LaImage(input_f);
		LaShell* la_mesh = new LaShell(input_f);
		LaImage *lge_img = new LaImage(input_f2);
		
		
		VizBox* visualiser = new VizBox(); 
		visualiser->ConstructImageOrthogonalPlanes(lge_img); 
		visualiser->ConstructMeshVisualiser(la_mesh);
		visualiser->ShowInit();
	}
	else if (method == 6)
	{
		cout << "\nMethod = " << method << ", Entropy of a mesh vertex around point with ID " << i_value << endl;
		//LaShell* la_mesh = new LaShell(input_f);

		ShellEntropy* entropy = new ShellEntropy(input_f); 
		entropy->GetPointEntropy(i_value); 

	}
	else if (method == 7)
	{
		LaShell* la_mesh = new LaShell(input_f);
		VizBox* visualiser = new VizBox();
		visualiser->ConstructMeshVisualiser(la_mesh);
		visualiser->ShowInit();
	}
	

}
