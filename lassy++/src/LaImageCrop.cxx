#include "../include/LaImageCrop.h"

using namespace std;


LaImageCrop::LaImageCrop()
{
	_x = 0; 
    _y = 0; 
    _z = 0; 
    _size_x = 0; 
    _size_y = 0; 
    _size_z = 0; 
    _image_cropped = ImageTypeInt::New();
	
}

LaImageCrop::~LaImageCrop() {

	// Remove memory allocated on heap
	//delete _index_image; 
}

void LaImageCrop::SetStart(int x, int y, int z) {
    _x = x; 
    _y = y; 
    _z = z; 
}

void LaImageCrop::SetSize(int size_x, int size_y, int size_z){
    _size_x = size_x;
    _size_y = size_y;
    _size_z = size_z;
}


void LaImageCrop::SetInputData(LaImage* img) {
	_image = img; 

	int max_x, max_y, max_z; 
	 _image->GetImageSize(max_x, max_y, max_z);
	
    _size_x = max_x; 
    _size_y = max_y; 
    _size_z = max_z;
	
  
}


LaImage* LaImageCrop::GetOutput(){
    return _image_cropped;
}

void LaImageCrop::Update()
{

    typedef itk::ExtractImageFilter< ImageTypeInt, ImageTypeInt > FilterType;

    // copy structure for index image 
 	ImageTypeInt::RegionType region;
  	
    ImageTypeInt::IndexType start;
	start[0] = _x; start[1] = _y; start[2] = _z;
	
    ImageTypeInt::SizeType size;
	size[0] = _size_x; size[1] = _size_y; size[2] = _size_z;
	
    region.SetSize(size);
	region.SetIndex(start);

    FilterType::Pointer filter = FilterType::New();
    filter->SetExtractionRegion(region);
    filter->SetInput(_image->GetImage());
    #if ITK_VERSION_MAJOR >= 4
    filter->SetDirectionCollapseToIdentity(); // This is required.
    #endif
    filter->Update();

    _image_cropped = filter->GetOutput();

	
}