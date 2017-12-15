/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellDeformationWithField deforms a mesh based on a deformation field 
*/
#pragma once

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


class LaShellDeformationWithField : public LaShellAlgorithms {

protected: 
    LaShell* _source_la;
    LaShell* _output_la;

    vtkSmartPointer<vtkPolyData> _SourcePolyData; 
	vtkSmartPointer<vtkFloatArray> _SourcePolyNormals;

    
public: 


    void SetInputData(LaShell* shell);
	
	void Update();

	LaShell* GetOutput();


	LaShellDeformationWithField();
	~LaShellDeformationWithField();

    

};