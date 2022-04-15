#pragma once


#include<iostream>
#include<SFML/Graphics.hpp>
class Button
{
public:
	Button (){ }
	Button(std::string t, sf::Vector2f size, sf::Color bgColor, sf::Color textcolor, int charSize)
	{
		text.setString(t);
		text.setFillColor(textcolor);
		text.setCharacterSize(charSize);


		
		button.setSize(size);
		button.setFillColor(bgColor);

	}

	void SetFont(sf::Font& font)
	{
		text.setFont(font);
	}

	void setBackColor(sf::Color color)
	{
		button.setFillColor(color);
	}
	
	void setTextColor(sf::Color color)
	{
		text.setFillColor(color);
	}
	void setPostion(sf::Vector2f pos)
	{
		button.setPosition(pos);
		float xPos = (pos.x + button.getGlobalBounds().width/2) - (text.getGlobalBounds().width/2);
		float yPos = (pos.y + button.getGlobalBounds().height / 2) - (text.getGlobalBounds().height / 2);
		text.setPosition(xPos, yPos);
	}


	void drawTo(sf::RenderWindow& window)
	{
		window.draw (button);
		window.draw(text);
	}
	bool isMouseOver(sf::RenderWindow& window)
	{
		int mouseX = sf::Mouse::getPosition(window).x;
		int mouseY = sf::Mouse::getPosition(window).y;

		float btnPosX = button.getPosition().x;
		float btnPosY = button.getPosition().y;

		float btnxPosWidth = button.getPosition().x + button.getLocalBounds().width;
		float btnxPosHeight = button.getPosition().y + button.getLocalBounds().height;

		if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnxPosHeight && mouseY > btnPosY)
		{
			return true;
		}
		return false;



	}

private:
	sf::RectangleShape button;
	sf::Text text;



};

