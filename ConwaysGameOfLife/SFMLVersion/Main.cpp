#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>

//first, we create a namespace to hold the variables that'll be useful to access in several functions
namespace Var
{
	sf::Vector2i screenDimensions(1000, 800);	//screen dimensions 
	sf::Vector2i cellSize(10, 10);				//cell size in pixels

	const int rows = screenDimensions.y / cellSize.y; //rows represent the y-axis
	const int columns = screenDimensions.x / cellSize.x; // columns represent the x-axis

	std::mt19937 generator(static_cast<unsigned int>(std::time(0))); //badly seeded mersienne twister
	std::bernoulli_distribution distribution(0.5); //bernoulli distribution to determine chance of being alive for cells

	
	int growth{50};
}

void initializeGrid(std::vector<std::vector<sf::RectangleShape>>& grid)
{
	//this function initialises the entire grid, we take the grid as a reference parameter and then iterate through it
	//assigning a size, position and colour to the individual grid coordinate
	for (int i = 0; i < Var::rows; ++i) {
		for (int j = 0; j < Var::columns; ++j) {
			grid[i][j].setSize(sf::Vector2f(Var::cellSize.x - 1, Var::cellSize.y - 1));
			grid[i][j].setPosition(j * Var::cellSize.x, i * Var::cellSize.y);
			grid[i][j].setFillColor(sf::Color::White);
		}
	}
}

void placePopulation(std::vector<std::vector<int>>& cellStates) {

	//for each grid coordinate, assign a random true or false state so long as populations is below growthCap
	//both stored in namespace Var, growthCap was introduced to easilier change the desired repeated introduction of cells

	int populations{};

	for (int i = 0; i < Var::rows; ++i) {
		for (int j = 0; j < Var::columns; ++j) {
			if (populations < Var::growth) {
				cellStates[i][j] = Var::distribution(Var::generator);
				++populations;
			}
		}
	}
}

int countLiveNeighbours(const std::vector<std::vector<int>>& cellStates, int row, int col)
{
	//variables containing amount of live neighbours and number of rows/columns
	int liveNeighbours{ 0 };
	int numRows = static_cast<int>(cellStates.size()); //we static cast because vectors have size_t type
	int numCols = static_cast<int>(cellStates[0].size());

	//check the status of each of the sorrounding cells (there's 8)
	for (int i{ -1 }; i <= 1; ++i) {
		for (int j{ -1 }; j <= 1; ++j) {
			if (i == 0 && j == 0) continue; //skip the cell that is being checked

			int newRow = row + i;
			int newCol = col + j;

			//check if the neighbouring cell is within bounds and alive
			if (newRow >= 0 && newRow < numRows && newCol >= 0 && newCol < numCols) {
				liveNeighbours += cellStates[newRow][newCol];
			}

			//this loop is a bit wrinkly. i and j loops through -1 to 1
			// and can be represented like this:
			// 
			// (i-1, j-1), (i-1, j), (i-1, j+1)
			// (i, j-1),			   (i, j+1)
			// (i+1, j-1), (i+1, j), (i+1, j+1)
			//
			//As we can see, i increments upwards in the y axis (inverted though)
			//and j increments upwards in the x-axis (not inverted)
			//using if (i == 0 && j == 0) continue; we skip the loop that checks
			//the center cell. the loop counts through the cells, starting at
			//(-1,-1) it then puts that into the newRow and newCol
			//then the following if statement checks if newRow is larger or equal
			//to zero and if that is higher than the number of rows in the board
			//and the same for columns in this one if statement. 
			//if those are all true, then we access the value at that cell, and add it
			//to liveNeighbours, so if it's alive (1) it adds 1 and if it's dead it adds 0
			//so that we end up with a count of how many live neighbours there are.
		}
	}

	return liveNeighbours; //at the end we return liveNeighbours for use in updateBoard()
}


void updateBoard(std::vector<std::vector<int>>& cellStates)
{
	//variables to initialize a new board which we will iterate over, and then
	//copy into original board and then repeat this in sequence over and over.
	//the syntax board[0].size(); is the same as board.size(); but for the inner vector
	int rows = cellStates.size();
	int cols = cellStates[0].size();

	std::vector<std::vector<int>> nextCellStates = cellStates; //create new board initialized as original board

	for (int i{ 0 }; i < rows; ++i) {
		for (int j{ 0 }; j < cols; ++j) {
			int liveNeighbours = countLiveNeighbours(cellStates, i, j);

			//now we implement the rules of the game of life, which is:
			//if a live cell has 2-3 neighbours, it stay alive
			//if a live cell has fewer than 2 or more than 3 live neighbours, it dies
			//a dead cell with exactly 3 live neighbours becomes alive
			//any other cell remains in its current state

			if (cellStates[i][j] == 1) { //if cell is alive
				if (liveNeighbours < 2 || liveNeighbours > 3) {
					nextCellStates[i][j] = 0; // dies due to over or underpopulation
				}
			}
			else { //cell is dead
				if (liveNeighbours == 3) {
					nextCellStates[i][j] = 1; // comes to life by reproduction
				}
			}
		}
	}

	//update the original board to the new state (we passed the board as reference)
	cellStates = nextCellStates;
}


int main()
{
	//we start by making the window
	sf::RenderWindow window(sf::VideoMode(Var::screenDimensions.x, Var::screenDimensions.y), "Conway's Game of Life");
	//we initialise the window with the x and y value of Var::screenDimensions

	//now we create the grid that the game of life will play in
	std::vector<std::vector<sf::RectangleShape>> grid(Var::rows, std::vector<sf::RectangleShape>(Var::columns));
	//this creates a grid of rectangle shapes that we will draw inside the loop
	//now we make a grid of cellstates, so that we can change and draw depending on it
	std::vector<std::vector<int>> cellStates(Var::rows, std::vector<int>(Var::columns, 0)); //0 for dead, 1 for alive<

	//framerate limit if i want to see what's going on better
	//window.setFramerateLimit(18);

	while (window.isOpen())
	{
		//now that we have the loop, we make the event puller so that we can do fun stuff
		//like close the window again
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed: window.close(); break;
			default:
				break;
			}

			// Detect mouse click
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				// Get mouse position and convert to grid coordinates
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				int column = mousePos.x / Var::cellSize.x;
				int row = mousePos.y / Var::cellSize.y;

				// Ensure the click is within the grid bounds
				if (row >= 0 && row < Var::rows && column >= 0 && column < Var::columns) {
					// Change cell color to indicate it was clicked
					grid[row][column].setFillColor(sf::Color::Green);
				}
			}
		}

		initializeGrid(grid);
		placePopulation(cellStates);

		for (int i = 0; i < Var::rows;++i) {
			for (int j = 0; j < Var::columns; ++j) {
				if (cellStates[i][j] == 1) {
					grid[i][j].setFillColor(sf::Color::Cyan);
				}
				else
				{
					grid[i][j].setFillColor(sf::Color::Black);
				}
			}
		}




		window.clear();
		//this is the drawing part of the loop
		
		//this draws the grid as white squares
		for (int i = 0; i < Var::rows; ++i) {
			for (int j = 0; j < Var::columns; ++j) {
				window.draw(grid[i][j]);
			}
		}

		window.display();

		updateBoard(cellStates);

	}
}