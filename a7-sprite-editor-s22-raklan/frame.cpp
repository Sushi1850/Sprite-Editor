#include "frame.h"
#include <QtDebug>

Frame::Frame(int width, int height)
    :image(width, height, QImage::Format_ARGB32)
{
    image.fill(0);
}

void Frame::setPixel(int x, int y, QColor color)
{
    image.setPixel(x,y,color.rgba());
}

QColor Frame::getPixel(int x, int y)
{
    return image.pixelColor(x,y);
}

QImage Frame::getImage()
{
    return image;
}

QPixmap Frame::getPixMap()
{
    return QPixmap::fromImage(image);
}

std::string Frame::frameAsString()
{
   std::string result;
   for(int h =0; h < image.height(); h++)
   {
       result.append("{ ");
       for(int w =0; w < image.width(); w++)
       {
           char pixel [23];
           QColor pColor = image.pixelColor(w,h);
           sprintf (pixel, "{ %d, %d, %d, %d, }", pColor.red(), pColor.green(), pColor.blue(), pColor.alpha());
           result.append(pixel);
           if(w != image.width() -1)
           {
               result.append(", ");
           }
       }
       if(h != image.height() -1)
       {
           result.append(", \n");
       }
   }
   return  result;
}

bool Frame::operator==(const Frame& rhs)
{
    return image == rhs.image;
}

bool Frame::exportPNG(QString fileName)
{
    return image.save(fileName, "PNG");
}

void Frame::write(QJsonObject &json, int frameNum) const
{
    QJsonArray pixelRowsArray;
    for(int h =0; h < image.height(); h++)
    {
        QJsonArray pixelsArray;
        for(int w =0; w < image.width(); w++)
        {

            QJsonArray pixel;

            QColor pColor = image.pixelColor(w,h);

            pixel.append(pColor.red());
            pixel.append(pColor.green());
            pixel.append(pColor.blue());
            pixel.append(pColor.alpha());

            pixelsArray.append(pixel);
        }
        pixelRowsArray.append(pixelsArray);
    }
    QString s = QString::number(frameNum);
    json["frame" + s] = pixelRowsArray;
}
