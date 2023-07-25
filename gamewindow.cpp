/***********************************************************************
 * File: GameWindow.cpp
 * Author:	鍾詩靈 B11115010
 *			陳仕興 B11115011
 *			魏美芳 B11115014
 * Create Date: 2023-05-26
 * Editor:	鍾詩靈 B11115010
 *			陳仕興 B11115011
 *			魏美芳 B11115014
 * Update Date: 2023-06-06
 * Description: This C++ program define the GameWindow class
***********************************************************************/
#include "gamewindow.h"
#include "ui_gamewindow.h"

// Intent: Create an instance of GameWindow.
// Pre: Parent widget must be provided.
// Post: GameWindow is created with necessary UI elements and connections.
GameWindow::GameWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    // Flip the player's Pokemon image horizontally
    QPixmap pixmap = ui->player_pkmn->pixmap()->transformed(QTransform().scale(-1, 1));
    ui->player_pkmn->setPixmap(pixmap);

    // Set the seed for random number generation based on current time
    std::srand(time(0));

    // Configure the log label
    this->logLabel->setWordWrap(true);
    this->logLabel->setAlignment(Qt::AlignTop);
    this->logLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    this->logLabel->setCursor(Qt::IBeamCursor);
    this->logLabel->setStyleSheet("padding: 5px; background: #eeeeee; color: #202020; border: 10px solid #625F66;");

    // Set the log label as the widget inside the scroll area
    ui->scrollArea->setWidget(this->logLabel);

    // Configure the health effect media player
    this->health_effect->setMedia(QUrl("qrc:/audio/audio/low_hp.mp3"));
    this->health_effect->pause();

    // Connect to mediaStatusChanged signal
    connect(this->health_effect, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
        // Check whether the music has already finished being play
        if (status == QMediaPlayer::EndOfMedia) {
            // Restart the music when it has finished playing
            this->health_effect->play();
        }
    });
}

// Intent: Destroy the GameWindow instance and free up resources.
// Pre: GameWindow object must exist.
// Post: GameWindow object is deleted.
GameWindow::~GameWindow()
{
    delete ui;
}

// Intent: Handle the button click event for the result button.
// Pre: The result button must be clicked.
// Post: If the game is not over, the application is quit. Otherwise, the ShowResultClicked signal is emitted.
void GameWindow::on_resultBtn_clicked()
{
    // Quit the application if the game is not over
    if (!this->isGameOver)
    {
        QApplication::quit();
    }
    // Emit the ShowResultClicked signal if the game is over
    else
    {
        emit ShowResultClicked();
    }
}

// Intent: Reset the game to its initial state.
// Pre: None.
// Post: All game-related variables and data are reset.
void GameWindow::resetGame()
{
    // Clear player's item list and Pokemon list
    playerItemList.clear();
    playerPokemonList.clear();
    enemyPokemonList.clear();

    // Reset current player and enemy Pokemon
    this->currentPlayerPokemon = nullptr;
    this->currentEnemyPokemon = nullptr;

    // Reset game-related variables
    this->turnCounter = 1;
    this->isTestMode = false;
    this->isGameOver = false;
    this->isPlayerWin = false;
    this->isErrorCommand = false;
    this->isPlayerTurn = true;
    this->isTurnFinished = false;
    this->isWaitingClick = false;

    // Reset command variables
    this->playerMoveCommand = "";
    this->enemyMoveCommand = "";
    this->selectedItemCommand = "";
    this->selectedPokemonCommand = "";

    // Reset menu text, log list, and log label
    this->menuText.clear();
    this->menuText.push_back({ "Fight", "Bag", "Pokemon", "Check Info" });
    this->logList = "";
    this->logLabel->setText("");

    // Set result button text
    ui->resultBtn->setText("Exit Game");

    // Reset progress bar styles and values
    QString progressStyle = "QProgressBar { color: white; border: 2px solid #eeeeee; background: rgba(0, 0, 0, 0); text-align: center;} QProgressBar::chunk { background-color: #01A651; }";
    ui->player_progress->setStyleSheet(progressStyle);
    ui->enemy_progress->setStyleSheet(progressStyle);
    ui->player_progress->setValue(100);
    ui->enemy_progress->setValue(100);
}

// Intent: Initialize the game with the specified parameters.
// Pre: None.
// Post: The game is initialized with the provided settings and data.
void GameWindow::initializeGame(bool isTestMode, QString filePath, QVector<Trainer> trainerList) {
    // Set all pokeball to black and white first
    for (int i = 0; i < 3; i++) {
        QLabel* pokeball = findChild<QLabel*>("player_active_" + QString::number(i));
        QString pokeballUrl = ":/img/img/Pokeball_BW.png";

        // Set Pixmap
        pokeball->setPixmap(pokeballUrl);
    }

    // Set all pokeball to black and white first
    for (int i = 0; i < 3; i++) {
        QLabel* pokeball = findChild<QLabel*>("enemy_active_" + QString::number(i));
        QString pokeballUrl = ":/img/img/Pokeball_BW.png";

        // Set Pixmap
        pokeball->setPixmap(pokeballUrl);
    }

    // Set the volume for music and health effect sounds
    this->music->setVolume(30);
    this->health_effect->setVolume(30);

    // Reset the game to its initial state
    this->resetGame();
    this->isTestMode = isTestMode;

    // Retrieve player's item list and Pokemon list from the trainer data
    playerItemList = trainerList[0].getItem();
    playerPokemonList = trainerList[0].getPokemon();
    enemyPokemonList = trainerList[1].getPokemon();

    // Set the current player and enemy Pokemon to the first ones in their respective lists
    this->currentPlayerPokemon = &playerPokemonList[0];
    this->currentEnemyPokemon = &enemyPokemonList[0];

    // Populate menu text with move names, item names, and Pokemon names
    QVector<QString> moveNameList;
    QVector<QString> itemNameList;
    QVector<QString> pokemonNameList;

    // Retrieve move names from the current player's Pokemon and add them to moveNameList
    for (Move& move : this->currentPlayerPokemon->getMoves()) {
        moveNameList.push_back(move.getName());
    }

    // Retrieve item names from the player's item list and add them to itemNameList
    for (Item& item : this->playerItemList) {
        itemNameList.push_back(item.getName());
    }

    // Retrieve Pokemon names from the player's Pokemon list and add them to pokemonNameList
    for (Pokemon& pokemon : this->playerPokemonList) {
        pokemonNameList.push_back(pokemon.getName());
    }

    // Update the menuText with moveNameList, itemNameList, and pokemonNameList
    this->menuText.push_back(moveNameList);
    this->menuText.push_back(itemNameList);
    this->menuText.push_back(pokemonNameList);

    // Update the UI elements related to the player and enemy Pokemon
    this->updatePlayerPokemonUI();
    this->updateEnemyPokemonUI();
    this->updateBaseMenuUI();

    // Check if filePath is empty, if so, set isReadingFile to false and return
    if (filePath == "") {
        this->isReadingFile = false;
        return;
    }

    // Set isReadingFile to true and open the file
    this->isReadingFile = true;
    this->file.setFileName(filePath);

    // Check whether the file can be opened
    if (!this->file.open(QIODevice::ReadOnly | QFile::Text)) {
        return;
    }

    // Set iFile to read file
    this->iFile.setDevice(&file);

    QString command;

    // Skip the first three lines in the file
    for (int i = 0; i < 3; i++) {
        iFile >> command;
    }

    // Read the next command
    iFile >> command;

    // Check if the command is "Test" to set isTestMode to true
    if (command == "Test") {
        this->isTestMode = true;
    }

    // Handle the "Run" command from the file
    this->handleRunCommand();
}


// Intent: Refresh the game UI.
// Pre: None.
// Post: The UI elements are updated based on the game state.
void GameWindow::refreshUI() {
    this->updatePlayerPokemonUI();
    this->updateEnemyPokemonUI();
    this->updateBaseMenuUI();

    this->logLabel->setText(this->logList);
}

// Intent: Update the UI elements related to the base menu.
// Pre: None.
// Post: The base menu UI is updated based on the game state.
void GameWindow::updateBaseMenuUI() {
    // Show all the menu buttons
    ui->pushButton_0->show();
    ui->pushButton_1->show();
    ui->pushButton_2->show();
    ui->pushButton_3->show();

    // Set index as which menu indicator
    this->menuStyleIndex = 0;

    // Update the button styling and text based on the menu text
    for (int i = 0; i < 4; i++) {
        QPushButton* button = findChild<QPushButton*>("pushButton_" + QString::number(i));
        button->setStyleSheet(this->baseButtonStyling);
        button->setText(this->menuText[0][i]);
    }

    // Update label for base menu
    ui->msg_label->setText("What will " + this->currentPlayerPokemon->getName() + " do?");

    // Hide 'back button' on base menu
    ui->backBtn->hide();

    // Check if the game is over
    if (this->isGameOver) {
        // Hide some menu buttons
        ui->pushButton_0->hide();
        ui->pushButton_1->hide();
        ui->pushButton_2->hide();

        // Play win sound effect and display victory message if the player win
        if (this->isPlayerWin) {
            this->health_effect->pause();
            this->music->setMedia(QUrl("qrc:/audio/audio/win.mp3"));
            this->music->play();
            ui->msg_label->setText("You defeated trainer " + this->trainerName + "!");
        }
        // Play lose sound effect and display defeat message if the player lose
        else {
            this->health_effect->pause();
            this->music->setMedia(QUrl("qrc:/audio/audio/lose.mp3"));
            this->music->play();
            ui->msg_label->setText("You were defeated by trainer " + this->trainerName + "!");
        }
    }
}



// Intent: Update the UI elements related to the battle menu.
// Pre: None.
// Post: The battle menu UI is updated based on the game state.
void GameWindow::updateBattleMenuUI() {
    // Set index as which menu indicator
    this->menuStyleIndex = 1;

    // Set the message label text
    ui->msg_label->setText("What will " + this->currentPlayerPokemon->getName() + " do?");

    // Get pokemon moves
    QVector<Move> moveList = this->currentPlayerPokemon->getMoves();

    // Get all button and set styling and functionality of battle menu
    for (int i = 0; i < 4; i++) {
        QPushButton* button = findChild<QPushButton*>("pushButton_" + QString::number(i));

        // Create the move information string
        QString moveInfo = "[" + moveList[i].getType() + "]   " + this->getFormatSpace(moveList[i].getName()) + "   PP " + QString::number(moveList[i].getPowerPoint()) + "/" + QString::number(moveList[i].getMaxPowerPoint());
        button->setText(moveInfo);

        // Set the button style based on the move type
        int typeIndex = this->getElementTypingIndex(moveList[i].getType());
        QString backgroundColorHex = typeColors[typeIndex];
        QColor backgroundColor(backgroundColorHex);

        // Calculate the relative luminance of the background color
        qreal bgLuminance = (0.2126 * backgroundColor.redF()) +
                            (0.7152 * backgroundColor.greenF()) +
                            (0.0722 * backgroundColor.blueF());

        // Define the threshold for determining if the background is dark
        qreal luminanceThreshold = 0.5;

        // Choose a light text color if the background is dark, otherwise choose a dark text color
        QString textColor = (bgLuminance <= luminanceThreshold) ? "white" : "black";

        // Set the button style with the appropriate text color and background color
        QString buttonStyle = this->baseButtonStyling;
        button->setStyleSheet(buttonStyle + "padding: 0px 10px; text-align: left; color: " + textColor + "; background: " + backgroundColorHex + ";");
    }

    // Show 'back button'
    ui->backBtn->show();
}

// Intent: Update the UI elements related to the bag menu.
// Pre: None.
// Post: The bag menu UI is updated based on the game state.
void GameWindow::updateBagMenuUI() {
    // Set index as which menu indicator
    this->menuStyleIndex = 2;

    // Get itemList
    QVector<Item> itemList = this->playerItemList;

    // Update the button styling and text based on the item list
    for (int i = 0; i < 4; i++) {
        QPushButton* button = findChild<QPushButton*>("pushButton_" + QString::number(i));
        button->setStyleSheet(this->baseButtonStyling);
        button->setText(this->getFormatSpace(itemList[i].getName()));
    }

    // Set the message label text
    ui->msg_label->setText("Select an item to be used: ");

    ui->backBtn->show();
}


// Intent: Update the UI elements related to the switch menu.
// Pre: None.
// Post: The switch menu UI is updated based on the game state.
void GameWindow::updateSwitchMenuUI() {
    // Set index as which menu indicator
    this->menuStyleIndex = 3;

    // Get pokemon list
    QVector<Pokemon> pokemonList = this->playerPokemonList;

    // Get the first three button and set functionality to switch pokemon using for loop
    for (int i = 0; i < 3; i++) {
        QPushButton* button = findChild<QPushButton*>("pushButton_" + QString::number(i));
        QString buttonStyle = this->baseButtonStyling;

        // Set the button style based on the Pokemon's HP
        if (pokemonList[i].getHp() > 0) {
            buttonStyle += "background: #2CB279; color: #eeeeee;";
        }
        else {
            button->hide();
        }

        // Hide the button if the player is not waiting for a click and it's the current player's Pokemon
        if (!this->isWaitingClick && (this->currentPlayerPokemon->getName() == pokemonList[i].getName())) {
            button->hide();
        }

        // Set the button text with the Pokemon's name and HP
        button->setText(pokemonList[i].getName() + "   HP " + QString::number(pokemonList[i].getHp()) + "/" + QString::number(pokemonList[i].getMaxHp()));
        button->setStyleSheet(buttonStyle);
    }

    // Set the style and text of the 4th button (not used)
    ui->pushButton_3->setStyleSheet(this->baseButtonStyling);
    ui->pushButton_3->setText("");
    ui->pushButton_3->hide();

    QString msg = "Choose a Pokemon: ";

    // Show the back button if the player is not waiting for a click
    if (!isWaitingClick) {
        ui->backBtn->show();
    }

    // Handle case of waiting for player click (using item or switch fainted pokemon)
    if (this->isWaitingClick) {
        // Update the message label text based on the game state
        if (this->currentPlayerPokemon->getHp() <= 0) {
            msg = this->currentPlayerPokemon->getName() + " is fainted! Choose next Pokemon: ";
            this->health_effect->pause();
        }
        else {
            msg = "Choose target pokemon: ";
        }

        // Hide the back button if the player is waiting for a click
        ui->backBtn->hide();
    }

    // Set the message label text
    ui->msg_label->setText(msg);
}

// Intent: Update the user interface elements related to the player's Pokemon to reflect the current state.
// Pre: Valid playerPokemonList and currentPlayerPokemon. Initialized UI elements.
// Post: UI elements display updated information about the player's Pokemon accurately.
void GameWindow::updatePlayerPokemonUI() {
    // Loop through each player's Pokemon
    for (int i = 0; i < this->playerPokemonList.size(); i++) {
        QLabel* pokeball = findChild<QLabel*>("player_active_" + QString::number(i));
        QString pokeballUrl = ":/img/img/Pokeball";

        // Set the pokeball image based on the Pokemon's HP
        if (this->playerPokemonList[i].getHp() <= 0) {
            pokeballUrl += "_BW.png";
        }
        else {
            pokeballUrl += ".png";
        }

        pokeball->setPixmap(pokeballUrl);
    }

    // Set the player Pokemon's image and name
    QString playerPokemonName = this->currentPlayerPokemon->getName();
    QString playerImgUrl = ":/sprites/sprites/" + playerPokemonName.toLower() + ".png";

    // Set image path to image file
    QFile imageFile(playerImgUrl);

    // If the Pokemon's image file doesn't exist, use a substitute image
    if (!imageFile.exists()) {
        playerImgUrl = ":/sprites/img/subs.png";
    }

    ui->player_pkmn->setPixmap(playerImgUrl);

    // Flip the player Pokemon's image horizontally
    QPixmap pixmap = ui->player_pkmn->pixmap()->transformed(QTransform().scale(-1, 1));
    ui->player_pkmn->setPixmap(pixmap);

    // Update pokemon name label
    ui->player_pkmn_label->setText(playerPokemonName);

    int index = 0;

    // Reset the status labels and styling for the player Pokemon
    if (this->currentPlayerPokemon->getStatus().empty()) {
        ui->player_pkmn_status_0->setStyleSheet("background: #D2242A; color: white;");
        ui->player_pkmn_status_1->setStyleSheet("background: #D2242A; color: white;");
        ui->player_pkmn_status_2->setStyleSheet("background: #D2242A; color: white;");
        ui->player_pkmn_status_0->setText("");
        ui->player_pkmn_status_1->setText("");
        ui->player_pkmn_status_2->setText("");
    }

    // Update the status labels and styling for the player Pokemon based on its status conditions
    for (auto& status : this->currentPlayerPokemon->getStatus()) {
        QLabel* statusLabel = findChild<QLabel*>("player_pkmn_status_" + QString::number(index));
        statusLabel->setText("");
        statusLabel->setStyleSheet("background: #D2242A; color: white;");

        if (status == "PAR") {
            statusLabel->setText("PAR");
            statusLabel->setStyleSheet("background: #FFCB38; color: white;");
            index++;
        }
        else if (status == "PSN") {
            statusLabel->setText("PSN");
            statusLabel->setStyleSheet("background: #A738FF; color: white;");
            index++;
        }
        else if (status == "BRN") {
            statusLabel->setText("BRN");
            statusLabel->setStyleSheet("background: #FF8038; color: white;");
            index++;
        }
    }

    // Calculate the percentage of health progress for the player Pokemon
    double percentage = ((double)this->currentPlayerPokemon->getHp() / (double)this->currentPlayerPokemon->getMaxHp()) * 100;

    QString progressStyle = "QProgressBar { color: white; border: 2px solid #eeeeee; background: rgba(0, 0, 0, 0); text-align: center;} ";

    // Set the progress bar chunk color based on the health percentage
    if (percentage > 50) {
        progressStyle += "QProgressBar::chunk { background-color: #01A651; }";
        this->health_effect->pause();
    }
    else if (percentage > 20) {
        progressStyle += "QProgressBar::chunk { background-color: gold; }";
        this->health_effect->pause();
    }
    else {
        progressStyle += "QProgressBar::chunk { background-color: red; }";
        this->health_effect->play();
    }

    // Update styling of progress bar
    ui->player_progress->setStyleSheet(progressStyle);

    // Set double percentage to int
    int displayedNum = (int)percentage;

    // Ensure the progress bar always displays at least 1% when the Pokemon is alive but has 0% displayed
    if (displayedNum <= 0 && this->currentPlayerPokemon->getHp() > 0) {
        displayedNum = 1;
    }

    // Update the progress bar value with the calculated percentage
    ui->player_progress->setValue(displayedNum);
}

// Intent: Update the UI elements related to the enemy's Pokemon.
// Pre: None.
// Post: The enemy's Pokemon UI is updated based on the game state.
void GameWindow::updateEnemyPokemonUI() {
    // Update the pokeball images based on the enemy Pokemon's HP
    for (int i = 0; i < this->enemyPokemonList.size(); i++) {
        QLabel* pokeball = findChild<QLabel*>("enemy_active_" + QString::number(i));
        QString pokeballUrl = ":/img/img/Pokeball";

        // Set the pokeball image to black and white if the enemy Pokemon has 0 HP
        if (this->enemyPokemonList[i].getHp() <= 0) {
            pokeballUrl += "_BW.png";
        }
        // Set the pokeball image to colored if the enemy Pokemon has HP
        else {
            pokeballUrl += ".png";
        }

        pokeball->setPixmap(pokeballUrl);
    }

    QString enemyPokemonName = this->currentEnemyPokemon->getName();
    QString enemyImgUrl = ":/sprites/sprites/" + enemyPokemonName.toLower() + ".png";

    // Set image path to image file
    QFile imageFile(enemyImgUrl);

    // Use a substitute image if the enemy Pokemon image file does not exist
    if (!imageFile.exists()) {
        enemyImgUrl = ":/sprites/img/subs.png";
    }

    ui->enemy_pkmn->setPixmap(enemyImgUrl);

    ui->enemy_pkmn_label->setText(enemyPokemonName);

    int index = 0;

    // Clear the status labels if the enemy Pokemon has no status effects
    if (this->currentEnemyPokemon->getStatus().empty()) {
        ui->enemy_pkmn_status_0->setStyleSheet("background: #D2242A; color: white;");
        ui->enemy_pkmn_status_1->setStyleSheet("background: #D2242A; color: white;");
        ui->enemy_pkmn_status_2->setStyleSheet("background: #D2242A; color: white;");
        ui->enemy_pkmn_status_0->setText("");
        ui->enemy_pkmn_status_1->setText("");
        ui->enemy_pkmn_status_2->setText("");
    }

    // Update the status labels based on the enemy Pokemon's status effects
    for (auto& status : this->currentEnemyPokemon->getStatus()) {
        QLabel* statusLabel = findChild<QLabel*>("enemy_pkmn_status_" + QString::number(index));
        statusLabel->setText("");
        statusLabel->setStyleSheet("background: #D2242A; color: white;");

        // Set the status label to "PAR" for paralyzed status
        if (status == "PAR") {
            statusLabel->setText("PAR");
            statusLabel->setStyleSheet("background: #FFCB38; color: white;");
            index++;
        }
        // Set the status label to "PSN" for poisoned status
        else if (status == "PSN") {
            statusLabel->setText("PSN");
            statusLabel->setStyleSheet("background: #A738FF; color: white;");
            index++;
        }
        // Set the status label to "BRN" for burned status
        else if (status == "BRN") {
            statusLabel->setText("BRN");
            statusLabel->setStyleSheet("background: #FF8038; color: white;");
            index++;
        }
    }

    // Calculate the percentage of health progress
    double percentage = ((double)this->currentEnemyPokemon->getHp() / (double)this->currentEnemyPokemon->getMaxHp()) * 100;

    QString progressStyle = "QProgressBar { color: white; border: 2px solid #eeeeee; background: rgba(0, 0, 0, 0); text-align: center;} ";

    // Set the progress bar color to green if health > 50%
    if (percentage > 50) {
        progressStyle += "QProgressBar::chunk { background-color: #01A651; }";
    }
    // Set the progress bar color to gold if health > 20%
    else if (percentage > 20) {
        progressStyle += "QProgressBar::chunk { background-color: gold; }";
    }
    // Set the progress bar color to red if health <= 20%
    else {
        progressStyle += "QProgressBar::chunk { background-color: red; }";
    }

    // Set styling for progress bar
    ui->enemy_progress->setStyleSheet(progressStyle);

    // Set double percentage to int
    int displayedNum = (int)percentage;

    // Ensure the progress bar displays at least 1% when HP is greater than 0
    if (displayedNum <= 0 && this->currentEnemyPokemon->getHp() > 0) {
        displayedNum = 1;
    }

    // Update the progress bar with the calculated percentage
    ui->enemy_progress->setValue(displayedNum);
}

// Intent: Handle the button click event for button 0.
// Pre: None.
// Post: The corresponding action is performed based on the current menu style.
void GameWindow::on_pushButton_0_clicked()
{
    // Update the battle menu UI
    if (this->menuStyleIndex == 0) {
        this->updateBattleMenuUI();
    }
    // Handle the battle based on the selected move
    else if (this->menuStyleIndex == 1) {
        this->playerMoveCommand = this->menuText[1][0];
        this->enemyMoveCommand = this->getEnemyRandomMove();
        this->handleBattle(false);
    }
    // Handle the bag selection
    else if (this->menuStyleIndex == 2) {
        this->selectedItemCommand = this->menuText[2][0];
        this->enemyMoveCommand = this->getEnemyRandomMove();
        this->handleBag();
    }
    // Handle swapping the active Pokemon or using an item
    else if (this->menuStyleIndex == 3) {
        this->selectedPokemonCommand = this->menuText[3][0];
        this->enemyMoveCommand = this->getEnemyRandomMove();

        // Handle case when waiting for player click (use item on pokemon or switch fainted pokemon)
        if (this->isWaitingClick) {
            // Handle swapping the active Pokemon
            if (this->currentPlayerPokemon->getHp() <= 0) {
                this->handleSwap(false);
            }
            // Handle using an item
            else {
                this->handleUseItem();
            }
        }
        // Handle swapping the active Pokemon
        else {
            this->handleSwap(true);
        }

        // Refresh the UI elements
        this->refreshUI();
    }
}

// Intent: Handle the button click event for button 1.
// Pre: None.
// Post: The corresponding action is performed based on the current menu style.
void GameWindow::on_pushButton_1_clicked()
{

    // Update the bag menu UI
    if (this->menuStyleIndex == 0) {
        this->updateBagMenuUI();
    }
    // Handle the battle based on the selected move
    else if (this->menuStyleIndex == 1) {
        this->playerMoveCommand = this->menuText[1][1];
        this->enemyMoveCommand = this->getEnemyRandomMove();
        this->handleBattle(false);
    }
    // Handle the bag selection
    else if (this->menuStyleIndex == 2) {
        this->selectedItemCommand = this->menuText[2][1];
        this->enemyMoveCommand = this->getEnemyRandomMove();
        this->handleBag();
    }
    else if (this->menuStyleIndex == 3) {
        this->selectedPokemonCommand = this->menuText[3][1];
        this->enemyMoveCommand = this->getEnemyRandomMove();

        // Handle case when waiting for player click (use item on pokemon or switch fainted pokemon)
        if (this->isWaitingClick) {
            // Handle swapping the active Pokemon
            if (this->currentPlayerPokemon->getHp() <= 0) {
                this->handleSwap(false);
            }
            // Handle using an item
            else {
                this->handleUseItem();
            }
        }
        // Handle swapping the active Pokemon
        else {
            this->handleSwap(true);
        }

        // Refresh the UI elements
        this->refreshUI();
    }
}

// Intent: Handle the button click event for button 2.
// Pre: None.
// Post: The corresponding action is performed based on the current menu style.
void GameWindow::on_pushButton_2_clicked()
{
    // Update the switch menu UI
    if (this->menuStyleIndex == 0) {
        this->updateSwitchMenuUI();
    }
    // Handle the battle based on the selected move
    else if (this->menuStyleIndex == 1) {
        this->playerMoveCommand = this->menuText[1][2];
        this->enemyMoveCommand = this->getEnemyRandomMove();
        this->handleBattle(false);
    }
    // Handle the bag selection
    else if (this->menuStyleIndex == 2) {
        this->selectedItemCommand = this->menuText[2][2];
        this->enemyMoveCommand = this->getEnemyRandomMove();
        this->handleBag();
    }
    else if (this->menuStyleIndex == 3) {
        this->selectedPokemonCommand = this->menuText[3][2];
        this->enemyMoveCommand = this->getEnemyRandomMove();

        // Handle case when waiting for player click (use item on pokemon or switch fainted pokemon)
        if (this->isWaitingClick) {
            // Handle swapping the active Pokemon
            if (this->currentPlayerPokemon->getHp() <= 0) {
                this->handleSwap(false);
            }
            // Handle using an item
            else {
                this->handleUseItem();
            }
        }
        else {
            // Handle swapping the active Pokemon
            this->handleSwap(true);
            // Refresh the UI elements
            this->refreshUI();
        }
    }
}

// Intent: Handle the button click event for button 3.
// Pre: None.
// Post: The corresponding action is performed based on the current menu style.
void GameWindow::on_pushButton_3_clicked()
{
    // Handle printing information
    if (this->menuStyleIndex == 0) {
        this->handlePrintInfo();
    }
    // Handle the battle based on the selected move
    else if (this->menuStyleIndex == 1) {
        this->playerMoveCommand = this->menuText[1][3];
        this->enemyMoveCommand = this->getEnemyRandomMove();
        this->handleBattle(false);
    }
    // Handle the bag selection
    else if (this->menuStyleIndex == 2) {
        this->selectedItemCommand = this->menuText[2][3];
        this->enemyMoveCommand = this->getEnemyRandomMove();
        this->handleBag();
    }
}

// Intent: Handle the button click event for the back button.
// Pre: None.
// Post: The base menu UI is updated.
void GameWindow::on_backBtn_clicked()
{
    // Update the base menu UI
    this->updateBaseMenuUI();
}

// Intent: Handle printing the information of the current Pokémon and moves.
// Pre: The currentPlayerPokemon and currentEnemyPokemon pointers are valid.
// Post: The information is formatted and displayed in the logLabel.
void GameWindow::handlePrintInfo() {
    // Store references to the current player and enemy Pokémon
    Pokemon* currentPlayerPokemon = this->currentPlayerPokemon;
    Pokemon* currentEnemyPokemon = this->currentEnemyPokemon;

    // Create a QString to store the printed information
    QString infoString;

    // Append the turn
    infoString += "[Turn" + QString::number(this->turnCounter) + "] Current Pokémon Information\n\n";

    // Append player's Pokémon information
    infoString += "Player's Pokémon: " + currentPlayerPokemon->getName() + "\n";
    infoString += "Type: " + QStringList(currentPlayerPokemon->getType().toList()).join(", ") + "\n";
    infoString += "HP: " + QString::number(currentPlayerPokemon->getHp()) + "/" + QString::number(currentPlayerPokemon->getMaxHp()) + "\n";
    infoString += "Attack: " + QString::number(currentPlayerPokemon->getAttack()) + "\n";
    infoString += "Special Attack: " + QString::number(currentPlayerPokemon->getSpAttack()) + "\n";
    infoString += "Defense: " + QString::number(currentPlayerPokemon->getDefense()) + "\n";
    infoString += "Special Defense: " + QString::number(currentPlayerPokemon->getSpDefense()) + "\n";
    infoString += "Speed: " + QString::number(currentPlayerPokemon->getSpeed()) + "\n";

    // If player has no status, set it to NONE
    if (!currentPlayerPokemon->getStatus().empty()) {
        infoString += "Status: " + QStringList(currentPlayerPokemon->getStatus().toList()).join(" ") + "\n";
    }
    else {
        infoString += "Status: NONE\n";
    }

    // Append enemy's Pokémon information
    infoString += "\n\nEnemy's Pokémon: " + currentEnemyPokemon->getName() + "\n";
                  infoString += "Type: " + QStringList(currentEnemyPokemon->getType().toList()).join(", ") + "\n";
    infoString += "HP: " + QString::number(currentEnemyPokemon->getHp()) + "/" + QString::number(currentEnemyPokemon->getMaxHp()) + "\n";
    infoString += "Attack: " + QString::number(currentEnemyPokemon->getAttack()) + "\n";
    infoString += "Special Attack: " + QString::number(currentEnemyPokemon->getSpAttack()) + "\n";
    infoString += "Defense: " + QString::number(currentEnemyPokemon->getDefense()) + "\n";
    infoString += "Special Defense: " + QString::number(currentEnemyPokemon->getSpDefense()) + "\n";
    infoString += "Speed: " + QString::number(currentEnemyPokemon->getSpeed()) + "\n";

    // If enemy has no status, set it to NONE
    if (!currentEnemyPokemon->getStatus().empty()) {
        infoString += "Status: " + QStringList(currentEnemyPokemon->getStatus().toList()).join(" ") + "\n";
    }
    else {
        infoString += "Status: NONE\n";
    }

    // Append player's Pokémon moves
    infoString += "\n\n" + currentPlayerPokemon->getName() + "'s Moves:\n";
    QVector<Move> playerMoves = currentPlayerPokemon->getMoves();
    for (Move& move : playerMoves) {
        infoString += "- " + this->getFormatSpace(move.getName()) + "\n";
        infoString += "   Type: " + move.getType() + "\n";
        infoString += "   Attack Type: " + move.getAttackType() + "\n";
        infoString += "   Power: " + QString::number(move.getPower()) + "\n";
        infoString += "   Accuracy: " + QString::number(move.getAccuracy()) + "\n";
        infoString += "   PP: " + QString::number(move.getPowerPoint()) + "/" + QString::number(move.getMaxPowerPoint()) + "\n";
        infoString += "   Effect: " + move.getEffect() + "\n";
    }

    // Append enemy's Pokémon moves
    infoString += "\n\n" + currentEnemyPokemon->getName() + "'s Moves:\n";
    QVector<Move> enemyMoves = currentEnemyPokemon->getMoves();
    for (Move& move : enemyMoves) {
        infoString += "- " + this->getFormatSpace(move.getName()) + "\n";
        infoString += "   Type: " + move.getType() + "\n";
        infoString += "   Attack Type: " + move.getAttackType() + "\n";
        infoString += "   Power: " + QString::number(move.getPower()) + "\n";
        infoString += "   Accuracy: " + QString::number(move.getAccuracy()) + "\n";
        infoString += "   PP: " + QString::number(move.getPowerPoint()) + "/" + QString::number(move.getMaxPowerPoint()) + "\n";
        infoString += "   Effect: " + move.getEffect() + "\n";
    }

    // Append the infoString to the logList and update the logLabel
    this->logList += infoString;
    this->logLabel->setText(this->logList);
}


// Intent: Handle the run command from the input file and execute corresponding actions.
// Pre: The input file is open and valid.
// Post: The commands from the input file are processed, and the game state is updated accordingly.
void GameWindow::handleRunCommand() {
    QString command;

    // Keep reading the file input until game over, end of file, or found error
    while (!this->isGameOver && !iFile.atEnd() && !isErrorCommand) {
        this->iFile >> command;

        isTurnFinished = false;

        // Handle battle command
        if (command == "Battle") {
            this->iFile >> this->playerMoveCommand;
            this->iFile >> this->enemyMoveCommand;
            this->handleBattle(false);
        }
        // Handle bag command
        else if (command == "Bag") {
            this->iFile >> this->selectedItemCommand;
            this->iFile >> this->selectedPokemonCommand;
            this->iFile >> this->enemyMoveCommand;
            this->handleBag();
        }
        // Handle Pokemon command
        else if (command == "Pokemon") {
            this->iFile >> this->selectedPokemonCommand;
            this->iFile >> this->enemyMoveCommand;
            this->handleSwap(true);
        }
        // Print status command
        else if (command == "Status") {
            this->printStatus();
        }
        // Print check command
        else if (command == "Check") {
            this->printCheck();
        }
        // Exit command, end the game
        else if (command == "Exit") {
            isGameOver = true;
        }

        // Update UI elements
        this->updatePlayerPokemonUI();
        this->updateEnemyPokemonUI();
        this->updateBaseMenuUI();
        this->logLabel->setText(this->logList);
    }

    // Display error message if there was an error command
    if (isErrorCommand) {
        this->logLabel->setText(this->logList);
    }

    // Set the game state to not reading file
    this->isReadingFile = false;
}


// Intent: Check if the move command is valid and has enough PP.
// Pre: moveCommand is the name of the move to be checked.
//      isFindingPlayerMove indicates whether the move is for the player's Pokémon or the enemy's Pokémon.
// Post: Returns true if the move is valid and has enough PP, false otherwise.
bool GameWindow::checkHandleMoveValid(QString moveCommand, bool isFindingPlayerMove) {
    bool isMoveUsageValid = false;
    bool isMoveHavePP = true;
    QString status;
    QVector<Move> currentPokemonMoves;

    // Determine the status and move list based on the player or enemy Pokémon
    if (isFindingPlayerMove) {
        status = "player " + currentPlayerPokemon->getName();
        currentPokemonMoves = currentPlayerPokemon->getMoves();
    }
    else {
        status = "opponent " + currentEnemyPokemon->getName();
        currentPokemonMoves = currentEnemyPokemon->getMoves();
    }

    // Check if the move exists in the Pokémon's move list and if it has enough PP
    for (Move& move : currentPokemonMoves) {
        if (move.getName() == moveCommand) {
            isMoveUsageValid = true;

            if (move.getPowerPoint() <= 0) {
                isMoveHavePP = false;
            }

            break;
        }
    }

    // Print error messages if the move is not valid or does not have enough PP
    if (!isMoveUsageValid) {
        QString errorMessage = moveCommand + " is not found on " + status + " " + currentPlayerPokemon->getName() + "'s move list. Ending battle...";
        isErrorCommand = true;
        return false;
    }
    else if (!isMoveHavePP) {
        QString errorMessage = moveCommand + " of " + status + currentPlayerPokemon->getName() + " does not have enough PP. Ending battle...";
        isErrorCommand = true;
        return false;
    }

    return true;
}

// Intent: Handle the battle between the player's Pokémon and the enemy's Pokémon.
// Pre: isOnlyEnemyAttack indicates if only the enemy should attack.
// Post: The battle is resolved, and the game state is updated accordingly.
void GameWindow::handleBattle(bool isOnlyEnemyAttack) {
    isTurnFinished = false;

    Move* playerMove = nullptr;
    Move* enemyMove = nullptr;

    // Check the validity of the player's move and assign the move pointer (if isOnlyEnemyAttack is false, this means player also attack)
    if (!isOnlyEnemyAttack) {
        // Check whether the move valid
        if (checkHandleMoveValid(this->playerMoveCommand, true)) {
            // Loop through the moves and store the founded move
            for (auto& move : currentPlayerPokemon->getMoves()) {
                if (this->playerMoveCommand == move.getName()) {
                    playerMove = &move;
                    break;
                }
            }
        }
        else {
            return;
        }
    }

    // Check the validity of the enemy's move and assign the move pointer
    if (checkHandleMoveValid(this->enemyMoveCommand, false)) {
        // Loop through the moves and store the founded move
        for (auto& move : currentEnemyPokemon->getMoves()) {
            if (this->enemyMoveCommand == move.getName()) {
                enemyMove = &move;
                break;
            }
        }
    }
    else {
        return;
    }

    Pokemon* firstActPokemon = nullptr;
    Pokemon* secondActPokemon = nullptr;

    Move* firstActMove = nullptr;
    Move* secondActMove = nullptr;

    isPlayerTurn = false;

    int playerPokemonSpeed = currentPlayerPokemon->getSpeed();
    int enemyPokemonSpeed = currentEnemyPokemon->getSpeed();

    // Adjust the Player Pokémon speeds based on their status
    for (auto status : currentPlayerPokemon->getStatus()) {
        if (status == "PAR") {
            playerPokemonSpeed /= 2;
            break;
        }
    }

    // Adjust the Enemy Pokémon speeds based on their status
    for (auto status : currentEnemyPokemon->getStatus()) {
        if (status == "PAR") {
            enemyPokemonSpeed /= 2;
        }
    }

    // Determine the order of actions based on Pokémon speeds
    if (!isOnlyEnemyAttack && playerPokemonSpeed >= enemyPokemonSpeed) {
        firstActPokemon = currentPlayerPokemon;
        secondActPokemon = currentEnemyPokemon;
        firstActMove = playerMove;
        secondActMove = enemyMove;
        isPlayerTurn = true;
    }
    else {
        firstActPokemon = currentEnemyPokemon;
        secondActPokemon = currentPlayerPokemon;
        firstActMove = enemyMove;
        secondActMove = playerMove;
    }

    // Check if the first acting Pokémon is paralyzed and handle its attack
    if (!checkHandleParalyzed(*firstActPokemon)) {
        handleAttack(*firstActPokemon, *firstActMove, *secondActPokemon);

        if (isPlayerTurn) {
            checkHandleEnemyFainted();
        }
        else {
            checkHandlePlayerFainted();
        }
    }

    isPlayerTurn = !isPlayerTurn;

    // Check if the turn is finished and handle conditional actions
    if (isOnlyEnemyAttack || isTurnFinished) {
        if (!isGameOver) {
            checkHandleConditional();
        }

        // When battle finished, add turn counter
        isTurnFinished = true;
        turnCounter++;
        return;
    }

    // Check if the second acting Pokémon is paralyzed and handle its attack
    if (!checkHandleParalyzed(*secondActPokemon) && !isTurnFinished) {
        handleAttack(*secondActPokemon, *secondActMove, *firstActPokemon);

        if (isPlayerTurn) {
            checkHandleEnemyFainted();
        }
        else {
            checkHandlePlayerFainted();
        }

        checkHandleConditional();
    }

    // When battle finished, add turn counter
    isTurnFinished = true;
    turnCounter++;
}

// Intent: Handle the swapping of Pokémon in battle.
// Pre: isSwapCommand indicates if it's a swap command or a switch command.
// Post: The active Pokémon is swapped with the selected Pokémon, and the game state is updated accordingly.
void GameWindow::handleSwap(bool isSwapCommand) {
    QVector<Pokemon> activePokemon;

    // Get the active Pokémon that have HP greater than 0
    for (auto& pokemon : playerPokemonList) {
        if (pokemon.getHp() > 0) {
            activePokemon.push_back(pokemon);
        }
    }

    QString switchedPokemon = this->selectedPokemonCommand;

    bool newPokemonFound = false;

    // Find the selected Pokémon in the player's Pokémon list
    for (auto& pokemon : playerPokemonList) {
        if (pokemon.getName() == switchedPokemon) {
            newPokemonFound = true;

            // Log the switch out and come back messages
            if (isSwapCommand) {
                QString turnMessage = "[Turn " + QString::number(turnCounter) + "] " + currentPlayerPokemon->getName() + ", switch out!";
                QString comeBackMessage = "[Turn " + QString::number(turnCounter) + "] " + "Come back!";
                this->logList += turnMessage + "\n";
                this->logList += comeBackMessage + "\n";
            }

            // Set the current player's Pokémon to the selected Pokémon
            currentPlayerPokemon = &pokemon;

            // Log the go message for the selected Pokémon
            QString goMessage = "[Turn " + QString::number(turnCounter) + "] " + "Go! " + switchedPokemon + "!";
            this->logList += goMessage + "\n";

            isTurnFinished = true;

            break;
        }
    }

    // Handle error if the selected Pokémon is not found in the player's Pokémon list
    if (!newPokemonFound) {
        QString errorMessage = switchedPokemon + " is not available in the trainer's party. Ending battle...";
        this->logList += errorMessage + "\n";
        isErrorCommand = true;
    }

    // Clear the menu text for moves and update it with the moves of the current player's Pokémon
    this->menuText[1].clear();
    for (Move& move : this->currentPlayerPokemon->getMoves()) {
        this->menuText[1].push_back(move.getName());
    }

    // Refresh the UI if waiting for click input
    if (this->isWaitingClick) {
        this->refreshUI();
    }

    this->isWaitingClick = false;

    // Handle battle if it's a swap command, otherwise, increment the turn counter
    if (isSwapCommand) {
        handleBattle(true);
    }
    else {
        turnCounter++;
    }
}

// Intent: Handle the bag command in battle.
// Pre: None.
// Post: The available items are displayed, and the game state is updated accordingly.
void GameWindow::handleBag() {
    QVector<Item> availableItems;

    // Get the available items that are not used (in non-test mode) or all items (in test mode)
    for (auto& item : this->playerItemList) {
        if (!isTestMode) {
            if (!item.getIsUsed()) {
                availableItems.push_back(item);
            }
        }
        else {
            availableItems.push_back(item);
        }
    }

    QString selectedItemName = this->selectedItemCommand;

    bool selectedItemNameFound = false;

    // Find the selected item in the player's item list
    for (Item& item : this->playerItemList) {
        if (item.getName() == selectedItemName) {
            selectedItemNameFound = true;
            this->selectedItem = &item;

            // Format the selected item name for display
            QString itemName = item.getName();
            selectedItemName = this->getFormatSpace(itemName);
            break;
        }
    }

    // Handle error if the selected item is not found in the player's item list
    if (!selectedItemNameFound) {
        QString errorMessage = selectedItemName + " is not available in the trainer's bag. Ending battle...";
        this->logList += errorMessage + "\n";
        isErrorCommand = true;
    }

    // Update the UI based on whether it's waiting for click input or reading a file
    if (!this->isReadingFile) {
        this->isWaitingClick = true;
        this->updateSwitchMenuUI();
    }
    else {
        this->handleUseItem();
    }
}

// Intent: Print the status of the Pokémon in battle.
// Pre: None.
// Post: The status of the current player's Pokémon and the current enemy's Pokémon is printed.
void GameWindow::printStatus() {
    QString statusMessage = "[Turn " + QString::number(turnCounter) + "] " + currentPlayerPokemon->getName() + " " + QString::number(currentPlayerPokemon->getHp()) + " ";

    // Add the status of the current player's Pokémon
    for (auto status : currentPlayerPokemon->getStatus()) {
        if (status != "NONE") {
            statusMessage += status + " ";
        }
    }

    statusMessage += currentEnemyPokemon->getName() + " " + QString::number(currentEnemyPokemon->getHp()) + " ";

    // Add the status of the current enemy's Pokémon
    for (auto status : currentEnemyPokemon->getStatus()) {
        if (status != "NONE") {
            statusMessage += status + " ";
        }
    }

    this->logList += statusMessage + "\n";
}

// Intent: Print the move names and their power points of the current player's Pokémon.
// Pre: None.
// Post: The move names and power points of the current player's Pokémon are printed.
void GameWindow::printCheck() {
    QString checkMessage = "[Turn " + QString::number(turnCounter) + "] ";

    // Add the move names and power points of the current player's Pokémon
    for (auto& move : currentPlayerPokemon->getMoves()) {
        checkMessage += move.getName() + " " + QString::number(move.getPowerPoint()) + " ";
    }

    this->logList += checkMessage + "\n";
}

// Intent: Handle the attack action in battle.
// Pre: attacker is the attacking Pokémon, usedMove is the move being used, and defender is the defending Pokémon.
// Post: The attack is executed, and the game state is updated accordingly.
void GameWindow::handleAttack(Pokemon& attacker, Move& usedMove, Pokemon& defender) {
    int damage = 0;
    int moveAccuracy = usedMove.getAccuracy();

    // If in test mode, set move accuracy to 100%
    if (isTestMode) {
        moveAccuracy = 100;
    }

    QString attackerStatus = "";
    QString defenderStatus = "opponent ";

    // Modify status strings based on player's turn
    if (!isPlayerTurn) {
        attackerStatus = "The opposing ";
        defenderStatus = "";
    }

    // Check if the attack hits based on move accuracy
    if (1 + (rand() % 100) <= moveAccuracy) {
        QString attackMessage = "[Turn " + QString::number(turnCounter) + "] " + attackerStatus + attacker.getName() + " used " + usedMove.getName() + "!";
        this->logList += attackMessage + "\n";
        damage = std::round(getDamageCalculation(attacker, usedMove, defender));

        // If the damage is negative, return (no damage is done)
        if (damage < 0) {
            return;
        }

        // Reduce PP of the move
        usedMove.setPowerPoint(usedMove.getPowerPoint() - 1);

        int newHealth = defender.getHp() - damage;

        // Update the defender's HP based on the damage taken
        if (newHealth <= 0) {
            defender.setHp(0);
        }
        else {
            defender.setHp(newHealth);
        }
    }
    // Check if the attack missed
    else {
        QString missMessage = "[Turn " + QString::number(turnCounter) + "] " + attackerStatus + attacker.getName() + "'s move missed!";
        this->logList += missMessage + "\n";
    }
}


// Intent: Handle the use of an item in battle.
// Pre: None.
// Post: The selected item is used on the selected Pokémon, and the game state is updated accordingly.
void GameWindow::handleUseItem() {
    // Update the UI state
    this->isWaitingClick = false;
    QString selectedPokemon = this->selectedPokemonCommand;
    bool isPokemonFound = false;

    // Find the selected Pokémon in the player's Pokémon list
    for (auto& pokemon : playerPokemonList) {
        if (pokemon.getName() != selectedPokemon) {
            continue;
        }

        isPokemonFound = true;

        // Check the type of boost provided by the selected item and apply it to the Pokémon's HP if applicable
        if (this->selectedItem->getBoostType() == "HP" && pokemon.getHp() > 0) {
            int newHp = pokemon.getHp() + this->selectedItem->getBoost();

            // Adjust hp to max hp if the hp exceed it
            if (this->selectedItem->getBoost() < 0 || newHp > pokemon.getMaxHp()) {
                newHp = pokemon.getMaxHp();
            }

            // Set new hp to the pokemon
            pokemon.setHp(newHp);

            // Log the message
            QString turnMessage = "[Turn " + QString::number(turnCounter) + "] " + "You used a " + getFormatSpace(selectedItem->getName()) + "!";
            QString hpRestoredMessage = "[Turn " + QString::number(turnCounter) + "] " + selectedPokemon + " had its HP restored.";
            this->logList += turnMessage + "\n";
            this->logList += hpRestoredMessage + "\n";
        }
    }

    // Handle error if the selected Pokémon is not found in the player's party
    if (!isPokemonFound) {
        QString errorMessage = selectedPokemon + " is not available in the trainer's party. Ending battle...";
        this->logList += errorMessage + "\n";
        isErrorCommand = true;
    }

    // Handle attack of the enemy after using item
    handleBattle(true);
}

// Intent: Check if the current player's Pokémon has fainted and handle the situation accordingly.
// Pre: None.
// Post: If the current player's Pokémon has fainted, the game state is updated and the appropriate actions are taken.
void GameWindow::checkHandlePlayerFainted() {
    // If player's Pokemon has HP remaining, refresh the UI and return
    if (currentPlayerPokemon->getHp() > 0) {
        this->refreshUI();
        return;
    }
    else {
        this->updateEnemyPokemonUI();
        this->updatePlayerPokemonUI();
    }

    // Player's Pokemon has fainted
    QString faintedMessage = "[Turn " + QString::number(turnCounter) + "] " + currentPlayerPokemon->getName() + " fainted!";
    this->logList += faintedMessage + "\n";

    // Check if the game has ended
    if (checkGameEnded()) {
        return;
    }

    // Handle the situation when reading commands from a file
    if (this->isReadingFile) {
        // Read the selectedPokemonCommand and perform a Pokemon swap
        iFile >> this->selectedPokemonCommand;
        handleSwap(false);
    }
    // Update the UI and log when waiting for user input
    else {
        // this->updateEnemyPokemonUI();
        // this->updatePlayerPokemonUI();
        this->logLabel->setText(this->logList);
        this->isWaitingClick = true;
        this->updateSwitchMenuUI();
    }
}


// Intent: Check if the current enemy's Pokémon has fainted and handle the situation accordingly.
// Pre: None.
// Post: If the current enemy's Pokémon has fainted, the game state is updated and the appropriate actions are taken.
void GameWindow::checkHandleEnemyFainted() {
    // If enemy's Pokemon has HP remaining
    if (currentEnemyPokemon->getHp() > 0) {
        // Check if waiting for user input or not
        if (!this->isWaitingClick) {
            // If not waiting for input, refresh the UI
            this->refreshUI();
        }
        // If waiting for input, update enemy and player Pokemon UI, and log
        else {
            this->updateEnemyPokemonUI();
            this->updatePlayerPokemonUI();
            this->logLabel->setText(this->logList);
        }

        return;
    }

    // Enemy's Pokemon has fainted
    QString faintedMessage = "[Turn " + QString::number(turnCounter) + "] " + "The opposing " + currentEnemyPokemon->getName() + " fainted!";
    this->logList += faintedMessage + "\n";

    // Check if the game has ended
    if (checkGameEnded()) {
        this->refreshUI();
        return;
    }

    // Find the next available enemy Pokemon with HP remaining
    for (auto& pokemon : enemyPokemonList) {
        // Set the currentEnemyPokemon to the next available Pokemon if the pokemon hp is 0
        if (pokemon.getHp() > 0) {
            currentEnemyPokemon = &pokemon;
            isTurnFinished = true;

            // Check if waiting for user input or not
            if (!this->isWaitingClick) {
                // If not waiting for input, refresh the UI
                this->refreshUI();
            }
            // If waiting for input, update enemy and player Pokemon UI, and log
            else {
                this->updateEnemyPokemonUI();
                this->updatePlayerPokemonUI();
                this->logLabel->setText(this->logList);
            }
            break;
        }
    }
}


// Intent: Check if the game has ended by determining if all player or enemy Pokémon have fainted.
// Pre: None.
// Post: If the game has ended, the appropriate game state variables and messages are updated, and the UI is refreshed.
bool GameWindow::checkGameEnded() {
    bool isAllFainted = true;

    // Check if all player Pokémon have fainted
    for (auto& pokemon : playerPokemonList) {
        if (pokemon.getHp() > 0) {
            isAllFainted = false;
            break;
        }
    }

    // If all player Pokémon have fainted, player loses the game
    if (isAllFainted) {
        isGameOver = true;
        isPlayerWin = false;
        isTurnFinished = true;
        QString loseMessage = "[Turn " + QString::number(turnCounter) + "] " + "You lose";
        this->logList += loseMessage + "\n";

        ui->resultBtn->setText("Go to Pokemon Center");
        this->refreshUI();

        return true;
    }

    isAllFainted = true;

    // Check if all enemy Pokémon have fainted
    for (auto& pokemon : enemyPokemonList) {
        if (pokemon.getHp() > 0) {
            isAllFainted = false;
            break;
        }
    }

    // If all enemy Pokémon have fainted, player wins the game
    if (isAllFainted) {
        isGameOver = true;
        isPlayerWin = true;
        isTurnFinished = true;
        QString winMessage = "[Turn " + QString::number(turnCounter) + "] " + "You win";
        this->logList += winMessage + "\n";

        // Update UI log and button
        ui->resultBtn->setText("Go to Hall of Fame");
        this->logLabel->setText(this->logList);

        return true;
    }

    return false;
}

// Intent: Check and handle the conditional effects on the current player and enemy Pokémon, such as poisoning or burning.
// Pre: None.
// Post: If the Pokémon is affected by a conditional status, their HP is reduced and the appropriate messages are added to the log.
void GameWindow::checkHandleConditional() {
    // Check conditional effects on the player's Pokémon
    for (auto conditional : currentPlayerPokemon->getStatus()) {
        if (conditional == "PSN" || conditional == "BRN") {
            int damage = std::round((float)(currentEnemyPokemon->getMaxHp() / 16));

            // Add messages for the conditional effects
            if (conditional == "PSN") {
                QString poisonMessage = "[Turn " + QString::number(turnCounter) + "] " + currentPlayerPokemon->getName() + " is hurt by its poisoning!";
                this->logList += poisonMessage + "\n";
            }
            else if (conditional == "BRN") {
                QString burnMessage = "[Turn " + QString::number(turnCounter) + "] " + currentPlayerPokemon->getName() + " is hurt by its burn!";
                this->logList += burnMessage + "\n";
            }

            // Reduce the Pokémon's HP and check if it has fainted
            currentPlayerPokemon->setHp(currentPlayerPokemon->getHp() - damage);

            checkHandlePlayerFainted();
        }
    }

    // Check conditional effects on the enemy's Pokémon
    for (auto conditional : currentEnemyPokemon->getStatus()) {
        if (conditional == "PSN" || conditional == "BRN") {
            int damage = std::round((float)(currentEnemyPokemon->getMaxHp() / 16));

            // Add messages for the conditional effects
            if (conditional == "PSN") {
                QString poisonMessage = "[Turn " + QString::number(turnCounter) + "] " + "The opposing " + currentEnemyPokemon->getName() + " is hurt by its poisoning!";
                this->logList += poisonMessage + "\n";
            }
            else if (conditional == "BRN") {
                QString burnMessage = "[Turn " + QString::number(turnCounter) + "] " + "The opposing " + currentEnemyPokemon->getName() + " is hurt by its burn!";
                this->logList += burnMessage + "\n";
            }

            // Reduce the Pokémon's HP and check if it has fainted
            currentEnemyPokemon->setHp(currentEnemyPokemon->getHp() - damage);

            checkHandleEnemyFainted();
        }
    }
}

// Intent: Check and handle the paralyzed condition for a given Pokémon.
// Pre: None.
// Post: If the Pokémon is paralyzed, there is a chance it cannot move, and the appropriate messages are added to the log.
bool GameWindow::checkHandleParalyzed(Pokemon attacker) {
    // Iterate over the attacker's status conditions
    for (auto conditional : attacker.getStatus()) {
        // Check if the attacker is paralyzed
        if (conditional == "PAR") {
            QString turn = "";

            // Determine the appropriate turn message based on player's turn
            if (!isPlayerTurn) {
                turn = "Opposing ";
            }

            // Construct and add the paralyzed message to the log
            QString paralyzedMessage = "[Turn " + QString::number(turnCounter) + "] " + turn + attacker.getName() + " is paralyzed!";
            this->logList += paralyzedMessage + "\n";

            int rate = 25;

            // Adjust the paralysis rate based on test mode
            if (isTestMode) {
                rate = 100;
            }

            // Check if the paralyzed Pokemon can move
            if ((1 + rand() % 100) <= rate) {
                // Construct and add the can't move message to the log
                QString cantMoveMessage = "[Turn " + QString::number(turnCounter) + "] " + "It can't move!";
                this->logList += cantMoveMessage + "\n";
                return true;
            }
        }
    }

    return false;
}


// Intent: Get a random move for the enemy Pokémon.
// Pre: None.
// Post: Returns the name of a valid move for the enemy Pokémon.
QString GameWindow::getEnemyRandomMove() {
    QVector<Move> moves = this->currentEnemyPokemon->getMoves();
    Move selectedMove = moves[rand() % 4];

    // Select a move that has available power points
    while (selectedMove.getPowerPoint() <= 0) {
        selectedMove = moves[rand() % 4];
    }

    return selectedMove.getName();
}


// Intent: Calculate the damage inflicted by a move used by an attacker on a defender.
// Pre: The attacker, usedMove, and defender must be valid Pokémon objects.
// Post: Returns the damage value calculated based on the attacker's move, attacker's stats, defender's stats, and type effectiveness.
int GameWindow::getDamageCalculation(Pokemon& attacker, Move& usedMove, Pokemon& defender) {
    // ============================= TYPE EFFECTIVENESS AND STAB =============================
    // Initialize damage multiplier and STAB value
    float damageMultiplier = 1.0;
    float STABValue = 1.0;

    // Get the index of the attacking element in the effectiveness chart
    int attackingElementIndex = getElementTypingIndex(usedMove.getType());

    // Get the defender's and attacker's types
    QVector<QString> defenderElements = defender.getType();
    QVector<QString> attackerElements = attacker.getType();

    // Calculate the damage multiplier based on the type effectiveness against the defender's types
    for (const auto& defenderElement : defenderElements) {
        // Get integer index of the defender type
        int defenderElementIndex = getElementTypingIndex(defenderElement);

        // Get the effectiveness from the effectiveness chart vector
        float effectiveness = effectivenessChart[attackingElementIndex][defenderElementIndex];

        // Multiply the effectiveness
        damageMultiplier *= effectiveness;
    }

    // Check if the move is of the same type as any of the attacker's types for the STAB bonus
    for (const auto& attackerElement : attackerElements) {
        // If the type matches, set STAB to 1.5
        if (usedMove.getType() == attackerElement) {
            STABValue = 1.5;
            break;
        }
    }

    // ============================= DAMAGE CALCULATION =============================
    // Initialize damage value, attack power, attack value, defense value, and critical value
    float damageValue = 0;
    int attackPower = usedMove.getPower();
    float attackValue = 0;
    float defenseValue = 0;
    float criticalValue = 1.0;

    // Check for critical hit chance (12.5% chance for critical hit)
    if (!isTestMode) {
        int chance = rand() % 100 + 1;
        criticalValue = (chance <= 12) ? 1.5 : 1.0;
    }

    // Display sound and messages based on the damage multiplier
    if (attackPower > 0) {
        // Play super effective sound and add message to log if it is supper effective
        if (damageMultiplier > 1.0) {
            this->music->setMedia(QUrl("qrc:/audio/audio/super_effective.wav"));
            this->music->play();
            QString superEffectiveMessage = "[Turn " + QString::number(turnCounter) + "] " + "It's super effective!";
            this->logList += superEffectiveMessage + "\n";
        }
        // Play not very effective sound and add message to log if it's not very effective
        else if (damageMultiplier < 1.0) {
            this->music->setMedia(QUrl("qrc:/audio/audio/notvery_effective.mpeg"));
            this->music->play();
            QString notVeryEffectiveMessage = "[Turn " + QString::number(turnCounter) + "] " + "It's not very effective...";
            this->logList += notVeryEffectiveMessage + "\n";
        }
        // Play effective sound, if it's effective
        else {
            this->music->setMedia(QUrl("qrc:/audio/audio/effective.mpeg"));
            this->music->play();
        }
    }

    // Apply move effects on the defender
    QString effect = usedMove.getEffect();

    if (effect != "NONE") {
        // Check if the defender already has the same effect
        bool isSameEffect = false;

        // Check whether there is same status for the defender
        for (auto& status : defender.getStatus()) {
            if (status == effect) {
                isSameEffect = true;
                break;
            }
        }

        // If the defender doesn't have the same effect, add it to the status
        if (!isSameEffect) {
            QVector<QString> currentStatus = defender.getStatus();
            currentStatus.push_back(effect);

            // Set the new effect to the defender
            defender.setStatus(currentStatus);
        }

        // Construct effect message based on the effect type and add it to the log
        QString turn = "";
        QString status = "";

        // If it is player turn, set turn to "The opposing "
        if (isPlayerTurn) {
            turn = "The opposing ";
        }

        // If the pokemon is paralyzed, print the following message
        if (effect == "PAR") {
            status = " is paralyzed, so it may be unable to move!";
        }
        // If the pokemon is burned, print the following message
        else if (effect == "BRN") {
            status = " was burned!";
        }
        // If the pokemon is poisoned, print the following message
        else if (effect == "PSN") {
            status = " was poisoned!";
        }

        // Update log
        QString effectMessage = "[Turn " + QString::number(turnCounter) + "] " + turn + defender.getName() + status;
        this->logList += effectMessage + "\n";
    }

    // Calculate attack and defense values
    if (usedMove.getAttackType() == "Physical") {
        attackValue = attacker.getAttack();
        defenseValue = defender.getDefense();
    }
    // Calculate special attack and special defense values
    else if (usedMove.getAttackType() == "Special") {
        attackValue = attacker.getSpAttack();
        defenseValue = defender.getSpDefense();
    }
    // If status attack, return -1 as an indicator for no damage
    else {
        usedMove.setPowerPoint(usedMove.getPowerPoint() - 1);
        return -1.0;
    }

    // Calculate the damage value using the damage formula
    float step1 = (2.0 * 50.0) + 10.0;
    float step2 = step1 / 250.0;
    float step3 = step2 * attackPower;
    float step4 = step3 * (attackValue / defenseValue);
    float step5 = step4 + 2.0;
    float step6 = step5 * criticalValue;
    float step7 = step6 * STABValue;
    damageValue = (int)(step7 * damageMultiplier);

    return damageValue;
}


// Intent: Format a string by inserting spaces before capital letters (except the first letter).
// Pre: The string must be a valid string.
// Post: Returns the formatted string with spaces before capital letters.
QString GameWindow::getFormatSpace(QString string) {
    QString result;

    // Keep the first letter as is
    result += string[0];

    // Insert a space before capital letters (except the first letter)
    for (int i = 1; i < string.length(); ++i) {
        if (string[i].isUpper()) {
            result += ' ';
        }
        result += string[i];
    }

    return result;
}

// Intent: Get the index of an element's typing in the effectiveness chart.
// Pre: The type must be a valid string representing a Pokémon type.
// Post: Returns the index of the element's typing in the effectiveness chart.
int GameWindow::getElementTypingIndex(QString type) {
    int index = 0;

    // Set index to 0, if the type is normal
    if (type == "Normal") {
        index = 0;
    }
    // Set index to 1, if the type is fire
    else if (type == "Fire") {
        index = 1;
    }
    // Set index to 2, if the type is water
    else if (type == "Water") {
        index = 2;
    }
    // Set index to 3, if the type is electric
    else if (type == "Electric") {
        index = 3;
    }
    // Set index to 4, if the type is grass
    else if (type == "Grass") {
        index = 4;
    }
    // Set index to 5, if the type is ice
    else if (type == "Ice") {
        index = 5;
    }
    // Set index to 6, if the type is fighting
    else if (type == "Fighting") {
        index = 6;
    }
    // Set index to 7, if the type is poison
    else if (type == "Poison") {
        index = 7;
    }
    // Set index to 8, if the type is ground
    else if (type == "Ground") {
        index = 8;
    }
    // Set index to 9, if the type is flying
    else if (type == "Flying") {
        index = 9;
    }
    // Set index to 10, if the type is psychic
    else if (type == "Psychic") {
        index = 10;
    }
    // Set index to 11, if the type is bug
    else if (type == "Bug") {
        index = 11;
    }
    // Set index to 12, if the type is rock
    else if (type == "Rock") {
        index = 12;
    }
    // Set index to 13, if the type is ghost
    else if (type == "Ghost") {
        index = 13;
    }
    // Set index to 14, if the type is dragon
    else if (type == "Dragon") {
        index = 14;
    }
    // Set index to 15, if the type is dark
    else if (type == "Dark") {
        index = 15;
    }
    // Set index to 16, if the type is steel
    else if (type == "Steel") {
        index = 16;
    }
    // Set index to 17, if the type is fairy
    else if (type == "Fairy") {
        index = 17;
    }

    return index;
}

// Intent: Get the result of the game.
// Pre: None.
// Post: Returns true if the player wins the game, false otherwise.
bool GameWindow::getResult() {
    return this->isPlayerWin;
}

// Intent: Set the enemy's information.
// Pre: The name and imgUrl must be valid strings.
// Post: The enemy's name and trainer image are updated in the game window.
void GameWindow::setEnemyInfo(QString name, QString imgUrl) {
    this->trainerName = name;

    // Set font
    QFont font;
    font.setPointSize(10);

    // If name is to long, then make the font size smaller
    if (name.length() > 10) {
        font.setPointSize(8);
    }

    // Update UI
    ui->enemy_name->setFont(font);
    ui->enemy_name->setText(name);
    ui->enemy_trainer->setPixmap(imgUrl);
}
