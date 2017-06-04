#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "lexicalanalyzer.h"
#include "syntaxanalyzer.h"
#include "data.h"
#include "reversenotation.h"
#include "calculatepolis.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLexical_analyzer_triggered();

    void on_actionSyntax_analyzer_triggered();

    void on_actionPolis_triggered();

    void on_actionCalculate_triggered();

private:
    void drawTable(QTableWidget *item, QVector<QPair<QString, int>> &list, QString &&capture);
    void drawLexemTable(QTableWidget *item, QVector<LexemData> &list, QString &&capture);
    void separateTypes(const QVector<LexemData> &lexems);

    void lexemsClear();

private:
    Ui::MainWindow *ui;
    LexicalAnalyzer _lexObj;
    SyntaxAnalyzer _syntaxObj;
    ReverseNotation _polis;
    CalculatePolis _calcObj;

    QVector<LexemData> _allLexem;
    QVector<QPair<QString, int>> _id;
    QVector<QPair<QString, int>> _const;

    QStringList _polisList;
    QVector<QPair<QString, int>> _intVariables;
    QVector<QPair<QString, double>> _realVariables;
};

#endif // MAINWINDOW_H
