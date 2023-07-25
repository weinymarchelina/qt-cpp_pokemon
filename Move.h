#pragma once
#include <QString>

class Move
{
private:
    QString name;
    QString type;
    QString attackType;
	int power;
	int accuracy;
	int powerPoint;
    int maxPowerPoint;
    QString effect;
public:
	Move();
    Move(QString, QString, QString, int, int, int, QString);

    void setName(QString);
    void setType(QString);
    void setAttackType(QString);
	void setPower(int);
	void setAccuracy(int);
	void setPowerPoint(int);
    void setEffect(QString);

    QString getName();
    QString getType();
    QString getAttackType();
	int getPower();
	int getAccuracy();
	int getPowerPoint();
    int getMaxPowerPoint();
    QString getEffect();
};
