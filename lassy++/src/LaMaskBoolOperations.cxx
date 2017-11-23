#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaMaskBoolOperations.h"
#include "../include/LaImage.h"

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

	InputImageType::Pointer mask1 = _mask_img1->GetImage(); 
	InputImageType::Pointer mask2 = _mask_img2->GetImage(); 

	_mask_img1->DeepCopy(_output_img);
	InputImageType::Pointer output = _output_img->GetImage();

	IteratorType  mask_img1_it(mask1, mask1->GetRequestedRegion());
	IteratorType  mask_img2_it(mask2, mask2->GetRequestedRegion());
	IteratorType  output_it(output, output->GetRequestedRegion());
	

	mask_img1_it.GoToBegin();
	mask_img2_it.GoToBegin();
	output_it.GoToBegin();

	while (!mask_img1_it.IsAtEnd())
	{
		switch (_which_operation)
		{
			case BOOL_AND: 
				if (mask_img1_it.Get() > 0 && mask_img2_it.Get() == 0)
					output_it.Set(0);

				if (mask_img1_it.Get() > 0)
					output_it.Set(1);			// mask it 

				++mask_img1_it;
				++mask_img2_it;
				++output_it;
			break; 

			case BOOL_OR: 
				if (mask_img1_it.Get() == 0 && mask_img2_it.Get() > 0)
					output_it.Set(1);
				
				if (mask_img1_it.Get() > 0)
					output_it.Set(1);			// mask it 
					
				++mask_img1_it;
				++mask_img2_it;
				++output_it; 
			break; 
		}
	}

	

}