#include "resizedialog.h"
#include "ui_resizedialog.h"

ResizeDialog::ResizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResizeDialog)
{
    ui->setupUi(this);
    ui->checkBox->setChecked(true);
}

ResizeDialog::~ResizeDialog()
{
    delete ui;
}

void ResizeDialog::setImageSize(const QSize &sz)
{
    ui->widthSpin->setValue(sz.width());
    ui->heigthSpin->setValue(sz.height());
    ratio = sz.width() / (double)sz.height();
    connect(ui->widthSpin, SIGNAL(valueChanged(int)), this, SLOT(onWidthChanged(int)));
    connect(ui->heigthSpin, SIGNAL(valueChanged(int)), this, SLOT(onHeightChanged(int)));
}

QSize ResizeDialog::imageSize() const
{
    return QSize(ui->widthSpin->value(), ui->heigthSpin->value());
}

void ResizeDialog::toggleKeepAspectRatio(int checkState)
{
    switch(checkState) {
    case Qt::Checked:
        connect(ui->widthSpin, SIGNAL(valueChanged(int)), this, SLOT(onWidthChanged(int)));
        connect(ui->heigthSpin, SIGNAL(valueChanged(int)), this, SLOT(onHeightChanged(int)));
        break;
    case Qt::Unchecked:
        ui->widthSpin->disconnect(this, SLOT(onWidthChanged(int)));
        ui->heigthSpin->disconnect(this, SLOT(onHeightChanged(int)));
        break;
    }
}

void ResizeDialog::onWidthChanged(int w)
{
    toggleKeepAspectRatio(Qt::Unchecked);
    ui->heigthSpin->setValue(int(w / ratio));
    toggleKeepAspectRatio(Qt::Checked);
}

void ResizeDialog::onHeightChanged(int h)
{
    toggleKeepAspectRatio(Qt::Unchecked);
    ui->widthSpin->setValue(int(h * ratio));
    toggleKeepAspectRatio(Qt::Checked);
}
