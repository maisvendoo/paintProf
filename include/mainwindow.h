#ifndef     MAINWINDOW_H
#define     MAINWINDOW_H

#include    <QMainWindow>
#include    <QSettings>

#include    "profile.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
namespace Ui
{
    class MainWindow;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:

    Ui::MainWindow  *ui;
    Profile         *profile;
    QSettings       *settings;

    QString         openPath;
    QString         savePath;

    void paintEvent(QPaintEvent *);

private slots:

    void openProfile();

    void saveProfile();

    void quit();
};

#endif // MAINWINDOW_H
