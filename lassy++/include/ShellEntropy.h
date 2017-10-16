#define HAS_VTK 1
#pragma once

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


using namespace std; 




class ShellEntropy {
private:
	vtkSmartPointer<vtkPolyData> _mesh_3d; 
	vector<int> _visited_point_list; 

public:
	
	ShellEntropy(vtkSmartPointer<vtkPolyData> mesh);

	void GetNeighboursAroundPoint(int pointID, vector<int>& pointNeighbours, int order);
	int RecursivePointNeighbours(vtkIdType pointId, int order);
	void GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int seed, vtkSmartPointer<vtkIdList> connectedVertices);
	bool InsertPointIntoVisitedList(vtkIdType id);

}; 

