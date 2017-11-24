/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellAtlas can create an atlas from a list of target shells, preferably with the same topology
*	The atlas aggregates (mean/median) the values stored at each vertex of the target shells 
*/
#pragma once

#define USE_DIRECT_COPY 1
#define USE_CLOSEST_POINT 2 


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

#include "LaShellShellDisplacement.h"
#include "LaShell.h"


class LaShellAtlas : public LaShellShellDisplacement {
private: 
	int _which_method;
public:
	void ReadShellComputeDisplacement(string poly_data_fn);
	
	void SetAtlasConstructionToUseDirectCopy();				// must ensure that all meshes have the same topology

	void SetAtlasConstructionToUseClosestPoint();			// much slower but more accurate


	void Update();
}; 