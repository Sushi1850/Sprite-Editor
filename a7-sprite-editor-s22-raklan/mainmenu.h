#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>
#include "mainwindow.h"
#include "model.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT
    // Create new window and model for the new button
    MainWindow *mainWindow;
    Model *model;

public:
    /**
     * @brief MainMenu is the constructor for the MainMenu class which is the introduction
     * page for the Le Spork Editor
     * @param parent
     */
    explicit MainMenu(QWidget *parent = nullptr);
    // Destuctor when the class is being removed
    ~MainMenu();

private slots:
    /**
     * @brief on_pixel8_pressed generates a 8x8 pixel canvas and display it on a
     * new window with a new model
     */
    void on_pixel8_pressed();

    /**
     * @brief on_pixel16_pressed generates a 16x16 pixel canvas and display it on a
     * new window with a new model
     */
    void on_pixel16_pressed();

    /**
     * @brief on_pixel32_pressed generates a 32x32 pixel canvas and display it on a
     * new window with a new model
     */
    void on_pixel32_pressed();

    /**
     * @brief on_pixel64_pressed generates a 64x64 pixel canvas and display it on a
     * new window with a new model
     */
    void on_pixel64_pressed();

    /**
     * @brief on_pixel128_pressed generates a 128x128 pixel canvas and display it on a
     * new window with a new model
     */
    void on_pixel128_pressed();

    /**
     * @brief on_pixel256_pressed generates a 256x256 pixel canvas and display it on a
     * new window with a new model
     */
    void on_pixel256_pressed();

private:
    Ui::MainMenu *ui;
};

#endif // MAINMENU_H
