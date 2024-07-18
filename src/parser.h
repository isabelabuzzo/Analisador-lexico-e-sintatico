#include "scanner.h"

class Parser
{
    public:
        Scanner* scanner; // retorna tokens
        Token* lToken;    // aponta token retornado pelo scanner

        void advance(); // atualiza token recebido
        void match(int); // verifica se o parâmetro casa com lToken
        void match(int, string); // verificacao dos lexemas

        // Não terminais da gramática
        void Program();
        void MainClass();
        void ClassDeclaration();
        void VarDeclaration();
        void MethodDeclaration();
        void Params();
        void Type();
        void Statement();

        void Expression();
        void Expression_();

        void RelExpression();
        void RelExpression_();

        void AddExpression();
        void AddExpression_();

        void MultExpression();
        void MultExpression_();

        void UnExpression();

        void PrimExpression();
        void PrimExpression_();

        void ExpressionsList();
        // Retorna mensagem de erro
        void error(string);

        // Funções para checagem
        // Mais fácil p entender
        bool isType();
        bool isStatement();
        bool isExpression();
        
    public:
        Parser(string);

        void run(); // inicia análise sintática
};
