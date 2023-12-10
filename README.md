# Emmerald-Engine

Emmerald Engine is a game engine in C++ currently being developed for the Engine Development subject project for the CITM Bachelor's degree in Video Game Design and Development. In which we have to develop a game engine under 4 months.


# Creators

The engine is being developed by:

* [Xarderos](https://github.com/Xarderos) (Xavi Prats Paredes)

* [GHOSTVCL](https://github.com/GHOSTVCL) (Joan Giol Pozo)


# Install

* Download the most recent release
* Extract the .zip
* Click on the Emmerald Engine folder and Execute Emmerald-Engine.exe


# Features

**Emmeral-Engine v.0.66:**

## 1st assignment

* GameObject Hierarchy & Inspector

* Mesh, Texture & Trasform components

* Drag&Drop texture & FBX importer

* Unity-like camera Movement

* Configuration for window, rendering & FPS

* Delete not working as expected, sorry about that

* When creating a primitive since you can't delete the house, you will not be able to see it until you go inside the house

## 2nd assignment

### Inspector:

The user should can use the inspector to modify a GameObject:
* Hierarchy: delete, reparent, create empty and create children
* Transform: translate, rotate and scale Game Objects
* Mesh: select or drop any imported mesh
* Texture: select or drop any imported texture
* Camera is a component with settings that can be modified

### GameObjects:

* Drag andDrop texture & FBX importer in the Hierarchy
* Create game objects with Objects Menu
* Delete game objects by pressing Objects/Delete GameObjects/Delete 
* Delete selected game object at Hierarchy by pressing Delete
* Move the GameObjects through the Hierarchy by dragging and dropping them
* GameObjects can be picked using the mouse

### Frustum:

All meshes use a bounding volume (AABB) and can be discarded using Frustum Culling. This can be visualized in the editor (debug raycast and boxes)

### Frustum Culling:

While playing, the engine only draws the meshes that can be seen by the main camera frustum. That can be seen if you press the "play" button

### Serialization:

All configuration can be Saved and Loaded through a json file, for example: Brightness, Show/Hide Wireframe...

### Windows:

The editor has 2 windows one with the scene(editor view) and another captured from a gameobject with a camera component(main game camera), each one has its own framebuffer and the user can visualize both at the same time

The user also can Star / Stop / Pause the simulation

### Components:

* Component Transform
* Component Mesh
* Component Material
* Component Camera


# Controls:

## Camera:

* Right Click on scene window to rotate the camera

* W/A/S/D to move the camera

* On scene window press Q/E for going UP/DOWN

* Hold Shift + W/A/S/D to move faster

* Press F to focus camera position to the center

* Alt + Left Click to rotate around the center

* Scroll up and down to move the camera in and out

## Hierarchy:

* In Objects window, Hover to open the primitive menu, click to create one

* In Objects window, Click on a gameObject in order to select it

* In Objects window, Click + Drag Mouse on a gameobject into another to change a gameobject parent or child

## Drag And Drop:

* This engine can handle the import of an fbx

* This engine handles the import of a PNG/DDS convert it into a texture and apply it on a gameobject, but the gameobject has to be selected first


## License

MIT License

Copyright (c) 2023 Xavi Prats Paredes & Joan Giol Pozo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.