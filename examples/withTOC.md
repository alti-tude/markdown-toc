- [1. Basic Setup](#basic-setup)
	- [1.1. How to Use](#how-to-use)
	- [1.2. libxinerma and libcursor errors](#libxinerma-and-libcursor-errors)
	- [1.3. Cmake](#cmake)
			- [1.3.0.1. Cmake With Externals Folder](#cmake-with-externals-folder)
	- [1.4. What the heck is Glew?](#what-the-heck-is-glew?)
	- [1.5. Headers](#headers)
- [2. Concepts](#concepts)
	- [2.1. Shader](#shader)
		- [2.1.1. Types](#types)
	- [2.2. Index Buffer](#index-buffer)
	- [2.3. Uniforms](#uniforms)
- [3. OpenGL Functions](#opengl-functions)
	- [3.1. Creating a Buffer (vertex_buffer)](#creating-a-buffer-(vertexbuffer))
	- [3.2. Vertex attributes (shader_basic)](#vertex-attributes-(shaderbasic))
	- [3.3. Shader Basics(shader_basic)](#shader-basics(shaderbasic))
		- [3.3.1. Compiling Each Shader](#compiling-each-shader)
		- [3.3.2. Creating a Program](#creating-a-program)
		- [3.3.3. Specifying Shaders(GLSL -> GL Shader Language)](#specifying-shaders(glsl-->-gl-shader-language))
		- [3.3.4. Using The Shader](#using-the-shader)
		- [3.3.5. Shader with Files(shader_file)](#shader-with-files(shaderfile))
	- [3.4. Index BUffer(shader_file)](#index-buffer(shaderfile))
		- [3.4.1. glDrawElements](#gldrawelements)
	- [3.5. Error Handling(errors)](#error-handling(errors))
	- [3.6. Uniforms(uniforms)](#uniforms(uniforms))
		- [3.6.1. Add uniform variable to the shader file](#add-uniform-variable-to-the-shader-file)
	- [3.7. Vertex Array Object(vao)](#vertex-array-object(vao))
		- [3.7.1. creating an actual VAO](#creating-an-actual-vao)
- [4. Reference](#reference)

# Basic Setup 

## How to Use
```
mkdir build
cd build
cmake ..
```
A make file will appear in  the build dir.
```
make all
```
All the tutorials will be compiled and executables will appear in ../tutorial0xxxxx dirs

## libxinerma and libcursor errors
```bash
sudo apt-get install libxcursor-dev
sudo apt install libxinerama-dev
```

## Cmake

This command will install all the libraries in the lookup root, so you don't need a externals forlders,
but that is preffered, for compatibility.
```
sudo apt install libglew-dev libglfw3-dev libglm-dev cmake pkg-config
```

#### Cmake With Externals Folder
```
cmake_minimum_required (VERSION 2.6)
project (your_project_name)

find_package(OpenGL REQUIRED)

add_executable(your_exe_name
    tutorial04_colored_cube/tutorial04.cpp
    common/shader.cpp
    common/shader.hpp
)
```

include directories(externals folder)
```
include_directories(
    external/AntTweakBar-1.15/include/
    external/glfw-2.7.2/include/
    external/glm-0.9.1/
    external/glew-1.5.8/include/
    .
)
```

linking
for opengl_libraries explanantion (https://cmake.org/cmake/help/v3.10/module/FindOpenGL.html#result-variables)
```
set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_executable(run 
    main.cpp
)

target_link_libraries(run ${ALL_LIBS}) 
```

## What the heck is Glew?
The OpenGL Extension Wrangler Library is a cross-platform C/C++ library that helps in querying and loading OpenGL extensions. GLEW provides efficient run-time mechanisms for determining which OpenGL extensions are supported on the target platform.  
  
<i><u>Basically</u></i> opengl is implemented in the graphics drivers. The glew wrangler abstracts the access to these functions in the drivers.

## Headers
```c++
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
```
------------------------------------------------------------------------------------------

# Concepts
## Shader
A program that runs in the gpu, reads the buffer, and displays.
### Types
1. Vertex Shader
    * code run once for each vertex
    * for positioning vertex
2. fragmentation shader
    * run once for each pixel
    * for deciding color of each pixel

## Index Buffer
* removes redundant storage of vertices
* must be all unsigned

## Uniforms  
* a method for passing data from the cpu to the shader(gpu)
-------------------------------------------------------------------------------------------
# OpenGL Functions
## Creating a Buffer (vertex_buffer)
```c++
GLfloat positions[]={
     0.0f,  0.5f,
    -0.5f, -0.5f,
     0.5f, -0.5f
};
unsigned int buffer;
glGenBuffers(1, &buffer); //the number of buffers and storage for buffer id
glBindBuffer(GL_ARRAY_BUFFER, buffer); //the type of buffer is ARRAY, selects the "buffer".
glBufferData(GL_ARRAY_BUFFER, 6*sizeof(GLfloat), positions, GL_STATIC_DRAW); //it assigns data to the current bounded buffer.
```
All the rendering etc, is done for the current *bounded* buffer only.

## Vertex attributes (shader_basic)
```c++
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
```
A vertex might have multiple attributes. Eg, 3 floats for position, 2 for normal, etc...  
  
We use these functions to parse out the attributes and assign them an integer identifier.  
Eg, in the above case the 2d coords are are the only attributes of each vertex and they are assigne to the index 0.  
  
The vertex shader are written so as to make use of these identifiers.  

* *index* -> the index of the attribute, to identify this one if the vertex has multiple attribs
* *size* -> the number of components of the attribute per vertex, eg 2 coords for the point
* *type* -> data type of the components
* *normalise* -> normalise the values so that they fall between 0 and 1 
* *stride* -> number of bytes between start of 2 vertices in the buffer
* *pointer* -> the bite offset within a vertex for each component of the attribute

Also, we need to enable the usage of this attrib
```c++
glEnableVertexAttribArray(0);
```

combined
```c++
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
```

enable before or after, doesn't matter

## Shader Basics(shader_basic)

### Compiling Each Shader
1. create empty shader of the given type(GL_VERTEX/FRAGMENT_SHADER)
2. add the string to the source code of the shader 
    * multiple strings can be provided, in form of array of pointer int the src
    * attr: ShaderId, number of strings provided in the array, the actual array pointer, pointer to and array of lengths(if null, then it is assumed that all the strings are null terminated)
3. compile shader
4. error handling
```c++
static unsigned int CompileShader(unsigned int type, std::string& source){
    unsigned int shaderId = glCreateShader(type); //creating empty shader
    const char* src = source.c_str(); //the actual compilation needs a pointer to the string
    glShaderSource(shaderId, 1, &src, NULL); //specifying source code
    glCompileShader(shaderId); //compiling(generates a compiled shader object intermediate)

    //error handling
    int compileStatus;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus); 
    if(compileStatus == GL_FALSE){
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        char* errMsg = (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, errMsg);
        std::cout << "Error in " << (type==GL_VERTEX_SHADER? "vertex":"fragment") << " shader\n" << errMsg << std::endl;
        
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}
```
### Creating a Program
1. create a program
2. compile the two shaders
3. attach them to the program
4. link the program and the shaders
5. validate the program
6. delete shader intermediates(the compiles objects, not the source)
```c++
static unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program); //just for validating if the current program can execute for the current state

    //delete the compilation intermediates (the compiled shaders, not the source)
    glDeleteShader(vs);    
    glDeleteShader(fs); 
    return program;
}
```
### Specifying Shaders(GLSL -> GL Shader Language)
```c++
std::string vertexShader = 
    "#version 330 core\n"
    "layout(location = 0) in vec2 pos;\n" // in means that we take input from location 0
    "void main(){\n"
    "gl_Position = vec4(pos.xy, 0.0, 1.0);\n"
    "}\n";

std::string fragmentShader =
    "#version 330 core\n"
    "out vec3 color;\n" //out means we are outputting something
    "void main(){\n"
    "color = vec3(1,0,0);\n"
    "}\n";
```

### Using The Shader
```c++
unsigned int programId = CreateShader(vertexShader, fragmentShader);
glUseProgram(programId);

//game loop

glDeleteProgram(programId);
```
### Shader with Files(shader_file)
```c++
struct ShaderSource{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSource ParseShader(std::string file){
    std::ifstream stream(file);
    // std::cout << file << std::endl;
    // std::cout << stream.is_open() << std::endl;
    // std::perror("error: ");
    enum class ShaderMode{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderMode mode = ShaderMode::NONE;
    std::string line;
    std::stringstream ss[2];

    while(getline(stream, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos)
                mode = ShaderMode::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                mode = ShaderMode::FRAGMENT;
        }
        else if(mode != ShaderMode::NONE)
            ss[(int)mode] << line << "\n";
    }

    return {ss[0].str(), ss[1].str()};
}
```

using it
```c++
ShaderSource src = ParseShader("shaders.shader");
unsigned int programId = CreateShader(src.vertexSource, src.fragmentSource);
glUseProgram(programId);
```

Shader File(shaders.shader)
```
#shader vertex
#version 330 core
layout(location = 0) in vec2 pos;
void main(){
gl_Position = vec4(pos.xy, 0.0, 1.0);
}

#shader fragment
#version 330 core
out vec3 color;
void main(){
color = vec3(1,0,0);
}
```

## Index BUffer(shader_file)

This will draw a square
```c++
float positions[] = {
    -0.5f, -0.5f, //index 0
    0.5f, -0.5f,  //index 1
    0.5f,  0.5f,  //index 2
    -0.5f,  0.5f  //index 3
};

unsigned int indices[] = {
    0, 1, 2, //the order in which to draw the triangles, each triplet composed of the index of vertices of the triangle;
    2, 3, 0
};
```

initialising index buffer
```c++
unsigned int ibo;
glGenBuffers(1, &ibo);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*6, (const void*)indices, GL_STATIC_DRAW);
```

in the while loop
```c++
do{
    glClear(GL_COLOR_BUFFER_BIT);

    //the last param is a pointer to the indices arr, this is null, cuz it has just been boun, so already selected
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL); //changed this
    
    glfwSwapBuffers(window);
    glfwPollEvents();   
}while(!glfwWindowShouldClose(window));
```
<b><u><i> All the valus in the index buffer must be unsigned </i></u></b>

### glDrawElements
* *mode* -> triangle or something else
* *count* -> of the number of indices to take from the index arr
* *type* -> data type of values in index buffer
* *pointer* -> to the indices arr, this is null, cuz it has just been boun, so already selected

## Error Handling(errors)
```c++
#define GLCall(x) GLClearError();\
    x;\
    assert(GLLogCall(#x, __FILE__, __LINE__));

static void GLClearError(){
    while(glGetError()!=GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line){
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
        std::cout << function << ": " << file << ":: " << line << std::endl;
        return false;
    }
    return true; 
}
```

## Uniforms(uniforms)
### Add uniform variable to the shader file
```c++
#shader vertex
#version 330 core
layout(location = 0) in vec2 pos;
void main(){
gl_Position = vec4(pos.xy, 0.0, 1.0);
}

#shader fragment
#version 330 core
out vec3 color;
uniform vec3 u_color; //added this

void main(){
color = u_color;
}
```

1. Bind the shader program(glUseProgram)
2. get the location of the uniform variable(glGetUniformLocation)
3. pass the values
```c++
GLCall(glUseProgram(programId));
GLCall(int u_color_location = glGetUniformLocation(programId, "u_color"));

//passing the values
GLCall(glUniform3f(u_color_location, r, 0.0f, 0.8f)); //3f means 3 floats
```
But this won't allow the coloring of different components differntly

## Vertex Array Object(vao)
```c++
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //compat profile defauts to giving us a vao, core profile needs us to create one explicitly
```

### creating an actual VAO
creating a vao, and binding it to a buffer
```c++
unsigned int vao;
GLCall(glGenVertexArrays(1, &vao));
GLCall(glBindVertexArray(vao));

//this buffer is automagically bound to the vao
unsigned int buffer;
GLCall(glGenBuffers(1, &buffer));
GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*2, (const void*)positions, GL_STATIC_DRAW));

GLCall(glEnableVertexAttribArray(0));
GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0));
```

unbind everything outside the game loop
```c++
GLCall(glBindVertexArray(0)); //unbind vertex array
GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); //unbind array buffer
GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); //unbind index buffer
GLCall(glUseProgram(0)); //unbind shader program
```

*Inside the loop*, we <b><u><i>don't</i></u></b> need to 
1. bind the array buffer again. 
2. enable the attribute
3. specify the attribute pointer

Just bind the
1. vao
2. index buffer
3. shader

```c++
GLCall(glBindVertexArray(vao)); //bind vao
GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); //bind index buffer
GLCall(glUseProgram(programId)); //bind shader program
GLCall(glUniform3f(u_color_location, r, 0.0f, 0.8f));

GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
```
----------------------------------------------------------------------------------------------------
# Reference
1. [tutorial site](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/)  
2. [cmake](http://www.opengl-tutorial.org/miscellaneous/building-your-own-c-application/)  
3. [cmake stackoverflow](https://stackoverflow.com/questions/9460242/how-to-compile-glut-opengl-project-with-cmake-and-kdevelop-in-linux)
4. [docs.gl](http://docs.gl)
