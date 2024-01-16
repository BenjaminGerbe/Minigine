# Minigine

Minigine is a Mini Engine coding in cpp with the opengl api. 
There's nothing innovatif it's only for educational purpose.

# How to build 
clone the project and use cmake to generate the build in x86. Only x86 will work beceause the precompile libs are in x86 !!
```
mkdir build
cd build
cmake -G [your generator] -A win32 ..
cmake --build .
```

> [!IMPORTANT]
> You need to copy past the glfw3.dll (in libs GLFW/lib-static-ucrt) and the yaml-cppd.dll (in the libs/yaml-cpp/lib/debug) in the source project near the .exe otherwise the project won't open.
