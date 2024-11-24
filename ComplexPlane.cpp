#include "ComplexPlane.h"
#include <complex>
#include <cmath>
#include <thread>

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
    m_pixelSize = { pixelWidth, pixelHeight };
    m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;
    m_planeCenter = { 0, 0 };
    m_planeSize = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_state = State::CALCULATING;
    m_vArray.setPrimitiveType(sf::Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray, states);
}

void ComplexPlane::updateRender() {
    if (m_state == State::CALCULATING) {
        std::vector<std::thread> threads;
        for (int j = 0; j < m_pixelSize.x; j++) {
            threads.emplace_back([this, j]() {
                for (int i = 0; i < m_pixelSize.y; i++) {
                    m_vArray[j + i * m_pixelSize.x].position = { static_cast<float>(j), static_cast<float>(i) };
                    sf::Vector2f coord = mapPixelToCoords({ j, i });
                    size_t iterations = countIterations(coord);
                    sf::Uint8 r, g, b;
                    iterationsToRGB(iterations, r, g, b);
                    m_vArray[j + i * m_pixelSize.x].color = { r, g, b };
                }
                });
        }
        for (auto& thread : threads) {
            thread.join();
        }
        m_state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn() {
    m_zoomCount++;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_planeSize = { xSize, ySize };
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    m_zoomCount--;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_planeSize = { xSize, ySize };
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
    m_planeCenter = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(sf::Text& text) {
    text.setString("This is my text!");
    sf::Font font;
    if (!font.loadFromFile("path/to/font.ttf")) {
        // Handle error
    }
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(100, 100));
}

int ComplexPlane::countIterations(sf::Vector2f coord) {
    std::complex<float> z(0, 0);
    std::complex<float> c(coord.x, coord.y);
    int iterations = 0;

    while (std::abs(z) <= 2.0 && iterations < MAX_ITER) {
        z = z * z + c;
        iterations++;
    }
    return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {
    if (count == MAX_ITER) {
        r = g = b = 0; // Black for points in the set
    }
    else {
        float t = static_cast<float>(count) / MAX_ITER;
        r = static_cast<sf::Uint8>(9 * (1 - t) * t * t * t * 255);
        g = static_cast<sf::Uint8>(15 * (1 - t) * (1 - t) * t * t * 255);
        b = static_cast<sf::Uint8>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    }
}

sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {
    float x = ((static_cast<float>(mousePixel.x) / m_pixelSize.x) * m_planeSize.x) + (m_planeCenter.x - m_planeSize.x / 2.0f);
    float y = ((static_cast<float>(mousePixel.y) / m_pixelSize.y) * m_planeSize.y) + (m_planeCenter.y - m_planeSize.y / 2.0f);
    return { x, y };
}