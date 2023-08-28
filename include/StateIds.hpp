#ifndef STATE_IDS_HPP_
#define STATE_IDS_HPP_

namespace States
{
    enum class ID
    {
        None,
        Main,
        Menu,
        Score,
        Help,
        Title,
        Racing,
        HorseRace
    };
}

//TODO there should be a more intuitive place to put these
enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

enum class ControlType
{
    Classic,
    Arcade
};

#endif //STATE_IDS_HPP_