#define HAS_VTK 1

#include "LaImage.h"
#include "LaImageInPaintMask.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application in paints a mask into a greyscale image. It is assumed that mask was created from a cropped version of the greyscale image 
*      Cropping parameters are (x,y,z,l,w,h)
*		Note the header is not required  
*/
int main(int argc, char * argv[])
{
	char* input_bin_fn, *output_fn, *input_img_fn; 
	
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;
    int x,y,z,l=-1,w=-1,h=-1;
	
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
                else if (string(argv[i]) == "-x") {
                    x = atoi(argv[i+1]);
                }
                else if (string(argv[i]) == "-y") {
                    y = atoi(argv[i+1]);
                }
                 else if (string(argv[i]) == "-z") {
                    z = atoi(argv[i+1]);
                }
                else if (string(argv[i]) == "-l") {
                    l = atoi(argv[i+1]);
                }
                else if (string(argv[i]) == "-w") {
                    w = atoi(argv[i+1]);
                }
                else if (string(argv[i]) == "-h") {
                    h = atoi(argv[i+1]);
                }

			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nIn-paints a mask into a greyscale image. \nIt is assumed that mask was created from a cropped"
            "version of the greyscale image. \nCropping parameters are (x,y,z,l,w,h)"
			"\n(Mandatory)\n\t-img <greyscale img> \n\t-bin <binary img>\n\t-o <new image filename>" 
            "\n\t-x, -y, -z (crop origin x,y,z) \n\t-l, -w, -h (crop size l,w,h)";
			
		exit(1);
	}
	else
	{
        
		LaImage* greyscale = new LaImage(input_img_fn);  // copy structure 
        LaImage* binary = new LaImage(input_bin_fn);  // copy structure 
       
        LaImageInPaintMask* algorithm = new LaImageInPaintMask();
		algorithm->SetInputData(greyscale);
		algorithm->SetInputData2(binary);
        algorithm->SetOutputFileName(output_fn);
        algorithm->SetCropOrigin(x,y,z);

        if (l > 0 && w > 0 && h > 0)
            algorithm->SetCropSize(l,w,h);
        algorithm->Update();

        
	}

}