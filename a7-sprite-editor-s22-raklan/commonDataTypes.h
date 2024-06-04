#ifndef COMMONDATATYPES_H
#define COMMONDATATYPES_H

/**
 * @brief The Tool enum defines each of the possible
 * Tools which the user is able to use to edit Sprites
 */
enum Tool{
    Pen,
    Eraser,
    PaintBucket,
    RectangleTool,
    EllipseTool
};

/**
 * @brief The UIButton enum defines each button on the UI that the user can click.
 */
enum UIButton{
    PenButton,
    EraserButton,
    PaintBucketButton,
    RectangleToolButton,
    EllipseToolButton,
    AddFrameButton,
    PreviousFrameButton,
    NextFrameButton,
    DeleteFrameButton,
    ClearFrameButton
};

#endif // COMMONDATATYPES_H
