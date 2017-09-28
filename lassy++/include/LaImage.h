#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <itkImageIterator.h>
#include "itkRescaleIntensityImageFilter.h"
#include <itkSmartPointer.h>
#include "itkVTKImageIO.h"
#include "itkImage.h"


// Lassy++ includes 
#include "MathBox.h"


#include <string>

/*
*	Templates can be complex and here are some guides: 
*	https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl
*	Stack: https://stackoverflow.com/questions/115703/storing-c-template-function-definitions-in-a-cpp-file
*
*	In order to avoid linker errors, I have physically moved the definition of the template functions into this .h file, 
*	even if it is not an inline function. This solution may (or may not!) cause significant code bloat, 
*	meaning the executable size may increase dramatically (or, if your compiler is smart enough, may not; try it and see)
*
*/

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
	void ConvertToVTKImage(const char* vtk_fn); 

	bool GetIntensityAt(int x, int  y, int z, short& pixelValue);
	bool GetImageSize(int& x, int& y, int& z);
	
	void WorldToImage(double &x, double &y, double &z); 
	void ImageToWorld(float &x, float &y, float &z);

	void InterrogateImage(double n_x, double n_y, double n_z, double centre_x, double centre_y, double centre_z, double& returnVal);
	void GetStatisticalMeasure(vector<Point3> vals, int measure, double& returnVal);
};

