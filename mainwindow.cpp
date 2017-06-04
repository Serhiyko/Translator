#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLexical_analyzer_triggered()
{
    _lexObj.mainLexemFunction();

    lexemsClear();

    _allLexem = _lexObj.getAllLexem();
    drawLexemTable(ui->tableWidget, _allLexem, "Lexem");

    _id = _lexObj.getAllId();
    drawTable(ui->tableWidget_2, _id, "ID");

    _const = _lexObj.getAllConst();
    drawTable(ui->tableWidget_3, _const, "CONST");

    separateTypes(_allLexem);
}

void MainWindow::lexemsClear()
{
    _allLexem.clear();
    ui->tableWidget->clear();
    _id.clear();
    ui->tableWidget_2->clear();
    _const.clear();
    ui->tableWidget_3->clear();
    _intVariables.clear();
    _realVariables.clear();
}

void MainWindow::drawLexemTable(QTableWidget *item, QVector<LexemData> &list, QString &&capture)
{
    item->setRowCount(list.count());
    item->setColumnCount(2);

    item->setHorizontalHeaderItem(0, new QTableWidgetItem(capture));
    item->setHorizontalHeaderItem(1, new QTableWidgetItem("Code"));
    for(int i = 0; i < item->rowCount(); ++i)
    {
        item->setItem(i, 0, new QTableWidgetItem(list.at(i).lexem));
        item->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(list.at(i).code)));
    }

    item->horizontalHeader()->setStretchLastSection(true);
    item->resizeRowsToContents();
}

void MainWindow::drawTable(QTableWidget *item, QVector<QPair<QString, int> > &list, QString &&capture)
{
    item->setRowCount(list.count());
    item->setColumnCount(2);

    item->setHorizontalHeaderItem(0, new QTableWidgetItem(capture));
    item->setHorizontalHeaderItem(1, new QTableWidgetItem("Code"));
    for(int i = 0; i < item->rowCount(); ++i)
    {
        item->setItem(i, 0, new QTableWidgetItem(list.at(i).first));
        item->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(list.at(i).second)));
    }

    item->horizontalHeader()->setStretchLastSection(true);
    item->resizeRowsToContents();
}

void MainWindow::on_actionSyntax_analyzer_triggered()
{
    if(!_allLexem.isEmpty())
    {
        _syntaxObj.setAllLexem(_allLexem);
        _syntaxObj.mainCheck();
    }
}

void MainWindow::separateTypes(const QVector<LexemData>& lexems)
{
    int i = 3;
    if(lexems[i].lexem == "int")
    {
        i++;
        while(lexems[i].lexem != ";")
        {
            if(lexems[i].lexem != ",")
            {
                _intVariables.push_back(qMakePair(lexems[i].lexem, 0));
            }
            i++;
        }
        i += 2;
        while(lexems[i].lexem != "{")
        {
            if(lexems[i].lexem != ",")
            {
                _realVariables.push_back(qMakePair(lexems[i].lexem, 0.0));
            }
            i++;
        }
    }
    else
    {
        i++;
        while(lexems[i].lexem != ";")
        {
            if(lexems[i].lexem != ",")
            {
                _realVariables.push_back(qMakePair(lexems[i].lexem, 0.0));
            }
            i++;
        }
        i += 2;
        while(lexems[i].lexem != "{")
        {
            if(lexems[i].lexem != ",")
            {
                _intVariables.push_back(qMakePair(lexems[i].lexem, 0));
            }
            i++;
        }
    }
}

void MainWindow::on_actionPolis_triggered()
{
    _polisList.clear();
    ui->textEdit->clear();
    _polis.setAllLexem(_lexObj.getAllLexem());
    _polisList = _polis.mainPolis();
    for(int i = 0; i < _polisList.count(); ++i)
    {
        QString str = _polisList[i];
        ui->textEdit->append(str);
    }
}

void MainWindow::on_actionCalculate_triggered()
{
    ui->textEdit_2->clear();
    _calcObj.getPolis(_polisList);
    _calcObj.getVariables(_intVariables, _realVariables);
    _calcObj.mainCalculation();
    QVector<QString> result =  _calcObj.returnResult();
    foreach (QString str, result)
    {
        ui->textEdit_2->append(str);
    }
}
