#include "Quadtree.h"
#include <iostream>
void Quadtree::subdivide()
{
	float x = boundary.left;
	float y = boundary.top;
	float w = boundary.width;
	float h = boundary.height;

	northEast = new Quadtree(sf::FloatRect(x, y, w / 2, h / 2));
	northWest = new Quadtree(sf::FloatRect(x + w / 2, y, w / 2, h / 2));
	southEast = new Quadtree(sf::FloatRect(x, y + h / 2, w / 2, h / 2));
	southWest = new Quadtree(sf::FloatRect(x + w / 2, y + h / 2, w / 2, h / 2));

}

Quadtree::Quadtree(sf::FloatRect boundary)
{
	this->boundary = boundary;
	this->capacity = 4;

}

bool Quadtree::insert(Point* point)
{
	if (!boundary.contains(sf::Vector2f(point->boundary.left, point->boundary.top))) {
		return false;
	}

	if (points.size() < capacity && northWest == nullptr) {
		points.push_back(point);
		return true;
	}

	if (northWest == nullptr) {
		subdivide();
	}

	if (northWest->insert(point)) return true;
	if (northEast->insert(point)) return true;
	if (southWest->insert(point)) return true;
	if (southEast->insert(point)) return true;

	return false;
}

std::vector<Point*> Quadtree::queryRange(sf::FloatRect range)
{
	std::vector<Point*> pointsInRange;
	if (!this->boundary.intersects(range)) {
		return pointsInRange;
	}

	for (int i = 0; i < points.size(); i++) {
		pointsInRange.push_back(points[i]);
	}
	if (northWest == nullptr) {
		return pointsInRange;
	}
	std::vector<Point*> temp;
	temp = northWest->queryRange(range);
	pointsInRange.insert(pointsInRange.end(), temp.begin(), temp.end());

	temp = northEast->queryRange(range);
	pointsInRange.insert(pointsInRange.end(), temp.begin(), temp.end());

	temp = southEast->queryRange(range);
	pointsInRange.insert(pointsInRange.end(), temp.begin(), temp.end());

	temp = southWest->queryRange(range);
	pointsInRange.insert(pointsInRange.end(), temp.begin(), temp.end());

	return pointsInRange;
}

void Quadtree::show(sf::RenderWindow& window)
{
	sf::RectangleShape shape(sf::Vector2f(boundary.width, boundary.height));
	shape.setPosition(boundary.left, boundary.top);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	window.draw(shape);

	if (northWest != nullptr) {
		northWest->show(window);
		northEast->show(window);
		southWest->show(window);
		southEast->show(window);
	}
}

void Quadtree::clear()
{
	points.clear();
	if (northWest) delete northWest;
	if (northEast) delete northEast;
	if (southWest) delete southWest;
	if (southEast) delete southEast;

	northWest = nullptr;
	northEast = nullptr;
	southWest = nullptr;
	southEast = nullptr;
}
