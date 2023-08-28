#include <StringArraySort.hpp>

#include <iostream>
#include <fstream>

StringArraySort::StringArraySort()
{}

//public
void StringArraySort::sortSaveArrayFile(std::vector<std::string> arrayIn)
{
	std::vector<std::string> sortedA = StringArraySort::sort(arrayIn);

	std::ofstream file("Sorted_Array.txt");
	if (file.is_open())
	{
		for (int i = 0; i < (int)sortedA.size(); ++i)
		{
			if (i != ((int)sortedA.size() - 1))
			{
				file << '"' << sortedA[i] << '"' << "," << std::endl;
			}
			else
			{
				file << '"' << sortedA[i] << '"' << std::endl;
			}
		}
		file.close();
	}
	else std::cout << "Unable to open file";
}

//private

std::vector<std::string> StringArraySort::sort(std::vector<std::string> arrayS)
{
	//std::sort(arrayS.begin(), arrayS.end(), [](std::string a, std::string b) {return a < b;}); //compare func with lambda expr
	std::sort(arrayS.begin(), arrayS.end(), &Compare::compare); //compare func with reference to struct
	return arrayS;
}