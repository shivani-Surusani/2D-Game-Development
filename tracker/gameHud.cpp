#include <SDL_image.h>
#include <sstream>
#include <algorithm>

#include "gameHud.h"
#include "gameData.h"
#include "ioMod.h"

gameHud& gameHud::getInstance() {
	static gameHud instance;
	return instance;
}

gameHud::gameHud() :
	text(Gamedata::getInstance().getXmlStr("gameHUD/text")),
	width(Gamedata::getInstance().getXmlInt("gameHUD/width")),
	height(Gamedata::getInstance().getXmlInt("gameHUD/height")),
	pos(Vector2f(Gamedata::getInstance().getXmlInt("gameHUD/position/x"),
				 Gamedata::getInstance().getXmlInt("gameHUD/position/y"))),
	bgColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/backgroundColor/r")),
			 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/backgroundColor/g")),
			 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/backgroundColor/b")),
			 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/backgroundColor/a"))}),
	borderColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/borderColor/r")),
				 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/borderColor/g")),
				 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/borderColor/b")),
				 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/borderColor/a"))}),
	textColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/textColor/r")),
			   static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/textColor/g")),
			   static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/textColor/b")),
			   static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameHUD/textColor/a"))}),
	on(true)
	{}

void gameHud::draw(bool win) {
	if(isON()) {
		SDL_Rect r;
		r.x = getPosition()[0];
		r.y = getPosition()[1];
		r.w = getWidth();
		r.h = getHeight();

		SDL_Renderer* renderer = IoMod::getInstance().getRenderer();
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
		SDL_RenderFillRect(renderer, &r);
		SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
		SDL_RenderDrawRect(renderer,&r);
		
		setText(text);
		std::ostringstream hudTxt;
		if(win){
			hudTxt << "You Win! Press R for Restart" << std::endl;
		}
		else {
			hudTxt << "You Loose! Press R for Restart" << std::endl;
		}
		IoMod::getInstance().writeText(hudTxt.str(),
											  getPosition()[0]+3, 
											  getPosition()[1]+3,
											  getWidth(),
											  getTextColor());
	}	
}
