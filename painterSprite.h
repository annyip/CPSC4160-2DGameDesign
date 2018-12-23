#ifndef PAINTERSPRITE__H
#define PAINTERSPRITE__H
#include <string>
#include "multisprite.h"

class painterSprite : public MultiSprite {
public:
  painterSprite(const std::string& name);
  painterSprite(const painterSprite& s);

  virtual void update(Uint32 ticks);

private:


};

#endif
