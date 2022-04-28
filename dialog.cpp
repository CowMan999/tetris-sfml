#include "includes.hpp"

Dialog::Dialog(std::string message, std::string subtitle, std::string minititle, sf::RenderWindow* window, sf::Font& font, unsigned short char_size, unsigned short char_size2, unsigned short char_size3, bool allow_close)
{
	this->window = window;

	// setup text
	this->text.setFont(font);
	this->text.setCharacterSize(char_size);
	this->text.setString(message);
	this->text.setFillColor(sf::Color::White);

	// setup text2
	this->text2.setFont(font);
	this->text2.setCharacterSize(char_size2);
	this->text2.setString(subtitle);
	this->text2.setFillColor(sf::Color::White);

	// setup text3
	this->text3.setFont(font);
	this->text3.setCharacterSize(char_size3);
	this->text3.setString(minititle);
	this->text3.setFillColor(sf::Color::White);

	this->allow_close = allow_close;

	// center text
	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	this->text.setPosition(window->getView().getCenter().x, window->getView().getCenter().y);

	// position text2 under main text
	sf::FloatRect text2Rect = this->text2.getLocalBounds();
	this->text2.setOrigin(text2Rect.left + text2Rect.width / 2.0f,
		text2Rect.top + text2Rect.height / 2.0f);
	this->text2.setPosition(window->getView().getCenter().x, window->getView().getCenter().y + char_size * 1.25);

	// position text3 under text2
	sf::FloatRect text3Rect = this->text3.getLocalBounds();
	this->text3.setOrigin(text3Rect.left + text3Rect.width / 2.0f,
		text3Rect.top + text3Rect.height / 2.0f);
	this->text3.setPosition(window->getView().getCenter().x, window->getView().getCenter().y + char_size * 2.25);

}

Dialog::~Dialog() {}

void Dialog::display()
{
	bool dialog_open = 1;

	// mini mainloop
	while(dialog_open && running)
	{
		sf::Event event;
		while(this->window->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				this->window->close();
				dialog_open = 0;
				break;

			case sf::Event::KeyPressed:
				if(event.key.code == sf::Keyboard::Return && allow_close) dialog_open = 0;
				break;

			case sf::Event::Resized:
				window->setView(getLetterboxView(window->getView(), event.size.width, event.size.height));
				break;

			
			default:
				break;
			}
		}

		// clear screen
		this->window->clear();

		// draw text
		this->window->draw(this->text);
		this->window->draw(this->text2);
		this->window->draw(this->text3);

		// display
		this->window->display();
	}

}

void Dialog::kill()
{
	this->running = false;
}