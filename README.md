Dependencies
The project requires the following dependencies to run:

OpenGL: The core graphics library.
GLUT (OpenGL Utility Toolkit): For managing windows, inputs, and basic rendering setup.
GLEW (OpenGL Extension Wrangler): Ensures the use of modern OpenGL functions.
GLM (OpenGL Mathematics): For handling matrix and vector operations.

Installing Dependencies on Ubuntu (or WSL)
You can install the necessary libraries using the following commands:
sudo apt update
sudo apt install libglew-dev libglfw3-dev libglm-dev
sudo apt install freeglut3-dev

How to run:
make
./scop path/to/obj_file.obj path/to/texture_file.ppm

Commands:
Moving the model:
'Z', 'W': Move the object up
'Q', 'A': Move the object left
'D'		: Move the object right
'S'		: Move the object down
'+'		: Move the object further away
'-'		: Move the object closer

Rotating the object:
'X'		: Toggle X axis rotation
'Y'		: Toggle Y axis rotation
'R'		: Stop all rotations
'J'		: Accelerate rotations
'K'		: Decelerate rotations