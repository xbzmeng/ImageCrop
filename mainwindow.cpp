#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingdialog.h"
#include "resizedialog.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openAction, SIGNAL(triggered()), this, SLOT(openPixmap()));
    connect(ui->sliceAction, SIGNAL(triggered()), this, SLOT(changeSetting()));
    connect(ui->quitAction, SIGNAL(triggered()), this, SLOT(leaveFullScreen()));
    connect(ui->resizeAction, SIGNAL(triggered()), this, SLOT(onResizeImage()));

    view = new GraphicsView();
    //view->setViewport(new QGLWidget);
    setCentralWidget(view);

    connect(view, SIGNAL(imageSizeChanged(const QSize &)), this, SLOT(onImageSizeChanged(const QSize &)));
    connect(view, SIGNAL(zoomFactorChanged(double)), this, SLOT(onZoomFactorChanged(double)));
    connect(view, SIGNAL(toggleFullscreen(bool)), this, SLOT(onToggleFullScreen(bool)));
    connect(ui->cropAction, SIGNAL(triggered(bool)), view, SLOT(onToggleCrop(bool)));

    QSettings settings("Jonathan", "Image Slice");
    pieceSize = settings.value("piece size", QSize(16, 16)).toSize();
    lastSaveDir = settings.value("save dir", QDir::currentPath()).toString();
    lastOpenDir = settings.value("open dir", QDir::currentPath()).toString();
    lastSize = settings.value("window size", QSize()).toSize();
    if (lastSize.width() > 200 && lastSize.height() > 200)
        resize(lastSize);

    imageSizeLabel = new QLabel();
    zoomFactorLabel = new QLabel();
    statusBar()->addPermanentWidget(imageSizeLabel);
    statusBar()->addPermanentWidget(zoomFactorLabel);

    setWindowIcon(QIcon(":/images/cut.png"));
    isModified = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openPixmap()
{
    QString fn = QFileDialog::getOpenFileName(this, QString("Open"), lastOpenDir, tr("Image file(*.png *.jpeg *.jpg *.tiff * *.bmp)"));
    if (!fn.isEmpty()) {
        QFileInfo fileInfo(fn);
        setWindowTitle(tr("Image Crop -- %1").arg(fileInfo.fileName()));
        lastOpenDir = fileInfo.path();
        ui->sliceAction->setEnabled(true);
        ui->cropAction->setEnabled(true);
        ui->resizeAction->setEnabled(true);
        view->setPixmap(QPixmap(fn));
        statusBar()->showMessage(tr("Image load sucessfully"));
        filename = fn;
        isModified = false;
    }
}

void MainWindow::changeSetting()
{
    SettingDialog dlg(this);
    dlg.setSaveDir(lastSaveDir);
    QPixmap pixmap = view->currentPixmap();
    dlg.setImageSize(pixmap.size());
    dlg.setPieceSize(pieceSize);
    if (dlg.exec() == QDialog::Accepted)
    {
        pieceSize = dlg.pieceSize();

        const int pieceWidth = pieceSize.width();
        const int pieceHeight = pieceSize.height();
        const int rows = ceil(pixmap.height() / (double)pieceHeight);
        const int cols = ceil(pixmap.width() / (double)pieceWidth);
        QString fn = dlg.saveDir();
        lastSaveDir = fn;
        for (int i = 0; i < cols; ++i)
            for (int j = 0; j < rows; ++j) {
                QPixmap slice = pixmap.copy(i * pieceWidth, j * pieceHeight, pieceWidth, pieceHeight);
                slice.save(QString("%1/%2.png").arg(fn).arg(j * cols + i + 1));
            }
        QMessageBox::information(this, tr("Notice"), tr("Slicing image sucessfully."));
    }
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    if (isModified) {
        QMessageBox msgBox(this);
        QAbstractButton *saveBtn = msgBox.addButton(QMessageBox::Save);
        QAbstractButton *saveAsBtn = msgBox.addButton(tr("Save As..."), QMessageBox::ActionRole);
        QAbstractButton *cancelBtn = msgBox.addButton(QMessageBox::Cancel);
        msgBox.addButton(QMessageBox::Discard);
        msgBox.setText(tr("Image has unsaved change. Whether to save?"));
        msgBox.setWindowTitle(tr("Warning"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        QAbstractButton *clickedButton = msgBox.clickedButton();
        if (clickedButton == saveBtn) {
            view->currentPixmap().save(filename);
        } else if (clickedButton  == cancelBtn) {
            evt->ignore();
            return;
        } else if (clickedButton == saveAsBtn) {
            QString fn = QFileDialog::getSaveFileName(this, tr("Save"), "", tr("png file(*.png);;jpeg file(*.jpg *.jpeg);;tiff file(*.tiff);;bmp file(*.bmp)"));
            if (!fn.isEmpty())
                view->currentPixmap().save(fn);
        }
    }
    evt->accept();
    QSettings settings("Jonathan", "Image Slice");
    settings.setValue("piece size", pieceSize);
    settings.setValue("save dir", lastSaveDir);
    settings.setValue("open dir", lastOpenDir);
    settings.setValue("window size", size());
}

void MainWindow::onImageSizeChanged(const QSize& sz)
{
    imageSizeLabel->setText(tr("image size: %1 x %2").arg(sz.width()).arg(sz.height()));
    imageSize = sz;
}

void MainWindow::onZoomFactorChanged(double zoomFactor)
{
    if (fabs(zoomFactor - 1) < 1e-5)
        zoomFactorLabel->setText("");
    else
        zoomFactorLabel->setText(tr("zoom: %1%").arg(zoomFactor * 100, 0, 'f', zoomFactor > 100 ? 0 : 2));
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *)
{
    onToggleFullScreen(!isFullScreen());
}

void MainWindow::onToggleFullScreen(bool fs)
{
    if (fs) {
        showFullScreen();
        ui->quitAction->setVisible(true);
    } else {
        leaveFullScreen();
    }
}

void MainWindow::leaveFullScreen()
{
    ui->quitAction->setVisible(false);
    showNormal();
}

void MainWindow::onResizeImage()
{
    ResizeDialog dlg(this);
    dlg.setImageSize(imageSize);
    if (dlg.exec() == QDialog::Accepted) {
        QSize newSize = dlg.imageSize();
        view->resizePixmap(newSize);
        if (!isModified && imageSize != newSize) {
            setWindowTitle(tr("%1 [*unsaved]").arg(windowTitle()));
            isModified = true;
        }
    }
}
