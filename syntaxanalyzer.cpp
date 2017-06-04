#include "syntaxanalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer() : _lexNumber(0)
{
}

void SyntaxAnalyzer::setAllLexem(QVector<LexemData> &list)
{
    clear();
    _lexems = qMove(list);
}

bool SyntaxAnalyzer::checkLexem(int i, const int codeNumber)
{
    if(_lexems.at(i).code != codeNumber)
    {
        QMessageBox msg;
        msg.setText("Error in line: " + QString::number(_lexems.at(i).stringNumber + 1) +
                    "\nError in " + _lexems.at(i).lexem + "!");
        msg.exec();
        return false;
    }
    return true;
}

void SyntaxAnalyzer::checkListOfDeclaration(int& lexNumber)
{
    while(_lexems.at(lexNumber).code != 19)//check ;
    {
        if(checkLexem(lexNumber++, 39))//check ID
        {
            if(_lexems.at(lexNumber).code == 20)//check ,
            {
                lexNumber++;
                continue;
            }
            else if(_lexems.at(lexNumber).code == 19)//check ;
            {
                lexNumber++;
                break;
            }
            else if(_lexems.at(lexNumber).code == 15)//check {
            {
                break;
            }
            else
            {
                QMessageBox msg;
                msg.setText("Error in line: " + QString::number(_lexems.at(lexNumber).stringNumber + 1) +
                            "\nError in " + _lexems.at(lexNumber).lexem + "!");
                msg.exec();
                exit(2);
            }
        }
        else{ exit(2); }
    }
}

bool SyntaxAnalyzer::checkOnDeclarations()
{
    if(checkLexem(_lexNumber++, 1))//check Program
    {
        if(checkLexem(_lexNumber++, 39))//check ID
        {
            if(checkLexem(_lexNumber++, 2))//check var
            {
                if(checkLexem(_lexNumber++, 3))//check int
                {
                    while(_lexems.at(_lexNumber).code != 4)//check real
                    {
                        checkListOfDeclaration(_lexNumber);
                    }
                    _lexNumber++;
                    while(_lexems.at(_lexNumber).code != 15)//check {
                    {
                        checkListOfDeclaration(_lexNumber);
                    }
                    _lexNumber++;
                    return true;
                }
                else{ exit(2); }
            }
            else{ exit(2); }
        }
        else{ exit(2); }
    }
    else{ exit(2); }

    return false;
}

bool SyntaxAnalyzer::checkOnOperators(const int code)
{
    while(_lexems.at(_lexNumber).code != 16)//check }
    {
        if(_lexems.at(_lexNumber).code == 5)//check cout
        {
            _lexNumber++;
            checkCout(_lexNumber);
        }
        else if(_lexems.at(_lexNumber).code == 6)//check cin
        {
            _lexNumber++;
            checkCin(_lexNumber);
        }
        else if(_lexems.at(_lexNumber).code == 39)//check ID(expression)
        {
            _lexNumber++;
            if(checkLexem(_lexNumber++, 27))//check =
            {
                checkExpression(_lexNumber, code);
            }
        }
        else if(_lexems.at(_lexNumber).code == 7)//check for
        {
            _lexNumber++;
            checkLoop(_lexNumber);
        }
        else if(_lexems.at(_lexNumber).code == 12)//check if
        {
            _lexNumber++;
            checkLogicalExpression(_lexNumber);
        }
        else if(_lexems.at(_lexNumber).code == 19){_lexNumber++;/* return true;*/}//check ;
        else if(_lexems.at(_lexNumber).code == 11){_lexNumber++;/* return true;*/}//check rof
        else if(_lexems.at(_lexNumber).code == 14){_lexNumber++;/* return true;*/}//check fi
        else
        {
            QMessageBox msg;
            msg.setText("Error in line: " + QString::number(_lexems.at(_lexNumber).stringNumber + 1) +
                        "\nError in " + _lexems.at(_lexNumber).lexem + "!");
            msg.exec();
            return false;
        }
    }

    return true;
}

void SyntaxAnalyzer::checkCout(int &lexNumber)
{
    while(_lexems.at(lexNumber).code != 19)//check ;
    {
        if(checkLexem(lexNumber++, 17))//check <<
        {
            if(_lexems.at(lexNumber).code == 39 || _lexems.at(lexNumber).code == 40)//check ID || Const
            {
                lexNumber++;
            }
            else
            {
                QMessageBox msg;
                msg.setText("Error in line: " + QString::number(_lexems.at(lexNumber).stringNumber + 1) +
                            "\nError in " + _lexems.at(lexNumber).lexem + "!");
                msg.exec();
                exit(2);
            }
        }
        else{ exit(2); }
    }
    lexNumber++;
}

void SyntaxAnalyzer::checkCin(int &lexNumber)
{
    while(_lexems.at(lexNumber).code != 19)//check ;
    {
        if(checkLexem(lexNumber++, 18))//check >>
        {
            if(!checkLexem(lexNumber++, 39))//check ID
            {
                exit(2);
            }
        }
        else{ exit(2); }
    }
    _lexNumber++;
}

bool SyntaxAnalyzer::checkExpression(int &lexNumber, const int code)
{
    int count = 0;
    while(_lexems.at(lexNumber).code != code)
    {
        if(_lexems.at(lexNumber).code == 39 || _lexems.at(lexNumber).code == 40)//check ID
        {
            lexNumber++;
            //check ( || ) || + || - || * || /
            if(_lexems.at(lexNumber).code == 26 || _lexems.at(lexNumber).code == 21 || _lexems.at(lexNumber).code == 22 ||
                    _lexems.at(lexNumber).code == 23 || _lexems.at(lexNumber).code == 24 || _lexems.at(lexNumber).code == 25)
            {
                if(_lexems.at(lexNumber).code == 25){ count++; }// (
                else if(_lexems.at(lexNumber).code == 26)
                {
                    count--;
                    if(count < 0)
                    {
                        QMessageBox msg;
                        msg.setText("Error in line: " + QString::number(_lexems.at(lexNumber).stringNumber + 1) +
                                    "\nError in " + _lexems.at(lexNumber).lexem + "!");
                        msg.exec();
                        exit(2);
                    }
                }
                lexNumber++;
            }//check < || > || == || >= || <= || !=
            else if(_lexems.at(lexNumber).code == 28 || _lexems.at(lexNumber).code == 29 || _lexems.at(lexNumber).code == 33 ||
                    _lexems.at(lexNumber).code == 34 || _lexems.at(lexNumber).code == 35 || _lexems.at(lexNumber).code == 36)
            {
                return true;
            }//check or || and || then
            else if(_lexems.at(lexNumber).code == 30 || _lexems.at(lexNumber).code == 31 || _lexems.at(lexNumber).code == 13)
            {
                return true;
            }
            else if(_lexems.at(lexNumber).code == 38){return true;}//check ]
            else if(_lexems.at(lexNumber).code == 19){return true;}//check ;
        }
        else if(_lexems.at(lexNumber).code == 22)//check -
        {
            lexNumber++;
            if(_lexems.at(lexNumber).code == 22)// -
            {
                QMessageBox msg;
                msg.setText("Error in line: " + QString::number(_lexems.at(lexNumber).stringNumber + 1) +
                            "\nError in " + _lexems.at(lexNumber).lexem + "!");
                msg.exec();
                exit(2);
            }
            else{ continue; }
        }
        else if(_lexems.at(lexNumber).code == 25)//check (
        {
            count++;
            lexNumber++;
            continue;
        }
        else if(_lexems.at(lexNumber).code == 26)//check )
        {
            count--;
            if(count < 0)
            {
                QMessageBox msg;
                msg.setText("Error in line: " + QString::number(_lexems.at(lexNumber).stringNumber + 1) +
                            "\nError in " + _lexems.at(lexNumber).lexem + "!");
                msg.exec();
                exit(2);
            }
            lexNumber++;
        }
        else if(_lexems.at(lexNumber).code == 21 || _lexems.at(lexNumber).code == 22 || _lexems.at(lexNumber).code == 23 ||
                _lexems.at(lexNumber).code == 24)//check + || - || * || /
        {
            lexNumber++;
        }
        else
        {
            QMessageBox msg;
            msg.setText("Error in line: " + QString::number(_lexems.at(lexNumber).stringNumber + 1) +
                        "\nError in " + _lexems.at(lexNumber).lexem + "!");
            msg.exec();
            exit(2);
        }
    }
    if(count != 0)
    {
        QMessageBox msg;
        msg.setText("Error in line: " + QString::number(_lexems.at(lexNumber).stringNumber + 1) +
                    "\nError in " + _lexems.at(lexNumber).lexem + "!");
        msg.exec();
        exit(2);
    }
    lexNumber++;
    return true;
}

void SyntaxAnalyzer::checkLoop(int &lexNumber)
{
    if(checkLexem(lexNumber++, 39))//check ID
    {
        if(checkLexem(lexNumber++, 27))//check =
        {
            checkExpression(lexNumber, 8);//check by
            checkExpression(lexNumber, 9);//check to
            checkExpression(lexNumber, 10);//check do
            if(_lexems.at(_lexNumber).code != 19)//check ;
            {
                if(!checkOnOperators(11)){exit(2);}//check rof
            }
        }
        else{exit(2);}
    }
    else {exit(2);}
}

void SyntaxAnalyzer::checkLogicalExpression(int& lexNumber)
{
    int count = 0;
    while(_lexems.at(lexNumber).code != 13)//check then
    {
        if(_lexems.at(lexNumber).code == 39 || _lexems.at(lexNumber).code == 40)//check id
        {
            if(!checkRelation(lexNumber, count)){ exit(2); }
            else if(_lexems.at(lexNumber).code == 30 || _lexems.at(lexNumber).code == 31)//check and || or
            {
                lexNumber++;
                continue;
            }
        }
        else if(_lexems.at(lexNumber).code == 37)//check [
        {
            count++;
            lexNumber++;
        }
        else if(_lexems.at(lexNumber).code == 32)//check not
        {
            lexNumber++;
        }
    }
    if(count != 0)
    {
        QMessageBox msg;
        msg.setText("Error in line: " + QString::number(_lexems.at(lexNumber).stringNumber + 1) +
                    "\nError in " + _lexems.at(lexNumber).lexem + "!");
        msg.exec();
        exit(2);
    }
    lexNumber++;
}

bool SyntaxAnalyzer::checkRelation(int& lexNumber, int& count)
{
    //check < || > || == || >= || <= || !=
    if(checkExpression(lexNumber, 28) || checkExpression(lexNumber, 29) || checkExpression(lexNumber, 33) ||
            checkExpression(lexNumber, 34) || checkExpression(lexNumber, 35) || checkExpression(lexNumber, 36))
    {
        lexNumber++;
        if(checkExpression(lexNumber, 13) || checkExpression(lexNumber, 30) ||
                checkExpression(lexNumber, 31))//check then || or || and
        {
            if(_lexems.at(lexNumber).code == 38)//check ]
            {
                count--;
                if(count < 0)
                {
                    QMessageBox msg;
                    msg.setText("Error in line: " + QString::number(_lexems.at(lexNumber).stringNumber + 1) +
                                "\nError in " + _lexems.at(lexNumber).lexem + "!");
                    msg.exec();
                    return false;
                }
                lexNumber++;
            }
            return true;
        }
    }
    return false;
}

void SyntaxAnalyzer::mainCheck()
{
    if(checkOnDeclarations())
    {
        if(checkOnOperators(19))
        {
            QMessageBox msg;
            msg.setText("Syntax analyzer is successful completed!");
            msg.exec();
        }

    }
}

void SyntaxAnalyzer::clear()
{
    _lexems.clear();
    _lexNumber = 0;
}
