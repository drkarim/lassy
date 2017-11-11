#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellAlgorithms.h"

using namespace std;


LaShellAlgorithms::LaShellAlgorithms()
{

}

LaShellAlgorithms::~LaShellAlgorithms() {}


void LaShellAlgorithms::SetInputData(LaShell* shell) {}

void LaShellAlgorithms::SetInputData2(LaShell* shell) {}


LaShell* LaShellAlgorithms::GetOutput() {
	return new LaShell();
}


void LaShellAlgorithms::Update() {}
