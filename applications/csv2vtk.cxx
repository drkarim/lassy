#define HAS_VTK 1

#include "LaImage.h"
#include <vtkSmartPointer.h>
#include <vtkPolyDataWriter.h>
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
*	   This simple application converts a list of csv point X,Y,Z to vtkpoint set and stores as a vtk 
*      These points could also be pixel locations in an image, and converted to World coordinates before storing as vtk  
*/
int main(int argc, char * argv[])
{
	char* input_csv_fn, *output_fn, *input_img_fn; 
	bool convert=true;
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;
    ifstream csvfilestream;
    vtkSmartPointer<vtkPoints> point_set = vtkSmartPointer<vtkPoints>::New();
    typedef itk::Image< unsigned short, 3 >  ImageType;
    int lines=0, num_points=0;

	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-csv") {
					input_csv_fn = argv[i + 1];
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
                
			}
            else if (string(argv[i]) == "--convert") {
                convert = true;
            }
			
		}
        
	}

	if (!(foundArgs1 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nPoints listed on a csv point to a VTK point set"
			"\n(Mandatory)\n\t-csv <csv file with points> \n\t-o <vtk points>\n\t(optional)\n\t-img <3D image>\n\t--convert <csv points from image to world>\n\n" << endl; 
			
		exit(1);
	}
    else if (convert)
    {
        if (!foundArgs2)
        {
            cerr << "Check your parameters: no image included with -img switch if --convert switch is on" << endl;
        }
    }
	
        if (strlen(input_csv_fn)!=0)
	        csvfilestream.open(input_csv_fn);
        else 
        {
            cerr << "CSV input file for files not set" << endl; 
            exit(1);
        }

        vector<vector<string> > csv_content = CSVReader::readCSV(csvfilestream);
        double x,y,z, p[3];
        // The CSV iterator is from here: 
        // https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
        for (int i=0;i<csv_content.size();i++)
        {
            x=-1e10; y=-1e10; z=-1e10; 
            
            vector<string> line = csv_content[i]; 
            

            for (int j=0;j<line.size();j++)
            {
                if (!isalpha(line[j].at(0))) {      // skip headers
                    float num = atof(line[j].c_str()); 
                    if (j==0) x = num ;
                    else if (j==1) y = num ;
                    else if (j==2) z = num ;
                    lines++;
                    cout << "reading point (" << x << "," << y << "," << z << ")\n";
                }
                else {
                    cout << "Encountered an entry that is not a number, header? " << line[j] << endl;
                }
                
            }

            if (x>-1e10 && y>-1e10 && z>-1e10 )
            {
                p[0] = x; p[1] = y; p[2] = z;

                if (convert)
                {
                    // convert image to world 
                    LaImage* greyscale = new LaImage(input_img_fn);  // copy structure 
                    ImageType::Pointer greyscale_pn = greyscale->GetImage();
                    ImageType::IndexType pixelIndex;
                    ImageType::PointType point;
                    pixelIndex[0] = x; pixelIndex[1] = y; pixelIndex[2] = z; 
                    greyscale_pn->TransformIndexToPhysicalPoint( pixelIndex, point );
                    
                    
                    p[0] = point[0]; 
                    p[1] = point[1]; 
                    p[2] = point[2];
                    point_set->InsertNextPoint(p);
                    num_points++;
            
                }
                else { 
                    point_set->InsertNextPoint(p);  
                    num_points++;
                }

            }   

           
        }   // end for 
		
        if (lines > 0 && num_points > 0 && lines != num_points)
        {
            vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
            polydata->SetPoints(point_set);
            vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New(); 
            writer->SetFileName(output_fn);
            writer->SetInputData(polydata);
            writer->Update();
        }
        else { 
            cerr << "Not all CSV lines could not converted to points, num_line" << lines << ", num_points=" << num_points; 
            exit(1);
        }
	

}   // end function