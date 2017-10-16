#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/ShellEntropy.h"
using namespace std;


ShellEntropy::ShellEntropy(vtkSmartPointer<vtkPolyData> mesh) {  
	if( mesh->GetPointData()->GetNumberOfArrays() <= 0) 
	{
		cout << "Error creating ShellEntropy class object, no point data in mesh! " << endl; 
		exit(1);
	}
	_mesh_3d->DeepCopy(mesh);
}

bool ShellEntropy::InsertPointIntoVisitedList(vtkIdType id)
{
	for (int i=0;i<_visited_point_list.size();i++)
	{
		if (_visited_point_list[i] == id)
			return false; 
	}
	_visited_point_list.push_back(id);
	return true; 
}


void ShellEntropy::GetNeighboursAroundPoint(int pointID, vector<int>& pointNeighbours, int order)
{
	
	_visited_point_list.clear();			// container that stores neighbours (vertex ids) 
											// during recursive lookup around a mesh vertex
											// 'order' levels deep
										
	RecursivePointNeighbours(pointID, order);

	for (int i=0;i<_visited_point_list.size();i++) { 
		pointNeighbours.push_back(_visited_point_list[i]);

	}
	cout << "This point has (recursive order n = " << order << ") = " << pointNeighbours.size() << " neighbours";
	cout << "\n";

}

int ShellEntropy::RecursivePointNeighbours(vtkIdType pointId, int order)
{
	double s;
	if (order == 0)
		return 0; 
	else
	{
		
		if (!InsertPointIntoVisitedList(pointId)) 
			return 0;			// already visited, no need to look further down this route
		else
		{	
			//vtkIdList* pointList = cell->GetPointIds();
			vtkIdList* pointList = vtkIdList::New();
			// get all neighbouring points of this point 
			GetConnectedVertices(_mesh_3d, pointId, pointList);
			
			// running through each neighbouring point 
			for(vtkIdType e = 0; e < pointList->GetNumberOfIds(); e++) 
			{
				RecursivePointNeighbours(pointList->GetId(e), order - 1);
			}
			return 1;		// keep recursing .. 0 will stop recursing .. returning just a dummy value 
			
		}
	}
}


void ShellEntropy::GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int seed, vtkSmartPointer<vtkIdList> connectedVertices) 
{ 

  //get all cells that vertex 'seed' is a part of 
  vtkSmartPointer<vtkIdList> cellIdList = vtkSmartPointer<vtkIdList>::New(); 
  mesh->GetPointCells(seed, cellIdList); 

  //cout << "There are " << cellIdList->GetNumberOfIds() << " cells that use point " << seed << endl; 

  //loop through all the cells that use the seed point 
	for(vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++) 
	{ 

		vtkCell* cell = mesh->GetCell(cellIdList->GetId(i)); 
		//cout << "The cell has " << cell->GetNumberOfEdges() << " edges." << endl; 

		//if the cell doesn't have any edges, it is a line 
		if(cell->GetNumberOfEdges() <= 0) 
		{ 
			continue; 
		} 

		// Going through the edges of each cell, remember that an edge is 
		// made up of two vertices 
		for(vtkIdType e = 0; e < cell->GetNumberOfEdges(); e++) 
		{ 
			vtkCell* edge = cell->GetEdge(e); 

			vtkIdList* pointIdList = edge->GetPointIds(); 
			//cout << "This cell uses " << pointIdList->GetNumberOfIds() << " points" << endl; 

			if(pointIdList->GetId(0) == seed || pointIdList->GetId(1) == seed) 
			{ 
				if(pointIdList->GetId(0) == seed) 
				 { 
				  connectedVertices->InsertNextId(pointIdList->GetId(1)); 
				 } 
				else 
				  { 
				  connectedVertices->InsertNextId(pointIdList->GetId(0)); 
				  } 
			} 
		} 


	} 


   // cout << "There are " << connectedVertices->GetNumberOfIds() << " points connected to point " << seed << endl; 
} 


