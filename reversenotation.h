#ifndef REVERSENOTATION_H
#define REVERSENOTATION_H

#include <QVector>
#include <QString>
#include <QtAlgorithms>
#include <QMessageBox>
#include <QRegExp>
#include <QStack>
#include <QStringList>
#include "data.h"

class ReverseNotation
{
public:
    ReverseNotation();

    void setAllLexem(QVector<LexemData>& list);
    QStringList mainPolis();

private:
    int searchBrackets();
    void separateStrings();
    void separateOperators(const QStringList& list);

    QString expressionProcessing(const QString& string);
    QString logicalExpression(const QString& string);
    QString loopExpression(const QString& string);

    QString ifProcessing(const QStringList &list);
    QString loopProcessing(const QStringList &list);

    void clear();

private:
    QVector<LexemData> _lexems;
    QStringList _programStr;
    QStringList _poliz;
    QStack<int> _stack;
    int _i;
};

#endif // REVERSENOTATION_H
