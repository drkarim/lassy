/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellContour draws the LA shell contour on the LA greyscale image
*	
*
*/
#pragma once


#include "LaImageAlgorithms.h"
#include "LaShell.h"
#include "LaImage.h"
#include "itkBinaryContourImageFilter.h"
#include "itkImageFileWriter.h"


class LaImageContour : public LaImageAlgorithms {

	LaImage* _input_greyscale;
    LaImage* _input_binary; 
	
	 

public:
		
	void SetInputData(LaImage* greyscale);
	void SetInputData2(LaImage* binary);


	void Update();

	LaImage* GetOutput();


	LaImageContour();
	~LaImageContour();
	
}; 