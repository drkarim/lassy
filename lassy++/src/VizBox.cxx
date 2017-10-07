#define HAS_VTK 1


/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/VizBox.h"


using namespace std;


VizBox::VizBox()
{
	_xSlice = vtkSmartPointer<vtkImageActor>::New();
	_ySlice = vtkSmartPointer<vtkImageActor>::New();
	_zSlice = vtkSmartPointer<vtkImageActor>::New();
	_mesh3DActor = vtkSmartPointer<vtkActor>::New();
	_renWin = vtkSmartPointer<vtkRenderWindow>::New();

	_renderer = vtkSmartPointer<vtkRenderer>::New();
	
	
}


void VizBox::SetLookupTable_image3d(LaImage* img3d)
{
	short min, max; 
	cout << "Setting black-white lookup table ... ";
	img3d->GetMinimumMaximum(min, max);
	_bwLut = vtkSmartPointer<vtkLookupTable>::New();

	_bwLut->SetRange(0, max); // image intensity range
	_bwLut->SetValueRange(0.0, 1.0); // from black to white
	_bwLut->SetSaturationRange(0.0, 0.0); // no color saturation
	_bwLut->SetRampToLinear();
	_bwLut->Build();
	cout << "completed! ";

}

void VizBox::SetLookupTable_mesh3d(LaShell* mesh3d)
{
	double min, max; 
	cout << "Setting colour lookup table ... ";
	mesh3d->GetMinimumMaximum(min, max);
	_colLut = vtkSmartPointer<vtkLookupTable>::New();


	_colLut->SetNumberOfColors(256);
	_colLut->SetTableRange(0,1);
	_colLut->SetHueRange (0.6667, 0.0);
	_colLut->SetSaturationRange (1.0, 1.0);
	_colLut->SetValueRange (1.0, 1.0);
	_colLut->Build();
	cout << "completed! ";

}

void VizBox::ConstructImageOrthogonalPlanes(LaImage* img3d)
{
	
	// Convert image to structured points 
	char* temp_vtk_fn = "temp_123234.vtk";
	img3d->ConvertToVTKImage(temp_vtk_fn);
	vtkSmartPointer<vtkStructuredPointsReader > reader = vtkSmartPointer<vtkStructuredPointsReader >::New();
	reader->SetFileName(temp_vtk_fn);
	reader->Update();

	vtkSmartPointer<vtkStructuredPoints> la_img_struct_pts = vtkSmartPointer<vtkStructuredPoints>::New();
	la_img_struct_pts = reader->GetOutput();
	
	vtkSmartPointer<vtkImageMapToColors> sliceXColors = vtkSmartPointer<vtkImageMapToColors>::New();
	vtkSmartPointer<vtkImageMapToColors> sliceYColors = vtkSmartPointer<vtkImageMapToColors>::New();
	vtkSmartPointer<vtkImageMapToColors> sliceZColors = vtkSmartPointer<vtkImageMapToColors>::New();

	int maxX, maxY, maxZ;
	img3d->GetImageSize(maxX, maxY, maxZ);
	int zPos = maxZ/2, yPos = maxY/2, xPos = maxX/2;

	this->SetLookupTable_image3d(img3d);

	sliceZColors->SetInputData(la_img_struct_pts);
	sliceZColors->SetLookupTable(_bwLut);
	//this->_zSlice->SetInputData(sliceZColors->GetOutput());
	_zSlice->GetMapper()->SetInputConnection(sliceZColors->GetOutputPort());
	_zSlice->SetDisplayExtent(0, maxX - 1, 0, maxY - 1, zPos, zPos);

	// X Plane
	sliceXColors->SetInputData(la_img_struct_pts);
	sliceXColors->SetLookupTable(_bwLut);
	//this->_xSlice->SetInputData(sliceXColors->GetOutput());
	_xSlice->GetMapper()->SetInputConnection(sliceXColors->GetOutputPort());
	_xSlice->SetDisplayExtent(xPos, xPos, 0, maxY - 1, 0, maxZ - 1);

	// Y Plane
	sliceYColors->SetInputData(la_img_struct_pts);
	sliceYColors->SetLookupTable(_bwLut);
	_ySlice->GetMapper()->SetInputConnection(sliceYColors->GetOutputPort());
	_ySlice->SetDisplayExtent(0, maxX - 1, yPos, yPos, 0, maxZ - 1);


	_renderer->AddActor(_xSlice);
	_renderer->AddActor(_ySlice);
	_renderer->AddActor(_zSlice);

}

void VizBox::ConstructMeshVisualiser(LaShell* mesh3d)
{
	vtkSmartPointer<vtkPolyDataMapper> MeshMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyData> meshPolyData = vtkSmartPointer<vtkPolyData>::New();
	mesh3d->GetMesh3D(meshPolyData);
	MeshMapper->SetInputData(meshPolyData);
	MeshMapper->SetColorModeToMapScalars();
	MeshMapper->SetScalarRange(0,5);

	this->SetLookupTable_mesh3d(mesh3d);
	MeshMapper->SetLookupTable(_colLut);

	_mesh3DActor->SetMapper(MeshMapper);
	_renderer->AddActor(_mesh3DActor);
}

void VizBox::ShowInit()
{
	_renWin->AddRenderer(_renderer);
	_renWin->Render();
	_renWin->SetSize(640, 480);
	_renWin->SetWindowName("Lassy3D");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> imagestyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> camerastyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	renderWindowInteractor->SetInteractorStyle(camerastyle);
	renderWindowInteractor->SetRenderWindow(_renWin);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
