#include "../include/LaImageFeatures.h"

using namespace std;


LaImageFeatures::LaImageFeatures()
{
	
	

}

LaImageFeatures::~LaImageFeatures() {

	// Remove memory allocated on heap
}


void LaImageFeatures::SetInputData(LaImage* img) {
	_image = img; 
}

void LaImageFeatures::SetInputData2(LaImage* img) {
	_mask_image = img;
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