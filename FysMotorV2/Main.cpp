//Veli-Pekka Anttonen TTV15SP

//SFML includet
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
//Omat includet
#include "Cube.h"
#include "Collision2D.h"
//Valamiit includet
#include <vector>
#include <Windows.h>
#include <iostream>


int main()
{
	//	Pidä nää niin kaikki näyttää OK!
	float Win_Wid = 960.f;
	float Win_Hei = 960.f;

	int Draw = 2;

	//Luodaan ikkuna
	sf::RenderWindow window(sf::VideoMode(Win_Wid, Win_Hei), "FysMotorV2");
	//Ikkunan paikan asettaminen näytölle
	window.setPosition(sf::Vector2i(200, 25));
	//Ruudunpäivityksen rajoittaminen (60fps)
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	//Luodaan kello ja deltatime
	sf::Clock Clock;
	Clock.restart();
	float DeltaTime;

	//Luodaan säiliö kuutioille ja sille iteraattori
	std::vector<Cube*>Cubes;
	std::vector<Cube*>::iterator Itr_Cube = Cubes.begin();
	std::vector<Cube*>::iterator Itr_Cube2 = Cubes.begin();
	//Luodaan säiliö seinille ja sille iteraattori
	std::vector<Cube>Walls;
	std::vector<Cube>::iterator Itr_Wall = Walls.begin();

	//Luodaan kuutioita
	Cube* Cube1 = new Cube(sf::Vector2f(50, 50), sf::Vector2f(Win_Hei/2, Win_Wid/2), sf::Vector2f(0.f, 0.f));
	Cube1->Mass = 1.f;
	Cube1->angularVelocity = 0.0f;
	Cube1->setColor(sf::Color::Green);
	Cubes.push_back(Cube1);
	Cube* Cube2 = new Cube(sf::Vector2f(50, 50), sf::Vector2f(500, 300), sf::Vector2f(0.f, 0.f));
	Cube2->Mass = 1.f;
	Cubes.push_back(Cube2);
	Itr_Cube2 = Cubes.end();

	//Luodaan muutama convexshape
	//sf::ConvexShape Con1;
	//Con1.setPointCount(3);
	//Con1.setPoint(0, sf::Vector2f(300.f, 300.f));
	//Con1.setPoint(1, sf::Vector2f(300.f, 350.f));
	//Con1.setPoint(2, sf::Vector2f(350.f, 350.f));
	//Con1.setFillColor(sf::Color::Red);
	//Con1.setPosition(400, 400);

	//sf::ConvexShape Con2;
	//Con2.setPointCount(4);
	//Con2.setPoint(0, sf::Vector2f(300.f, 300.f));
	//Con2.setPoint(1, sf::Vector2f(300.f, 350.f));
	//Con2.setPoint(2, sf::Vector2f(350.f, 350.f));
	//Con2.setPoint(3, sf::Vector2f(350.f, 300.f));
	//Con2.setFillColor(sf::Color::Magenta);
	//Con2.setPosition(400, 500);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//Luodaan seinät
	float wall_mass = 100000.f;
	Cube Wall_Top(sf::Vector2f(Win_Wid, 50.f), sf::Vector2f(Win_Wid / 2.f, 0.f), sf::Vector2f(0.f, 0.f));
	Wall_Top.setColor(sf::Color::Cyan);
	Wall_Top.Mass = wall_mass;
	Wall_Top.setDynamic(false);
	Wall_Top.isWall = true;
	Wall_Top.isVerticalWall = false;
	Walls.push_back(Wall_Top);
	Cube Wall_Bot(sf::Vector2f(Win_Wid, 50.f), sf::Vector2f(Win_Hei / 2.0f, Win_Hei), sf::Vector2f(0.f, 0.f));
	Wall_Bot.setColor(sf::Color::Cyan);
	Wall_Bot.Mass = wall_mass;
	Wall_Bot.setDynamic(false);
	Wall_Bot.isWall = true;
	Wall_Bot.isVerticalWall = false;
	Walls.push_back(Wall_Bot);
	Cube Wall_Lef(sf::Vector2f(50.f, Win_Hei), sf::Vector2f(0.f, Win_Wid / 2.0f), sf::Vector2f(0.f, 0.f));
	Wall_Lef.setColor(sf::Color::Cyan);
	Wall_Lef.Mass = wall_mass;
	Wall_Lef.setDynamic(false);
	Wall_Lef.isWall = true;
	Wall_Lef.isVerticalWall = true;
	Walls.push_back(Wall_Lef);
	Cube Wall_Rig(sf::Vector2f(50.f, Win_Hei), sf::Vector2f(Win_Wid, Win_Wid / 2.0f), sf::Vector2f(0.f, 0.f));
	Wall_Rig.setColor(sf::Color::Cyan);
	Wall_Rig.Mass = wall_mass;
	Wall_Rig.setDynamic(false);
	Wall_Rig.isWall = true;
	Wall_Rig.isVerticalWall = true;
	Walls.push_back(Wall_Rig);
	////////////////////////////////////////////////////////////////////////////////////////////////////

	//Luodaan törmäystarkistus
	Collision2D Tarkistaja(&window);


	//Luodaan voimia
	sf::Vector2f Force(0.f, 0.f);
	sf::Vector2f Zero_Velocity(0.f, 0.f);

	sf::Event event;
	//Ikkuna Loop!
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			//Lopetuskäsky (näppäin 0 tai Esc)
			if (GetAsyncKeyState(0x30) || GetAsyncKeyState(0x1B)) //0 tai esc sammuttaa ohjelman
			{
				window.close();
				Sleep(200);
			}
		}
		//DeltaTimen asettaminen
		DeltaTime = Clock.restart().asSeconds();
		// Kuutioiden iteraattorin asettaminen
		Itr_Cube = Cubes.begin();

		////////////////////////////////////////////////////////////////////////////////////////////////////

		//Jos painetaan nuolinäppäimiä, jotain tulisi tapahtua

		if (GetAsyncKeyState(0x60))	//Numpad 0
		{
			Zero_Velocity = sf::Vector2f(0.f, 0.f);
			Cube1->Velocity = Zero_Velocity;
			Cube1->angularVelocity = 0;
		}
		else if (GetAsyncKeyState(0x61)) //Numpad1
		{
			Cube1->angularVelocity += 1.f;
		}
		else if (GetAsyncKeyState(0x62)) //Numpad2
		{
			Cube1->angularVelocity -= 1.f;
		}
		else if (GetAsyncKeyState(0x63)) //Numpad3
		{
			Cube1->setPosition(sf::Vector2f(Win_Hei / 2, Win_Wid / 2));
			Cube2->setPosition(sf::Vector2f(Win_Hei / 2 + 100, Win_Wid / 2 + 100));

			Cube1->Velocity = Zero_Velocity;
			Cube1->angularVelocity = 0;

			Cube2->Velocity = Zero_Velocity;
			Cube2->angularVelocity = 0;
		}
		else if (GetAsyncKeyState(0x64)) //Numpad4
		{
			//	Tulostaa cube 1:n tiedot
			std::cout << "Cube1: " << std::endl;
			std::cout << "Position: " << Cube1->getPosition().x << " x, " << Cube1->getPosition().y <<" y." << std::endl;
			std::cout << "Velocity:" << Cube1->getVelocity().x << " x, " << Cube1->getVelocity().y << " y." << std::endl << std::endl;
			std::cout << "AngularVelocity:" << Cube1->getAngularVelocity() << " degrees" << std::endl << std::endl;
		}
		else if (GetAsyncKeyState(0x65)) //Numpad5
		{
			//	Tulostaa cube 2:n tiedot
			std::cout << "Cube2: " << std::endl;
			std::cout << "Position: " << Cube2->getPosition().x << " x, " << Cube2->getPosition().y << " y." << std::endl;
			std::cout << "Velocity:" << Cube2->getVelocity().x << " x, " << Cube2->getVelocity().y << " y." << std::endl << std::endl ;
		}
		else
		{

		}

		// Vihreän kuution liikuttaminen
		if (GetAsyncKeyState(0x26)) //Up arrow key
		{
			(*Itr_Cube)->Velocity += sf::Vector2f(0.f, -100.f) * DeltaTime;
		}
		else if (GetAsyncKeyState(0x28)) //Down arrow key
		{
			(*Itr_Cube)->Velocity += sf::Vector2f(0.f, 100.f) * DeltaTime;
		}
		else if (GetAsyncKeyState(0x25)) //Left arrow key
		{
			(*Itr_Cube)->Velocity += sf::Vector2f(-100.f, 0.f) * DeltaTime;
		}
		else if (GetAsyncKeyState(0x27)) //Right arrow key
		{
			(*Itr_Cube)->Velocity += sf::Vector2f(100.f, 0.f) * DeltaTime;
		}
		else
		{

		}


		////////////////////////////////////////////////////////////////////////////////////////////////////

		//Törmäyksen tarkistus
		Tarkistaja.SAT(*Cubes.at(1), *Cubes.at(0));

		for (auto w : Walls)
		{
			Tarkistaja.SAT(w, *Cubes.at(0));
			Tarkistaja.SAT(w, *Cubes.at(1));
		}

		//Ikkunan putsaus
		window.clear();

		////////////////////////////////////////////////////////////////////////////////////////////////////

		//Cubejen päivitys ja piirto
		for (Itr_Cube = Cubes.begin(); Itr_Cube != Cubes.end(); Itr_Cube++)
		{
			(*Itr_Cube)->Update(DeltaTime);
			(*Itr_Cube)->DrawConvexShape(window);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////

		//Seinien piirto ja päivitys
		for (Itr_Wall = Walls.begin(); Itr_Wall != Walls.end(); Itr_Wall++)
		{
			Itr_Wall->Update(DeltaTime);
			Itr_Wall->DrawConvexShape(window);
		}

		// CollisionPointin piirto
		Tarkistaja.DrawCollisionPoint();

		window.display();
	}

	for (auto c : Cubes)
	{
		auto tmp = c;
		c = nullptr;
		delete tmp;
	}

	return 0;
}
