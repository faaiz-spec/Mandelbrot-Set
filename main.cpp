#include "ComplexPlane.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

int main() {

	// Create a video mode object
	int pixelWidth = VideoMode::getDesktopMode().width / 2;
	int pixelHeight = VideoMode::getDesktopMode().height / 2;
	VideoMode vm(pixelWidth, pixelHeight);
	// Create and open a window for the game
	RenderWindow window(vm, "Mandlebrot Set", Style::Default);

	ComplexPlane plane(pixelWidth, pixelHeight);

	Font font;
	if (!font.loadFromFile("Mojangles.ttf"))
	{
		cout << "poopoo caca\n";
		return 1;
	}

	Text text;
	text.setFont(font);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					// Zoom in on left-click
					plane.zoomIn();
					plane.setCenter({ event.mouseButton.x, event.mouseButton.y });
				}
				else if (event.mouseButton.button == Mouse::Right)
				{
					// Zoom out on right-click
					plane.zoomOut();
				}
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				// Update mouse location
				plane.setMouseLocation({ event.mouseMove.x, event.mouseMove.y });
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// update renderer, update text variable
		plane.updateRender();
		plane.updateText(text);

		// Clear the screen
		window.clear();

		// Draw the Mandelbrot set
		window.draw(plane);
		window.draw(text);

		// Display the frame
		window.display();
	}

	return 0;
}