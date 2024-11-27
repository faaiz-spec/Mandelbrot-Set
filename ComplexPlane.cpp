#include "ComplexPlane.h"

using namespace sf;
using namespace std;

// i found it a little hard to read with this notation so i just changed it to be simpler
// Construct a complex plane
ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixelSize = { pixelWidth, pixelHeight };
    m_aspectRatio = (static_cast<float>(pixelHeight) / pixelWidth);
    m_planeCenter = { 0,0 };
    m_planeSize = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_state = State::CALCULATING;
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

// Convert pixel to complex coordinates
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) const
{
    float real = m_planeCenter.x + (mousePixel.x - m_pixelSize.x / 2.0f) * (m_planeSize.x / m_pixelSize.x);
    float imag = m_planeCenter.y + (mousePixel.y - m_pixelSize.y / 2.0f) * (m_planeSize.y / m_pixelSize.y);
    return sf::Vector2f(real, imag);
}

// Count Mandelbrot iterations
int ComplexPlane::countIterations(sf::Vector2f coord) const 
{
    float re, im, reTemp;
    int iterations;
    re = 0; im = 0;
    iterations = 0;

    while (re * re + im * im < 4 && iterations < MAX_ITER) {
        reTemp = re * re - im * im + coord.x;
        im = 2 * re * im + coord.y;
        re = reTemp;
        iterations++;
    }
    return iterations;
}

// Map iterations to RGB color with HSV color sliding
void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const
{
    static int slider = 1530 / MAX_ITER;
    if (count == MAX_ITER)
        r = g = b = 0;
    else
    {
        if (count <= MAX_ITER / 6)
        {
            r = 255;
            g = count * slider;
            b = 0;
        }
        else if (count <= 2 * MAX_ITER / 6)
        {
            r = 510 - count * slider;
            g = 255;
            b = 0;
        }
        else if (count <= 3 * MAX_ITER / 6)
        {
            r = 0;
            g = 255;
            b = -510 + count * slider;
        }
        else if (count <= 4 * MAX_ITER / 6)
        {
            r = 0;
            g = 1020 - count * slider;
            b = 255;
        }
        else if (count <= 5 * MAX_ITER / 6)
        {
            r = -1020 + count * slider;
            g = 0;
            b = 255;
        }
        else
        {
            r = 255;
            g = 0;
            b = 1530 - count * slider;
        }
    }
}

// Update vertex array with Mandelbrot values and multithreading
void ComplexPlane::updateRender()
{
    if (m_state == CALCULATING)
    {
        const int THREAD_COUNT = std::thread::hardware_concurrency();
        const int rowsPerThread = m_pixelSize.y / THREAD_COUNT;
        std::vector<std::thread> threads;

        for (int t = 0; t < THREAD_COUNT; ++t) {
            int startRow = t * rowsPerThread;
            int endRow = (t == THREAD_COUNT - 1) ? m_pixelSize.y : (t + 1) * rowsPerThread;

            threads.emplace_back(&ComplexPlane::calculateRow, this, startRow, endRow);
        }

        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        m_state = DISPLAYING;
    }
}

void ComplexPlane::calculateRow(int startRow, int endRow)
{
    for (int i = startRow; i < endRow; ++i)
        for (int j = 0; j < m_pixelSize.x; ++j)
        {
            m_vArray[j + i * m_pixelSize.x].position = { (float)j, (float)i };
            size_t count = countIterations(mapPixelToCoords({ j,i }));
            Uint8 r, g, b;
            iterationsToRGB(count, r, g, b);
            m_vArray[j + i * m_pixelSize.x].color = { r, g, b };
            m_state = DISPLAYING;
        }

}

// Zoom in
void ComplexPlane::zoomIn() 
{
    ++m_zoomCount;
    float x_size = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
    float y_size = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
    m_planeSize = { x_size, y_size };
    m_state = CALCULATING;
}

// Zoom out
void ComplexPlane::zoomOut() 
{
    --m_zoomCount;
    float x_size = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
    float y_size = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
    m_planeSize = { x_size, y_size };
    m_state = CALCULATING;
}

// Set new center based on mouse click
void ComplexPlane::setCenter(sf::Vector2i mousePixel)
{
    m_planeCenter = mapPixelToCoords(mousePixel);
}

// Update mouse location
void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) 
{
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

// Update the text with the values
void ComplexPlane::updateText(Text& text)
{
    stringstream textToPass;

    textToPass << "Mandlebrot Set\n"
        << "Center: (" << m_planeCenter.x << ", " << m_planeCenter.y << ")\n"
        << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n"
        << "Left Click to zoom in\n"
        << "Right Click to zoom out";

    text.setString(textToPass.str());
}

// Draw the plane
void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vArray);
}