/***********************************************************************
 * File: Move.cpp
 * Author:	鍾詩靈 B11115010
 *			陳仕興 B11115011
 *			魏美芳 B11115014
 * Create Date: 2023-05-26
 * Editor:	鍾詩靈 B11115010
 *			陳仕興 B11115011
 *			魏美芳 B11115014
 * Update Date: 2023-06-05
 * Description: This C++ program define the Move class
***********************************************************************/
#include "Move.h"

// Intent: Default constructor for Move class
// Pre: None
// Post: Initialize Move attributes with default values
Move::Move()
{
    this->name = "NONE";
    this->type.clear();
    this->attackType.clear();
    this->power = 0;
    this->accuracy = 0;
    this->powerPoint = 0;
    this->maxPowerPoint = 0;
    this->effect = "NONE";
}

// Intent: Parameterized constructor for Move class
// Pre: 'name', 'type', 'attackType', and 'effect' are valid QStrings, 'power' and 'accuracy' are valid integer values, 'powerPoint' is a valid integer value, and 'maxPowerPoint' is a valid integer value greater than or equal to 'powerPoint'
// Post: Initialize Move attributes with the provided values
Move::Move(QString name, QString type, QString attackType, int power, int accuracy, int powerPoint, QString effect)
{
    this->name = name;
    this->type = type;
    this->attackType = attackType;
    this->power = power;
    this->accuracy = accuracy;
    this->powerPoint = powerPoint;
    this->maxPowerPoint = powerPoint;
    this->effect = effect;
}

// Intent: Set the name of the Move
// Pre: 'name' is a valid QString
// Post: Set the name attribute of the Move
void Move::setName(QString name)
{
    this->name = name;
}

// Intent: Set the type of the Move
// Pre: 'type' is a valid QString
// Post: Set the type attribute of the Move
void Move::setType(QString type)
{
    this->type = type;
}

// Intent: Set the attack type of the Move
// Pre: 'attackType' is a valid QString
// Post: Set the attackType attribute of the Move
void Move::setAttackType(QString attackType)
{
    this->attackType = attackType;
}

// Intent: Set the power of the Move
// Pre: 'power' is a valid integer value
// Post: Set the power attribute of the Move
void Move::setPower(int power)
{
    this->power = power;
}

// Intent: Set the accuracy of the Move
// Pre: 'accuracy' is a valid integer value
// Post: Set the accuracy attribute of the Move
void Move::setAccuracy(int accuracy)
{
    this->accuracy = accuracy;
}

// Intent: Set the power points of the Move
// Pre: 'powerPoint' is a valid integer value
// Post: Set the powerPoint attribute of the Move
void Move::setPowerPoint(int powerPoint)
{
    this->powerPoint = powerPoint;
}

// Intent: Set the effect of the Move
// Pre: 'effect' is a valid QString
// Post: Set the effect attribute of the Move
void Move::setEffect(QString effect)
{
    this->effect = effect;
}

// Intent: Get the name of the Move
// Pre: None
// Post: Return the name attribute of the Move
QString Move::getName()
{
    return this->name;
}

// Intent: Get the type of the Move
// Pre: None
// Post: Return the type attribute of the Move
QString Move::getType()
{
    return this->type;
}

// Intent: Get the attack type of the Move
// Pre: None
// Post: Return the attackType attribute of the Move
QString Move::getAttackType()
{
    return this->attackType;
}

// Intent: Get the power of the Move
// Pre: None
// Post: Return the power attribute of the Move
int Move::getPower()
{
    return this->power;
}

// Intent: Get the accuracy of the Move
// Pre: None
// Post: Return the accuracy attribute of the Move
int Move::getAccuracy()
{
    return this->accuracy;
}

// Intent: Get the power points of the Move
// Pre: None
// Post: Return the powerPoint attribute of the Move
int Move::getPowerPoint()
{
    return this->powerPoint;
}

// Intent: Get the maximum power points of the Move
// Pre: None
// Post: Return the maxPowerPoint attribute of the Move
int Move::getMaxPowerPoint()
{
    return this->maxPowerPoint;
}

// Intent: Get the effect of the Move
// Pre: None
// Post: Return the effect attribute of the Move
QString Move::getEffect()
{
    return this->effect;
}

