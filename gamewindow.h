#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <cctype>
#include <time.h>

#include <QFile>
#include <QTextStream>
#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QColor>
#include <QMediaPlayer>

#include "Pokemon.h"
#include "Move.h"
#include "Item.h"
#include "Trainer.h"


namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    void initializeGame(bool isTestMode, QString filePath, QVector<Trainer> trainerList);
    void resetGame();
    void updateBaseMenuUI();
    void updateBattleMenuUI();
    void updateBagMenuUI();
    void updateSwitchMenuUI();
    void updatePlayerPokemonUI();
    void updateEnemyPokemonUI();
    void refreshUI();

    void handleRunCommand();
    void handlePrintInfo();
    void handleBattle(bool isOnlyEnemyAttack);
    void handleBag();
    void handleSwap(bool isSwitchCommand);
    void printStatus();
    void printCheck();

    void handleUseItem();
    void handleAttack(Pokemon& attacker, Move& usedMove, Pokemon& defender);
    void checkHandlePlayerFainted();
    void checkHandleEnemyFainted();

    bool checkGameEnded();
    bool checkHandleMoveValid(QString moveCommand, bool isFindingPlayerMove);
    bool checkHandleParalyzed(Pokemon attacker);
    void checkHandleConditional();
    void setEnemyInfo(QString name, QString imgUrl);

    QString getFormatSpace(QString string);
    QString getEnemyRandomMove();
    int getElementTypingIndex(QString type);
    int getDamageCalculation(Pokemon& attacker, Move& usedMove, Pokemon& defender);
    bool getResult();

signals:
    void ShowResultClicked();

private slots:
    void on_resultBtn_clicked();

    void on_pushButton_0_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_backBtn_clicked();



private:
    Ui::GameWindow *ui;
    QMediaPlayer* music = new QMediaPlayer;
    QMediaPlayer* health_effect = new QMediaPlayer;
    QVector<Item> playerItemList;
    QVector<Pokemon> playerPokemonList;
    QVector<Pokemon> enemyPokemonList;
    bool isReadingFile = false;
    QFile file;
    QTextStream iFile;
    QLabel* logLabel = new QLabel();
    QString logList;
    QVector<QVector<QString>> menuText;
    Item* selectedItem;
    QString trainerName;

    Pokemon* currentPlayerPokemon;
    Pokemon* currentEnemyPokemon;

    QString playerMoveCommand;
    QString enemyMoveCommand;
    QString selectedItemCommand;
    QString selectedPokemonCommand;

    // menuStyleIndex: 0 for normal menu, 1 for battle menu, 2 for bag menu, 3 for switch pokemon menu
    int menuStyleIndex = 0;
    int turnCounter = 1;
    bool isTestMode = false;
    bool isGameOver = false;
    bool isPlayerWin = false;
    bool isErrorCommand = false;
    bool isPlayerTurn = true;
    bool isTurnFinished = false;
    bool isWaitingClick = false;

    QString baseButtonStyling = "border: 1px solid black; color: #202020; padding: 2.5px; text-transform: uppercase;";

    QVector<QString> typeColors = { "#010204", "#FE9C53", "#4D90D5", "#F4D23B", "#63BB5C", "#73CEBF",
                                   "#CE3F6A", "#AC69C9", "#D97746", "#8FA8DE", "#F97077", "#90C02B",
                                   "#C8B78B", "#5269AB", "#096DC3", "#5A5365", "#5A8FA1", "#EC8FE7" };

    QVector<QVector<float>> effectivenessChart = {
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

#endif // GAMEWINDOW_H
