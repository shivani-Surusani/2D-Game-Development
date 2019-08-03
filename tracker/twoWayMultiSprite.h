#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "explodingSprite.h"

class ExplodingSprite;

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  TwoWayMultiSprite(const std::string&, const Vector2f& pos, const Vector2f& vel, const Image*);
  virtual ~TwoWayMultiSprite();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

  void explode();

protected:
  std::vector<Image *> images;
  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfRightFrames;
  unsigned numberOfLeftFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
  int getDistance(const TwoWayMultiSprite*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
