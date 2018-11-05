#define HAS_VTK 1

#include "LaImage.h"
#include "LaImageMarkPoints.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application will draw points from physical space to a greyscale image.
*       Points are input as a CSV file
*/
int main(int argc, char * argv[])
{
	char* input_csv_fn, *output_fn, *input_img_fn; 
	double scaling=-1;
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;

	
	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-csv") {
					input_csv_fn = argv[i + 1];
					foundArgs1 = true;
				}
				
				else if (string(argv[i]) == "-img") {
					input_img_fn = argv[i + 1];
					foundArgs2 = true; 
				}
			
				else if (string(argv[i]) == "-o") {
					output_fn = argv[i + 1];
                    foundArgs3 = true; 
					
				}
                else if (string(argv[i]) == "-scaling") {
					scaling = atof(argv[i + 1]);
                    
					
				}


			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nDraw points in a greyscale image, points are input as csv"
			"\n(Mandatory)\n\t-img <greyscale img> \n\t-csv <csv containing points>\n\t-o <new image filename>\n\n" << endl; 
			
		exit(1);
	}
	else
	{
        
		LaImage* greyscale = new LaImage(input_img_fn);  // copy structure 
        
       
        LaImageMarkPoints* algorithm = new LaImageMarkPoints();
		algorithm->SetInputData(greyscale);
		algorithm->SetInputCSVFileName(input_csv_fn);
        
        if (scaling > 0)
        {
            algorithm->SetScalingFactor(scaling);
        }

        algorithm->Update();

        LaImage* out_img = new LaImage(); 
        out_img = algorithm->GetOutput();
        out_img->Export(output_fn);
		
	}

}