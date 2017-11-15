#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaImageNormalInterrogator.h"

using namespace std;


LaImageNormalInterrogator::LaImageNormalInterrogator()
{
	
	_direction[0] = -1; 
	_direction[1] = 1; 

	_steps = 4; 

	_doLogging = false; 

	_aggregate_scalar = 0;

	_aggregation_method = MAX;

	_zscore_mean = 0; 
	_zscore_std = 1; 

}

LaImageNormalInterrogator::~LaImageNormalInterrogator() {}


void LaImageNormalInterrogator::SetInputData(LaImage* img) {
	_image = img; 
}

void LaImageNormalInterrogator::SetInputData2(LaImage* img) {
	_mask_image = img;
}


void LaImageNormalInterrogator::SetDirectionVector(double* dir_vec)
{
	_dirvec_line[0] = dir_vec[0];
	_dirvec_line[1] = dir_vec[1];
	_dirvec_line[2] = dir_vec[2];
}

void LaImageNormalInterrogator::SetLineOrigin(double* origin)
{
	_origin_line[0] = origin[0];
	_origin_line[1] = origin[1];
	_origin_line[2] = origin[2];
}

void LaImageNormalInterrogator::SetInterrogationDirections(int* direction)
{
	_direction[0] = direction[0];
	_direction[1] = direction[1]; 
}

void LaImageNormalInterrogator::SetStepSize(double size)
{
	_steps = size;
}

void LaImageNormalInterrogator::SetLoggingToTrue()
{
	_doLogging = true; 
}

void LaImageNormalInterrogator::SetZScoreMean(double mean)
{
	_zscore_mean = mean; 
}

void LaImageNormalInterrogator::SetZScoreStd(double std)
{
	_zscore_std = std;
}


void LaImageNormalInterrogator::ZScoreAggregator()
{
	_aggregate_scalar = (_aggregate_scalar - _zscore_mean) / _zscore_std;
	
}

double LaImageNormalInterrogator::GetIntensity()
{
	ZScoreAggregator();
	return _aggregate_scalar; 
}

void LaImageNormalInterrogator::SetAggregationMethodToMean()
{
	_aggregation_method = MEAN;
}

void LaImageNormalInterrogator::SetAggregationMethodToMedian()
{
	_aggregation_method = MEDIAN;
}

void LaImageNormalInterrogator::SetAggregationMethodToMax()
{
	_aggregation_method = MAX;
}

void LaImageNormalInterrogator::SetAggregationMethodToIntegral()
{
	_aggregation_method = INTEGRAL;
}


void LaImageNormalInterrogator::Update() {

	bool isExplore = true;	// by default look around a normal, except if there is a mask image involved

	int size = 0, j = 0, indexOfPointInArray = -1, currIndex = 0, a, b, c;
	double insty = 0, x = 0, y = 0, z = 0;
	typedef itk::Image< unsigned short, 3 >  ImageType;
	ImageType::PointType point;

	double scar_step_min, scar_step_max, scar_step_size = 1;
	
	scar_step_min = _direction[0] * _steps;
	scar_step_max = _direction[1] * _steps;


	vector<Point3> pointsOnAndAroundNormal;

	std::ofstream ofs;
	if (_doLogging) {
		ofs.open("intensity_log.csv", std::ofstream::out | std::ofstream::app);

		//ofs << "NormalStep,CentrePixel_X,CentrePixelY,CentrePixelZ,PixelVaue" << endl;
		//ofs << "==========,=============,===========,=============,=========" << endl;
	}
	int MaxX, MaxY, MaxZ;
	_image->GetImageSize(MaxX, MaxY, MaxZ);

	// normalize
	MathBox::normalizeVector(_dirvec_line[0], _dirvec_line[1], _dirvec_line[2]);


	for (double i = scar_step_min; i <= scar_step_max; i += scar_step_size)
	{
		x = _origin_line[0] + (i*_dirvec_line[0]);
		y = _origin_line[1] + (i*_dirvec_line[1]);
		z = _origin_line[2] + (i*_dirvec_line[2]);
		x = floor(x); y = floor(y); z = floor(z);

		// by default look around a normal, except if there is a mask image involved
		// Explore only inside mask 
		if (_mask_image != NULL)
		{
			short maskValue;
			isExplore = false;

			_mask_image->GetIntensityAt(x, y, z, maskValue);
			if (maskValue > 0)
			{
				isExplore = true;
			}

		}

		if (_aggregation_method != INTEGRAL)
		{
			for (a = -1; a <= 1; a++) {
				for (b = -1; b <= 1; b++) {
					for (c = -1; c <= 1; c++) {
						if (x + a >= 0 && x + a<MaxX && y + b >= 0 && y + b<MaxY && z + c >= 0 && z + c<MaxZ) {
							if (isExplore) {
								pointsOnAndAroundNormal.push_back(Point3(x + a, y + b, z + c));

							}
						}
					}
				}
			}
		}
		else 
		{	
			// In integral, only consider points exactly on the normal
			pointsOnAndAroundNormal.push_back(Point3(x, y, z));
		}

		// Some intensity logging to CSV file 
		if (isExplore && _doLogging) {
			short pixelValue = -1;
			_image->GetIntensityAt(x, y, z, pixelValue);

			ofs << i << "," << x << "," << y << "," << z << "," << pixelValue << endl;
		}

	}		// end for 
	
	

	if (pointsOnAndAroundNormal.size() > 0) {
		GetStatisticalMeasure(pointsOnAndAroundNormal, _aggregation_method, insty);			// statistical measure 2 returns max 
		if (_doLogging) {
			ofs << ",,,aggregate=,"<<insty<<endl;
		}
	}
	else {
		insty = 0;
	}
	pointsOnAndAroundNormal.clear();

	_aggregate_scalar = insty;

	if (_doLogging) {
		ofs.close();
	}
}



void LaImageNormalInterrogator::GetStatisticalMeasure(vector<Point3> vals, int measure, double& returnVal)
{
	double sum = 0, max = -1;
	//double visitedStatus;
	int size = vals.size();
	short pixelValue;

	if (measure == MEAN)			// reutrn mean 
	{
		for (int i = 0; i<size; i++)
		{
			_image->GetIntensityAt(vals[i]._x, vals[i]._y, vals[i]._z, pixelValue);
			sum += pixelValue;
		}
		returnVal = sum / size;
	}
	else if (measure == MAX)			// return max 
	{

		for (int i = 0; i < size; i++)
		{
			_image->GetIntensityAt(vals[i]._x, vals[i]._y, vals[i]._z, pixelValue);
			if (pixelValue > max) {
				max = pixelValue;
			}
		}
		
		if (max == -1)
			returnVal = 0;
		else {
			returnVal = max;
		}
		/*
		for (int i = 0; i<size; i++)
		{
		_image->GetIntensityAt(vals[i]._x, vals[i]._x, vals[i]._z, pixelValue);

		// **************** VISITED NOT IMPLEMENTED YET ****************
		//visitedStatus = visited_im->Get(vals[i]._x, vals[i]._y, vals[i]._z);
		visitedStatus = 0;

		if (pixelValue > max && visitedStatus < 1) {
		max = pixelValue;
		maxIndex = i;
		}
		}
		if (max == -1)
		returnVal = 0;
		else {
		returnVal = max;

		// now change the visited status of this max pixel
		visited_im->Put(vals[maxIndex]->_x, vals[maxIndex]->_y, vals[maxIndex]->_z, 1);
		}*/
	}
	else if (measure == INTEGRAL)			// sum along the normal (integration)
	{
		for (int i = 0; i<size; i++)
		{
			_image->GetIntensityAt(vals[i]._x, vals[i]._y, vals[i]._z, pixelValue);
			sum += pixelValue;
		}
		returnVal = sum;
	}


}