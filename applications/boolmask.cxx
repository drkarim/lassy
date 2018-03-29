#define HAS_VTK 1

#define DO_AND 1
#define DO_OR 2
#define DO_XOR 3
#define DO_ANOTB 4

#include "LaMaskBoolOperations.h"
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
	
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;

	int method = DO_OR; 
	
	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-i1") {
					input_f1 = argv[i + 1];
					foundArgs1 = true;
				}
				
				else if (string(argv[i]) == "-o") {
					output_f = argv[i + 1];
					foundArgs2 = true; 
				}

				else if (string(argv[i]) == "-i2") {
					input_f2 = argv[i + 1];
					foundArgs3 = true;
				}

				else if (string(argv[i]) == "-m") {
					method = atoi(argv[i + 1]);
				
				}

			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nPerforms boolean operations on two masks"
			"\n(Mandatory)\n\t-i1 <mask1> \n\t-i2 <mask2>\n\t-o <output filename>\n"
			"== Optional =="
			"\n\t-m <which operation> (1=AND, 2=OR, 3-XOR, 4-A_NOT_B)" << endl; 
			
		exit(1);
	}
	else
	{
		LaImage* mask1 = new LaImage(input_f1);
		LaImage* mask2 = new LaImage(input_f2);
		LaImage* img_out = new LaImage();

		LaMaskBoolOperations* algorithm = new LaMaskBoolOperations();
		algorithm->SetInputData(mask1); 
		algorithm->SetInputData2(mask2); 

		switch (method)
		{
			case DO_AND: 
				algorithm->SetBooleanOperationToAND();
				break; 
			case DO_OR: 
				algorithm->SetBooleanOperationToOR();
				break; 
			case DO_XOR: 
				algorithm->SetBooleanOperationToXOR();
				break; 
			case DO_ANOTB: 
				algorithm->SetBooleanOperationToANOTB();
				break;
		}
		

		algorithm->Update();

		img_out = algorithm->GetOutput();

		img_out->Export(output_f);
	}

}