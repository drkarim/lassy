/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaImage class encapsulates a 3D image that is read into memory as an ITK image
*	For more information: https://itk.org/Doxygen43/html/classitk_1_1Image.html
*
*
*/
#pragma once

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <itkImageIterator.h>
#include "itkRescaleIntensityImageFilter.h"
#include <itkSmartPointer.h>
#include "itkVTKImageIO.h"
#include "itkImage.h"
#include <itkMinimumMaximumImageCalculator.h>


// Lassy++ includes 
#include "MathBox.h"


#include <string>


using namespace std;


class LaImage {
private:
	
	typedef itk::Image< unsigned short, 3 >    InputImageType;
	typedef itk::Image< unsigned short, 3 >    OutputImageType_SHORT;

	typedef itk::ImageRegionIterator<InputImageType>  IteratorType;

	itk::SmartPointer<InputImageType>  _image;


public:
	// Constructor with default values for data members
	LaImage(const char* img_fn)
	{
		typedef itk::ImageFileReader< InputImageType  >  ReaderType;
		typedef itk::ImageFileWriter< OutputImageType_SHORT >  WriterType;
		ReaderType::Pointer reader = ReaderType::New();
		WriterType::Pointer writer = WriterType::New();

		reader->SetFileName(img_fn);
		reader->Update();

		_image = reader->GetOutput();

	}


	itk::SmartPointer<InputImageType> GetImage()
	{
		return _image;
	}

	
	void PixelToFile(const char* output_fn); 


	// Returns the pixel value at an image location (x,y,z), returns false if position is out of bounds. 
	bool GetIntensityAt(int x, int  y, int z, short& pixelValue);

	// Returns the maximum size of a 3D image in every direction 
	bool GetImageSize(int& x, int& y, int& z);

	/*
	*	Converts to a VTK structured points 
	*	This format can be processed by VTK algorithms such as Marching cubes 
	*/
	void ConvertToVTKImage(const char* vtk_fn);


	/*
	*	Returns the minimum and maximum intensity in an image 
	*/
	void GetMinimumMaximum(short& min, short& max);

	/*
	*	Transformations from World to image co-ordinate space 
	*	It follows the ITK-VTK convention of how world co-ordinate is defined from image space. 
	*	For more details please refer to: 
	*	https://wwwhomes.doc.ic.ac.uk/~rkarim/mediawiki/index.php?title=World_to_image_cordinate_systems
	*/
	void WorldToImage(double &x, double &y, double &z); 
	void ImageToWorld(float &x, float &y, float &z);
	
	/*
	*	Interrogates a 3D image along a straigth line. The straight line is defined by the vector <n_x, n_y, n_z>, positioned at (centre_x, centre_y, centre_z)
	*	Note that the interrogation is by default 4 pixels in both directions of the vector 
	*	There is an option to interrogate the image only in the region defined by the mask_img parameter 
	*/
	void InterrogateImage(double n_x, double n_y, double n_z, double centre_x, double centre_y, double centre_z, double& returnVal, double steps=4, bool doLogging=false, int CellID_ForLogging=0, LaImage* mask_img=NULL);
	void GetStatisticalMeasure(vector<Point3> vals, int measure, double& returnVal);
};

