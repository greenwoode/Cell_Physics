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
//#include "grid.h"
//#include "cell.h"

void flushPixels(sf::Uint8*);

int main(int argc, char *argv[]) {

    const unsigned int width = 1920;
    const unsigned int height = 1440;

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

            //pixels[(i * 4) + (j * width * 4) + 0] = i % 255;
            //pixels[(i * 4) + (j * width * 4) + 1] = (i + j) % 255;
            //pixels[(i * 4) + (j * width * 4) + 2] = j % 255;
            //pixels[(i * 4) + (j * width * 4) + 3] = (255 - ((i * j) % 255));

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

                //pixels[(i * 4) + (j * width * 4) + 0] = (i + frame) % 255;
                //pixels[(i * 4) + (j * width * 4) + 1] = ((i + j) + frame) % 255;
                //pixels[(i * 4) + (j * width * 4) + 2] = (j + frame) % 255;
                //pixels[(i * 4) + (j * width * 4) + 3] = (255 - (((i * j) + frame) % 255));

            }
        }

        // activate the window's context
        window.setActive(true);

        // set up OpenGL states
           //change the positon of the render
       // glRasterPos2i(-1664, -375);
        
        // clear framebuffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // draw to the window
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        //flush pixel array
        memset(pixels, 0, sizeof(pixels));




       



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