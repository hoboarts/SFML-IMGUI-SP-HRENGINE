#include <NameGenerator.hpp>
#include <Utils.hpp>
#include <NameDictionary.hpp>

using namespace Utils::Random;

namespace
{
    int fstNLastEl = (int) FIRST_NAMES.size() - 1;
    int midNLastEl = (int) MIDDLE_NAMES.size() - 1;
    int lstNLastEl = (int) LAST_NAMES.size() - 1;
    const std::vector<int> varNRng = { NameGenerator::NameVariants::Only_First, NameGenerator::NameVariants::Without_Last_Glue };
}

NameGenerator::NameGenerator()
{}

//public
std::string NameGenerator::generateName()
{
    int variant = value(varNRng[0], varNRng[1]);
    std::string resultName;

    switch (variant)
    {
    case 0://ONLY FIRST
        resultName = nameSelector(fstNLastEl, 0, 0);
        break;
    case 1://ONLY MIDDLE
        resultName = nameSelector(0, midNLastEl, 0);
        break;
    case 2://ONLY LAST
        resultName = nameSelector(0, 0, lstNLastEl);
        break;
    case 3://WITH THE FULL
        resultName = nameSelector(fstNLastEl, midNLastEl, lstNLastEl,true);
        break;
    case 4://FULL
        resultName = nameSelector(fstNLastEl, midNLastEl, lstNLastEl);
        break;
    case 5://WITH THE O FIRST
        resultName = nameSelector(fstNLastEl,0,0,true);
        break;
    case 6://WITH THE O MIDDLE
        resultName = nameSelector(0, midNLastEl, 0, true);
        break;
    case 7://WITH THE O LAST
        resultName = nameSelector(0, 0, lstNLastEl, true);
        break;
    case 8://WITH THE WO FIRST
        resultName = nameSelector(0, midNLastEl, lstNLastEl, true);
        break;
    case 9://WITH THE WO MIDDLE
        resultName = nameSelector(fstNLastEl, 0, lstNLastEl, true);
        break;
    case 10://WITH THE WO LAST
        resultName = nameSelector(fstNLastEl, midNLastEl, 0, true);
        break;
    case 11://WO FIRST
        resultName = nameSelector(0, midNLastEl, lstNLastEl, false, false);
        break;
    case 12://WO MIDDLE
        resultName = nameSelector(fstNLastEl, 0, lstNLastEl, false, false);
        break;
    case 13://WO LAST
        resultName = nameSelector(fstNLastEl, midNLastEl, 0, false, false);
        break;
    case 14://WO FIRST G
        resultName = nameSelector(0, midNLastEl, lstNLastEl, false, true);
        break;
    case 15://WO MIDDLE G
        resultName = nameSelector(fstNLastEl, 0, lstNLastEl, false, true);
        break;
    case 16://WO LAST G
        resultName = nameSelector(fstNLastEl, midNLastEl, 0, false, true);
        break;
    }

    return resultName;
}

//private
std::string NameGenerator::nameSelector(int rng1, int rng2, int rng3, bool hasThe, bool glued)
{
    std::string resultName = "";
    //-------------------------
    
    if (hasThe)
    {
        if (rng1)
        {
            int varNameIdentifier = value(0, rng1);
            resultName = "The " + FIRST_NAMES[varNameIdentifier];
        }
        if (rng2)
        {
            int varNameIdentifier = value(0, rng2);
            resultName = "The " + MIDDLE_NAMES[varNameIdentifier];
        }
        if (rng3)
        {
            int varNameIdentifier = value(0, rng3);
            resultName = "The " + LAST_NAMES[varNameIdentifier];
        }
        if (rng1 && rng2)
        {
            int varNameIdentifier = value(0, rng1);
            int varNameIdentifier1 = value(0, rng2);
            resultName = "The " + FIRST_NAMES[varNameIdentifier] + " " + MIDDLE_NAMES[varNameIdentifier1];
        }
        if (rng1 && rng3)
        {
            int varNameIdentifier = value(0, rng1);
            int varNameIdentifier1 = value(0, rng3);
            resultName = "The " + FIRST_NAMES[varNameIdentifier] + " " + LAST_NAMES[varNameIdentifier1];
        }
        if (rng2 && rng3)
        {
            int varNameIdentifier = value(0, rng2);
            int varNameIdentifier1 = value(0, rng3);
            resultName = "The " + MIDDLE_NAMES[varNameIdentifier] + " " + LAST_NAMES[varNameIdentifier1];
        }
        if (rng1 && rng2 && rng3)
        {
            int varNameIdentifier = value(0, rng1);
            int varNameIdentifier1 = value(0, rng2);
            int varNameIdentifier2 = value(0, rng3);
            resultName = "The " + FIRST_NAMES[varNameIdentifier] + " " + MIDDLE_NAMES[varNameIdentifier1] + " " + LAST_NAMES[varNameIdentifier2];
        }
    }
    else//NO THE
    {
        if (glued)
        {
            if (rng1 && rng2)
            {
                int varNameIdentifier = value(0, rng1);
                int varNameIdentifier1 = value(0, rng2);
                resultName = FIRST_NAMES[varNameIdentifier] + MIDDLE_NAMES[varNameIdentifier1];
            }
            if (rng1 && rng3)
            {
                int varNameIdentifier = value(0, rng1);
                int varNameIdentifier1 = value(0, rng3);
                resultName = FIRST_NAMES[varNameIdentifier] + LAST_NAMES[varNameIdentifier1];
            }
            if (rng2 && rng3)
            {
                int varNameIdentifier = value(0, rng2);
                int varNameIdentifier1 = value(0, rng3);
                resultName = MIDDLE_NAMES[varNameIdentifier] + LAST_NAMES[varNameIdentifier1];
            }
        }
        else// NOT GLUED
        {
            if (rng1)
            {
                int varNameIdentifier = value(0, rng1);
                resultName = FIRST_NAMES[varNameIdentifier];
            }
            if (rng2)
            {
                int varNameIdentifier = value(0, rng2);
                resultName = MIDDLE_NAMES[varNameIdentifier];
            }
            if (rng3)
            {
                int varNameIdentifier = value(0, rng3);
                resultName = LAST_NAMES[varNameIdentifier];
            }
            if (rng1 && rng2)
            {
                int varNameIdentifier = value(0, rng1);
                int varNameIdentifier1 = value(0, rng2);
                resultName = FIRST_NAMES[varNameIdentifier] + " " + MIDDLE_NAMES[varNameIdentifier1];
            }
            if (rng1 && rng3)
            {
                int varNameIdentifier = value(0, rng1);
                int varNameIdentifier1 = value(0, rng3);
                resultName = FIRST_NAMES[varNameIdentifier] + " " + LAST_NAMES[varNameIdentifier1];
            }
            if (rng2 && rng3)
            {
                int varNameIdentifier = value(0, rng2);
                int varNameIdentifier1 = value(0, rng3);
                resultName = MIDDLE_NAMES[varNameIdentifier] + " " + LAST_NAMES[varNameIdentifier1];
            }
            if (rng1 && rng2 && rng3)
            {
                int varNameIdentifier = value(0, rng1);
                int varNameIdentifier1 = value(0, rng2);
                int varNameIdentifier2 = value(0, rng3);
                resultName = FIRST_NAMES[varNameIdentifier] + " " + MIDDLE_NAMES[varNameIdentifier1] + " " + LAST_NAMES[varNameIdentifier2];
            }
        }
    }

    return resultName;
}