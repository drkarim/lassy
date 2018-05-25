#define HAS_VTK 1


#include "LaShellStatistics.h"
#include <numeric> 
#include <sstream>

/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application demonstrates the LaShellStatistics class that calculates simple statistics on the 
*      the shell's scalar values 
*/
int main(int argc, char * argv[])
{
	char* input_f1, *output_f, *input_f2;

	bool foundArgs1 = false,foundArgs2 = false;

	int method = DO_STANDARD_STATS;

	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-i") {
					input_f1 = argv[i + 1];
					foundArgs1 = true;
				}

				else if (string(argv[i]) == "-m") {
					method = atoi(argv[i + 1]);

				}

				else if (string(argv[i]) == "-o") {
					output_f = argv[i + 1];
					foundArgs2 = true;

				}
				
			}

		}
	}

	if (!(foundArgs1))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nCalculates ordinary statistics on the vertex values of the shell "
			
			"\n(Mandatory)\n\t-i <source_mesh_vtk> \n\t== Optional ==\n\t-m (1=Median, Mean, Std, 2=Not implemented)" << endl;
			

		exit(1);
	}
	else
	{
		LaShell* source = new LaShell(input_f1);
		
		LaShellStatistics* algorithm = new LaShellStatistics();
		algorithm->SetInputData(source);
		

		switch (method)
		{
			case DO_STANDARD_STATS:
				algorithm->SetOperationToStandardStats();
                cout << "\nOrdinary statistics:" << endl; 
				break;

			
		}

		algorithm->Update();
        
		if (!foundArgs2) {
			cout << "\n\tMedian=\t\t\t" << fixed << setprecision(2) << algorithm->GetOutputValue("median") << "" 
			"\tMean=\t\t\t" << fixed <<  setprecision(2) << algorithm->GetOutputValue("mean") << ""
			"\n\tStandard Dev.=\t\t" << fixed << setprecision(2) << algorithm->GetOutputValue("std") << endl; 
		}
		else {
			ofstream out; 
			out.open(output_f); 
			out << fixed << setprecision(2) << algorithm->GetOutputValue("median") << "," << fixed <<  setprecision(2) << algorithm->GetOutputValue("mean") << "," << fixed << setprecision(2) << algorithm->GetOutputValue("std") << endl;
			out.close();
		}
	}

}