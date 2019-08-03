#ifndef SMARTSPRITE_H
#define SMARTSPRITE_H

#include "twoWayMultiSprite.h"
#include <string.h>

class ExplodingSprite;

class SmartSprite : public TwoWayMultiSprite {
public:
	SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
	SmartSprite(const SmartSprite&);
	SmartSprite& operator=(const SmartSprite&);	

	virtual void update(Uint32 ticks);
	virtual void draw() const;
	virtual void explode();
	void notify(const Vector2f& p) { playerPos = p;}

	bool killSprites() const { return spriteCollide;}
	virtual void collide();

private:
	enum MODE {NORMAL, EVADE};
	Vector2f playerPos;
	int playerWidth;
	int playerHeight;
	MODE currentMode;
	bool spriteCollide;
	ExplodingSprite* explosion;
	float safeDistance;
	
	void runLeft();
	void runRight();
	void runUp();
	void runDown();
};
#endif
