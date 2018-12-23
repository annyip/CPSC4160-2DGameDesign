CXX = g++

# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl2-config --cflags` -g -W -Wall -Werror -std=c++11 -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/

LDFLAGS = `sdl2-config --libs` -lm -lexpat -lSDL2_ttf -lSDL2_image -lSDL2_mixer

OBJS = \
  renderContext.o \
	ioMod.o \
	parseXML.o \
	gameData.o \
	viewport.o \
	world.o \
	collisionStrategy.o\
	spriteSheet.o \
	image.o \
	imageFactory.o \
	frameGenerator.o \
	explodingSprite.o \
	sprite.o \
	multisprite.o \
	painterSprite.o \
	vector2f.o \
	smartSprite.o \
	bullet.o \
	bulletPool.o \
	chunk.o \
	player.o \
	clock.o \
	HUD.o \
	engine.o \
	TwoWayMultiSprite.o \
	sound.o \
	main.o
EXEC = run

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
	rm -rf frames/*.bmp
