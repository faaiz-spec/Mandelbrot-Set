#pragma once
#ifndef ComplexPlane_H
#define ComplexPlane_H

#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

class ComplexPlane : public sf::Drawable {
private:
    sf::VertexArray m_vArray;    // Vertex array for rendering
    sf::RenderStates m_state;   // Render states
    sf::Vector2f m_mouseLocation; // Current mouse position in world coordinates
    sf::Vector2i m_pixelSize;   // Size of the plane in pixels
    sf::Vector2f m_planeCenter; // Center of the complex plane
    sf::Vector2f m_planeSize;   // Size of the plane in complex coordinates
    int m_zoomCount;            // Zoom level
    float m_aspectRatio;        // Aspect ratio of the display

   
    // Internal functions
public:
    int countIterations(sf::Vector2f coord) const;
    void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const;
    sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel) const;
    void updateRender();
    ComplexPlane(int pixelWidth, int pixelHeight);
    void zoomIn();
    void zoomOut();
    void setCenter(sf::Vector2i mousePixel);
    void setMouseLocation(sf::Vector2i mousePixel);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif 