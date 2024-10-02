Dependencies  
The project requires the following dependencies to run:  
OpenGL: The core graphics library.  
GLUT (OpenGL Utility Toolkit): For managing windows, inputs, and basic rendering setup.  
  
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
'S'		: Move the object down  
'D'		: Move the object right  
'+'		: Move the object further away  
'-'		: Move the object closer  
'O'		: Increase the object speed  
'P'		: Decrease the object speed  
  
Rotating the object:  
'X'		: Toggle X axis rotation  
'Y'		: Toggle Y axis rotation  
'R'		: Stop all rotations  
'J'		: Accelerate rotations  
'K'		: Decelerate rotations  
  
Camera commands:  
'C'					: Toggle camera mode  
'Z', 'W'		: Move forward  
'Q', 'A'		: Move left  
'S'				: Move down  
'D'				: Move right  
'+'				: Move up  
'-'				: Move down  
'LEFT_ARROW'	: Turn left  
'RIGHT_ARROW'	: Turn right  
'DOWN_ARROW'	: Turn down  
'UP_ARROW'		: Turn up  
