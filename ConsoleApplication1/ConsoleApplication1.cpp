//standard stuff
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <sstream>

//GUI stuff
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

//Parrelel stuff
#include <omp.h>


//Local Stuff
#include "grid.h"
#include "cell.h"

void flushPixels(sf::Uint8*);

int main(int argc, char *argv[]) {

    const unsigned int width = 1920;
    const unsigned int height = 1080;

    const unsigned int gridWidth = 1280;
    const unsigned int gridHeight = 720;

    omp_set_num_threads(int(omp_get_max_threads() * 0.50));

    sf::RenderWindow window(sf::VideoMode(width, height), "StarDust");
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Clock clock;
    float lastTime = 0;
    float currentTime = 0;

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(25);
    text.setString("0");
    text.setFillColor(sf::Color::White);
    
    printf("created window\n");

    // [i][j][k] = [width][height][depth] = 
    // [(i * 4) + (j * width * 4) + k]
    unsigned char* pixels = new unsigned char[width * height * 4];

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {

            pixels[(i * 4) + (j * width * 4) + 0] = 0;
            pixels[(i * 4) + (j * width * 4) + 1] = 0;
            pixels[(i * 4) + (j * width * 4) + 2] = 0;
		}
	}

    printf("created pixel array\n");

    std::stringstream ss;

    window.setFramerateLimit(30);
    long int frame = 0;
    float fps = 0;
    bool running = true;
    double t = 0.0;

    grid Grid = grid(gridWidth, gridHeight, -9.81);

    printf("created Grid\n");

    cell* tempCell;

    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {

            if ((i == 0) || (j == 0) || (i == (gridWidth - 1)) || (j == (gridHeight - 1))) {
                tempCell = new cell((gridWidth / i + gridHeight / j), i, j, 150, 150, 150, 0, 9999999.9);
                Grid.addCell(tempCell, i, j);
            }
            else {
                tempCell = new cell((gridWidth / i + gridHeight / j), i, j, 0, 0, 0, 1, 0.00000129);
                Grid.addCell(tempCell, i, j);
            }
            

        }
    }
    printf("Created Board\n");

    for (int i = (gridWidth/2 - 5); i < (gridWidth / 2 + 5); i++) {
        for (int j = (gridHeight*0.75); j < (gridHeight * 0.75) + 15; j++) {

            Grid.deleteCell(i, j);
            tempCell = new cell((gridWidth / i + gridHeight / j), i, j, 242, 215, 126, 2, 0.00153);
            Grid.addCell(tempCell, i, j);

        }
    }

    printf("filled Board\n");

    sf::Color tempColor;

    clock.restart();
    while (running)
    {


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                printf("\nclosing\n");
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
        
        currentTime = clock.getElapsedTime().asSeconds();
        fps = 1.f / (currentTime - lastTime);
        lastTime = currentTime;

        Grid.update();
        t += Grid.getTimestep();

        #pragma omp parallel for schedule(dynamic, 256)
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                pixels[(i * 4) + (j * width * 4) + 0] = Grid.getColorRAt(i, j);
                pixels[(i * 4) + (j * width * 4) + 1] = Grid.getColorGAt(i, j);
                pixels[(i * 4) + (j * width * 4) + 2] = Grid.getColorBAt(i, j);
            }
        }

        ss << "time: " << t << "s, fps: " << int(fps);
        text.setString(std::string(ss.str()));
        ss.str(std::string());
        text.setOrigin(text.getLocalBounds().width, text.getLocalBounds().height / 2.);
        text.setPosition(width - 2, height - ((text.getLocalBounds().height / 2.) + 15));

        window.clear();

        // activate the window's context
        window.setActive(true);

        // set up OpenGL states

        
        // clear framebuffers
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw to the window
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        //flush pixel array (In theory, prevent ghosting)
        //memset(pixels, 0, sizeof(pixels));

        // deactivate the window's context
        window.setActive(false);

        window.draw(text);
        window.display();
        frame++;
    }

    

    return 0;
}

void flushPixels(sf::Uint8 *array) {

    for (int i = 0; i < sizeof(array); i++) {
        array[i] = 0;
    }

}