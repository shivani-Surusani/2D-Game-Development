#ifndef HUDPROJ_H
#define HUDPROJ_H

#include <iostream>
#include <SDL.h>
#include <string>

#include "vector2f.h"

class HudProj {
public:	
	static HudProj& getInstance();
	
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	const Vector2f& getPosition() const {return pos; }
	SDL_Color getBgColor() const {return bgColor; }
	SDL_Color getBorderColor() const {return borderColor; }
	SDL_Color getTextColor() const {return textColor; }
	bool isON() const {return on; }

	void setWidth(int w) {width = w; }
	void setHeight(int h) {height = h; }
	void setPosition(const Vector2f& p) {pos = p; }
	void setBgColor(const SDL_Color& bgc) {bgColor = bgc; }
	void setBorderColor(const SDL_Color& bc) {borderColor = bc; }
	void setTextColor(const SDL_Color& tc) {textColor = tc; }
	void setON(bool s) { on = s; }

	void draw(int, int);
private:
	int width;
	int height;
	Vector2f pos;
	bool on;
	SDL_Color bgColor;
	SDL_Color borderColor;
	SDL_Color textColor;

	HudProj();
	HudProj(const HudProj&);
	HudProj& operator=(const HudProj&);

};
#endif
