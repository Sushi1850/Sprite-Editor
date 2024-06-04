#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
    delete mainWindow;
    delete model;
}

void MainMenu::on_pixel8_pressed()
{
    model = new Model(nullptr, 8);
    mainWindow = new MainWindow(*model);
    mainWindow->show();
    this->close();
}


void MainMenu::on_pixel16_pressed()
{
    model = new Model(nullptr, 16);
    mainWindow = new MainWindow(*model);
    mainWindow->show();
    this->close();
}

void MainMenu::on_pixel32_pressed()
{
    model = new Model(nullptr, 32);
    mainWindow = new MainWindow(*model);
    mainWindow->show();
    this->close();
}

void MainMenu::on_pixel64_pressed()
{
    model = new Model(nullptr, 64);
    mainWindow = new MainWindow(*model);
    mainWindow->show();
    this->close();
}

void MainMenu::on_pixel128_pressed()
{
    model = new Model(nullptr, 128);
    mainWindow = new MainWindow(*model);
    mainWindow->show();
    this->close();
}

void MainMenu::on_pixel256_pressed()
{
    model = new Model(nullptr, 256);
    mainWindow = new MainWindow(*model);
    mainWindow->show();
    this->close();
}
