/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	Interrogates a 3D image along a straigth line. The straight line is defined by the vector <n_x, n_y, n_z>, positioned at (centre_x, centre_y, centre_z)
*	Note that the interrogation is by default 4 pixels in both directions of the vector
*	There is an option to interrogate the image only in the region defined by the mask_img parameter
*/
#pragma once

#define MEAN 1 
#define MEDIAN 2 
#define MAX 3
#define INTEGRAL 3

#include "LaImage.h"
#include "LaImageAlgorithms.h"



class LaImageNormalInterrogator : public LaImageAlgorithms {

	double* _dirvec_line; 
	double* _origin_line;	
	LaImage* _image; 
	LaImage* _mask_image;
	double _aggregate_scalar; 
	double _steps; 
	
	int* _direction;

	bool _doLogging; 

	int _aggregation_method; 

	double _zscore_mean; 
	double _zscore_std;

	void GetStatisticalMeasure(vector<Point3> vals, int measure, double& returnVal);
	void ZScoreAggregator();
	
public:
	// Constructor with default values for data members
	/*static LaShellAlgorithms *New();*/
	
	void SetInputData(LaImage* image); 
	void SetInputData2(LaImage* mask_img); 
	
	void Update(); 

	void SetLineOrigin(double*); 
	void SetDirectionVector(double*);
	void SetInterrogationDirections(int*);
	void SetStepSize(double);
	
	void SetZScoreMean(double); 
	void SetZScoreStd(double);

	void SetRecordLogs();

	double GetIntensity();


	void SetAggregationMethodToMax(); 
	void SetAggregationMethodToMean(); 
	void SetAggregationMethodToMedian();
	void SetAggregationMethodToIntegral();

	LaImageNormalInterrogator();
	~LaImageNormalInterrogator();
	
}; 