#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QColorDialog>
#include "canvas.h"
#include "model.h"

#include <QDebug>

MainWindow::MainWindow(Model& _model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = &_model;
    canvasGraphic = new QGraphicsScene(this);
    previewGraphic = new QGraphicsScene(this);
    onionGraphic = new QGraphicsScene(this);

    setupSignalsAndSlots();
    setupCanvas();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvasGraphic;
    delete previewGraphic;
    delete onionGraphic;
    delete model;
}

void MainWindow::setupSignalsAndSlots()
{
    /*===PAINTING===*/
    connect(ui->canvasView,
            &canvas::canvasDragged,
            model,
            &Model::mouseUsed);
    connect(ui->canvasView,
            &canvas::canvasPressed,
            model,
            &Model::mouseUsed);
    connect(this,
            &MainWindow::colorChanged,
            model,
            &Model::colorChanged);

    /*===SAVING LOADING & EXPORTING===*/
    //=View=
    auto menuActions = ui->menuFile->actions();
    QAction* exportAction = menuActions.back();
    connect(exportAction,
            &QAction::triggered,
            this,
            &MainWindow::on_exportMenu_Action);
    connect(ui->actionSave_Sprite,
            &QAction::triggered,
            this,
            &MainWindow::on_saveMenu_Action);
    connect(ui->actionOpen_Sprite,
            &QAction::triggered,
            this,
            &MainWindow::on_openMenu_Action);
    //=Model=
    connect(this,
            &MainWindow::saveProject,
            model,
            &Model::saveProject);
    connect(this,
            &MainWindow::exportFame,
            model,
            &Model::exportFame);

    /*===MODEL UPDATES FROM VIEW===*/
    connect(ui->disablePreviewScaling,
             &QAbstractButton::toggled,
             model,
             &Model::togglePreviewScaling);
    connect(ui->previewFPSSlider,
            &QAbstractSlider::valueChanged,
            model,
            &Model::previewFPSChanged);
    connect(this,
            &MainWindow::uiButtonPressed,
            model,
            &Model::uiButtonPressed);
    connect(ui->brushTransparencySlider,
            &QSlider::valueChanged,
            model,
            &Model::alphaChanged);

    /*===VIEW UPDATES FROM MODEL===*/
    connect(model,
            &Model::updateCanvas,
            this,
            &MainWindow::updateCanvas);
    connect(model,
            &Model::updatePreview,
            this,
            &MainWindow::updatePreview);
    connect(model,
            &Model::updateOnionSkin,
            this,
            &MainWindow::updateOnionSkin);
    connect(model,
            &Model::updateCurrentFrameIndex,
            this,
            &MainWindow::updateFrameIndex);
    connect(model,
            &Model::updateNumberOfFrames,
            this,
            &MainWindow::updateNumberOfFrames);

    /*===MISC===*/
    connect(ui->previewFPSSlider,
            &QAbstractSlider::valueChanged,
            this,
            &MainWindow::previewFPSChanged);
}

void MainWindow::setupCanvas()
{
    ui->previewFPSValue->setText(QString::number(ui->previewFPSSlider->value()));
    int size = model->getSize();
    QPointF point = QPointF(size/2, size/2);
    ui->canvasView->centerOn(point);

    ui->canvasView->setScene(canvasGraphic);
    ui->previewView->setScene(previewGraphic);
    ui->onionSkin->setScene(onionGraphic);

    ScaleCanvas();
    ScaleOnionSkin();
}

void MainWindow::ScaleCanvas()
{
    int size = model->getSize();
    switch(size) {
        case 8:
        ui->canvasView->scale(size*8, size*8);
        break;
        case 16:
        ui->canvasView->scale(size*2, size*2);
        break;
        case 32:
        ui->canvasView->scale(size/2, size/2);
        break;
        case 64:
        ui->canvasView->scale(size/8, size/8);
        break;
        case 128:
        ui->canvasView->scale(size/32, size/32);
        break;
    }
}

void MainWindow::ScaleOnionSkin()
{
    int size = model->getSize();
    switch(size) {
        case 8:
        ui->onionSkin->scale((float)size, (float)size);
        break;
        case 16:
        ui->onionSkin->scale((float)size, (float)size);
        break;
        case 32:
        ui->onionSkin->scale((float)size/4, (float)size/4);
        break;
        case 64:
        ui->onionSkin->scale((float)size/64, (float)size/64);
        break;
        case 128:
        ui->onionSkin->scale((float)size/1024, (float)size/1024);
        break;
    }
}


void MainWindow::on_color1Button_pressed()
{
    QColor color = QColorDialog::getColor(model->getLeftMouseColor(), this, "Color Picker");
    color.setAlpha(ui->brushTransparencySlider->value());
    if(color.isValid())
    {
        ui->color1Button->setStyleSheet("background-color: " + color.name());
        emit colorChanged(color, Qt::LeftButton);
    }
}


void MainWindow::on_color2Button_pressed()
{
    QColor color = QColorDialog::getColor(model->getRightMouseColor(), this, "Color Picker");
    color.setAlpha(ui->brushTransparencySlider->value());
    if(color.isValid())
    {
        ui->color2Button->setStyleSheet("background-color: " + color.name());
        emit colorChanged(color, Qt::RightButton);
    }
}

void MainWindow::updateCanvas(QPixmap updatedCanvas)
{
   canvasGraphic->clear();
   canvasGraphic->addPixmap(updatedCanvas);
}

void MainWindow::updatePreview(QPixmap updatedPreview)
{
   previewGraphic->clear();
   previewGraphic->addPixmap(updatedPreview);
}

void MainWindow::updateOnionSkin(QPixmap updatedOnionSkin)
{
   onionGraphic->clear();
   onionGraphic->addPixmap(updatedOnionSkin);
}

void MainWindow::updateFrameIndex(QString frameIndex)
{
   ui->FrameIndex->setText(frameIndex);
}

void MainWindow::updateNumberOfFrames(QString frameSize)
{
    ui->TotalFrames->setText(frameSize);
}

void MainWindow::on_penButton_clicked()
{
    highlightButton(PenButton);
    emit uiButtonPressed(PenButton);
}


void MainWindow::on_eraserButton_clicked()
{
    highlightButton(EraserButton);
    emit uiButtonPressed(EraserButton);
}


void MainWindow::on_bucketButton_clicked()
{
    highlightButton(PaintBucketButton);
    emit uiButtonPressed(PaintBucketButton);
}


void MainWindow::on_rectangleButton_clicked()
{
    highlightButton(RectangleToolButton);
    emit uiButtonPressed(RectangleToolButton);
}


void MainWindow::on_ellipseButton_clicked()
{
    highlightButton(EllipseToolButton);
    emit uiButtonPressed(EllipseToolButton);
}

void MainWindow::on_addFrameButton_pressed()
{
    emit uiButtonPressed(AddFrameButton);
}

void MainWindow::on_deleteFrameButton_pressed()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are You Sure?",
                                  "Are you sure you want to delete this frame?");
    if(reply == QMessageBox::Yes)
    {
        emit uiButtonPressed(DeleteFrameButton);
    }
}


void MainWindow::on_actionAbout_LeeSporkSprite_triggered()
{
    QFile file(":/txt/LeSporkEditor.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    QMessageBox::information(this, "About LeSporkEditor", in.readAll());
}


void MainWindow::on_actionLeSporkSprite_Help_triggered()
{
    QFile file(":/txt/LeSporkEditorHelp.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    QMessageBox::information(this, "LeSporkEditor Help", in.readAll());
}

void MainWindow::on_previousFrame_clicked()
{
    emit uiButtonPressed(PreviousFrameButton);
}

void MainWindow::on_nextFrame_clicked()
{
    emit uiButtonPressed(NextFrameButton);
}

void MainWindow::on_saveMenu_Action()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save Project","/home/.", "SSP (*.ssp)");
    emit saveProject(filePath);
}

void MainWindow::on_openMenu_Action()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Project","/home/.", "SSP (*.ssp)");
    Model* newModel = new Model(filePath);
    MainWindow* newMainWindow = new MainWindow(*newModel);
    newMainWindow->show();
}

void MainWindow::on_exportMenu_Action()
{
    qDebug() << "Export Triggured \n";
    QString filePath = QFileDialog::getSaveFileName(this, "Export PNG","/home/.", "PNG (*.png)");
    emit exportFame(filePath);
    qDebug() << filePath << " ::::: End \n";
}

void MainWindow::on_action8x8_triggered()
{
    Model* newModel = new Model(nullptr, 8);
    MainWindow* newMainWindow = new MainWindow(*newModel);
    newMainWindow->show();
}


void MainWindow::on_action16x16_triggered()
{
    Model* newModel = new Model(nullptr, 16);
    MainWindow* newMainWindow = new MainWindow(*newModel);
    newMainWindow->show();
}


void MainWindow::on_action32x32_triggered()
{
    Model* newModel = new Model(nullptr, 32);
    MainWindow* newMainWindow = new MainWindow(*newModel);
    newMainWindow->show();
}


void MainWindow::on_action64x64_triggered()
{
    Model* newModel = new Model(nullptr, 64);
    MainWindow* newMainWindow = new MainWindow(*newModel);
    newMainWindow->show();
}


void MainWindow::on_action128x128_triggered()
{
    Model* newModel = new Model(nullptr, 128);
    MainWindow* newMainWindow = new MainWindow(*newModel);
    newMainWindow->show();
}

void MainWindow::previewFPSChanged(int newValue)
{
    ui->previewFPSValue->setText(QString::number(newValue));
}

void MainWindow::highlightButton(UIButton buttonToHighlight)
{
    //Just reset every button's border, then give a border to the desired button
    ui->penButton->setStyleSheet(ui->penButton->styleSheet()+"border: none;");
    ui->eraserButton->setStyleSheet(ui->eraserButton->styleSheet()+"border: none;");
    ui->bucketButton->setStyleSheet(ui->bucketButton->styleSheet()+"border: none;");
    ui->rectangleButton->setStyleSheet(ui->rectangleButton->styleSheet()+"border: none;");
    ui->ellipseButton->setStyleSheet(ui->ellipseButton->styleSheet()+"border: none;");
    switch(buttonToHighlight)
    {
        case PenButton:
            ui->penButton->setStyleSheet(ui->penButton->styleSheet()+"border: 4px solid red;");
            break;
        case EraserButton:
            ui->eraserButton->setStyleSheet(ui->eraserButton->styleSheet()+"border: 4px solid red;");
            break;
        case PaintBucketButton:
            ui->bucketButton->setStyleSheet(ui->bucketButton->styleSheet()+"border: 4px solid red;");
            break;
        case RectangleToolButton:
            ui->rectangleButton->setStyleSheet(ui->rectangleButton->styleSheet()+"border: 4px solid red;");
            break;
        case EllipseToolButton:
            ui->ellipseButton->setStyleSheet(ui->ellipseButton->styleSheet()+"border: 4px solid red;");
            break;
        default:
            //OoOOooOoOOOo, spooky...
            break;
    }
}

void MainWindow::on_clearFrameButton_pressed()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are You Sure?",
                                  "Are you sure you want to clear this frame? All changes will be lost!");
    if(reply == QMessageBox::Yes)
    {
        emit uiButtonPressed(ClearFrameButton);
    }
}

