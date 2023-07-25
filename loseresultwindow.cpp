/***********************************************************************
 * File: loseresultwindow.cpp
 * Author:	鍾詩靈 B11115010
 *			陳仕興 B11115011
 *			魏美芳 B11115014
 * Create Date: 2023-05-26
 * Editor:	鍾詩靈 B11115010
 *			陳仕興 B11115011
 *			魏美芳 B11115014
 * Update Date: 2023-06-05
 * Description: This C++ program define the LoseResultWindow class
***********************************************************************/
#include "loseresultwindow.h"
#include "ui_loseresultwindow.h"

// Intent: Initialize the LoseResultWindow.
// Pre: None.
// Post: LoseResultWindow is initialized and set up.
LoseResultWindow::LoseResultWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoseResultWindow)
{
    ui->setupUi(this);

    // Flip the Chansey image horizontally
    QPixmap chanseyPixmap = ui->chansey->pixmap()->transformed(QTransform().scale(-1, 1));
    ui->chansey->setPixmap(chanseyPixmap);

    // Flip the Blissey image horizontally
    QPixmap blisseyPixmap = ui->blissey->pixmap()->transformed(QTransform().scale(-1, 1));
    ui->blissey->setPixmap(blisseyPixmap);
}

// Intent: Clean up and destroy the LoseResultWindow.
// Pre: None.
// Post: LoseResultWindow is destroyed.
LoseResultWindow::~LoseResultWindow()
{
    delete ui;
}

// Intent: Handle the button click event to go back to the main menu.
// Pre: None.
// Post: Signal "BackToMenuClicked" is emitted.
void LoseResultWindow::on_menuBtn_clicked()
{
    emit BackToMenuClicked();
}

// Intent: Set the images for the defeated Pokemons.
// Pre: A list of URLs for the images is provided.
// Post: Images for the defeated Pokemons are set.
void LoseResultWindow::setPkmnImages(QVector<QString> urlList)
{
    for (int i = 0; i < 3; i++) {
        // Construct the object name of the QLabel
        QString labelObjectName = "pkmn_" + QString::number(i);

        // Retrieve the QLabel object using findChild() function
        QLabel* imgLabel = findChild<QLabel*>(labelObjectName);

        // Set the pixmap of the QLabel to the corresponding URL from urlList
        imgLabel->setPixmap(urlList[i]);
    }

}
