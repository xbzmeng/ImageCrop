#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class QRubberBand;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget * parent = NULL);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void setPixmap(const QPixmap &pixmap);
    void resizePixmap(const QSize &sz);
    QPixmap currentPixmap() const;
    double zoomFactor() const;
signals:
    void imageSizeChanged(const QSize &);
    void zoomFactorChanged(double );
    void toggleFullscreen(bool);
public slots:
    void onToggleCrop(bool);
private:
    QPoint origin;
    QRubberBand *rubberBand;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *imageItem;

    double factor;
    bool isCroping;
    QPixmap pixmap;
};

#endif // GRAPHICSVIEW_H
