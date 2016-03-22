/*
	UI Application
	--------------


	@FoXik0169
*/

#pragma once

#include "Types.h"
#include "Widget.h"

struct GLFWwindow;
struct NVGcontext;

/*
	Extend this class to create new application.
*/
class Application
{
public:

	Application();
	~Application();

	/*
		Implement application initialization here.
		eg. load window layout, display widgets, etc...
	*/
	virtual void Start(int argc, char *argv[]);

	virtual void Tick(float dt);

	/*
		Implement application key events here.
		eg. shortcuts...
	*/
	virtual void OnKeyPress();
	virtual void OnKeyRelease();

	/*
		Implement any calculation based upon new window size
	*/
	virtual void OnWindowSize(int width, int height);

	int Run(int argc, char* argv[]);

	static Application *GetSingleton() { 
		return _singleton; 
	};

	void SetBackgroundColor(Color background);
	void SetWindowTitle(const char *title);

protected:

	double mouseX, mouseY;
	int winSizeX, winSizeY;
	int fbSizeX, fbSizeY;
	float pxRatio;

	Color clearColor;

private: 

	static Application *_singleton;

	bool _init();

	double lastTime;
	double fps;

	/*
		Low-level applicatiion events.
	*/
	void _tick(float dt);
	void _clear();
	void _viewport();

	static void _onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void _onCursor(GLFWwindow* window, double xpos, double ypos);
	static void _onWindowSize(GLFWwindow* window, int width, int height);
	static void _onWindowPos(GLFWwindow* window, int x, int y);

	GLFWwindow *_w;
	NVGcontext *_vg;

};