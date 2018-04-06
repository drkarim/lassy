#define HAS_VTK 1

#include "LaImage.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application reads a binary pixel values from a CSV text file
*/
int main(int argc, char * argv[])
{
	char* input_csv_fn, *output_fn, *input_img_fn; 
	
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


			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nCreate a binary image from CSV"
			"\n(Mandatory)\n\t-csv <csv input> \n\t-img <image for structure>\n\t-o <new image filename>\n\n" << endl; 
			
		exit(1);
	}
	else
	{
        
		LaImage* img = new LaImage(input_img_fn);  // copy structure 
        img->EmptyImage(); // Sets all pixels to 0
		img->FileToPixel(input_csv_fn);

        img->Export(output_fn);
		
	}

}