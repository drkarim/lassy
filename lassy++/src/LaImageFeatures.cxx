#include "../include/LaImageFeatures.h"

using namespace std;


LaImageFeatures::LaImageFeatures()
{
	
	_feature_index_map = ImageTypeInt::New();
	_max_features = 10;

}

LaImageFeatures::~LaImageFeatures() {

	// Remove memory allocated on heap
	//delete _index_image; 
}


void LaImageFeatures::SetInputData(LaImage* img) {
	_image = img; 

	int max_x, max_y, max_z; 
	 _image->GetImageSize(max_x,max_y,max_z);
	long total_size = max_x*max_y*max_z;
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
		
				_feature_index_map->SetPixel(pixelIndex, index++);
			}
		}
	}

	// set feature vector 
	_image_features = vector<vector<int> >(total_size, vector<int>(_max_features));
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

void LaImageFeatures::Update()
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