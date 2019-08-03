#include <SDL_image.h>
#include <sstream>
#include <algorithm>
#include <string>

#include "hudProj.h"
#include "gameData.h"
#include "ioMod.h"

HudProj& HudProj::getInstance() {
	static HudProj instance;
	return instance;
}

HudProj::HudProj() :
	width(Gamedata::getInstance().getXmlInt("HudProj/width")),
	height(Gamedata::getInstance().getXmlInt("HudProj/height")),
	pos(Vector2f(Gamedata::getInstance().getXmlInt("HudProj/position/x"),
				 Gamedata::getInstance().getXmlInt("HudProj/position/y"))),
	on(true),
	bgColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/backgroundColor/r")),
			 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/backgroundColor/g")),
			 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/backgroundColor/b")),
			 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/backgroundColor/a"))}),
	borderColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/borderColor/r")),
				 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/borderColor/g")),
				 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/borderColor/b")),
				 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/borderColor/a"))}),
	textColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/textColor/r")),
			   static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/textColor/g")),
			   static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/textColor/b")),
			   static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HudProj/textColor/a"))})
	{}

void HudProj::draw(int activeProj, int freeProj) {
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

		std::ostringstream hudTxt;
		hudTxt << "Object Pooling" << std::endl;
		IoMod::getInstance().writeText(hudTxt.str(),
											  getPosition()[0]+3, 
											  getPosition()[1]+3,
											  getWidth(),
											  getTextColor());

		std::ostringstream active;
		active << "Active Projectiles: " << activeProj << std::endl;
		IoMod::getInstance().writeText(active.str(),
											  getPosition()[0]+3, 
											  getPosition()[1]+50,
											  getWidth(),
											  getTextColor());
		
		std::ostringstream free;
		free<< "Free Projectiles: " << freeProj << std::endl;
		IoMod::getInstance().writeText(free.str(),
											  getPosition()[0]+3, 
											  getPosition()[1]+100,
											  getWidth(),
											  getTextColor());
		}	
}
