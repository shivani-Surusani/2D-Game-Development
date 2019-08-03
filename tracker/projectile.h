#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <iostream>

#include "twoWayMultiSprite.h"
#include "gameData.h"
#include "vector2f.h"

class TwoWayMultiSprite;

class Projectile : public TwoWayMultiSprite {
public:
	Projectile(const string& name);
	Projectile(const Projectile& p);

	//virtual void update(Uint32 ticks);
	void collide() {setPosition(Vector2f(-200, -200)); setVelocity(Vector2f(0, 0)); }
	bool isTooFar() const { return tooFar; }
	void setStartingPos(const Vector2f& v) {startingPos = v; }
	void reset() {
		tooFar = false;
		distance = 0;
	}

private:
	Vector2f startingPos; 
	float distance; 
	float maxDistance;
	bool tooFar;
};

#endif
