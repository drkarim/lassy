#define HAS_VTK 1

#include "LaImage.h"
#include "LaImageHoleFilling.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application fills holes in an image's pixels with mean of its non-zero neighbours
*/
int main(int argc, char * argv[])
{
	char* output_fn, *input_img_fn; 
	int nn=1, iter=1, direc=XYZ;;

	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;
    
	
	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				
				 if (string(argv[i]) == "-img") {
					input_img_fn = argv[i + 1];
					foundArgs1 = true; 
				}
			
				else if (string(argv[i]) == "-o") {
					output_fn = argv[i + 1];
                    foundArgs2 = true; 
					
				}
                else if (string(argv[i]) == "-nn") {
                    nn = atoi(argv[i+1]);
                }
                else if (string(argv[i]) == "-iter") {
                    iter = atoi(argv[i+1]);
                }
                
               
			}
            else if (string(argv[i]) == "--xy") {
                direc = XY;
            }
            else if (string(argv[i]) == "--yz") {
                direc = YZ;
            }
            else if (string(argv[i]) == "--xz") {
                direc = XZ;
            }
			
		}
	}

	if (!(foundArgs1 && foundArgs2 ))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nfills holes in an image's pixels with mean of its non-zero neighbours"
			"\n(Mandatory)\n\t-img <img> \n\t-o <new image filename>"
            "\n(Optional)\n\t-nn min neighbours required for mean"
            "\n\t-iter number of iterations (default 1)\n";
            
		exit(1);
	}
	else
	{
        
		LaImageFloat* greyscale = new LaImageFloat(input_img_fn);  // copy structure 
        
       
        LaImageHoleFilling* algorithm = new LaImageHoleFilling();
		algorithm->SetInputData(greyscale);
		algorithm->SetOutputFileName(output_fn);
        algorithm->SetMinNeighboursForMean(nn);
        algorithm->SetNumberOfIterations(iter);
        algorithm->SetInterpolateDirection(direc);

        cout << "Running algorithm with\n\tNeighour size = " << nn << "\n\tIterations"
        "= " << iter << "\n\tdirection = " << direc << endl;
        algorithm->Update();

        
	}

}