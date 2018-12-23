#include "player.h"
#include "gameData.h"
#include "imageFactory.h"
#include "smartSprite.h"
#include <iostream>
#include <list>
#include "explodingSprite.h"

Player::Player( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  initialVelocity(getVelocity()),
  observers(),
  collision(false),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet")),
  //bulletList(),
  //freeList(),
  minSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  //bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/frameInterval")),
  //timeSinceLastBulletFrame(0),
  bullets(bulletName),
  playerDirection(1),
  explosion(nullptr),
  playerLives(Gamedata::getInstance().getXmlInt(name+"/playerLives"))
{}

Player::Player(const Player& s) :
  Drawable(s),
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  initialVelocity( s.initialVelocity ),
  observers(),
  collision(s.collision),
  bulletName(s.bulletName),
  //bulletList(),
  //freeList(),
  minSpeed(s.minSpeed),
  //bulletInterval(s.bulletInterval),
  //timeSinceLastBulletFrame(s.timeSinceLastFrame),
  bullets(s.bulletName),
  playerDirection(s.playerDirection),
  explosion(nullptr),
  playerLives(s.playerLives)
  { }

Player::~Player(){
  if(explosion) delete explosion;
}

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  initialVelocity = ( s.initialVelocity );
  observers = ( s.observers );
  collision = ( s.collision );
  bulletName = ( s.bulletName );
  minSpeed = ( s.minSpeed );
  //bullets =  ( s.bulletName );
  playerDirection = ( s.playerDirection );
  explosion = ( s.explosion );
  /*bulletList = (s.bulletList);
  freeList = (s.freeList);
  minSpeed = (s.minSpeed);
  bulletInterval = ( s.bulletInterval);
  timeSinceLastBulletFrame = (s.timeSinceLastFrame);*/

  playerLives = ( s.playerLives );
  return *this;
}

void Player::draw() const {
  if ( explosion ){
    explosion->draw();
  }
  else{
    images[currentFrame]->draw(getX(), getY(), getScale());
    bullets.draw();
  }
}

void Player::stop() {
  setVelocity( Vector2f(0, 0) );
  //setVelocityX(0.93*getVelocityX() );
}

void Player::right() {
  if (getX() < worldWidth-getScaledWidth()) {
    if ( timeSinceLastFrame > frameInterval ) {
        timeSinceLastFrame = 0;
      currentFrame = (currentFrame+1) % (numberOfFrames/2);

    }
    setVelocityX(initialVelocity[0]);
    playerDirection = 1;
  }
}

void Player::left()  {
  if (getX() > 0) {
    if ( timeSinceLastFrame > frameInterval ) {
        timeSinceLastFrame = 0;
      currentFrame = (numberOfFrames/2) + (currentFrame+1) % (numberOfFrames/2);

    }
     setVelocityX(-initialVelocity[0]);
     playerDirection = 0;
  }
}

void Player::shoot(){
  //TODO: make some kind of delay so the player wont shoot
  // 2 times on one hit of spacebar

  float x = getX()+getImage()->getWidth()/4;
  float y = getY()+getImage()->getHeight()/2;
  // I'm not adding minSpeed to y velocity:
  if(playerDirection == 1){ //facing right
    //printf("shoot right\n");
    //SDL_Delay(200); //not sure if this works
    if(getVelocityX() < 0) minSpeed = minSpeed * -1;
    //std::cout << "facing right: " << lol[0] << std::endl;
    bullets.shoot(Vector2f(x, y), Vector2f(minSpeed+getVelocityX(), 0));
  }
  else{ //WHY WONT YOU SHOOT LEFT?? D: ----nevermind...works now
    //printf("shoot left\n");
    //std::cout << "facing left: " << getVelocityX() << std::endl;
    bullets.shoot(Vector2f(x, y), Vector2f(getVelocityX()-minSpeed, 0));

  }
}


bool Player::collidedWith(const Drawable* obj) const {
  return bullets.collidedWith( obj );
}


void Player::explode() {
  if ( !explosion ) {
    Sprite
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
    //playerLives--;
    std::cout << "in player explode " << std::endl;

    //std::cout << "in Player explode - lives left: " << playerLives << std::endl;
  }
}

void Player::attach(SmartSprite* o){
   observers.push_back(o);
}

void Player::detach(SmartSprite* o){
 std::list<SmartSprite*>::iterator itr = observers.begin();
  while (itr != observers.end()) {
    if (*itr == o) {
      itr = observers.erase(itr);
      return;
    }
    ++itr;
  }
}

void Player::notify(){
  std::list<SmartSprite*>::iterator itr = observers.begin();
  while (itr != observers.end()) {
    //(*itr)->setPlayerPosition(getPosition());
    //++itr;
     int offset = rand()%5;
    offset = offset*(rand()%2?-1:1);
    const Vector2f off(offset, offset);
    (*itr)->setPlayerPosition( getPosition()+off );
    ++itr;
  }
}


int Player::getPlayerLives() {return playerLives;}

void Player::update(Uint32 ticks) {

  //advanceFrame(ticks);
  timeSinceLastFrame += ticks;

  bullets.update(ticks);

  if ( explosion ) {
    explosion->update(ticks);
  if ( explosion->chunkCount() == 0 ) {
    playerLives--;
    delete explosion;
    explosion = NULL;
  }
    return;
  }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );

  }
  if ( getX() >= worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }

  notify();

  stop();


}
