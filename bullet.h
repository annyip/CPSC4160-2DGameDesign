#include <iostream>
#include "multisprite.h"
#include "gameData.h"

//where do you set the starting postion of the bullet???

class Bullet : public MultiSprite {
public:
  explicit Bullet(const string& name) :
    MultiSprite(name), 
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false) 
  { }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }

private:
  float distance;
  float maxDistance;
  bool tooFar;
};
