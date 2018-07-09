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


class LaImageFloat {
private:
	
	typedef itk::Image<  float, 3 >    InputImageType;
	typedef itk::Image<  float, 3 >    OutputImageType;

	typedef itk::ImageRegionIterator<InputImageType>  IteratorType;

	itk::SmartPointer<InputImageType>  _image;


public:
	// Constructor with default values for data members
	LaImageFloat() {
		_image = InputImageType::New();
	}

	/*
	*	Needs an existing image NifTII, GIPL or NRRD 
	*/
	LaImageFloat(const char* img_fn)
	{
		typedef itk::ImageFileReader< InputImageType  >  ReaderType;
		typedef itk::ImageFileWriter< OutputImageType >  WriterType;
		ReaderType::Pointer reader = ReaderType::New();
		WriterType::Pointer writer = WriterType::New();

		reader->SetFileName(img_fn);
		reader->Update();

		_image = reader->GetOutput();

	}

    /*
	*	Returns pointer to the ITK image
	*/
	itk::SmartPointer<InputImageType> GetImage()
	{
		return _image;
	}


    // Returns the pixel value at an image location (x,y,z), returns false if position is out of bounds. 
	bool GetIntensityAt(int x, int  y, int z, float& pixelValue);

	// Returns the maximum size of a 3D image in every direction 
	bool GetImageSize(int& x, int& y, int& z);

    void GetMinimumMaximum(short& min, short& max);

    

	
	
	
};

