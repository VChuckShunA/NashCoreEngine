#include <iostream>
#include <memory>
#include <fstream>

#include "SFML/Graphics.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"


class customCircle :public sf::CircleShape
{
private:
public:
	std::string shapeText;
	int positionX, positionY, speedX, speedY;
	float colour[3];
	int radius, width, height;
	sf::Text myText;
	sf::Font myFont;
	customCircle(std::string shapeText, int positionX, int positionY, int speedX, int speedY, int radius, float colours[3],std:: string fontPath)
		:shapeText(shapeText), positionX(positionX), positionY(positionY), speedX(speedX), speedY(speedY), radius(radius)
	{
		
		
		this->setRadius(radius);
		this->setPointCount(32);
		for (int i = 0; i < 3; ++i) {
			this->colour[i] = colours[i];
		}
		this->setPosition(positionX, positionY);
		this->setFillColor(sf::Color(colour[0] * 255, colour[1] * 255, colour[2] * 255));
		if (!myFont.loadFromFile(fontPath))
		{
			//if we can't load the font, print and error to the the console and exit
			std::cerr << "Could not load font! \n";
			exit(-1);
		}
		std::cout << shapeText << std::endl;
		myText.setString(shapeText);
		myText.setFont(myFont);
		myText.setCharacterSize(25);
		myText.setPosition(positionX, positionY);

	}
	
};

int main(int argc, char* argv[])
{
	//create new window of size w*h pixels
	//top-left of the window is (0,0) and bottom right is (w,h)
	// you will have to read these from the confic file
	std::ifstream fin("config.txt");
	std::string temp;
	int width;
	int height;
	int fontSize;
	std::string fontPath;
	//let's make a shape that we will draw to the screen
	float circleRadius;//radius to draw the circle 50
	int circleSegments = 32; //number of segments to draw the circle with 32
	float circleSpeedX;//we will use this to move the cirlce later 1.0
	float circleSpeedY; //you will read these values from the file 0.5
	bool drawCircle = true; // whether or not to draw the cricle
	bool drawText = true; //whether or not to draw the text
	float positionX;
	float positionY;
	float c1, c2, c3;
	std::vector<customCircle*> shapes;
	std::string shapeText;
	while (fin >> temp)
	{
		if (temp == "Window")
		{
			fin >> width >> height;
		}
		else if (temp == "Font")
		{
			fin>> fontPath >>fontSize;
		}
		else if (temp == "Circle")
		{
			fin >> shapeText >> positionX>>positionY>>circleSpeedX>>circleSpeedY>>c1>>c2>>c3>>circleRadius;
			//sf::CircleShape* circle = new sf::CircleShape(circleRadius, circleSegments); //create a circle shape with radius 50
			
			float newColour[3] = { c1,c2,c3 };
			customCircle* circle = new customCircle(shapeText,positionX,positionY,circleSpeedX,circleSpeedY,circleRadius,newColour,fontPath);
			circle->setPosition(positionX, positionY); // set the top left position of the circle
			circle->setFillColor(sf::Color(c1 * 255, c2 * 255, c3 * 255));
			shapes.push_back(circle); 
			std::cout << shapeText << " : " << positionX << positionY << " : " << circleSpeedX <<
				" : " << circleSpeedY << " : " << circleSpeedY << " : " << c1 << c2 << c3 << " : " << circleRadius << std::endl;

		}
	}

	const int wWidth = width;
	const int wHeight = height;
	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
	window.setFramerateLimit(60); //limit frame rate to 60 fps

	//initialize IMGUI and create a clock used for internal timing
	ImGui::SFML::Init(window);
	sf::Clock deltaClock;


	//scale the imgui ui by a given factorm does not affect text size
	ImGui::GetStyle().ScaleAllSizes(1.0f);

	//the imgui color {r,g,b} wheel requires floats from 0-1 instead of ints from 0-255
	float c[3] = { 0.1f,1.0f,1.0f };

	
	//create sfml cricle shape based on our parameters
	sf::CircleShape circle(circleRadius, circleSegments); //create a circle shape with radius 50

	circle.setPosition(10.0f, 10.0f); // set the top left position of the circle
	
	//let's load a font so we can display some text

	sf::Font myFont;

	//attemtp to load the font from a file
	if (!myFont.loadFromFile(fontPath))
	{
		//if we can't load the font, print and error to the the console and exit
		std::cerr << "Could not load font! \n";
		exit(-1);
	}

	//set up the text object that will be drawn to the screen
	sf::Text text("Sample Text", myFont, fontSize);

	//poisition the top left corner of the text so that the text aligns on the bottom
	//text character size is in pixels, so move the text up from the bottom by its height

	text.setPosition(0, wHeight - (float)text.getCharacterSize());

	//set up a character array to set the text
	char displayString[255] = "Sample Text";

	//main loop -  continues for each frame while window is open
	while (window.isOpen())
	{
		//event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			//pass the event to igui to be parsed
			ImGui::SFML::ProcessEvent(window, event);

			/// this event trggers when the window is closed
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			//this event is triggered when a key is pressed
			if (event.type == sf::Event::KeyPressed)
			{
				//print the key that was pressed to the console
				std::cout << "Key pressed with code = " << event.key.code << "\n";

				//example , what happens when x is pressed
				if (event.key.code == sf::Keyboard::X)
				{
					//reverse the x direction the circle on the screen
					circleSpeedX *= -1.0f;
				}
			}
		}

		//update imgui for this frame with the time that the last frame took
		ImGui::SFML::Update(window, deltaClock.restart());

		//draw the UI
		ImGui::Begin("Window title");
		ImGui::Text("Window text");
		ImGui::Checkbox("Draw Circle", &drawCircle);
		ImGui::SameLine();
		ImGui::Checkbox("Draw Text", &drawText);
		ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
		ImGui::SliderInt("Sides", &circleSegments, 3, 64);
		ImGui::ColorEdit3("Color Circle", c);
		ImGui::InputText("Text", displayString, 255);
		if (ImGui::Button("Set Text"))
		{
			text.setString(displayString);
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset Cricle"))
		{
			circle.setPosition(0, 0);
		}
		ImGui::End();

		//set the circle properties, because they may have been updated with the ui
		circle.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255));
		circle.setPointCount(circleSegments);
		circle.setRadius(circleRadius);

		//basic animation - move each from if it's still in frame
		circle.setPosition(circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY);
		for (customCircle* shape : shapes) {
			shape->setPosition(shape->getPosition().x + shape->speedX, shape->getPosition().y + shape->speedY);
			shape->myText.setPosition(shape->getPosition().x+shape->radius , shape->getPosition().y + shape->radius );
		}

		for (customCircle* shape : shapes) {
			window.draw(*shape);
		}

		
		//basic rendering function calls
		window.clear(); //clear the window of anything previously drawn
		if (drawCircle) //draw the ciricle if the boolean is true
		{
			window.draw(circle);
			for (sf::Shape* shape : shapes) {
				window.draw(*shape);
			}
		}
		if (drawText) //draw the text if the boolean is true
		{
			window.draw(text);

			for (customCircle* shape : shapes) {
				window.draw(shape->myText);
			}
		}
		ImGui::SFML::Render(window); //draw the ui last so it's on top
		window.display(); //cal the window display function
	}
	return 0;
}