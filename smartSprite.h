#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "multisprite.h"
#include "player.h"
#include "gameData.h"
#include "vector2f.h"

class ExplodingSprite;

class SmartSprite : public MultiSprite {
public:
  SmartSprite(const std::string& name, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&);
  virtual void update(Uint32 ticks);
  ~SmartSprite();
  void setPlayerPosition(const Vector2f& pos);
  virtual void explode();
  virtual void draw() const;
  bool isExploding();



private:
  enum MODE {NORMAL, EVADE};
  MODE currentMode;
  Vector2f playerPosition;
  int playerWidth;
  int playerHeight;
  //Player& player;
  float safeDistance;

  void goLeft();
  void goRight();

  SmartSprite& operator=(const SmartSprite&);

  ExplodingSprite* explosion;


};
#endif
