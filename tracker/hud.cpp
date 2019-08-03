#include <SDL_image.h>
#include <sstream>
#include <algorithm>

#include "hud.h"
#include "gameData.h"
#include "ioMod.h"

Hud& Hud::getInstance() {
	static Hud instance;
	return instance;
}

Hud::Hud() :
	text(Gamedata::getInstance().getXmlStr("HUD/text")),
	width(Gamedata::getInstance().getXmlInt("HUD/width")),
	height(Gamedata::getInstance().getXmlInt("HUD/height")),
	pos(Vector2f(Gamedata::getInstance().getXmlInt("HUD/position/x"),
				 Gamedata::getInstance().getXmlInt("HUD/position/y"))),
	bgColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backgroundColor/r")),
			 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backgroundColor/g")),
			 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backgroundColor/b")),
			 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backgroundColor/a"))}),
	borderColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/borderColor/r")),
				 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/borderColor/g")),
				 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/borderColor/b")),
				 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/borderColor/a"))}),
	textColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/textColor/r")),
			   static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/textColor/g")),
			   static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/textColor/b")),
			   static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/textColor/a"))}),
	on(true),
	clock(Clock::getInstance() ),
	msgFPSPos(Vector2f(Gamedata::getInstance().getXmlInt("viewFPS/loc/x"),Gamedata::getInstance().getXmlInt("viewFPS/loc/y") ))
	{}

void Hud::draw() {
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
		hudTxt << getText();
		IoMod::getInstance().writeText(hudTxt.str(),
											  getPosition()[0]+3, 
											  getPosition()[1]+3,
											  getWidth(),
											  getTextColor());

		std::ostringstream FpsString;
		FpsString << "FPS: " << Clock::getInstance().getFps() << std::endl;
		IoMod::getInstance().writeText(FpsString.str(),
											  getPosition()[0]+3, 
											  getPosition()[1]+30,
											  getWidth(),
											  getTextColor());
		
		
		
		IoMod::getInstance().writeText("LSHIFT -> Shoot",
											  getPosition()[0]+3, 
											  getPosition()[1]+60,
											  getWidth(),
											  getTextColor());
	
	
		IoMod::getInstance().writeText("Space -> Jump",
											  getPosition()[0]+3, 
											  getPosition()[1]+90,
											  getWidth(),
											  getTextColor());
	
	
		IoMod::getInstance().writeText("F1 -> Toggle HUD",
											  getPosition()[0]+3, 
											  getPosition()[1]+120,
											  getWidth(),
											  getTextColor());

	
		IoMod::getInstance().writeText("F2 -> Toggle Object pool HUD",
											  getPosition()[0]+3, 
											  getPosition()[1]+150,
											  getWidth(),
											  getTextColor());
		
		IoMod::getInstance().writeText("G -> Toggle God Mode",
											  getPosition()[0]+3, 
											  getPosition()[1]+180,
											  getWidth(),
											  getTextColor());
		
		IoMod::getInstance().writeText("Instructions: Avoid Enemies and beware of Exploding Sprites. Kill all enemies to WIN",
											  getPosition()[0]+3, 
											  getPosition()[1]+220,
											  getWidth(),
											  getTextColor());
	}	
}
