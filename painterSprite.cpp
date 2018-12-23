#include <time.h>
#include "painterSprite.h"
#include "gameData.h"
#include "renderContext.h"
#include <iostream>
#include <functional>
#include <random>
#include <cmath>

painterSprite::painterSprite(const std::string& name) :
    MultiSprite(name)
{}


painterSprite::painterSprite(const painterSprite& s) : MultiSprite(s)
{}


void painterSprite::update(Uint32 ticks){
  MultiSprite::advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if(getY() > worldHeight){
    setY(0 - getScaledHeight());
  }

}
