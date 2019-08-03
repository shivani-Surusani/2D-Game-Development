#include "shootingSprite.h"
#include "player.h"
#include "gameData.h"

ShootingSprite::ShootingSprite(const std::string& name) :
  Player(name),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets( bulletName ),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  worldWidth( Gamedata::getInstance().getXmlInt("world/width") ),
  worldHeight( Gamedata::getInstance().getXmlInt("world/height") )
{ }

ShootingSprite::ShootingSprite(const ShootingSprite& s) :
  Player(s),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight)
{ }

/*
ShootingSprite& ShootingSprite::operator=(const ShootingSprite& s) {
	ShootingSprite::operator=(s);
	bulletName = (s.bulletName);
	bullets = (s.bullets);
	minSpeed = (s.minSpeed);
	worldWidth = (s.worldWidth);
	worldHeight = (s.worldHeight);
}
*/
void ShootingSprite::shoot() { 
  // I'm not adding minSpeed to y velocity:
  if(isLookingRight()) {
  	float x = getX()+getScaledWidth();
  	float y = getY()+getScaledHeight()/2;
	bullets.shoot( Vector2f(x, y), Vector2f((minSpeed+getVelocityX()), 0));
  }
  else {
  	float x = getX() - getScaledWidth();
  	float y = getY()+getScaledHeight()/2;
	bullets.shoot( Vector2f(x, y), Vector2f(-(minSpeed+getVelocityX()), 0));
  }
}

bool ShootingSprite::collidedWith(const Drawable* obj) const {
  return bullets.collidedWith( obj );
}

void ShootingSprite::draw() const { 
  Player::draw();
  bullets.draw();
}

void ShootingSprite::update(Uint32 ticks) { 
  Player::update(ticks);
  bullets.update(ticks);
}

bool ShootingSprite::shooting() {
	return bullets.shooting();
}

std::list<Bullet> ShootingSprite::getBulletList() {
	return bullets.getBulletList();
}

int ShootingSprite:: getActiveProj() {
	return bullets.bulletCount();
}

int ShootingSprite:: getFreeProj() {
	return bullets.freeCount();
}
