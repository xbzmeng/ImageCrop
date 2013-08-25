#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();
    QSize pieceSize() const;
    void setPieceSize(const QSize &);
    QString saveDir();
    void setSaveDir(const QString &);
    void setImageSize(const QSize &);
public slots:
    void chooseSavePath();
    
private:
    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H
