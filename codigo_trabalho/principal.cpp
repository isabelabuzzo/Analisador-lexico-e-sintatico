// #include "scanner.h"
#include "parser.h"


int main(int argc, char* argv[]) 
{
    //Verifica se foi executado corretamente
    //Essa main espera receber o nome do arquivo a ser
    //executado na linha de comando.
    if (argc != 2) // se não retornou os 2 args
    {
        cout << "Uso: ./compiler nome_arquivo.mj\n";
        return 1;
    }


    /* -------------- ANÁLISE LÉXICA --------------
    //string input;
    
    //getline(cin, input);

    // cria o objeto scanner com o arquivo
    Scanner* scanner = new Scanner(argv[1]);
    
    Token* t;
    // repetindo token até end of file
    do
    {
        t = scanner->nextToken();
        // caso tenha atributo, printar o atributo
        if (t->attribute != UNDEF) {
            cout << Token::returnTokenName(t->attribute) << " ";
        } else {
            cout << Token::returnTokenName(t->name) << " ";
    }
    }while (t->name != END_OF_FILE);

    delete scanner;
    ------------------------------------------ */

    Parser *parser = new Parser(argv[1]);

    parser->run();

    return 0;
}
