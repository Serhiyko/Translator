#include "window.h"
#include <QRegExp>
#include <QMessageBox>

Window::Window(QWidget *parent) : QDialog(parent)
{
    _label = new QLabel;
    _line = new QLineEdit;
    _buttonOk = new QPushButton("Ok");
    _buttonUpdate = new QPushButton("Update");

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(_label);
    layout->addWidget(_line);

    QHBoxLayout* layout1 = new QHBoxLayout;
    layout1->addWidget(_buttonOk);
    layout1->addWidget(_buttonUpdate);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(layout);
    vlayout->addLayout(layout1);

    connect(_buttonUpdate, SIGNAL(clicked()), this, SLOT(setVariable()));
    connect(_buttonOk, SIGNAL(clicked(bool)), this, SLOT(close()));

    setLayout(vlayout);
}

void Window::setLabelName(const QString& name)
{
    _label->setText(name);
}

QString Window::getVariable()
{
    _line->clear();
    return _varData;
}

void Window::setVariable()
{
    QRegExp tmp("\\d*\\.?\\d+$");
    if(_line->text() != "")
    {
        if(tmp.exactMatch(_line->text()))
        {
            _varData = _line->text();
        }
        else
        {
            QMessageBox msg;
            msg.setText("Invalid input variable, reenter variable please!");
            msg.exec();
        }
    }
}
