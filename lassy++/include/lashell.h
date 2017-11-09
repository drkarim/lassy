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
#pragma once
#define HAS_VTK 1
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkPolyDataReader.h>
#include <vtkFileOutputWindow.h>
#include <vtkPolyDataWriter.h>
#include <vtkCellData.h>
#include <vtkCellDataToPointData.h>
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
	
	void SetMesh3D(vtkPolyData* input);

	void GetMesh3D(vtkPolyData* mesh_output);
	void GetMinimumMaximum(double &min, double& max);		// not implemented yet!
	/*
	*	Exports the VTK mesh to a file 
	*/
	void ExportVTK(char* vtk_fn);

	/*
	*	Converts a binary 3D image to a smooth mesh. 
	*	It takes a threshold value for the iso-surface
	*	(Leave it to the default value for binary images provided background = 0)
	*/
	void BinaryImageToShell(LaImage *la_mask, double threshold=0.5);

	
	//void SurfaceProjection(LaImage* raw_img, bool doLogging=false, LaImage* mask_img=NULL);
	
	void ConvertToPointData();

	vector<double> GetMeshVertexValues();


	void ComputeMeshNeighbourhoodTransform(vtkSmartPointer<vtkPolyData> new_mesh);
	
}; 