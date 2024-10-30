#include <iostream>
#include <vector>
#include <string>

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

int main()
{
	int rows{ 20 };		//define the number of rows
	int cols{ 20 };		//define the number of columns

	//create a 2D vector board, initially filled with 0's (all dead)
	std::vector<std::vector<int>> board(rows, std::vector<int>(cols, 0));

	//2D vectors is a vector of vectors. The outer vector represents the rows
	//and the inner vector represents the colums of each row. if this was
	//a coordinate systen (it kind of is) the outer vector represents the y-axis
	//and the inner vector represents the x-axis in terms of the grid.

	//set a few cells to alive (1) to get started
	board[1][2] = 1;
	board[2][3] = 1;
	board[3][1] = 1;
	//we access a column with the syntax (outervector)[row][column](zero indexed)

	//now we call the displayBoard() function and can see the board in the console
	displayBoard(board);

	return 0;
}