#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaImageAlgorithms.h"

using namespace std;


LaImageAlgorithms::LaImageAlgorithms()
{

}

LaImageAlgorithms::~LaImageAlgorithms() {}


void LaImageAlgorithms::SetInputData(LaImage* shell) {}

void LaImageAlgorithms::SetInputData2(LaImage* shell) {}


LaImage* LaImageAlgorithms::GetOutput() {
	return new LaImage();
}


void LaImageAlgorithms::Update() {}