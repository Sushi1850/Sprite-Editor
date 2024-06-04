#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QFile>
#include <QAction>
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a new MainWindow for editing and interacting with Sprites
     * @param parent the Parent widget of this object
     */
    MainWindow(Model&, QWidget *parent = nullptr);
    /**
     * @brief Destroys the MainWindow object
     *
     */
    ~MainWindow();

private slots:
    /**
     * @brief Informs the view that the user clicked the "Left Mouse Color" button.
     * It then allows the user to select a new color to bind to that button, then updates
     * the Model with the selected choice
     */
    void on_color1Button_pressed();
    /**
     * @brief Informs the view that the user clicked the "Right Mouse Color" button.
     * It then allows the user to select a new color to bind to that button, then updates
     * the Model with the selected choice
     */
    void on_color2Button_pressed();

    /**
     * @brief Informs the View that the user clicked the Pen Tool Button. It will
     * then update the button highlighting accordingly, then inform the Model that
     * the current Tool has changed
     */
    void on_penButton_clicked();
    /**
     * @brief Informs the View that the user clicked the Eraser Tool Button. It will
     * then update the button highlighting accordingly, then inform the Model that
     * the current Tool has changed
     */
    void on_eraserButton_clicked();
    /**
     * @brief Informs the View that the user clicked the Paint Bucket Tool Button. It will
     * then update the button highlighting accordingly, then inform the Model that
     * the current Tool has changed
     */
    void on_bucketButton_clicked();
    /**
     * @brief Informs the View that the user clicked the Rectangle Tool Button. It will
     * then update the button highlighting accordingly, then inform the Model that
     * the current Tool has changed
     */
    void on_rectangleButton_clicked();
    /**
     * @brief Informs the View that the user clicked the Ellipse Tool Button. It will
     * then update the button highlighting accordingly, then inform the Model that
     * the current Tool has changed
     */
    void on_ellipseButton_clicked();
    /**
     * @brief Informs the Model that the user would like to add a new
     * frame to the current Sprite
     */
    void on_addFrameButton_pressed();
    /**
     * @brief Informs the View that the user clicked the "Erase Frame" Button. It will
     * request confirmation from the user, then inform the Model that the User wishes
     * to delete the current frame
     */
    void on_deleteFrameButton_pressed();    
    /**
     * @brief Opens a window informing the User of the various tools and features at
     * their disposal
     */
    void on_actionAbout_LeeSporkSprite_triggered();
    /**
     * @brief Displays a window with instructions on using each of the tools and
     * features offered by the editor
     */
    void on_actionLeSporkSprite_Help_triggered();
    /**
     * @brief Displays the previous frame
     * to the current frame for editing
     */
    void on_previousFrame_clicked();
    /**
     * @brief Displays the next frame to the
     * current frame for editing
     */
    void on_nextFrame_clicked();
    /**
     * @brief Opens a dialog allowing the user to export the current frame as a .png image
     */
    void on_exportMenu_Action();
    /**
     * @brief Opens a dialog allowing the user to save their Sprite as a .ssp file
     */
    void on_saveMenu_Action();
    /**
     * @brief Opens a dialog allowing the user to open a previously saved .ssp file for editing
     */
    void on_openMenu_Action();
    /**
     * @brief Opens a new editing window with an 8x8 canvas
     */
    void on_action8x8_triggered();
    /**
     * @brief Opens a new editing window with a 16x16 canvas
     */
    void on_action16x16_triggered();
    /**
     * @brief Opens a new editing window with a 32x32 canvas
     */
    void on_action32x32_triggered();
    /**
     * @brief Opens a new editing window with a 64x64 canvas
     */
    void on_action64x64_triggered();
    /**
     * @brief Opens a new editing window with a 128x128 canvas
     */
    void on_action128x128_triggered();
    /**
     * @brief Updates the displayed value of the FPS slider for the user
     * @param newValue
     */
    void previewFPSChanged(int newValue);

    void on_clearFrameButton_pressed();

public slots:
    /**
     * @brief Updates the canvas to display the given QPixmap
     * @param newPixmap the QPixmap which the canvas should display
     */
    void updateCanvas(QPixmap newPixmap);
    /**
     * @brief Updates the preview to display the given QPixmap
     * @param newPixmap the QPixmap which the preview should display
     */
    void updatePreview(QPixmap newPixmap);
    /**
     * @brief Updates the "onion skin" to display the given QPixmap
     * @param newPixmap the QPixmap which the onion skin should display
     */
    void updateOnionSkin(QPixmap newPixmap);
    /**
     * @brief Updates the frame Index to display in label
     * @param frameIndex
     */
    void updateFrameIndex(QString frameIndex);
    /**
     * @brief Updates the fram Size to display in label
     * @param frameSize
     */
    void updateNumberOfFrames(QString frameSize);

signals:
    /**
     * @brief Informs the Model to save the given QColor to the
     * given MouseButton for later painting
     * @param newColor the new color to save
     * @param associatedMouseButton the mouse button to which newColor should be saved
     */
    void colorChanged(QColor newColor, Qt::MouseButton associatedMouseButton);
    /**
     * @brief Informs the Model that the given UIButton has been pressed
     * @param the button that was pressed
     */
    void uiButtonPressed(UIButton pressedButton);
    /**
     * @brief Requests the Moddel to export the current frame at the given filePath
     * @param filePath the filepath to which the frame should be exported
     */
    void exportFame(QString filePath);
    /**
     * @brief Requests the Moddel to save the current Sprite at the given filePath
     * @param filePath the filepath to which the Sprite should be save
     */
    void saveProject(QString filePath);
    /**
     * @brief Requests the Moddel to open the saved file at the given filePath
     * @param filePath the filepath from which the frame should be opened
     */
    void openProject(QString filePath);

private:
    /**
     * @brief The backing ui object that contains each UI component
     */
    Ui::MainWindow *ui;
    /**
     * @brief A reference to the Model, primarily used for the View to request
     * properties from the model such as currently selected colors and frame size
     */
    Model* model;
    /**
     * @brief The backing component for the Canvas itself. Setting the Pixmap of
     * this object is reflected on the Canvas
     */
    QGraphicsScene *canvasGraphic;
    /**
     * @brief The backing component for the preview itself. Setting the Pixmap of
     * this object is reflected on the preview
     */
    QGraphicsScene *previewGraphic;
    /**
     * @brief The backing component for the "onion skin" itself. Setting the Pixmap of
     * this object is reflected on the onion skin
     */
    QGraphicsScene *onionGraphic;
    /**
     * @brief Helper method that calls connect on the various signals and slots
     * required for the program to work.
     */
    void setupSignalsAndSlots();
    /**
     * @brief Helper method that sets up the various UI display components
     */
    void setupCanvas();
    /**
     * @brief Scales the canvas view to show up at an appropriate size
     */
    void ScaleCanvas();
    /**
     * @brief Scales the onion skin to show up at the appropriate size
     */
    void ScaleOnionSkin();
    /**
     * @brief Highlights the given UIButton, and un-highlights every other button. This is
     * used to display the user's currently selected tool. NOTE--This method will only highlight the
     * tool buttons; passing in any other UIButton will only un-highlight all buttons, no button
     * will recieve highlighting
     */
    void highlightButton(UIButton);

};
#endif // MAINWINDOW_H
