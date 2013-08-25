#include "mainview.h"
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

GraphicsView::GraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    rubberBand = NULL;
    scene = new QGraphicsScene();
    setScene(scene);
    imageItem = NULL;
    factor = 1;
    isCroping = false;
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (!pixmap.isNull() && isCroping) {
        origin = event->pos();
        if (!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (!pixmap.isNull() && isCroping)
        rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *)
{
    if (pixmap.isNull() || !isCroping) return;
    rubberBand->hide();
    QList<QGraphicsItem *> item = items(rubberBand->geometry());
    if (item.count() > 0) {
        QRectF rubberRect = imageItem->mapFromScene(mapToScene(rubberBand->geometry())).boundingRect();
        QRect intersectedRect = rubberRect.intersected(imageItem->boundingRect()).toRect();
        if (!intersectedRect.isEmpty()) {
            QString fn = QFileDialog::getSaveFileName(this, tr("Save"), "", tr("png file(*.png);;jpeg file(*.jpg *.jpeg);;tiff file(*.tiff);;bmp file(*.bmp)"));
            if (!fn.isEmpty()) {
                            QPixmap saveImg = imageItem->pixmap().copy(intersectedRect);
                            saveImg.save(fn);
            }
        }
    }
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if (!pixmap.isNull()) {
        double delta = event->delta() / 120.0;
        if (fabs(factor - 500) < 1e-5 && delta > 0) return;
        if (fabs(factor - 0.01) < 1e-5 && delta < 0) return;
        double zoomFactor = delta > 0 ? 1.05 : 0.95;
        factor *= zoomFactor;
        if (factor > 500) factor = 500;
        if (factor < 0.01) factor = 0.01;
        scale(zoomFactor, zoomFactor);
        emit zoomFactorChanged(factor);
    }
}

void GraphicsView::setPixmap(const QPixmap &p)
{
    pixmap = p;
    scene->setSceneRect(QRect(QPoint(), pixmap.size()));

    resetTransform();
    if (imageItem)
        imageItem->setPixmap(pixmap);
    else
        imageItem = scene->addPixmap(pixmap);

    emit imageSizeChanged(pixmap.size());
    factor = 1;
    emit zoomFactorChanged(factor);
}

double GraphicsView::zoomFactor() const
{
    return factor;
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *)
{
    emit toggleFullscreen(!parentWidget()->isFullScreen());
}

void GraphicsView::onToggleCrop(bool crop)
{
    isCroping = crop;
}

void GraphicsView::resizePixmap(const QSize &sz)
{
    setPixmap(pixmap.scaled(sz));
}

QPixmap GraphicsView::currentPixmap() const
{
    return pixmap;
}

