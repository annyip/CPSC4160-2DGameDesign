#include "smartSprite.h"
#include "renderContext.h"
#include "multisprite.h"
#include "player.h"
#include <cmath>
#include "gameData.h"
#include <random>
#include <functional>
#include "vector2f.h"
#include "explodingSprite.h"


SmartSprite::SmartSprite(const std::string& name, const Vector2f& pos, int w, int h):
	MultiSprite(name,Gamedata::getInstance().getXmlInt("world/width")),
	currentMode(NORMAL),
	playerPosition(pos),
	playerWidth(w),
	playerHeight(h),
	//player(p),//safeDistance = 300
	safeDistance(Gamedata::getInstance().getXmlInt("Crab/safeDistance")),
	explosion(nullptr)

{
	int num = rand()%2 ? 1 : -1;
	if( num == -1){
		setVelocityX( -fabs( getVelocityX() ) );
	}
	else
		setVelocityX( fabs( getVelocityX() ) );
}


void SmartSprite::setPlayerPosition(const Vector2f& pos){
	playerPosition = pos;
}


void SmartSprite::goLeft(){
	setVelocityX(-abs(getVelocityX()));
}

void SmartSprite::goRight(){
	setVelocityX(fabs(getVelocityX()));
}


SmartSprite::SmartSprite(const SmartSprite& s):
	MultiSprite(s),
	currentMode(s.currentMode),
	playerPosition(s.playerPosition),
	playerWidth(s.playerWidth),
	playerHeight(s.playerHeight),
	//player(s.player),
	safeDistance(s.safeDistance),
	explosion(s.explosion)
{}

SmartSprite::~SmartSprite(){
		delete explosion;
}

SmartSprite& SmartSprite::operator=(const SmartSprite& s) {
	MultiSprite::operator=(s);
	currentMode = (s.currentMode);
	playerPosition = (s.playerPosition);
	playerWidth = (s.playerWidth);
	playerHeight = (s.playerHeight);
	safeDistance = (s.safeDistance);
	explosion = (s.explosion);
	return *this;
}



void SmartSprite::explode() {
  if ( !explosion ) {
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
    printf("smartsprite explode\n");


  }
}

void SmartSprite::draw() const{
	if ( explosion ){
		explosion->draw();
	}
   else {
   		images[currentFrame]->draw(getX(), getY(), getScale());
	}
}

bool SmartSprite::isExploding(){
        if(explosion) return true;
        else return false;
}


void SmartSprite::update(Uint32 ticks){

	if ( explosion ) {
		//printf("we explodinggg\n");
    	explosion->update(ticks);
	  	if ( explosion->chunkCount() == 0 ) {
	    	delete explosion;
	   		explosion = NULL;
	  	}
    	return;
  	}

	 MultiSprite::update(ticks);
	 //Vector2f pos = player.getPosition();
	 float spriteX = getX() + getImage()->getWidth()/3;
	 float playerX = playerPosition[0] + playerWidth/3;
	 float spriteY = getY() + getImage()->getHeight()/3;
	 float playerY = playerPosition[1] + playerHeight/3;
	 float distanceToPlayer = hypot((spriteX-playerX),(spriteY-playerY));
	//std::cout << distanceToPlayer << std::endl;


	 if(currentMode == NORMAL){

	    if(safeDistance > distanceToPlayer){
	    	//if the crab's safe distance is greater than the distance to player
	    	//then it needs to evade
	    	//std::cout <<"normal to EVADE" << std::endl;
	      	currentMode = EVADE;
	    }
	}
	else if(currentMode == EVADE){
		if(safeDistance < distanceToPlayer){
			//if the crab's safe distance this less than the distance to the distance to
			//the player, revert back to normal
			//td::cout <<"EVADE to normal " << std::endl;

			currentMode = NORMAL;
		}
		else{

			if(spriteX < playerX){
				//std::cout <<"im evadinggggg - go left" << std::endl;

				goLeft();
			}
			else if(spriteX > playerX){
				//std::cout <<"im evadinggggg - go r" << std::endl;

				goRight();
			}
		}
	}
}
