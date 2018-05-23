#define HAS_VTK 1

#include "LaShellShellCombine.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application demonstrates the LaShellShellCombine class
*/
int main(int argc, char * argv[])
{
	char* input_f1, *input_f2, *output_f, *scalar_array_name;
	int operation = WHEN_OVERLAP_USE_SOURCE1;
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false, foundArgs4=false, foundArgs5=false;


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
				else if (string(argv[i]) == "-overlap") {
					operation = atoi(argv[i + 1]);
					foundArgs4 = true;
				}
                else if (string(argv[i]) == "-scalar") {
					scalar_array_name = argv[i + 1];
					foundArgs5 = true;
				}
                

			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3 && foundArgs5))
	{
		cerr << "Combines scalars from two meshes\nCheck your parameters\n\nUsage:"
			"\n(Mandatory)\n\t-i1 <1st Mesh in VTK> \n\t-i2 <2nd Mesh in VTK>"
			"\n\t-o <output_vtk>"
			"\n\n(Optional)"
			"\n\t-overlap <when overlap, which scalar?: 1-Scalar of Mesh-1, 2-Scalar of Mesh-2>\n" << endl;


		exit(1);
	}
	else
	{
		LaShell* source1 = new LaShell(input_f1);
		LaShell* source2 = new LaShell(input_f2);
		LaShell* la_out = new LaShell(input_f2);

		LaShellShellCombine* algorithm = new LaShellShellCombine();
		algorithm->SetInputData(source1);
		algorithm->SetInputData2(source2);
		
		switch (operation)
		{
            case WHEN_OVERLAP_USE_SOURCE1:
                algorithm->SetOverlapToSource1();
                break;
            case WHEN_OVERLAP_USE_SOURCE2:
                algorithm->SetOverlapToSource2();
                break;
         
		}

		if (algorithm->SetScalarArrayName(scalar_array_name)) 
        {
            algorithm->Update();

            la_out = algorithm->GetOutput();

            la_out->ExportVTK(output_f);
        }
	}

}