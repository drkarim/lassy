#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaMaskBoolOperations.h"

using namespace std;


LaMaskBoolOperations::LaMaskBoolOperations()
{
	_mask_img1 = new LaImage(); 
	_mask_img2 = new LaImage(); 
	_output_img = new LaImage();
	_which_operation = BOOL_AND;
}

LaMaskBoolOperations::~LaMaskBoolOperations() {}


void LaMaskBoolOperations::SetInputData(LaImage* img) {

	_mask_img1 = img; 
}

void LaMaskBoolOperations::SetInputData2(LaImage* img) {

	_mask_img2 = img; 
}


LaImage* LaMaskBoolOperations::GetOutput() {
	return _output_img;
}

void LaMaskBoolOperations::SetBooleanOperationToAND() 
{
	_which_operation = BOOL_AND; 
}

void LaMaskBoolOperations::SetBooleanOperationToOR() 
{
	_which_operation = BOOL_OR; 
}

void LaMaskBoolOperations::SetBooleanOperationToXOR() 
{
	_which_operation = BOOL_XOR; 
}



void LaMaskBoolOperations::Update() {

	typedef itk::Image< unsigned short, 3 >    InputImageType;
	typedef itk::ImageRegionIterator<InputImageType>  IteratorType;

	InputImageType::Pointer _mask1 = _mask_img1->GetImage(); 
	InputImageType::Pointer _mask2 = _mask_img2->GetImage(); 

	IteratorType  mask_img1_it(_mask1, _mask1->GetRequestedRegion());
	IteratorType  mask_img2_it(_mask2, _mask2->GetRequestedRegion());
	
	mask_img1_it.GoToBegin();
	mask_img2_it.GoToBegin();

	while (!mask_img1_it.IsAtEnd())
	{
		switch (_which_operation)
		{
			case BOOL_AND: 
				if (mask_img1_it.Get() == 0 && mask_img2_it.Get() > 0)
					mask_img1_it.Set(0); 
				else if (mask_img1_it.Get() > 0 && mask_img2_it.Get() == 0)
					mask_img1_it.Set(0); 

				
				++mask_img1_it;
				++mask_img2_it;
			break; 

			case BOOL_OR: 
				if (mask_img1_it.Get() == 0 && mask_img2_it.Get() > 0)
					mask_img1_it.Set(1);
				
				++mask_img1_it;
				++mask_img2_it;
			break; 
		}
	}

	_output_img = _mask_img1;	

}