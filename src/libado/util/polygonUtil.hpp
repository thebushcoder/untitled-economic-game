/*
 * polygonUtil.hpp
 *
 *  Created on: 20 Feb 2018
 *      Author: linbob
 */

#ifndef LIBADO_UTIL_POLYGONUTIL_HPP_
#define LIBADO_UTIL_POLYGONUTIL_HPP_

#include <SFML/Graphics.hpp>

namespace PolygonUtil{

	// Given three colinear points p, q, r, the function checks if
	// point q lies on line segment 'pr'
	static bool onSegment(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r){
		if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
				q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
			return true;
		return false;
	}

	// To find orientation of ordered triplet (p, q, r).
	// The function returns following values
	// 0 --> p, q and r are colinear
	// 1 --> Clockwise
	// 2 --> Counterclockwise
	static int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r){
		int val = (q.y - p.y) * (r.x - q.x) -
				  (q.x - p.x) * (r.y - q.y);

		if (val == 0) return 0;  // colinear
		return (val > 0)? 1: 2; // clock or counterclock wise
	}

	// The function that returns true if line segment 'p1q1'
	// and 'p2q2' intersect.
	static bool doIntersect(sf::Vector2f p1, sf::Vector2f q1, sf::Vector2f p2, sf::Vector2f q2){
		// Find the four orientations needed for general and
		// special cases
		int o1 = orientation(p1, q1, p2);
		int o2 = orientation(p1, q1, q2);
		int o3 = orientation(p2, q2, p1);
		int o4 = orientation(p2, q2, q1);

		// General case
		if (o1 != o2 && o3 != o4)
			return true;

		// Special Cases
		// p1, q1 and p2 are colinear and p2 lies on segment p1q1
		if (o1 == 0 && onSegment(p1, p2, q1)) return true;

		// p1, q1 and p2 are colinear and q2 lies on segment p1q1
		if (o2 == 0 && onSegment(p1, q2, q1)) return true;

		// p2, q2 and p1 are colinear and p1 lies on segment p2q2
		if (o3 == 0 && onSegment(p2, p1, q2)) return true;

		 // p2, q2 and q1 are colinear and q1 lies on segment p2q2
		if (o4 == 0 && onSegment(p2, q1, q2)) return true;

		return false; // Doesn't fall in any of the above cases
	}

	// Returns true if the point p lies inside the polygon[] with n vertices
	// https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
	static bool isInside(std::deque<sf::Vector2f>& polygon, int n, sf::Vector2f p)
	{
		// There must be at least 3 vertices in polygon[]
		if (n < 3)  return false;

		// Create a point for line segment from p to infinite
		sf::Vector2f extreme = {99999, p.y};

		// Count intersections of the above line with sides of polygon
		int count = 0, i = 0;
		do
		{
			int next = (i+1)%n;

			// Check if the line segment from 'p' to 'extreme' intersects
			// with the line segment from 'polygon[i]' to 'polygon[next]'
			if (doIntersect(polygon[i], polygon[next], p, extreme))
			{
				// If the point 'p' is colinear with line segment 'i-next',
				// then check if it lies on segment. If it lies, return true,
				// otherwise false
				if (orientation(polygon[i], p, polygon[next]) == 0)
				   return onSegment(polygon[i], p, polygon[next]);

				count++;
			}
			i = next;
		} while (i != 0);

		// Return true if count is odd, false otherwise
		return count & 1;  // Same as (count%2 == 1)
	}

	static int Is_Left(const sf::Vector2f &p0,
					const sf::Vector2f &p1,
					const sf::Vector2f &point)
	{
		return ((p1.x - p0.x) * (point.y - p0.y) -
				(point.x - p0.x) * (p1.y - p0.y));
	}

	// http://forums.codeguru.com/showthread.php?497679-To-check-if-a-point-is-inside-a-polygon
	static bool windingAlgo(sf::Vector2f p, std::deque<sf::Vector2f>& polygonPoints){
		// The winding number counter.
		int winding_number = 0;

		// Loop through all edges of the polygon.

		for (auto i = 0; i < polygonPoints.size(); ++i)             // Edge from point1 to points_list[i+1]
		{
			sf::Vector2f point1(polygonPoints[i]);
			sf::Vector2f point2;

			// Wrap?
			if (i == (polygonPoints.size() - 1))
			{
				point2 = polygonPoints[0];
			}
			else
			{
				point2 = polygonPoints[i + 1];
			}

			if (point1.y <= p.y)                                    // start y <= point.y
			{
				if (point2.y > p.y)                                 // An upward crossing
				{
					if (Is_Left(point1, point2, p) > 0)             // Point left of edge
					{
						++winding_number;                               // Have a valid up intersect
					}
				}
			}
			else
			{
				// start y > point.y (no test needed)
				if (point2.y <= p.y)                                // A downward crossing
				{
					if (Is_Left(point1, point2, p) < 0)             // Point right of edge
					{
						--winding_number;                               // Have a valid down intersect
					}
				}
			}
		}

		return (winding_number != 0);
	}

	// https://www.codeproject.com/Tips/84226/Is-a-Point-inside-a-Polygon
	static bool pnPoly(std::deque<sf::Vector2f>& polygon, int n, sf::Vector2f p){
		bool flag = false;

		int i, j;
		for (i = 0, j = n - 1; i < n; j = i++) {
			sf::Vector2f a = polygon[i];
			sf::Vector2f b = polygon[j];
			if ( ((a.y > p.y) != (b.y > p.y)) &&
					(p.x < (b.x - a.x) * (p.y - a.y) /
							(b.y - a.y) + a.x) ){
			   flag = !flag;
			}
		}

		return flag;
	}

}



#endif /* LIBADO_UTIL_POLYGONUTIL_HPP_ */
