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
 * Description: This C++ program declare the Item class
***********************************************************************/
#pragma once
#include <QString>

class Item
{
private:
    QString name;
	int boost;
    QString boostType;
	bool isUsed;

public:
	Item();
    Item(QString, int, QString);

    void setName(QString);
	void setBoost(int);
    void setBoostType(QString);
	void setIsUsed(bool);

    QString getName();
	int getBoost();
    QString getBoostType();
	bool getIsUsed();
};
