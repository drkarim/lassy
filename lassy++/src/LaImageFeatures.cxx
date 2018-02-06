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


void LaImageFeatures::SetOutputFile(const char* csv_filename)
{
	_csv_filename = std::string(csv_filename);
}