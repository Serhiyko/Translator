#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <QVector>
#include <QPair>
#include <QString>
#include <QMessageBox>
#include "data.h"

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer();

    void setAllLexem(QVector<LexemData>& list);
    void mainCheck();

private:
    bool checkLexem(int i, const int codeNumber);

    bool checkOnDeclarations();
    void checkListOfDeclaration(int &lexNumber);

    bool checkOnOperators(const int code);

    void checkCout(int &lexNumber);
    void checkCin(int &lexNumber);
    bool checkExpression(int &lexNumber, const int code);
    void checkLoop(int &lexNumber);
    void checkLogicalExpression(int& lexNumber);
    bool checkRelation(int& lexNumber, int& count);

    void clear();

private:
     QVector<LexemData> _lexems;

     int _lexNumber;
};

#endif // SYNTAXANALYZER_H
