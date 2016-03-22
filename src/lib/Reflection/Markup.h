#pragma once

#include "../types.h"
#include <map>

struct NVGcontext;
class Widget;

namespace pugi {
	class xml_node;
	class xml_document;
}

/*
	Interface for Markup manipulation
*/
class Markup
{
public:

	static void Load(const char* filename);

	static void Draw(NVGcontext *context, int width, int height, float pxRatio, float deltaTime);
	static void Tick(float deltaTime);

	static void SizeChanged(int width, int height);
	static void CursorMoved(double x, double y);

	static void AddWidget(const char *parent_id, const char* type, std::map<std::string, std::string> attributes = std::map<std::string, std::string>());

	static const pugi::xml_document& GetDocument();

private:
	static pugi::xml_document document;
	static pugi::xml_node root;

	static Point size;

	static bool dirty;
};