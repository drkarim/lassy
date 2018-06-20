#define HAS_VTK 1

#include "LaImage.h"
#include "LaImageContour.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application will draw the binary image contour on the greyscale image
*
*		Note the header is not required  
*/
int main(int argc, char * argv[])
{
	char* input_bin_fn, *output_fn, *input_img_fn; 
	
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;

	
	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-bin") {
					input_bin_fn = argv[i + 1];
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


			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nDraw contour of binary image on greyscale"
			"\n(Mandatory)\n\t-img <greyscale img> \n\t-bin <binary img>\n\t-o <new image filename>\n\n" << endl; 
			
		exit(1);
	}
	else
	{
        
		LaImage* greyscale = new LaImage(input_img_fn);  // copy structure 
        LaImage* binary = new LaImage(input_bin_fn);  // copy structure 
       
        LaImageContour* algorithm = new LaImageContour();
		algorithm->SetInputData(greyscale);
		algorithm->SetInputData2(binary);

        algorithm->Update();

        LaImage* out_img = new LaImage(); 
        out_img = algorithm->GetOutput();
        out_img->Export(output_fn);
		
	}

}