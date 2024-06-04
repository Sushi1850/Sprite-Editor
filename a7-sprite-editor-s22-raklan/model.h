#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QMouseEvent>
#include <vector>
#include <fstream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "frame.h"
#include "commonDataTypes.h"

class Model : public QObject
{
private:
    Q_OBJECT
    std::vector<Frame> frames;
    int currentFrameIndex = 0;
    int frameSize;
    int previewFps;
    Tool currentTool;
    bool isDrawingShape;
    QColor leftMouseColor = Qt::black;
    QColor rightMouseColor = Qt::white;
    QPoint shapeStartPosition;
    bool playPreview = true;
    int previewFrameIndex = 0;
    bool previewScaling = true;

    /**
     * @brief Controller for the preview itself. It will handle updating the Preview at the
     * user's selected fps playback settings.
     */
    void previewController();
    /**
     * @brief Handles getting the appropriate pixmap for the frame before the user's
     * currently edited frame
     * @param framesIndex the index of the frame for which the pixmap should be retrieved.
     * @return the scaled pixmap for the appropriate frame
     */
    QPixmap getOnionSkinPixmap(int framesIndex);
    /**
     * @brief Helper method that, given a QPoint containing the raw coordinates of a point on the canvas,
     * this will scale it accordingly and return a new point containing the coordinates
     * of the pixel where the user clicked, which can then be used to set a pixel of the
     * current frame
     * @param unscaledPoint A point containing the raw x- and y-coordinate where the canvas was clicked
     * @return A point containing the pixel coordinate on the frame where the user clicked
     */
    QPoint getScaledPoint(QPoint unscaledPoint);
    /**
     * @brief Helper method used in scaling based on the frame size. Helps to keep things straight
     * between the View's display size and the Model
     * @return An int representing a scale factor for the current frame size
     */
    int getScalar();
    /**
     * @brief The preview requires slightly different scaling than the canvas, so it has its own
     * helper method for determining the scale factor. See getScalar() for details on what the
     * scalar is used for
     * @return an int representing a scale factor for the current frame size
     */
    int getPreviewWindowScalar();
    /**
     * @brief Writes the current Sprite to the given filepath as a JSON object, saved in a
     * .ssp file
     */
    void write(QString filepath) const;
    /**
     * @brief Reads a previously saved .ssp file, populating each frame as dictated by the
     * savefile
     */
    void read(QString filepath);

    /**
     * @brief Given a starting position, color to paint with, and color to paint over, this will
     * fill all pixels of the target color within boundaries dictated by either the canvas itself
     * or any pixel that does not match the target color
     * @param startPos The starting position (In scaled pixel coordinates) where painting should start
     * @param replacementColor The color which should be painted with
     * @param targetColor The color which should be painted over
     */
    void bucketFill(QPoint startPos, QColor replacementColor, QColor targetColor);

    /**
     * @brief rectangleTool applies color changes to pixels in a rectangle shape.
     * Will only change the border pixels (does not fill the inside of the rectangle).
     * @param startPos the start corner of the rectangle
     * @param endPos the end corner of the rectangle
     * @param paintColor the color to apply to the pixels
     */
    void rectangleTool(QPoint startPos, QPoint endPos, QColor paintColor);

    /**
     * @brief ovalTool applies color changes to pixels in an oval/ellipse shape.
     * Will only change the edge pixels (does not fill the inside of the oval).
     * This method uses the Midpoint ellipse algorithm, and it is adapted from
     * psuedo code found here www.cpp.edu/~raheja/CS445/MEA.pdf.
     * @param startPos start x and y values
     * @param endPos end x and y values
     * @param paintColor the color to apply to the pixels
     */
    void ovalTool(QPoint startPos, QPoint endPos, QColor paintColor);

public:
    /**
     * @brief Constructs a Model from scratch
     * @param parent A parent QObject
     * @param frameSize the size in pixels of each side of the Sprite (only square Sprites are allowed)
     */
    explicit Model(QObject *parent = nullptr, int frameSize = 16);
    /**
     * @brief Constructs a Model using a .ssp file found at filepath
     * @param filepath the filepath to the .ssp file of a previously saved Sprite
     * @param parent A parent QObject
     */
    Model(QString filepath);
    /**
     * @brief Returns the color saved to the Left Mouse Button. Used by the View to keep
     * the color selector buttons current
     * @return The QColor saved to the Left Mouse Button
     */
    QColor getLeftMouseColor();
    /**
     * @brief Returns the color saved to the Right Mouse Button. Used by the View to keep
     * the color selector buttons current
     * @return The QColor saved to the Right Mouse Button
     */
    QColor getRightMouseColor();
    /**
     * @brief Returns the frame size of the current Sprite
     * @return the frame size of the current Sprite
     */
    int getSize();    

public slots:
    /**
     * @brief Driver for write(), gives the View a public slot to bind to.
     */
    void saveProject(QString filepath);
    /**
     * @brief Exports the current frame as a .png file, saved at the given
     * filepath
     * @param the filepath at which the .png file should be saved
     */
    void exportFame(QString filePath);
    /**
     * @brief Informs the Model that the mouse has been clicked and/or dragged on the
     * canvas. Uses the given QMouseEvent to determine where and how to paint
     */
    void mouseUsed(QMouseEvent*);
    /**
     * @brief Informs the Model that one of the UI Buttons was pressed and will
     * take the appropriate action, whether that's updating the current Tool, the current
     * frame, or anything else the model must track
     */
    void uiButtonPressed(UIButton);
    /**
     * @brief Informs the Model that the user has selected a new color and
     * will bind it to the given MouseButton
     */
    void colorChanged(QColor, Qt::MouseButton);
    /**
     * @brief Informs the Model that the user has changed the transparency value
     * of the Tools, and updates accordingly
     * @param alpha The new alpha value
     */
    void alphaChanged(int alpha);
    /**
     * @brief Toggles whether the preview is displayed at a scaled view or not.
     * @param checked the value of the checkbox on the View this should be related to
     */
    void togglePreviewScaling(bool checked);
    /**
     * @brief Informs the Model that the user has updated the "Preview FPS" slider and
     * updates the preview playback rate accordingly
     * @param value the new value of the slider
     */
    void previewFPSChanged(int value);

signals:
    /**
     * @brief Updates the canvas with the given QPixmap
     * @param updatedCanvas the new QPixmap to display
     */
    void updateCanvas(QPixmap updatedCanvas);
    /**
     * @brief Updates the preview with the given QPixmap
     * @param updatedPreview the new QPixmap to display
     */
    void updatePreview(QPixmap newPreview);
    /**
     * @brief Updates the onion skin with the given QPixmap
     * @param updatedOnionSkin the new QPixmap to display
     */
    void updateOnionSkin(QPixmap newOnionSkin);
    /**
     * @brief Updates the current frame index to send it to the view
     * @param frameIndex
     */
    void updateCurrentFrameIndex(QString frameIndex);
    /**
     * @brief Updates the frame size to send it to the view
     * @param frameSize
     */
    void updateNumberOfFrames(QString frameSize);

};

#endif // MODEL_H
