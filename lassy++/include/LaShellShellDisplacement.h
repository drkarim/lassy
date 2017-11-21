/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellShellDisplacement computes the displacement for each vertex between a source and target mesh 
*/
#pragma once

#define ONE_TARGET 1 
#define MULTIPLE_TARGETS 2 
#define AGGREGATE_MEAN 1 
#define AGGREGATE_MEDIAN 2 

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
#include <sstream>

#include "LaShellAlgorithms.h"
#include "LaShell.h"


class LaShellShellDisplacement : public LaShellAlgorithms {

private: 
	LaShell* _source_la;
	LaShell* _target_la;
	LaShell* _output_la;

	string _multiple_target_fn; 
	vector<string> _filename_list; 
	vector<vector< double> > _displacements; 
	int _num_targets;		
	int _num_targets_read;
	int _aggregate_method;		// default is median
	int _total_targets; 

	vtkSmartPointer<vtkPolyData> _SourcePolyData; 

	double GetEuclidean(double*, double*);
	bool ReadShellNameList(const char* fn);
	void PrepareDisplacementContainer(string first_file, vector<vector< double> >& scalar_container);
	void ReadShellComputeDisplacement(string); 
	void AggregateAllDisplacements();

public:
		
	void SetInputData(LaShell* shell);
	void SetInputData2(LaShell* shell);

	void SetInputMultipleTargets(char* name_list); 

	void SetAggregateMethodToMean(); 
	void SetAggregateMethodToMedian();
	
	void Update();

	LaShell* GetOutput();


	LaShellShellDisplacement();
	~LaShellShellDisplacement();
	
}; 