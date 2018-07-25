
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShell2ShellPointsCSV.h"
#include "../include/MathBox.h"
using namespace std;


LaShell2ShellPointsCSV::LaShell2ShellPointsCSV() {
    _source_la = new LaShell(); 
    _target_la = new LaShell();
    _source_in_target_la = new LaShell();
    _scaling_factor = 1;
    
}

void LaShell2ShellPointsCSV::SetSourceData(LaShell* shell) {

	_source_la = shell; 
    _point_set = vtkSmartPointer<vtkPoints>::New();
    
}

void LaShell2ShellPointsCSV::SetTargetData(LaShell* shell) {

	_target_la = shell; 
    
}

void LaShell2ShellPointsCSV::SetSourceInTargetData(LaShell* shell)
{
    _source_in_target_la = shell;
}

void LaShell2ShellPointsCSV::SetOutputFileName(char* fn)
{
    _csv_filename = fn;
}


void LaShell2ShellPointsCSV::ReadCSVFile(const char* input_fn) {

	_csvfilestream.open(input_fn);

    vector<vector<string> > csv_content = CSVReader::readCSV(_csvfilestream);
    double x,y,z, p[3];
    float scalar;
	// The CSV iterator is from here: 
	// https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
	for (int i=0;i<csv_content.size();i++)
    {
        x=-1e10; y=-1e10; z=-1e10; scalar=-1;
		vector<string> line = csv_content[i]; 
		
        for (int j=0;j<line.size();j++)
		{
            if (!isalpha(line[j].at(0))) {      // skip headers
                float num = atof(line[j].c_str()); 
                if (j==0) x = num ;
                else if (j==1) y = num ;
                else if (j==2) z = num ;
                //else if (j==3) scalar = num;
            }else {
                cout << "Encountered an entry that is not a number, header? " << line[j] << endl;
            }
		}

        if (x>-1e10 && y>-1e10 && z>-1e10)
        {
            p[0] = _scaling_factor*x; p[1] = _scaling_factor*y; p[2] = _scaling_factor*z;
            //cout << "reading point " << x << "," << y << "," << z << "," << scalar << endl;
            _point_set->InsertNextPoint(p);

        }   

       
    }   

}





void LaShell2ShellPointsCSV::LocateCSVPointsInSource() {

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

        if (closestPointID > -1) {
            _closest_point_ids_in_source.push_back(closestPointID);
        }
        else {
            _closest_point_ids_in_source.push_back(-1);
            cout << "Colocation failed for this point: " << xyz[0] << "," << xyz[1] << "," << xyz[2] << endl;
        }

    }

    
}

void LaShell2ShellPointsCSV::LocateSourcePointsOnTarget() {

    double xyz_source[3], xyz_target[3]; 
    int closestPointID=-1;

    vtkSmartPointer<vtkPolyData> source_mesh = vtkSmartPointer<vtkPolyData>::New();
    _source_in_target_la->GetMesh3D(source_mesh);


    vtkSmartPointer<vtkPolyData> target_mesh = vtkSmartPointer<vtkPolyData>::New();
    _target_la->GetMesh3D(target_mesh);

    vtkSmartPointer<vtkPointLocator> point_locator = vtkSmartPointer<vtkPointLocator>::New();
	point_locator->SetDataSet(target_mesh); 
	point_locator->AutomaticOn(); 
	point_locator->BuildLocator();
    target_mesh->BuildLinks();

    ofstream out; 
    out.open(_csv_filename);
    out << "x,y,z\n";

    // for every point in _point_set, find closest point on mesh, lets call it p 
    for (int i=0;i<_closest_point_ids_in_source.size();i++)
    {
        int point_id_in_source = _closest_point_ids_in_source[i]; 
        
        if (point_id_in_source > -1) {

            source_mesh->GetPoint(point_id_in_source, xyz_source); 
            closestPointID = point_locator->FindClosestPoint(xyz_source); 

            if (closestPointID > -1) {
                _closest_point_ids_in_target.push_back(closestPointID);
                target_mesh->GetPoint(closestPointID, xyz_target);

                out << xyz_target[0] << "," << xyz_target[1] << "," << xyz_target[2] << endl;
            }
            else {
                out << "0,0,0" << endl;
            }

        }
        else { 
            out << "0,0,0" << endl;
        }
        
        
    }

    out.close();

    
}


void LaShell2ShellPointsCSV::Update() 
{
    LocateCSVPointsInSource();
    LocateSourcePointsOnTarget();

} 


LaShell2ShellPointsCSV::~LaShell2ShellPointsCSV() {

	delete _source_la; 

}
