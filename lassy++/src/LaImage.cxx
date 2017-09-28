#include <itkImageIterator.h>

#include <iostream>    // using IO functions
#include <string>      // using string

#include "../include/LaImage.h"
using namespace std;



// Pixel iterator 
void LaImage::PixelToFile(const char* output_fn)
{
	ofstream out;
	typedef itk::ImageRegionIterator<InputImageType>       IteratorType;

	out.open(output_fn);
	IteratorType  it(_image, _image->GetRequestedRegion());
	it.GoToBegin();

	while (!it.IsAtEnd())
	{
		out << it.Get() << "\t";
		++it;
	}
	out.close();
}

void LaImage::ConvertToVTKImage(const char* vtk_fn)
{

	typedef itk::VTKImageIO                    ImageIOType;
	typedef itk::Image< unsigned short, 3 >    ImageType;
	typedef itk::ImageFileWriter< ImageType >  WriterType;
	ImageIOType::Pointer vtkIO = ImageIOType::New();
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(vtk_fn);
	writer->SetInput(_image); 

	vtkIO->SetFileTypeToASCII();
	writer->SetImageIO(vtkIO);
	writer->Update();
}