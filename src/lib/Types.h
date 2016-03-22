#pragma once

/*
	
*/

float Clamp(float n, float min = 0.0f, float max = 1.0f);

struct Point {
	float x;
	float y;

	Point(float x, float y): x(x), y(y) {}
	Point(float xy): x(xy), y(xy) {}
	Point(): x(0), y(0) {}

	Point operator=(Point p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}

};

struct Rect {
	Point position;
	Point size;

	// Checks if given point is inside this rectangle
	bool ContainsPoint(Point point)
	{
		if (point.x > position.x && point.y > position.y && point.x < (position.x + size.x) && point.y < (position.y + size.y)) return true;
		return false;
	}
	bool ContainsPoint(float x, float y)
	{
		return ContainsPoint(Point(x, y));
	}

	Rect operator-(float value)
	{
		return{ {position.x + value, position.y + value}, {size.x - value*2, size.y - value*2} };
	}

	Rect(): position(), size() {}
	Rect(Point position, Point size) : position(position), size(size) {}

};

struct Color;

struct FColor {
	float r;
	float g;
	float b;
	float a;

	FColor(float red, float green, float blue, float alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	FColor(float red, float green, float blue)
	{
		r = red;
		g = green;
		b = blue;
		a = 1.0f;
	}

	FColor operator+(FColor &c)
	{
		float red = Clamp(c.r+r);
		float green = Clamp(c.g + g);
		float blue = Clamp(c.b + b);
		float alpha = Clamp(c.a + a);
		return{ red,green,blue,alpha };
	}
	FColor operator-(FColor &c)
	{
		float red = Clamp(c.r - r);
		float green = Clamp(c.g - g);
		float blue = Clamp(c.b - b);
		float alpha = Clamp(c.a - a);
		return{ red,green,blue,alpha };
	}
	FColor operator*(float scalar)
	{
		float red = Clamp(scalar * r);
		float green = Clamp(scalar * g);
		float blue = Clamp(scalar * b);
		float alpha = Clamp(scalar * a);
		return{ red,green,blue,alpha };
	}
	FColor operator/(float scalar)
	{
		float red = Clamp(scalar / r);
		float green = Clamp(scalar / g);
		float blue = Clamp(scalar / b);
		float alpha = Clamp(scalar / a);
		return{ red,green,blue,alpha };
	}

	FColor()
	{
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		a = 1.0f;
	}

	FColor(Color &c);
};

struct Color {
	
	union {
		unsigned int rgba;

		struct {
			unsigned char a;
			unsigned char b;
			unsigned char g;
			unsigned char r;
		};
	};

	Color(FColor &c);

	Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	Color(unsigned char red, unsigned char green, unsigned char blue)
	{
		r = red;
		g = green;
		b = blue;
		a = 255;
	}

	Color()
	{
		r = 255;
		g = 255;
		b = 255;
		a = 255;
	}

	Color(unsigned int hexaRGBA)
	{
		rgba = hexaRGBA;
	}

	static Color Color::Lerp(Color from, Color to, float amount);

	Color operator+(Color &c)
	{
		return Color(FColor(*this) + FColor(c));
	}

	Color operator-(Color &c)
	{
		return Color(FColor(*this) - FColor(c));
	}

	Color operator*(float scalar)
	{
		return Color(FColor(*this) * scalar);
	}

	Color operator/(float scalar)
	{
		return Color(FColor(*this) / scalar);
	}

	bool operator==(Color &c)
	{
		return (rgba == c.rgba);
	}

};

template<typename T>
T TLerp(T &from, T &to, float amount)
{
	return (from + (to - from) * amount);
}