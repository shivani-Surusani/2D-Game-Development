#ifndef GAMEHUD_H
#define GAMEHUD_H

#include <iostream>
#include <SDL.h>
#include <string>

#include "vector2f.h"

class gameHud {
public:	
	static gameHud& getInstance();
	
	const std::string& getText() const {return text; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	const Vector2f& getPosition() const {return pos; }
	SDL_Color getBgColor() const {return bgColor; }
	SDL_Color getBorderColor() const {return borderColor; }
	SDL_Color getTextColor() const {return textColor; }
	bool isON() const {return on; }

	void setText(const std::string& t) {text = t; }
	void setWidth(int w) {width = w; }
	void setHeight(int h) {height = h; }
	void setPosition(const Vector2f& p) {pos = p; }
	void setBgColor(const SDL_Color& bgc) {bgColor = bgc; }
	void setBorderColor(const SDL_Color& bc) {borderColor = bc; }
	void setTextColor(const SDL_Color& tc) {textColor = tc; }
	void setON(bool s) { on = s; }

	void draw(bool win);
private:
	std::string text;
	int width;
	int height;
	Vector2f pos;
	SDL_Color bgColor;
	SDL_Color borderColor;
	SDL_Color textColor;
	bool on;

	gameHud();
	gameHud(const gameHud&);
	gameHud& operator=(const gameHud&);

};
#endif
