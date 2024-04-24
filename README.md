# Nigozi Engine
![image](https://github.com/LukaLominadze/NigoziEngine/assets/142942110/a63aec34-2335-42a5-90e3-f77dd0552cff)

Nigozi Engine is a game engine framework designed to make 2D devolopment easier in C++. The engine is in the beginning stages of devlopment, many features and systems are yet to come!

***

## Getting Started
Visual Studio 2020 (and above) is recommended, but you are free to try other applications to build the project.

<ins>**1. Downloading the repository:**</ins>

Start cloning the repository with ```git clone --recursive https://github.com/LukaLominadze/NigoziEngine```

<ins>**2. Building the project:**</ins>

The engine primarly only supports Windows and the build scripts will only run on this platform.

After cloning the repository, simply run the ```Setup-windows.bat``` file.

You can type in the preffered build option, if a Visual Studio solution is needed, type the prefix 'vs' and the year.
```cmd
vs2022'
```
![image](https://github.com/LukaLominadze/PongSDL/assets/142942110/5c6a2334-fbde-451f-bcfa-c0c9bfac9eb8)

Press enter and the project will be built.

# Dependencies
Nigozi Engine is primararly using SDL2 and SDL2_image libraries, which are provided in the vendor folder from the root didrectory.

# The plan
The plan for Nigozi Engine is to make a 2D framework that makes the development process simple and clear and to provide core functionality and features many games have.

# Short-term goals
- Reimplement the layer system for more optimal performance
- Reimplement the Event system for better performance, clarity and usability
- Implement basic components
- Create a collision system
- Create sprite animator
- Implement deltaTime

# Long-term goals
- Create functional UI layer for debugging
- Implement scenes to allow the creation of levels
- Implement a tilemap system
- Implement Audio system and spacial audio
- Create games for preview of the engine's capabilities
