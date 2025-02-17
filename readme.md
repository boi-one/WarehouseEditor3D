# Warehouse Editor 3D
The warehouse editor 3D is made so you can create, edit and view conveyor setups in 2D and 3D space.  
[Link to the previous version of the warehouse editor](https://github.com/boi-one/WarehouseEditor)

---  

### How To Build

1. install cmake

2. create a new directory in the root called `build`

3. go to the build directory in the commandline and type `cmake ..`
###### If this didn't work change the path to vcpkg.cmake inside of the cmakelist.txt

---  
### Overview
#### Index  
[Layers](#Layers)   
[Conveyors](#Conveyors)    
[Settings](#Settings-Menu)    
[Camera Projection](#Camera-Projection)    
[Info Window](#Info-Window)  
### Layers
When you open the program you'll see something like this:

![empty scene.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/empty%20scene.png)

In the bottom left you'll see the layer window, this window will display all the current layers. The currently selected layer is displayed green.

![layers in layer window.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/layers%20in%20layer%20window.png)

Each layer has some  information and options. you could move the layer up and down in the window, which will affect the position of the layer in 3D space but we will get to that later. You could also select a layer, when selecting a layer this will become the active layer and you can edit it. You can also delete a layer but only when it is **not** selected. The layer can be hidden, this can be done any time with any layer. And at last there is the "edit conveyor width" dropdown. In here you can change the width of all of the conveyors in the selected layer.

When opening one of the layers you can see its content, you can see the current selected layer has a conveyor amount of 3 and when its open you can see information about those conveyors in more detail, you can also change the width of an idividual conveyor.

![conveyors in layer.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/conveyors%20in%20layer.png)

The last thing you can edit in the conveyor are its attributes, this doesn't do anything in the program but you can save custom data and give it a name, for example you want to say how fast the conveyor moves. Click on create attribute and in the name input you can type "speed" then in the value input you can type "10". Because 10 is a number and not a text string select the checkbox next to delete.

![conveyorattribute.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/conveyorattribute.png)

Each conveyor itself can also be selected, the selected conveyor will be dark green and also can't be deleted.
A conveyor exist out of points, when you open the conveyor, you can view all the points and their positions. When clicking on one of the points the 2D camera centers on the point.

![points in conveyor.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/points%20in%20conveyor.png)
### Conveyors

When clicking with the mouse somewhere on the screen a blue point will appear, this is the start of a conveyor. The green line is a part of the conveyor which isn't placed yet but it shows you how it would look like if you placed it there.

![place step 1.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/place%20step%201.png)

when clicking again a second point is added and a line is rendered between them.

![place step 2.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/place%20step%202.png)

The conveyor is color coded, there are four different colors, blue, yellow, white and gray.  
**Blue:** meaning the conveyor is currently being edited.  
**Yellow:** meaning the conveyor is selected but you can't edit it, only move it.  
**White:** meaning the conveyor is in the selected layer.  
**Gray:** meaning the conveyor is in another layer which is not selected.  

![conveyor color codes.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/conveyor%20color%20codes.png)

When right-clicking close to a point in the selected conveyor the green line will start at that point. This way you can make branching conveyors.

![change selected point.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/change%20selected%20point.png)

When there are multiple conveyors in a layer you can merge the two conveyors creating one big conveyor. You can do this by pressing shift close to the desired connection point. The second conveyor will disappear from the list and merge with the selected conveyor.

![connect conveyors.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/connect%20conveyors.png)

You can unselect a conveyor by pressing "Z" and to go out of edit mode you can press "X". when you press "X" you get into selection mode, in this mode you can only move the conveyor, you do this by dragging it while holding the middle mouse button. When you want to exit selection mode you can press "Z" to unselect the conveyor or right-click close to a point in the selected conveyor to go back into edit mode.
### Settings Menu

When pressing escape you will see a new window popup, this is the settings menu, here you can load and save what you've made, quit the program, toggle settings and see the key bindings. Most of the settings are pretty self explanatory but snap to grid can maybe be a little unclear, this setting will snap the placing position of points of a conveyor to the grid in the background. This way you can place conveyors with very precise distances from each other if this is desired. The shortcut for this toggle is "G". 
### Camera Projection

The camera projection setting will toggle between the orthographic projection (meaning there is no perspective, so everything looks flat) and perspective projection (meaning you'll see depth).  You can also switch between these projections when pressing "Tab". When you ever used a program like Unity which has the Z be the depth of the scene you'll probably wonder why the Z is up and the Y is the depth, this is so the conversion between 2D and 3D is smoother, with this coordinate system it will look like you look down on your 2D scene but in 3D instead of looking at it from the front. The field of view and the mouse sensitivity can be changed in the settings menu and will save when closing the settings menu.

One thing to note is that the 2D camera and the 3D camera have seperate position so when you move to a specific point in 3D space and you want to see it in 2D you'll have to manually go there in 2D.

![2D 3D comparision.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/2D%203D%20comparision.png)

If you still remember the layer explanation you'll probably remember me telling you about that the position of the layer in the list affects the position of the layer in 3D, so layer 2 is lower than layer 8. This will create a more immersive experience (probably).

![3D layer view 1.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/3D%20layer%20view%201.png)  
![3D layer view 2.png](https://github.com/boi-one/WarehouseEditor3D/blob/master/readme/3D%20layer%20view%202.png)

### Info Window

The info window contains miscellaneous info about the fps, the important positions and the screen resolution. This window can be toggled off using "I". 

###### Some of the images are of an older version but that shouldn't be an issue.

