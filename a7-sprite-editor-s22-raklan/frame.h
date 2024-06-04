#ifndef FRAME_H
#define FRAME_H

#include <QImage>
#include <QPixmap>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class Frame
{
private:
    QImage image;


public:
    /**
     * @brief Frame is the object that is displayed on the canvas.
     * @param width which determine how wide the canvas is for this frame
     * @param height which determine how tall the canvas is for this frame
     */
    Frame(int width, int height);

    /**
     * @brief setPixel set a pixel with a specific color
     * @param x is the coordinate in the x axis of the frame
     * @param y is the coordinate in the y axis of the frame
     * @param color
     */
    void setPixel(int x, int y, QColor color);

    /**
     * @brief getPixel retrieves the color of a certain pixel
     * @param x is the coordinate in the x axis of the frame
     * @param y is the coordinate in the y axis of the frame
     * @return the color of the pixel
     */
    QColor getPixel(int x, int y);

    /**
     * @brief getImage returns the image
     * @return the image
     */
    QImage getImage();

    /**
     * @brief getPixMap returns the pixMap
     * @return the pixMap
     */
    QPixmap getPixMap();

    /**
     * @brief frameAsString generate a string format of the frame
     * @return a string of the frame
     */
    std::string frameAsString();

    /**
     * @brief operator == compares a given image to this object image
     * @param rhs the other image
     * @return a true/false on whether or not this frame is the same as the other
     */
    bool operator==(const Frame& rhs);

    /**
     * @brief exportPNG Export the string into a PNG format
     * @param fileName the filename that is being exported to a PNG
     * @return a true/false on whether it was able to save it as a PNG
     */
    bool exportPNG(QString fileName);

    /**
     * @brief write stores a given frame to the json array
     * @param json is the object that is used to store the frame in an array
     * @param framNum the frame number
     */
    void write(QJsonObject &json, int framNum) const;

    /**
     * @brief read
     * @param json
     * @param framNum
     */
};

#endif // FRAME_H
