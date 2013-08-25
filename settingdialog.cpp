#include "settingdialog.h"
#include "ui_settingdialog.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(ui->dirBtn, SIGNAL(clicked()), this, SLOT(chooseSavePath()));
    ui->dirBtn->setText(QDir::currentPath());
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

QSize SettingDialog::pieceSize() const
{
    return QSize(ui->widthSpin->value(), ui->heightSpin->value());
}

void SettingDialog::chooseSavePath()
{
    QString dirName = QFileDialog::getExistingDirectory(this);
    if (!dirName.isEmpty()) {
        ui->dirBtn->setText(dirName);
    }
}

void SettingDialog::setPieceSize(const QSize &sz)
{
    ui->widthSpin->setValue(sz.width());
    ui->heightSpin->setValue(sz.height());
}

QString SettingDialog::saveDir()
{
    return ui->dirBtn->text();
}

void SettingDialog::setSaveDir(const QString &dir)
{
    ui->dirBtn->setText(dir);
}

void SettingDialog::setImageSize(const QSize& sz)
{
    ui->widthSpin->setMaximum(sz.width());
    ui->heightSpin->setMaximum(sz.height());
}
