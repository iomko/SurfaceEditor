# SurfaceEditor
 Application that allows us to manipulate with planar or non-planar surfaces.

# Scientific paper
The full paper describing this project is available here:

[Read the paper (PDF)](paper/my_scientific_paper.pdf)

## Supported Platforms
Currently, this project is **supported and tested only on Linux** systems.
Support for other operating systems (such as macOS or Windows) may be added in the future.

### Dependencies
 To successfully build and use this project, cmake is requiring you to already have those packages installed: X11, glfw, OpenGL, glm, TIFF, CURL, Eigen3
#### Installation of those packages
_**X11 package**_  
Ubuntu / Mint / Debian : <code>sudo apt install xorg-dev</code>  
Fedora : <code>sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel</code>    
Arch : <code>sudo pacman -S libx11 libxrandr libxinerama libxcursor libxi</code> 

_**GLFW package**_  
Ubuntu / Mint / Debian : <code>sudo apt install libglfw3-dev</code>  
Fedora : <code>sudo dnf install glfw-devel</code>    
Arch : <code>sudo pacman -S glfw</code>  

_**OpenGL package**_  
Ubuntu / Mint / Debian : <code>sudo apt install mesa-common-dev libglu1-mesa-dev</code>  
Fedora : <code>sudo dnf install mesa-libGL-devel mesa-libGLU-devel</code>    
Arch : <code>sudo pacman -S mesa glu</code>

_**GLM package**_  
Ubuntu / Mint / Debian : <code>sudo apt install libglm-dev</code>  
Fedora : <code>sudo dnf install glm-devel</code>    
Arch : <code>sudo pacman -S glm</code>  

_**TIFF package**_  
Ubuntu / Mint / Debian : <code>sudo apt install libtiff-dev</code>  
Fedora : <code>sudo dnf install libtiff-devel</code>    
Arch : <code>sudo pacman -S libtiff</code>

_**CURL package**_  
Ubuntu / Mint / Debian : <code>sudo apt install libcurl4-openssl-dev</code>  
Fedora : <code>sudo dnf install libcurl-devel</code>    
Arch : <code>sudo pacman -S curl</code>

_**Eigen3 package**_  
Ubuntu / Mint / Debian : <code>sudo apt install libeigen3-dev</code>  
Fedora : <code>sudo dnf install eigen3-devel</code>    
Arch : <code>sudo pacman -S eigen3</code>

### Generating files with the CMake command-line tool
Run: <code>cmake -S path/to/project_dir -B path/to/build_dir</code>

### Compiling the project
<code>cd path/to/build_dir</code>  
<code>make</code>  
