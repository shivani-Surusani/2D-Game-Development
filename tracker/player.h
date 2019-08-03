#ifndef PLAYER_H
#define PLAYER_H
#include<SDL.h>
#include<string>
#include<vector>
#include<cmath>
#include<list>

#include "drawable.h"
#include "smartSprite.h"

class ExplodingSprite;

class Player : public Drawable {
public:
	Player(const std::string&);
	Player(const Player&);
	Player& operator=(const Player&);
	virtual ~Player();

	virtual void draw() const;
	virtual void update( Uint32 ticks );

	void collided() {collision = true;}
	void missed() {collision = false;}

	virtual const Image* getImage() const { return images[currentFrame]; }
	int getScaledWidth() const { return getScale()*images[currentFrame]->getWidth();  }
	int getScaledHeight() const { return getScale()*images[currentFrame]->getHeight();  }
	const SDL_Surface* getSurface() const { return images[currentFrame]->getSurface(); }

	void right();
	void left();
	void up();
	void down();
	void jump();
	void stop();

	void attach( SmartSprite* o) {observers.emplace_back(o); }
	void detach (SmartSprite* o) ;

	void explode();

	bool isLookingRight() {return lookingRight;}

	void toggleGodMode() { godMode = !godMode; }
	bool isGod() {return godMode;}

protected:
	std::list<SmartSprite*> observers;

private:
	std::vector<Image *>images;
	ExplodingSprite* explosion;

	unsigned currentFrame;
	unsigned numberOfRightFrames;
	unsigned numberOfLeftFrames;
	unsigned frameInterval;
	float timeSinceLastFrame;
	int worldWidth;
	int worldHeight;
	
	bool collision;
	Vector2f initialVelocity;
	bool jumping;
	bool godMode;
	bool lookingRight;

	void advanceFrame(Uint32 ticks);

};
#endif
