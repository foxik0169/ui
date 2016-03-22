#include "PCH.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg\nanovg_gl.h"
#include "Application.h"
#include "pugixml\pugixml.hpp"
#include "Reflection\WidgetManager.h"
#include "Reflection\Markup.h"
#include "StyleManager.h"

Application *Application::_singleton = nullptr;

int main(int argc, char* argv[])
{
	return Application::GetSingleton()->Run(argc, argv);
}

Application::Application()
{
	_singleton = this;
}

Application::~Application()
{
}

void Application::Start(int argc, char * argv[])
{
	printf("Application start\n");
}

void Application::Tick(float dt)
{
}

void Application::OnKeyPress()
{
}

void Application::OnKeyRelease()
{
}

void Application::OnWindowSize(int width, int height)
{
}

int Application::Run(int argc, char* argv[])
{
	_init();

	SetWindowTitle("UI App");

	Start(argc, argv);
	
	glfwSwapInterval(1);

	_viewport();
	Markup::SizeChanged(winSizeX, winSizeY);
	
	StyleManager::GetSingleton()->ParseFonts(_vg);

	while (!glfwWindowShouldClose(_w))
	{
		// count delta and fps
		double dt, t;

		t = glfwGetTime();
		dt = t - lastTime;
		lastTime = t;

		fps = 1.0 / dt;

		printf("Current fps: %lf\r", fps);

		// loop base		
		_viewport();

		_clear();

		_tick(dt);

		glfwSwapBuffers(_w);
		glfwPollEvents();
	}

	return 0;
}

void Application::SetBackgroundColor(Color background)
{
	clearColor = background;
}

void Application::SetWindowTitle(const char * title)
{
	glfwSetWindowTitle(_w, title);
}

bool Application::_init()
{
	if (!glfwInit())
	{
		// handle error -> backend not created
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfwWindowHint(GLFW_SAMPLES, 4);

	_w = glfwCreateWindow(1000, 600, "UIApp", NULL, NULL);
	if (!_w) {
		// handle error -> couldn't create window
	}

	glfwSetKeyCallback(_w, _onKey);
	glfwSetCursorPosCallback(_w, _onCursor);
	glfwSetWindowSizeCallback(_w, _onWindowSize);
	glfwSetWindowPosCallback(_w, _onWindowPos);

	glfwMakeContextCurrent(_w);
#ifdef NANOVG_GLEW
	if (glewInit() != GLEW_OK) {
		// handle error -> glew couldn't been initialized
	}
#endif

	_vg = nvgCreateGL2(NVG_STENCIL_STROKES);
	

	if (_vg == NULL) {
		// handle error -> nanovg couldn't been initialized
	}

	glfwSetTime(0);

	lastTime = glfwGetTime();

	return true;
}

void Application::_tick(float dt)
{
	// draw here
	Tick(dt);
	Markup::Tick(dt);
	Markup::Draw(_vg, winSizeX, winSizeY, pxRatio, dt);

}

void Application::_clear()
{
	FColor nClearColor(clearColor);
	glClearColor(nClearColor.r, nClearColor.g, nClearColor.b, nClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Application::_viewport()
{
	glfwGetCursorPos(_w, &mouseX, &mouseY);
	glfwGetWindowSize(_w, &winSizeX, &winSizeY);
	glfwGetFramebufferSize(_w, &fbSizeX, &fbSizeY);

	pxRatio = (float)fbSizeX / (float)winSizeX;

	glViewport(0, 0, fbSizeX, fbSizeY);
}

void Application::_onKey(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	// handle keys
}

void Application::_onCursor(GLFWwindow * window, double xpos, double ypos)
{
	// handle cursor
	Markup::CursorMoved(xpos, ypos);
}

void Application::_onWindowSize(GLFWwindow * window, int width, int height)
{
	glfwSwapInterval(0);

	Application::GetSingleton()->_viewport();
	Application::GetSingleton()->_clear();
	Application::GetSingleton()->_tick(0);
	glfwSwapBuffers(Application::GetSingleton()->_w);

	Application::GetSingleton()->OnWindowSize(width, height);
	Markup::SizeChanged(width, height);

	glfwSwapInterval(1);
}

void Application::_onWindowPos(GLFWwindow * window, int x, int y)
{

	
	glfwSwapInterval(0);

	Application::GetSingleton()->_viewport();
	Application::GetSingleton()->_clear();
	Application::GetSingleton()->_tick(0);
	glfwSwapBuffers(Application::GetSingleton()->_w);

	int winSizeX = Application::GetSingleton()->winSizeX;
	int winSizeY = Application::GetSingleton()->winSizeY;

	Application::GetSingleton()->OnWindowSize(winSizeX, winSizeY);
	Markup::SizeChanged(winSizeX, winSizeY);

	glfwSwapInterval(1);
}
