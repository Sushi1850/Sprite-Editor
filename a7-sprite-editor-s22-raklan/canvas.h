#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsView>
#include <QtWidgets>
#include <Qwidget>
#include <QMouseEvent>
#include <QDebug>

class canvas : public QGraphicsView
{
Q_OBJECT
public:
    /**
     * @brief canvas is the area where the user can create or edit their sprite. Display as pixels
     * @param parent
     */
    canvas(QWidget* parent = 0);

protected:
    /**
     * @brief mouseMoveEvent is an event that triggers when a mouse has moved
     * @param event is the MouseEvent
     */
    void mouseMoveEvent(QMouseEvent *event);
    /**
     * @brief mousePressEvent is an event that triggers when a mouse button is clicked
     * @param event is the MouseEvent
     */
    void mousePressEvent(QMouseEvent *event);

signals:
    /**
     * @brief canvasDragged is trigger when a button is click and drag on the canvas
     * @param event is the QMouseEvent that contain mouse related information
     */
    void canvasDragged(QMouseEvent *event);
    /**
     * @brief canvasPressed is trigger when a button is click on the canvas
     * @param event is the QMouseEvent that contain mouse related information
     */
    void canvasPressed(QMouseEvent *event);
};

#endif // CANVAS_H
