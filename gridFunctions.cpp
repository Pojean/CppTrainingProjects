#include "gridFunctionsHeader.h"

namespace GridVars
{
	//variable namespace for future modability

	int windowHeight{ 1000 }; //window height in pixels
	int windowWidth{ 800 }; //window width in pixels
	int cellHeight{ 10 }; //cell height/width in pixels

	sf::Vector2i screenDimensions(windowHeight, windowWidth); //screen dimensions variable
	sf::Vector2i cellSize(cellHeight, cellHeight); // cell size variable

	int rows = screenDimensions.y / cellSize.y;
	int columns = screenDimensions.x / cellSize.x;
}

//-------------------------- Initializers --------------------------------------
void initializeGrid(std::vector<std::vector<sf::RectangleShape>>& grid)
{
	//initializes grid rectangle objects for drawing later
	for (int i = 0; i < GridVars::rows; ++i) {
		for (int j = 0; j < GridVars::columns; ++j) {

			grid[i][j].setSize(sf::Vector2f(GridVars::cellSize.x - 1, GridVars::cellSize.y - 1)); //assigns a size to each object in vector
			grid[i][j].setPosition(j * GridVars::cellSize.x, i * GridVars::cellSize.y); //assigns a position based on iterations
		}
	}
}

void initializeCells(std::vector<std::vector<int>>& cellStates)
{
	//initializes grid with 0 values as baseline
	for (int i = 0; i < GridVars::rows; ++i) {
		for (int j = 0; j < GridVars::columns;++j) {
			cellStates[i][j] = 0;
		}
	}
}
//------------------------- Initializers end -----------------------------------


//grid drawing function, pulls from cellstates to draw the grid squares with set colours
void drawGrid(sf::RenderWindow& window, std::vector<std::vector<sf::RectangleShape>>& grid, std::vector<std::vector<int>>& cellStates)
{
	for (int i = 0; i < GridVars::rows; ++i) {
		for (int j = 0; j < GridVars::columns;++j) {
			window.draw(grid[i][j]);
			//i know i want more fill colours later, so i'm making a switch so i can seperate the colours better
			switch (cellStates[i][j])
			{
			case 0: grid[i][j].setFillColor(sf::Color::White); break;
			default:
				break;
			}
		}
	}
}