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

	_meshPolyData = vtkSmartPointer<vtkPolyData>::New();

	_renderer = vtkSmartPointer<vtkRenderer>::New();
	
	_xPos = 0; 
	_yPos = 0; 
	_zPos = 0; 

	_maxX = 0;
	_maxY = 0; 
	_maxZ = 0; 

	_mesh3DActor_opacity = 1;		// when displayed the mesh is fully opaque 
	
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

	img3d->GetImageSize(_maxX, _maxY, _maxZ);
	_zPos = _maxZ/2, _yPos = _maxY/2, _xPos = _maxX/2;

	cout << "X = " << _maxX << " ," << "Y = " << _maxY << ", Z = " << _maxZ << endl;
	cout << "xPos= " << _xPos << ", yPos=" << _yPos << ", zPos= " << _zPos << endl;

	this->SetLookupTable_image3d(img3d);

	sliceZColors->SetInputData(la_img_struct_pts);
	sliceZColors->SetLookupTable(_bwLut);
	//this->_zSlice->SetInputData(sliceZColors->GetOutput());
	_zSlice->GetMapper()->SetInputConnection(sliceZColors->GetOutputPort());
	_zSlice->SetDisplayExtent(0, _maxX - 1, 0, _maxY - 1, _zPos, _zPos);

	// X Plane
	sliceXColors->SetInputData(la_img_struct_pts);
	sliceXColors->SetLookupTable(_bwLut);
	//this->_xSlice->SetInputData(sliceXColors->GetOutput());
	_xSlice->GetMapper()->SetInputConnection(sliceXColors->GetOutputPort());
	_xSlice->SetDisplayExtent(_xPos, _xPos, 0, _maxY - 1, 0, _maxZ - 1);

	// Y Plane
	sliceYColors->SetInputData(la_img_struct_pts);
	sliceYColors->SetLookupTable(_bwLut);
	_ySlice->GetMapper()->SetInputConnection(sliceYColors->GetOutputPort());
	_ySlice->SetDisplayExtent(0, _maxX - 1, _yPos, _yPos, 0, _maxZ - 1);


	_renderer->AddActor(_xSlice);
	_renderer->AddActor(_ySlice);
	_renderer->AddActor(_zSlice);

}

void VizBox::CalculateContours(int direction, double slice)
{
	
	double bounds[6];
	
	_meshPolyData->GetBounds(bounds); 

	std::cout << "Bounds: "
		<< bounds[0] << ", " << bounds[1] << " "
		<< bounds[2] << ", " << bounds[3] << " "
		<< bounds[4] << ", " << bounds[5] << std::endl;
	
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin((bounds[1] + bounds[0]) / 2.0, (bounds[3] + bounds[2]) / 2.0, slice);
	plane->SetNormal(0, 0, 1);

	// Create cutter
	vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
	cutter->SetInputData(_meshPolyData);
	cutter->SetCutFunction(plane);

	// Create cutter
	double high = plane->EvaluateFunction((bounds[1] + bounds[0]) / 2.0, (bounds[3] + bounds[2]) / 2.0, bounds[5]);
	cutter->GenerateValues(10, .99, .99 * high);
	// cutter->GenerateValues(1, bounds[4], bounds[5]);
	

	vtkSmartPointer<vtkPolyDataMapper> cutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cutterMapper->SetInputConnection(cutter->GetOutputPort());
	cutterMapper->ScalarVisibilityOff();

	// Create cut actor
	vtkSmartPointer<vtkActor> cutterActor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
	cutterActor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());
	cutterActor->GetProperty()->SetLineWidth(4);
	cutterActor->SetMapper(cutterMapper);

	_renderer->AddActor(cutterActor);

}

void VizBox::ConstructMeshVisualiser(LaShell* mesh3d)
{
	vtkSmartPointer<vtkPolyDataMapper> MeshMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	
	mesh3d->GetMesh3D(_meshPolyData);
	MeshMapper->SetInputData(_meshPolyData);
	MeshMapper->SetColorModeToMapScalars();
	MeshMapper->SetScalarRange(0,5);

	this->SetLookupTable_mesh3d(mesh3d);
	MeshMapper->SetLookupTable(_colLut);

	_mesh3DActor->SetMapper(MeshMapper);
	_renderer->AddActor(_mesh3DActor);
}

void VizBox::ShowInit()
{
	_renderer->SetBackground(1,1,1);
	_renWin->AddRenderer(_renderer);
	_renWin->Render();
	_renWin->SetSize(640, 480);
	_renWin->SetWindowName("Lassy3D");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> imagestyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> camerastyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	

	vtkSmartPointer<vtkCallbackCommand> keypressCallback = vtkSmartPointer<vtkCallbackCommand>::New();
  	keypressCallback->SetCallback( VizBox::KeypressCallbackFunction );
	keypressCallback->SetClientData(this); 
  	renderWindowInteractor->AddObserver( vtkCommand::KeyPressEvent, keypressCallback );

	renderWindowInteractor->SetInteractorStyle(camerastyle);
	renderWindowInteractor->SetRenderWindow(_renWin);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}

void VizBox::KeypressCallbackFunction( vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* clientData, void* vtkNotUsed(callData) )
{
  std::cout << "Keypress callback" << std::endl;
 
  vtkRenderWindowInteractor *iren =  static_cast<vtkRenderWindowInteractor*>(caller);
  VizBox *vizbox = static_cast<VizBox*>(clientData); 
  // std::cout << "Pressed: " << iren->GetKeySym() << std::endl;                                                    
  char *key = iren->GetKeySym(); 
 
  switch (*key)
  {
	case 'X': 
		vizbox->MoveSlice(1, 1);
		break;

	case 'x': 
		vizbox->MoveSlice(1, -1);
		break; 

	case 'Y':
		vizbox->MoveSlice(2, 1);
		break;

	case 'y':
		vizbox->MoveSlice(2, -1);
		break;

	case 'Z':
		vizbox->MoveSlice(3, 1);
		break;

	case 'z':
		vizbox->MoveSlice(3, -1);
		break;
	
	case 'o': 
		vizbox->ChangeMeshOpacity(0.25);
		break; 
	
	case 't':
		vizbox->ChangeMeshOpacity(-0.25);
		break;
  }

}

void VizBox::ChangeMeshOpacity(double amount)
{
	double opac = _mesh3DActor_opacity; 
	cout << "changing opacity, currently = " << opac; 
	if (opac + amount > 0 && opac + amount <= 1)
	{
		_mesh3DActor_opacity += amount;
		cout << ", changing to = " << _mesh3DActor_opacity << endl;
		_mesh3DActor->GetProperty()->SetOpacity(_mesh3DActor_opacity);
		_renWin->Render();
	}
}

void VizBox::MoveSlice(int slice_dir, int increment)
{	
	vtkSmartPointer<vtkImageActor> slice;
	cout << "Moving slice: direction = " << slice_dir << ", increment = " << increment; 
	switch (slice_dir) {
		case 1:
			slice = _xSlice;
			
			if (_xPos+increment > 0 && _xPos + increment < _maxX)
			{
				cout << ", x direction\n";
				_xPos = _xPos+increment; 
				slice->SetDisplayExtent(_xPos, _xPos, 0, _maxY - 1, 0, _maxZ - 1);
			}

			
		break; 
		case 2: 
			slice = _ySlice; 
			 
			if (_yPos+increment > 0 && _yPos + increment < _maxY)
			{
				_yPos = _yPos+increment; 
				slice->SetDisplayExtent(0, _maxX - 1, _yPos, _yPos, 0, _maxZ - 1);
			}
			
		break; 
		case 3: 
			slice = _zSlice; 
			
			if (_zPos+increment > 0 && _zPos + increment < _maxZ)
			{
				_zPos = _zPos+increment; 
				slice->SetDisplayExtent(0, _maxX - 1, 0, _maxY - 1, _zPos, _zPos);

				CalculateContours(0, _zPos);
			}
			
		break;

	}

	_renWin->Render();
}
