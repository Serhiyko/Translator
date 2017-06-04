#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>

class Window : public QDialog
{
    Q_OBJECT
public:
    Window(QWidget* parent = 0);

    void setLabelName(const QString& name);
    QString getVariable();

private: 
    QLabel *_label;
    QLineEdit * _line;
    QPushButton *_buttonUpdate;
    QPushButton *_buttonOk;
    QString _varData;

private slots:
    void setVariable();
};

#endif // WINDOW_H
