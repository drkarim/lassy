/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaImageInPaintMask in paints a mask into a greyscale image. It is assumed that mask was created from a cropped 
*	version of the greyscale image 
*   Cropping parameters are (x,y,z,l,w,h)
*/
#pragma once


#include "LaImageAlgorithms.h"
#include "LaImage.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"


class LaImageInPaintMask : public LaImageAlgorithms {

	LaImage* _input_greyscale;
    LaImage* _input_binary; 
	int _crop_x; 
    int _crop_y; 
	int _crop_z; 
    int _crop_l; 
    int _crop_w; 
    int _crop_h;
    const char* _output_fn;

public:
		
	void SetInputData(LaImage* greyscale);
	void SetInputData2(LaImage* binary);
    void SetOutputFileName(const char* output_fn);

    void SetCropOrigin(int x, int y, int z);
    void SetCropSize(int l, int w, int h);

	void Update();


	LaImageInPaintMask();
	~LaImageInPaintMask();
	
}; 