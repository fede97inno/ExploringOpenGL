#include <iostream>
#include <format>
#include "Window.h"
#include "Ex01TriangleDraw.h"
#include "Ex02QuadDraw.h"
#include "Ex03QuadIndexDraw.h"
#include "Ex04QuadColorDraw.h"
#include "Ex05QuadTextureDraw.h"
#include "Ex05DoubleArray.h"
#include "Ex06QuadPerspectiveDraw.h"
#include "Ex07CubePerspectiveDraw.h"
#include "Ex08PhongDraw.h"

int main() 
{
	Window window{800,600, "Hello OpenGL"};

	float titleUpdateMaxTime =1.0f;
	float titleUpdateElapsed = 0.0f;

	//Ex01TriangleDraw Scene;
	//Ex02QuadDraw Scene;
	//Ex03QuadIndexDraw Scene;
	//Ex04QuadColorDraw Scene;
	//Ex05QuadTextureDraw Scene;
	//Ex05DoubleArray Scene;
	//Ex06QuadPerspectiveDraw Scene;
	//Ex07CubePerspectiveDraw Scene;
	Ex08PhongDraw Scene;
	Scene.Start();

	while (window.IsOpened())
	{
		float deltaTime = window.DeltaTime();
		titleUpdateElapsed += deltaTime;

		if (titleUpdateElapsed >= titleUpdateMaxTime)
		{
			int fps = static_cast<int>(1.0f / deltaTime);
			std::string titleString = std::format("OpenGLApp | DeltaTime: {} - FPS: {}", deltaTime, fps);
			window.SetTitle(titleString);
			titleUpdateElapsed -= titleUpdateMaxTime;
		}

		Scene.Update(deltaTime);

		window.Update();
	}
	
	// glfwDestroyWindow(window);
	Scene.Destroy();
	return 0;	
}