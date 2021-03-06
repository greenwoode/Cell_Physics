//standard stuff
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <sstream>
#include "Button.h"
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
    const unsigned int height = 1440;

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

   //rectangle on the right side ofthe screen
    sf::RectangleShape rectangle(sf::Vector2f(120, 50));
    // sets the size of the rectangle to  (600,1440)
    rectangle.setSize(sf::Vector2f((float)600, height));
    //sets color
    rectangle.setFillColor(sf::Color(42, 58, 61));
    rectangle.setOrigin(rectangle.getLocalBounds().width, 0);
    // changes the position of the rectangle on the window
    rectangle.setPosition(width, 0);
    
    // this is the rectangle on the bottom of the screen 
    //reference the above object on documentation
    sf::RectangleShape botrectangle(sf::Vector2f(120, 50));

    botrectangle.setSize(sf::Vector2f(width, (float) 300));
    botrectangle.setFillColor(sf::Color(42, 58, 61));
    botrectangle.setOrigin(0, botrectangle.getLocalBounds().height);
    botrectangle.setPosition(0,height);




    Button btn1("Resume", { 100,100 }, sf::Color::Green, sf::Color::Black, 20);
    btn1.setPostion({0,botrectangle.getGlobalBounds().height + 900});
    btn1.SetFont(font);

    Button btn2("Pause", { 100,100 }, sf::Color::Green, sf::Color::Black, 20);
    btn2.setPostion({200,botrectangle.getGlobalBounds().height + 900 });
    btn2.SetFont(font);

    Button btn3("Regular", { 100,100 }, sf::Color::Green, sf::Color::Black, 20);
    btn3.setPostion({ 400,botrectangle.getGlobalBounds().height + 900 });
    btn3.SetFont(font);

    Button btn4("Fast Forward", { 100,100 }, sf::Color::Green, sf::Color::Black, 20);
    btn4.setPostion({ 600,botrectangle.getGlobalBounds().height + 900 });
    btn4.SetFont(font);

    Button btn5("Sand", { 100,100 }, sf::Color::Green, sf::Color::Black, 20);
    btn5.setPostion({ rectangle.getGlobalBounds().width + 750, 250 });
    btn5.SetFont(font);

    Button btn6("Air", { 100,100 }, sf::Color::Green, sf::Color::Black, 20);
    btn6.setPostion({ rectangle.getGlobalBounds().width + 750, 500 });
    btn6.SetFont(font);

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

    printf("created GridObject\n");

    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {

            if ((i == 0) || (j == 0) || (i == (gridWidth - 1)) || (j == (gridHeight - 1))) {
                Grid.addCell((gridWidth / (i+1) + gridHeight / (j+1)), i, j, 150, 150, 150, 0, 9999999.9);
            }
            else {
                Grid.addCell((gridWidth / (i + 1) + gridHeight / (j + 1)), i, j, 0, 0, 0, 1, 0.00000129);
            }
            

        }
    }
    printf("Created Board\n");

    for (int i = (gridWidth/2 - 5); i < (gridWidth / 2 + 5); i++) {
        for (int j = (gridHeight*0.75); j < (gridHeight * 0.75) + 15; j++) {

            Grid.deleteCell(i, j);
            Grid.addCell((gridWidth / (i + 1) + gridHeight / (j + 1)), i, j, 242, 215, 126, 2, 0.00153);

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

           
                switch (event.type)
                {
                case sf::Event::MouseMoved:
                    if (btn1.isMouseOver(window))
                    {
                        btn1.setBackColor(sf::Color::White);
                    }
                    else
                    {
                        btn1.setBackColor(sf::Color::Green);
                    }
                    if (btn2.isMouseOver(window))
                    {
                        btn2.setBackColor(sf::Color::White);
                    }
                    else
                    {
                        btn2.setBackColor(sf::Color::Green);
                    }
                    if (btn3.isMouseOver(window))
                    {
                        btn3.setBackColor(sf::Color::White);
                    }
                    else
                    {
                        btn3.setBackColor(sf::Color::Green);
                    }
                    if (btn4.isMouseOver(window))
                    {
                        btn4.setBackColor(sf::Color::White);
                    }
                    else
                    {
                        btn4.setBackColor(sf::Color::Green);
                    }
                    if (btn5.isMouseOver(window))
                    {
                        btn5.setBackColor(sf::Color::White);
                    }
                    else
                    {
                        btn5.setBackColor(sf::Color::Green);
                    }
                    if (btn6.isMouseOver(window))
                    {
                        btn6.setBackColor(sf::Color::White);
                    }
                    else
                    {
                        btn6.setBackColor(sf::Color::Green);
                    }
                }
            


        }
        
        currentTime = clock.getElapsedTime().asSeconds();
        fps = 1.f / (currentTime - lastTime);
        lastTime = currentTime;

        Grid.update();
        t += Grid.getTimestep();

        #pragma omp parallel for schedule(dynamic, 256)
        for (int i = 0; i < gridWidth; i++) {
            for (int j = (height - gridHeight); j < height; j++) {
                pixels[(i * 4) + (j * width * 4) + 0] = Grid.getColorRAt(i, j - (height - gridHeight));
                pixels[(i * 4) + (j * width * 4) + 1] = Grid.getColorGAt(i, j - (height - gridHeight));
                pixels[(i * 4) + (j * width * 4) + 2] = Grid.getColorBAt(i, j - (height - gridHeight));
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
           //change the positon of the render
       // glRasterPos2i(-1664, -375);
        
        // clear framebuffers
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw to the window
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        //flush pixel array (In theory, prevent ghosting)
        //memset(pixels, 0, sizeof(pixels));




       



        // deactivate the window's context
        window.setActive(false);
        
        window.draw(rectangle);
        window.draw(botrectangle);
        btn1.drawTo(window);
        btn2.drawTo(window);
        btn3.drawTo(window);
        btn4.drawTo(window);
        btn5.drawTo(window);
        btn6.drawTo(window);
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