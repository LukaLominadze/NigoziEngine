# Nigozi Engine


https://github.com/user-attachments/assets/0b82034c-a8a8-4e6f-9ab3-210ffa51dbb3



Nigozi Engine is a game engine framework designed to make 2D devolopment easier in C++. The engine is in the beginning stages of devlopment, many features and systems are yet to come!

***

## Platform
The engine primarly supports Windows.

## Getting Started
Visual Studio 2020 (and above) is recommended.

<ins>**1. Downloading the repository:**</ins>

Start cloning the repository with ```git clone --recursive https://github.com/LukaLominadze/NigoziEngine```

<ins>**2. Building the project:**</ins>

After cloning the repository, run the ```Setup-windows.bat``` file.

The project is now ready.

# Dependencies
- GLFW - For window creation and event polling
- GLEW - For rendering with OpenGL
- GLM - For math
- StbImage - For handling/loading images
- DearImGui - For UI

# The plan
The plan for Nigozi Engine is to make a 2D framework that makes the development process simple and clear and to provide core functionality and features many games have.

# Short-term goals
- Add ECS
- Add new shaders

# Long-term goals
- Create an Editor to make game creation easier
- Add a scripting language
- Implement audio systems

## Writing Code
Once a project is set up with NigoziEngine, you can go ahead and create a main file. This is where the entry point of the application will be.

Firs you will need to define "MAIN" before including Nigozi to indicate where the entry point is.

There are 2 external functions:
CreateApplicationProps() - To define the window title, size, flags (vsync on/off, fullscreen on/off)
OnApplicationInitialized(Nigozi::Application& app) - To add some extra initialization code before running the program.
```cpp
#define MAIN // sets the file as entry point

#include <Nigozi.h>

extern Nigozi::ApplicationProps CreateApplicationProps() {
	return { "MyGame", 960, 540, true, false }; // returns the window parameters
}

extern void OnApplicationInitialized(Nigozi::Application& app) {
	// Write additional initialization code here
}
```

The application is ready to be launched.

## Layers
Layers is an essential part of the engine, that simplifies the process of adding new systems in the engine.

Create a new file and a layer class:
```cpp
#include <Nigozi.h>

using namespace Nigozi;

class ExampleLayer : public Layer
{
};
```

The base class layer has virtual functions which are run at different parts of the program

- OnAttach - When the layer is added to the application
- OnDetach - When the application closes.
- OnUpdate(timestep) - Runs every frame. "timestep" represents time between the frames (delta time)
- OnEvent(Event& e) - Runs every time an event occurs: mouse click, scroll, key press, release, etc.
- OnRender() - Runs before rendering the frame
- OnImGuiRender() - Runs after OnRender and is used to draw UI.
```cpp
class ExampleLayer : public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float timestep) override;
	void OnEvent(Event& e) override;
	void OnRender() override;
	void OnImGuiRender() override;
};
```

You can go ahead and implement these functions the way you want.

To add layers to the application, let's go back to our main file, in the OnApplicationInitialized function, and use the application PushLayer function to add an instance of our layer to the engine.

Final code should look something like this:
```cpp
#define MAIN

#include <Nigozi.h>
#include "ExampleLayer.h"

extern Nigozi::ApplicationProps CreateApplicationProps() {
	return { "MyGame", 960, 540, true, false };
}

extern void OnApplicationInitialized(Nigozi::Application& app) {
	app.PushLayer(new ExampleLayer());
}
```

The application is ready to be launched.
