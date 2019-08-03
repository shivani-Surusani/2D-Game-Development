#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twoWayMultiSprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "hud.h"
#include "hudProj.h"
#include "collisionStrategy.h"

// [capture clause] (parameters) -> return-type {body}
auto Less = [](const Drawable* i, const Drawable* j) {
  return i->getScale() < j->getScale();
};

class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};



Engine::~Engine() { 
  for (auto sprite : sprites) {
  	delete sprite;
  }
  
  for (auto p : player) {
  	delete p;
  }
  
  for (auto z : witch) {
  	delete z;
  }
  
  for (CollisionStrategy* strategy : strategies) {
  	delete strategy;
  }
  
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  gdata(Gamedata::getInstance()),
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  greenSky("greenSky", Gamedata::getInstance().getXmlInt("greenSky/factor") ),
  greenClouds("greenClouds", Gamedata::getInstance().getXmlInt("greenClouds/factor") ),
  greenMountains("greenMountains", Gamedata::getInstance().getXmlInt("greenMountains/factor") ),
  greenBuildings("greenBuildings", Gamedata::getInstance().getXmlInt("greenBuildings/factor") ),
  greenBridge("greenBridge", Gamedata::getInstance().getXmlInt("greenBridge/factor") ),
  greenHouse("greenHouse", Gamedata::getInstance().getXmlInt("greenHouse/factor") ),
  greenGround("greenGround", Gamedata::getInstance().getXmlInt("greenGround/factor") ),
  viewport( Viewport::getInstance() ),
  strategies(std::vector<CollisionStrategy*> {}),
  currentStrategy(0),
  currentSprite(0),
  collision(false),
  sprites(),
  witch(),
  player(),
  colour({0, 0, 0xff, 0}),
  Life(3),
  //sound(),
  makeVideo( false ),
  gamehud(gameHud::getInstance()),
  hud(Hud::getInstance()),
  hudProj(HudProj::getInstance())
{
	//To unpause if it is paused initially
	clock.unpause();

    int n = gdata.getXmlInt("ghost/count"); //Number of ghosts
    int m = gdata.getXmlInt("zombie/count"); //Number of zombies
    int witchNum = gdata.getXmlInt("witch/count"); //Number of witchs
	
	for(int x = 0; x < witchNum; ++x) {
		TwoWayMultiSprite* painters = new TwoWayMultiSprite("witch");
		float sc = Gamedata::getInstance().getRandFloat(0.09,1);
		painters->setScale(sc);
		painters->setVelocityY(0);
		painters->setPosition( Vector2f(Gamedata::getInstance().getRandFloat(0, 800), Gamedata::getInstance().getRandFloat(0, 350) ) );
		witch.push_back(painters);	
	}
	sort(witch.begin(), witch.end(), Less);

	ShootingSprite* p = new ShootingSprite("knightWalk");
	p->setScale(0.10);
	player.push_back(p);	

   Vector2f pos = player[0]->getPosition();
   int w = player[0]->getScaledWidth();
   int h = player[0]->getScaledHeight();

  
   for (int i = 0; i < n; ++i) {
  	 sprites.push_back(new SmartSprite("ghost", pos, w, h) );
   }

   for (int i = 0; i < m; ++i) {
   		SmartSprite* s = new SmartSprite("zombie", pos, w, h);
   		sprites.push_back(s);
   		s->setVelocityY(0);
   		s->setScale(0.47);
   }

   for (unsigned int i = 0; i < sprites.size(); ++i) {
   		player[0]->attach(sprites[i]);
   }
   
   strategies.push_back( new PerPixelCollisionStrategy );
   strategies.push_back( new RectangularCollisionStrategy );
   strategies.push_back( new MidPointCollisionStrategy );
	
   Viewport::getInstance().setObjectToTrack(player[0]);
   std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
 
 // Pointer to draw witches at different layers
 //std::vector<TwoWayMultiSprite*>::const iterator witchPointer = witch.begin();
 auto witchPointer = witch.begin();
 
 //Background
 (*witchPointer)->draw(); //witch #1
 ++witchPointer;

 greenSky.draw();
 
// (*witchPointer)->draw(); //witch #2
// ++witchPointer;
 
 greenClouds.draw();
 
 (*witchPointer)->draw(); //witch #3
 ++witchPointer;
 
 greenMountains.draw();
 
 (*witchPointer)->draw(); //witch #4
 ++witchPointer;
 
 greenBuildings.draw();
 
 (*witchPointer)->draw(); //witch #5
 ++witchPointer;
 
 greenBridge.draw();
 greenHouse.draw();
 greenGround.draw();
	
 for (const Player* p : player) {
 	p->draw();
 }
/* 
 for (const TwoWayMultiSprite* wi : witch) {
 	wi->draw();
 }
*/


 for (const Drawable* sprite : sprites) {
 	sprite->draw();
 }
	
  strategies[currentStrategy]->draw();
  
  hud.draw();
  hudProj.draw(player[0]->getActiveProj(), player[0]->getFreeProj());

 std::stringstream strm;
 strm << "Enemies Remaining: " << sprites.size() << std::endl;
 io.writeText(strm.str(), 30, 500);

 std::stringstream gmode;
	if(player[0]->isGod()){
 		gmode << "God Mode ON " << std::endl;
	}
	else {
 		gmode << "God Mode OFF " << std::endl;
	}
 io.writeText(gmode.str(), 400, 500);
 
 std::stringstream lyf;
 lyf << "Lives Remaining: " << Life << std::endl;
 io.writeText(lyf.str(), 700, 500);
 
//Gameover Huds
if(Life < 1) {
	gamehud.draw(false);
	clock.pause();
}
else if(sprites.size() < 1) {
	gamehud.draw(true);
	clock.pause();
}


 viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {

  checkForCollisions();
  
  for (TwoWayMultiSprite* wi : witch) {
  	wi->update(ticks);
  }


  for (Player* p : player) {
  	p->update(ticks);
  }

//Trying to delete the collided sprite during update
	auto sp = sprites.begin();
	while(sp != sprites.end() ) {
		if( (*sp)->killSprites() ) {
			SmartSprite* doa = *sp;
			//delete doa;
			sp = sprites.erase(sp);
			(doa)->update(ticks);
		}
		else {
			(*sp)->update(ticks);
			++sp;
		}
	}

  greenSky.update();
  greenClouds.update();
  greenMountains.update();
  greenBuildings.update();
  greenBridge.update();
  greenHouse.update();
  greenGround.update();
 
  viewport.update(); // always update viewport last
}

void Engine::checkForCollisions() {
	collision = false;
	auto it = sprites.begin();
	while( it != sprites.end() ) {
	if( strategies[currentStrategy]->execute(*player[0], **it) ) {
		collision = true;
		player[0]->collided();
		if(player[0]->isGod()) {
			
			(*it)->explode();
			//sound->scream();
		}
		else {
			player[0]->explode();
			--Life;
		}
		++it;
	}
	
	else if( player[0]->collidedWith((*it)) ) {
		(*it)->explode();
		//sound->scream();
		player[0]->missed();
		collision = false;
	}
	else{
		++it;
		player[0]->missed();
		collision = false;
	}

	}
}

void Engine::switchSprite() {
	++currentSprite;
	currentSprite = currentSprite % (player.size());
    Viewport::getInstance().setObjectToTrack(player[currentSprite]);
	} 

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (currentStrategy + 1) % strategies.size();
        }
        if ( keystate[SDL_SCANCODE_E] ) {
			player[currentSprite]->explode();
		}
        if ( keystate[SDL_SCANCODE_F1] ) {
          hud.setON(!hud.isON());
        }
        if ( keystate[SDL_SCANCODE_F2] ) {
          hudProj.setON(!hudProj.isON());
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
	  	if(keystate[SDL_SCANCODE_SPACE]){
			player[0]->jump();
	  	}
	  	if(keystate[SDL_SCANCODE_LSHIFT]){
			player[0]->shoot();
			//sound->spear();
	  	}
	  	if(keystate[SDL_SCANCODE_R]){
			return true;
	  	}
	  	if(keystate[SDL_SCANCODE_G]){
			player[0]->toggleGodMode();
	  	}
		
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          //std::cout << "Terminating frame capture" << std::endl;
          //makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if( keystate[SDL_SCANCODE_A] ) {
	  	player[0]->left();	  
	  }
      if( keystate[SDL_SCANCODE_D] ) {
	  	player[0]->right();	  
	  }
	  /*
      if( keystate[SDL_SCANCODE_W] ) {
	  	player[0]->up();	 
	  }
      if( keystate[SDL_SCANCODE_S] ) {
	  	player[0]->down();	  
	  }
	  */
	  
	  draw();
      update(ticks);
      
	  if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
