#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QVector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QRegExp>
#include <QPair>
#include "data.h"

class LexicalAnalyzer
{
public:
    LexicalAnalyzer();

    void mainLexemFunction();
    QVector<LexemData> &getAllLexem();
    const QVector<QPair<QString, int>> &getAllId();
    const QVector<QPair<QString, int> > &getAllConst();

private:
    void setServiceLexem();
    void readProgramText();

    void stringProcessing(unsigned int numberLine);
    void eraceElement();
    void separateOperator();
    void replaceElement(int iter, QVector<QString>& list, const QString replaceable);
    void separateVariables(unsigned int numberLine, const QString &inputString);
    bool compareVariables_Diff_Row();

    void separateLexem();
    void additionalSeparation(const QString& string, QVector<QPair<QString, int> > &list, int& position, bool check);

    void dublicateId();
    void writeLexem(int &numberLine);
    void record(const QString& string, QVector<QPair<QString, int>>& list, QVector<LexemData> &resultList, int &position,
                int& numberLine);
    void clear();

private:
    QVector<QString> _serviceLexem;
    QVector<QString> _programText;
    QVector<QString> _allLexem;
    QVector<QString> _addInt;
    QVector<QString> _addReal;

    int _lineNumber;
    int _posId;
    int _posConst;

    QVector<QPair<QString, int>> _ID;
    QVector<QPair<QString, int>> _CONST;
    QVector<QPair<QString, int>> _idAdd;
    QVector<LexemData> _resultLexem;

    QString _programName;
};

#endif // LEXICALANALYZER_H
