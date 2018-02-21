#include "include/Engine/Object/Game.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

//Constructor
Object::Game::Game()
{
	DEFINE_OBJECT_CONSTRUCTOR("Game");
	DEFINE_SINGLETON_OBJECT(Game);
}

//Deconstructor
Object::Game::~Game()
{

}