#pragma once
#include <SFML/Graphics.hpp>
#include "random_mt.h"

//this file contains grid functionality

namespace GridVars
{

	extern int windowHeight; //window height in pixels
	extern int windowWidth; //window width in pixels
	extern int cellHeight; //cell height/width in pixels

	extern sf::Vector2i screenDimensions; //screen dimensions variable
	extern sf::Vector2i cellSize; // cell size variable

	extern int rows; 
	extern int columns;
}

//forward declarations
void initializeGrid(std::vector<std::vector<sf::RectangleShape>>& grid);
void initializeCells(std::vector<std::vector<int>>& cellStates);
void drawGrid(sf::RenderWindow& window, std::vector<std::vector<sf::RectangleShape>>& grid, std::vector<std::vector<int>>& cellStates);
