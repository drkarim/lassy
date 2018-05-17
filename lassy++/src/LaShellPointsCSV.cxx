
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellPointsCSV.h"
using namespace std;


LaShellPointsCSV::LaShellPointsCSV() {
    _source_la = new LaShell(); 
    _copy_method = POINT_COPY;
}

void LaShellPointsCSV::SetInputData(LaShell* shell) {

	_source_la = shell; 
    _point_set = vtkSmartPointer<vtkPoints>::New();
}

void LaShellPointsCSV::SetCopyMethodToPointCopy()
{
    _copy_method = POINT_COPY;
}

void LaShellPointsCSV::SetCopyMethodToNeighbourCopy()
{
    _copy_method = NEIGHBOUR_COPY;
}


void LaShellPointsCSV::ReadCSVFile(const char* input_fn) {

	_csvfilestream.open(input_fn);

    vector<vector<string> > csv_content = CSVReader::readCSV(_csvfilestream);
    double x,y,z, scalar, p[3];
	// The CSV iterator is from here: 
	// https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
	for (int i=0;i<csv_content.size();i++)
    {
        x=-1e10; y=-1e10; z=-1e10; scalar=-1;
		vector<string> line = csv_content[i]; 
		
        for (int j=0;j<line.size();j++)
		{
			double num = atof(line[j].c_str()); 
			if (j==0) x = num ;
			else if (j==1) y = num ;
			else if (j==2) z = num ;
			else if (j==3) scalar = num;
		}

        if (x>-1e10 && y>-1e10 && z>-1e10)
        {
            p[0] = x; p[1] = y; p[2] = z;
            _point_set->InsertNextPoint(p);

        }   

        if (scalar > -1e10)
        {
            _scalars.push_back(scalar);
        }
    }   

}


void LaShellPointsCSV::LocatePoints() {

    double xyz[3]; 
    int closestPointID=-1;

    vtkSmartPointer<vtkPolyData> mesh = vtkSmartPointer<vtkPolyData>::New();
    _source_la->GetMesh3D(mesh);

    vtkSmartPointer<vtkPointLocator> point_locator = vtkSmartPointer<vtkPointLocator>::New();
	point_locator->SetDataSet(mesh); 
	point_locator->AutomaticOn(); 
	point_locator->BuildLocator();
    mesh->BuildLinks();


    // for every point in _point_set, find closest point on mesh, lets call it p 
    for (int i=0;i<_point_set->GetNumberOfPoints();i++)
    {
        _point_set->GetPoint(i, xyz); 
        closestPointID = point_locator->FindClosestPoint(xyz); 

        _closest_point_ids.push_back(closestPointID);
		
    }

    // Find N nearest neighbours of p, assign scalar to all these points 
    
}


void LaShellPointsCSV::VisualisePoints() {

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(_point_set);
 
    vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    
    #if VTK_MAJOR_VERSION <= 5
        vertexGlyphFilter->AddInput(polydata);
    #else
        vertexGlyphFilter->AddInputData(polydata);
    #endif
        vertexGlyphFilter->Update();

    vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetFileName("LaShellPointsCSV_polydata.vtk"); 
    writer->SetInputData(polydata);
    writer->Update();
}

void LaShellPointsCSV::Update() 
{
    LocatePoints();

    ofstream out; 
    double xyz[3];
    out.open("LaShellPointsCSV_output.csv"); 

    out << "x,y,z,point_id\n"; 
    
    for (int i=0;i<_point_set->GetNumberOfPoints();i++)
    {
        _point_set->GetPoint(i, xyz); 

        if (i < _closest_point_ids.size())
        {
            out << xyz[0] << "," << xyz[1] << "," << xyz[2] << "," << _closest_point_ids[i] << endl; 
        }
        
    }

    out.close();
    
}


LaShellPointsCSV::~LaShellPointsCSV() {

	delete _source_la; 

}
