#ifndef MENUWINDOW_H
#define MENUWINDOW_H
#include "gamewindow.h"
#include "winresultwindow.h"
#include "loseresultwindow.h"

#include "Item.h"
#include "Pokemon.h"
#include "Trainer.h"

#include <algorithm>
#include <time.h>
#include <cmath>

#include <QMainWindow>
#include <QMediaPlayer>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QWidget>
#include <QDebug>
#include <QCoreApplication>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MenuWindow; }
QT_END_NAMESPACE

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();
    void handlePokemonFile(QString filePath);
    void handleMoveFile(QString filePath);
    void handleGameDataFile(QString filePath);
    void handleCaseFile();
    void handleResetFile();


private slots:
    void on_playBtn_clicked();
    void moveToMenu();
    void moveToResult();
    void moveToGame();

    void on_caseBtn_clicked();

    void on_resetBtn_clicked();

    void on_pkmnLibBtn_clicked();

    void on_moveLibBtn_clicked();

    void on_gameDataBtn_clicked();

    void on_testBtn_clicked();

    void on_leftBtn_clicked();

    void on_rightBtn_clicked();

private:
    Ui::MenuWindow *ui;
    GameWindow _GameWindowPage;
    WinResultWindow _WinResultWindowPage;
    LoseResultWindow _LoseResultWindowPage;
    int enemyIndex;
    QMediaPlayer* music = new QMediaPlayer;
    QString caseFilePath;
    bool isTestMode;

    QVector<Pokemon> pokedex;
    QVector<Move> moveCodex;
    QVector<Trainer> trainerList;

    QVector<QString> enemyName = {"Gary", "Red", "May", "N", "Cynthia", "Clay", "Iris", "Raihan", "Giovanni", "Teacher Assistant"};
    QVector<QString> enemyPkmn = {"umbreon", "charizard-mega-x", "blaziken-mega", "furret", "garchomp", "krookodile", "haxorus", "duraludon", "mewtwo", "shiny_hisuian_zorua"};


    QVector<QVector<double>> effectivenessChart = {
        {1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5,  0.0,  1.0,  1.0,  0.5,  1.0},
        {1.0,  0.5,  0.5,  1.0,  2.0,  2.0,  1.0,  1.0,  1.0,  1.0,  1.0,  2.0,  0.5,  1.0,  0.5,  1.0,  2.0,  1.0},
        {1.0,  2.0,  0.5,  1.0,  0.5,  1.0,  1.0,  1.0,  2.0,  1.0,  1.0,  1.0,  2.0,  1.0,  0.5,  1.0,  1.0,  1.0},
        {1.0,  1.0,  2.0,  0.5,  0.5,  1.0,  1.0,  1.0,  0.0,  2.0,  1.0,  1.0,  1.0,  1.0,  0.5,  1.0,  1.0,  1.0},
        {1.0,  0.5,  2.0,  1.0,  0.5,  1.0,  1.0,  0.5,  2.0,  0.5,  1.0,  0.5,  2.0,  1.0,  0.5,  1.0,  0.5,  1.0},
        {1.0,  0.5,  0.5,  1.0,  2.0,  0.5,  1.0,  1.0,  2.0,  2.0,  1.0,  1.0,  1.0,  1.0,  2.0,  1.0,  0.5,  1.0},
        {2.0,  1.0,  1.0,  1.0,  1.0,  2.0,  1.0,  0.5,  1.0,  0.5,  0.5,  0.5,  2.0,  0.0,  1.0,  2.0,  2.0,  0.5},
        {1.0,  1.0,  1.0,  1.0,  2.0,  1.0,  1.0,  0.5,  0.5,  1.0,  1.0,  1.0,  0.5,  0.5,  1.0,  1.0,  0.0,  2.0},
        {1.0,  2.0,  1.0,  2.0,  0.5,  1.0,  1.0,  2.0,  1.0,  0.0,  1.0,  0.5,  2.0,  1.0,  1.0,  1.0,  2.0,  1.0},
        {1.0,  1.0,  1.0,  0.5,  2.0,  1.0,  2.0,  1.0,  1.0,  1.0,  1.0,  2.0,  0.5,  1.0,  1.0,  1.0,  0.5,  1.0},
        {1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  2.0,  2.0,  1.0,  1.0,  0.5,  1.0,  1.0,  1.0,  1.0,  0.0,  0.5,  1.0},
        {1.0,  0.5,  1.0,  1.0,  2.0,  1.0,  0.5,  0.5,  1.0,  0.5,  2.0,  1.0,  1.0,  0.5,  1.0,  2.0,  0.5,  0.5},
        {1.0,  2.0,  1.0,  1.0,  1.0,  2.0,  0.5,  1.0,  0.5,  2.0,  1.0,  2.0,  1.0,  1.0,  1.0,  1.0,  0.5,  1.0},
        {0.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  2.0,  1.0,  1.0,  2.0,  1.0,  0.5,  1.0,  1.0},
        {1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  2.0,  1.0,  0.5,  0.0},
        {1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5,  1.0,  1.0,  1.0,  2.0,  1.0,  1.0,  2.0,  1.0,  0.5,  1.0,  0.5},
        {1.0,  0.5,  0.5,  0.5,  1.0,  2.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  2.0,  1.0,  1.0,  1.0,  0.5,  2.0},
        {1.0,  0.5,  1.0,  1.0,  1.0,  1.0,  2.0,  0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  2.0,  2.0,  0.5,  1.0}
    };
};
#endif // MENUWINDOW_H
