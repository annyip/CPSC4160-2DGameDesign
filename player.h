#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include "drawable.h"
#include <list>
#include "smartSprite.h"
#include "bulletPool.h"

class SmartSprite;
class ExplodingSprite;

class Player : public Drawable {


public:
  Player(const std::string&);
  Player(const Player&);
  ~Player();

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

  void right();
  void left();
  void stop();
  void shoot();


  virtual void attach(SmartSprite* o); //attach all crabs once loated
  void detach(SmartSprite* o);
  void notify();

  virtual bool collidedWith(const Drawable*) const;
  virtual void explode();

  int getPlayerLives();

private:

  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  Vector2f initialVelocity;

  void advanceFrame(Uint32 ticks);
  Player& operator=(const Player&);
  std::list<SmartSprite*> observers;

  bool collision;

  /*std::string bulletName;
  std::list<Bullet*> bulletList;
  std::list<Bullet*> freeList;

  float minSpeed;
  float bulletInterval;
  float timeSinceLastBulletFrame;*/
  std::string bulletName;
  float minSpeed;
  BulletPool bullets;

  bool playerDirection; //1 for right, 0 for left

  ExplodingSprite* explosion;
  int playerLives;
};
#endif
