# pg-linux-template

PG1 template compatible with linux (and maybe osx)

I am not the author of most of the source files and I certainly do not agree with the code style of this project. I left the original source codes almost untouched, including their undefined behavior and memory leaks, to make it as faithful as possible to the original :)

# changes

- dx11 was replaced with SDL2 and opengl
- I fixed few warnings
- tabs were replaced with spaces

# dependencies

- sdl2
- opengl
- glew
- freeimage3
- intel embree

and imgui, but it's already included :)


# usage

Take a look [here](build.sh)

```sh
./build.sh run
```
