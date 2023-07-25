/***********************************************************************
 * File: main.cpp
 * Author:	Áé¸ÖÆF B11115010
 *			³¯¥K¿³ B11115011
 *			ÃQ¬üªÚ B11115014
 * Create Date: 2023-05-26
 * Editor:	Áé¸ÖÆF B11115010
 *			³¯¥K¿³ B11115011
 *			ÃQ¬üªÚ B11115014
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
