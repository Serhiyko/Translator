#include "reversenotation.h"
#include <QDebug>

ReverseNotation::ReverseNotation()
{
    _i = 0;
}

void ReverseNotation::setAllLexem(QVector<LexemData> &list)
{
    clear();
    _lexems = qMove(list);
}

QStringList ReverseNotation::mainPolis()
{
    separateStrings();
    _poliz.clear();
    separateOperators(_programStr);
    return _poliz;
}

int ReverseNotation::searchBrackets()
{
    for(auto iter = _lexems.begin(); iter != _lexems.end(); ++iter)
    {
        if((*iter).lexem == "{")
        {
            return (*iter).stringNumber;
        }
    }
    return 0;
}

void ReverseNotation::separateStrings()
{
    int number = searchBrackets();
    if(number == 0)
    {
        QMessageBox msg;
        msg.setText("Some problem in searchBrackets!");
        msg.exec();
        exit(2);
    }

    auto iter = _lexems.end() - 1;
    int size = (*iter).stringNumber;
    for(int i = ++number; i < size; ++i)
    {
        QString tmp = "";
        for(int j = 0; j < _lexems.count(); ++j)
        {
            if(_lexems[j].stringNumber == i)
            {
                tmp += " " + _lexems[j].lexem;
            }
            else if(_lexems[j].stringNumber > i)
            {
                tmp.remove(0, 1);
                _programStr.append(tmp);
                tmp.clear();
                break;
            }
        }
    }
}

void ReverseNotation::separateOperators(const QStringList& list)
{
    QRegExp loop("\\b(for)(.*)");
    QRegExp ifExpr("\\b(if)(.*)");
    QRegExp cin("\\b(cin)(.*)");
    QRegExp cout("\\b(cout)(.*)");
    QString out;
    for(int i = 0; i < list.count(); ++i)
    {
        if(loop.exactMatch(list[i]))// loop
        {
            int count = 1;
            QStringList strList;
            for(int j = 0; j < list.count();)
            {
                strList.append(list[i]);
                i++;
                if(list.count() > 1 && loop.exactMatch(list[i])){ count++; }
                if(count > 1 && (list[i] == "rof ;" || list[i] == "rof")){ count--; }
                else if(list.count() > 1 && count == 1 && (list[i] == "rof ;" || list[i] == "rof"))
                {
                    strList.append(list[i]);
                    break;
                }
                else if(list.count() == 1){ break; }
            }
            out = loopProcessing(strList);
        }
        else if(ifExpr.exactMatch(list[i]))// if
        {
            int count = 1;
            QStringList strList;
            for(int j = 0; j < list.count();)
            {
                strList.append(list[i]);
                i++;
                if(list.count() > 1 && ifExpr.exactMatch(list[i])){ count++; }
                if(count > 1 && (list[i] == "fi ;" || list[i] == "fi")){ count--; }
                else if(list.count() > 1 && count == 1 && (list[i] == "fi ;" || list[i] == "fi"))
                {
                    strList.append(list[i]);
                    break;
                }
                else if(list.count() == 1){ break; }
            }
            out = ifProcessing(strList);
        }
        else if(cin.exactMatch(list[i]))
        {
            QStringList cinList;
            cinList = list[i].split(" ");
            out.clear();
            for(int j = 0; j < cinList.count(); ++j)
            {
                if(cinList[j] != "cin" && cinList[j] != ">>" && cinList[j] != ";")
                {
                    out += cinList[j] + " read ";
                }
            }
            out.remove(out.count() - 1, 1);
            _poliz.append(out);
        }
        else if(cout.exactMatch(list[i]))
        {
            QStringList coutList;
            coutList = list[i].split(" ");
            out.clear();
            for(int j = 0; j < coutList.count(); ++j)
            {
                if(coutList[j] != "cout" && coutList[j] != "<<" && coutList[j] != ";")
                {
                    out += coutList[j] + " write ";
                }
            }
            out.remove(out.count() - 1, 1);
            _poliz.append(out);
        }
        else if((list[i] == "fi" || list[i] == "fi ;") && list.count() == 1)
        {
            QStringList listFi;
            listFi.append(list[i]);
            out = ifProcessing(listFi);
        }
        else if((list[i] == "rof" || list[i] == "rof ;") && list.count() == 1)
        {
            QStringList listRof;
            listRof.append(list[i]);
            out = loopProcessing(listRof);
        }
        else
        {
            out = expressionProcessing(list[i]);
            _poliz.append(out);
        }
    }
}

QString ReverseNotation::expressionProcessing(const QString& string)
{
    QString outString = "";
    QStringList list = string.split(" ");
    QStack<QString>stack;
    for(int i = 0; i < list.count(); ++i)
    {
        if (list[i] == ";"){ break; }
        else if(list[i] != "+" && list[i] != "-" && list[i] != "*" && list[i] != "/" && list[i] != "(" && list[i] != ")" &&
                list[i] != "=")
        {
            outString += list[i] + " ";
        }
        else if(stack.isEmpty()){ stack.push(list[i]); }
        else if((stack.top() == "*" || stack.top() == "/") &&
                (list[i] == "+" || list[i] == "-" || list[i] == "*" || list[i] == "/"))
        {
            outString += stack.pop() + " ";
            i--;
        }
        else if((stack.top() == "+" || stack.top() == "-" || stack.top() == "@") && (list[i] == "+" || list[i] == "-"))
        {
            outString += stack.pop() + " ";
            i--;
        }
        else if(list[i] == "(")
        {
            stack.push(list[i]);
        }
        else if(list[i] == ")")
        {
            for(int j = 0; j <= stack.count(); ++j)
            {
                if(stack.top() != "("){ outString += stack.pop() + " "; }
                else{ stack.pop(); break; }
            }
        }
        else if(list[i] == "-" && (list[i - 1] == "=" || list[i - 1] == "("))
        {
            stack.push("@");
        }
        else
        {
            stack.push(list[i]);
        }
    }
    if(!stack.isEmpty())
    {
        /*for(int j = 0; j <= stack.count(); ++j)*/ while(!stack.isEmpty())
        {
            outString += stack.pop() + " ";
        }
    }
    outString.remove(outString.count() - 1, 1);
    return outString;
}

QString ReverseNotation::ifProcessing(const QStringList& list)
{
    QString outString = "";
    for(int i = 0; i < list.count(); ++i)
    {
        if((list[i] != "fi ;" && list[i] != "fi") && i == 0)
        {
            ++_i;
            _stack.push(_i);
            outString = logicalExpression(list[i]);
            outString += " m{" + QString::number(_i) + "} УПЛ";
            _poliz.append(outString);
            ++_i;
        }
        else if(list[i] == "fi ;" || list[i] == "fi")
        {
            outString = "m{" + QString::number(_stack.pop() + 1) + "}:";
            _poliz.append(outString);
            continue;
        }
        else
        {
            QStringList strList;
            strList.append(list[i]);
            separateOperators(strList);
        }
    }
    return outString;
}

QString ReverseNotation::logicalExpression(const QString &string)
{
    QString outString = "";
    QStringList list = string.split(" ");
    QStack<QString>stack;
    for(int i = 0; i < list.count(); ++i)
    {
        if(list[i] == "then"){ break; }
        else if(list[i] != "+" && list[i] != "-" && list[i] != "*" && list[i] != "/" && list[i] != "[" && list[i] != "]" &&
                list[i] != "=" && list[i] != "<" && list[i] != "<=" && list[i] != ">" && list[i] != ">=" && list[i] != "and" &&
                list[i] != "or" && list[i] != "not" && list[i] != "if" && list[i] != "then")
        {
            outString += list[i] + " ";
        }
        else if(stack.isEmpty() || list[i] == "if"){ stack.push(list[i]); }
        else if(list[i] == "["){ stack.push(list[i]); }
        else if((stack.top() == "*" || stack.top() == "/") &&
                (list[i] == "+" || list[i] == "-" || list[i] == "*" || list[i] == "/" || list[i] == ">" || list[i] == "<" ||
                 list[i] == ">=" || list[i] == "<=" || list[i] == "=" || list[i] == "!=" || list[i] == "or" || list[i] == "and" ||
                 list[i] == "not"))
        {
            outString += stack.pop() + " ";
            i--;
        }
        else if((stack.top() == "+" || stack.top() == "-" || stack.top() == "@") && (list[i] == "+" || list[i] == "-" ||
                list[i] == ">" || list[i] == "<" || list[i] == ">=" || list[i] == "<=" || list[i] == "=" || list[i] == "!=" ||
                list[i] == "or" || list[i] == "and" || list[i] == "not"))
        {
            outString += stack.pop() + " ";
            i--;
        }
        else if((stack.top() == ">" || stack.top() == "<" || stack.top() == "=" || stack.top() == "!=" || stack.top() == ">=" ||
                 stack.top() == "<=") && (list[i] == "or" || list[i] == "and" || list[i] == "not"))
        {
            outString += stack.pop() + " ";
            i--;
        }
        else if(stack.top() == "not" && (list[i] == "and" || list[i] == "not"))
        {
            outString += stack.pop() + " ";
            i--;
        }
        else if(stack.top() == "and" && list[i] == "or")
        {
            outString += stack.pop() + " ";
            i--;
        }
        else if(list[i] == "]")
        {
            for(int j = 0; j <= stack.count(); ++j)
            {
                if(stack.top() != "["){ outString += stack.pop() + " "; }
                else{ stack.pop(); break; }
            }
        }
        else if(list[i] == "-" && (list[i - 1] == "=" || list[i - 1] == "[" || list[i - 1] == "<" || list[i - 1] == "<="
                                   || list[i - 1] == "=" || list[i - 1] == ">" || list[i - 1] == ">=" || list[i - 1] == "or"
                                   || list[i - 1] == "and" || list[i - 1] == "not"))
        {
            stack.push("@");
        }
        else
        {
            stack.push(list[i]);
        }
    }
    for(int j = 0; j < stack.count(); ++j)
    {
        outString += stack.pop() + " ";
    }
    outString.remove(outString.count() - 1, 1);
    return outString;
}

QString ReverseNotation::loopProcessing(const QStringList &list)
{
    QString outString = "";
    for(int i = 0; i < list.count(); ++i)
    {
        if((list[i] != "rof ;" && list[i] != "rof") &&i == 0)
        {
            outString = loopExpression(list[i]);
            _poliz.append(outString);
        }
        else if(list[i] == "rof ;" || list[i] == "rof")
        {
            int frst=_stack.at(_stack.count() - 2);
            int scnd = _stack.pop();
            outString.clear();
            outString = "m{"+ QString::number(frst) +"} БП m{"+ QString::number(scnd) +"}:";
            _stack.pop();
            _poliz.append(outString);
            continue;
        }
        else
        {
            QStringList strList;
            strList.append(list[i]);
            separateOperators(strList);
        }
    }
    return outString;
}

QString ReverseNotation::loopExpression(const QString& string)
{
    QString outString = "";
    QStringList list = string.split(" ");

    for(int i = 0; i < list.count(); ++i)
    {
        if(list[i] == "for")
        {
            ++_i;
            i++;
            QString tmpStr = "";
            while(list[i] != "by")
            {
                tmpStr.append(list[i++] + " ");
            }
            tmpStr.remove(tmpStr.length() - 1, 1);
            outString += expressionProcessing(tmpStr);
        }
        if(list[i] == "by")
        {
            _stack.push(_i);
            outString += " r{j} 1 = m{" + QString::number(_i) + "}: r{j+1} ";
            i++;
            QString tmpStr = "";
            while(list[i] != "to")
            {
                tmpStr.append(list[i++] + " ");
            }
            tmpStr.remove(tmpStr.length() - 1,1);
            outString += expressionProcessing(tmpStr);
        }
        if(list[i] == "to")
        {
            ++_i;
            outString += " = r{j} 0 = m{" + QString::number(_i) + "} УПЛ id id r{j+1} + = m{" + QString::number(_i) +"}: r{j} 0 = id ";
            i++;
            QString tmpStr = "";
            while(list[i] != "do")
            {
                tmpStr.append(list[i++] + " ");
            }
            tmpStr.remove(tmpStr.length() - 1,1);
            outString += expressionProcessing(tmpStr);
        }
        if(list[i] == "do")
        {
            ++_i;
            _stack.push(_i);
            outString += " - r{j+1} * 0 <= m{" + QString::number(_i) +"} УПЛ";
        }
    }
    return outString;
}

void ReverseNotation::clear()
{
    _programStr.clear();
    _stack.clear();
    _poliz.clear();
    _lexems.clear();
    _i = 0;
}
