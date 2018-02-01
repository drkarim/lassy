/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellGapsInBinary finds gaps within a user-defined path of a binarised data in shell 
*   The path is constructed from user-defined points and computing the Djikstra's shortest path between 
*   these adjacent points 
*
*   Note: It uses the built-in VTK renderer for displaying the mesh and taking user input
*/ 
#pragma once


#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkLookupTable.h>
#include <vtkCellLocator.h>
#include "vtkPolyData.h"
#include <vtkCellArray.h>
#include "vtkGenericCell.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkFieldData.h"
#include <vtkCellData.h>
#include <vtkPointLocator.h>
#include <vtkMath.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataReader.h>
#include <vtkPlanes.h>
#include <vtkPointData.h>
#include <vtkPolyDataWriter.h>
#include <vtkDijkstraGraphGeodesicPath.h>
#include <vtkTriangle.h>
#include <vtkCellDataToPointData.h>
#include <vtkSelectPolyData.h>
#include <vtkCamera.h>
#include <vtkImageActor.h>
#include <vtkStructuredPoints.h>
#include <vtkTubeFilter.h>
#include <vtkPolyLine.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLineSource.h>
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <string>
#include <sstream>

#include "LaShellAlgorithms.h"
#include "LaShell.h"


class LaShellGapsInBinary : public LaShellAlgorithms {

protected: 

    // Non-static variables 
	LaShell* _source_la;
	LaShell* _target_la;
    LaShell* _output_la;

public:  
     int _neighbourhood_size;
    double _fill_threshold;
    vector<int> _GlobalPointContainer;
    vector<int> _visited_point_list;		// stores the neighbours around a point
    

    // static variables 
	vector<vtkActor*> _actors;				// actors representing shortest path betwee points 
    vector<vtkPolyData*> _paths;
    vtkSmartPointer<vtkCellPicker> _cell_picker;
    vtkSmartPointer<vtkPolyData> _SourcePolyData; 
    vtkSmartPointer<vtkRenderWindow> _RenderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> _InteractorRenderWindow;

    vector<vtkDijkstraGraphGeodesicPath*> _shortestPaths; 
    vector<int> _cellidarray; 
    vector<vtkPolyDataMapper*> _pathMappers;			// container to store shortest paths between points selected by user

    // Functions
    void GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int seed, vtkSmartPointer<vtkIdList> connectedVertices);
    int RecursivePointNeighbours(vtkIdType pointId, int order);
    bool InsertPointIntoVisitedList(vtkIdType id);
    void RetainPointsInGlobalContainer(vector<int> p);

    bool IsThisNeighbourhoodCompletelyFilled(vector<int>);
    
    
    void GetNeighboursAroundPoint(int pointID, vector<int>& pointNeighbours, int order);
    void StatsInNeighbourhood(vector<int> points, double& mean, double& variance);
	void SetInputData(LaShell* shell);
    void SetNeighbourhoodSize(int s);
    void SetFillThreshold(double s);
    
    
    // Static functions
    static void KeyPressEventHandler(vtkObject* obj, unsigned long,void *sr, void *v);    
    static vtkIdType GetFirstCellVertex(vtkPolyData* poly, vtkIdType cellID, double* point_xyz);
    static void CreateSphere(vtkRenderer* renderer, double radius, double* position3D);;
    double ComputePercentageEncirclement(vector<vtkDijkstraGraphGeodesicPath*> allShortestPaths);
    
    vtkSmartPointer<vtkPolyData> GetSourcePolyData();
    vtkSmartPointer<vtkRenderWindowInteractor> GetWindowInteractor();
    vtkSmartPointer<vtkCellPicker> GetCellPicker();

    //void Update();
    
    void Run();


	LaShellGapsInBinary();
	~LaShellGapsInBinary();
	
}; 