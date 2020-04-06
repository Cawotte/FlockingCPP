#pragma once

#define _USE_MATH_DEFINES
#include <math.h> 

namespace Utils {

	namespace Vector2 {

		//DECLARATION

		//Default Vectors
		template <class Vec2 = sf::Vector2f>
		Vec2 up();

		template <class Vec2 = sf::Vector2f>
		Vec2 down();

		template <class Vec2 = sf::Vector2f>
		Vec2 right();

		template <class Vec2 = sf::Vector2f>
		Vec2 left();

		template <class Vec2 = sf::Vector2f>
		Vec2 zero();

		//Methods
		template <class Vec2 = sf::Vector2f>
		float getMagnitude(const Vec2& vector);

		template <class Vec2 = sf::Vector2f>
		float getAngleDegree(const Vec2& vector);

		template <class Vec2 = sf::Vector2f>
		float getAngleRadian(const Vec2& vector);

		template <class Vec2 = sf::Vector2f>
		Vec2 normalized(const Vec2& vector);

		// IMPLEMENTATION

		//Default Vectors
		template <class Vec2>
		Vec2 up()
		{
			return Vec2(0., -1.);
		}

		template <class Vec2>
		Vec2 down()
		{
			return Vec2(0., 1.);
		}

		template <class Vec2>
		Vec2 right()
		{
			return Vec2(1., 0.);
		}

		template <class Vec2>
		Vec2 left()
		{
			return Vec2(-1., 0.);
		}

		template <class Vec2>
		Vec2 zero()
		{
			return Vec2(0., 0.);
		}
	
		//Methods
		template <class Vec2>
		float getMagnitude(const Vec2& vector)
		{
			return sqrt((vector.x * vector.x) + (vector.y * vector.y));
		}

		template <class Vec2>
		float getAngleRadian(const Vec2& vector)
		{
			return atan2(vector.x, -vector.y);
		}

		template <class Vec2>
		float getAngleDegree(const Vec2& vector)
		{
			return getAngleRadian(vector) * 180 / M_PI;
		}

		template <class Vec2>
		Vec2 normalized(const Vec2& vector)
		{
			return vector / getMagnitude<Vec2>(vector);
		}

	}



}


