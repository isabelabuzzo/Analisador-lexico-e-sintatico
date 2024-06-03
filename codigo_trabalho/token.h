#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

// nomes serão transformados em inteiros posteriormente
enum Names 
{
    UNDEF,//0
    ID,//1
    INT,//2

    RELOP,//3
    EQ,//4  relop
    NEQ,//5  relop
    GT,//6  relop
    LT,//7  relop

    LOP,//8
    AND,//9 lop
    NOT,//10 lop

    ARTOP,//11
    PLUS,//12 artop
    MINUS,//13 artop
    MULT,//14 artop
    DIVIDE,//15 artop

    SEP,//16
    LB,//17 sep
    RB,//18  sep
    LCB,//19 sep
    RCB,//20 sep
    LSB,//21 sep
    RSB,//22 sep
    SC,//23 ; sep
    DOT,//24 . sep
    COMMA,//25 , sep

    ASSIGN,//26 =
    END_OF_FILE//27
};

class Token 
{
    public: 
        int name;       // nome do token
        int attribute;  // atributo que pode ser vazio
        string lexeme;  // texto reconhecido
    
        // construtores para cada tipo de token

        // apenas nome
        Token(int name) // exemplo: if, else, id.
        {
            this->name = name;
            attribute = UNDEF;
        }

        // nome e lexema
        Token(int name, string l) // exemplo: relop.
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        // nome e atributo
        Token(int name, int attr) 
        {
            this->name = name;
            attribute = attr;
        }
};
