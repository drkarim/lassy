#define HAS_VTK 1


/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellGapsInBinary.h"

using namespace std;


LaShellGapsInBinary::LaShellGapsInBinary()
{
	_source_la = new LaShell();
	_target_la = new LaShell();
	_output_la = new LaShell();
	_SourcePolyData = vtkSmartPointer<vtkPolyData>::New();

	_neighbourhood_size = 3; 
	_fill_threshold = 0.5;
	_run_count = 0;
	_fileOutName = "encircle_data_r";
}

LaShellGapsInBinary::~LaShellGapsInBinary() {
	delete _source_la; 
	delete _target_la; 
	delete _output_la; 
}

void LaShellGapsInBinary::SetInputData(LaShell* shell) {
	_source_la = shell;
	_source_la->GetMesh3D(_SourcePolyData);
	
}

void LaShellGapsInBinary::SetNeighbourhoodSize(int s)
{
	_neighbourhood_size = s; 
}

void LaShellGapsInBinary::SetFillThreshold(double s)
{
	_fill_threshold = s; 
}

vtkSmartPointer<vtkRenderWindowInteractor> LaShellGapsInBinary::GetWindowInteractor()
{
	return _InteractorRenderWindow;
}

vtkSmartPointer<vtkPolyData> LaShellGapsInBinary::GetSourcePolyData()
{
	return _SourcePolyData;
}


vtkSmartPointer<vtkCellPicker> LaShellGapsInBinary::GetCellPicker()
{
	return _cell_picker;
}



/*
*	Get N-order neighbours of a vertex 
*/
void LaShellGapsInBinary::GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int seed, vtkSmartPointer<vtkIdList> connectedVertices) 
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

void LaShellGapsInBinary::RetainPointsInGlobalContainer(vector<int> p)
{
	bool found = false;
	for (int j=0;j<p.size();j++){
		for (int i=0;i<_GlobalPointContainer.size()&&!found;i++)
		{
			if (p[j] == _GlobalPointContainer[i])
				found = !found;
		}
		if (!found)
			_GlobalPointContainer.push_back(p[j]);
	}
}

/*bool LaShellGapsInBinary::InsertPointIntoVisitedList(vtkIdType id)
{
	for (int i=0;i<_visited_point_list.size();i++)
	{
		if (_visited_point_list[i] == id)
			return false; 
	}
	_visited_point_list.push_back(id);
	return true; 
}*/

bool LaShellGapsInBinary::InsertPointIntoVisitedList2(vtkIdType id, int order)
{
	for (int i=0;i<_visited_point_list.size();i++)
	{
		if (_visited_point_list[i].first == id)
			return false; 
	}
	_visited_point_list.push_back(std::make_pair(id, order));
	return true; 
}

int LaShellGapsInBinary::RecursivePointNeighbours(vtkIdType pointId, int order)
{
	double s;
	if (order == 0)
		return 0; 
	else
	{
		
		if (!InsertPointIntoVisitedList2(pointId, order)) 
			return 0;			// already visited, no need to look further down this route
		else
		{	
			//vtkIdList* pointList = cell->GetPointIds();
			vtkIdList* pointList = vtkIdList::New();
			// get all neighbouring points of this point 
			GetConnectedVertices(_SourcePolyData, pointId, pointList);
			
			// running through each neighbouring point 
			for(vtkIdType e = 0; e < pointList->GetNumberOfIds(); e++) 
			{
				RecursivePointNeighbours(pointList->GetId(e), order - 1);
			}
			return 1;		// keep recursing .. 0 will stop recursing .. returning just a dummy value 
			
		}
	}
}

void LaShellGapsInBinary::GetNeighboursAroundPoint2(int pointID, vector<pair<int, int> >& pointNeighbourAndOrder, int max_order)
{
	_visited_point_list.clear();			// container that stores neighbours (vertex ids) 
											// during recursive lookup around a mesh vertex
											// 'order' levels deep
										
	RecursivePointNeighbours(pointID, max_order);

	for (int i=0;i<_visited_point_list.size();i++) { 
		//pointNeighbours.push_back(_visited_point_list[i]);
		pointNeighbourAndOrder.push_back(std::make_pair(_visited_point_list[i].first,_visited_point_list[i].second));

	}
	cout << "This point has (recursive order n = " << max_order << ") = " << pointNeighbourAndOrder.size() << " neighbours";
	cout << "\n";
}

/*void LaShellGapsInBinary::GetNeighboursAroundPoint(int pointID, vector<int>& pointNeighbours, int order)
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

}*/

bool LaShellGapsInBinary::IsThisNeighbourhoodCompletelyFilled(vector<int> points)
{
	double isFilled = 0;
	vtkFloatArray* scalars = vtkFloatArray::New();
	scalars = vtkFloatArray::SafeDownCast(_SourcePolyData->GetPointData()->GetScalars());  // bring al lthe scalars to an array 
	cout << "Now exploring this point's neighbourhood: " << endl;
	for (int i=0;i<points.size();i++)
	{
		cout << "neighbour point = " << points[i] << ", scar value = " << scalars->GetTuple1(points[i]) << ", threshold satisfy? " << (scalars->GetTuple1(points[i]) > _fill_threshold ? "Yes":"No") << endl;
		if (scalars->GetTuple1(points[i]) > _fill_threshold)
			isFilled++;
	}
	
	double total = points.size();
	double percentage_scar_in_this_neighbourhood =  100*(isFilled/total);
	cout << "% scar in this neighbourhood = " << percentage_scar_in_this_neighbourhood << ", threshold satisfy? " << (percentage_scar_in_this_neighbourhood > _neighbourhood_size ? "Yes":"No") <<"\n\n";

	if (percentage_scar_in_this_neighbourhood > _neighbourhood_size)
		return true;
	else 
		return false;
}


void LaShellGapsInBinary::StatsInNeighbourhood(vector<int> points, double& mean, double& variance) 
{
	vector<double> point_neighbours; 
	double isScar = 0, std;
	vtkFloatArray* scalars = vtkFloatArray::New();
	scalars = vtkFloatArray::SafeDownCast(_SourcePolyData->GetPointData()->GetScalars());  // bring al lthe scalars to an array 
	for (int i=0;i<points.size();i++)
	{		
		point_neighbours.push_back(scalars->GetTuple1(points[i]));
	}

	mean = MathBox::CalcMean(point_neighbours); 
	std = MathBox::CalcStd(point_neighbours, mean); 
	variance = std*std; 
}


void LaShellGapsInBinary::ExtractImageDataAlongTrajectory(vector<vtkSmartPointer<vtkDijkstraGraphGeodesicPath> > allShortestPaths)
{
	double xyz[3];
	bool ret; 
	double mean, variance; 
	typedef map<vtkIdType, int>::iterator it_type; 
	map<vtkIdType, int> vertex_ids; 
	int closestPointID=-1;
	vector<pair<int, int> > pointNeighbours; 
	double pathSegmentHasScar=0;
	int count=0;
	ofstream out; 
	xyz[0]=1e-10; xyz[1]=1e-10; xyz[2]=1e-10;
	
	stringstream ss; 
	ss << _fileOutName << _run_count << ".txt";
	out.open(ss.str().c_str(), std::ios_base::app); 
	out << "MainVertexSeq,VertexID,X,Y,Z,VertexDepth,MeshScalar" << endl;
	// the recursive order - how many levels deep around a point do you want to explore?
	// default is 3 levels deep, meaning neighbours neighbours neighbour.
	int order = _neighbourhood_size;		

	vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
	scalars = vtkFloatArray::SafeDownCast(_SourcePolyData->GetPointData()->GetScalars());  // bring al lthe scalars to an array 
	

	// collect all vertex ids lying in shortest path 
	for (int i=0;i<allShortestPaths.size();i++)
	{
		vtkIdList* vertices_in_shortest_path = vtkIdList::New(); 
		vertices_in_shortest_path = allShortestPaths[i]->GetIdList();		// getting vertex id for each shortest path
		
		for (int j=0;j<vertices_in_shortest_path->GetNumberOfIds();j++)
		{
			 vertex_ids.insert(make_pair(vertices_in_shortest_path->GetId(j),-1));		// map avoids duplicates
																			// only using keys, no associated value always -2
		}
	}	
	
	cout << "There were a total of " << vertex_ids.size() << " vertices in the shortest path you have selected\n" << endl;

	

	for (it_type iterator = vertex_ids.begin(); iterator != vertex_ids.end(); iterator++)
	{
		cout << "Exploring around vertex with id = " << iterator->first << "\n============================\n";
		double scalar = -1;

		if (iterator->first > 0 && iterator->first < _SourcePolyData->GetNumberOfPoints())
		{
			_SourcePolyData->GetPoint(iterator->first, xyz);
			scalar = scalars->GetTuple1(iterator->first);
		}
		out <<  count << "," << iterator->first << "," << xyz[0] << "," << xyz[1] << "," << xyz[2] << "," << 0 << "," << scalar << endl;
		GetNeighboursAroundPoint2(iterator->first, pointNeighbours, order);			// the key is the vertex id
		//RetainPointsInGlobalContainer(pointNeighbours);
		//pointNeighbours.clear();
		
		//StatsInNeighbourhood(pointNeighbours, mean, variance); 
		
		for (int j=0;j<pointNeighbours.size();j++)
		{
			int pointNeighborID = pointNeighbours[j].first; 
			int pointNeighborOrder = pointNeighbours[j].second;
			scalar = -1;
			// simple sanity check 
			if (pointNeighborID > 0 && pointNeighborID < _SourcePolyData->GetNumberOfPoints())
			{	
				 scalar = scalars->GetTuple1(pointNeighborID); 
				 _SourcePolyData->GetPoint(pointNeighborID, xyz);
			}
			out <<  count << "," << pointNeighborID << "," << xyz[0] << "," << xyz[1] << "," << xyz[2] << "," << pointNeighborOrder << "," << scalar << endl;
		}
			
		pointNeighbours.clear();
		count++; 
	}

	
	out.close();
	
}


// A cell has three vertex. Cell is a polygon within a mesh. You input the mesh, the id of the cell/polygon 
// and this function returns the 3D position of one of the cell's vertex (as point_xyz)
// and the point ID (as function return) 
vtkIdType LaShellGapsInBinary::GetFirstCellVertex(vtkPolyData* poly, vtkIdType cellID, double point_xyz[])
{
	vtkIdType vertID;
	vtkCell* cell;
	cell = poly->GetCell(cellID);

	vtkSmartPointer<vtkIdList> cell_vert = vtkSmartPointer<vtkIdList>::New();
	poly->GetCellPoints(cellID, cell_vert);

	vertID = cell_vert->GetId(0);
	poly->GetPoint(vertID, point_xyz);
	return vertID;
}



/*
*	This will handle the event when a user presses the 'x' on the keyboard 
*/
void LaShellGapsInBinary::KeyPressEventHandler(vtkObject* obj, unsigned long eventId,
                                      void* clientdata, 
                                      void* vtkNotUsed(calldata))
{
	/* Remember you are marking your pick position with sphere. First step is to pick and then to place a sphere in the cell you have picked
		The code below picks a cell, and you mark one of the cell's vertex with a sphere
	*/
	double screenX, screenY;		// where in the screen you wil be clicking 
	vtkIdType cellID, pointID=-1;		// to store cellID of the picked cell
	
	LaShellGapsInBinary* this_class_obj = reinterpret_cast<LaShellGapsInBinary*>(clientdata); 
	
	
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkRenderWindowInteractor::SafeDownCast(obj);
	//vtkSmartPointer<vtkRenderWindowInteractor> iren = this_class_obj->GetWindowInteractor();
	//iren = this_class_obj->_InteractorRenderWindow;
	vtkSmartPointer<vtkRenderWindow> renderWin = iren->GetRenderWindow();			// and from there you get your renderer and your renderwindow 
	vtkSmartPointer<vtkRendererCollection> rendererCollection = renderWin->GetRenderers();		// a render collection is a collection of your renderers but you only have one 
	vtkSmartPointer<vtkRenderer> renderer = rendererCollection->GetFirstRenderer();
	
	vtkSmartPointer<vtkPolyData> poly_data = vtkSmartPointer<vtkPolyData>::New();
	poly_data = this_class_obj->GetSourcePolyData(); 

	
	// x is for picking points 
	if (iren->GetKeyCode()=='x')
	{
		
		double *pick_position = new double[3]; 
		

		screenX = iren->GetEventPosition()[0];			// get the x and y co-ordinates on the screen where you have hit 'x'
		screenY = iren->GetEventPosition()[1];

		vtkSmartPointer<vtkCellPicker> cell_picker = this_class_obj->_cell_picker;
		
		cell_picker->Pick(screenX, screenY, 0.0, renderer);
		cellID = cell_picker->GetCellId();
		cell_picker->GetPickPosition(pick_position);
		
		pointID = LaShellGapsInBinary::GetFirstCellVertex(poly_data, cellID, pick_position);

		cout << "Point id picked = " << pointID << " and co-ordinates of its position = " << pick_position[0] << ", " << pick_position[1] << "," << pick_position[2] << ")\n";				
		
		this_class_obj->_pointidarray.push_back(pointID);
		
		
		LaShellGapsInBinary::CreateSphere(renderer, 1.5, pick_position);		// now draw the sphere
		
		iren->Render();
		delete[] pick_position; 
		
	}
	/*
	*	l for line, c for compeleting the circle from picked points
	*/
	else if (iren->GetKeyCode()=='l' || iren->GetKeyCode()=='c') { 

				///////////////////////////////////////////////Dijkstra////////////////////////////////////////////////////////////////
				
				//dijkstra->SetInputConnection(reader->GetOutputPort());
				
				int lim = this_class_obj->_pointidarray.size();
				for(int i=0; i<lim; i++){
					vtkSmartPointer<vtkDijkstraGraphGeodesicPath> dijkstra = vtkSmartPointer<vtkDijkstraGraphGeodesicPath>::New();
					dijkstra->SetInputData(poly_data);
					if(i<lim-1){	
						dijkstra->SetStartVertex(this_class_obj->_pointidarray[i]);
						dijkstra->SetEndVertex(this_class_obj->_pointidarray[i+1]);
						cout << "Computing shortest paths between points " << this_class_obj->_pointidarray[i] << " and  " << this_class_obj->_pointidarray[i+1]  << endl;
					}
					else if(iren->GetKeyCode()=='c' && i == lim-1){
						dijkstra->SetStartVertex(this_class_obj->_pointidarray[i]);
						dijkstra->SetEndVertex(this_class_obj->_pointidarray[0]);
						cout << "Computing shortest paths between points " << this_class_obj->_pointidarray[i] << " and  " << this_class_obj->_pointidarray[0]  << endl;
					}
					
					dijkstra->Update();
					vtkSmartPointer<vtkPolyDataMapper> pathMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
					pathMapper->SetInputConnection(dijkstra->GetOutputPort());
					
					this_class_obj->_shortestPaths.push_back(dijkstra);
					this_class_obj->_pathMappers.push_back(pathMapper);			// store all the shortest paths 
					this_class_obj->_paths.push_back(dijkstra->GetOutput());	// and the path as polygonal data
				}
				
				
				// now draw all the paths 
				for (int i=0;i<this_class_obj->_paths.size();i++){
					vtkSmartPointer<vtkActor> pathActor = vtkSmartPointer<vtkActor>::New();
					pathActor->SetMapper(this_class_obj->_pathMappers[i]);
					pathActor->GetProperty()->SetColor(1,0,0); // Red
					pathActor->GetProperty()->SetLineWidth(4);
		
					///////////////////////////////////////////////Dijkstra///////////////////////////////////////////////////////////////////
					this_class_obj->_actors.push_back(pathActor);
				}

				for (int i=0;i<this_class_obj->_actors.size();i++)
					renderer->AddActor(this_class_obj->_actors[i]);

				renderWin->AddRenderer(renderer); 
				iren->Render();

				// compute percentage encirlcement
				this_class_obj->ExtractImageDataAlongTrajectory(this_class_obj->_shortestPaths);
				
				this_class_obj->_pointidarray.clear();
				this_class_obj->_paths.clear();
				this_class_obj->_shortestPaths.clear();
				this_class_obj->_pathMappers.clear();
				this_class_obj->_run_count++;

	}

	//delete [] pick_position;
	
}


// this wil draw a sphere of a given radus to the renderer
void LaShellGapsInBinary::CreateSphere(vtkSmartPointer<vtkRenderer> renderer, double radius, double position3D[])
{
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New(); 
	sphere->SetThetaResolution(8);
	sphere->SetPhiResolution(8);
	sphere->SetRadius(radius);

	vtkSmartPointer<vtkPolyDataMapper> sphereMapper =vtkSmartPointer<vtkPolyDataMapper>::New(); 
	sphereMapper->ScalarVisibilityOff();
	sphereMapper->SetInputData(sphere->GetOutput());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(sphereMapper); 
	actor->GetProperty()->SetColor(0,0,1); 
	actor->SetPosition(position3D); 
	renderer->AddActor(actor);
	
}

void LaShellGapsInBinary::Run()
{
	double max_scalar=-1, min_scalar=1e9, s;
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New(); 
	mapper->SetInputData(_SourcePolyData); 

	/////////////////////////////////////////Code for Hue-Color Values////////////////////////////////////////////////////////////////
	// first you find what is the maximum and minimum scalar 
	vtkFloatArray* scalars = vtkFloatArray::New();
	scalars = vtkFloatArray::SafeDownCast(_SourcePolyData->GetPointData()->GetScalars());  // bring al lthe scalars to an array 
	
	for (vtkIdType i=0;i<_SourcePolyData->GetNumberOfPoints();i++)		// running through each point i 
	{
		s = scalars->GetTuple1(i); 
		if (s > max_scalar)         
			max_scalar = s;
	
		if (s < min_scalar)
			min_scalar = s; 
	}
	cout << "Maximum and minimum scalars = (" << max_scalar << "," << min_scalar << ")" << endl;
	// this is your polydata mapper object 
	mapper->SetScalarRange(min_scalar, max_scalar);      // you must tell your mapper and lookuptable what is the range of scalars first 
	mapper->SetScalarModeToUsePointData();    // mapper->SetScalarModeToUsePointData(); is also possible if you are using cell data 
	mapper->ScalarVisibilityOn(); 
	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New(); 
	lut->SetTableRange(min_scalar, max_scalar); 
	lut->SetHueRange(0.3, 0.0);  // this is the way you tell which colors you want to be displayed.
	lut->Build();     // this is important 
	mapper->SetLookupTable(lut); 
	
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New(); 
	actor->SetMapper(mapper); 

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New(); 
	_RenderWindow = vtkSmartPointer<vtkRenderWindow>::New(); 
	
	renderer->AddActor(actor); 

	_RenderWindow->AddRenderer(renderer); 

	_InteractorRenderWindow = vtkSmartPointer<vtkRenderWindowInteractor>::New(); 
	_InteractorRenderWindow->SetRenderWindow(_RenderWindow); 
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> interactorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	_InteractorRenderWindow->SetInteractorStyle(interactorStyle);

	
	_RenderWindow->Render();

	/*
	_cell_picker = vtkSmartPointer<vtkCellPicker>::New();
	_cell_picker->SetTolerance(0.0005);	// You set a tolerance meaning to what degree of accuracy it is able to select points 
	_InteractorRenderWindow->SetPicker(_cell_picker);		// and you tell which interactor this cell picker is part of (you only have one interactor) 
	*/
	// point pickers
	_cell_picker = vtkSmartPointer<vtkCellPicker>::New();
	_cell_picker->SetTolerance(0.005);
	_InteractorRenderWindow->SetPicker(_cell_picker);

	vtkCallbackCommand *callback = vtkCallbackCommand::New();			
    callback->SetCallback(LaShellGapsInBinary::KeyPressEventHandler);		
	callback->SetClientData(this);		
    _InteractorRenderWindow->AddObserver(vtkCommand::KeyPressEvent, callback);		// invoke callback when key is pressed

	_InteractorRenderWindow->Start();

	
}