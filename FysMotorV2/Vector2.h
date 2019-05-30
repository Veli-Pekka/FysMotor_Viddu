#pragma once
#include <math.h>
#include <SFML\System\Vector2.hpp>
class Vector2
{
	float x;
	float y;
public:
	Vector2();
	Vector2(float x, float y);
	~Vector2();

			//Operaattorit
	//Plus-yhtäkuin eli plussattava muuttuu
	Vector2& operator+= (const Vector2& v1);
	//Miinus-yhtäkuin eli miinusatettava muuttuu
	Vector2& operator-= (const Vector2& v1);
	//Plus eli palauttaa uuden vektorin
	Vector2 operator+ (const Vector2& v1);
	//Miinus eli palauttaa uuden vektorin
	Vector2 operator- (const Vector2& v1);

	//Pienempi kuin-vertailu
	bool operator< (const Vector2& v1);
	//Suurempi kuin-vertailu 
	bool operator> (const Vector2& v1);
	//Suurempi kuin pituus
	bool operator> (const double& vPituus);

	//Kertominen
	Vector2 operator* (const Vector2& v1);


};