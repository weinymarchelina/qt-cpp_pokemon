/***********************************************************************
 * File: menuwindow.cpp
 * Author:	鍾詩靈 B11115010
 *			陳仕興 B11115011
 *			魏美芳 B11115014
 * Create Date: 2023-05-26
 * Editor:	鍾詩靈 B11115010
 *			陳仕興 B11115011
 *			魏美芳 B11115014
 * Update Date: 2023-06-05
 * Description: Source file for the MenuWindow class.
***********************************************************************/
#include "menuwindow.h"
#include "ui_menuwindow.h"

// Intent: Initialize the MenuWindow
// Pre: Parent QWidget must be provided
// Post: MenuWindow object is created and set up with UI components and initial values
MenuWindow::MenuWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    // Set game name
    setWindowTitle("Pokemon: Super Battle Pokemon Brawl");

    // Reset case file path
    this->caseFilePath = "";

    // Insert the GameWindowPage and ResultWindowPage into the stacked widget
    ui->stackedWidget->insertWidget(1, &_GameWindowPage);
    ui->stackedWidget->insertWidget(2, &_WinResultWindowPage);
    ui->stackedWidget->insertWidget(3, &_LoseResultWindowPage);

    // Connect signals and slots
    connect(&_WinResultWindowPage, SIGNAL(BackToMenuClicked()), this, SLOT(moveToMenu()));
    connect(&_LoseResultWindowPage, SIGNAL(BackToMenuClicked()), this, SLOT(moveToMenu()));
    connect(&_GameWindowPage, SIGNAL(ShowResultClicked()), this, SLOT(moveToResult()));

    // Reset enemy to 0 and its UI
    this->enemyIndex = 0;
    ui->enemy->setPixmap(":/img/img/trainer_" + QString::number(this->enemyIndex) + ".png");
    ui->enemyLabel->setText("Opponent : " + this->enemyName[enemyIndex]);
    ui->pkmn_enemy->setPixmap(":/sprites/sprites/" + this->enemyPkmn[enemyIndex] + ".png");

    // Set music to base music
    this->music->setMedia(QUrl("qrc:/audio/audio/trainer_0.mp3"));
    this->music->play();
    this->music->setVolume(30);

    // Connect to mediaStatusChanged signal
    connect(this->music, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
        // Check whether the music has already finished being play
        if (status == QMediaPlayer::EndOfMedia)
        {
            // Restart the music when it has finished playing
            this->music->play();
        }
    });

    // Get the current directory path for default game mode
    QString currentPath = QDir::currentPath() += "/";

    // If there is no case file, then set default game mod esetting of the battle based on the enemy
    if (this->caseFilePath == "") {
        this->pokedex.clear();
        this->handlePokemonFile(currentPath + "custom_data/PokemonLibFull.txt");
        this->moveCodex.clear();
        this->handleMoveFile(currentPath + "custom_data/MoveLibFull.txt");
        this->trainerList.clear();
        this->handleGameDataFile(currentPath + "custom_data/GameData" + this->enemyName[this->enemyIndex] + ".txt");
    }
}

// Intent: Clean up the MenuWindow
// Pre: None
// Post: MenuWindow object is deleted
MenuWindow::~MenuWindow()
{
    delete ui;
}

// Intent: Move back to the main menu
// Pre: None
// Post: Game state is reset, enemy index is set to 0, and the UI is updated accordingly
void MenuWindow::moveToMenu() {
    // Reset the menu properties
    this->handleResetFile();

    // Get the current directory path for default game mode
    QString currentPath = QDir::currentPath() += "/";

    // Reset enemy to 0 and its UI
    this->enemyIndex = 0;
    ui->enemy->setPixmap(":/img/img/trainer_" + QString::number(this->enemyIndex) + ".png");
    ui->enemyLabel->setText("Opponent : " + this->enemyName[enemyIndex]);
    ui->pkmn_enemy->setPixmap(":/sprites/sprites/" + this->enemyPkmn[enemyIndex] + ".png");

    // If there is no case file, then set default game mod esetting of the battle based on the enemy
    if (this->caseFilePath == "") {
        this->pokedex.clear();
        this->handlePokemonFile(currentPath + "custom_data/PokemonLibFull.txt");
        this->moveCodex.clear();
        this->handleMoveFile(currentPath + "custom_data/MoveLibFull.txt");
        this->trainerList.clear();
        this->handleGameDataFile(currentPath + "custom_data/GameData" + this->enemyName[this->enemyIndex] + ".txt");
    }

    // Reset enemy audio
    this->music->setMedia(QUrl("qrc:/audio/audio/trainer_" + QString::number(this->enemyIndex) + ".mp3"));
    this->music->play();

    // Return to menu page
    ui->stackedWidget->setCurrentIndex(0);
}

// Intent: Move to the game screen
// Pre: None
// Post: Game screen is displayed
void MenuWindow::moveToGame() {
    ui->stackedWidget->setCurrentIndex(1);
}

// Intent: Move to the result screen
// Pre: None
// Post: Result screen is displayed based on the game outcome
void MenuWindow::moveToResult() {
    // Get the result of the game (win or lose)
    bool isWin = _GameWindowPage.getResult();

    // Initialize a list to store the URLs of the player's Pokémon images
    QVector<QString> urlList;

    // Iterate over the Pokémon in the player's team
    for (auto& pokemon : this->trainerList[0].getPokemon()) {
        // Get the name of the Pokémon and construct the URL for its image
        QString playerPokemonName = pokemon.getName();
        QString playerImgUrl = ":/sprites/sprites/" + playerPokemonName.toLower() + ".png";

        // Check if the image file exists
        QFile imageFile(playerImgUrl);
        if (!imageFile.exists()) {
            // If the image file does not exist, display a debug message and use a substitute image
            qDebug() << "Image file does not exist: " << playerImgUrl;
            playerImgUrl = ":/sprites/img/subs.png";
        }

        // Add the URL to the list
        urlList.push_back(playerImgUrl);
    }

    // Check if the player won the game
    if (isWin) {
        // Set the win theme music and play it
        this->music->setMedia(QUrl("qrc:/audio/audio/win_theme.mp3"));
        this->music->play();

        // Set the Pokémon images in the win result window
        _WinResultWindowPage.setPkmnImages(urlList);

        // Switch to the win result window in the stacked widget
        ui->stackedWidget->setCurrentIndex(2);
    }
    else {
        // Create a QMediaPlayer object for the healing sound
        QMediaPlayer* heal = new QMediaPlayer();
        heal->setMedia(QUrl("qrc:/audio/audio/heal.mp3"));
        heal->play();

        // Set the lose theme music and play it
        this->music->setMedia(QUrl("qrc:/audio/audio/lose_theme.mp3"));
        this->music->play();

        // Set the Pokémon images in the lose result window
        _LoseResultWindowPage.setPkmnImages(urlList);

        // Switch to the lose result window in the stacked widget
        ui->stackedWidget->setCurrentIndex(3);
    }
}


// Intent: Handle the click event of the play button
// Pre: None
// Post: Move to the game window and initialize the game
void MenuWindow::on_playBtn_clicked()
{
    // Check if all needed files are inputted
    if (this->pokedex.empty() && this->moveCodex.empty() && this->trainerList.empty())
    {
        return;
    }

    // Set the enemy image URL based on the enemy index
    QString enemyImgUrl = ":/img/img/trainer_" + QString::number(this->enemyIndex) + ".png";

    // Set the enemy information in the game window
    _GameWindowPage.setEnemyInfo(enemyName[this->enemyIndex], enemyImgUrl);

    // Initialize the game in the game window
    _GameWindowPage.initializeGame(this->isTestMode, this->caseFilePath, this->trainerList);

    // Move to the game window
    this->moveToGame();
}

// Intent: Handle the reading and processing of the Pokemon file
// Pre: The file path is valid
// Post: The Pokemon data is stored in the pokedex vector
void MenuWindow::handlePokemonFile(QString filePath) {
    // Open the file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        // If the file fails to open, return from the function
        return;
    }

    QTextStream iFile(&file);

    QString pokemonName;

    // Read the contents of the file line by line
    while (!iFile.atEnd()) {
        // Read the name of the Pokémon
        iFile >> pokemonName;

        int typeAmount, hp, atk, def, spAtk, spDef, speed;
        QVector<QString> type;

        // Read the number of types for the Pokémon
        iFile >> typeAmount;

        // Read each type and store them in the type vector
        for (int i = 0; i < typeAmount; i++) {
            QString typeTemp;
            iFile >> typeTemp;

            type.push_back(typeTemp);
        }

        // Read the HP, attack, defense, special attack, special defense, and speed of the Pokémon
        iFile >> hp >> atk >> def >> spAtk >> spDef >> speed;

        qDebug() << pokemonName;

        // Create a new Pokémon object with the read data
        Pokemon newPokemon = Pokemon(pokemonName, type, hp, atk, spAtk, def, spDef, speed);

        // Add the new Pokémon to the pokedex
        this->pokedex.push_back(newPokemon);
    }

    // Close the file
    file.close();

    // Set the text of the pkmnLibEdit field with the filename from the file path
    ui->pkmnLibEdit->setText(QFileInfo(filePath).fileName());
}


// Intent: Handle the reading and processing of the Move file
// Pre: The file path is valid
// Post: The Move data is stored in the moveCodex vector
void MenuWindow::handleMoveFile(QString filePath) {
    // Open the file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        // If the file fails to open, return from the function
        return;
    }

    QTextStream in(&file);

    // Read move name
    QString moveName;
    in >> moveName;

    int count = 0;

    // Read the contents of the file line by line
    while (!in.atEnd()) {
        QString type, atkType, effect;
        int power, accuracy, powerPoint;

        // Read the move data from the file
        in >> type >> atkType >> power >> accuracy >> powerPoint >> effect;

        // Check if the move has a special effect
        bool hasEffect = (effect == "PAR" || effect == "BRN" || effect == "PSN") ? true : false;

        // Create a new Move object with the read data and add it to the moveCodex vector
        if (hasEffect) {
            Move newMove = Move(moveName, type, atkType, power, accuracy, powerPoint, effect);
            this->moveCodex.push_back(newMove);
            in >> moveName;
        }
        else {
            // Create a new Move object with "NONE" effect and add it to the moveCodex vector
            Move newMove = Move(moveName, type, atkType, power, accuracy, powerPoint, "NONE");
            this->moveCodex.push_back(newMove);
            moveName = effect;
        }
        count++;
    }

    // Close the file
    file.close();

    // Set the text of the moveLibEdit field with the filename from the file path
    ui->moveLibEdit->setText(QFileInfo(filePath).fileName());
}


// Intent: Read and process game data file
// Pre: The file path of the game data file is provided
// Post: The game data is parsed and stored in the appropriate data structures
void MenuWindow::handleGameDataFile(QString filePath) {
    // Open the file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        // If the file fails to open, return from the function
        return;
    }

    // Set file to read on in QTextStream
    QTextStream in(&file);
    int amount;

    // Read the contents of the file line by line
    while (!in.atEnd()) {
        in >> amount;

        QVector<Pokemon> pokemonNotes;

        // Read the specified number of Pokemon data entries
        for (int i = 0; i < amount; i++) {
            QString name;
            QVector<Move> move;
            int moveAmount;
            in >> name >> moveAmount;

            // Read all the Pokemon's moves
            for (int j = 0; j < moveAmount; j++) {
                QString moveName;
                in >> moveName;

                // Search the move in the moveCodex
                for (int k = 0; k < moveCodex.size(); k++) {
                    // If the move is in the codex, add it to the move vector
                    if (moveName == moveCodex[k].getName()) {
                        move.push_back(moveCodex[k]);
                        break;
                    }
                }
            }

            // Search for the given pokemon in the pokedex
            for (int j = 0; j < pokedex.size(); j++) {
                // If the pokemon is found in the pokedex, append the pokemon to pokemonNotes and add the moves to it
                if (pokedex[j].getName() == name) {
                    Pokemon pokemon = pokedex[j];
                    pokemon.setMoves(move);
                    pokemonNotes.push_back(pokemon);
                    break;
                }
                // If the pokemon is not found in the pokedex, it is ignored
            }
        }

        // Create a vector to store item data
        QVector<Item> itemList;

        // Add some default items to the item list
        itemList.push_back(Item("Potion", 20, "HP"));
        itemList.push_back(Item("SuperPotion", 60, "HP"));
        itemList.push_back(Item("HyperPotion", 120, "HP"));
        itemList.push_back(Item("MaxPotion", -1, "HP"));

        // Create a new trainer with the parsed data and add it to the trainerList
        this->trainerList.push_back(Trainer(pokemonNotes, itemList));
    }

    // Close the file
    file.close();

    // Set the text of the gameDataEdit field with the filename extracted from the file path
    ui->gameDataEdit->setText(QFileInfo(filePath).fileName());
}


// Intent: Handle loading a case file and populating the data structures with the file contents.
// Pre: None.
// Post: Case file is loaded and data structures (pokedex, moveCodex, trainerList) are populated.
void MenuWindow::handleCaseFile() {
    // Clear existing data structures
    this->pokedex.clear();
    this->moveCodex.clear();
    this->trainerList.clear();

    // Open a file dialog to allow the user to select a text file
    QString filePath = QFileDialog::getOpenFileName(this, "Open the file", QString(), "Text Files (*.txt)");

    // Get the directory path of the selected file
    QString currentPath = QFileInfo(filePath).dir().path() += "/";

    // Open the file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        // If the file fails to open, show a warning message box and return from the function
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }

    QTextStream iFile(&file);

    QVector<QString> fileList;

    // Read the first three lines in the file (Pokemons file, Moves files, Game Data files)
    for (int i = 0; i < 3; i++) {
        QString fileName;
        iFile >> fileName;

        QString fullPath = currentPath + fileName;
        fileList.push_back(fullPath);
    }

    QString command;

    iFile >> command;

    // Check if the command is "Test"
    if (command == "Test") {
        // Set test mode flag and update UI label
        this->isTestMode = true;
        ui->testLabel->setText("Test Mode On");
    }

    // Process the Pokemon file
    handlePokemonFile(fileList[0]);

    // Process the Move file
    handleMoveFile(fileList[1]);

    // Process the Game Data file
    handleGameDataFile(fileList[2]);

    // Disable the file selection buttons
    ui->pkmnLibBtn->setDisabled(true);
    ui->moveLibBtn->setDisabled(true);
    ui->gameDataBtn->setDisabled(true);

    // Store the selected case file path
    this->caseFilePath = filePath;

    // Update UI label
    ui->caseLabel->setText("Case File Input");

    // Close the file
    file.close();
}

// Intent: Reset the file selections and clear the data structures.
// Pre: None.
// Post: File selections are reset and data structures (pokedex, moveCodex, trainerList) are cleared.
void MenuWindow::handleResetFile()
{
    // Clear existing data structures
    this->pokedex.clear();
    this->moveCodex.clear();
    this->trainerList.clear();

    // Reset file selection UI elements
    ui->pkmnLibEdit->setText("");
    ui->pkmnLibBtn->setDisabled(false);

    ui->moveLibEdit->setText("");
    ui->moveLibBtn->setDisabled(false);

    ui->gameDataEdit->setText("");
    ui->gameDataBtn->setDisabled(false);

    // Reset UI labels and flags
    ui->caseLabel->setText("Manual Input");
    ui->testLabel->setText("Test Mode Off");

    this->caseFilePath = "";
    this->isTestMode = false;
}


// Intent: Handle the button click event for selecting a case file.
// Pre: None.
// Post: Case file is handled.
void MenuWindow::on_caseBtn_clicked()
{
    this->handleCaseFile();
}

// Intent: Handle the button click event for resetting the file selections.
// Pre: None.
// Post: File selections are reset.
void MenuWindow::on_resetBtn_clicked()
{
    this->handleResetFile();
}

// Intent: Handle the button click event for selecting a Pokemon library file.
// Pre: None.
// Post: Pokemon library file is handled.
void MenuWindow::on_pkmnLibBtn_clicked()
{
    this->pokedex.clear();
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file", QString(), "Text Files (*.txt)");
    this->handlePokemonFile(fileName);
}

// Intent: Handle the button click event for selecting a Move library file.
// Pre: None.
// Post: Move library file is handled.
void MenuWindow::on_moveLibBtn_clicked()
{
    this->moveCodex.clear();
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file", QString(), "Text Files (*.txt)");
    this->handleMoveFile(fileName);
}

// Intent: Handle the button click event for selecting a game data file.
// Pre: None.
// Post: Game data file is handled.
void MenuWindow::on_gameDataBtn_clicked()
{
    this->trainerList.clear();
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file", QString(), "Text Files (*.txt)");
    this->handleGameDataFile(fileName);
}

// Intent: Handle the button click event for toggling test mode.
// Pre: None.
// Post: Test mode is toggled.
void MenuWindow::on_testBtn_clicked()
{
    // If a case file is selected, return from the function
    if (this->caseFilePath != "") {
        return;
    }

    // Toggle test mode
    if (this->isTestMode) {
        ui->testLabel->setText("Test Mode Off");
    }
    else {
        ui->testLabel->setText("Test Mode On");
    }

    this->isTestMode = !this->isTestMode;
}

// Intent: Handle the button click event for switching to the previous opponent.
// Pre: None.
// Post: The previous opponent is displayed.
void MenuWindow::on_leftBtn_clicked()
{
    // Decrement the enemy index
    enemyIndex--;

    // If the enemy index becomes negative, wrap around to the last opponent
    if (this->enemyIndex < 0) {
        enemyIndex = 9;
    }

    // Update UI elements with the new opponent data
    ui->enemy->setPixmap(":/img/img/trainer_" + QString::number(this->enemyIndex) + ".png");
    ui->enemyLabel->setText("Opponent : " + this->enemyName[enemyIndex]);
    ui->pkmn_enemy->setPixmap(":/sprites/sprites/" + this->enemyPkmn[enemyIndex] + ".png");

    // Play the corresponding audio for the opponent
    this->music->setMedia(QUrl("qrc:/audio/audio/trainer_" + QString::number(this->enemyIndex) + ".mp3"));
    this->music->play();

    // Get the current directory path for default game mode
    QString currentPath = QDir::currentPath() += "/";

    // If there is no case file, then set default game mod esetting of the battle based on the enemy
    if (this->caseFilePath == "") {
        this->pokedex.clear();
        this->handlePokemonFile(currentPath + "custom_data/PokemonLibFull.txt");
        this->moveCodex.clear();
        this->handleMoveFile(currentPath + "custom_data/MoveLibFull.txt");
        this->trainerList.clear();
        this->handleGameDataFile(currentPath + "custom_data/GameData" + this->enemyName[this->enemyIndex] + ".txt");
    }
}

// Intent: Handle the button click event for switching to the next opponent.
// Pre: None.
// Post: The next opponent is displayed.
void MenuWindow::on_rightBtn_clicked()
{
    // Increment the enemy index
    enemyIndex++;

    // If the enemy index exceeds the maximum number of opponents, wrap around to the first opponent
    if (this->enemyIndex > 9) {
        enemyIndex = 0;
    }

    // Update UI elements with the new opponent data
    ui->enemy->setPixmap(":/img/img/trainer_" + QString::number(this->enemyIndex) + ".png");
    ui->enemyLabel->setText("Opponent : " + this->enemyName[enemyIndex]);
    ui->pkmn_enemy->setPixmap(":/sprites/sprites/" + this->enemyPkmn[enemyIndex] + ".png");

    // Play the corresponding audio for the opponent
    this->music->setMedia(QUrl("qrc:/audio/audio/trainer_" + QString::number(this->enemyIndex) + ".mp3"));
    this->music->play();

    // Get the current directory path for default game mode
    QString currentPath = QDir::currentPath() += "/";

    // If there is no case file, then set default game mod esetting of the battle based on the enemy
    if (this->caseFilePath == "") {
        this->pokedex.clear();
        this->handlePokemonFile(currentPath + "custom_data/PokemonLibFull.txt");
        this->moveCodex.clear();
        this->handleMoveFile(currentPath + "custom_data/MoveLibFull.txt");
        this->trainerList.clear();
        this->handleGameDataFile(currentPath + "custom_data/GameData" + this->enemyName[this->enemyIndex] + ".txt");
    }
}


