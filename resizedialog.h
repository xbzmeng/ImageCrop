#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>

namespace Ui {
class ResizeDialog;
}

class ResizeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ResizeDialog(QWidget *parent = 0);
    ~ResizeDialog();
    void setImageSize(const QSize &);
    QSize imageSize() const;
public slots:
    void toggleKeepAspectRatio(int);
    void onWidthChanged(int);
    void onHeightChanged(int);
private:
    Ui::ResizeDialog *ui;
    double ratio;
};

#endif // RESIZEDIALOG_H
