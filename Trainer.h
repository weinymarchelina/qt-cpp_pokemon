/***********************************************************************
 * File: main.cpp
 * Author:	����F B11115010
 *			���K�� B11115011
 *			�Q���� B11115014
 * Create Date: 2023-05-26
 * Editor:	����F B11115010
 *			���K�� B11115011
 *			�Q���� B11115014
 * Update Date: 2023-06-
 * Description: This C++ program declare the Trainer class
***********************************************************************/
#pragma once
#include "Pokemon.h"
#include "Item.h"
#include <QVector>

class Trainer
{
private:
    QVector <Pokemon> pokemon;
    QVector <Item> item;
public:
	Trainer();
    Trainer(QVector <Pokemon>, QVector <Item>);

    void setPokemon(QVector <Pokemon>);
    void setItem(QVector <Item>);

    QVector <Pokemon>& getPokemon();
    QVector <Item>& getItem();
};
