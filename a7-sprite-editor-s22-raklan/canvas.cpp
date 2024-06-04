#include "canvas.h"

canvas::canvas(QWidget *parent) : QGraphicsView(parent)
{

}

void canvas::mouseMoveEvent(QMouseEvent *event)
{
    emit canvasDragged(event);
}

void canvas::mousePressEvent(QMouseEvent *event)
{
    emit canvasPressed(event);
}
