/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaImageFeatures class extracts various image features around labels 
*	and writes it to a csv file to facilitate data analysis in another environment (e.g. Python)
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

#include <itkDenseFrequencyContainer2.h>
#include "itkHistogramToTextureFeaturesFilter.h"
#include "itkScalarImageToCooccurrenceMatrixFilter.h"
#include "itkVectorContainer.h"
#include "itkAddImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"



// Lassy++ includes 
#include "MathBox.h"

#include <string>

#include "LaImage.h"
#include "LaImageAlgorithms.h"

using namespace std;


class LaImageFeatures : public LaImageAlgorithms {
private:
	
	LaImage* _image; 
	LaImage* _mask_image;
	string _csv_filename; 
	short _which_value;
	
public:
	
	void SetInputData(LaImage* image); 			// intensity image 
	void SetInputData2(LaImage* mask_img); 		// label image
	void SetOutputFile(const char* output);
	void SetPixelValue(short p); 
	
	void Update(); 

	// constructors and desctructors
	LaImageFeatures();
	~LaImageFeatures();
	
}; 