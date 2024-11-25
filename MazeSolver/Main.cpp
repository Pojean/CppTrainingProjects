#include <SFML/Graphics.hpp>
#include <iostream>
#include "gridFunctionsHeader.h"
#include <vector>
#include "random_mt.h"



int main() {
    //RenderWindow object
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Maze Solver");

    //grid objects, i will need two, one for squares, one for cellstates
    //the variables are storef in gridFunctionsHeader.h
    std::vector<std::vector<sf::RectangleShape>> grid(GridVars::rows, std::vector<sf::RectangleShape>(GridVars::columns));
    std::vector<std::vector<int>> cellStates(GridVars::rows, std::vector<int>(GridVars::columns));

    initializeGrid(grid);
    initializeCells(cellStates);


    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        drawGrid(window, grid, cellStates);

        window.display();
    }

    return 0;
}
