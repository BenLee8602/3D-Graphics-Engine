#include <windows.h>
#include <chrono>
#include "graphics.h"
using namespace bl;

int winSizeX = 1280;
int winSizeY = 720;
bool run = true;
BITMAPINFO bitmapInfo;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
	// window size changed
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);

		winSizeX = rect.right - rect.left;
		winSizeY = rect.bottom - rect.top;
		int bufferSize = winSizeX * winSizeY * sizeof(unsigned int);

		GraphicsBL::setBufferSize({ winSizeX, winSizeY });

		bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biWidth = winSizeX;
		bitmapInfo.bmiHeader.biHeight = winSizeY;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
	} break;

	// exit
	case WM_CLOSE: run = false; break;
	case WM_DESTROY: run = false; break;

	default: result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}


int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// register window class
	WNDCLASS winClass = {};
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpszClassName = "GraphicsBL";
	winClass.lpfnWndProc = WindowProc;
	RegisterClass(&winClass);

	// create window
	HWND window = CreateWindowA(
		winClass.lpszClassName,
		winClass.lpszClassName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		winSizeX, winSizeY, 0, 0,
		hInstance,
		0
	);
	HDC hdc = GetDC(window);

	// time
	std::chrono::steady_clock::time_point frameStart;
	std::chrono::steady_clock::time_point frameEnd;
	float dtime = 0.0f;

	// add objects and lighting to scene
	//GraphicsBL::addLight(new Light_Pt({ 255, 0, 0 }));
	//GraphicsBL::addEntity(new Entity("model/teapot.obj", ShadingType::VERTEX));

	//*  LIGHTING
	GraphicsBL::setAmbientLightColor({ 25, 25, 25 });
	GraphicsBL::addLight(new Light_Dir({ 255, 0, 255 }));

	const ShadingType st = ShadingType::PIXEL;
	GraphicsBL::addEntity(new Entity("model/teapot.obj", st, { -2.0, 0, 0 }, {  25,  25,  25 }));
	GraphicsBL::addEntity(new Entity("model/teapot.obj", st, { -1.5, 0, 0 }, { 255,  25,  25 }));
	GraphicsBL::addEntity(new Entity("model/teapot.obj", st, { -1.0, 0, 0 }, {  25, 255,  25 }));
	GraphicsBL::addEntity(new Entity("model/teapot.obj", st, { -0.5, 0, 0 }, {  25,  25, 255 }));
	GraphicsBL::addEntity(new Entity("model/teapot.obj", st, {  0.0, 0, 0 }, { 255, 255,  25 }));
	GraphicsBL::addEntity(new Entity("model/teapot.obj", st, {  0.5, 0, 0 }, { 255,  25, 255 }));
	GraphicsBL::addEntity(new Entity("model/teapot.obj", st, {  1.0, 0, 0 }, {  25, 255, 255 }));
	GraphicsBL::addEntity(new Entity("model/teapot.obj", st, {  1.5, 0, 0 }, { 255, 255, 255 }));
	//*/

	/*  MOUNTAINS
	GraphicsBL::setAmbientLightColor({ 100, 80, 100 });
	GraphicsBL::setBackgroundColor({ 150, 110, 110 });
	GraphicsBL::addLight(new Light_Dir({ 155, 40, 0 }, { -10, 1, 0 }));
	GraphicsBL::addEntity(new Entity("model/terrain3.obj", ShadingType::FLAT, { -64, 0, -64 }));
	GraphicsBL::addEffect(new Fog(20, 100, { 150, 110, 110 }, true));
	//*/

	/*  MOUNTAINS 2
	GraphicsBL::setAmbientLightColor({ 50, 50, 50 });
	GraphicsBL::addLight(new Light_Dir({ 100, 100, 100 }, { -3, 1, 0 }));
	GraphicsBL::addEffect(new Fog(20, 50, { 150, 50, 50 }, true));
	GraphicsBL::addEffect(new Fog(70, 100, { 150, 150, 50 }, true));
	GraphicsBL::addEntity(new Entity("model/terrain3.obj", ShadingType::FLAT, { -64, 0, -64 }, { 150, 50, 150 }));
	//*/
	
	// main loop
	while (run) {
		frameStart = std::chrono::steady_clock::now();
		
		// window message handling
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// render frame
		const void* output = GraphicsBL::renderFrame(dtime);

		// update frame
		StretchDIBits(
			hdc, 0, 0,
			winSizeX, winSizeY,
			0, 0,
			winSizeX, winSizeY,
			output, &bitmapInfo,
			DIB_RGB_COLORS, SRCCOPY
		);

		frameEnd = std::chrono::steady_clock::now();
		dtime = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart).count() / 1000000.0f;
	}

	return 0;
}
