#include <SDL_image.h>
#include "ioMod.h"
#include "gameData.h"
#include "renderContext.h"

IoMod& IoMod::getInstance() {
  static IoMod instance;
  return instance;
}

IoMod::~IoMod() { 
  TTF_CloseFont(font);
  TTF_Quit(); 
}

//replacing arrow in getInstance()->getRenderer() to .
IoMod::IoMod() : 
  init(TTF_Init()),
  renderer( RenderContext::getInstance().getRenderer() ),
  font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file").c_str(),
                    Gamedata::getInstance().getXmlInt("font/size"))),
  textColor({0xff, 0, 0, 0}),
  colour({0, 0, 0xff, 0})
{
  if ( init == -1 ) {
    throw std::string("error: Couldn't init font");
  }
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  textColor.r = Gamedata::getInstance().getXmlInt("font/red");
  textColor.g = Gamedata::getInstance().getXmlInt("font/green");
  textColor.b = Gamedata::getInstance().getXmlInt("font/blue");
  textColor.a = Gamedata::getInstance().getXmlInt("font/alpha");
}

IoMod::IoMod(const IoMod& s) :
	init(s.init),
	renderer(s.renderer),
	font(s.font),
	textColor(s.textColor),
	colour(s.colour)
	{}

SDL_Texture* IoMod::readTexture(const std::string& filename) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename.c_str());
  if ( texture == NULL ) {
    throw std::string("Couldn't load ") + filename;
  }
  return texture;
}

SDL_Surface* IoMod::readSurface(const std::string& filename) {
  SDL_Surface *surface = IMG_Load(filename.c_str());
  if ( !surface ) {
    throw std::string("Couldn't load ") + filename;
  }
  return surface;
}

void IoMod::writeText(const std::string& msg, int x, int y) const {
  SDL_Surface* surface = 
    TTF_RenderText_Solid(font, msg.c_str(), textColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

//Overloading to include font color
void IoMod::writeText(const std::string& msg, int x, int y, SDL_Color colour) const {
  
  //Set Color
  //colour.r = Gamedata::getInstance().getXmlInt("font/red");
  //colour.g = Gamedata::getInstance().getXmlInt("font/green");
  //colour.b = Gamedata::getInstance().getXmlInt("font/blue");
  //colour.a = Gamedata::getInstance().getXmlInt("font/alpha");
  
  SDL_Surface* surface = TTF_RenderText_Solid(font, msg.c_str(), colour);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void IoMod::writeText(const std::string& msg, int x, int y, int wrapWidth, SDL_Color colour) const {
  
  //Set Color
  colour.r = Gamedata::getInstance().getXmlInt("font/red");
  colour.g = Gamedata::getInstance().getXmlInt("font/green");
  colour.b = Gamedata::getInstance().getXmlInt("font/blue");
  colour.a = Gamedata::getInstance().getXmlInt("font/alpha");
  
  //SDL_Surface* surface = TTF_RenderText_Solid(font, msg.c_str(), colour);
  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, msg.c_str(), colour, wrapWidth);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}
