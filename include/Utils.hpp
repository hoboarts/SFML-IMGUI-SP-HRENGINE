#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <random>
#include <cassert>

namespace
{
	static std::random_device rd;
	static std::mt19937_64 gen(rd());
	//static std::mt19937_64 gen(std::random_device); // ERROR
}

namespace Utils
{
	namespace Position
	{
		template <typename T>
		static inline void centreOrigin(T& transformable)
		{
			sf::FloatRect bounds = transformable.getLocalBounds();
			transformable.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
		}
	}

	namespace Random
	{
		static inline int value(int begin, int end)
		{
			assert(begin < end);
			//std::mt19937_64 gen(rd()); //DOES IT NEED STATIC ?
			std::uniform_int_distribution<> dis(begin, end);
			return dis(gen);
		}
	}
}

#endif //UTILS_HPP_