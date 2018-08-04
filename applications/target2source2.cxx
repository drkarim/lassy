#define HAS_VTK 1

#include "LaShellShellIntersectionMultiArray.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*
*		Copies scalars with multiarray support from source to target
*       Provides two options for copy - closest point along surface normal, point id 
*/
int main(int argc, char * argv[])
{
	char* input_f1, *input_f2,  *output_f;
	bool use_point_id = false; 
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3=false; 
	
	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-source") {
					input_f1 = argv[i + 1];
					foundArgs1 = true;
				}
				else if (string(argv[i]) == "-target") {
					input_f2 = argv[i + 1];
					foundArgs2 = true;
				}
				
				else if (string(argv[i]) == "-o") {
					output_f = argv[i + 1];
					foundArgs3 = true; 
				}

			}
            else if (string(argv[i])== "--pointid")
            {
                use_point_id = true;
            }
			
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nCopies the target scalars to source\nbased on vertex normals by default or point id\n\n"
			"\n(Mandatory)\n\t-source <source_mesh_vtk> \n\t-target <target_mesh_vtk> \n\t-o <output_vtk>\n" 
            "\n\t--pointid <use point id for copy, source and target must be exact same meshes"<< endl; 
			

		exit(1);
	}
	else
	{
		LaShell* source = new LaShell(input_f1);
		LaShell* target = new LaShell(input_f2);
		LaShell* la_out = new LaShell(input_f2);

		LaShellShellIntersectionMultiArray* wt = new LaShellShellIntersectionMultiArray();
		wt->SetInputData(source);
		wt->SetInputData2(target); 
		

		if (!use_point_id) { 
			cout << "\n\nUsing point normals for copying scalars ...." << endl;
			wt->SetCopyScalarsUsingNormal();
			
		}
		else {
			cout << "\n\nUsing point normals for copying scalars ...." << endl;
            wt->SetCopyScalarsUsingPointid();
		}

		
		wt->Update(); 

		la_out = wt->GetOutput(); 

		la_out->ExportVTK(output_f);
	}

}