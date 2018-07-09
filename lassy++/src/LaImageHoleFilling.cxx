#define HAS_VTK 1

#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaImageHoleFilling.h"


using namespace std;


LaImageHoleFilling::LaImageHoleFilling()
{
    _min_neighbours = 0;
    _iter = 1;
    _interpolate_direction = XYZ;
}
  

LaImageHoleFilling::~LaImageHoleFilling()
{
    // delete
}

void LaImageHoleFilling::SetInputData(LaImageFloat* im)
{
	_input_img = im;
}

void LaImageHoleFilling::SetOutputFileName(const char* output_fn)
{
    _output_fn = output_fn;
}

void LaImageHoleFilling::SetMinNeighboursForMean(int size)
{
    _min_neighbours = size;
}

void LaImageHoleFilling::SetNumberOfIterations(int iter)
{
    _iter = iter;
}

void LaImageHoleFilling::SetInterpolateDirection(int dir)
{
    _interpolate_direction = dir;
}

void LaImageHoleFilling::Update()
{
	int max_x, max_y, max_z, max_x2, max_y2, max_z2,all_neighbours; 
	typedef itk::Image<  float, 3 >  ImageType;
    typedef itk::Image< float, 3 >  OutputImageType;

	ImageType::IndexType pixelIndex;

    ImageType::Pointer input_im = _input_img->GetImage();
    _input_img->GetImageSize(max_x, max_y, max_z);
    cout << max_x << "," << max_y << "," << max_z << endl;

    OutputImageType::SpacingType spacing_out;
    OutputImageType::SizeType size;
    OutputImageType::RegionType region;
    OutputImageType::IndexType start;

    ImageType::RegionType in_region;

    using InputImageIterator = itk::ImageRegionIterator<ImageType>;
    using OutputImageIterator = itk::ImageRegionIterator<OutputImageType>;
   

    start[0] = 0;
    start[1] = 0;
    start[2] = 0; 
    size[0] = max_x; 
    size[1] = max_y; 
    size[2] = max_z;

    // spacing 
    ImageType::SpacingType spacing_in;
    spacing_in = input_im->GetSpacing();
    spacing_out[0] = spacing_in[0]; 
    spacing_out[1] = spacing_in[1];
    spacing_out[2] = spacing_in[2];
    
    region.SetSize(size);
    region.SetIndex(start);

    in_region.SetSize(size); 
    in_region.SetIndex(start);

    OutputImageType::Pointer output_im = OutputImageType::New();
    output_im->SetRegions(region);
    output_im->SetSpacing(spacing_out);
    output_im->Allocate();
    
    OutputImageIterator out_it(output_im,region);
    InputImageIterator in_it(input_im,in_region);

    // copy output to input
    
    for (in_it.GoToBegin(), out_it.GoToBegin(); ! in_it.IsAtEnd(); ++in_it, ++out_it )
    {
       out_it.Set(in_it.Get());
    }

    ImageType::SizeType radius;

    switch (_interpolate_direction)
    {
        case XY: 
            radius[0] = 1;
            radius[1] = 1;
            radius[2] = 0;
            all_neighbours = 9;
            break; 
        case YZ: 
            radius[0] = 0;
            radius[1] = 1;
            radius[2] = 1;
            all_neighbours = 9;
            break; 
        case XZ:
            radius[0] = 1;
            radius[1] = 0;
            radius[2] = 1;
            all_neighbours = 9;
            break; 
        default: 
            radius[0] = 1;
            radius[1] = 1;
            radius[2] = 1;
            all_neighbours = 27;
            break;
    }
    
    input_im = _input_img->GetImage();
    itk::NeighborhoodIterator<ImageType> neighbour_it(radius, input_im,in_region);
        
    for (int iter=0;iter<_iter;iter++) {
        
        out_it.GoToBegin();
        while(!neighbour_it.IsAtEnd())
        {
            
            if (neighbour_it.GetCenterPixel() == 0)     // only deal with holes in the image
            {
                int neighbours = 0;
                float accum = 0;

                // all center pixel's neighbours 
                for (unsigned int i = 0; i < all_neighbours; i++)
                {
                    ImageType::IndexType index = neighbour_it.GetIndex(i);
                    //std::cout << index[0] << " " << index[1] << std::endl;
                
                    bool IsInBounds;
                    float neighbour = neighbour_it.GetPixel(i, IsInBounds);
                    if(IsInBounds)
                        {
                            if (neighbour > 0)
                            {
                                accum += neighbour;
                                neighbours++;
                            }
                        }
                    

                }
                if (neighbours > _min_neighbours)
                    out_it.Set(accum/neighbours);
            }
            ++neighbour_it;
            ++out_it;
        }

        // copy output to input 
        for (in_it.GoToBegin(), out_it.GoToBegin(); ! in_it.IsAtEnd(); ++in_it, ++out_it )
        {
            in_it.Set(out_it.Get());
        }
        neighbour_it.GoToBegin();
    }
    

    typedef  itk::ImageFileWriter< OutputImageType  > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(_output_fn);
    writer->SetInput(output_im);
    writer->Update();
}



