import vtk
import sys 

# Author: Rashed Karim, 2018
# Hello world of vtk visualisation for lassy shells 
# On Windows, use Anaconda to install vtk 
# On MacOS, use pip install vtk 
# Usage: python vtkTest.py lahsell.vtk 

# Generate polygon data for a cube
reader = vtk.vtkPolyDataReader()
reader.SetFileName(sys.argv[1])

# Create a mapper for the cube data
lashell_mapper = vtk.vtkPolyDataMapper()
reader.Update()
lashell_mapper.SetInputData(reader.GetOutput())

# Connect the mapper to an actor
lashell_actor = vtk.vtkActor()
lashell_actor.SetMapper(lashell_mapper)
lashell_actor.GetProperty().SetColor(1.0, 0.0, 0.0)

## Create the graphics structure. The renderer renders into the render
# window. The render window interactor captures mouse events and will
# perform appropriate camera or actor manipulation depending on the
# nature of the events.
ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
 
# Add the actors to the renderer, set the background and size
ren.AddActor(lashell_actor)
ren.SetBackground(0.1, 0.2, 0.4)
renWin.SetSize(200, 200)
 
# This allows the interactor to initalize itself. It has to be
# called before an event loop.
iren.Initialize()
 
# We'll zoom in a little by accessing the camera and invoking a "Zoom"
# method on it.
ren.ResetCamera()
ren.GetActiveCamera().Zoom(1.5)
renWin.Render()
 
# Start the event loop.
iren.Start()