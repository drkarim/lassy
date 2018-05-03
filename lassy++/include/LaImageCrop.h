/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaImageCeop class crops an image based on a user-defined ROI 
*
*/
#pragma once
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <itkImageIterator.h>
#include "itkRescaleIntensityImageFilter.h"
#include <itkSmartPointer.h>
#include "itkVTKImageIO.h"
#include <itkMinimumMaximumImageCalculator.h>

#include <itkExtractImageFilter.h>


#include "LaImage.h"
#include "LaImageAlgorithms.h"

using namespace std;


class LaImageCrop : public LaImageAlgorithms {
private:
	
	LaImage* _image; 
	
	typedef unsigned int PixelTypeInt;
	typedef itk::Image< PixelTypeInt, 3 >  ImageTypeInt;

    int _x, _y, _z, _size_x, _size_y, _size_z;

    itk::SmartPointer<ImageTypeInt>  _image_cropped;
	
public:
	
	void SetInputData(LaImage* image); 			// intensity image 

	void SetOutputFile(const char* output);
	void SetStart(int x, int y, int z); 
	void SetSize(int size_x, int size_y, int size_z);

	LaImage* GetOutput();
	
	void Update();
	

	// constructors and desctructors
	LaImageCrop();
	~LaImageCrop();
	
}; 