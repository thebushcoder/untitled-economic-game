/*
 * lineShape.cpp
 *
 *  Created on: 22 Dec 2017
 *      Author: linbob
 */

#include "lineShape.hpp"
#include <cmath>

namespace sf
{

LineShape::LineShape(const Vector2f& point1, const Vector2f& point2):
    m_direction(point2 - point1), pointA(point1), pointB(point2)
{
    setPosition(point1);
    setThickness(2.f);
}


void LineShape::setThickness(float thickness)
{
    m_thickness = thickness;
    update();
}


float LineShape::getThickness() const
{
    return m_thickness;
}

Vector2f LineShape::getPointA(){
	return pointA;
}
Vector2f LineShape::getPointB(){
	return pointB;
}

float LineShape::getLength() const
{
    return std::sqrt(m_direction.x*m_direction.x+m_direction.y*m_direction.y);
}


std::size_t LineShape::getPointCount() const
{
    return 4;
}


Vector2f LineShape::getPoint(std::size_t index) const
{
    Vector2f unitDirection = m_direction/getLength();
    Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

    Vector2f offset = (m_thickness/2.f)*unitPerpendicular;

    switch (index)
    {
        default:
        case 0: return offset;
        case 1: return (m_direction + offset);
        case 2: return (m_direction - offset);
        case 3: return (-offset);
    }
}

} // namespace sf


