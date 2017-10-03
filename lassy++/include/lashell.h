/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShell class encapsulates a 3D polygonal mesh that is read into memory as a VTK
*	For more information: https://www.vtk.org/doc/nightly/html/classvtkPolyData.html
*
*/
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
#include <vtkFloatArray.h>
#include <vtkCellData.h>
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

	/*
	*	Exports the VTK mesh to a file 
	*/
	void ExportVTK(char* vtk_fn);

	/*
	*	Converts a binary 3D image to a smooth mesh. 
	*	It takes a threshold value for the iso-surface
	*	(Leave it to the default value for binary images provided background = 0)
	*/
	void ConvertMaskToMesh(LaImage *la_mask, double threshold=0.5);

	/*
	*	Interrogates a 3D image along the surface normals of a 3D surface mesh.
	*	Projects the interrogated value to the mesh vertex 
	*	Note that the interrogation is by default 4 pixels in both directions of the vector
	*
	*	There is an option to interrogate the image only in the region defined by the mask_img parameter
	*/
	void SurfaceProjection(LaImage* raw_img, LaImage* mask_img=NULL);
	

	vector<double> GetMeshVertexValues();
	
}; 