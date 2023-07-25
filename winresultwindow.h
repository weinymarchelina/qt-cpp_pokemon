#ifndef WINRESULTWINDOW_H
#define WINRESULTWINDOW_H

#include <QWidget>
#include <QVector>
#include <QString>

namespace Ui {
class WinResultWindow;
}

class WinResultWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WinResultWindow(QWidget *parent = nullptr);
    ~WinResultWindow();
    void setPkmnImages(QVector<QString> urlList);

private slots:
    void on_menuBtn_clicked();

signals:
    void BackToMenuClicked();

private:
    Ui::WinResultWindow *ui;
};

#endif // WINRESULTWINDOW_H
