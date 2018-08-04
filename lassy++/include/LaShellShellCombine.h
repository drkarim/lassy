/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellShellCombine combines scalars from two topologically equivalent VTK meshes
*   If at location xyz, new scalar = scalar1 OR scalar2, the value in scalar1/2 is copied to new_scalar 
*	When there is an overlap (i.e. both scalar1 and scalar2 present at xyz), the user can define which scalar to choose
*
*/
#pragma once

#define WHEN_OVERLAP_USE_SOURCE1 1 
#define WHEN_OVERLAP_USE_SOURCE2 2 


#include "LaShellAlgorithms.h"
#include "LaShell.h"


class LaShellShellCombine : public LaShellAlgorithms {

	LaShell* _source_la_1;
	LaShell* _source_la_2;
	LaShell* _output_la;
    const char* _scalar_array_name;
	string _output_scalar_name;
    int _scalar_array_location_in_source1; 
    int _scalar_array_location_in_source2;
	int _overlap_preference; 
	bool _write_to_field_data;
	

public:
		
	void SetInputData(LaShell* shell);
	void SetInputData2(LaShell* shell);


	void SetOverlapToSource1();
	void SetOverlapToSource2();
    bool SetScalarArrayNames(const char* array_name1, const char* array_name2);
	void SetWriteDataToField();
	void SetOutputScalarName(string array_name);
	

	void Update();

	LaShell* GetOutput();


	LaShellShellCombine();
	~LaShellShellCombine();

private: 
    int FindScalarArray(LaShell* shell, const char* array_name);
}; 