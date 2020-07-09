#include "car.hpp"

sPos::sPos(){}

sPos::sPos(int16_t aX, int16_t aY) : x(aX), y(aY){}

sPos::sPos(const sPos &other)
{
    x = other.x;
    y = other.y;    
}

int16_t sPos::getX() const
{
    return x;
}

int16_t sPos::getY() const
{
    return y;
}

void sPos::increaseX(const int16_t &add)
{
    x += add;
}

void sPos::increaseY(const int16_t &add)
{
    y += add;
}

void sPos::decreaseX(const int16_t &reduce)
{
    x -= reduce;
}

void sPos::decreaseY(const int16_t &reduce)
{
    y -= reduce;
}

sSize::sSize(){}

sSize::sSize(int16_t aW, int16_t aH) : width(aW), height(aH){}

int16_t sSize::getWidth() const
{
    return width;
}

int16_t sSize::getHeight() const
{
    return height;
}

sSpeedometer::sSpeedometer(){}

sSpeedometer::sSpeedometer(int16_t s, int16_t m) : speed(s), meter(m){}

void sSpeedometer::increaceMeter(const int16_t &speed)
{
    meter += speed;
}

int16_t sSpeedometer::getSpeed() const
{
    return speed;
}

int16_t sSpeedometer::getMeter() const
{
    return meter;
}

cCar::cCar() : size(defaultCarDimension, defaultCarDimension){}

cCar::cCar(const sPos &p, eDirection d) : pos(p), size(defaultCarDimension, defaultCarDimension),
                                          speedometer(1, 0), dir(d), holdSpawnplace(true)
{
    ++cCar::carGlobalCount;
    carUID = carGlobalCount;
}

void cCar::move()
{
    changePosition();
    useEnergySource();    
}

eDirection cCar::getDir() const
{    
    return dir;
}

int16_t cCar::getIntDir() const
{     
    return static_cast<int16_t>(dir);
}

int16_t cCar::getCarSpeed() const
{
    return speedometer.getSpeed();
}

int16_t cCar::getCarMeter() const
{
    return speedometer.getMeter();
}

bool cCar::outOfResp()
{
    if (holdSpawnplace)
    {
        if (speedometer.getMeter() > maxCarDimension)
        {
            holdSpawnplace = false;
            return true;
        }
    }
    return false;
}

bool cCar::outOfCrosssroad() const
{
    int16_t outOfCrossroadDistance = ((static_cast<int16_t>(dir) % 2) ? screenHeight : screenWidth) / 2 + checkPosDistance;
    return (speedometer.getMeter() >= outOfCrossroadDistance);
}

bool cCar::outOfScreen() const
{
    return ((pos.getX() >= screenWidth) || (pos.getX() <= 0) || (pos.getY() >= screenHeight) || (pos.getY() <= 0));
}

bool cCar::notIntersect(const std::shared_ptr<cCar> &other) const
{
    if (static_cast<int16_t>(dir) % 2)
    {
        return (abs(other->pos.getY() - pos.getY() + speedometer.getSpeed()) >= (size.getHeight() + other->size.getHeight()) / 2);
    }
    else
    {
        return (abs(other->pos.getX() - pos.getX() + speedometer.getSpeed()) >= (size.getWidth() + other->size.getWidth()) / 2);
    }
}

std::string cCar::report()
{
    std::string rez = std::to_string(carUID) + ',' + std::to_string(pos.getX()) + ',' + std::to_string(pos.getY());
    return rez;
}

void cCar::changePosition()
{
    switch (dir)
    {
    case (eDirection::RIGHT):
        pos.increaseX(speedometer.getSpeed());
        break;

    case (eDirection::DOWN):
        pos.decreaseY(speedometer.getSpeed());
        break;

    case (eDirection::LEFT):
        pos.decreaseX(speedometer.getSpeed());
        break;

    case (eDirection::UP):
        pos.increaseY(speedometer.getSpeed());
        break;
    }
    speedometer.increaceMeter(speedometer.getSpeed());
}

void cCar::useEnergySource(){}

cCar::~cCar(){}

cElectro::cElectro(){}

cElectro::~cElectro(){}

cElectro::cElectro(const sPos &p, eDirection d, int16_t chrg) : cCar(p, d)
{
    charge = chrg;
}

void cElectro::useEnergySource()
{
    --charge;
}

cGas::cGas(){}

cGas::~cGas(){}

cGas::cGas(const sPos &p, eDirection d, int16_t f) : cCar(p, d)
{
    fuel = f;
}

void cGas::useEnergySource()
{
    --fuel;
}

cHybrid::cHybrid(){}

cHybrid::~cHybrid(){}

cHybrid::cHybrid(const sPos &p, eDirection d, int16_t f, int16_t chrg) : cCar(p, d)
{
    charge = chrg;
    fuel = f;
}

void cHybrid::useEnergySource()
{
    uniformRandomNumber(1) ? --fuel : --charge;
}