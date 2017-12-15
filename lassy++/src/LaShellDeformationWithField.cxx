#define HAS_VTK 1

#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellDeformationWithField.h"

using namespace std;


LaShellDeformationWithField::LaShellDeformationWithField()
{
	_source_la = new LaShell();
	_output_la = new LaShell();
	_SourcePolyData = vtkSmartPointer<vtkPolyData>::New();
	_DeformedMesh = vtkSmartPointer<vtkPolyData>::New();

	_vertexPostDeformation = vtkSmartPointer<vtkPoints>::New(); 
	
}

LaShellDeformationWithField::~LaShellDeformationWithField() {
	delete _source_la; 
	delete _output_la; 
}

void LaShellDeformationWithField::SetInputData(LaShell* shell)
{
    _source_la = shell; 
    _source_la->GetMesh3D(_SourcePolyData);
}



LaShell* LaShellDeformationWithField::GetOutput() {
	return _output_la;
}

// See https://www.vtk.org/Wiki/VTK/Examples/Cxx/Filtering/SurfaceFromUnorganizedPointsWithPostProc
void LaShellDeformationWithField::ReconstructSurface(vtkSmartPointer<vtkPoints> points)
{
	vtkSmartPointer<vtkSurfaceReconstructionFilter> surf = vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	polydata->SetPoints(_vertexPostDeformation);
	surf->SetInputData(polydata);

	vtkSmartPointer<vtkContourFilter> contourFilter = vtkSmartPointer<vtkContourFilter>::New();
	contourFilter->SetInputConnection(surf->GetOutputPort());
	contourFilter->SetValue(0, 0.0);

	// Sometimes the contouring algorithm can create a volume whose gradient
	// vector and ordering of polygon (using the right hand rule) are
	// inconsistent. vtkReverseSense cures this problem.
	vtkSmartPointer<vtkReverseSense> reverse = vtkSmartPointer<vtkReverseSense>::New();
	reverse->SetInputConnection(contourFilter->GetOutputPort());
	reverse->ReverseCellsOn();
	reverse->ReverseNormalsOn();
	reverse->Update();

	_DeformedMesh = transform_back(points, reverse->GetOutput());

}

// copied ditto from https://www.vtk.org/Wiki/VTK/Examples/Cxx/Filtering/SurfaceFromUnorganizedPointsWithPostProc
// The objective is again to use vtkSurfaceReconstructionFilter to construct a surface from a set of points. 
// However, the direct output of the vtkSurfaceReconstructionFilter does not match with the spatial position of the original point set. 
// Instead, there is a scale change and translation in between. To cure this, in this example we added a post transformation 
// to move the reconstructed surface back to the position of the original points, which in some cases might be more favorable.
vtkSmartPointer<vtkPolyData> LaShellDeformationWithField::transform_back(vtkSmartPointer<vtkPoints> pt, vtkSmartPointer<vtkPolyData> pd)
{
	// The reconstructed surface is transformed back to where the
	// original points are. (Hopefully) it is only a similarity
	// transformation.

	// 1. Get bounding box of pt, get its minimum corner (left, bottom, least-z), at c0, pt_bounds

	// 2. Get bounding box of surface pd, get its minimum corner (left, bottom, least-z), at c1, pd_bounds

	// 3. compute scale as: 
	//       scale = (pt_bounds[1] - pt_bounds[0])/(pd_bounds[1] - pd_bounds[0]);

	// 4. transform the surface by T := T(pt_bounds[0], [2], [4]).S(scale).T(-pd_bounds[0], -[2], -[4])


	// 1.
	double pt_bounds[6];  // (xmin,xmax, ymin,ymax, zmin,zmax)
	pt->GetBounds(pt_bounds);


	// 2.
	double pd_bounds[6];  // (xmin,xmax, ymin,ymax, zmin,zmax)
	pd->GetBounds(pd_bounds);

	//   // test, make sure it is isotropic
	//   std::cout<<(pt_bounds[1] - pt_bounds[0])/(pd_bounds[1] - pd_bounds[0])<<std::endl;
	//   std::cout<<(pt_bounds[3] - pt_bounds[2])/(pd_bounds[3] - pd_bounds[2])<<std::endl;
	//   std::cout<<(pt_bounds[5] - pt_bounds[4])/(pd_bounds[5] - pd_bounds[4])<<std::endl;
	//   // TEST  


	// 3
	double scale = (pt_bounds[1] - pt_bounds[0]) / (pd_bounds[1] - pd_bounds[0]);


	// 4.
	vtkSmartPointer<vtkTransform> transp = vtkSmartPointer<vtkTransform>::New();
	transp->Translate(pt_bounds[0], pt_bounds[2], pt_bounds[4]);
	transp->Scale(scale, scale, scale);
	transp->Translate(-pd_bounds[0], -pd_bounds[2], -pd_bounds[4]);

	vtkSmartPointer<vtkTransformPolyDataFilter> tpd = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
#if VTK_MAJOR_VERSION <= 5
	tpd->SetInput(pd);
#else
	tpd->SetInputData(pd);
#endif
	tpd->SetTransform(transp);
	tpd->Update();


	return tpd->GetOutput();
}



void LaShellDeformationWithField::Update() {

	// VTK error logging 
	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow = vtkSmartPointer<vtkFileOutputWindow>::New();
	fileOutputWindow->SetFileName("vtkLog.txt");
	vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
	if (outputWindow)
	{
		outputWindow->SetInstance(fileOutputWindow);
	}

	// compute normals for finding direction of displacement 
	vtkSmartPointer<vtkPolyDataNormals> Source_Poly_Normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	Source_Poly_Normals->ComputeCellNormalsOn();
	Source_Poly_Normals->SetInputData(_SourcePolyData);
	Source_Poly_Normals->Update();
	_SourcePolyNormals = vtkFloatArray::SafeDownCast(_SourcePolyData->GetPointData()->GetNormals());

	vtkSmartPointer<vtkFloatArray> Deformation_Field = vtkSmartPointer<vtkFloatArray>::New();
	Deformation_Field = vtkFloatArray::SafeDownCast(_SourcePolyData->GetPointData()->GetScalars());

	double *normal_vec = new double[3]; 
	double *after_deform = new double[3]; 
	double *before_deform = new double[3];
	
	

	for (vtkIdType i = 0; i < _SourcePolyData->GetNumberOfPoints(); ++i) {
		
		normal_vec[0] = 0; 
		normal_vec[1] = 0;
		normal_vec[2] = 0;

		_SourcePolyData->GetPoint(i, before_deform);

		double deformation = Deformation_Field->GetTuple1(i); 

		// the deformation is x steps in the direction of normal 
		_SourcePolyNormals->GetTuple(i, normal_vec);

		after_deform[0] = before_deform[0] + (deformation*normal_vec[0]);
		after_deform[1] = before_deform[1] + (deformation*normal_vec[1]);
		after_deform[2] = before_deform[2] + (deformation*normal_vec[2]);

		_vertexPostDeformation->InsertNextPoint(after_deform[0], after_deform[1], after_deform[2]); 

	}

	ReconstructSurface(_vertexPostDeformation);

	_output_la->SetMesh3D(_DeformedMesh);

	delete [] normal_vec; 
	delete [] after_deform; 
	delete [] before_deform; 

	
}
