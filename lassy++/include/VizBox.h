/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The VizBox class is a 3D visualiser for Lassy entities such as LaImage and LaShell  
*	It uses 2D orthogonal planes to visualise the 3D data. This is essentially slicing 
*	through the 3D data. It uses 3D surface rendering for visualising the mesh 
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
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkObjectFactory.h>
#include <vtkCallbackCommand.h>
#include <vtkProperty.h>

using namespace std; 

#include "../include/LaImage.h"
#include "../include/LaShell.h"

class VizBox {
private:
	vtkSmartPointer<vtkImageActor> _xSlice; 
	vtkSmartPointer<vtkImageActor> _ySlice;
	vtkSmartPointer<vtkImageActor> _zSlice;

	vtkSmartPointer<vtkActor> _mesh3DActor;

	vtkSmartPointer<vtkLookupTable> _bwLut;
	vtkSmartPointer<vtkLookupTable> _colLut;

	LaImage* _la_img;

	vtkSmartPointer<vtkRenderer> _renderer;
	vtkSmartPointer<vtkRenderWindow> _renWin;

	int _xPos, _yPos, _zPos; 
	int _maxX, _maxY, _maxZ;

	double _mesh3DActor_opacity;
	

public:
	// Constructor with default values for data members
	VizBox();

	/*
	*	This will create the 2D orthogonal planes by slicing through the data
	*	and converting these 2D slices to VTK entites which can be visualised 
	*/ 
	void ConstructImageOrthogonalPlanes(LaImage* img3d);

	/*
	*	A color lookup table for drawing the 3D images and mesh  
	*/
	void SetLookupTable_image3d(LaImage* img3d);
	void SetLookupTable_mesh3d(LaShell* mesh3D);

	void ConstructMeshVisualiser(LaShell* mesh3d);

	/*
	*	Moves a 3D slice forward or backward by a user-defined 
	*	increment (or -ve for decrement)
	*/
	void MoveSlice(int slice_dir, int increment);

	void ChangeMeshOpacity(double direction); 

	/*
	*	Draws the visualisations created on the screen 
	*	Also sets the window interactor 
	*/
	void ShowInit();

	static void KeypressCallbackFunction( vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* vtkNotUsed(clientData), void* vtkNotUsed(callData) );
	
}; 


/*
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  public:
    static KeyPressInteractorStyle* New();
    vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);
 
    virtual void OnKeyPress() 
    {
      // Get the keypress
      vtkRenderWindowInteractor *rwi = this->Interactor;
      std::string key = rwi->GetKeySym();
 
      // Output the key that was pressed
      std::cout << "Pressed " << key << std::endl;
 
      // Handle an arrow key
      if(key == "x")
        {
        std::cout << "The x key was pressed." << std::endl;
        }

		if(key == "X")
        {
        std::cout << "The X key was pressed." << std::endl;
        }
 
      // Handle a "normal" key
      if(key == "y")
        {
        std::cout << "The y key was pressed." << std::endl;
        }
	
	if(key == "z")
        {
        std::cout << "The z key was pressed." << std::endl;
        }
	
	
 
      // Forward events
      vtkInteractorStyleTrackballCamera::OnKeyPress();
    }
 
};
vtkStandardNewMacro(KeyPressInteractorStyle);

*/