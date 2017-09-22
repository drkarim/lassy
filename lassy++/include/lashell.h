#define HAS_VTK 1
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkPolyDataReader.h>


#include <string>

using namespace std; 


class LAtrium {
private:
	vtkSmartPointer<vtkPolyData> _mesh_3d; 
	
	vector<double> _mesh_vertex_values; 

public:
	// Constructor with default values for data members
	LAtrium(const char* vtk_filename);
	
	void GetMesh3D(vtkPolyData* mesh_output);

	vector<double> GetMeshVertexValues();
}; 