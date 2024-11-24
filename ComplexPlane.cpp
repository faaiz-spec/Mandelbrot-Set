#include "ComplexPlane.h"
#include <complex>
#include <cmath>
#include <thread>

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_vArray(sf::Points, pixelWidth* pixelHeight), m_pixelSize(pixelWidth, pixelHeight),
    m_planeCenter(-0.5f, 0.0f), m_planeSize(3.0f, 2.0f), m_zoomCount(0) {
    m_aspectRatio = static_cast<float>(pixelWidth) / pixelHeight;
    m_planeSize.x *= m_aspectRatio; // Maintain aspect ratio
    updateRender();
}

// Convert pixel to complex coordinates
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) const {
    float real = m_planeCenter.x + (mousePixel.x - m_pixelSize.x / 2.0f) * (m_planeSize.x / m_pixelSize.x);
    float imag = m_planeCenter.y + (mousePixel.y - m_pixelSize.y / 2.0f) * (m_planeSize.y / m_pixelSize.y);
    return sf::Vector2f(real, imag);
}

// Count Mandelbrot iterations
int ComplexPlane::countIterations(sf::Vector2f coord) const {
    const int maxIterations = 1000;
    float x = 0, y = 0;
    int iteration = 0;

    while (x * x + y * y < 4 && iteration < maxIterations) {
        float xTemp = x * x - y * y + coord.x;
        y = 2 * x * y + coord.y;
        x = xTemp;
        iteration++;
    }
    return iteration;
}

// Map iterations to RGB color
void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const {
    if (count == 1000) {
        r = g = b = 0; // Black for points inside the set
    }
    else {
        r = count % 256;
        g = (count / 3) % 256;
        b = (count / 5) % 256;
    }
}

// Update vertex array with Mandelbrot values
void ComplexPlane::updateRender() {
    for (int y = 0; y < m_pixelSize.y; ++y) {
        for (int x = 0; x < m_pixelSize.x; ++x) {
            sf::Vector2f coord = mapPixelToCoords({ x, y });
            size_t iterations = countIterations(coord);

            sf::Uint8 r, g, b;
            iterationsToRGB(iterations, r, g, b);

            m_vArray[y * m_pixelSize.x + x].position = sf::Vector2f(x, y);
            m_vArray[y * m_pixelSize.x + x].color = sf::Color(r, g, b);
        }
    }
}

// Zoom in
void ComplexPlane::zoomIn() {
    m_planeSize /= 1.5f;
    updateRender();
}

// Zoom out
void ComplexPlane::zoomOut() {
    m_planeSize *= 1.5f;
    updateRender();
}

// Set new center based on mouse click
void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
    m_planeCenter = mapPixelToCoords(mousePixel);
    updateRender();
}

// Update mouse location
void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

// Draw the plane
void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray, states);
}