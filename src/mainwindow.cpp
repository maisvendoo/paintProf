#include    "mainwindow.h"
#include    "ui_mainwindow.h"

#include    <QDir>
#include    <QAction>
#include    <QFileDialog>
#include    <QFileInfo>
#include    <QGridLayout>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , profile(Q_NULLPTR)
  , settings(new QSettings("maisvendoo", "paintProfile", Q_NULLPTR))
  , openPath("./")
  , savePath("./")
{
    ui->setupUi(this);

    openPath = settings->value("openPath", QDir::homePath()).toString();
    savePath = settings->value("savePath", QDir::homePath()).toString();

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openProfile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveProfile);

    profile = new Profile(ui->profileFrame->width(), ui->profileFrame->height());

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(profile);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    ui->profileFrame->setLayout(layout);

    connect(ui->dsbBeginCoord, QOverload<double>::of(&QDoubleSpinBox::valueChanged), profile, &Profile::setBeginCoord);

    connect(ui->sbStartKm, QOverload<int>::of(&QSpinBox::valueChanged), profile, &Profile::setInitKm);
    profile->setInitKm(ui->sbStartKm->value());

    setCentralWidget(ui->widget);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::paintEvent(QPaintEvent *)
{
    ui->profileFrame->update();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::openProfile()
{
    profile->clean();

    QString path = QFileDialog::getOpenFileName(Q_NULLPTR,
                                                tr("Open profile file"),
                                                openPath,
                                                tr("Profile text file (*.txt, *.conf"));

    if (path.isEmpty())
        return;

    openPath = QFileInfo(path).path();
    settings->setValue("openPath", openPath);

    profile->load(path);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::saveProfile()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::quit()
{
    QApplication::quit();
}
