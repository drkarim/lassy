
#include <itkImageIterator.h>

#include <iostream>    // using IO functions
#include <string>      // using string

#include "../include/LaImageFloat.h"

bool LaImageFloat::GetImageSize(int& x, int& y, int& z)
{
	bool is_within_limits = false; 
	typedef  float PixelType;
	typedef itk::Image< PixelType, 3 >  ImageType;
	ImageType::RegionType region = _image->GetLargestPossibleRegion();

	int size_x = region.GetSize()[0];
	int size_y = region.GetSize()[1];
	int size_z = region.GetSize()[2];

	if (!(x >= 0 && x < size_x || (y >= 0 && y < size_y) || (z >= 0 && z < size_z)))
	{
		is_within_limits = false;
	}
	else
	{
		is_within_limits = true;
	}

	x = size_x; 
	y = size_y; 
	z = size_z; 
	
	return is_within_limits;

}

bool LaImageFloat::GetIntensityAt(int x, int  y, int z, float& pixelValue)
{

	typedef  float PixelType;
	typedef itk::Image< PixelType, 3 >  ImageType;
	
	int Max_X=x, Max_Y=y, Max_Z=z; 

	if (this->GetImageSize(Max_X, Max_Y, Max_Z))
	{
		ImageType::IndexType pixelIndex;
		pixelIndex[0] = x;
		pixelIndex[1] = y;
		pixelIndex[2] = z;

		pixelValue = _image->GetPixel(pixelIndex);

		return true; 
	}
	else {
		return false; 
	}
}

void LaImageFloat::GetMinimumMaximum(short& min, short& max)
{
	typedef float PixelType;
	typedef itk::Image< PixelType, 3 >  ImageType;
	typedef itk::MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;

	ImageCalculatorFilterType::Pointer imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(_image);
	imageCalculatorFilter->Compute();

	min = imageCalculatorFilter->GetMinimum(); 
	max = imageCalculatorFilter->GetMaximum(); 
}
