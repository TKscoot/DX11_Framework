#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <sstream>

#include "Window.h"
#include "D3D.h"
#include "Application.h"
#include "Timer.h"


Window window = {};
D3D d3d = {};
Application app = {};

Timer frameTimer = {};
Timer debugTimer = {};

double deltaTime = 0.0;

void MessageLoop();

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmd)
{
	debugTimer.Start();

	// Create Window 
	window.Initialize(hInstance, nCmd);

	// Initialize DirectX
	d3d.Initialize(hInstance, window);

	app.Initialize(&d3d, &window);

	MessageLoop();

	d3d.Finalize();

	return 0;
}

void MessageLoop() 
{
	MSG msg = {};

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			frameTimer.Start();

			// Game Loop...
			app.Update(deltaTime);
			app.Render(deltaTime);


			if (debugTimer.GetElapsedMilliseconds() > 1000.0)
			{
				debugTimer.Stop();

				std::stringstream titleStringStream;
				titleStringStream << "DirectX11 Framework! DeltaTime: ";
				titleStringStream << std::to_string(deltaTime);
				titleStringStream << " FPS: ";
				titleStringStream << std::to_string((int)(1 / deltaTime * 1000));

				window.SetWindowTitle(titleStringStream.str());

				debugTimer.Start();
			}
			

			frameTimer.Stop();

			deltaTime = frameTimer.GetElapsedMilliseconds();
		}
	}
}