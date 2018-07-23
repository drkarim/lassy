#define HAS_VTK 1

#include "LaImage.h"
#include "itkImageFileWriter.h"

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
*	   Sets image pixels according to a values in a csv file
*      In the csv file, each line is the pixel index along with pixel value listed as:
*      X,Y,Z,pixel value 
*/
int main(int argc, char * argv[])
{
	char* input_csv_fn, *output_fn, *input_img_fn; 
	bool convert=true;
	bool foundArgs1 = false, foundArgs2 = false, foundArgs3 = false;
    ifstream csvfilestream;
    vtkSmartPointer<vtkPoints> point_set = vtkSmartPointer<vtkPoints>::New();
    typedef itk::Image< unsigned short, 3 >  InputImageType;
    typedef itk::Image< float, 3 >  OutputImageType;
    int lines=0, num_points=0;
    double x_translate=0, y_translate=0;

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
                else if (string(argv[i]) == "-x") {
                    x_translate = atof(argv[i+1]);
                }
                else if (string(argv[i]) == "-y") {
                    y_translate = atof(argv[i+1]);
                }
                
			}
            
			
		}
        
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nRequired parameters not found"
			"\n(Mandatory)\n\t-csv <csv file with points> \n\t-img <3D image that will be set>\n\t-o <output image filename>"
            "\n\t(optional)\n\t-y <translate in y direction>\n\t-x <translate in x direction>\n\n" << endl; 
			
		exit(1);
	}
   else {
	
        if (strlen(input_csv_fn)!=0)
	        csvfilestream.open(input_csv_fn);
        else 
        {
            cerr << "CSV input file for files not set" << endl; 
            exit(1);
        }

        vector<vector<string> > csv_content = CSVReader::readCSV(csvfilestream);
        double x,y,z, pixel_value, p[3], sp[3];
        int size_x, size_y, size_z; 
        
        LaImage* greyscale = new LaImage(input_img_fn);  // copy structure 
        InputImageType::Pointer greyscale_pn = greyscale->GetImage();
        
        OutputImageType::SpacingType spacing_out;
        InputImageType::SpacingType spacing_in;
        spacing_in = greyscale_pn->GetSpacing();
        spacing_out[0] = spacing_in[0]; 
        spacing_out[1] = spacing_in[1];
        spacing_out[2] = spacing_in[2];

        OutputImageType::RegionType region;
        OutputImageType::IndexType start;
        start[0] = 0;
        start[1] = 0;
        start[2] = 0; 

        OutputImageType::SizeType size;
        greyscale->GetImageSize(size_x, size_y, size_z); 

        size[0] = size_x; 
        size[1] = size_y; 
        size[2] = size_z;
        
        region.SetSize(size);
        region.SetIndex(start);
        OutputImageType::Pointer output_im = OutputImageType::New();
        output_im->SetRegions(region);
        output_im->SetSpacing(spacing_out);
        output_im->Allocate();

        // The CSV iterator is from here: 
        // https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
        for (int i=0;i<csv_content.size();i++)
        {
            x=-1e10; y=-1e10; z=-1e10, pixel_value=-1e10; 
            
            vector<string> line = csv_content[i]; 
            

            for (int j=0;j<line.size();j++)
            {
                if (!isalpha(line[j].at(0))) {      // skip headers
                    float num = atof(line[j].c_str()); 
                    if (j==0) x = round(num) ;
                    else if (j==1) y = round(num) ;
                    else if (j==2) z = round(num);
                    else if (j==3) pixel_value = num ;
                    lines++;
                }
                else {
                    cout << "Encountered an entry that is not a number, header? " << line[j] << endl;
                }
                
                
            }
            
            if (x>-1e10 && y>-1e10 && z>-1e10 && pixel_value > -1e10 )
            {
                x += x_translate; 
                y += y_translate; 

                p[0] = x; 
                p[1] = y;
                p[2] = z;

                // convert image to world 
                
                OutputImageType::IndexType pixelIndex;
                
                pixelIndex[0] = x; pixelIndex[1] = y; pixelIndex[2] = z; 
                
                if (x >= 0 && x < size_x && y >= 0 && y < size_y && z >= 0 && z < size_z)
                {
                    output_im->SetPixel(pixelIndex, pixel_value);
                }
                else { 
                    cerr << "Pixel indices in csv out of bounds of image: (" << x << "," << y << "," << z << ")\n";
                    //exit(1);
                }
                
                
               
            }   

           
        }   // end for 
		
        typedef  itk::ImageFileWriter< OutputImageType  > WriterType;
        WriterType::Pointer writer = WriterType::New();
        writer->SetFileName(output_fn);
        writer->SetInput(output_im);
        writer->Update();
   } // end else 

}   // end function