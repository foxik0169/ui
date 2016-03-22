#pragma once

#include "../Types.h"
#include <map>
#include <functional>
#include <vector>
#include "pugixml\pugixml.hpp"

struct NVGcontext;
class Style;

class StyleManager
{
public:
	StyleManager(): fontsLoaded(false) {}
	~StyleManager();

	typedef std::function<void(Style *s, NVGcontext *context, pugi::xml_node instruction_node, float deltaTime, Rect bounds)> StyleInstructionDelegate;
	typedef std::map<std::string, Style*> StyleMap;

	void registerStyleInstruction(const char *name, StyleInstructionDelegate func);
	static StyleManager *GetSingleton();

	void Load(const char *filename);

	void ParseFonts(NVGcontext *context);
	Style *ReflectStyle(pugi::xml_node);

	bool AreFontsLoaded() { return fontsLoaded; }

	static bool isBinding(pugi::xml_attribute attr);
	static unsigned int getColor(pugi::xml_attribute attr);
		
private:
	typedef std::map<std::string, StyleInstructionDelegate> StyleInstructionMap;

	StyleInstructionMap instructionMap;
	StyleMap styleDefaults;

	std::vector<std::pair< pugi::xml_node , StyleManager::StyleInstructionDelegate>> parseInstructions(const pugi::xml_node &);

	void parseNomralState(pugi::xml_node style);

	static StyleManager *singleton;
	static void Init();

	pugi::xml_document document;

	bool fontsLoaded;

};