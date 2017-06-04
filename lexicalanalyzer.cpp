#include "lexicalanalyzer.h"

LexicalAnalyzer::LexicalAnalyzer() : _lineNumber(0), _posId(1), _posConst(-1)
{
}

void LexicalAnalyzer::setServiceLexem()
{
    _serviceLexem = {"Program", "var", "int", "real", "cout", "cin", "for", "by", "to", "do", "rof", "if", "then", "fi", "{", "}",
                   "<<", ">>", ";", ",", "+", "-", "*", "/", "(", ")", "=", ">", "<", "or", "and", "not", "==", "!=", ">=", "<=",
                   "[", "]", "id", "const"};
}

void LexicalAnalyzer::readProgramText()
{
    QFile file("C:/Users/Serhiy/Documents/QT/Cursova_SAPR/program.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox msg;
        msg.setText("File is not opened!");
        msg.exec();
        exit(3);
    }

    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        _programText.append(line);
    }
    file.close();
}

void LexicalAnalyzer::stringProcessing(unsigned int numberLine)
{
    QString _inputString = _programText.at(numberLine);
    QString localStr = "";

    for(int i = 0; i < _inputString.count(); ++i)
    {
        if(_inputString.at(i) != '\r' && _inputString.at(i) != '\n' && _inputString.at(i) != ' ')
        {
            if(_inputString.at(i) != ',' && _inputString.at(i) != ';' && _inputString.at(i) != '=' && _inputString.at(i) != '*'
                    && _inputString.at(i) != '/' && _inputString.at(i) != '+' && _inputString.at(i) != '-' && _inputString.at(i) != '('
                    && _inputString.at(i) != ')' && _inputString.at(i) != '<' && _inputString.at(i) != '>' && _inputString.at(i) != '!'
                    && _inputString.at(i) != '{' && _inputString.at(i) != '}' && _inputString.at(i) != '[' && _inputString.at(i) != ']')
            {
                localStr += _inputString.at(i);
            }
            else
            {
                _allLexem.push_back(localStr);
                localStr.clear();
                localStr += _inputString.at(i);
                _allLexem.append(localStr);
                localStr.clear();
            }
        }
        else
        {
            _allLexem.push_back(localStr);
            localStr.clear();
        }

        if(i == (_inputString.count() - 1))
        {
            _allLexem.push_back(localStr);
            localStr.clear();
        }
    }

    eraceElement();
    separateOperator();
    eraceElement();

    if(numberLine == 0)
    {
        _programName = _allLexem.last();
    }

    separateVariables(numberLine, _inputString);
}

void LexicalAnalyzer::eraceElement()
{
    for(int i = 0; i < _allLexem.count(); ++i)
    {
        if(_allLexem.at(i) == "")
        {
            _allLexem.removeAt(i);
        }
    }
}

void LexicalAnalyzer::separateOperator()
{
    for(int i = 0; i < _allLexem.count() - 1; ++i)
    {
        if(_allLexem.at(i) == _allLexem.at(i + 1))
        {
            if(_allLexem.at(i) == ">")
            {
                replaceElement(i, _allLexem, ">>");
            }
            if(_allLexem.at(i) == "<")
            {
                replaceElement(i, _allLexem, "<<");
            }
            if(_allLexem.at(i) == "=")
            {
                replaceElement(i, _allLexem, "==");
            }
        }
        else if(_allLexem.at(i) == "!" && _allLexem.at(i + 1) == "=")
        {
            replaceElement(i, _allLexem, "!=");
        }
        else if(_allLexem.at(i) == ">" && _allLexem.at(i + 1) == "=")
        {
            replaceElement(i, _allLexem, ">=");
        }
        else if(_allLexem.at(i) == "<" && _allLexem.at(i + 1) == "=")
        {
            replaceElement(i, _allLexem, "<=");
        }
    }
}

void LexicalAnalyzer::replaceElement(int iter, QVector<QString> &list, const QString replaceable)
{
    list.removeAt(iter);
    list.insert(iter, replaceable);
    list.removeAt(iter + 1);
}

void LexicalAnalyzer::separateVariables(unsigned int numberLine, const QString& inputString)
{
    QRegExp tmpInt("(.*)(int)(.*)");
    QRegExp tmpReal("(.*)(real)(.*)");

    if(numberLine == 1){ _lineNumber = numberLine; }

    if(tmpInt.exactMatch(inputString))
    {
        for(int i = 0; i < _allLexem.count(); ++i)
        {
            _addInt.append(_allLexem.at(i));
        }
    }
    else if(tmpReal.exactMatch(inputString))
    {
        for(int i = 0; i < _allLexem.count(); ++i)
        {
            _addReal.append(_allLexem.at(i));
        }
    }
    if(tmpInt.exactMatch(inputString) || tmpReal.exactMatch(inputString))
    {
        int position = 1;
        for(int i = 0; i < _allLexem.count() - 1; ++i)
        {
            for(int j = position++; j < _allLexem.count(); ++j)
            {
                if((_allLexem.at(i) != "," && (_allLexem.at(i) == _allLexem.at(j))) || (_allLexem.at(i) == _programName))
                {
                    QMessageBox msg;
                    msg.setText("Error: variable " + _allLexem.at(i) + " has already existed!" );
                    msg.exec();
                    exit(3);
                }
            }
        }
    }
}

bool LexicalAnalyzer::compareVariables_Diff_Row()
{
    for(int i = 0; i < _addInt.count() - 1; ++i)
    {
        for(int j = 0; j < _addReal.count(); ++j)
        {
            if(_addInt.at(i) != "," && (_addInt.at(i) == _addReal.at(j)))
            {
                QMessageBox msg;
                msg.setText("Error: variable " + _addReal.at(j) + " has already existed!" );
                msg.exec();
                return false;
            }
        }
    }
    return true;
}

void LexicalAnalyzer::separateLexem()
{
    for(int i = 0; i < _allLexem.count(); ++i)
    {
        for(int j = 0; j < _serviceLexem.count(); ++j)
        {
            if(_allLexem.at(i) == _serviceLexem.at(j)){ break; }
            else
            {
                if(j == (_serviceLexem.count() - 2))
                {
                    QRegExp tmp("(^([A-Za-z_])\\w+)|([A-Za-z_])");
                    if(tmp.exactMatch(_allLexem.at(i)))
                    {
                        additionalSeparation(_allLexem.at(i), _ID, _posId, true);
                    }
                }
                else if(j == (_serviceLexem.count() - 1))
                {
                    QRegExp tmp("\\d*\\.?\\d+$");
                    if(tmp.exactMatch(_allLexem.at(i)))
                    {
                        additionalSeparation(_allLexem.at(i), _CONST, _posConst, false);
                    }
                }
            }
        }
    }
}

void LexicalAnalyzer::additionalSeparation(const QString& string, QVector<QPair<QString, int> > &list, int& position, bool check)
{
    bool result = true;
    if(list.isEmpty())
    {
        list.append(qMakePair(string, position));
        if(check){ position++; }
        else{ position--; }
    }
    else
    {
        for(int z = 0; z < list.count(); ++z)
        {
            if(string == list.at(z).first){ result = false; break; }
        }
        if(result)
        {
            list.append(qMakePair(string, position));
            if(check){ position++; }
            else{ position--; }
        }
    }
}

void LexicalAnalyzer::dublicateId()
{
    for(int i = 0; i < _ID.count(); ++i)
    {
        _idAdd.append(_ID[i]);
    }
}

void LexicalAnalyzer::writeLexem(int &numberLine)
{
    for(int i = 0; i < _allLexem.count(); ++i)
    {
        for(int j = 0; j < _serviceLexem.count(); ++j)
        {
            if(_allLexem.at(i) == _serviceLexem.at(j))
            {
                LexemData data = {numberLine,_allLexem.at(i), j + 1};
                _resultLexem.append(data);
                break;
            }
            else
            {
                if(j == (_serviceLexem.count() - 2))
                {
                    QRegExp tmp("\\b([A-Za-z_])(.*)");
                    if(tmp.exactMatch(_allLexem.at(i)))
                    {
                        record(_allLexem.at(i), _idAdd, _resultLexem, j, numberLine);
                    }
                }
                else if(j == (_serviceLexem.count() - 1))
                {
                    QRegExp tmp("((\\.?|\\d*\\.?)\\d*)(.*)");
                    if(tmp.exactMatch(_allLexem.at(i)))
                    {
                        record(_allLexem.at(i), _CONST, _resultLexem, j, numberLine);
                    }
                }
            }
        }
    }
}

void LexicalAnalyzer::record(const QString& string, QVector<QPair<QString, int> > &list, QVector<LexemData> &resultList,
                             int &position, int &numberLine)
{
    bool result = false;
    for (int z = 0; z < list.count(); ++z)
    {
        if (string == list.at(z).first) { result = true; break; }
    }

    if (result)
    {
        LexemData data = {numberLine, string, position + 1};
        resultList.append(data);
    }
}

void LexicalAnalyzer::mainLexemFunction()
{
    clear();
    setServiceLexem();
    readProgramText();
    for(int i = 0; i < _programText.count(); ++i)
    {
        stringProcessing(i);
        if(i == _lineNumber + 1)
        {
            if(!compareVariables_Diff_Row()){ exit(1); }
        }
        separateLexem();
        if(i <= _lineNumber + 1)
        {
            dublicateId();
            _ID.clear();
        }
        writeLexem(i);
        _allLexem.clear();
    }
    QMessageBox msg;
    msg.setText("Lexical analyzer is successful complited!" );
    msg.exec();
}

QVector<LexemData> &LexicalAnalyzer::getAllLexem()
{
    return _resultLexem;
}

const QVector<QPair<QString, int>> &LexicalAnalyzer::getAllId()
{
    return _idAdd;
}

const QVector<QPair<QString, int>> &LexicalAnalyzer::getAllConst()
{
    return _CONST;
}

void LexicalAnalyzer::clear()
{
    _lineNumber = 0;
    _posId = 1;
    _posConst = -1;

    _serviceLexem.clear();
    _programText.clear();
    _allLexem.clear();
    _addInt.clear();
    _addReal.clear();
    _ID.clear();
    _CONST.clear();
    _idAdd.clear();
    _resultLexem.clear();
}
