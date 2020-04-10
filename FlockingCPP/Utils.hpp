#pragma once

#define _USE_MATH_DEFINES //Pour utiliser PI
#include <math.h> 

//UniqueVector
#include <vector>
#include <memory>

#include "SFML/Graphics.hpp"

namespace utils {

	namespace graphics {

		/* Get a Drawable line for SFML, show the given vector from the starting point with the given color */
		sf::Drawable* getVectorShape(const sf::Vector2f start, const sf::Vector2f vector, const sf::Color color);

	}
	namespace vector {

		template<class T>
		class UniqueVector
		{

		private:

			std::vector<std::unique_ptr<T>> master;


		public:

			UniqueVector() {}

			UniqueVector(int size) : master(std::vector<std::unique_ptr<T>>(size)) {}

			std::vector<std::unique_ptr<T>>* getMaster()
			{
				return &master;
			}

			std::vector<const T*> getAllEntries()
			{
				std::vector<const T*> entries;

				for (auto const& ptr : master) {

					entries.push_back(ptr.get());
				}
				return entries;
			}

			template <class V>
			std::vector<V*> getEntriesOfType()
			{
				std::vector<V*> entries;

				for (auto const& ptr : master)
				{
					V* elm = dynamic_cast<V*>(ptr.get());

					//Add to list elm if of desired type
					if (elm != nullptr) {
						entries.push_back(elm);
					}

				}
				return entries;
			}

		};

	}
	namespace vector2 {

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
		float getAngleRadian(const Vec2& vector);

		template <class Vec2 = sf::Vector2f>
		float getAngleDegree(const Vec2& vector);

		template <class Vec2 = sf::Vector2f>
		Vec2 getVector2FromRadian(const float radian);

		template <class Vec2 = sf::Vector2f>
		Vec2 getVector2FromDegree(const float degree);

		template <class Vec2 = sf::Vector2f>
		Vec2 getRandom(const int maxX, const int maxY);

		template <class Vec2 = sf::Vector2f>
		float getMagnitude(const Vec2& vector);

		template <class Vec2 = sf::Vector2f>
		float getDistance(const Vec2& a, const Vec2& b);

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
		float getAngleRadian(const Vec2& vector)
		{
			return atan2(vector.x, -vector.y);
		}

		template <class Vec2>
		float getAngleDegree(const Vec2& vector)
		{
			return getAngleRadian(vector) * 180 / (float)M_PI;
		}

		template <class Vec2>
		Vec2 getVector2FromRadian(const float radian) 
		{
			return Vec2(cos(radian), sin(radian));
		}

		template <class Vec2>
		Vec2 getVector2FromDegree(const float degree)
		{
			float radian = degree * ((float)M_PI / 180.);
			return getVector2FromRadian(radian);
		}

		template <class Vec2>
		Vec2 getRandom(const int maxX, const int maxY) 
		{
			return Vec2(rand() % maxX, rand() % maxY);
		}

		template <class Vec2>
		float getMagnitude(const Vec2& vector)
		{
			return sqrt((vector.x * vector.x) + (vector.y * vector.y));
		}

		template <class Vec2>
		float getDistance(const Vec2& a, const Vec2& b)
		{
			Vec2 ab = a - b;
			return sqrt(ab.x * ab.x + 
						ab.y * ab.y);
		}

		template <class Vec2>
		Vec2 normalized(const Vec2& vector)
		{
			float magnitude = getMagnitude<Vec2>(vector);

			//If the magnitude is not null
			if (magnitude > 0.) 
			{
				return vector / magnitude;
			}
			else 
			{
				return vector;
			}
		}

	}





}


