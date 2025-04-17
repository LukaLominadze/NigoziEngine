# Nigozi Engine

![poster](https://github.com/user-attachments/assets/90d53fd3-baf6-422a-8be8-be63c046fb53)

## ___Sandbox application showcasing the engine in action!___

https://github.com/user-attachments/assets/eba49d67-91fa-48c7-a1f6-ef870e182ad0

## ___The editor (in development) to be used for game development with Nigozi!___

https://github.com/user-attachments/assets/e2313d77-cdcb-4b7f-a6dc-f41b61983543

Nigozi Engine is a game engine framework designed to make 2D devolopment easier in C++. The engine is in the beginning stages of devlopment, many features and systems are yet to come!

***

## Platform
The engine primarly is supported on Windows and Linux.

## Getting Started
Visual Studio 2020 (and above) is recommended.

<ins>**1. Downloading the repository:**</ins>

Start cloning the repository with ```git clone --recursive https://github.com/LukaLominadze/NigoziEngine```

<ins>**2. Building the project:**</ins>

After cloning the repository, run the ```Setup-windows.bat``` file.
or if you are on linux, run ```setup-linux.sh```, then build with ```sudo make```

The project is now ready.

# Dependencies
- GLFW - For window creation and event polling
- GLEW - For rendering with OpenGL
- GLM - For math
- StbImage - For handling/loading images
- DearImGui - For UI
- ENTT - For implementing game objects and components

# The plan
The plan for Nigozi Engine is to make a 2D framework that makes the development process simple and clear and to provide core functionality and features many games have.

# Achieved goals
- Added ENTT
- Started creating the editor

# Short-term goals
- Add new shaders
- Add UI for creating entities
- Add CTRL+Z function

# Long-term goals
- Create an Editor to make game creation easier
- Add a scripting language
- Implement audio systems

## Writing Code
Once a project is set up with NigoziEngine, you can go ahead and create a main file. This is where the entry point of the application will be.

Firs you will need to define "MAIN" before including Nigozi to indicate where the entry point is.

Then, you will need to define a function for creating the application:
```cpp
#define MAIN // sets the file as entry point

#include <Nigozi.h>

extern Nigozi::Application* CreateApplication() {
	Nigozi::Aplication app = new Nigozi::Application{ "MyGame", 960, 540, true, false };
	return app;
}
```

The application is ready to be launched.

## Layers
Layers is an essential part of the engine, that simplifies the process of adding new systems in the engine.

Create a new file and a layer class.
The base class layer has virtual functions which are run at different parts of the program
```cpp
class ExampleLayer : public Layer
{
public:
	void OnAttach() override; // When the layer is added to the application
	void OnDetach() override; // When the application closes.
	void OnUpdate(float timestep) override; // Runs every frame. "timestep" represents time between the frames (delta time)
	void OnEvent(Event& e) override; // Runs every time an event occurs: mouse click, scroll, key press, release, etc.
	void OnRender() override; // Runs before rendering the frame
	void OnImGuiRender() override; // Runs after OnRender and is used to draw UI.
};
```

You can go ahead and implement these functions the way you want.

To add layers to the application, let's go back to our main file, in the CreateApplication() function.

Final code should look something like this:
```cpp
#define MAIN

#include <Nigozi.h>
#include "ExampleLayer.h"

extern Nigozi::Application* CreateApplication() {
	Nigozi::Aplication app = new Nigozi::Application{ "MyGame", 960, 540, true, false };
	app->PushLayer(new ExampleLayer());
	return app;
}
```

The application is ready to be launched.
