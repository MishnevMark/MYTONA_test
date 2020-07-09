#include "traffic/traffic.hpp"

int main()
{
    auto mainObjectPtr = std::make_unique<cTraffic>();

    int16_t carsOnScreen, totalCars;
    std::cout << "Enter maximum of vehicles on the roads" << std::endl;
    std::cin >> carsOnScreen;
    std::cout << "Total number of vehicles" << std::endl;
    std::cin >> totalCars;

    mainObjectPtr->movingCars(carsOnScreen, totalCars);

    return 0;
}