#define HAS_VTK 1
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkPolyDataReader.h>
#include <vtkMarchingCubes.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkDecimatePro.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataMapper.h>
#include <vtkFileOutputWindow.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkPolyDataWriter.h>
#include <string>
#include "LaImage.h"

using namespace std; 


class LaShell {
private:
	vtkSmartPointer<vtkPolyData> _mesh_3d; 
	
	vector<double> _mesh_vertex_values; 

public:
	// Constructor with default values for data members
	LaShell();
	LaShell(const char* vtk_filename);
	
	void GetMesh3D(vtkPolyData* mesh_output);

	void ExportVTK(char* vtk_fn);
	void ConvertMaskToMesh(LaImage *la_mask, double);
	

	vector<double> GetMeshVertexValues();
	
}; 