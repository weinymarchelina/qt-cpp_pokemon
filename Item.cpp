/***********************************************************************
 * File: Item.cpp
 * Author:	??? B11115010
 *			??? B11115011
 *			??? B11115014
 * Create Date: 2023-05-26
 * Editor:	??? B11115010
 *			??? B11115011
 *			??? B11115014
 * Update Date: 2023-06-05
 * Description: This C++ program define the Item class
***********************************************************************/
#include "Item.h"

// Intent: Create an instance of Item with default values.
// Pre: None.
// Post: Item is created with default values ("NONE", 0, "HP") and isUsed flag is set to false.
Item::Item()
{
    this->name = "NONE";
    this->boost = 0;
    this->boostType = "HP";
    this->isUsed = false;
}

// Intent: Create an instance of Item with provided values.
// Pre: None.
// Post: Item is created with the specified name, boost, boostType, and isUsed flag is set to false.
Item::Item(QString name, int boost, QString boostType)
{
    this->name = name;
    this->boost = boost;
    this->boostType = boostType;
    this->isUsed = false;
}

// Intent: Set the name of the item.
// Pre: None.
// Post: The name of the item is updated with the provided name.
void Item::setName(QString name)
{
    this->name = name;
}

// Intent: Set the boost value of the item.
// Pre: None.
// Post: The boost value of the item is updated with the provided boost value.

void Item::setBoost(int boost)
{
    this->boost = boost;
}

// Intent: Set the boost type of the item.
// Pre: None.
// Post: The boost type of the item is updated with the provided boost type.

void Item::setBoostType(QString boostType)
{
    this->boostType = boostType;
}

// Intent: Set the isUsed flag of the item.
// Pre: None.
// Post: The isUsed flag of the item is updated with the provided value.
void Item::setIsUsed(bool isUsed)
{
    this->isUsed = isUsed;
}

// Intent: Get the name of the item.
// Pre: None.
// Post: The name of the item is returned.
QString Item::getName()
{
    return this->name;
}

// Intent: Get the boost value of the item.
// Pre: None.
// Post: The boost value of the item is returned.
int Item::getBoost()
{
    return this->boost;
}

// Intent: Get the boost type of the item.
// Pre: None.
// Post: The boost type of the item is returned.
QString Item::getBoostType()
{
    return this->boostType;
}

// Intent: Get the isUsed flag of the item.
// Pre: None.
// Post: The isUsed flag of the item is returned.
bool Item::getIsUsed()
{
    return this->isUsed;
}
