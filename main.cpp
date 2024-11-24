#include "ComplexPlane.h"
#include <iostream>
#include <complex>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <thread>

using namespace sf;
using namespace std;

int main() {

    const int windowWidth = 800;
    const int windowHeight = 600;

    // Create a window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Mandelbrot Set");

    // Create the ComplexPlane object
    ComplexPlane plane(windowWidth, windowHeight);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Zoom in on left-click
                    plane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                    plane.zoomIn();
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    // Zoom out on right-click
                    plane.zoomOut();
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                // Update mouse location
                plane.setMouseLocation({ event.mouseMove.x, event.mouseMove.y });
            }
        }

        // Clear the screen
        window.clear();

        // Draw the Mandelbrot set
        window.draw(plane);

        // Display the frame
        window.display();
    }

    return 0;
}