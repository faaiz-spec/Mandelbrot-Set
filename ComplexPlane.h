#pragma once
#ifndef ComplexPlane_H
#define ComplexPlane_H

#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum class State { CALCULATING, DISPLAYING };

class ComplexPlane : public sf::Drawable {
public:
	ComplexPlane(int pixelWidth, int pixelHeight);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void zoomIn();
	void zoomOut();

	void setCenter(sf::Vector2i mousePixel);
	void setMouseLocation(sf::Vector2i mousePixel);

	void loadText(sf::Text& text);
	void updateRender();

	int countIterations(sf::Vector2f coord);
	void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);
	sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel);

private:
	sf::VertexArray m_vArray;
	State m_state;
	sf::Vector2f m_mouseLocation;
	sf::Vector2i m_pixelSize;
	sf::Vector2f m_planeCenter;
	sf::Vector2f m_planeSize;
	int m_zoomCount;
	float m_aspectRatio;
};

#endif 