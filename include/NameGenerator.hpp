#ifndef NAMEGENERATOR_HPP_
#define NAMEGENERATOR_HPP_

#include <string>

class NameGenerator
{
public:
	enum NameVariants
	{
		Only_First,
		Only_Middle,
		Only_Last,
		With_The_Full,
		Full,
		With_The_Only_First,
		With_The_Only_Middle,
		With_The_Only_Last,
		With_The_Without_First,
		With_The_Without_Middle,
		With_The_Without_Last,
		Without_First,
		Without_Middle,
		Without_Last,
		Without_First_Glue,
		Without_Middle_Glue,
		Without_Last_Glue
	};

	NameGenerator();
	~NameGenerator() = default;
	NameGenerator(const NameGenerator&) = delete;
	const NameGenerator& operator = (const NameGenerator&) = delete;

	std::string generateName();
	//----
private:
	std::string nameSelector(int = NULL, int = NULL, int = NULL, bool = false, bool = false);
	//----
};

#endif //NAMEGENERATOR_HPP_