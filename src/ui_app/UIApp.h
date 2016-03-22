#pragma once

#include <stdio.h>
#include "../src/lib/Application.h"

class UIApp : public Application
{
public:

	virtual void Start(int argc, char * argv[]) override;
	virtual void Tick(float dt);

	int count = 0;

	typedef Application Super;
};
