#pragma once

#include "util.hpp"

#include <iostream>
#include <memory>

const int16_t screenWidth = 1024;
const int16_t screenHeight = 768;
const int16_t defaultCarDimension = 60;
const int16_t maxCarDimension = 80;
const int16_t distanceBetweenCars = 20;
constexpr int16_t spawnDistance = (maxCarDimension + distanceBetweenCars) / 2;
constexpr int16_t checkPosDistance = 3 * spawnDistance;
const int16_t numberOfCarTypes = 3;
const int16_t numberOfRoad = 4;
const int16_t fuelReserve = 1500;
const int16_t energyReserve = 1500;

enum class eDirection
{
    RIGHT = 0,
    DOWN,
    LEFT,
    UP
};

enum class eCarType
{
    GAS = 0,
    ELECTRO,
    HYBRID
};

struct sPos
{
public:
    sPos();
    sPos(const sPos &);
    sPos(int16_t aX, int16_t aY);
    int16_t getX() const;
    int16_t getY() const;
    void increaseX(const int16_t &add);
    void decreaseX(const int16_t &reduce);
    void increaseY(const int16_t &add);
    void decreaseY(const int16_t &reduce);

private:
    int16_t x;
    int16_t y;
};

struct sSize
{
public:
    sSize();
    sSize(int16_t aW, int16_t aH);
    int16_t getWidth() const;
    int16_t getHeight() const;

private:
    int16_t width;
    int16_t height;
};

struct sSpeedometer
{
public:
    sSpeedometer();
    sSpeedometer(int16_t s, int16_t m);
    void increaceMeter(const int16_t &speed);
    int16_t getSpeed() const;
    int16_t getMeter() const;

private:
    int16_t speed;
    int16_t meter;
};

class cCar
{
public:
    void move();
    eDirection getDir() const;
    int16_t getIntDir() const;
    int16_t getCarSpeed() const;
    int16_t getCarMeter() const;
    bool outOfResp();
    bool outOfCrosssroad() const;
    bool outOfScreen() const;
    bool notIntersect(const std::shared_ptr<cCar> &other) const;    
    std::string report();

protected:
    cCar();
    cCar(const sPos &p, eDirection d);
    void changePosition();
    virtual void useEnergySource();
    virtual ~cCar();

private:
    sPos pos;
    const sSize size;
    sSpeedometer speedometer;
    eDirection dir;
    bool holdSpawnplace;    
    int16_t carUID;
    static inline int16_t carGlobalCount = 0;
};

class cElectro : virtual public cCar
{
public:
    cElectro(const sPos &p, eDirection d, int16_t chrg);
    ~cElectro();

protected:
    cElectro();
    int16_t charge;
    void useEnergySource();
};

class cGas : virtual public cCar
{
public:
    cGas(const sPos &p, eDirection d, int16_t f);
    ~cGas();

protected:
    cGas();
    int16_t fuel;
    void useEnergySource();
};

class cHybrid : public cElectro, public cGas
{
public:
    cHybrid(const sPos &p, eDirection d, int16_t f, int16_t chrg);
    ~cHybrid();

private:
    cHybrid();
    void useEnergySource();
};