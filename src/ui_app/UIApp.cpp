#include "UIApp.h"
#include "../src/lib/Reflection/Markup.h"
#include "../src/lib/Style/StyleManager.h"

static UIApp app;

void UIApp::Start(int argc, char * argv[])
{
	StyleManager::GetSingleton()->Load("style.xml");
	Markup::Load("test.xml");

	SetWindowTitle("UI Test application");
	//SetBackgroundColor(Color(255, 255, 255)); #default

}

void UIApp::Tick(float dt)
{
}
