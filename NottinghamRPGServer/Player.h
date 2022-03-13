#pragma once
#include <string>
#include "SFML/Network.hpp"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class Player
{

public:

	std::string name;
	enum moveState { Right,Left,Up,Down,Stop } movestate ;
	sf::Vector2f location;

};

