#ifndef CALCULATEPOLIS_H
#define CALCULATEPOLIS_H

#include <QStringList>
#include <QVector>
#include <QString>
#include <QPair>
#include <QMessageBox>

#include "window.h"

class CalculatePolis
{
public:
    CalculatePolis();

    void getPolis(QStringList& polis);
    void getVariables(QVector<QPair<QString, int>>& intVar, QVector<QPair<QString, double>>& realVar);
    void mainCalculation();
    QVector<QString> returnResult();

private:
    void calculatePolis(const QStringList& polis);

    ///calculate expression
    bool calculateExpression(QStringList &list);
    QString processingCalculation(const QString& first, const QString& second, const QString& sign);

    ///calculate loop
    void calculateLoop(const QString& string, const QStringList& loopList);
    void processingLoop(const QStringList& id, const QString& step, const QString& range, const QStringList& list);
    int getLoopMark(const QString& capture);

    ///calculate if
    void calculateIf(const QString& string, const QStringList& ifList);
    bool processingIf(const QString& first, const QString& second, const QString& sign);
    int getIfMark(const QString& capture);

    ///cin processing
    void cinProcessing(const QString &string);

    /// cout processing
    void coutProcessing(const QString& string);

    ///additional processing
    bool checkOnInt(const QString& string);
    bool checkOnReal(const QString& string);

    int getIntValue(const QString& string);
    double getRealValue(const QString& string);

    void updateIntVariables(const QString& string, int value);
    void updateRealVariables(const QString& string, double value);

    void clear();

private:
    QStringList _polis;
    QVector<QPair<QString, int>> _intVariables;
    QVector<QPair<QString, double>> _realVariables;

    QVector<QString> _result;

    Window* _wnd;
};

#endif // CALCULATEPOLIS_H
