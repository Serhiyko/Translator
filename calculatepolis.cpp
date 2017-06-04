#include "calculatepolis.h"

CalculatePolis::CalculatePolis()
{
    _wnd = new Window();
}

void CalculatePolis::clear()
{
    _polis.clear();
    _intVariables.clear();
    _realVariables.clear();
    _result.clear();
}

void CalculatePolis::getPolis(QStringList& polis)
{
    clear();
    _polis = qMove(polis);
}

void CalculatePolis::getVariables(QVector<QPair<QString, int>> &intVar, QVector<QPair<QString, double>> &realVar)
{
    _intVariables = qMove(intVar);
    _realVariables = qMove(realVar);
}

void CalculatePolis::mainCalculation()
{
    calculatePolis(_polis);
}

void CalculatePolis::calculatePolis(const QStringList &polis)
{
    QRegExp loop("(.+)\\Wj\\W 1 = m(.+)");
    QRegExp ifExpression("(.+)УПЛ$");
    QRegExp cin("(.+)read");
    QRegExp cout("(.+)write");
    for(int i = 0; i < polis.count(); ++i)
    {
        if(polis[i].at(polis[i].count() - 1) == '=')// expression
        {
            QStringList list = polis[i].split(' ');
            calculateExpression(list);
            if(checkOnInt(list[0]))
            {
                updateIntVariables(list[0], list[1].toInt());
            }
            else if(checkOnReal(list[0]))
            {
                updateRealVariables(list[0], list[1].toDouble());
            }
        }
        else if (loop.exactMatch(polis[i]))// loop
        {
            int mark = getLoopMark(polis[i]);
            QRegExp loopmark("^m\\W" + QString::number(mark) + "\\W(.+)");
            int j = ++i;
            i--;
            QStringList list;
            while(!loopmark.exactMatch(polis[j]))
            {
                list.append(polis[j]);
                j++;
            }
            calculateLoop(polis[i], list);
            i = j;
        }
        else if(ifExpression.exactMatch(polis[i]))// if
        {
            int mark = getIfMark(polis[i]);
            QRegExp ifMark("m\\W" + QString::number(mark + 1) + "\\W:");
            int j = ++i;
            i--;
            QStringList list;
            while(!ifMark.exactMatch(polis[j]))
            {
                list.append(polis[j]);
                j++;
            }
            calculateIf(polis[i], list);
            i = j;
        }
        else if(cin.exactMatch(polis[i]))// cin
        {
            cinProcessing(polis[i]);
        }
        else if(cout.exactMatch(polis[i]))// cout
        {
            coutProcessing(polis[i]);
        }
    }
}

QVector<QString> CalculatePolis::returnResult()
{
    return _result;
}

///calculate expression
bool CalculatePolis::calculateExpression(QStringList& list)
{
    bool check = false;
    QRegExp tmpConst("\\-?\\d*\\.?\\d+$");
    for(int i = 0; i < list.count() - 2; ++i)
    {
        if((list[i] != "+" && list[i] != "-" && list[i] != "*" && list[i] != "/" && list[i] != "@" /**/
            && list[i] != ">" && list[i] != "<" && list[i] != ">=" && list[i] != "<=" && list[i] != "==" && list[i] != "!=") &&
           (list[i + 1] != "+" && list[i + 1] != "-" && list[i + 1] != "*" && list[i + 1] != "/" /**/ && list[i + 1] != ">"
            && list[i + 1] != ">=" && list[i + 1] != "<" && list[i + 1] != "<=" && list[i + 1] != "==" && list[i + 1] != "!="))
        {
            if(tmpConst.exactMatch(list[i]) && list[i + 1] == "@")
            {
                QString str = list[i];
                list.removeAt(i + 1);
                list.replace(i, "-" + str);
                i = 0;
            }
            else if((checkOnInt(list[i])) && list[i + 1] == "@")
            {
                list.removeAt(i + 1);
                updateIntVariables(list[i], -1);
                i = 0;
            }
            else if((checkOnReal(list[i])) && list[i + 1] == "@")
            {
                list.removeAt(i + 1);
                updateRealVariables(list[i], -1);
                i = 0;
            }
            else if(list[i + 2] == "+")
            {
                QString str = processingCalculation(list[i], list[i + 1], list[i + 2]);
                list.removeAt(i + 2);
                list.removeAt(i + 1);
                list.replace(i, str);
                i = 0;
            }
            else if(list[i + 2] == "-")
            {
                QString str = processingCalculation(list[i], list[i + 1], list[i + 2]);
                list.removeAt(i + 2);
                list.removeAt(i + 1);
                list.replace(i, str);
                i = 0;
            }
            else if(list[i + 2] == "*")
            {
                QString str = processingCalculation(list[i], list[i + 1], list[i + 2]);
                list.removeAt(i + 2);
                list.removeAt(i + 1);
                list.replace(i, str);
                i = 0;
            }
            else if(list[i + 2] == "/")
            {
                QString str = processingCalculation(list[i], list[i + 1], list[i + 2]);
                list.removeAt(i + 2);
                list.removeAt(i + 1);
                list.replace(i, str);
                i = 0;
            }
            else if(list[i + 2] == ">")
            {
                check = processingIf(list[i], list[i + 1], ">");
                list.removeAt(i + 2);
                list.removeAt(i + 1);
                if(check){ list.replace(i, "1"); }
                else{ list.replace(i, "0"); }
                i = 0;
            }
            else if(list[i + 2] == "<")
            {
                check = processingIf(list[i], list[i + 1], "<");
                list.removeAt(i + 2);
                list.removeAt(i + 1);
                if(check){ list.replace(i, "1"); }
                else{ list.replace(i, "0"); }
                i = 0;
            }
            else if(list[i + 2] == "==")
            {
                check = processingIf(list[i], list[i + 1], "==");
                list.removeAt(i + 2);
                list.removeAt(i + 1);
                if(check){ list.replace(i, "1"); }
                else{ list.replace(i, "0"); }
                i = 0;
            }
            else if(list[i + 2] == "!=")
            {
                check = processingIf(list[i], list[i + 1], "!=");
                list.removeAt(i + 2);
                list.removeAt(i + 1);
                if(check){ list.replace(i, "1"); }
                else{ list.replace(i, "0"); }
                i = 0;
            }
            else if(list[i + 2] == "<=")
            {
                check = processingIf(list[i], list[i + 1], "<=");
                list.removeAt(i + 2);
                list.removeAt(i + 1);
                if(check){ list.replace(i, "1"); }
                else{ list.replace(i, "0"); }
                i = 0;
            }
            else if(list[i + 2] == ">=")
            {
                check = processingIf(list[i], list[i + 1], ">=");
                list.removeAt(i + 2);
                list.removeAt(i + 1);
                if(check){ list.replace(i, "1"); }
                else{ list.replace(i, "0"); }
                i = 0;
            }
            else if(list[i + 2] == "and")
            {
                if(list[i] == "1" && list[i + 1] == "1"){ check = true; }
                else{ check = false; }
            }
            else if(list[i + 2] == "or")
            {
                if(list[i] == "0" && list[i + 1] == "0"){ check = false; }
                else{ check = true; }
            }
            else if(list[i + 2] == "not")
            {
                if(list[i + 1] == "0"){ check = true; }
                else{ check = false; }
            }
        }
    }

    if(list.count() == 3 && (list.last() == "and" || list.last() == "or"))
    {
        if(list[2] == "and")
        {
            if(list[0] == "1" && list[1] == "1"){ check = true; }
            else{ check = false; }
        }
        else if(list[2] == "or")
        {
            if(list[0] == "0" && list[1] == "0"){ check = false; }
            else{ check = true; }
        }
    }
    else if ( list.count() == 2 && list.last() == "not")
    {
        if(list.first() == "0"){ check = true; }
        else{ check = false; }
    }
    return check;
}

QString CalculatePolis::processingCalculation(const QString& first, const QString& second, const QString& sign)
{
    int firstIntNumber = 0;
    double firstDoubleNumber = 0;
    int secondIntNumber = 0;
    double secondDoubleNumber = 0;

    QRegExp tmpConst("\\-?\\d*\\.?\\d+$");
    //check constant
    if(tmpConst.exactMatch(first))
    {
        QRegExp tmpInt("\\-?\\d*");
        QRegExp tmpReal("\\-?\\d*\\.?\\d+$");
        if(tmpInt.exactMatch(first)){ firstIntNumber = first.toInt(); }
        else if(tmpReal.exactMatch(first)){ firstDoubleNumber = first.toDouble(); }
        else
        {
            QMessageBox msg;
            msg.setText("Error: first const in calculations!");
            msg.exec();
            exit(2);
        }
    }
    if(tmpConst.exactMatch(second))
    {
        QRegExp tmpInt("\\-?\\d*");
        QRegExp tmpReal("\\-?\\d*\\.?\\d+$");
        if(tmpInt.exactMatch(second)){ secondIntNumber = second.toInt(); }
        else if(tmpReal.exactMatch(second)){ secondDoubleNumber = second.toDouble(); }
        else
        {
            QMessageBox msg;
            msg.setText("Error const in calculations!");
            msg.exec();
            exit(2);
        }
    }

    //check id
    if(checkOnInt(first)){ firstIntNumber = getIntValue(first); }
    if(checkOnReal(first)){ firstDoubleNumber = getRealValue(first); }
    if(checkOnInt(second)){ secondIntNumber = getIntValue(second); }
    if(checkOnReal(second)){ secondDoubleNumber = getRealValue(second); }

    if(sign == "+")
    {
        if(firstIntNumber == 0)//first double
        {
            if(secondIntNumber == 0)//second double
            {
                return QString::number(firstDoubleNumber + secondDoubleNumber);
            }
            else
            {
                return QString::number(firstDoubleNumber + secondIntNumber);
            }
        }
        else
        {
            if(secondIntNumber == 0)//second double
            {
                return QString::number(firstIntNumber + secondDoubleNumber);
            }
            else
            {
                return QString::number(firstIntNumber + secondIntNumber);
            }
        }
    }
    if(sign == "-")
    {
        if(firstIntNumber == 0)//first double
        {
            if(secondIntNumber == 0)//second double
            {
                return QString::number(firstDoubleNumber - secondDoubleNumber);
            }
            else
            {
                return QString::number(firstDoubleNumber - secondIntNumber);
            }
        }
        else
        {
            if(secondIntNumber == 0)//second double
            {
                return QString::number(firstIntNumber - secondDoubleNumber);
            }
            else
            {
                return QString::number(firstIntNumber - secondIntNumber);
            }
        }
    }
    if(sign == "*")
    {
        if(firstIntNumber == 0)//first double
        {
            if(secondIntNumber == 0)//second double
            {
                return QString::number(firstDoubleNumber * secondDoubleNumber);
            }
            else
            {
                return QString::number(firstDoubleNumber * secondIntNumber);
            }
        }
        else
        {
            if(secondIntNumber == 0)//second double
            {
                return QString::number(firstIntNumber * secondDoubleNumber);
            }
            else
            {
                return QString::number(firstIntNumber * secondIntNumber);
            }
        }
    }
    if(sign == "/")
    {
        if(firstIntNumber == 0)//first double
        {
            if(secondIntNumber == 0)//second double
            {
                return QString::number(firstDoubleNumber / secondDoubleNumber);
            }
            else
            {
                return QString::number(firstDoubleNumber / secondIntNumber);
            }
        }
        else
        {
            if(secondIntNumber == 0)//second double
            {
                return QString::number(firstIntNumber / secondDoubleNumber);
            }
            else
            {
                return QString::number(firstIntNumber / secondIntNumber);
            }
        }
    }
    return "";
}

///calculate loop
void CalculatePolis::calculateLoop(const QString& string, const QStringList &loopList)
{
    ////////Calculate id of the loop////////////////
    QStringList list = string.split(' ');
    QString expression = "";
    for(int i = 0; i < list.count() - 1; ++i)
    {
        if(list[i] != "r{j}" && list[i + 1] != "1")
        {
            expression += list[i] + " ";
        }
        else{ break; }
    }
    expression.remove(expression.count() - 1, 1);
    QStringList id = expression.split(' ');
    calculateExpression(id);

    //////////////Calculate Step////////////////////
    QString step = "";
    int iterLast = 0;
    int iterFirst = 0;
    for(int i = 0; i < list.count() - 2; ++i)
    {
        if(list[i] == "=" && list[i + 1] == "r{j}" && list[i + 2] == "0")
        {
            iterLast = i;
            while(list[i] != "r{j+1}")
            {
                i--;
            }
            iterFirst = ++i;
            if(iterFirst < iterLast)
            {
                step += list[i] + " ";
            }
            break;
        }
    }
    step.remove(step.count() - 1, 1);
    step = "step " + step + " =";
    QStringList stepList = step.split(' ');
    calculateExpression(stepList);

    //////////////Calculate range of the loop////////////////
    QString lastRange = "";
    iterLast = 0;
    iterFirst = 0;
    for(int i = 0; i < list.count() - 2; ++i)
    {
        if(list[i] == "-" && list[i + 1] == "r{j+1}")
        {
            iterLast = i;
            while(list[i] != "id")
            {
                i--;
            }
            iterFirst = ++i;
            if(iterFirst < iterLast)
            {
                lastRange += list[i] + " ";
            }
            break;
        }
    }
    lastRange.remove(lastRange.count() - 1, 1);
    lastRange = "range " + lastRange + " =";
    QStringList rangeList = lastRange.split(' ');
    calculateExpression(rangeList);

    processingLoop(id, stepList[1], rangeList[1], loopList);
}

void CalculatePolis::processingLoop(const QStringList& id, const QString &step, const QString &range,
                                    const QStringList &list)
{
    if(checkOnInt(id[0]))
    {
        updateIntVariables(id[0], id[1].toInt());
        for(int i = getIntValue(id[0]); i < range.toInt(); i = i + step.toInt())
        {
            updateIntVariables(id[0], i);
            calculatePolis(list);
        }
    }
    else if(checkOnReal(id[0]))
    {
        updateRealVariables(id[0], id[1].toDouble());
        for(double i = getRealValue(id[0]); i < range.toDouble(); i = i + step.toDouble())
        {
            updateRealVariables(id[0], i);
            calculatePolis(list);
        }
    }

}

int CalculatePolis::getLoopMark(const QString& capture)
{
    QStringList list = capture.split(' ');
    QString mark = "";
    for(int i = 0; i < list.count() - 1; ++i)
    {
        if(list[i] == "r{j}" && list[i + 1] == "1")
        {
            mark = list[i + 3];
            break;
        }
    }
    mark.remove(0, 2);
    mark.remove(mark.count() - 2, 2);

    return mark.toInt();
}

///calculate if
void CalculatePolis::calculateIf(const QString &string, const QStringList &ifList)
{
    QStringList list = string.split(' ');
    list.removeLast();
    list.removeLast();
    if(calculateExpression(list))
    {
        calculatePolis(ifList);
    }
}

bool CalculatePolis::processingIf(const QString &first, const QString &second, const QString &sign)
{
    int firstIntNumber = 0;
    double firstDoubleNumber = 0;
    int secondIntNumber = 0;
    double secondDoubleNumber = 0;

    QRegExp tmpConst("\\-?\\d*\\.?\\d+$");
    //check constant
    if(tmpConst.exactMatch(first))
    {
        QRegExp tmpInt("\\-?\\d*");
        QRegExp tmpReal("\\-?\\d*\\.?\\d+$");
        if(tmpInt.exactMatch(first)){ firstIntNumber = first.toInt(); }
        else if(tmpReal.exactMatch(first)){ firstDoubleNumber = first.toDouble(); }
        else
        {
            QMessageBox msg;
            msg.setText("Error: first const in calculations!");
            msg.exec();
            exit(2);
        }
    }
    if(tmpConst.exactMatch(second))
    {
        QRegExp tmpInt("\\-?\\d*");
        QRegExp tmpReal("\\-?\\d*\\.?\\d+$");
        if(tmpInt.exactMatch(second)){ secondIntNumber = second.toInt(); }
        else if(tmpReal.exactMatch(second)){ secondDoubleNumber = second.toDouble(); }
        else
        {
            QMessageBox msg;
            msg.setText("Error const in calculations!");
            msg.exec();
            exit(2);
        }
    }

    //check id
    if(checkOnInt(first)){ firstIntNumber = getIntValue(first); }
    if(checkOnReal(first)){ firstDoubleNumber = getRealValue(first); }
    if(checkOnInt(second)){ secondIntNumber = getIntValue(second); }
    if(checkOnReal(second)){ secondDoubleNumber = getRealValue(second); }

    if(sign == ">")
    {
        if(firstIntNumber == 0)//first double
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstDoubleNumber > secondDoubleNumber) { return true; }
            }
            else
            {
                if(firstDoubleNumber > secondIntNumber){ return true; }
            }
        }
        else
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstIntNumber > secondDoubleNumber){ return true; }
            }
            else
            {
                if(firstIntNumber > secondIntNumber){ return true; }
            }
        }
    }
    if(sign == "<")
    {
        if(firstIntNumber == 0)//first double
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstDoubleNumber < secondDoubleNumber) { return true; }
            }
            else
            {
                if(firstDoubleNumber < secondIntNumber){ return true; }
            }
        }
        else
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstIntNumber < secondDoubleNumber){ return true; }
            }
            else
            {
                if(firstIntNumber < secondIntNumber){ return true; }
            }
        }
    }
    if(sign == "==")
    {
        if(firstIntNumber == 0)//first double
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstDoubleNumber == secondDoubleNumber) { return true; }
            }
            else
            {
                if(firstDoubleNumber == secondIntNumber){ return true; }
            }
        }
        else
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstIntNumber == secondDoubleNumber){ return true; }
            }
            else
            {
                if(firstIntNumber == secondIntNumber){ return true; }
            }
        }
    }
    if(sign == "!=")
    {
        if(firstIntNumber == 0)//first double
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstDoubleNumber != secondDoubleNumber) { return true; }
            }
            else
            {
                if(firstDoubleNumber != secondIntNumber){ return true; }
            }
        }
        else
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstIntNumber != secondDoubleNumber){ return true; }
            }
            else
            {
                if(firstIntNumber != secondIntNumber){ return true; }
            }
        }
    }
    if(sign == ">=")
    {
        if(firstIntNumber == 0)//first double
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstDoubleNumber >= secondDoubleNumber) { return true; }
            }
            else
            {
                if(firstDoubleNumber >= secondIntNumber){ return true; }
            }
        }
        else
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstIntNumber >= secondDoubleNumber){ return true; }
            }
            else
            {
                if(firstIntNumber >= secondIntNumber){ return true; }
            }
        }
    }
    if(sign == "<=")
    {
        if(firstIntNumber == 0)//first double
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstDoubleNumber <= secondDoubleNumber) { return true; }
            }
            else
            {
                if(firstDoubleNumber <= secondIntNumber){ return true; }
            }
        }
        else
        {
            if(secondIntNumber == 0)//second double
            {
                if(firstIntNumber <= secondDoubleNumber){ return true; }
            }
            else
            {
                if(firstIntNumber <= secondIntNumber){ return true; }
            }
        }
    }
    return false;
}

int CalculatePolis::getIfMark(const QString &capture)
{
    QStringList list = capture.split(' ');
    QString mark = list.at(list.count() - 2);
    mark.remove(0, 2);
    mark.remove(mark.count() - 1, 1);

    return mark.toInt();
}

///cin processing
void CalculatePolis::cinProcessing(const QString& string)
{
    QStringList list = string.split(' ');

    foreach (QString str, list)
    {
        if(str != "read")
        {
            if(checkOnInt(str))
            {
                _wnd->setLabelName("Enter int " + str + ":");
                _wnd->setModal(true);
                _wnd->exec();
                int var = _wnd->getVariable().toInt();
                updateIntVariables(str, var);
            }
            else if(checkOnReal(str))
            {
                _wnd->setLabelName("Enter real " + str + ":");
                _wnd->setModal(true);
                _wnd->exec();
                double var = _wnd->getVariable().toDouble();
                updateRealVariables(str, var);
            }
        }
    }
}

/// cout processing
void CalculatePolis::coutProcessing(const QString &string)
{
    QStringList list = string.split(" ");
    foreach (QString str, list)
    {
        if(str != "write")
        {
            if(checkOnInt(str))
            {
                int res = getIntValue(str);
                _result.push_back(str + " = " + QString::number(res));
            }
            else if(checkOnReal(str))
            {
                double res = getRealValue(str);
                _result.push_back(str + " = " + QString::number(res));
            }
        }
    }
}

///additional expression
bool CalculatePolis::checkOnInt(const QString& string)
{
    for(int i = 0; i < _intVariables.count(); ++i)
    {
        if(_intVariables[i].first == string){ return true; }
    }

    return false;
}

bool CalculatePolis::checkOnReal(const QString& string)
{
    for(int i = 0; i < _realVariables.count(); ++i)
    {
        if(_realVariables[i].first == string){ return true; }
    }

    return false;
}

void CalculatePolis::updateIntVariables(const QString& string, int value)
{
    for(int i = 0; i < _intVariables.count(); ++i)
    {
        if(_intVariables[i].first == string)
        {
            if(value == -1)
            {
                _intVariables[i].second = -_intVariables[i].second;
            }
            else{ _intVariables[i].second = value; }
        }
    }
}

void CalculatePolis::updateRealVariables(const QString& string, double value)
{
    for(int i = 0; i < _realVariables.count(); ++i)
    {
        if(_realVariables[i].first == string)
        {
            if(value == -1)
            {
                _realVariables[i].second = -_realVariables[i].second;
            }
            else{ _realVariables[i].second = value; }
        }
    }
}

int CalculatePolis::getIntValue(const QString& string)
{
    for(int i = 0; i < _intVariables.count(); ++i)
    {
        if(_intVariables[i].first == string){ return _intVariables[i].second; }
    }

    return 0;
}

double CalculatePolis::getRealValue(const QString& string)
{
    for(int i = 0; i < _realVariables.count(); ++i)
    {
        if(_realVariables[i].first == string){ return _realVariables[i].second; }
    }

    return 0;
}
