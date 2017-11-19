# 3D_Motor

[![Build status](https://ci.appveyor.com/api/projects/status/7s5gxwcn7h3we5dx?svg=true)](https://ci.appveyor.com/project/rogerta97/3d-motor)

Contains a 3D motor developed by Pau Bonet and Roger Tello as a project for teh 3D engines subject on the UPC Design & Development of Videogames degree. 

Github repository link: https://github.com/rogerta97/3D_Motor

Github of the authors:

	- Roger Tello: https://github.com/rogerta97

	- Pau Bonet: https://github.com/BooLAW

Engine Information:

Menu:
->Files

	-Exit(L_shift + ESC): exits the applitacion
	
->Tools

	-Console:shows and hides the console panel, the console LOGS the start LOGS and when you import geometry it shows the details of it.
	
	-RandomNum:shows and hides a random generator tool panel.
	
	-Configuration:shows the configuration panel where we can find a Menu Items with functionalities that apply to the scene or the engine workflow, it also shows the system information.
	
	-Performance:shows the Start-up performance of the modules and the run time graphs of the modules with update.
	
->Help

	-DEMO GUI: shows the original demo panel from ImGui.
	
	-Documentation:opens the github repository page.  
	
	-Download latest: brings you to the latest release/draft.
	
	-Report a bug:brings you to github issues.
	
	-License:brings you to the MIT license about the github repository
	
	-About:Prints a short explanation about the engine(authors,libraries used,info of the engine)
	
InGame:
	-Camera Movement:
	
		- WASD: movement (forward,left,backwards,right)
		
		- F: camera focuses the last mesh
		
		- C & X: move up and down the camera.
		
		- Alt + left mouse click: camera orbits with mouse movement.
		
		- Mouse wheel: zoom in and out.
		
		- Right Click:camera moves with the mouse movement.
		
	-Import .FBX files: 
	
		- Drag and drop into the scene.
		
Extra Features: 
	
	- Parents can be assigned and deasigned in runtime. 
	
	- Objects can be deleted, it's child will delete too. 
	
	- Components can be deleted.
	
	- A main camera can be picked in runtime.
	
	- Resource manager


License:

MIT License

Copyright (c) 2017 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
