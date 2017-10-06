#define HAS_VTK 1


/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/VizBox.h"


using namespace std;


VizBox::VizBox(LaImage* la_img)
{
	_xSlice = vtkSmartPointer<vtkImageActor>::New();
	_ySlice = vtkSmartPointer<vtkImageActor>::New();
	_zSlice = vtkSmartPointer<vtkImageActor>::New();
	_renWin = vtkSmartPointer<vtkRenderWindow>::New();

	_renderer = vtkSmartPointer<vtkRenderer>::New();
	
	_la_img = la_img; 
}


void VizBox::SetLookupTable()
{
	short min, max; 
	cout << "Setting black-white lookup table ... ";
	_la_img->GetMinimumMaximum(min, max);
	_bwLut = vtkSmartPointer<vtkLookupTable>::New();

	_bwLut->SetRange(0, max); // image intensity range
	_bwLut->SetValueRange(0.0, 1.0); // from black to white
	_bwLut->SetSaturationRange(0.0, 0.0); // no color saturation
	_bwLut->SetRampToLinear();
	_bwLut->Build();
	cout << "completed! ";

}

void VizBox::ConstructImageOrthogonalPlanes()
{
	
	// Convert image to structured points 
	char* temp_vtk_fn = "temp_123234.vtk";
	_la_img->ConvertToVTKImage(temp_vtk_fn);
	vtkSmartPointer<vtkStructuredPointsReader > reader = vtkSmartPointer<vtkStructuredPointsReader >::New();
	reader->SetFileName(temp_vtk_fn);
	reader->Update();

	vtkSmartPointer<vtkStructuredPoints> la_img_struct_pts = vtkSmartPointer<vtkStructuredPoints>::New();
	la_img_struct_pts = reader->GetOutput();
	
	vtkSmartPointer<vtkImageMapToColors> sliceXColors = vtkSmartPointer<vtkImageMapToColors>::New();
	vtkSmartPointer<vtkImageMapToColors> sliceYColors = vtkSmartPointer<vtkImageMapToColors>::New();
	vtkSmartPointer<vtkImageMapToColors> sliceZColors = vtkSmartPointer<vtkImageMapToColors>::New();

	
	int maxX, maxY, maxZ;
	_la_img->GetImageSize(maxX, maxY, maxZ);
	int zPos = maxZ/2, yPos = maxY/2, xPos = maxX/2;

	this->SetLookupTable();

	sliceZColors->SetInputData(la_img_struct_pts);
	sliceZColors->SetLookupTable(_bwLut);
	this->_zSlice->SetInputData(sliceZColors->GetOutput());
	this->_zSlice->SetDisplayExtent(0, maxX - 1, 0, maxY - 1, zPos, zPos);

	// X Plane
	sliceXColors->SetInputData(la_img_struct_pts);
	sliceXColors->SetLookupTable(_bwLut);
	this->_xSlice->SetInputData(sliceXColors->GetOutput());
	this->_xSlice->SetDisplayExtent(xPos, xPos, 0, maxY - 1, 0, maxZ - 1);

	// Y Plane
	sliceYColors->SetInputData(la_img_struct_pts);
	sliceYColors->SetLookupTable(_bwLut);
	this->_ySlice->SetInputData(sliceYColors->GetOutput());
	this->_ySlice->SetDisplayExtent(0, maxX - 1, yPos, yPos, 0, maxZ - 1);

	_renderer->AddActor(_xSlice);
	_renderer->AddActor(_ySlice);
	_renderer->AddActor(_zSlice);

}

void VizBox::ShowInit()
{
	_renWin->AddRenderer(_renderer);
	_renWin->Render();
}
