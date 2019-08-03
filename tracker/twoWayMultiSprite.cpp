#include "twoWayMultiSprite.h"
#include "gameData.h"
#include "imageFactory.h"

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) 
	{
		if(getVelocityX() > 0)	
    	{	
			currentFrame = numberOfRightFrames + (currentFrame+1) % numberOfRightFrames;
		}
		else
		{
			currentFrame = (currentFrame+1) % numberOfLeftFrames;
		}
			
	timeSinceLastFrame = 0;
	}

}

Vector2f TwoWayMultiSprite::makeVelocity(int vx, int vy) const {
	float newvx =  Gamedata::getInstance().getRandFloat(vx-50, vx+50);
	float newvy =  Gamedata::getInstance().getRandFloat(vy-50, vy+50);
	newvx *= [](){ if(rand()%2) return -1; else return 1; }();
	newvy *= [](){ if(rand()%2) return -1; else return 1; }();

	return Vector2f(newvx, newvy);
}

TwoWayMultiSprite::~TwoWayMultiSprite( ) { if ( explosion ) delete explosion; }

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  //images( ImageFactory::getInstance()->getImages(name) ),
  images( ImageFactory::getInstance().getImages(name) ),
  explosion(nullptr),
  currentFrame(0),
  numberOfRightFrames( Gamedata::getInstance().getXmlInt(name+"/rightframes") ),
  numberOfLeftFrames( Gamedata::getInstance().getXmlInt(name+"/leftframes") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s) :
  Drawable(s), 
  images(s.images),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfRightFrames( s.numberOfRightFrames ),
  numberOfLeftFrames( s.numberOfLeftFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  explosion = (s.explosion); 
  currentFrame = (s.currentFrame);
  numberOfRightFrames = ( s.numberOfRightFrames );
  numberOfLeftFrames = ( s.numberOfLeftFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void TwoWayMultiSprite::explode() {
	if(!explosion) {
		Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
		std::cout << "exploding From 2way : " << sprite.getName() << std::endl;
		sprite.setScale( getScale() );
		explosion = new ExplodingSprite(sprite);
	}
}

void TwoWayMultiSprite::draw() const { 
  if( explosion ) explosion->draw(); 
  else images[currentFrame]->draw(getX(), getY(), getScale());

}

void TwoWayMultiSprite::update(Uint32 ticks) { 
  if( explosion ) {
  	explosion->update(ticks);
	if( explosion->chunkCount() == 0 ) {
		delete explosion;
		explosion  = NULL;
	}
	return;
  }

  advanceFrame(ticks);
  
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
