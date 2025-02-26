#pragma once
#include <SFML/Graphics.hpp>

class Point {
public:
	Point(std::string tag, sf::FloatRect boundary, int index) {
		this->tag = tag;
		this->boundary = boundary;
		this->index = index;
	}
	std::string tag;
	sf::FloatRect boundary;
	int index;
};

class Quadtree
{
private :
	sf::FloatRect boundary;
	int capacity;

	std::vector<Point*> points;

	//Children
	Quadtree* northWest;
	Quadtree* northEast;
	Quadtree* southWest;
	Quadtree* southEast;

	void subdivide();

public: 
	Quadtree(sf::FloatRect boundary);

	  bool insert(Point* point);
	  std::vector<Point*> queryRange(sf::FloatRect range);

	  void show(sf::RenderWindow& window);

	  void clear();

};

