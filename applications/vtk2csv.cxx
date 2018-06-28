#define HAS_VTK 1

#include "LaImage.h"
#include <vtkSmartPointer.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyDataReader.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>

#include <string>
#include <sstream>
#include <vector>

#include "CSVReader.h"
#include <numeric> 

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This simple application converts vtk point set to image pixel indices and writes as a csv
*/
int main(int argc, char * argv[])
{
	char* input_fn, *output_fn_csv, *output_fn_img, *input_img_fn; 
	bool convert=true;
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false,foundArgs4=false;
    ofstream out;
    vtkSmartPointer<vtkPoints> point_set = vtkSmartPointer<vtkPoints>::New();
    typedef itk::Image< unsigned short, 3 >  ImageType;
    ImageType::IndexType pixelIndex;
    ImageType::PointType point;
    
    double p[3];

	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-vtk") {
					input_fn = argv[i + 1];
					foundArgs1 = true;
				}
				
				else if (string(argv[i]) == "-img") {
					input_img_fn = argv[i + 1];
					foundArgs2 = true; 
				}
			
				else if (string(argv[i]) == "-csv") {
					output_fn_csv = argv[i + 1];
                    foundArgs3 = true; 
					
				}
                else if (string(argv[i]) == "-out") {
					output_fn_img = argv[i + 1];
                    foundArgs4 = true; 
					
				}
                
			}
            
			
		}
        
	}

	if (!(foundArgs1 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nFrom VTK point set to image"
			"\n(Mandatory)\n\t-vtk <VTK points set> \n\t-img <3D image world to image space>\n\t-csv <csv output filename>\n\t(optional)\n\t-out <3D image with marked points>\n\n" << endl; 
			
		exit(1);
	}
    
    out.open(output_fn_csv);
    LaImage* image = new LaImage(input_img_fn);
    vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New(); 
    reader->SetFileName(input_fn);
    reader->Update();
    vtkSmartPointer<vtkPolyData> poly = vtkSmartPointer<vtkPolyData>::New(); 
    poly = reader->GetOutput(); 

    ImageType::Pointer image_p = image->GetImage();
    for (int i=0;i<poly->GetNumberOfPoints();i++)
    {
        poly->GetPoint(i, p); 
        point[0] = p[0]; 
        point[1] = p[1]; 
        point[2] = p[2];
        const bool isInside = image_p->TransformPhysicalPointToIndex( point, pixelIndex );

        if ( isInside )
        {
            cout << "point (" << point[0] << "," << point[1]<< "," << point[2] << ") can be marked on the image" << endl;
            out << pixelIndex[0] << "," << pixelIndex[1] << "," << pixelIndex[2] << endl;

            if (foundArgs4) {
                // MarkPoint(pixelIndex, 2);         
            }
           
        }
        else {
            cout << "ERROR: point (" << point[0] << "," << point[1]<< "," << point[2] << ") is outside and was transformed to "
            "= (" << pixelIndex[0] << "," << pixelIndex[1] << "," << pixelIndex[2] << ") outside of the image" << endl;
        }

    }

    out.close();
      

	

}   // end function