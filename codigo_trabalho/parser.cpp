#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
	advance();
}

void
Parser::run()
{
    try {
        Program();
        cout << "Compilacao finalizada com sucesso." << endl;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

/*
void
Parser::match(int t)
{
	cout << "(int match) Current token: " << Token::returnTokenName(lToken->name) << endl;
	if (lToken->name == t || lToken->attribute == t || lToken->name == 1)
		advance();
	else {
		cout << "(int match) Current token: " << Token::returnTokenName(lToken->name) << endl;
		error("Erro no match int");
		}
}
*/

void
Parser::match(int t, string lexeme)
{
	if (lToken->name == t && (lToken->lexeme == lexeme || lToken->lexeme == "")){
    	advance();
    } else {
		string errorMsg = "LINHA " + to_string(scanner->getLine()) + ": Erro de sintaxe. Token atual: " + Token::returnTokenName(lToken->name) + " (" + lToken->lexeme + "), esperado: " + Token::returnTokenName(t) + " (" + lexeme + ")";
		throw runtime_error(errorMsg);
	}
        
}

// Produções
// * - 0 ou mais repeticoes
// ? - 0 ou uma repeticao
// token em negrito; nao terminal em italico
void 

// Program → MainClass ( ClassDeclaration )∗ EOF
Parser::Program()
{
	MainClass();
	while (lToken->name == RESERVED && lToken->lexeme == "class") // ( ClassDeclaration )∗
	{
		ClassDeclaration();
	}
	match(END_OF_FILE, "");

}
// MainClass → class ID { public static void main (String[ ] ID){ Statement } }
void 
Parser::MainClass()
{
	match(RESERVED, "class");
	match(ID, "Um ID eh esperado apos 'class'");
	match(RCB, "Um '{' eh esperado apos 'class ID'");
	match(RESERVED, "public");
	match(RESERVED, "static");
	match(RESERVED, "void");
	match(RESERVED, "main");
	match(RB, "Um '(' eh esperado apos 'main'");
	match(RESERVED, "String");
	match(RSB, "Um '[' eh esperado apos 'String'");
	match(LSB, "Um ']' eh esperado apos 'String['");
	match(ID, "Um ID eh esperado apos 'String[]'");
	match(LB, "Um ')' eh esperado apos 'ID'");
	match(RCB, "Um '{' eh esperado apos 'main(String[] ID)'");
	Statement();
	match(LCB, "Um '}' eh esperado no final da classe");
	match(LCB, "Um '}' eh esperado no final do bloco");
}

// ClassDeclaration → class ID (extends ID)? { (VarDeclaration)∗ (MethodDeclaration)∗ }
void 
Parser::ClassDeclaration()
{
	match(RESERVED, "class");
	match(ID, "Um ID eh esperado apos 'class'");

	if (lToken->name == RESERVED && lToken->lexeme == "extends") // (extends ID)?
	{
		advance();
		match(ID, "Um ID eh esperado apos 'extends'");
	}

	match(RCB, "Um '{' eh esperado apos 'class ID' ou 'class ID extends ID'");

	while (isType()) // (VarDeclaration)∗
		VarDeclaration();

	while(lToken->name == RESERVED && lToken->lexeme == "public") // (MethodDeclaration)∗
		MethodDeclaration();

	match(LCB, "Um '}' eh esperado para fechar a classe");

}

//  VarDeclaration → Type ID ;
void 
Parser::VarDeclaration()
{
	Type();
	match(ID, "Um ID eh esperado apos o tipo.");
	match(SC, "Um ';' eh esperado apos 'ID' ao declarar variavel.");
}

// MethodDeclaration → public Type ID ( (Params)? ) { (VarDeclaration)∗ (Statement)∗ return Expression ; }
void 
Parser::MethodDeclaration()
{
	advance();
	Type();
	match(ID, "Um ID eh esperado apos tipo na declaracao de metodo");
	match(RB, "Um '(' eh esperado apos 'ID' na declaracao de metodo");

	if (isType()) // (Params)?
		Params();
	
	match(LB, "Um ')' eh esperado apos parametros na declaracao de metodo");
	match(RCB, "Um '{' eh esperado apos ( parametros ) ou após ID na declaracao de metodo.");

	while (isType()) // (VarDeclaration)∗
		VarDeclaration();

	while (isStatement()){ // (Statement)∗
		Statement();
	}

	match(RESERVED, "return");
	Expression();
	match(SC, "Um ';' eh esperado apos 'return Expression' na declaracao de metodo");
	match(LCB, "Um '}' eh esperado para fechar a declaracao do metodo");
}

//  Params → Type ID (, Type ID)∗
void 
Parser::Params()
{
	Type();
	match(ID, "Um ID eh esperado apos tipo nos parametros do metodo");
	
	while (lToken->name == COMMA)
	{
		advance();
		Type();
		match(ID, "Um ID eh esperado apos tipo nos parametros do metodo");
	}
}

//  Type → int([])?| boolean | ID
void 
Parser::Type()
{
	if (lToken->name == RESERVED && lToken->lexeme == "int")
	{
		advance();
		while(lToken->name == RSB)
		{
			advance();
			match(LSB, "Um ']' eh esperado apos 'int['");
		}
	} else if (lToken->name == RESERVED && lToken->lexeme == "boolean"){
		advance();
	} else if (lToken->name == ID) {
		advance();
	} else {
		error("Tipo invalido. Esperado 'int', 'boolean' ou ID.");
	}
}

//  Statement
void 
Parser::Statement()
{
	// { (Statement)∗ }
	if (lToken->name == RCB){
		advance();
		while (isStatement()){
			Statement();
		}
		match(LCB, "Um '}' eh esperado para fechar o bloco de statements");
	// if ( Expression ) Statement else Statement
	} else if (lToken->lexeme == "if"){
		advance();
		match(RB, "Um '(' eh esperado apos 'if'");
		Expression();
		match(LB, "Um ')' eh esperado apos expressao em 'if'");
		Statement();
		match(RESERVED, "else");
		Statement();
	// while ( Expression ) Statement
	} else if (lToken->lexeme == "while"){
		advance();
		match(RB, "Um '(' eh esperado apos 'while'");
		Expression();
		match(LB, "Um ')' eh esperado apos expressao em 'while'");
		Statement();
	// System.out.println ( Expression ) ;
	} else if (lToken->lexeme == "System.out.println") {
		advance();
		match(RB, "Um '(' eh esperado apos 'System.out.println'");
		Expression();
		match(LB, "Um ')' eh esperado apos expressao em 'System.out.println'");
		match(SC, "Um ';' eh esperado apos 'System.out.println(Expression)'");
	// ID = Expression ;
	// ID [ Expression ] = Expression ;
	} else if (lToken->name == ID){
		advance();
		if (lToken->name == ASSIGN){
			advance();
			Expression();
			match(SC, "Um ';' eh esperado apos 'ID = Expression'");
		} else if (lToken->name == RSB) {
			advance();
			Expression();
			match(LSB, "Um ']' eh esperado apos 'ID['");
			match(ASSIGN, "Um '=' eh esperado apos 'ID[Expression]'");
			Expression();
			match(SC, "Um ';' eh esperado apos 'ID[Expression] = Expression'");
		} else {
			error("Atribuicao de ID invalida. Esperado '=' ou '['.");
		}
	} else {
		error("Statement invalido.");
	}
}

//  Expression → RelExpression Expression'
void 
Parser::Expression()
{
	RelExpression();
	Expression_();
}

//  Expression' → && RelExpression Expression'
void 
Parser::Expression_()
{
	if (lToken->name == AND){
		advance();
		RelExpression();
		Expression_();	
	}
}

//  RelExpression → AddExpression RelExpression'
void 
Parser::RelExpression()
{
	AddExpression();
	RelExpression_();
}

//  RelExpression' → < AddExpression RelExpression'| > AddExpression RelExpression'| == AddExpression RelExpression'| != AddExpression RelExpression'
void 
Parser::RelExpression_()
{
	if (lToken->name == LT || lToken->name == GT || lToken->name == EQ || lToken->name == NEQ){
		advance();
		AddExpression();
		RelExpression_();
	}
}

//  AddExpression → MultExpression AddExpression'
void 
Parser::AddExpression()
{
	MultExpression();
	AddExpression_();
}	

//  AddExpression' → + MultExpression AddExpression' | - MultExpression AddExpression'
void 
Parser::AddExpression_()
{
	if (lToken->name == PLUS || lToken->name == MINUS){
		advance();
		MultExpression();
		AddExpression_();
	}
}

// MultExpression → UnExpression MultExpression'
void 
Parser::MultExpression()
{
	UnExpression();
	MultExpression_();
}	

//  MultExpression' → * UnExpression MultExpression' | / UnExpression MultExpression'
void 
Parser::MultExpression_()
{
	if (lToken->name == MULT || lToken->name == DIVIDE){
		advance();
		UnExpression();
		MultExpression_();
	}
}

/*
UnExpression → ! UnExpression
| − UnExpression
| INTEGER_LITERAL
| true
| false
| new int [ Expression ]
| PrimExpression . length
| PrimExpression [ Expression ]
| PrimExpression
*/
void 
Parser::UnExpression()
{
	//cout << " AQUICurrent token: " << Token::returnTokenName(lToken->name) << endl;
	if (lToken->name == NOT || lToken->name == MINUS){
		advance();
		UnExpression();
	} else if (lToken->lexeme == "true" || lToken->lexeme == "false" || lToken->name == INT){
		advance();

	} else if (lToken->lexeme == "new"){ // caso seja new id, será tratado em Prim
		advance();
		if (lToken->lexeme == "int"){
			advance();
			match(RSB, "Um '[' eh esperado apos 'new int'");
			Expression();
			match(LSB, "Um ']' eh esperado apos 'new int['");
		} else if (lToken->name == ID){
			advance();
			match(RB, "Um '(' eh esperado apos 'new ID'");
			match(LB, "Um ')' eh esperado apos 'new ID('");
			PrimExpression_();
		}
	} else {
		PrimExpression();
		if (lToken->name == DOT){
			advance();
			match(RESERVED, "length");
		}
		if (lToken->name == RSB){
			advance();
			Expression();
			match(LSB, "Um ']' eh esperado apos 'PrimExpression['");
		}
	}
}


// PrimExpression → ID PrimExpression' | this PrimExpression' | new ID ( ) PrimExpression' | ( Expression ) PrimExpression'
void 
Parser::PrimExpression()
{
	if (lToken->name == ID) {
		advance();
		PrimExpression_();
	} else if (lToken->lexeme == "this"){
		advance();
		PrimExpression_();
	} else if(lToken->lexeme == "new"){
		advance();
		match(ID, "Um ID eh esperado apos 'new'");
		match(RB, "Um '(' eh esperado apos 'new ID'");
		match(LB, "Um ')' eh esperado apos 'new ID('");
		PrimExpression_();
	} else if(lToken->name == RB){
		advance();
		Expression();
		match(LB, "Um ')' eh esperado apos expressao em 'PrimExpression'");
		PrimExpression_();
	}
}

//  PrimExpression' → . ID PrimExpression' | . ID ( (ExpressionsList)? ) PrimExpression'
void 
Parser::PrimExpression_()
{
	if (lToken->name == DOT){
		advance();
		match(ID, "Um ID eh esperado apos '.'");

		if(lToken->name == RB){
			advance();
			if(isExpression()){
				ExpressionsList();
			}
			match(LB, "Um ')' eh esperado apos 'ID('");
			PrimExpression_();
		}
	}
}

// ExpressionsList → Expression (, Expression )∗
void 
Parser::ExpressionsList()
{
	Expression();
	while(lToken->name == COMMA){
		advance();
		Expression();
	}
}	


bool
Parser::isType()
{
    return (lToken->lexeme == "int" || lToken->lexeme == "boolean" || lToken->name == ID);
}
bool
Parser::isStatement()
{
	return (lToken->lexeme == "if" || lToken->lexeme == "while" || lToken->lexeme == "System.out.println" || lToken->name == ID || lToken->name == RCB);	
}
bool 
Parser::isExpression()
{
    return (lToken->name == INT || lToken->lexeme == "true" || lToken->lexeme == "false" ||
            lToken->lexeme == "new" || lToken->lexeme == "this" || lToken->name == ID || 
            lToken->name == RB || lToken->name == NOT || lToken->name == MINUS);
}

void
Parser::error(string str)
{
	string errorMsg = "LINHA " + to_string(scanner->getLine()) + ": " + str;
    throw runtime_error(errorMsg);
}