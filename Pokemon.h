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
 * Description: This C++ program declare the Pokemon class
***********************************************************************/
#pragma once
#include <QString>
#include <QVector>
#include "Move.h"

class Pokemon
{
private:
    QString name;
    QVector <QString> type;
	int hp;
	int maxHp;
	int attack;
	int spAttack;
	int defense;
	int spDefense;
	int speed;
    QVector<QString> status;
    QVector <Move> moves;

public:
	Pokemon();
    Pokemon(QString, QVector <QString>, int, int, int, int, int, int, QVector<QString>, QVector<Move>);
    Pokemon(QString, QVector <QString>, int, int, int, int, int, int);

    void setName(QString);
    void setType(QVector <QString>);
	void setHp(int);
	void setAttack(int);
	void setSpAttack(int);
	void setDefense(int);
	void setSpDefense(int);
	void setSpeed(int);
    void setStatus(QVector<QString>);
    void setMoves(QVector<Move>);

    QString getName();
    QVector <QString> getType();
	int getHp();
	int getAttack();
	int getSpAttack();
	int getDefense();
	int getSpDefense();
	int getSpeed();
	int getMaxHp();
    QVector<QString> getStatus();
    QVector <Move>& getMoves();

	//Pokemon& operator= (const Pokemon&);
};
