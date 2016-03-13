#include <stdio.h>

#ifdef NANOVG_GLEW
#  include <GL/glew.h>
#endif

#include <GLFW\glfw3.h>
#include "nanovg\nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg\nanovg_gl.h"

NVGcontext* vg = NULL;
int font = -1;

static void do_draw(GLFWwindow* window, NVGcontext *vg, int font)
{
	double mx, my, t, dt;
	int winWidth, winHeight;
	int fbWidth, fbHeight;
	float pxRatio;

	glfwGetCursorPos(window, &mx, &my);
	glfwGetWindowSize(window, &winWidth, &winHeight);
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	pxRatio = (float)fbWidth / (float)winWidth;

	glViewport(0, 0, fbWidth, fbHeight);

	glClearColor(0.4f, 0.3f, 0.32f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, mx, my, 30, 30, 5);
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 100));
	nvgFill(vg);

	nvgSave(vg);

	nvgBeginPath(vg);
	nvgFontSize(vg, 18.0f);
	nvgFontFaceId(vg, font);
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

	nvgText(vg, 20, 20, "UI App", nullptr);
	nvgFillColor(vg, nvgRGBA(235, 220, 180, 255));
	nvgFill(vg);

	nvgRestore(vg);

	nvgEndFrame(vg);
}

static void on_key(GLFWwindow* w, int key, int scancode, int action, int mods)
{

}

static void on_mouse(GLFWwindow* w, double x, double y)
{

}

static void on_window(GLFWwindow* w, int x, int y)
{
	int fbWidth, fbHeight;

	if (vg != NULL)
	{
		glfwGetFramebufferSize(w, &fbWidth, &fbHeight);
		glViewport(0, 0, fbWidth, fbHeight);

		do_draw(w, vg, font);

		glfwSwapBuffers(w);
	}
}

int main(int argc, char* argv[])
{
	printf("nanovg debug application\n");
	GLFWwindow* window;

	if (!glfwInit()) {
		printf("Failed to init GLFW.");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1000, 600, "UIApp", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// event callbacks
	glfwSetKeyCallback(window, on_key);
	glfwSetCursorPosCallback(window, on_mouse);
	glfwSetWindowSizeCallback(window, on_window);

	glfwMakeContextCurrent(window);
#ifdef NANOVG_GLEW
	if (glewInit() != GLEW_OK) {
		printf("Could not init glew.\n");
		return -1;
	}
#endif

	vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

	if (vg == NULL) {
		printf("Could not init nanovg.\n");
		return -1;
	}

	glfwSwapInterval(0);

	glfwSetTime(0);

	font = nvgCreateFont(vg, "default", "pcf.ttf");
	if (font == -1)
	{
		printf("Failed to load font!\n");
	}

	while (!glfwWindowShouldClose(window))
	{
		do_draw(window, vg, font);

		glfwSwapBuffers(window);
		glfwWaitEvents();
	}

	nvgDeleteGL2(vg);

	glfwTerminate();
	return 0;
}