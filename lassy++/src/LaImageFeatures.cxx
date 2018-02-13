#include "../include/LaImageFeatures.h"

using namespace std;


LaImageFeatures::LaImageFeatures()
{
	
	_feature_index_map = ImageTypeInt::New();
	_max_features = 15;
	_total_size = 0;
	_my_nan = -1e9;
}

LaImageFeatures::~LaImageFeatures() {

	// Remove memory allocated on heap
	//delete _index_image; 
}


void LaImageFeatures::SetInputData(LaImage* img) {
	_image = img; 

	int max_x, max_y, max_z; 
	 _image->GetImageSize(max_x,max_y,max_z);
	_total_size = max_x*max_y*max_z;
	 /*int _default_feature_value = -1;
	 _image_features = vector<vector<vector<vector<int> > > >( _max_features, vector<vector<vector<int> > >(max_z, vector<vector<int> >(max_y, vector<int>(max_x, _default_feature_value))));*/
	
	 // copy structure for index image 
 	ImageTypeInt::RegionType region;
  	ImageTypeInt::IndexType start;
	start[0] = 0; start[1] = 0; start[2] = 0;
	ImageTypeInt::SizeType size;
	size[0] = max_x; size[1] = max_y; size[2] = max_z; 
	region.SetSize(size);
	region.SetIndex(start);

  	_feature_index_map->SetRegions(region);
  	_feature_index_map->Allocate();

	// reset index image values
	int index=0;
	// Set pixels in a square to one value
	for(int x = 0; x < max_x; x++)
	{
		for(int y = 0; y < max_y; y++)
		{
			for(int z = 0; z < max_z; z++) { 
				
				ImageTypeInt::IndexType pixelIndex;
				pixelIndex[0] = x; pixelIndex[1] = y; pixelIndex[2] = z;
		
				_feature_index_map->SetPixel(pixelIndex, index++);	// storing index to _image_features vector 
			}
		}
	}

	// set feature vector 
	_image_features = vector<vector<double> >(_total_size, vector<double>(_max_features));

	for (int i = 0; i < _total_size; i++)
	{
		for (int j = 0; j < _max_features; j++)
		{
			_image_features[i][j] = _my_nan;
		}
	}
}

void LaImageFeatures::SetInputData2(LaImage* img) {
	_mask_image = img;
}

void LaImageFeatures::SetMaxFeatures(int max)
{
	_max_features = max;
}

void LaImageFeatures::SetPixelValue(short pixel_value) {
	_which_value = pixel_value;
}

void LaImageFeatures::SetOutputFile(const char* csv_filename)
{
	_csv_filename = std::string(csv_filename);
}

void LaImageFeatures::SetFeatureValue(int x, int y, int z, int feature_index, double feature_value)
{
	ImageTypeInt::IndexType pixelIndex;

	pixelIndex[0] = x;      
	pixelIndex[1] = y;      
	pixelIndex[2] = z; 

	double index = _feature_index_map->GetPixel(pixelIndex);

	if (index >= 0 && index < _total_size) {
		if (feature_index < _max_features)
		{
			_image_features[index][feature_index] = feature_value;
			return; 
		}
	}
	cout << "\nError in SetFeatureValue(): Could not assign feature value - out of bounds" << endl;

}

/*
*	The first discerning feature (i.e. intensity) 
*	to determine whether a pixel has any listed features at all 
*/
void LaImageFeatures::ExtractFeature_Intensity_Pos()
{
	int max_x, max_y, max_z; 
	short mask_value, intensity_value;

	_image->GetImageSize(max_x, max_y, max_z);
	for (int x = 0; x < max_x; x++)
	{
		for (int y = 0; y < max_y; y++)
		{
			for (int z = 0; z < max_z; z++) {
				
				_mask_image->GetIntensityAt(x, y, z, mask_value);
				_image->GetIntensityAt(x, y, z, intensity_value); 

				if (mask_value > 0)
				{
					int index_of_intensity_feature = LaImageFeatures::intensity;
					this->SetFeatureValue(x, y, z, index_of_intensity_feature, intensity_value);

					// Set XYZ 3D position 
					int index_pos_X = LaImageFeatures::pos_x;
					int index_pos_Y = LaImageFeatures::pos_y;
					int index_pos_Z = LaImageFeatures::pos_z;
					int index_pos_mask = LaImageFeatures::which_class;

					this->SetFeatureValue(x, y, z, index_pos_X, x);
					this->SetFeatureValue(x, y, z, index_pos_Y, y);
					this->SetFeatureValue(x, y, z, index_pos_Z, z);

					this->SetFeatureValue(x, y, z, index_pos_mask, mask_value);
				}

			}
		}
	}
}



void LaImageFeatures::Update()
{

	// First Extract features 
	ExtractFeature_Intensity_Pos();

	// Write Features to File 
	ofstream out;
	out.open(_csv_filename, std::fstream::out | std::fstream::trunc);
	out << "Intensity,X,Y,Z,Class" << endl;
	stringstream ss; 

	for (int i = 0; i < _total_size; i++)
	{
		double intensity_feature = _image_features[i][0];

		for (int j = 0; j < _max_features && intensity_feature != _my_nan ; j++)
		{
			
			// running through all available features, taking index from our enumeration
			// see enum: _feature_list
			for (int k = LaImageFeatures::intensity; k != LaImageFeatures::Last; k++)
			{
				_feature_list feature_index = static_cast<_feature_list>(k);
				double feature_value = _image_features[i][feature_index];
				
				if (feature_value != _my_nan)
				{
					if (k == 0)
						ss << feature_value;
					else 
						ss << "," << feature_value; 
				}

			}
			ss << endl; 
			out << ss.str().c_str(); 
			ss.str(std::string());
		}
	}
	out.close();

}


void LaImageFeatures::Update_OLD()
{
	typedef itk::Image< unsigned short, 3 >    InputImageType;
	typedef itk::Neighborhood<unsigned short, 3> NeighborhoodType;
	typedef itk::ImageRegionIterator<InputImageType>  IteratorType;
	typedef itk::RegionOfInterestImageFilter<InputImageType, InputImageType> roiType;
	typedef itk::Statistics::ScalarImageToCooccurrenceMatrixFilter<InputImageType> Image2CoOccuranceType;
	typedef Image2CoOccuranceType::HistogramType HistogramType;
	typedef itk::Statistics::HistogramToTextureFeaturesFilter<HistogramType> Hist2FeaturesType;
	typedef InputImageType::OffsetType OffsetType;


	InputImageType::Pointer mask = _mask_image->GetImage();
	InputImageType::Pointer org_image = _image->GetImage();
	
	ofstream out;
	out.open(_csv_filename, std::fstream::out | std::fstream::trunc);

	roiType::Pointer roi = roiType::New();
	roi->SetInput(org_image);
	
	InputImageType::RegionType window;
	InputImageType::RegionType::SizeType size;
	
	//window size=3x3x3, do not change without changng for loop 
	// for further info: https://public.kitware.com/pipermail/insight-users/2014-October/051018.html
	size.Fill(3); 
	window.SetSize(size);
	InputImageType::IndexType pi; //pixel index

	NeighborhoodType neighborhood;
	neighborhood.SetRadius(1);
	unsigned int centerIndex = neighborhood.GetCenterNeighborhoodIndex();
	OffsetType offset;
	short mask_value; 

	centerIndex = 1;		// only one direction 
	
	out << "Offset,X,Y,Z,inertia,correlation,energy,intensity" << endl;
	for (unsigned int d = 0; d < centerIndex; d++)		// 13 offsets  i.e. 13 directions 
	{
		offset = neighborhood.GetOffset(d);
		Image2CoOccuranceType::Pointer glcmGenerator = Image2CoOccuranceType::New();
		glcmGenerator->SetOffset(offset);
		glcmGenerator->SetNumberOfBinsPerAxis(16); //reasonable number of bins
		glcmGenerator->SetPixelValueMinMax(0, 255); //for input UCHAR pixel type
		Hist2FeaturesType::Pointer featureCalc = Hist2FeaturesType::New();

		//slide window over the entire image
		for (unsigned x = 1; x < org_image->GetLargestPossibleRegion().GetSize(0) - 1; x++)
		{
			pi.SetElement(0, x);
			window.SetIndex(0, x - 1);
			for (unsigned y = 1; y < org_image->GetLargestPossibleRegion().GetSize(1) - 1; y++)
			{
				
				pi.SetElement(1, y);
				window.SetIndex(1, y - 1);
				for (unsigned z = 1; z < org_image->GetLargestPossibleRegion().GetSize(2) - 1; z++)
				{
					_mask_image->GetIntensityAt(x, y, z, mask_value);

					if (mask_value == _which_value)		// only selected pixels with a certain value
					{
					pi.SetElement(2, z);
						window.SetIndex(2, z - 1);		// window placed at top-left of current pixel 
						roi->SetRegionOfInterest(window);
						roi->Update();
						glcmGenerator->SetInput(roi->GetOutput());
						glcmGenerator->Update();
						featureCalc->SetInput(glcmGenerator->GetOutput());
						featureCalc->Update();

						// total 9 features 
						double inertia = featureCalc->GetFeature(Hist2FeaturesType::Inertia);
						double correlation = featureCalc->GetFeature(Hist2FeaturesType::Correlation);
						double energy = featureCalc->GetFeature(Hist2FeaturesType::Energy);
						short intensity; 
						_image->GetIntensityAt(x, y, z, intensity);

						out << d << "," << x << "," << y << "," << z << "," << inertia << "," << correlation << "," << energy <<"," << intensity << endl;
					
					}	// end if 
				}		// end for 
				

			} // end for 

		} // end for 
	}

	out.close();

	
}