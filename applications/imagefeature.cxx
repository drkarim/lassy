#define HAS_VTK 1

#define DO_AND 1
#define DO_OR 2
#define DO_XOR 3

#include "LaImageFeatures.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application performs boolean ooerations on masks
*/
int main(int argc, char * argv[])
{
	char* input_f1, *output_f, *input_f2; 
	short pixel_value=1; 
	
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;
	bool isHaralick = false;
	int method = DO_OR; 
	
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

				else if (string(argv[i]) == "-m") {
					input_f2 = argv[i + 1];
					foundArgs3 = true;
				}

				else if (string(argv[i]) == "-p") {
					pixel_value = atoi(argv[i + 1]);
					
				}

			
			}
			else if (string(argv[i]) == "--haralick") {
				isHaralick = true;
			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nExtract image features"
			"\n(Mandatory)\n\t-i <img> \n\t-m <binary mask>\n\t-o <new csv filename>\n\t-p pixel value to read\n\n"
			"(Optional)\n\t--haralick - For Haralick feature output" << endl;
			
		exit(1);
	}
	else
	{
		cout << "Only reading pixels with value " << pixel_value << endl;
		LaImage* org_img = new LaImage(input_f1);
		LaImage* mask = new LaImage(input_f2);
		

		LaImageFeatures* algorithm = new LaImageFeatures();
		algorithm->SetInputData(org_img);
		algorithm->SetInputData2(mask); 
		algorithm->SetOutputFile(output_f);
		
		
		if (isHaralick)
			algorithm->Update_Haralick();
		else 
			algorithm->Update();

		
	}

}