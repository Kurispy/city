ECS 175: Computer Graphics
UC Davis, Spring 2013
3D City Scene
Christopher Chen
tech@ucdavis.edu

Requirements:
GLUT
GLEW
OpenGL
C++
cmake 


Overview:
A very basic 3D city scene with moving cars and apartment structures. You can 
press 1-9 to attach the camera to one of the cars. 0 Returns the camera to 
normal. Uses backface culling to remove any faces that won't be seen. Clipping 
is not currently implemented.

Note:
The streets texture doesn't seem to work on some CSIF machines, probably due to
its large size. I have included a low resolution version of the texture just in 
case the normal version doesn't work. Simply rename the file "streetslowres.bmp"
to "streets.bmp" and it should work.

Road textures found here:
http://blenderartists.org/forum/showthread.php?203964-Road-Texture-Pack

Car textures found here:
http://xmdservices.com/modules/com_joomlawatch/car-texture-999.html

Structure texture found here:
http://www.zeroterminal.com/textures/235-free-modern-glass-building-texture.html

Skybox texture found here:
http://www.keithlantz.net/wp-content/uploads/2011/10/skybox_texture.jpg

Wall texture found here:
http://www.3dmd.net/gallery/albums/textures/concrete/seamless_dark_smooth_concrete_texture.jpg


Commands to create makefile + compile + run:

cmake .
make
./city.x

Controls:

Mouse       Click and drag to look around
WASD        Movement
1-9         Attach Camera
0           Unattach Camera
ESC, q      Quit
