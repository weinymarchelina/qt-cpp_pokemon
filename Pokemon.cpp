/***********************************************************************
 * File: Pokemon.cpp
 * Author:	??? B11115010
 *			??? B11115011
 *			??? B11115014
 * Create Date: 2023-05-26
 * Editor:	??? B11115010
 *			??? B11115011
 *			??? B11115014
 * Update Date: 2023-06-05
 * Description: This C++ program define the Pokemon class
***********************************************************************/

#include "Pokemon.h"

// Intent: Default constructor for Pokemon class
// Pre: None
// Post: Initialize Pokemon attributes with default values
Pokemon::Pokemon()
{
    this->name = "NONE";
    this->type.clear();
    this->hp = 0;
    this->maxHp = 0;
    this->attack = 0;
    this->spAttack = 0;
    this->defense = 0;
    this->spDefense = 0;
    this->speed = 0;
    this->status.clear();
    this->moves.clear();
}

// Intent: Parameterized constructor for Pokemon class
// Pre: 'name' is a valid QString, 'type' is a valid QVector of QStrings, 'hp', 'attack', 'spAttack', 'defense', 'spDefense', and 'speed' are valid integer values, 'status' is a valid QVector of QStrings, 'moves' is a valid QVector of Move objects
// Post: Initialize Pokemon attributes with the provided values
Pokemon::Pokemon(QString name, QVector<QString> type, int hp, int attack, int spAttack, int defense, int spDefense, int speed, QVector<QString> status, QVector<Move> moves)
{
    this->name = name;
    this->type = type;
    this->hp = hp;
    this->maxHp = hp;
    this->attack = attack;
    this->spAttack = spAttack;
    this->defense = defense;
    this->spDefense = spDefense;
    this->speed = speed;
    this->status = status;
    this->moves = moves;
}

// Intent: Parameterized constructor for Pokemon class without status and moves
// Pre: 'name' is a valid QString, 'type' is a valid QVector of QStrings, 'hp', 'attack', 'spAttack', 'defense', 'spDefense', and 'speed' are valid integer values
// Post: Initialize Pokemon attributes with the provided values and clear status and moves
Pokemon::Pokemon(QString name, QVector<QString> type, int hp, int attack, int spAttack, int defense, int spDefense, int speed)
{
    this->name = name;
    this->type = type;
    this->hp = hp;
    this->maxHp = hp;
    this->attack = attack;
    this->spAttack = spAttack;
    this->defense = defense;
    this->spDefense = spDefense;
    this->speed = speed;
    this->status.clear();
    this->moves.clear();
}

// Intent: Set the name of the Pokemon
// Pre: 'name' is a valid QString
// Post: Set the name attribute of the Pokemon
void Pokemon::setName(QString name)
{
    this->name = name;
}

// Intent: Set the type of the Pokemon
// Pre: 'type' is a valid QVector of QStrings
// Post: Set the type attribute of the Pokemon
void Pokemon::setType(QVector<QString> type)
{
    this->type = type;
}

// Intent: Set the HP of the Pokemon
// Pre: 'hp' is a valid integer value
// Post: Set the hp attribute of the Pokemon
void Pokemon::setHp(int hp)
{
    this->hp = hp;
}

// Intent: Set the attack stat of the Pokemon
// Pre: 'attack' is a valid integer value
// Post: Set the attack attribute of the Pokemon
void Pokemon::setAttack(int attack)
{
    this->attack = attack;
}

// Intent: Set the special attack stat of the Pokemon
// Pre: 'spAttack' is a valid integer value
// Post: Set the spAttack attribute of the Pokemon
void Pokemon::setSpAttack(int spAttack)
{
    this->spAttack = spAttack;
}

// Intent: Set the defense stat of the Pokemon
// Pre: 'defense' is a valid integer value
// Post: Set the defense attribute of the Pokemon
void Pokemon::setDefense(int defense)
{
    this->defense = defense;
}

// Intent: Set the special defense stat of the Pokemon
// Pre: 'spDefense' is a valid integer value
// Post: Set the spDefense attribute of the Pokemon
void Pokemon::setSpDefense(int spDefense)
{
    this->spDefense = spDefense;
}

// Intent: Set the speed stat of the Pokemon
// Pre: 'speed' is a valid integer value
// Post: Set the speed attribute of the Pokemon
void Pokemon::setSpeed(int speed)
{
    this->speed = speed;
}

// Intent: Set the status of the Pokemon
// Pre: 'status' is a valid QVector of QStrings
// Post: Set the status attribute of the Pokemon
void Pokemon::setStatus(QVector<QString> status)
{
    this->status = status;
}

// Intent: Set the moves of the Pokemon
// Pre: 'moves' is a valid QVector of Move objects
// Post: Set the moves attribute of the Pokemon
void Pokemon::setMoves(QVector<Move> moves)
{
    this->moves = moves;
}

// Intent: Get the name of the Pokemon
// Pre: None
// Post: Return the name attribute of the Pokemon
QString Pokemon::getName()
{
    return this->name;
}

// Intent: Get the type of the Pokemon
// Pre: None
// Post: Return the type attribute of the Pokemon
QVector<QString> Pokemon::getType()
{
    return this->type;
}

// Intent: Get the HP of the Pokemon
// Pre: None
// Post: Return the hp attribute of the Pokemon
int Pokemon::getHp()
{
    return this->hp;
}

// Intent: Get the attack stat of the Pokemon
// Pre: None
// Post: Return the attack attribute of the Pokemon
int Pokemon::getAttack()
{
    return this->attack;
}

// Intent: Get the special attack stat of the Pokemon
// Pre: None
// Post: Return the spAttack attribute of the Pokemon
int Pokemon::getSpAttack()
{
    return this->spAttack;
}

// Intent: Get the defense stat of the Pokemon
// Pre: None
// Post: Return the defense attribute of the Pokemon
int Pokemon::getDefense()
{
    return this->defense;
}

// Intent: Get the special defense stat of the Pokemon
// Pre: None
// Post: Return the spDefense attribute of the Pokemon
int Pokemon::getSpDefense()
{
    return this->spDefense;
}

// Intent: Get the speed stat of the Pokemon
// Pre: None
// Post: Return the speed attribute of the Pokemon
int Pokemon::getSpeed()
{
    return this->speed;
}

// Intent: Get the status of the Pokemon
// Pre: None
// Post: Return the status attribute of the Pokemon
QVector<QString> Pokemon::getStatus()
{
    return this->status;
}

// Intent: Get the moves of the Pokemon
// Pre: None
// Post: Return a reference to the moves attribute of the Pokemon
QVector<Move>& Pokemon::getMoves()
{
    return this->moves;
}

// Intent: Get the maximum HP of the Pokemon
// Pre: None
// Post: Return the maxHp attribute of the Pokemon
int Pokemon::getMaxHp()
{
    return this->maxHp;
}
