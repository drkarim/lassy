/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The VizBox class is a 3D visualiser for LaImage
*	It uses 2D orthogonal planes to visualise the 3D data. This is essentially slicing 
*	through the 3D data. 
*
*/
#define HAS_VTK 1
#pragma once

#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkSmartPointer.h>
#include <vtkImageMapToColors.h>
#include <vtkLookupTable.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageMapper3D.h>
#include <vtkInteractorStyleTrackballCamera.h>

using namespace std; 

#include "../include/LaImage.h"

class VizBox {
private:
	vtkSmartPointer<vtkImageActor> _xSlice; 
	vtkSmartPointer<vtkImageActor> _ySlice;
	vtkSmartPointer<vtkImageActor> _zSlice;
	vtkSmartPointer<vtkLookupTable> _bwLut;
	LaImage* _la_img;

	vtkSmartPointer<vtkRenderer> _renderer;
	vtkSmartPointer<vtkRenderWindow> _renWin;
	

public:
	// Constructor with default values for data members
	VizBox(LaImage* la_img);

	/*
	*	This will create the 2D orthogonal planes by slicing through the data
	*	and converting these 2D slices to VTK entites which can be visualised 
	*/ 
	void ConstructImageOrthogonalPlanes();

	/*
	*	A color lookup table for the drawing the 2D orthogonal slices 
	*/
	void SetLookupTable();

	/*
	*	Draws the visualisations created on the screen 
	*	Also sets the window interactor 
	*/
	void ShowInit();
	
}; 

