#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "gameData.h"
#include "gameHud.h"
#include "hud.h"
#include "hudProj.h"
#include "smartSprite.h"
#include "shootingSprite.h"
#include "sound.h"

class Player;
class CollisionStrategy;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();

private:
  const Gamedata& gdata;
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock ;

  SDL_Renderer * const renderer;
  World greenSky;
  World greenClouds;
  World greenMountains;
  World greenBuildings;
  World greenBridge;
  World greenHouse;
  World greenGround;
  Viewport& viewport;

  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  int currentSprite;
  bool collision;
 
  std::vector<SmartSprite*> sprites;
  std::vector<TwoWayMultiSprite*> witch;
  std::vector<ShootingSprite*> player;
  //TwoWayMultiSprite* zombie;

  SDL_Color colour;
  int Life;
  //SDLSound sound;

  bool makeVideo;

  gameHud& gamehud;
  Hud& hud;
  HudProj& hudProj;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
};
