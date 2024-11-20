#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
    m_pixel_size = { pixelWidth, pixelHeight };
    m_aspectRatio = (float)pixelHeight / pixelWidth; // Calculate aspect ratio
    m_plane_center = { 0, 0 };
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_state = State::CALCULATING;

    // Initialize VertexArray
    m_vArray.setPrimitiveType(sf::Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
    target.draw(m_vArray);
}

void ComplexPlane::updateRender() {
    if (m_state == State::CALCULATING) {
        for (int j = 0; j < m_pixel_size.x; ++j) {
            for (int i = 0; i < m_pixel_size.y; ++i) {
                m_vArray[j + i * m_pixel_size.x].position = { (float)j, (float)i };
                Vector2f coord = mapPixelToCoords({ j, i });
                size_t iterations = countIterations(coord);
                Uint8 r, g, b;
                iterationsToRGB(iterations, r, g, b);
                m_vArray[j + i * m_pixel_size.x].color = { r, g, b };
            }
        }
        m_state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn() {
    m_zoomCount++;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { xSize, ySize };
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    m_zoomCount--;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { xSize, ySize };
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) {
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel) {
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text) {
    std::stringstream ss;
    ss << "Mouse Position: " << m_mouseLocation.x << ", " << m_mouseLocation.y << "\n";
    ss << "Current Center: " << m_plane_center.x << ", " << m_plane_center.y;
    text.setString(ss.str());
}

int ComplexPlane::countIterations(Vector2f coord) {
    // Implementation of iteration counting
    int count = 0;
    std::complex<float> z(0, 0);
    std::complex<float> c(coord.x, coord.y);
    while (std::abs(z) < 2.0f && count < MAX_ITER) {
        z = z * z + c;
        count++;
    }
    return count;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {
    // Implementation of color mapping
    if (count == MAX_ITER) {
        r = g = b = 0; // Black for max iterations
    }
    else {
        // Color mapping based on the number of iterations
        float t = (float)count / MAX_ITER;
        r = static_cast<Uint8>(9 * (1 - t) * t * t * t * 255);
        g = static_cast<Uint8>(15 * (1 - t) * (1 - t) * t * t * 255);
        b = static_cast<Uint8>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    }
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {
    float x = ((float)mousePixel.x / m_pixel_size.x) * (m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
    float y = ((float)(m_pixel_size.y - mousePixel.y) / m_pixel_size.y) * (m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);
    return { x, y };
}