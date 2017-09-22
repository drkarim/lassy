#define HAS_VTK 1
#include "lashell.h"
#include <numeric> 

int main(int argc, char * argv[])
{
	char* input_f, *output_f, *output_poly_fn, *output_txt_fn;
	bool foundArgs1=false, foundArgs2=false; 
	int method=0; 

	if (argc >= 1)
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) {
				if (string(argv[i]) == "-i") {
					input_f = argv[i + 1];
					foundArgs1 = true;
				}
				else if (string(argv[i]) == "-m")
				{
					method = atoi(argv[i + 1]); 

				}
				else if (string(argv[i]) == "-o") {
					output_f = argv[i + 1];
					foundArgs2 = true;
				}

			}
		}
	}

	if (!(foundArgs1 ))
	{
		cerr << "Cheeck your parameters\n\nUsage: \n\t-i <file_with_filenmes.txt>" << endl;
		exit(1);
	}


	if (method >= 0)
	{
		LAtrium* la = new LAtrium(input_f); 
		vector<double> v = la->GetMeshVertexValues(); 

		float average = accumulate(v.begin(), v.end(), 0.0) / v.size();
		cout << "Mean vertex scalar = " << average << endl; 
		

	}
}
