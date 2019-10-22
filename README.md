# TinyPokemonWorld

WARNING: This is a work in progress Portfolio item.

[Emscpriten](https://emscripten.org/) + [GLFW3](https://www.glfw.org/) + [GLAD](https://www.glfw.org/) + [GLM](https://github.com/g-truc/glm/releases) + [ImGui](https://github.com/ocornut/imgui)

## Setup
OPTIONAL: Setup [Emscpriten](https://emscripten.org/)
1. Create the folder "external" in the root of this project `$ mkdir external`
2. Download [GLAD with opengl es 3.2](http://glad.dav1d.de/#profile=core&specification=gl&api=gl%3Dnone&api=gles1%3Dnone&api=gles2%3D3.2&api=glsc2%3Dnone&language=c&loader=on) and place the unzipped files in `./external/glad` 
3. Download [GLFW3 3.2.1](https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip) and place the unzipped files in `./external/glfw-3.2.1`
4. Download [GLM 0.9.9.5](https://github.com/g-truc/glm/releases/download/0.9.9.5/glm-0.9.9.5.zip) and place the unzipped files in `./external/glm-0.9.9.5`
5. Download [ImGui v1.71](https://github.com/ocornut/imgui/archive/v1.71.zip) and place the unzipped files in `./external/imgui-1.71` 
6. Download [unit_test.hpp](https://github.com/timostrating/unit_test) an place it in `./external/unit_test`  

## Building it

Build the Emscpriten version

`$ cd build/emscripten` <br/>
`$ emconfigure cmake ./` <br/>
`$ make` <br/>
`$ emrun out/TinyPokemonWorld.html` <br/>


Or build the normal desktop version

`$ cd build/desktop` <br/>
`$ cmake ./` <br/>
`$ make` <br/>

## Inspired by

https://github.com/hilkojj/cpp-GAME-utils <br/>
https://github.com/hilkojj/cpp-GAME-utils-template

https://github.com/emscripten-core/emscripten/tree/incoming/tests

https://www.amazon.com/Computer-Graphics-OpenGL-Donald-Hearn/dp/0130153907