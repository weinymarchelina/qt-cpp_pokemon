/***********************************************************************
 * File: winresultwindow.cpp
 * Author:	鍾詩靈 B11115010
 *			陳仕興 B11115011
 *			魏美芳 B11115014
 * Create Date: 2023-05-26
 * Editor:	鍾詩靈 B11115010
 *			陳仕興 B11115011
 *			魏美芳 B11115014
 * Update Date: 2023-06-05
 * Description: This C++ program define the WinResultWindow class
***********************************************************************/
#include "winresultwindow.h"
#include "ui_winresultwindow.h"

// Intent: Initialize the WinResultWindow object
// Pre: 'parent' is a valid QWidget pointer
// Post: WinResultWindow object is created and UI is set up
WinResultWindow::WinResultWindow(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WinResultWindow)
{
    ui->setupUi(this);
}

// Intent: Destructor for WinResultWindow object
// Pre: None
// Post: WinResultWindow object is destroyed, memory is freed
WinResultWindow::~WinResultWindow()
{
    delete ui;
}

// Intent: Handle click event on the menu button
// Pre: None
// Post: Emit the BackToMenuClicked signal
void WinResultWindow::on_menuBtn_clicked()
{
    emit BackToMenuClicked();
}

// Intent: Set the Pokemon images in the WinResultWindow
// Pre: 'urlList' is a QVector of QStrings containing valid URLs for the images
// Post: Pokémon images are set in the UI based on the URLs provided
void WinResultWindow::setPkmnImages(QVector<QString> urlList)
{
    // Iterate over the first 3 elements of the urlList
    for (int i = 0; i < 3; i++)
    {
        // Find the QLabel widget with the object name "pkmn_" + i and retrieve its pointer
        QLabel* imgLabel = findChild<QLabel*>("pkmn_" + QString::number(i));

        // Set the pixmap of the imgLabel to the corresponding URL from urlList
        imgLabel->setPixmap(urlList[i]);
    }
}


