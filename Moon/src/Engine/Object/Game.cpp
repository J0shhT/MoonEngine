#include "include/Engine/Object/Game.h"

#include "include/Engine/StandardOut.h"

using namespace Moon;

//Constructor
Object::Game::Game() : Object("Game", "game")
{
	DEFINE_OBJECT_CONSTRUCTOR();
	DEFINE_SINGLETON_OBJECT(Game);
}

//Deconstructor
Object::Game::~Game()
{

}