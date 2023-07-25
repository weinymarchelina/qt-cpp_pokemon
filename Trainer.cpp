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
 * Description: This C++ program define the Trainer class
***********************************************************************/
#include "Trainer.h"

// Intent: Default constructor for Trainer class
// Pre: None
// Post: Clear the pokemon and item vectors
Trainer::Trainer()
{
    this->pokemon.clear();
    this->item.clear();
}

// Intent: Parameterized constructor for Trainer class
// Pre: 'pokemon' and 'item' are valid QVector objects
// Post: Set the pokemon and item vectors with the provided values
Trainer::Trainer(QVector<Pokemon> pokemon, QVector<Item> item)
{
    this->pokemon = pokemon;
    this->item = item;
}

// Intent: Set the pokemon vector for the Trainer
// Pre: 'pokemon' is a valid QVector object
// Post: Set the pokemon vector with the provided values
void Trainer::setPokemon(QVector<Pokemon> pokemon)
{
    this->pokemon = pokemon;
}

// Intent: Set the item vector for the Trainer
// Pre: 'item' is a valid QVector object
// Post: Set the item vector with the provided values
void Trainer::setItem(QVector<Item> item)
{
    this->item = item;
}

// Intent: Get the pokemon vector of the Trainer
// Pre: None
// Post: Return a reference to the pokemon vector
QVector<Pokemon>& Trainer::getPokemon()
{
    return this->pokemon;
}

// Intent: Get the item vector of the Trainer
// Pre: None
// Post: Return a reference to the item vector
QVector<Item>& Trainer::getItem()
{
    return this->item;
}
