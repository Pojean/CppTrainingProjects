#include <iostream>
#include <vector>
#include <thread> //for sleep_for
#include <chrono> //for chrono::seconds	
#include <cstdlib> //for system()

// THIS CODE IS WRITTEN MOSTLY BY CHATGPT EXCEPT FOR THE COMMENTS
// I AM FOLLOWING ALONG MANUALLY TYPING AND INTERROGATING CHATGPT
// ABOUT THE SYNTAX AND EXPRESSIONS TO UNDERSTAND HOW THEY WORK

void displayBoard(const std::vector<std::vector<int>>& board)
{
	//we use a range-based for loop to traverse through the 2D vector
	//printing a # if the cell is alive (1) or a . if the cell is dead (0)

	for (const auto& row : board) {
		for (int cell : row) {
			std::cout << (cell == 1 ? "#" : ".") << " ";
		}
		std::cout << '\n';
	}
	std::cout << std::string(39, '-') << '\n'; // prints a line of - that is 20 long
	//through testing i found that at least for a 20x20 grid 39 - matches the width
	//when i begin to expand on this and make it modifiable i will see if N-1 works
}

int countLiveNeighbours(const std::vector<std::vector<int>>& board, int row, int col)
{
	//variables containing amount of live neighbours and number of rows/columns
	int liveNeighbours{ 0 };
	int numRows = static_cast<int>(board.size()); //we static cast because vectors have size_t type
	int numCols = static_cast<int>(board[0].size());

	//check the status of each of the sorrounding cells (there's 8)
	for (int i{ -1 }; i <= 1; ++i) {
		for (int j{ -1 }; j <= 1; ++j) {
			if (i == 0 && j == 0) continue; //skip the cell that is being checked

			int newRow = row + i;
			int newCol = col + j;

			//check if the neighbouring cell is within bounds and alive
			if (newRow >= 0 && newRow < numRows && newCol >= 0 && newCol < numCols) {
				liveNeighbours += board[newRow][newCol];
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

void updateBoard(std::vector<std::vector<int>>& board)
{
	//variables to initialize a new board which we will iterate over, and then
	//copy into original board and then repeat this in sequence over and over.
	//the syntax board[0].size(); is the same as board.size(); but for the inner vector
	int row = board.size();
	int cols = board[0].size();

	std::vector<std::vector<int>> nextBoard = board; //create new board initialized as original board

	for (int i{ 0 }; i < row; ++i) {
		for (int j{ 0 }; j < cols; ++j) {
			int liveNeighbours = countLiveNeighbours(board, i, j);

			//now we implement the rules of the game of life, which is:
			//if a live cell has 2-3 neighbours, it stay alive
			//if a live cell has fewer than 2 or more than 3 live neighbours, it dies
			//a dead cell with exactly 3 live neighbours becomes alive
			//any other cell remains in its current state

			if (board[i][j == 1]) { //if cell is alive
				if (liveNeighbours < 2 || liveNeighbours > 3) {
					nextBoard[i][j] = 0; // dies due to over or underpopulation
				}
			}
			else { //cell is dead
				if (liveNeighbours == 3) {
					nextBoard[i][j] = 1; // comes to life by reproduction
				}
			}
		}
	}

	//update the original board to the new state (we passed the board as reference)
	board = nextBoard;
}

void initializeBoard(std::vector<std::vector<int>>& board)
{
	//this function initializes the board with a set pattern, later i want to make it random
	//there are two pattern (probnably more i don't know) that i can pick: 
	//the glider, or the blinker, i'm gonna pick the glider which is supposed to
	//glide across the grid and self replicate

	board[1][0] = 1;
	board[2][1] = 1;
	board[0][2] = 1;
	board[1][2] = 1;
	board[2][2] = 1;
}

int main()
{
	int rows{ 20 };		//define the number of rows
	int cols{ 20 };		//define the number of columns

	//create a 2D vector board, initially filled with 0's (all dead)
	std::vector<std::vector<int>> board(rows, std::vector<int>(cols, 0));
	initializeBoard(board); //set initial pattern

	//2D vectors is a vector of vectors. The outer vector represents the rows
	//and the inner vector represents the colums of each row. if this was
	//a coordinate systen (it kind of is) the outer vector represents the y-axis
	//and the inner vector represents the x-axis in terms of the grid.

	//set a few cells to alive (1) to get started
	board[1][2] = 1;
	board[2][3] = 1;
	board[3][1] = 1;
	//we access a column with the syntax (outervector)[row][column](zero indexed)

	//main simulation loop, i want it to repeat for a set amount of iterations
	//that way i can easily make that an input later

	int iterations{ 500 }; //we'll start with 500 iterations
	while (iterations < iterations + 1) //a flexible condition for later.
	{
		//i am very unfamiliar with <cstdlib> but i can use system("CLS); to clear the display
		//before each redraw, which will be a more pleasant viewing experience.
		system("CLS");
		displayBoard(board); //shows the current sate of the board
		updateBoard(board); //updates to the next generation

		//i'm very unfamilar with the next expression but it allows me to pause
		//the simulation for an amount of time before the next generation

		//std::this_thread::sleep_for(std::chrono::milliseconds(50)); //pauses for 50 miliseconds

		++iterations; //increments iterations
	}
	return 0;
}
