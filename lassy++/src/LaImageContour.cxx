#define HAS_VTK 1

#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaImageContour.h"

using namespace std;


LaImageContour::LaImageContour()
{

}

LaImageContour::~LaImageContour()
{
    // delete
}

void LaImageContour::SetInputData(LaImage* greyscale)
{
	_input_greyscale = greyscale;
}

void LaImageContour::SetInputData2(LaImage* binary)
{
	_input_binary = binary;
}

void LaImageContour::Update()
{
	int max_x, max_y, max_z, max_x2, max_y2, max_z2; 
	typedef itk::Image< unsigned short, 3 >  ImageType;
	short outline;
	ImageType::IndexType pixelIndex;
	
	typedef itk::BinaryContourImageFilter <ImageType, ImageType > FilterType;
    FilterType::Pointer contour = FilterType::New();
    contour->SetInput(_input_binary->GetImage());
	contour->SetForegroundValue(0);
  	contour->SetBackgroundValue(1);
	contour->Update();
	ImageType::Pointer contour_img = contour->GetOutput();

	ImageType::Pointer greyscale = _input_greyscale->GetImage(); 

	_input_greyscale->GetImageSize(max_x, max_y, max_z);
	_input_binary->GetImageSize(max_x2, max_y2, max_z2);

	if (max_x != max_x2 || max_y != max_y2 || max_z != max_z2)
	{
		cerr << "\n\nERROR: The greyscale and input image dimensions dont match!\n";
		exit(1); 
	}

	// Now store magnitude for pixels in mask
	for (int x = 0; x < max_x; x++)
	{
		for (int y = 0; y < max_y; y++)
		{
			for (int z = 0; z < max_z; z++) {
				pixelIndex[0] = x;      // x position of the pixel
				pixelIndex[1] = y;      // y position of the pixel
				pixelIndex[2] = z;
				outline = contour_img->GetPixel(pixelIndex); 

				if (outline == 0) {

					_input_greyscale->GetImage()->SetPixel(pixelIndex,255);
					
				}

			}
		}
	}
	
	typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("contour.nii.gz");
  writer->SetInput(contour_img);
  writer->Update();

}

LaImage* LaImageContour::GetOutput()
{
	return _input_greyscale;
}



