# How To Train Your Particles
![sampleframe](https://github.com/kushquad/How-To-Train-Your-Particles/blob/master/frame.png "Sample frame")
## Instructions to Run:
This project is meant to be run on a Linux machine with support for OpenGL 3.0+ and a gcc C++ compiler(4.7+)

Download the package freeglut3-dev using your default package manager.
In the /physics directory, run 'python main.py' to run a particle simulation for the given scene.
This creates a particle cache file .cch in /cache directory.
Now, compile the C++ code in /graphics directory with 'g++ *.cpp -lglut -lm -lGL -lGLU'
Run the executable ./a.out.
The executable will allow the user to navigate the scene using direction keys for movement. W and S keys control going up or down. To pause/play the particle simulation within scene, press P. To view the particle simulation at a set pace, press I. This will move through one round of simulation.

N and M keys control the tilt of the camera, middle mouse can be used for zoom. Panning is supported with the mouse using the left mouse button. Press Esc to quit the program.

Ensure that the particle cache is created first and then only the C++ executable run. Currently the graphic engine is set to read only 75 frames of particle simulation, this can be set in scene.cpp as the maxframes variable. Python is set to export 96 frames. This can be set in constants.py as the variable MAX_ITERATIONS.

## To-do
1. Clearly, writing the physics engine in Python was to quickly prototype the collision and split behaviours and make necessary iterations to improve it. I plan to optimize it by using C++/CUDA to accelerate the collision detection and response functions. Most of the operations are map/filter operations which are clearly parallelizable. 
2. Texturing a few surfaces, and applying shaders to the others. Using a skybox for the scene to give an environment. I plan to use a Phong shader with shine, to create a ceramic look for the rabbit. The other geometry I plan to give a glassy reflective look 
3. Making use of the age/life mechanic I had planned for, both for particle rendering and simulation.
4. Improving the particle rendering look more realistically like sparks. This includes giving glow. 
5. Patching a few bugs with the collision engine. 
6. Improving the design and structure of both the engines.
