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

#include "itkRescaleIntensityImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"

#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "itkImageDuplicator.h"

// Lassy++ includes 
#include "MathBox.h"

#include <string>
#include <iomanip>

#include "LaImage.h"
#include "LaImageAlgorithms.h"

using namespace std;


class LaImageFeatures : public LaImageAlgorithms {
private:
	
	LaImage* _image; 
	LaImage* _mask_image;
	
	string _csv_filename; 
	short _which_value;
	short _mask_val_SMD; /* which mask value for Signed Maurer Distance Filter */
	int _max_features; 
	long _total_size;
	double _my_nan;			/* Our own NaN, just a large number telling us there is no number present */
	vector<vector<double> > _image_features;
	
	typedef unsigned int PixelTypeInt;
	typedef itk::Image< PixelTypeInt, 3 >  ImageTypeInt;

	/*
	*	The index location in _image_features storing this feature
	*	Note this enumeration should not exceed _max_features (default is 15) unless set by SetMaxFeatures
	*/
	enum _feature_list
	{
		intensity = 0,		/* at index 0 in _image_features, and so on */
		pos_x = 1,	/* The xyz 3D co-ordinate of pixel is not a very useful feature but still needed for calculations */
		pos_y = 2, 
		pos_z = 3,
		grad_mag = 4,
		maurer_distance = 5,
		which_class=6,
		Last		    /* keep it as the last feature in the list for iterating: https://goo.gl/rWSYBj */
	};

	/*
	*	_Feature_index_map is the 3D input image with pixels that contain 
	*	index to the _image_features vector that store a maximum of _max_features (default=10)
	*	features for each pixel
	*/
	itk::SmartPointer<ImageTypeInt>  _feature_index_map;
	
public:
	
	void SetInputData(LaImage* image); 			// intensity image 
	void SetInputData2(LaImage* mask_img); 		// label image
	void SetOutputFile(const char* output);
	void SetPixelValue(short p); 
	void SetMaxFeatures(int max);

	void SetFeatureValue(int x, int y, int z, int feature_index, double feature_value);
	void ExtractFeature_Intensity_Pos();
	void ExtractFeature_GradientMagnitude();
	void ExtractFeature_SignedMaurerDistance();
	
	void Update();
	void Update_OLD(); 

	// constructors and desctructors
	LaImageFeatures();
	~LaImageFeatures();
	
}; 