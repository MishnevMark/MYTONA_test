#pragma once

#include "car.hpp"

#include <deque>
#include <array>
#include <vector>
#include <fstream>

class carFactory
{
public:
    static std::shared_ptr<cCar> factoryMethod(const eCarType &carType, const sPos &pos, const eDirection &dir);
};

class cTraffic
{
public:
    cTraffic();
    ~cTraffic();
    void movingCars(int16_t carsOnScreen, int16_t totalCars);

private:
    inline static const std::array<sPos, numberOfRoad> spawnPos =
        {sPos(0, screenHeight / 2 - spawnDistance),
         sPos(screenWidth / 2 - spawnDistance, screenHeight),
         sPos(screenWidth, screenHeight / 2 + spawnDistance),
         sPos(screenWidth / 2 + spawnDistance, 0)};

    std::array<std::deque<std::shared_ptr<cCar>>, numberOfRoad> sectionsBeforeCrossroad;
    std::array<std::deque<std::shared_ptr<cCar>>, numberOfRoad> sectionsAfterCrossroad;
    std::array<std::shared_ptr<cCar>, numberOfRoad> carsOnCrossroad;
    std::array<std::shared_ptr<cCar>, numberOfRoad> carsOnStopline;

    int16_t carOnStoplineCount;
    bool crossroadLoked;
    bool possibleToSpawn;
    int16_t currCarCount;
    int16_t deadCarCount;

private:
    void spawnCar();
    void moveCarsAfterManagement();
    void trafficManagement();
    void moveCarsInQueue();
    void tryToMoveFront(int16_t dir);
    void putCarOnCrossroad(const int16_t &dir);
    void putCarOnCrossroad(const eDirection &dir);
    void manageTwoCars();
    bool crossroadEmpty();
    void writeCarPosToFile(std::ofstream &out, const int16_t &iter);
};