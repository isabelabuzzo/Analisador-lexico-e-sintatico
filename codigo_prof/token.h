#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF, //0
    ID, //1
    NUMBER,
    INTEGER_LITERAL,
    //Continuar
    //Nomes e atributos dos tokens da linguagem
    END_OF_FILE
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name) // exemplo: if, else, id.
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l) // exemplo: relop.
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr) 
        {
            this->name = name;
            attribute = attr;
        }
};
