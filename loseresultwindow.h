#ifndef LOSERESULTWINDOW_H
#define LOSERESULTWINDOW_H

#include <QWidget>

namespace Ui {
class LoseResultWindow;
}

class LoseResultWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoseResultWindow(QWidget *parent = nullptr);
    ~LoseResultWindow();
    void setPkmnImages(QVector<QString> urlList);

private slots:
    void on_menuBtn_clicked();

signals:
    void BackToMenuClicked();

private:
    Ui::LoseResultWindow *ui;
};

#endif // LOSERESULTWINDOW_H
