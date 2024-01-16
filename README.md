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
> I really recommande to copy past the imgui.ini from the root of the project to the source project.

# Fonctionalities: 
|  |  |
|--|--|
|3D convex shape <br><img src="https://github.com/LaGerbe-Bohu/Minigine/assets/10485864/f9306bc0-1a29-4b6b-8879-1a60566b0e87" height="200" /> |MLP <br> <img src="https://github.com/LaGerbe-Bohu/Minigine/assets/10485864/ca0a082a-8b70-409b-8945-56e12dc894ac" height="200" />|
|Voronoi <br><img src="https://github.com/LaGerbe-Bohu/Minigine/assets/10485864/af81fb97-6ec7-40c8-960e-4778d71d8121" height="200" /> |Trochoidale waves <br> <img src="https://github.com/LaGerbe-Bohu/Minigine/assets/10485864/7a28045d-08cf-4560-8586-fb99a6a79269" height="200" />|
