#include <iostream>
#include <SDL.h>
#include "imageFactory.h"

class RenderContext{
public:
  //GOF TO mayer
  //static RenderContext* getInstance();
  static RenderContext& getInstance();
  ~RenderContext();
  SDL_Window* getWindow() const { return window; }
  SDL_Renderer* getRenderer() const { return renderer; }

private:
  //GOF TO mayer
  //static RenderContext& instance;
  SDL_Window* window;
  SDL_Renderer* renderer;

  //GOF TO mayer
  //ImageFactory* factory;
  ImageFactory& factory;

  SDL_Window* initWindow();
  SDL_Renderer* initRenderer();
  RenderContext();
  RenderContext(const RenderContext&) = delete;
  RenderContext& operator=(const RenderContext&) = delete;
};
