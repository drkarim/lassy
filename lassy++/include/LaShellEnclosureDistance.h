/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellEnclosureDistance class computes the thickness at each vertex of a convex enclosed shell
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
#include <vtkPolyDataNormals.h>
#include <vtkModifiedBSPTree.h>
#include <string>


#include "LaShellShellIntersection.h"



class LaShellEnclosureDistance : public LaShellShellIntersection {

private:
	LaShell* _source_shell;
	LaShell* _output_shell;
	

	int _which_direction;	
	bool _do_logging;

public:
	
	LaShellEnclosureDistance();
	~LaShellEnclosureDistance();

	/*
	*	Enclosed distance is computed as the distance between a vertex (source) to another vertex on the other side (target) of the shell. 
	*	This function is useful for creating a finite line for computing intersection between the finite line and shell. 
	*	This moves the source vertex by a small amount, so that intersection computations can correctly find the target vertex on other side, 
	*	and avoid the source vertex in the intersection calculations 
	*/
	static void MoveStartingPositionBy(double* start, double which_direction, double* direction_vec, double move_distance, double* new_start);

	void SetLoggingToTrue();

	void SetInputData(LaShell* shell); 
	//virtual void SetInputData2(LaShell* shell);
	
	void Update(); 

	LaShell* GetOutput(); 

	
}; 