#include "model.h"
#include "commonDataTypes.h"
#include "qpainter.h"
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <queue>

namespace std {
    template <> struct hash<QPoint>
    {
        size_t operator()(const QPoint& p) const
        {
            return std::hash<uint32_t>()(p.x()) ^ std::hash<uint32_t>()(p.y());
        }
    };
}

Model::Model(QObject *parent, int _frameSize)
    : QObject{parent}
{
    previewFps = 3;
    frameSize = _frameSize;
    currentTool = Pen;
    frames.push_back(Frame(frameSize, frameSize));
    QTimer::singleShot(1000/previewFps, this, &Model::previewController);
}

Model::Model(QString filepath)
{
    previewFps = 3;
    currentTool = Pen;
    // load frames, width, and height:
    read(filepath);
    QTimer::singleShot(1000/previewFps, this, &Model::previewController);
    QTimer::singleShot(500, [this](){emit updateCanvas(frames[currentFrameIndex].getPixMap());});

}

void Model::saveProject(QString filepath)
{
    write(filepath);
}

QPixmap Model::getOnionSkinPixmap(int framesIndex)
{
    QImage onionFrameCopy = frames[framesIndex].getImage().copy();
    QPainter p;
    p.begin(&onionFrameCopy);
    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p.fillRect(onionFrameCopy.rect(), QColor(0, 0, 0, 120));
    p.end();
    QPixmap onionPixmap = QPixmap::fromImage(onionFrameCopy);
    QSize newSize(frameSize * getScalar(), frameSize * getScalar());
    return onionPixmap.scaled(newSize);
}

void Model::bucketFill(QPoint startPos, QColor replacementColor, QColor targetColor)
{
    std::queue<QPoint> q;
    q.push(startPos);
    while(!q.empty())
    {
        QPoint n = q.front();
        q.pop();
        int x = n.x();
        int y = n.y();
        QColor currentPixelColor = frames[currentFrameIndex].getPixel(x,y);

        if(currentPixelColor == targetColor)
        {
            frames[currentFrameIndex].setPixel(x,y, replacementColor);
            //West
            if(x - 1 >= 0)
            {
                QPoint newPoint(x-1,y);
                if(frames[currentFrameIndex].getPixel(newPoint.x(),newPoint.y()) != replacementColor)
                {
                    q.push(newPoint);
                }
            }
            //East
            if(x + 1 < frameSize)
            {
                QPoint newPoint(x+1,y);
                if(frames[currentFrameIndex].getPixel(newPoint.x(),newPoint.y()) != replacementColor)
                {
                    q.push(newPoint);
                }
            }
            //North
            if(y + 1 < frameSize)
            {
                QPoint newPoint(x,y+1);
                if(frames[currentFrameIndex].getPixel(newPoint.x(),newPoint.y()) != replacementColor)
                {
                    q.push(newPoint);
                }
            }
            //South
            if(y - 1 >= 0)
            {
                QPoint newPoint(x,y-1);
                if(frames[currentFrameIndex].getPixel(newPoint.x(),newPoint.y()) != replacementColor)
                {
                    q.push(newPoint);
                }
            }
        }
        else
        {
            //qDebug() << "Non target Pixel: " << x << " : " << y;
        }
    }
}

void Model::previewController()
{
    if(playPreview && frames.size() != 0)
    {
        if(previewFrameIndex < frames.size() -1)
        {
            previewFrameIndex++;
        }
        else
        {
            previewFrameIndex = 0;
        }
        QPixmap preview = frames.at(previewFrameIndex).getPixMap();
        if(previewScaling)
        {
            QSize newSize(frameSize * getPreviewWindowScalar(), frameSize * getPreviewWindowScalar());
            preview = preview.scaled(newSize);
        }

        emit updatePreview(preview);
    }
    QTimer::singleShot(1000/previewFps, this, &Model::previewController);
}

void Model::rectangleTool(QPoint startPos, QPoint endPos, QColor paintColor)
{
    int xEnd = std::max(startPos.x(), endPos.x());
    int xStart = std::min(startPos.x(), endPos.x());
    int yEnd = std::max(startPos.y(), endPos.y());
    int yStart = std::min(startPos.y(), endPos.y());

    // will color top and bottom borders and the corners
    for(int x = xStart; x <= xEnd; x++)
    {
        frames[currentFrameIndex].setPixel(x, yStart, paintColor);
        frames[currentFrameIndex].setPixel(x, yEnd, paintColor);
    }
    // will color left and right borders (but not corners)
    for(int y = yStart + 1; y < yEnd; y++)
    {
        frames[currentFrameIndex].setPixel(xStart, y, paintColor);
        frames[currentFrameIndex].setPixel(xEnd, y, paintColor);
    }
}

void Model::ovalTool(QPoint startPos, QPoint endPos, QColor paintColor)
{
    int xEnd = std::max(startPos.x(), endPos.x());
    int xStart = std::min(startPos.x(), endPos.x());
    int yEnd = std::max(startPos.y(), endPos.y());
    int yStart = std::min(startPos.y(), endPos.y());
    // obtain center of ellipse:
    int xRadius = (xEnd - xStart)/2;
    int yRadius = (yEnd-yStart)/2;
    int xCenter = xStart + (xRadius);
    int yCenter = yStart + (yRadius);

    float pX, pY, p1, p2, x, y;
    x = 0;
    y = yRadius;

    // Prediction parameter values for ellipse region 1
    p1 = (yRadius * yRadius) - (xRadius * xRadius * yRadius) + (0.25 * xRadius * xRadius);
    pX = 2 * yRadius * yRadius * x;
    pY = 2 * xRadius * xRadius * y;

    // Set Pixels for ellipse region 1
    while (pX < pY)
    {
        // plot ellipse:
        frames[currentFrameIndex].setPixel(x + xCenter, y + yCenter, paintColor);
        frames[currentFrameIndex].setPixel(x + xCenter, -y + yCenter, paintColor);
        frames[currentFrameIndex].setPixel(-x + xCenter, y + yCenter, paintColor);
        frames[currentFrameIndex].setPixel(-x + xCenter, -y + yCenter, paintColor);

        // Update prediction parameters:
        if (p1 < 0)
        {
            x++;
            pX = pX + (2 * yRadius * yRadius);
            p1 = p1 + pX + (yRadius * yRadius);
        }
        else
        {
            x++;
            y--;
            pX = pX + (2 * yRadius * yRadius);
            pY = pY - (2 * xRadius * xRadius);
            p1 = p1 + pX - pY + (yRadius * yRadius);
        }
    }

    // Decision parameter of region 2
    p2 = ((yRadius * yRadius) * ((x + 0.5) * (x + 0.5))) +
         ((xRadius * xRadius) * ((y - 1) * (y - 1))) -
         (xRadius * xRadius * yRadius * yRadius);

    // Set Pixels for ellipse region 2
    while (y >= 0)
    {
        // plot ellipse:
        frames[currentFrameIndex].setPixel(x + xCenter, y + yCenter, paintColor);
        frames[currentFrameIndex].setPixel(x + xCenter, -y + yCenter, paintColor);
        frames[currentFrameIndex].setPixel(-x + xCenter, y + yCenter, paintColor);
        frames[currentFrameIndex].setPixel(-x + xCenter, -y + yCenter, paintColor);

        // Update prediction parameters:
        if (p2 > 0)
        {
            y--;
            pY = pY - (2 * xRadius * xRadius);
            p2 = p2 + (xRadius * xRadius) - pY;
        }
        else
        {
            y--;
            x++;
            pX = pX + (2 * yRadius * yRadius);
            pY = pY - (2 * xRadius * xRadius);
            p2 = p2 + pX - pY + (xRadius * xRadius);
        }
    }
}

void Model::mouseUsed(QMouseEvent *event)
{
    QPoint clickedPoint = getScaledPoint(QPoint(event->position().x(), event->position().y()));
    if(currentTool == Pen)
    {
        QColor colorToPaint;
        if(event->buttons() == Qt::RightButton)
        {
            colorToPaint = rightMouseColor;
        }
        else if(event->buttons() == Qt::LeftButton)
        {
            colorToPaint = leftMouseColor;
        }
        frames[currentFrameIndex].setPixel(clickedPoint.x(), clickedPoint.y(), colorToPaint);
    }
    else if(currentTool == Eraser)
    {
        frames[currentFrameIndex].setPixel(clickedPoint.x(), clickedPoint.y(), Qt::transparent);
    }
    else if(currentTool == PaintBucket)
    {
       QColor colorToPaint;
       if(event->buttons() == Qt::RightButton)
       {
           colorToPaint = rightMouseColor;
       }
       else if(event->buttons() == Qt::LeftButton)
       {
           colorToPaint = leftMouseColor;
       }
       bucketFill(clickedPoint,
                  colorToPaint,
                  frames[currentFrameIndex].getPixel(clickedPoint.x(), clickedPoint.y()));
    }
    else if(currentTool == RectangleTool)
    {
        if(isDrawingShape){
            QColor colorToPaint;
            if(event->buttons() == Qt::RightButton)
            {
                colorToPaint = rightMouseColor;
            }
            else if(event->buttons() == Qt::LeftButton)
            {
                colorToPaint = leftMouseColor;
            }
            rectangleTool(shapeStartPosition, clickedPoint, colorToPaint);
            isDrawingShape = false;
        }
        else
        {
            shapeStartPosition = clickedPoint;
            isDrawingShape = true;
        }
    }
    else if(currentTool == EllipseTool)
    {
        if(isDrawingShape)
        {
            QColor colorToPaint;
            if(event->buttons() == Qt::RightButton)
            {
                colorToPaint = rightMouseColor;
            }
            else if(event->buttons() == Qt::LeftButton)
            {
                colorToPaint = leftMouseColor;
            }
            ovalTool(shapeStartPosition, clickedPoint, colorToPaint);
            isDrawingShape = false;
        }
        else
        {
            shapeStartPosition = clickedPoint;
            isDrawingShape = true;
        }
    }

    emit updateCanvas(frames[currentFrameIndex].getPixMap());
}

void Model::uiButtonPressed(UIButton buttonPressed)
{
    isDrawingShape = false;
    switch(buttonPressed)
    {
        case PenButton:
            currentTool = Pen;
            break;
        case EraserButton:
            currentTool = Eraser;
            break;
        case PaintBucketButton:
            currentTool = PaintBucket;
            break;
        case RectangleToolButton:
            currentTool = RectangleTool;
            break;
        case EllipseToolButton:
            currentTool = EllipseTool;
            break;
        case AddFrameButton:
            //Checking if the current Frame is seperate from the frames one
            frames.push_back(Frame(frameSize, frameSize));
            currentFrameIndex++;
            break;
        case PreviousFrameButton:
            if (currentFrameIndex > 0)
            {
                currentFrameIndex--;
            }
            break;
        case NextFrameButton:
            if (currentFrameIndex < frames.size()-1)
            {
                currentFrameIndex++;
            }
            break;
        case DeleteFrameButton:
            if (currentFrameIndex > 0)
            {
                frames.erase(std::find(frames.begin(),frames.end(),frames[currentFrameIndex]));
                currentFrameIndex--;
            }
            break;
        case ClearFrameButton:
            for(int y = 0; y < frameSize; y++)
                for (int x = 0; x < frameSize; x++)
                    frames[currentFrameIndex].setPixel(x, y, Qt::transparent);
        break;

    }
    emit updateNumberOfFrames(QString::number(frames.size()));
    emit updateCurrentFrameIndex(QString::number(currentFrameIndex + 1));
    emit updateCanvas(frames[currentFrameIndex].getPixMap());
    if(currentFrameIndex > 0)
    {
        emit updateOnionSkin(getOnionSkinPixmap(currentFrameIndex-1));
    }
    else
    {
        emit updateOnionSkin(QPixmap());
    }
}
void Model::colorChanged(QColor newColor, Qt::MouseButton mouseButton)
{
    if(mouseButton == Qt::LeftButton)
    {
        leftMouseColor = newColor;
    }
    else if(mouseButton == Qt::RightButton)
    {
        rightMouseColor = newColor;
    }
}

void Model::alphaChanged(int alpha)
{
    leftMouseColor.setAlpha(alpha);
    rightMouseColor.setAlpha(alpha);
}

QColor Model::getLeftMouseColor()
{
    return leftMouseColor;
}
QColor Model::getRightMouseColor()
{
    return rightMouseColor;
}

QPoint Model::getScaledPoint(QPoint unscaledPoint)
{
    QPoint scaledPoint;
    int scalar = getScalar();
    if(frameSize <= 16)
    {
        scaledPoint.setX(unscaledPoint.x()/(frameSize*scalar));
        scaledPoint.setY(unscaledPoint.y()/(frameSize*scalar));
    }
    else
    {
        scaledPoint.setX(unscaledPoint.x()/(frameSize/scalar));
        scaledPoint.setY(unscaledPoint.y()/(frameSize/scalar));
    }
    return scaledPoint;
}

int Model::getScalar()
{
    switch(frameSize)
      {
      case 8:
          return 8;
      case 16:
          return 2;
      case 32:
          return 2;
      case 64:
          return 8;
      case 128:
          return 32;
      default:
          return 1;
    }
}

int Model::getPreviewWindowScalar()
{
    switch(frameSize)
      {
      case 8:
          return 16;
      case 16:
          return 8;
      case 32:
          return 4;
      case 64:
          return 2;
      case 128:
          return 1;
      default:
          return 1;
    }
}

int Model::getSize()
{
    return frameSize;
}

void Model::exportFame(QString filePath)
{
    frames[currentFrameIndex].exportPNG(filePath);
}

void Model::togglePreviewScaling(bool checked)
{
    previewScaling = !checked;
}

void Model::previewFPSChanged(int value)
{
    previewFps = value;
}

void Model::write(QString filepath) const
{
    QFile projectFile(filepath);

    if (!projectFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
    }
    QJsonObject projectObject;
    projectObject["height"] = frameSize;
    projectObject["width"] = frameSize;
    projectObject["numberOfFrames"] = (int)frames.size();
    QJsonObject framesArray;

    for(int i = 0; i < (int)frames.size(); i++)
    {
        frames[i].write(framesArray, i);
    }
    projectObject["frames"] = framesArray;

    projectFile.write(QJsonDocument(projectObject).toJson());
}

void Model::read(QString filepath)
{
    QFile projectFile(filepath);

    if (!projectFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
    }

    QByteArray saveData = projectFile.readAll();

    QJsonDocument projectDoc(QJsonDocument::fromJson(saveData));
    QJsonObject projectObject = projectDoc.object();

    if(projectObject.contains("height"))
    {
        frameSize = projectObject["height"].toInt();
    }

    if(projectObject.contains("frames"))
    {
        QJsonObject framesArray = projectObject["frames"].toObject();
        frames.clear();
        currentFrameIndex = 0;

        for(int frameIndex = 0; frameIndex < framesArray.size(); frameIndex++)
        {
            Frame f(frameSize, frameSize);
            frames.push_back(f);
            QString s = QString::number(frameIndex);
            QJsonArray rowsArray = framesArray["frame" + s].toArray();

            for(int rowIndex = 0; rowIndex < rowsArray.size(); rowIndex++)
            {
                QJsonArray pixelsArray = rowsArray[rowIndex].toArray();
                for(int pixelIndex = 0; pixelIndex < pixelsArray.size(); pixelIndex++)
                {
                    QJsonArray pixelColors = pixelsArray[pixelIndex].toArray();
                    QColor pixelColor(pixelColors[0].toInt(),pixelColors[1].toInt(),
                            pixelColors[2].toInt(), pixelColors[3].toInt());
                    frames[frameIndex].setPixel(pixelIndex, rowIndex, pixelColor);
                }
            }
        }
    }
}
