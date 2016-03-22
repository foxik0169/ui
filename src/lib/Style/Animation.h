#pragma once

#include "Types.h"
#include "pugixml\pugixml.hpp"

class Animation {
public:
	enum Function{
		F_Linear
	};

	Animation(pugi::xml_attribute attrib, double destValue, float time);
	Animation(pugi::xml_attribute attrib, unsigned int destValue, float time);

	~Animation() { /*free(debug);*/ }

	bool IsDone() { return isDone; }
	void Tick(float dt);
	pugi::xml_attribute GetAttribute() { return attribute; }

private:
	bool isColor;
	bool isDone;

	float current_time;
	float dest_time;

	//void *debug;

	union {
		double from_d;
		Color from_c;
	};

	union {
		double to_d;
		Color to_c;
	};


	pugi::xml_attribute attribute;

};