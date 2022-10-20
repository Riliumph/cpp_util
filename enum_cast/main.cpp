#include <iostream>
#include "enum.hpp"

enum struct COLOR
{
    RED,
    BLUE,
    GREEN,
    QTY
};

int main(void)
{
    COLOR colors[e_cast(COLOR::QTY)] = {COLOR::RED, COLOR::BLUE, COLOR::GREEN};

    for (auto color : colors)
    {
        switch (color)
        {
        case COLOR::RED:
            std::cout << "RED" << std::endl;
            break;
        case COLOR::BLUE:
            std::cout << "BLUE" << std::endl;
            break;
        case COLOR::GREEN:
            std::cout << "GREEN" << std::endl;
            break;
        default:
            std::cout << "UNDEFINED" << std::endl;
            break;
        }
    }
    return 0;
}
