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

/*
*	Converts an ITK image to VTK image which is a Structured points data-set 
*/
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

void LaImage::WorldToImage(double &x, double &y, double &z)
{
	typedef itk::Image< unsigned short, 3 >  ImageType;
	ImageType::PointType point;
	ImageType::IndexType pixelIndex;

	point[0] = x; 
	point[1] = y; 
	point[2] = z; 
	
	_image->TransformPhysicalPointToIndex(point, pixelIndex);
	x = pixelIndex[0]; 
	y = pixelIndex[1];
	z = pixelIndex[2];
}

void LaImage::ImageToWorld(float &x, float &y, float &z)
{
	// nothing yet 
}

bool LaImage::GetImageSize(int& x, int& y, int& z)
{
	bool is_within_limits = false; 
	typedef unsigned short PixelType;
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

bool LaImage::GetIntensityAt(int x, int  y, int z, short& pixelValue)
{

	typedef unsigned short PixelType;
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

void LaImage::InterrogateImage(double n_x, double n_y, double n_z, double centre_x, double centre_y, double centre_z, double& returnVal)
{
	double scar_step_min=-4, scar_step_max=4, scar_step_size=1; 

	int size = 0, j = 0, indexOfPointInArray = -1, currIndex = 0, a, b, c;
	double insty = 0, x = 0, y = 0, z = 0;
	typedef itk::Image< unsigned short, 3 >  ImageType;
	ImageType::PointType point;

	vector<Point3> pointsOnAndAroundNormal;

	int MaxX, MaxY, MaxZ; 
	this->GetImageSize(MaxX, MaxY, MaxZ);

	// normalize
	MathBox::normalizeVector(n_x, n_y, n_z);

	for (double i = scar_step_min; i <= scar_step_max; i += scar_step_size)
	{
		x = centre_x + (i*n_x);
		y = centre_y + (i*n_y);
		z = centre_z + (i*n_z);
		x = floor(x); y = floor(y); z = floor(z);
		for (a = -1; a <= 1; a++) {
			for (b = -1; b <= 1; b++) {
				for (c = -1; c <= 1; c++) {
					if (x + a >= 0 && x + a<MaxX && y + b >= 0 && y + b<MaxY && z + c >= 0 && z + c<MaxZ) {
						pointsOnAndAroundNormal.push_back(Point3(x + a, y + b, z + c));
					}
				}
			}
		}
	}
	
	

	this->GetStatisticalMeasure(pointsOnAndAroundNormal, 1, insty);			// statistical measure 2 returns max 
	insty = insty / 100;
		
	pointsOnAndAroundNormal.clear();

	returnVal = insty;
	
}


void LaImage::GetStatisticalMeasure(vector<Point3> vals, int measure, double& returnVal)
{
	double sum = 0, max = -1; 
	//double visitedStatus;
	int size = vals.size();
	short pixelValue; 

	if (measure == 1)			// reutrn mean 
	{
		for (int i = 0; i<size; i++)
		{
			this->GetIntensityAt(vals[i]._x, vals[i]._y, vals[i]._z, pixelValue);
			sum += pixelValue; 
		}
		returnVal = sum / size;
	}
	else if (measure == 2)			// return max 
	{
		/*
		for (int i = 0; i<size; i++)
		{
			_image->GetIntensityAt(vals[i]._x, vals[i]._x, vals[i]._z, pixelValue);

			// **************** VISITED NOT IMPLEMENTED YET ****************
			//visitedStatus = visited_im->Get(vals[i]._x, vals[i]._y, vals[i]._z);
			visitedStatus = 0; 

			if (pixelValue > max && visitedStatus < 1) {
				max = pixelValue;
				maxIndex = i;
			}
		}
		if (max == -1)
			returnVal = 0;
		else {
			returnVal = max;

			// now change the visited status of this max pixel 
			visited_im->Put(vals[maxIndex]->_x, vals[maxIndex]->_y, vals[maxIndex]->_z, 1);
		}*/
	}
	else if (measure == 3)			// sum along the normal (integration)
	{
		for (int i = 0; i<size; i++)
		{
			this->GetIntensityAt(vals[i]._x, vals[i]._y, vals[i]._z, pixelValue);
			sum += pixelValue;
		}
		returnVal = sum;
	}


}