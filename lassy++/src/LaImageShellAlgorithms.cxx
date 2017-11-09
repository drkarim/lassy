#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaImageShellAlgorithms.h"

using namespace std;


LaImageShellAlgorithms::LaImageShellAlgorithms()
{

}

LaImageShellAlgorithms::~LaImageShellAlgorithms() {}


void LaImageShellAlgorithms::SetInputDataShell(LaShell* shell) {}

void LaImageShellAlgorithms::SetInputDataImage(LaImage* shell) {}


LaShell* LaImageShellAlgorithms::GetOutput() {
	return new LaShell();
}


void LaImageShellAlgorithms::Update() {}