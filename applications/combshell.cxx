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
	char* input_f1, *input_f2, *output_f, *scalar_array_name, *scalar_array_name1, *scalar_array_name2, *output_scalar_name;
	int operation = WHEN_OVERLAP_USE_SOURCE1;
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false, foundArgs4=false, foundArgs5=false, foundArgs6=false;
	bool is_field_data = false;


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
                else if (string(argv[i]) == "-scalar1") {
					scalar_array_name1 = argv[i + 1];
					foundArgs5 = true;
				}
				else if (string(argv[i]) == "-scalar2") {
					scalar_array_name2 = argv[i + 1];
					foundArgs6 = true;
					
				}
                else if (string(argv[i]) == "-scalar_out") {
					output_scalar_name = argv[i + 1];
					
				}

			}
			else if (string(argv[i]) == "--field") {
				is_field_data = true; 
			}
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3 && foundArgs5 && foundArgs6))
	{
		cerr << "Combines scalars from two meshes\nCheck your parameters\n\nUsage:"
			"\n(Mandatory)\n\t-i1 <1st Mesh in VTK> \n\t-i2 <2nd Mesh in VTK>"
			"\n\t-o <output_vtk>"
			"\n\t-scalar1 <input scalar array name in mesh 1>"
			"\n\t-scalar2 <input scalar array name in mesh 2>"
			"\n\t-scalar_out <output scalar array name in output mesh>"
			"\n\n(Optional)"
			"\n\t--field (read and write as field data)"
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

		
		

		if (is_field_data)
		{
			cout << "\nReading and writing field data" << endl;
			algorithm->SetWriteDataToField();
		}

		switch (operation)
		{
            case WHEN_OVERLAP_USE_SOURCE1:
                algorithm->SetOverlapToSource1();
                break;
            case WHEN_OVERLAP_USE_SOURCE2:
                algorithm->SetOverlapToSource2();
                break;
         
		}

		if (algorithm->SetScalarArrayNames(scalar_array_name1, scalar_array_name2)) 
        {

			algorithm->SetOutputScalarName(output_scalar_name);
            algorithm->Update();

            la_out = algorithm->GetOutput();

            la_out->ExportVTK(output_f);
        }
	}

}