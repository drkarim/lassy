/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellShellIntersection class computes all locations where the vertex normals of the source mesh intersect with the target mesh
*
*/
#pragma once
#define HAS_VTK 1

#define MAPPING_METHOD_DISTANCE 1 
#define MAPPING_METHOD_TRANSFER 2
 


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
#include "MathBox.h"


using namespace std; 


class LaShellShellIntersection : public LaShellAlgorithms {


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
	LaShellShellIntersection();
	~LaShellShellIntersection();


	static LaShellShellIntersection *New();
	
	void SetInputData(LaShell* shell);			// source
	void SetInputData2(LaShell* shell);			// target 
	
	void SetDirectionToOppositeNormal();

	void SetMapIntersectionToDistance();
	void SetMapIntersectionToCopyScalar();
	void SetDefaultMappingValue(double);

	void Update();

	LaShell* GetOutput();

	
}; 