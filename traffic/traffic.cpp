#include "traffic.hpp"

cTraffic::cTraffic() : carOnStoplineCount(0), crossroadLoked(false), possibleToSpawn(true)
{
    std::cout << "cTraffic ready" << std::endl;
}

std::shared_ptr<cCar> carFactory::factoryMethod(const eCarType &carType, const sPos &pos, const eDirection &dir)
{
    switch (carType)
    {
    case (eCarType::GAS):
        return std::make_shared<cGas>(pos, dir, fuelReserve);
        break;

    case (eCarType::ELECTRO):
        return std::make_shared<cElectro>(pos, dir, energyReserve);
        break;

    case (eCarType::HYBRID):
        return std::make_shared<cHybrid>(pos, dir, fuelReserve, energyReserve);
        break;
    };   
}

void cTraffic::movingCars(int16_t carsOnScreen, int16_t totalCars)
{
    int16_t iterCount = 0;
    std::ofstream outCSV;

    outCSV.open("carData.txt");

    while (deadCarCount < totalCars)
    {
        if (possibleToSpawn && (currCarCount < carsOnScreen) && (deadCarCount + currCarCount < totalCars))
        {
            spawnCar();
        };

        moveCarsAfterManagement();

        trafficManagement();

        moveCarsInQueue();

        writeCarPosToFile(outCSV, iterCount);

        ++iterCount;
    };
}

void cTraffic::writeCarPosToFile(std::ofstream &out, const int16_t &iter)
{
    for (auto road : sectionsAfterCrossroad)
    {
        for (auto car : road)
        {
            out << iter << ',' << car->report() << std::endl;
        }
    }

    for (auto car : carsOnCrossroad)
    {
        if (car)
        {
            out << iter << ',' << car->report() << std::endl;
        }
    }

    for (auto road : sectionsBeforeCrossroad)
    {
        for (auto car : road)
        {
            out << iter << ',' << car->report() << std::endl;
        }
    }
}

void cTraffic::spawnCar()
{
    int16_t roadForSpawn = uniformRandomNumber(numberOfRoad - 1);

    eDirection carDirection = static_cast<eDirection>(roadForSpawn);

    eCarType carType = static_cast<eCarType>(uniformRandomNumber(numberOfCarTypes - 1));

    sectionsBeforeCrossroad[roadForSpawn].push_back(carFactory::factoryMethod(carType, spawnPos[roadForSpawn], carDirection));

    possibleToSpawn = false;
    ++currCarCount;
}

void cTraffic::moveCarsInQueue()
{

    for (const auto &road : sectionsBeforeCrossroad)
    {
        if (!road.empty())
        {
            tryToMoveFront(road.front()->getIntDir());

            for (auto it = road.begin() + 1; it != road.end(); ++it)
            {
                if (it->get()->notIntersect(*(it - 1)))
                {
                    it->get()->move();
                }
            }

            if (road.back()->outOfResp())
            {
                possibleToSpawn = true;
            }
        }
    }
}

void cTraffic::tryToMoveFront(int16_t dir)
{
    int16_t queueLength = ((dir % 2) ? screenHeight : screenWidth) / 2 - checkPosDistance;

    if (!carsOnStopline[dir])
    {
        if (sectionsBeforeCrossroad[dir].front()->getCarMeter() < queueLength)
        {
            sectionsBeforeCrossroad[dir].front()->move();
        }
        else
        {
            carsOnStopline[dir] = sectionsBeforeCrossroad[dir].front();
            ++carOnStoplineCount;
        }
    }
}

void cTraffic::trafficManagement()
{
    if (crossroadLoked)
    {

        for (auto car : carsOnCrossroad)
        {
            if (car)
            {
                car->move();

                if (car->outOfCrosssroad())
                {
                    int16_t direct = car->getIntDir();

                    sectionsAfterCrossroad[direct].push_back(std::move(carsOnCrossroad[direct]));

                    if (crossroadEmpty())
                    {
                        crossroadLoked = false;
                    }
                }
            }
        }
    }
    else
    {
        int16_t freePos = 0;
        switch (carOnStoplineCount)
        {
        case (0):
            break;

        case (1):
            for (auto car : carsOnStopline)
            {
                if (car)
                {
                    putCarOnCrossroad(car->getIntDir());
                }
            }
            break;

        case (2):
            manageTwoCars();
            break;

        case (3):

            for (auto car : carsOnStopline)
            {
                if (!car)
                {
                    int16_t dir1 = (freePos + 1) % numberOfRoad;
                    int16_t dir2 = (freePos + 3) % numberOfRoad;
                    putCarOnCrossroad(dir1);
                    putCarOnCrossroad(dir2);
                    break;
                };

                ++freePos;
            }
            break;

        case (4):
            putCarOnCrossroad(eDirection::RIGHT);
            putCarOnCrossroad(eDirection::LEFT);
            break;
        }
    }
}

void cTraffic::manageTwoCars()
{
    int16_t dirSum = 0;
    for (auto car : carsOnStopline)
    {
        if (car)
        {
            dirSum += (car->getIntDir() == 3) ? 4 : car->getIntDir();
        }
    }

    switch (dirSum)
    {
    case (1):
        putCarOnCrossroad(eDirection::RIGHT);
        break;

    case (2):
        putCarOnCrossroad(eDirection::RIGHT);
        putCarOnCrossroad(eDirection::LEFT);
        break;

    case (3):
        putCarOnCrossroad(eDirection::DOWN);
        break;

    case (4):
        putCarOnCrossroad(eDirection::UP);
        break;

    case (5):
        putCarOnCrossroad(eDirection::DOWN);
        putCarOnCrossroad(eDirection::UP);
        break;

    case (6):
        putCarOnCrossroad(eDirection::LEFT);
        break;
    }
}

void cTraffic::putCarOnCrossroad(const int16_t &dir)
{
    carsOnCrossroad[dir] = sectionsBeforeCrossroad[dir].front();
    sectionsBeforeCrossroad[dir].pop_front();
    carsOnStopline[dir] = nullptr;
    crossroadLoked = true;
    --carOnStoplineCount;
}

void cTraffic::putCarOnCrossroad(const eDirection &dir)
{
    int16_t intDir = static_cast<int16_t>(dir);
    putCarOnCrossroad(intDir);
}

void cTraffic::moveCarsAfterManagement()
{
    for (auto &road : sectionsAfterCrossroad)
    {
        if (!road.empty())
        {
            for (auto car : road)
            {
                car->move();
            }
            if (road.front()->outOfScreen())
            {
                road.pop_front();
                --currCarCount;
                ++deadCarCount;
            }
        }
    }
}

bool cTraffic::crossroadEmpty()
{
    for (auto car : carsOnCrossroad)
    {
        if (car)
            return false;
    }
    return true;
}

cTraffic::~cTraffic(){}
