#define HAS_VTK 1

#include "LaMaskDataShellOperations.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application masks the scalars in a shell, and provides an aggregated output
*      It is useful for performing templated analysis on atria
*/
int main(int argc, char * argv[])
{
	char* input_f1, *input_f2, *output_f;
	int operation = MEAN;
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false, foundArgs4=false;
	bool isSuppress = false; 

	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-d") {
					input_f1 = argv[i + 1];
					foundArgs1 = true;
				}
				else if (string(argv[i]) == "-m") {
					input_f2 = argv[i + 1];
					foundArgs2 = true;
				}

				else if (string(argv[i]) == "-o") {
					output_f = argv[i + 1];
					foundArgs3 = true;
				}
				else if (string(argv[i]) == "-a") {
					operation = atoi(argv[i + 1]);
					foundArgs4 = true;
				}

			}
			else if (string(argv[i]) == "--sup") {
				isSuppress = true; 
			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs4))
	{
		cerr << "Aggregates scalars masked in VTK mesh\nCheck your parameters\n\nUsage:"
			"\n(Mandatory)\n\t-d <1st Scalar data VTK mesh> \n\t-m <Mask data VTK mesh>"
			"\n\t-a <which aggregate method: 1-MEAN, 2-MEDIAN, 3-STDEV>\n"
			"\n\n(Optional)"
            "\n\t-o <output_vtk>\n";
			"\n\t--sup <suppress text decoration, one line output>\n";
			


		exit(1);
	}
	else
	{
		LaShell* source1 = new LaShell(input_f1);       // DATA SHELL
		LaShell* source2 = new LaShell(input_f2);       // MASK SHELL
		LaShell* la_out = new LaShell();

		LaMaskDataShellOperations* algorithm = new LaMaskDataShellOperations();
		algorithm->SetInputData(source1);
		algorithm->SetInputData2(source2);      
		
		switch (operation)
		{
		    case MEAN:
                algorithm->SetOperationToMean();
                algorithm->Update();
                if (!isSuppress) cout << "\n\nMean = ";
                cout << setprecision (2) << fixed << algorithm->GetOutputValue();
				if (!isSuppress) cout << endl;
                break;
			
			case MEDIAN:
                algorithm->SetOperationToMedian();
                algorithm->Update();
                if (!isSuppress) cout << "\n\nMedian = "; 
                cout << setprecision (2) << fixed << algorithm->GetOutputValue();
				if (!isSuppress) cout << endl;
                break;

			case STDEV:
                algorithm->SetOperationToStdev();
                algorithm->Update();
                if (!isSuppress) cout << "\n\nStdev = ";
                cout << setprecision (2) << fixed << algorithm->GetOutputValue();
				if (!isSuppress) cout << endl;
                break;

		}

        if (foundArgs3) {
            la_out = algorithm->GetOutput();
		    la_out->ExportVTK(output_f);
        }
	}

}