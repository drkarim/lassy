#define HAS_VTK 1


#include "LaShell2ShellPointsCSV.h"


/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*
*	   This application demonstrates the LaShell2ShellPointsCSV class 
*      Reads a CSV file containing 3D points, locates them in source shell and then locates their closest points in target shell
*      Outputs a CSV file containing cloest points xyz in target shell
*/
int main(int argc, char * argv[])
{
	char* input_f1, *input_f2, *input_f3, *input_csv,  *output_csv;
    
	bool foundArgs1 = false;
    bool foundArgs2 = false;
    bool foundArgs3 = false;
    bool foundArgs4 = false;
    bool foundArgs5 = false;

	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-source") {
					input_f1 = argv[i + 1];
					foundArgs1 = true;
				} 
                else if (string(argv[i]) == "-target") {
					input_f2 = argv[i + 1];
					foundArgs2 = true;
				}
                else if (string(argv[i]) == "-starget") {
					input_f3 = argv[i + 1];
					foundArgs3 = true;
				}

				else if (string(argv[i]) == "-csv") {
					input_csv = argv[i + 1];
                    foundArgs4 = true; 
				}
                else if (string(argv[i]) == "-out") {
					output_csv = argv[i + 1];
                    foundArgs5 = true; 
				}

                
			} // end outer if 
			

		} 
		
	}

	if (!(foundArgs1 && foundArgs2 && foundArgs3 && foundArgs4 && foundArgs5))
	{
		cerr << "Cheeck your parameters\n\nUsage:"
			"\nReads a CSV file containing 3D points, locates them in source shell and then locates their closest points in target shell."
            "\nOutputs a CSV file containing cloest points xyz in target shell"
			"\n(Mandatory)\n\t-source <source shell1> \n\t-target <target shell2> \n\t-starget (source registered to target vtk)"
            "\n\t-csv <csv file>\n\t-out <csv output>\n" << endl;
			
		exit(1);
	}
	else
	{
		LaShell* source = new LaShell(input_f1);
        LaShell* target = new LaShell(input_f2);
        LaShell* source_in_target = new LaShell(input_f3);
        
		LaShell2ShellPointsCSV* algorithm = new LaShell2ShellPointsCSV();
		
        algorithm->SetSourceData(source);
        algorithm->SetTargetData(target);
        algorithm->SetSourceInTargetData(source_in_target);

        algorithm->SetOutputFileName(output_csv);
        algorithm->ReadCSVFile(input_csv);
        
       
		algorithm->Update();
       

      
       
	}

}