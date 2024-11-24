#include "ComplexPlane.h"
#include <iostream>
#include <complex>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <thread>

using namespace sf;
using namespace std;

int main() {

    VideoMode desktopMode = VideoMode::getDesktopMode();
    int width = desktopMode.width / 2;
    int height = desktopMode.height / 2;

    RenderWindow window(VideoMode(width, height), "Mandelbrot Set");
    ComplexPlane complexPlane(width, height);
    Font font;
    Text text;

    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(Color::White);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Right) {
                    complexPlane.zoomOut();
                }
                else if (event.mouseButton.button == Mouse::Left) {
                    complexPlane.zoomIn();
                }
                complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
            }
            if (event.type == Event::MouseMoved) {
                complexPlane.setMouseLocation({ event.mouseMove.x, event.mouseMove.y });
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
            }
        }

        complexPlane.updateRender();
        complexPlane.loadText(text);

        window.clear();
        complexPlane.draw(window, RenderStates::Default);
        window.draw(text);
        window.display();
    }

    return 0;
}