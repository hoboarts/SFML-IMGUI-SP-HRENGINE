#ifndef STRINGARRAYSORT_HPP_
#define STRINGARRAYSORT_HPP_

#include <string>
#include <vector>
#include <algorithm>

class StringArraySort final
{
public:
	struct Compare
	{
		static bool compare(std::string a, std::string b) { return a < b; }
	};

	StringArraySort();
	~StringArraySort() = default;
	StringArraySort(const StringArraySort&) = delete;
	const StringArraySort& operator = (const StringArraySort&) = delete;

	void sortSaveArrayFile(std::vector<std::string> arrayIn);

private:
	std::vector<std::string> sort(std::vector<std::string> arrayS);
};

#endif STRINGARRAYSORT_HPP_