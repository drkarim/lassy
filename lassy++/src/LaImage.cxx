#include <itkImageIterator.h>

#include <iostream>    // using IO functions
#include <string>      // using string

#include "../include/LaImage.h"
#include "../include/CSVReader.h"
using namespace std;


void LaImage::Export(const char* output_fn)
{
	typedef itk::Image< unsigned short, 3 >    ImageType;
	typedef  itk::ImageFileWriter< ImageType  > WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(output_fn);
	writer->SetInput(_image);
	writer->Update();
}

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

void LaImage::FileToPixel(const char* input_fn)
{
	ifstream infile(input_fn); 
	double x,y,z,pixel; 
	typedef unsigned short PixelType;
	typedef itk::Image< PixelType, 3 >  ImageType;
	ImageType::RegionType region = _image->GetLargestPossibleRegion();

	int size_x = region.GetSize()[0];
	int size_y = region.GetSize()[1];
	int size_z = region.GetSize()[2];

	vector<vector<string> > csv_content = CSVReader::readCSV(infile);

	// The CSV iterator is from here: 
	// https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
	for (int i=0;i<csv_content.size();i++)
    {
		x=-1; y=-1; z=-1; pixel=-1;
		vector<string> line = csv_content[i]; 
		for (int j=0;j<line.size();j++)
		{
			int num = atoi(line[j].c_str()); 
			if (j==0) x = num ;
			else if (j==1) y = num ;
			else if (j==2) z = num ;
			else if (j==3) pixel = num;
		}
		cout << "read line:  " << x << ","  << y << "," << z << "," << pixel << endl;
		if (x >= 0 && x < size_x && y >= 0 && y < size_y && z >=0 && z < size_z && pixel >= 0)
		{
			ImageType::IndexType pixelIndex;
			pixelIndex[0] = x;
			pixelIndex[1] = y;
			pixelIndex[2] = z;

			_image->SetPixel(pixelIndex, pixel);
		}
		else {
			cout << "\nOut of bounds while reading line: " << x << ","  << y << "," << z << "," << pixel << ", carrying on ...";
		}
		
	}
}

void LaImage::EmptyImage()
{
	typedef itk::ImageRegionIterator<InputImageType>       IteratorType;

	IteratorType  it(_image, _image->GetRequestedRegion());
	it.GoToBegin();

	while (!it.IsAtEnd())
	{
		it.Set(0);
		++it;
	}
	
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

void LaImage::GetMinimumMaximum(short& min, short& max)
{
	typedef unsigned short PixelType;
	typedef itk::Image< PixelType, 3 >  ImageType;
	typedef itk::MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;

	ImageCalculatorFilterType::Pointer imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(_image);
	imageCalculatorFilter->Compute();

	min = imageCalculatorFilter->GetMinimum(); 
	max = imageCalculatorFilter->GetMaximum(); 
}


void LaImage::DeepCopy(LaImage* output_img)
{
	typedef unsigned short PixelType;
	typedef itk::Image< PixelType, 3 >  ImageType;
	
	ImageType::Pointer output = output_img->GetImage(); 

	output->SetRegions(_image->GetLargestPossibleRegion());
	output->Allocate();

	itk::ImageRegionConstIterator<ImageType> inputIterator(_image, _image->GetLargestPossibleRegion());
	itk::ImageRegionIterator<ImageType> outputIterator(output, output->GetLargestPossibleRegion());

	while(!inputIterator.IsAtEnd())
	{
		outputIterator.Set(inputIterator.Get());
		++inputIterator;
		++outputIterator;
	}
}
