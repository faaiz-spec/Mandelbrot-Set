#pragma once
#ifndef ComplexPlane_H
#define ComplexPlane_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include <thread>
#include <vector>

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum State
{
	CALCULATING,
	DISPLAYING
};

class ComplexPlane : public sf::Drawable
{
public:
	ComplexPlane(int pixelWidth, int pixelHeight);
	void draw(sf::RenderTarget& target, sf::RenderStates States) const; // draw should actually be public, but i dont think theres much difference
	void zoomIn();
	void zoomOut();
	void setCenter(sf::Vector2i mousePixel);
	void setMouseLocation(sf::Vector2i mousePixel);
	void updateText(sf::Text& text);
	void updateRender();

private:
	int countIterations(sf::Vector2f coord) const;
	void calculateRow(int startRow, int endRow);
	void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const;
	sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel) const;

	State m_state;
	sf::VertexArray m_vArray;
	sf::Vector2f m_mouseLocation;
	sf::Vector2i m_pixelSize;
	sf::Vector2f m_planeCenter;
	sf::Vector2f m_planeSize;
	int m_zoomCount;
	float m_aspectRatio;
};

#endif 