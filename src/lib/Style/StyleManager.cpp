#include "PCH.h"
#include "Style.h"
#include "StyleManager.h"
#include <sstream>
#include "Application.h"

StyleManager* StyleManager::singleton = nullptr;

StyleManager * StyleManager::GetSingleton()
{
	if (!singleton) Init();
	return singleton;
}

StyleManager::~StyleManager()
{
	for (auto style : styleDefaults) delete style.second;
}

void StyleManager::Load(const char * filename)
{
	document.load_file(filename);

	std::vector<std::pair<pugi::xml_node, StyleManager::StyleInstructionDelegate>> instructions;

	if (auto a = Application::GetSingleton())
	{
		a->SetBackgroundColor(Color(document.child("app").attribute("bg").as_uint()));
	}

	for (auto style : document.children("style"))
	{
		const char *name = style.attribute("name").as_string();
		const char *element = style.attribute("element").as_string();

		parseNomralState(style);

		instructions = parseInstructions(style);
		styleDefaults.insert(std::make_pair(element, new Style(instructions, style)));
	}
}

Style *StyleManager::ReflectStyle(pugi::xml_node node)
{
	StyleManager *ins = StyleManager::GetSingleton();
	StyleMap::iterator wpmi = ins->styleDefaults.find(node.name());

	if (wpmi != ins->styleDefaults.end())
	{
		Style *s = new Style(*wpmi->second);
		s->SetNode(node);
		return s;
	}

	return nullptr;
}

bool StyleManager::isBinding(pugi::xml_attribute attr)
{
	return attr.as_string()[0] == '@';
}

unsigned int StyleManager::getColor(pugi::xml_attribute attr)
{
	if (attr.as_string()[0] == '#')
	{

	}
	return 0;
}

std::vector<std::pair< pugi::xml_node , StyleManager::StyleInstructionDelegate>> StyleManager::parseInstructions(const pugi::xml_node &style_node)
{
	std::vector<std::pair< pugi::xml_node , StyleManager::StyleInstructionDelegate>> out;

	for (pugi::xml_node instruction = style_node.child("visual").first_child(); instruction; instruction = instruction.next_sibling())
	{
		StyleInstructionMap::iterator d = instructionMap.find(std::string(instruction.name()));
		if (d != instructionMap.end())
		{
			out.push_back(std::make_pair(instruction, d->second));
		}
	}

	return out;
}

void StyleManager::parseNomralState(pugi::xml_node style)
{
	for (auto attr : style.children("attribute"))
	{
		if (attr.attribute("public").as_bool()) continue;
		style.child("normal").append_child(attr.attribute("name").as_string()).append_attribute("value").set_value(attr.attribute("default").value());
	}
}

void StyleManager::ParseFonts(NVGcontext *context)
{
	for (pugi::xml_node font : document.children("font"))
	{
		const char *fontName = font.attribute("name").as_string();
		const char *fontFile = font.attribute("file").as_string();
		int font = nvgCreateFont(context, fontName, fontFile);

		if (font < 0) printf("Font couldn't been loaded!");
		else printf("Font '%s' loaded.\n", fontName);
	}
	fontsLoaded = true;
}

void StyleManager::Init()
{
	singleton = new StyleManager();
	
	singleton->registerStyleInstruction("rounded-rect", [](Style *s, NVGcontext *context, pugi::xml_node instruction_element, float dt, Rect bounds) {
		// delegate for rounded rectangle

		unsigned int fill_c = s->GetUIntValue(instruction_element.attribute("fill-color"));
		unsigned int border_c = s->GetUIntValue(instruction_element.attribute("border-color"));

		unsigned int border_width = instruction_element.attribute("border-width").as_uint();
		unsigned int radius = instruction_element.attribute("radius").as_uint();

		Color f_color(fill_c);
		Color b_color(border_c);

		nvgBeginPath(context);
		nvgRoundedRect(context, bounds.position.x + border_width/2, bounds.position.y + border_width / 2, bounds.size.x - border_width, bounds.size.y - border_width, radius);
	
		nvgFillColor(context, nvgRGB(f_color.r, f_color.g, f_color.b));
		nvgFill(context);

		if (border_width > 0)
		{
			nvgStrokeWidth(context, border_width);
			nvgStrokeColor(context, nvgRGB(b_color.r, b_color.g, b_color.b));
			nvgStroke(context);
		}
		nvgClosePath(context);
	
	});

	singleton->registerStyleInstruction("text", [](Style *s, NVGcontext *context, pugi::xml_node instruction_element, float dt, Rect bounds) {
		// delegate for text
		// default behaviour -> center in parent

		unsigned int t_color = s->GetUIntValue(instruction_element.attribute("color"));
		unsigned int f_size = s->GetUIntValue(instruction_element.attribute("size"));
		const char *text = s->GetStringValue(instruction_element.attribute("value"));
		const char *font = instruction_element.attribute("font").as_string("default");

		Color text_color(t_color);
		
		nvgFontFace(context, font);

		nvgFontSize(context, f_size);
		nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		
		nvgFillColor(context, nvgRGB(text_color.r, text_color.g, text_color.b));

		// xmin, ymin, xmax, ymax
		float tbounds[4];
		nvgTextBounds(context, bounds.position.x, bounds.position.y, text, nullptr, tbounds);
		
		float tWidth = tbounds[2] - tbounds[0];
		float tHeight = tbounds[3] - tbounds[1];

		nvgText(context, bounds.position.x + (bounds.size.x / 2 - tWidth/2), bounds.position.y + (bounds.size.y / 2), text, nullptr);

	});

}

void StyleManager::registerStyleInstruction(const char *name, StyleInstructionDelegate func)
{
	_ASSERTE(instructionMap.find(name) == instructionMap.end());
	instructionMap.insert(std::make_pair(name, func));
}