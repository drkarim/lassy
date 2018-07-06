#define HAS_VTK 1

#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaImageInPaintMask.h"

using namespace std;


LaImageInPaintMask::LaImageInPaintMask()
{
   _crop_l = -1; 
	_crop_w = -1; 
	_crop_h = -1;
}

LaImageInPaintMask::~LaImageInPaintMask()
{
    // delete
}

void LaImageInPaintMask::SetInputData(LaImage* greyscale)
{
	_input_greyscale = greyscale;
}

void LaImageInPaintMask::SetInputData2(LaImage* binary)
{
	_input_binary = binary;
}

void LaImageInPaintMask::SetOutputFileName(const char* output_fn)
{
    _output_fn = output_fn;
}

void LaImageInPaintMask::SetCropOrigin(int x, int y, int z) {
	_crop_x = x; 
	_crop_y = y; 
	_crop_z = z; 
}

void LaImageInPaintMask::SetCropSize(int l, int w, int h) {
	_crop_l = l; 
	_crop_w = w; 
	_crop_h = h;
}

void LaImageInPaintMask::Update()
{
	int max_x, max_y, max_z, max_x2, max_y2, max_z2; 
	typedef itk::Image< unsigned short, 3 >  ImageType;
    typedef itk::Image<unsigned short, 3 >  OutputImageType;
	
	ImageType::IndexType pixelIndex;
    ImageType::IndexType pixelIndex2;

    _input_greyscale->GetImageSize(max_x, max_y, max_z);
	_input_binary->GetImageSize(max_x2, max_y2, max_z2);

    ImageType::Pointer greyscale_pn = _input_greyscale->GetImage();
    ImageType::Pointer mask_pn = _input_binary->GetImage();

    OutputImageType::SpacingType spacing_out;
    OutputImageType::SizeType size;
    OutputImageType::RegionType region;
    OutputImageType::IndexType start;

    start[0] = 0;
    start[1] = 0;
    start[2] = 0; 
    size[0] = max_x; 
    size[1] = max_y; 
    size[2] = max_z;

    // spacing 
    ImageType::SpacingType spacing_in;
    spacing_in = greyscale_pn->GetSpacing();
    spacing_out[0] = spacing_in[0]; 
    spacing_out[1] = spacing_in[1];
    spacing_out[2] = spacing_in[2];
    
    region.SetSize(size);
    region.SetIndex(start);
    OutputImageType::Pointer output_im = OutputImageType::New();
    output_im->SetRegions(region);
    output_im->SetSpacing(spacing_out);
    output_im->Allocate();
    
    if (_crop_l < 0)
    {
        _crop_l = max_x2; 
        _crop_w = max_y2; 
        _crop_h = max_z2;
    }

    itk::ImageRegionIterator<OutputImageType> imageIterator(output_im,region);
 
    while(!imageIterator.IsAtEnd())
    {
        imageIterator.Set(0);
        ++imageIterator;
    }


    int start_x = _crop_x;
    int start_y = _crop_y;
    int start_z = _crop_z;

    for (int i=start_x;i<start_x+_crop_l;i++)
    {
        for (int j=start_y;j<start_y+_crop_w;j++)
        {
            for (int k=start_z;k<start_z+_crop_h;k++)
            {
                pixelIndex[0] = i; 
                pixelIndex[1] = j; 
                pixelIndex[2] = k; 

                pixelIndex2[0] = i-start_x; 
                pixelIndex2[1] = j-start_y; 
                pixelIndex2[2] = k-start_z; 

                if (pixelIndex[0] < max_x && pixelIndex[1] < max_y && pixelIndex[2] < max_z && pixelIndex2[0] < max_x2 
                && pixelIndex2[1] < max_y2 && pixelIndex2[2] < max_z2)
                    output_im->SetPixel(pixelIndex, mask_pn->GetPixel(pixelIndex2));
            }
        }
    }
	
	typedef  itk::ImageFileWriter< OutputImageType  > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(_output_fn);
    writer->SetInput(output_im);
    writer->Update();

}



