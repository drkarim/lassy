/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellShellIntersectionMultiArray class copies multi-array (i.e. multiple) scalars from one vtk to other, using two separate methods
*
*/
#pragma once
#define HAS_VTK 1

#define COPY_USING_POINTID 1 
#define COPY_USING_NORMAL 2
 


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
#include <vtkPolyDataNormals.h>
#include <vtkModifiedBSPTree.h>
#include <vtkPointLocator.h>
#include <string>

#include "LaShellAlgorithms.h"
#include "LaShellShellIntersection.h"
#include "MathBox.h"


using namespace std; 


class LaShellShellIntersectionMultiArray : public LaShellShellIntersection {


private:
	LaShell* _source_la; 
	LaShell* _target_la; 
	LaShell* _output_la;

	vector<Point3> _intersections;

	int _which_mapping;

	// when no intersection is found, this value is mapped, default is 0
	double _mapping_default_value;		
	
	double _which_direction;
	
	

protected:
	static double GetEuclidean(double* p1, double* p2); 
	static void GetFiniteLine(double* start, double* direction, double max_distance, double which_direction, double* end);
	
		
public:
	LaShellShellIntersectionMultiArray();
	~LaShellShellIntersectionMultiArray();


	static LaShellShellIntersectionMultiArray *New();
	
	void SetInputData(LaShell* shell);			// source
	void SetInputData2(LaShell* shell);			// target 
	
	void SetDirectionToOppositeNormal();

	void SetCopyScalarsUsingPointid();
	void SetCopyScalarsUsingNormal();
	void SetDefaultMappingValue(double);

	void Update();

	LaShell* GetOutput();

	
}; 