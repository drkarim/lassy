
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaImageMarkPoints.h"
#include "../include/MathBox.h"
using namespace std;


LaImageMarkPoints::LaImageMarkPoints() {
    _csv_filename = "";
    _scaling_factor = 1;
}




LaImageMarkPoints::~LaImageMarkPoints() {

    // nothing to delete yet 

}

void LaImageMarkPoints::SetInputData(LaImage* img) {

	_input_img = img; 
    _point_set = vtkSmartPointer<vtkPoints>::New();
    _point_set_t = vtkSmartPointer<vtkPoints>::New();
}

void LaImageMarkPoints::SetScalingFactor(double scale)
{
    _scaling_factor = scale;
}

void LaImageMarkPoints::SetInputCSVFileName(const char* csv_fn) 
{
    _csv_filename = csv_fn;
}


void LaImageMarkPoints::ReadCSVFile() {

    if (strlen(_csv_filename)!=0)
	    _csvfilestream.open(_csv_filename);
    else 
    {
        cerr << "CSV input file for files not set" << endl; 
        exit(1);
    }

    vector<vector<string> > csv_content = CSVReader::readCSV(_csvfilestream);
    double x,y,z, xt, yt, zt, p[3], pt[3];
	// The CSV iterator is from here: 
	// https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
	for (int i=0;i<csv_content.size();i++)
    {
        x=-1e10; y=-1e10; z=-1e10; 
        xt=-1e10; yt=-1e10; zt=-1e10; 
		vector<string> line = csv_content[i]; 
		
        for (int j=0;j<line.size();j++)
		{
			float num = atof(line[j].c_str()); 
			if (j==0) x = num ;
			else if (j==1) y = num ;
			else if (j==2) z = num ;
			else if (j==3) xt = num; 
            else if (j==4) yt = num; 
            else if (j==5) zt = num;
		}

        if (x>-1e10 && y>-1e10 && z>-1e10 & xt>-1e10 && yt>-1e10 && zt>-1e10)
        {
            p[0] = _scaling_factor*x; p[1] = _scaling_factor*y; p[2] = _scaling_factor*z;
            pt[0] = _scaling_factor*xt; pt[1] = _scaling_factor*yt; pt[2] = _scaling_factor*zt;
            //cout << "reading point " << x << "," << y << "," << z << "," << scalar << endl;
            _point_set->InsertNextPoint(p);
            _point_set_t->InsertNextPoint(pt);

        }   

       
    }   

}


void LaImageMarkPoints::MarkPoint(ImageType::IndexType pixelIndex, int markerpointsize) { 

    ImageType::Pointer greyscale = _input_img->GetImage();
    ImageType::IndexType pixelIndex2;
    int x,y,z;
    int maxX, maxY, maxZ, n; 
    _input_img->GetImageSize(maxX, maxY, maxZ); 
    
    n = markerpointsize;
    x = pixelIndex[0]; 
    y = pixelIndex[1]; 
    z = pixelIndex[2];

    for(int i = -n; i < n; i++)
	{
		for (int j = -n; j < n; j++)
        {
            for (int k=-n;k<n;k++){
                
                if (x+i > 0 && x+i < maxX && y+j > 0 && y+j < maxY && z+k>0 && z+k<maxZ){
                    pixelIndex2[0] = x+i; 
                    pixelIndex2[1] = y+j; 
                    pixelIndex2[2] = z+k;
                    greyscale->SetPixel(pixelIndex2, 255);
                }

            }
        }
    }
}

void LaImageMarkPoints::Update() 
{
    ReadCSVFile();
	ImageType::IndexType pixelIndex;
    ImageType::PointType point;
    ImageType::PointType point_t;

    double xyz[3], xyz_t[3];
    ImageType::Pointer image; 
    image = _input_img->GetImage();
   
    for (int i=0;i<_point_set->GetNumberOfPoints();i++)
    {
        _point_set->GetPoint(i, xyz); 
        _point_set_t->GetPoint(i, xyz_t); 
        point_t[0] = xyz_t[0]; point_t[1] = xyz_t[1]; point_t[2] = xyz_t[2];

        // Transform from physical to index 
        const bool isInside = image->TransformPhysicalPointToIndex( point_t, pixelIndex );
        if ( isInside )
        {
            cout << "point (" << xyz_t[0] << "," << xyz_t[1]<< "," << xyz_t[2] << ") can be marked on the image" << endl;
            MarkPoint(pixelIndex, 2);
            
        }
        else {
            cout << "ERROR: point (" << xyz_t[0] << "," << xyz_t[1]<< "," << xyz_t[2] << ") is outside and was transformed to "
            "= (" << pixelIndex[0] << "," << pixelIndex[1] << "," << pixelIndex[2] << ") outside of the image" << endl;
        }
    }
    
}

LaImage* LaImageMarkPoints::GetOutput()
{
    return _input_img; 
}
