#define HAS_VTK 1
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkSmartPointer.h>
#include <vtkImageMapToColors.h>
#include <vtkLookupTable.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

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
	void ConstructImageOrthogonalPlanes();
	void SetLookupTable();

	void ShowInit();
	
}; 

