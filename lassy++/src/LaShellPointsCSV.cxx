
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellPointsCSV.h"
#include "../include/MathBox.h"
using namespace std;


LaShellPointsCSV::LaShellPointsCSV() {
    _source_la = new LaShell(); 
    _output_la = new LaShell();
    _copy_method = POINT_COPY;
    _neighbour_radius = 5;     
    _new_scalar_array_name = "new_scalar";
    _insert_scalar_value = 1;
    _containers_set = false;
    _write_all_neighbours = false; 
    _scaling_factor = 1;
    _write_to_field_data  = false; 
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

void LaShellPointsCSV::SetNeighbourRadius(int radius)
{
    _neighbour_radius = _scaling_factor*radius;
    cout << "Warning: using a scaling factor of " << _scaling_factor << ", hence radius becomes = " << _neighbour_radius << "\n\n";
}

void LaShellPointsCSV::SetArrayName(const char* array_name)
{
    _new_scalar_array_name = array_name;
}

void LaShellPointsCSV::SetInsertScalarValue(double val)
{
    _insert_scalar_value = val;
}

void LaShellPointsCSV::SetScalingFactor(int scale)
{
    _scaling_factor = scale;
    _neighbour_radius = _scaling_factor*_neighbour_radius; 
    
}

void LaShellPointsCSV::SetWriteDataToField()
{
    _write_to_field_data= true; 
}

void LaShellPointsCSV::SetWriteAllNeighboursInCSV()
{
    _write_all_neighbours = true; 
}

void LaShellPointsCSV::ReadCSVFile(const char* input_fn) {

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
			float num = atof(line[j].c_str()); 
			if (j==0) x = num ;
			else if (j==1) y = num ;
			else if (j==2) z = num ;
			else if (j==3) scalar = num;
		}

        if (x>-1e10 && y>-1e10 && z>-1e10)
        {
            p[0] = _scaling_factor*x; p[1] = _scaling_factor*y; p[2] = _scaling_factor*z;
            //cout << "reading point " << x << "," << y << "," << z << "," << scalar << endl;
            _point_set->InsertNextPoint(p);

        }   

        if (scalar > -1e9 && scalar < 1e9 )
        {
            //cout << scalar << endl;
            _scalars.push_back(scalar);         // _scalars correspond to points in _point_set
        }
        else  {
            
            _scalars.push_back(0);         
        }
    }   

}



void LaShellPointsCSV::InitNeighbourPointListingContainer()
{
    if (!_containers_set) 
    {
        int num_points = _point_set->GetNumberOfPoints(); 

        _neighbour_point_set.resize(num_points);

        vtkSmartPointer<vtkPolyData> mesh = vtkSmartPointer<vtkPolyData>::New();
        _source_la->GetMesh3D(mesh);

        int num_vertices = mesh->GetNumberOfPoints(); 

        for (int i=0;i<num_vertices;i++)
            _earmark_point.push_back(0);
        
        _containers_set = true;
    }

}

void LaShellPointsCSV::LocateNeighboursOfPoints()
{

    double xyz[3], m_xyz[3]; 
    vtkSmartPointer<vtkPolyData> mesh = vtkSmartPointer<vtkPolyData>::New();
    _source_la->GetMesh3D(mesh);
    
    InitNeighbourPointListingContainer();
   
    // for every point in _point_set, find closest point on mesh, lets call it p 
    for (int i=0;i<_point_set->GetNumberOfPoints();i++)
    {
        _point_set->GetPoint(i, xyz); 
        
        
        for (int j=0;j<mesh->GetNumberOfPoints();j++)
        {
            mesh->GetPoint(j, m_xyz);

            if (MathBox::EuclideanDistance(xyz, m_xyz) < _neighbour_radius)
            {
                _neighbour_point_set[i].push_back(j);
                
                if (i<_scalars.size())
                    _earmark_point[j] = _scalars[i];
                else 
                    _earmark_point[j] = 1;
            }
        }
    }

    cout << "Neigbbour point size = " << _neighbour_point_set.size() << endl;

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

    InitNeighbourPointListingContainer();

    // for every point in _point_set, find closest point on mesh, lets call it p 
    for (int i=0;i<_point_set->GetNumberOfPoints();i++)
    {
        _point_set->GetPoint(i, xyz); 
        closestPointID = point_locator->FindClosestPoint(xyz); 

        _closest_point_ids.push_back(closestPointID);

        if (closestPointID < _scalars.size() && closestPointID < _earmark_point.size()) 
            _earmark_point[closestPointID] = _scalars[i];
        else if (closestPointID < _earmark_point.size())
            _earmark_point[closestPointID] = 1;
		
    }

    // Find N nearest neighbours of p, assign scalar to all these points 
    
}

void LaShellPointsCSV::InsertScalarData() { 

    vtkSmartPointer<vtkPolyData> mesh = vtkSmartPointer<vtkPolyData>::New();
    _source_la->GetMesh3D(mesh);

    vtkSmartPointer<vtkFloatArray> new_scalar = vtkSmartPointer<vtkFloatArray>::New();
    new_scalar->SetNumberOfComponents(1);
    new_scalar->SetName(_new_scalar_array_name);

    for (int i=0;i<mesh->GetNumberOfPoints();i++)
    {
        if (_earmark_point[i] > 0)              // _earmark_point stores the scalar value to write for this point
            new_scalar->InsertNextValue(_earmark_point[i]); 
        else 
            new_scalar->InsertNextValue(0); 
    }

    if (!_write_to_field_data)
        mesh->GetPointData()->SetScalars(new_scalar);
    else {
        cout << "Warning: Writing to field data .. "; 
        mesh->GetFieldData()->AddArray(new_scalar);
    }

    // a new mesh is created 
    _output_la->SetMesh3D(mesh);
    
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

LaShell* LaShellPointsCSV::GetOutput() {
    
    return _output_la;
}


void LaShellPointsCSV::Update() 
{
    LocatePoints();

    ofstream out; 
    double xyz[3], xyz_t[3];
    vtkSmartPointer<vtkPolyData> mesh = vtkSmartPointer<vtkPolyData>::New();
    _source_la->GetMesh3D(mesh);

    out.open("LaShellPointsCSV_output.csv"); 
    out << "XYZ read from CSV and closest XYZ_t found on input shell\n";
    out << "x,y,z,x_t,y_t,z_t\n"; 
       
    for (int i=0;i<_point_set->GetNumberOfPoints();i++)
    {
        _point_set->GetPoint(i, xyz); 
    
        if (i < _closest_point_ids.size())
        {
            mesh->GetPoint(_closest_point_ids[i], xyz_t);        
            out << xyz[0] << "," << xyz[1] << "," << xyz[2] << "," << xyz_t[0] << "," << xyz_t[1] << "," << xyz_t[2];
            
            if (_write_all_neighbours) {
                if (_neighbour_point_set.size() > 0 && i < _neighbour_point_set.size())
                {
                    for (int j=0;j<_neighbour_point_set[i].size();j++)
                    {
                        out << "," << _neighbour_point_set[i][j];
                    }
                    out << endl;
                }
                else 
                    out << endl;
            }
            else {
                out << endl;
            }
        }
        
    }

    out.close();
    
}


LaShellPointsCSV::~LaShellPointsCSV() {

	delete _source_la; 

}
