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
//#include "grid.h"
//#include "cell.h"

void flushPixels(sf::Uint8*);

int main(int argc, char *argv[]) {

    const unsigned int width = 1920;
    const unsigned int height = 1080;

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
    
    // [i][j][k] = [width][height][depth] = 
    // [(i * 4) + (j * width * 4) + k]
    sf::Uint8* pixels = new sf::Uint8[width * height * 4];

    /*cell* cells = new cell[width * height];
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            cells[i + j * width] = cell(i, j, sf::Color());
            cells[i + j * width].setColor(i%255, 0 , j%255);
        }
    }*/

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {

            pixels[(i * 4) + (j * width * 4) + 0] = i % 255;
            pixels[(i * 4) + (j * width * 4) + 1] = (i + j) % 255;
            pixels[(i * 4) + (j * width * 4) + 2] = j % 255;
            pixels[(i * 4) + (j * width * 4) + 3] = (255 - ((i * j) % 255));

        }
    }

    std::stringstream ss;

    window.setFramerateLimit(30);
    long int frame = 0;
    float fps = 0;
    bool running = true;

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

        ss << "fps: " << int(fps);
        text.setString(std::string(ss.str()));
        ss.str(std::string());
        text.setOrigin(text.getLocalBounds().width, text.getLocalBounds().height / 2.);
        text.setPosition(width - 2, height - ((text.getLocalBounds().height / 2.) + 15));

        window.clear();

        #pragma omp parallel for schedule(dynamic, 256)
        for (int i = 0; i < width; i++) {
            //printf("Thread %d: i=%d\n", omp_get_thread_num(), i);
            for (int j = 0; j < height; j++) {
                //cells[i + j * width].setColor((cells[i + j * width].getColor().r + 1) % 255, 0, (cells[i + j * width].getColor().b + 1) % 255);
                
                pixels[(i * 4) + (j * width * 4) + 0] = (pixels[(i * 4) + (j * width * 4) + 0] + 1) % 255;
                pixels[(i * 4) + (j * width * 4) + 1] = (pixels[(i * 4) + (j * width * 4) + 1] + 1) % 255;
                pixels[(i * 4) + (j * width * 4) + 2] = (pixels[(i * 4) + (j * width * 4) + 2] + 1) % 255;
                pixels[(i * 4) + (j * width * 4) + 3] = (pixels[(i * 4) + (j * width * 4) + 3] + 1) % 255;

            }
        }

        // activate the window's context
        window.setActive(true);

        // set up OpenGL states

        
        // clear framebuffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw to the window
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        //flush pixel array
        memset(pixels, 0, sizeof(pixels));

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