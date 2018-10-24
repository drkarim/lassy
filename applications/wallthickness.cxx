#define HAS_VTK 1

#include "LaShellShellIntersection.h"
#include "vtkDistancePolyDataFilter.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*		This application computes the distance between the source and target meshes
*		The distance is calculated from the vertex normal direction of the source mesh 
*		The source and target meshes are not required to have to have the same toplogy 
*		The output mesh is the same toplogy as the source mesh, and contains this computed distance at each vertex 
*	
*/
int main(int argc, char * argv[])
{
	char* input_f1, *input_f2,  *output_f;
	int direction = 1; 
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3=false; 
	
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

			}
			else if (string(argv[i]) == "--reverse") {
				direction = -1;
			}

			else if (string(argv[i]) == "--distance") {
				direction = 2;
			}
		}
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nCalculates the thickness from the source to target"
			"\nThe final thickness is mapped to the source"
			"\n(Mandatory)\n\t-i1 <source_mesh_vtk> \n\t-i2 <target_mesh_vtk> \n\t-o <output_vtk>"
			"\n(Optional)\n\t--reverse <reverse direction of probing normal from source>"
			"\n\t--distance <shortest distance from target to source mesh>\n\n";
			

		exit(1);
	}
	else
	{
		LaShell* source = new LaShell(input_f1);
		LaShell* target = new LaShell(input_f2);
		LaShell* la_out = new LaShell(input_f2);

		LaShellShellIntersection* wt = new LaShellShellIntersection();
		wt->SetInputData(source);
		wt->SetInputData2(target); 
		wt->SetMapIntersectionToDistance();

		if (direction < 0) {
			cout << "\n\nImportant: Computing thickness in reverse direction to surface normals pointing outwards" << endl;
			wt->SetDirectionToOppositeNormal();
			
			wt->Update(); 
			la_out = wt->GetOutput(); 
			la_out->ExportVTK(output_f);
			
		}
		else if (direction == 1) {
			cout << "\n\nComputing thickness in surface normal direction (pointing outwards)" << endl; 
			
			wt->Update(); 
			la_out = wt->GetOutput(); 
			la_out->ExportVTK(output_f);
			
		}
		else if (direction == 2)
		{
			cout << "\n\nComputing shortest distance to target surface" << endl; 
			
			LaShell* la_out_d1 = new LaShell(input_f1);
			LaShell* la_out_d2 = new LaShell(input_f2);

			vtkSmartPointer<vtkPolyData> la_out_d1_poly = vtkSmartPointer<vtkPolyData>::New(); 
			vtkSmartPointer<vtkPolyData> la_out_d2_poly = vtkSmartPointer<vtkPolyData>::New(); 

			la_out_d1->GetMesh3D(la_out_d1_poly);
			la_out_d2->GetMesh3D(la_out_d2_poly);

			vtkSmartPointer<vtkDistancePolyDataFilter> distanceFilter = vtkSmartPointer<vtkDistancePolyDataFilter>::New();
			distanceFilter->SetInputData(0, la_out_d1_poly);
			distanceFilter->SetInputData(1, la_out_d2_poly);
			distanceFilter->Update();

			vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New(); 
			writer->SetInputData(distanceFilter->GetOutput()); 
			writer->SetFileName(output_f);
			writer->Update();
			


		}

		
		
	}

}