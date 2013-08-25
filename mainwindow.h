#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainview.h"

class QLabel;
class QRubberBand;
class QGraphicsScene;
class QGraphicsPixmapItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
public slots:
    void openPixmap();
    void changeSetting();
    void onImageSizeChanged(const QSize &);
    void onZoomFactorChanged(double);
    void onToggleFullScreen(bool);
    void leaveFullScreen();
    void onResizeImage();
private:
    Ui::MainWindow *ui;
    QSize pieceSize;

    QString lastSaveDir;
    QString lastOpenDir;
    QSize lastSize;

    GraphicsView *view;

    QLabel *imageSizeLabel;
    QLabel *zoomFactorLabel;

    QString filename;
    QSize imageSize;
    bool isModified;
};

#endif // MAINWINDOW_H
