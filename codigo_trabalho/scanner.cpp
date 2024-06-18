#include "scanner.h"    

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input)
{
    //this->input = input;
    //cout << "Entrada: " << input << endl;
    pos = 0;
    line = 1;

    ifstream inputFile(input, ios::in); // verifica se arquivo esta aberto
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,line) ) // se estiver, preenche o input
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else // se não estiver
        cout << "Unable to open file\n"; 

//    //A próxima linha deve ser comentada posteriormente.
//    //Ela é utilizada apenas para verificar se o 
//    //preenchimento de input foi feito corretamente.
    cout << this->input << endl;

}

// getter q retorna a linha do arq
int
Scanner::getLine()
{
    return line;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    int state = 0;
    string lexeme;

    while (true){
        switch (state)
        {
        case 0:
            if (input[pos] == '\0')
            {
                tok = new Token(END_OF_FILE, "");
                return tok;
            }
            else if (input[pos] == '<')
                state = 5;
            else if (input[pos] == '=')
                state = 13;
            else if (input[pos] == '>')
                state = 6;
            else if(input[pos] == '*')
                state = 7;
            else if(input[pos] == '-')
                state = 8;
            else if(input[pos] == '+')
                state = 9;
            else if(input[pos] == '/')
                state = 10;
            else if(input[pos] == '&')
                state = 11;
            else if(input[pos] == '!')
                state = 16;
            else if(input[pos] == ')')
                state = 19;
            else if(input[pos] == '(')
                state = 20;
            else if(input[pos] == '{')
                state = 22;
            else if(input[pos] == '}')
                state = 21;
            else if(input[pos] == ']')
                state = 24;
            else if(input[pos] == '[')
                state = 23;
            else if(input[pos] == '.')
                state = 25;
            else if(input[pos] == ',')
                state = 26;
            else if(input[pos] == ';')
                state = 27; 
            else if (isalpha(input[pos])){
                state = 1;
                lexeme += input[pos]; // palavra reservada
            } else if (isdigit(input[pos]))
                state = 3;
            else if (isspace(input[pos]))
                state = 28;
            else
                lexicalError();

            pos++;
            break;
        
        case 1: // id e palavras reservadas
            if (!isalnum(input[pos]) && input[pos] != '_') {
                    state = 2;
                } else {
                    lexeme += input[pos]; // palavra reservada
                }
            pos++;
            break;

        case 2: // id e palavras reservadas
            pos--;
            if (lexeme == "boolean" || lexeme == "class" || lexeme == "else" ||
                lexeme == "extends" || lexeme == "false" || lexeme == "if" ||
                lexeme == "int" || lexeme == "length" || lexeme == "main" ||
                lexeme == "new" || lexeme == "public" || lexeme == "return" ||
                lexeme == "static" || lexeme == "String" || lexeme == "this" ||
                lexeme == "true" || lexeme == "void" || lexeme == "while") {
                state = 36; // palavra reservada
            } else if (lexeme == "System") {
                pos--;      // voltando a posição para o dot
                state = 35; // caso especial de system.out.print
            } else {
                state = 34; // id normal
            }
            pos++;
            break;

        case 34: // ID normal
            tok = new Token(ID, "");
            pos--;
            return tok;

        case 35:
            if (input.substr(pos, 12) == ".out.println") {
                lexeme += input.substr(pos, 12); // avança ate o final da palavra
                pos += 12;
                state = 36; // reconhece token
            } else {
                state = 34;
            }
            pos++;
            break;

        case 36: // palavra reservada
            tok = new Token(RESERVED, lexeme);
            pos--;
            return tok;

        
        case 3: // digito
            if (!isdigit(input[pos]))
                state = 4;
            
            pos++;
            break;

        case 4: // digito
            tok = new Token(INT, "");
            pos--;
            return tok;

        case 5: // <
            tok = new Token(LT, "");
            return tok;

        case 6: // >
            tok = new Token(GT, "");
            return tok;

        case 7: // *
            tok = new Token(MULT, "");
            return tok;
        
        case 8: // -
            tok = new Token(MINUS, "");
            return tok;
        
        case 9: // +
            tok = new Token(PLUS, "");
            return tok;

        case 10: // / ou comentário
            if(input[pos] == '/'){
                state = 30;
            } else if (input[pos] == '*'){ // comentario */
                state = 32;
            } else{
                state = 31;
                pos++;
            }
            break;
        
        case 30: // passa para o proximo enquanto não houver quebra de linha
            if (input[pos] == '\n')
                state = 0;

            pos++;
            break;

        case 31: // divide
            tok = new Token(DIVIDE, "");
            pos--;
            return tok;

        case 32: // comentario */
            if (input[pos] == '*')
                state = 33;

            pos++;
            break;
        
        case 33:
            if (input[pos] == '/')
                state = 0;
            else
                state = 32;

            pos++;
            break;

        
        case 11: // &&
            if(input[pos] == '&')
                state = 12;
            else
                lexicalError();

            pos++;
            break;
        
        case 12: // &&
            tok = new Token(AND, "");
            return tok;
        
        case 13: // == ou =
            if(input[pos] == '=') // ==
            {
                state = 14;

            } else { // =
                state = 15;
            }
            
            pos++;
            break;

        case 14: // ==
            tok = new Token(EQ, "");
            return tok;
        
        case 15: // =
            tok = new Token(ASSIGN, "");
            pos--;
            return tok;

        case 16: // != ou !
            pos++;
            if(input[pos] == '=')
            {
                state = 17;

            } else { // !
                state = 18;
                pos++;
            }
            break;
        
        case 17: // !=
            tok = new Token(NEQ, "");
            return tok;

        case 18: // !
            tok = new Token(NOT, "");
            pos--;
            return tok;
        
        case 19: // )
            tok = new Token(LB, "");
            return tok;
        
        case 20: // (
            tok = new Token(RB, "");
            return tok;

        case 21: // }
            tok = new Token(LCB, "");
            return tok;

        case 22: // {
            tok = new Token(RCB, "");
            return tok;

        case 23: // [
            tok = new Token(RSB, "");
            return tok;
        
        case 24: // ]
            tok = new Token(LSB, "");
            return tok;
        
        case 25: // .
            tok = new Token(DOT, "");
            return tok;
        
        case 26: // ,
            tok = new Token(COMMA, "");
            return tok;
        
        case 27: // ;
            tok = new Token(SC, "");
            return tok;

        case 28: // pos++ enquanto for espaço
            if (!isspace(input[pos]))
                state = 29;

            pos++;
            break;

        case 29: // espaço
            pos--;
            state = 0;
            break;

        default:
            lexicalError();
        }
    }
}

void 
Scanner::lexicalError()
{
    cout << "Token mal formado\n";
    
    exit(EXIT_FAILURE);
}