#include <iostream>
#include <cmath>
#include "bullet.h"
#include "collisionStrategy.h"

void Bullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  if(getVelocityX() > worldWidth - getScaledWidth()) {
  		setVelocityX(fabs(getVelocityX()));	
  }
  if(getVelocityX() < 0 ){
  		setVelocityX(-fabs(getVelocityX()));	
  }

  TwoWayMultiSprite::update(ticks);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
}

