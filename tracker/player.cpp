#include "player.h"
#include "gameData.h"
#include "imageFactory.h"

Player::~Player( ) { if(explosion) delete explosion; }

Player::Player( const std::string& name):
	Drawable(name,
			Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
					 Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
			Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
					 Gamedata::getInstance().getXmlInt(name+"/speedY"))
			),
	observers(std::list<SmartSprite*> {}),
	images( ImageFactory::getInstance().getImages(name) ),
    explosion(nullptr),	
	currentFrame(0),
	numberOfRightFrames( Gamedata::getInstance().getXmlInt(name+"/leftframes") ),
	numberOfLeftFrames( Gamedata::getInstance().getXmlInt(name+"/rightframes") ),
	frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
	timeSinceLastFrame(0),
	worldWidth( Gamedata::getInstance().getXmlInt("world/width") ),
	worldHeight( Gamedata::getInstance().getXmlInt("world/height") ),
	collision(false),
	initialVelocity( getVelocity() ),
	jumping(false),
	godMode(false),
	lookingRight(true)
	{ setVelocityY( 0 ); }

Player::Player(const Player& s) :
	Drawable(s),
	observers(std::list<SmartSprite*> {}),
	images(s.images),
	explosion(s.explosion),
	currentFrame(s.currentFrame),
	numberOfRightFrames(s.numberOfRightFrames),
	numberOfLeftFrames(s.numberOfLeftFrames),
	frameInterval(s.frameInterval),
	timeSinceLastFrame(s.timeSinceLastFrame),
	worldWidth(s.worldWidth),
	worldHeight(s.worldHeight),
	collision(s.collision),
	initialVelocity(s.initialVelocity),
	jumping(s.jumping),
	godMode(s.godMode),
	lookingRight(s.lookingRight)
	{ setVelocityY(0); }

Player& Player::operator=(const Player& s) {
	Drawable::operator=(s);
	observers = s.observers;
	images = (s.images);
	explosion = (s.explosion);
	currentFrame = (s.currentFrame);
	numberOfRightFrames = (s.numberOfRightFrames);
	numberOfLeftFrames = (s.numberOfLeftFrames);
	frameInterval = (s.frameInterval);
	timeSinceLastFrame = (s.timeSinceLastFrame);
	worldWidth = (s.worldWidth);
	worldHeight = (s.worldHeight);
	collision = s.collision;
	initialVelocity = (s.initialVelocity);
	jumping = (s.jumping);
	godMode = (s.godMode);
	lookingRight = (s.lookingRight);
	return *this;
}

void Player::explode() {
	if(!explosion) {
	Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
	sprite.setScale( getScale() );
	explosion = new ExplodingSprite(sprite);
	}
}

void Player::draw() const {
	if(explosion) explosion->draw();
	else images[currentFrame]-> draw(getX(), getY(), getScale());
}


void Player::stop() {
	setVelocityX(0);
	setVelocityY(0);
}

void Player::right() {
	if( getX() < worldWidth - getScaledWidth()) {
		setVelocityX(initialVelocity[0]);
		currentFrame = (currentFrame) % numberOfRightFrames;
		lookingRight = true;
	}
}

void Player::left() {
	if( getX() > 0 ) {
		setVelocityX( -initialVelocity[0] );
		currentFrame = numberOfRightFrames + (currentFrame) % numberOfLeftFrames;
		lookingRight = false;
	}
}

void Player::down() {
	//if ( getY() < worldHeight- getScaledHeight() ) {
	//std::cout << "Y pos is: " << getY() << std::endl;
	if ( getY() < 380) {
		setVelocityY( initialVelocity[1]);
	}
}

void Player::up() {
	if( getY() > 0 ) {
		setVelocityY( -initialVelocity[1] );		
	}
}

void Player::jump() {
	if(!jumping){
		setVelocityY( -initialVelocity[1] );
		jumping = true;
	}
}

void Player::update(Uint32 ticks) {
	if(explosion) {
		explosion->update(ticks);
		if(explosion->chunkCount() == 0) {
			delete explosion;
			explosion = nullptr;
		}
		return;
	}

	std::list<SmartSprite*>::iterator o = observers.begin();
	while( o != observers.end() ) {
		(*o)->notify( getPosition() );
		++o;
	}
	
	if(!collision) {
		advanceFrame(ticks);
	}

	Vector2f incr = getVelocity() * static_cast<float>(ticks) *  0.001;
	setPosition(getPosition() + incr); 

	if( getY() < 0 ) {
		setVelocityY( -fabs(getVelocityY()) );
	}

	if( getY() > worldHeight - getScaledHeight() ) {
		setVelocityY( fabs(getVelocityY()) );
	} 

	if( getX() < 0 ) {
		setVelocityX( -fabs(getVelocityX()) );
	}

	if( getX() > worldWidth - getScaledWidth() ) {
		setVelocityX( fabs(getVelocityX()) );
	} 
	
	if(jumping){
		setVelocityX( 0 );
		if(getY() < 200){
			setVelocityY( getVelocityY() + 70 );
			//std::cout << "Going up, slowing down" << getY() << std::endl;
		}
		else if( getY() > 380 ) {
			setVelocityY(0);
			//std::cout << "Stop" << getVelocityY()  << std::endl;
			jumping = false;
			setY(380);
		}
	}
	else{
		stop();
	}

}

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		//To advance Frames towards Right
		if(getVelocityX() > 0)
    	{
			currentFrame = (currentFrame+1) % numberOfRightFrames;
		}
		//To keep the player idle
		else if(getVelocityX() == 0)
		{
			currentFrame = currentFrame;
		}
		//To advance Frames towards Left 
		else
		{
			currentFrame = numberOfRightFrames + (currentFrame+1) % numberOfLeftFrames;
		}

		timeSinceLastFrame = 0;
	}

}

void Player::detach (SmartSprite* o) {
	std::list<SmartSprite*>::iterator sp = observers.begin();
	while (sp != observers.end()) {
		if(*sp == o) {
			sp = observers.erase(sp);
			return;
		}
		++sp;
	}
}
