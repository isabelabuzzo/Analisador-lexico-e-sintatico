#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
	advance();
}

void
Parser::run()
{
    Program();
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

/**/
void
Parser::match(int t)
{
	cout << "(int match) Current token: " << Token::returnTokenName(lToken->name) << endl;
	if (lToken->name == t || lToken->attribute == t || lToken->name == 1)
		advance();
	else
		cout << "(int match) Current token: " << Token::returnTokenName(lToken->name) << endl;
		error("Erro no match int");
}

void
Parser::match(int t, string lexeme)
{
    //cout << "(lexeme match) Current token: " << Token::returnTokenName(lToken->name) << endl;
	// " with lexeme: " << lToken->lexeme <<
	if (lToken->name == t && (lToken->lexeme == lexeme || lToken->lexeme == ""))
        advance();
    else{
		cout << "Token esperado: " << Token::returnTokenName(t) << " com lexema: " << lexeme << endl;
		error("Erro inesperado." );
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
	cout << "Entrou em Program" << endl;
	MainClass();
	while (lToken->name == RESERVED && lToken->lexeme == "class") // ( ClassDeclaration )∗
	{
		ClassDeclaration();
	}
	match(END_OF_FILE);

}
// MainClass → class ID { public static void main (String[ ] ID){ Statement } }
void 
Parser::MainClass()
{
	cout << "Entrou em MC" << endl;
	match(RESERVED, "class");
	//cout << "Current token: " << Token::returnTokenName(lToken->name) << endl;
	match(ID, "");
	match(RCB, "");
	match(RESERVED, "public");
	match(RESERVED, "static");
	match(RESERVED, "void");
	match(RESERVED, "main");
	match(RB, "");
	match(RESERVED, "String");
	match(RSB, "");
	match(LSB, "");
	match(ID, "");
	match(LB, "");
	match(RCB, "");
	Statement();
	match(LCB, "");
	match(LCB, "");
	cout << "Saiu de MC" << endl;
}

// ClassDeclaration → class ID (extends ID)? { (VarDeclaration)∗ (MethodDeclaration)∗ }
void 
Parser::ClassDeclaration()
{
	match(RESERVED, "class");
	match(ID, "");

	if (lToken->name == RESERVED && lToken->lexeme == "extends") // (extends ID)?
	{
		advance();
		match(ID, "");
	}

	match(RCB, "");

	while (isType()) // (VarDeclaration)∗
		VarDeclaration();

	while(lToken->name == RESERVED && lToken->lexeme == "public") // (MethodDeclaration)∗
		MethodDeclaration();

	match(LCB, "");

}

//  VarDeclaration → Type ID ;
void 
Parser::VarDeclaration()
{
	Type();
	match(ID, "");
	match(SC, "");
}

// MethodDeclaration → public Type ID ( (Params)? ) { (VarDeclaration)∗ (Statement)∗ return Expression ; }
void 
Parser::MethodDeclaration()
{
	match(RESERVED, "public");
	Type();
	match(ID, "");
	match(RB, "");

	if (isType()) // (Params)?
		Params();
	
	match(LB, "");
	match(RCB, "");

	while (isType()) // (VarDeclaration)∗
		VarDeclaration();

	while (isStatement()){ // (Statement)∗
		Statement();
	}

	match(RESERVED, "return");
	Expression();
	match(SC, "");
	match(LCB, "");
}

//  Params → Type ID (, Type ID)∗
void 
Parser::Params()
{
	Type();
	match(ID, "");
	
	while (lToken->name == SEP && lToken->attribute == COMMA)
	{
		advance();
		Type();
		match(ID, "");
	}
}

//  Type → int([])?| boolean | ID
void 
Parser::Type()
{
	if (lToken->name == RESERVED && lToken->lexeme == "int")
	{
		advance();
		while(lToken->name == SEP && lToken->attribute == RSB)
		{
			advance();
			match(LSB, "");
		}
	} else if (lToken->name == RESERVED && lToken->lexeme == "boolean"){
		advance();
	} else if (lToken->name == ID) {
		advance();
	} else {
		error("Tipo inválido.");
	}
}

//  Statement
void 
Parser::Statement()
{
	// { (Statement)∗ }
	if (lToken->name == SEP && lToken->attribute == RCB){
		while (isStatement()){
			Statement();
		}
	// if ( Expression ) Statement else Statement
	} else if (lToken->lexeme == "if"){
		advance();
		match(RB, "");
		Expression();
		match(LB, "");
		Statement();
		match(RESERVED, "else");
		Statement();
	// while ( Expression ) Statement
	} else if (lToken->lexeme == "while"){
		advance();
		match(RB, "");
		Expression();
		match(LB, "");
		Statement();
	// System.out.println ( Expression ) ;
	} else if (lToken->lexeme == "System.out.println") {
		advance();
		match(RB, "");
		Expression();
		match(LB, "");
		match(SC, "");
	// ID = Expression ;
	// ID [ Expression ] = Expression ;
	} else if (lToken->name == ID){
		advance();
		if (lToken->name == ASSIGN){
			advance();
			Expression();
			match(SC, "");
		} else if (lToken->attribute == RSB) {
			advance();
			Expression();
			match(LSB, "");
			match(ASSIGN, "");
			Expression();
			match(SC, "");
		} else {
			error("Atribuição de ID inválida.");
		}
	} else {
		error("Statement inválido.");
	}
}

//  Expression → RelExpression Expression'
void 
Parser::Expression()
{
	if (lToken->attribute == AND){
		Expression_();
	} else {
		RelExpression();
		Expression_();
	}
}

//  Expression' → && RelExpression Expression'
void 
Parser::Expression_()
{
	match(AND, "");
	RelExpression();
	Expression_();
}

//  RelExpression → AddExpression RelExpression'
void 
Parser::RelExpression()
{
	if (lToken->attribute == LT || lToken->attribute == GT || lToken->attribute == EQ || lToken->attribute == NEQ){
		RelExpression_();
	} else {
		AddExpression();
		RelExpression_();
	}
}

//  RelExpression' → < AddExpression RelExpression'| > AddExpression RelExpression'| == AddExpression RelExpression'| != AddExpression RelExpression'
void 
Parser::RelExpression_()
{
	if (lToken->attribute == LT){
		advance();
		AddExpression();
		RelExpression_();
	} else if (lToken->attribute == GT){
		advance();
		AddExpression();
		RelExpression_();
	} else if (lToken->attribute == EQ){
		advance();
		AddExpression();
		RelExpression_();
	} else if (lToken->attribute == NEQ){
		advance();
		AddExpression();
		RelExpression_();
	} else {
		error("Expressão relacional inválida.");
	}
}

//  AddExpression → MultExpression AddExpression'
void 
Parser::AddExpression()
{
	if (lToken->attribute == PLUS || lToken->attribute == MINUS){
		AddExpression_();
	} else {
		MultExpression();
		AddExpression_();
	}
}	

//  AddExpression' → + MultExpression AddExpression' | - MultExpression AddExpression'
void 
Parser::AddExpression_()
{
	if (lToken->attribute == PLUS){
		advance();
		MultExpression();
		AddExpression_();
	} else if (lToken->attribute == MINUS){
		advance();
		MultExpression();
		AddExpression_();
	} else {
		error("Expressão de adição inválida.");
	}
}

// MultExpression → UnExpression MultExpression'
void 
Parser::MultExpression()
{
	if (lToken->attribute == MULT || lToken->attribute == DIVIDE){
		MultExpression_();
	} else {
		UnExpression();
		MultExpression_();
	}
}	

//  MultExpression' → * UnExpression MultExpression' | / UnExpression MultExpression'
void 
Parser::MultExpression_()
{
	if (lToken->attribute == MULT){
		advance();
		UnExpression();
		MultExpression_();
	} else if (lToken->attribute == DIVIDE){
		advance();
		UnExpression();
		MultExpression_();
	} else {
		error("Expressão de multiplicação inválida.");
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
	if (lToken->attribute == NOT || lToken->attribute == MINUS){
		advance();
		match(INT, "");
	} else if (lToken->lexeme == "true" || lToken->lexeme == "false"){
		advance();
	} else if (lToken->lexeme == "new"){
		advance();
		match(INT, "");
		match(RSB, "");
		Expression();
		match(LSB, "");
	} else {
		PrimExpression();
		if (lToken->attribute == DOT){
			advance();
			match(RESERVED, "length");
		} else if (lToken->attribute == RSB){
			advance();
			Expression();
			match(LSB, "");
		}
	}
}


// PrimExpression → ID PrimExpression' | this PrimExpression' | new ID ( ) PrimExpression' | ( Expression ) PrimExpression'
void 
Parser::PrimExpression()
{
	if (lToken->attribute == DOT){
		PrimExpression_();
	} else {
		if (lToken->name == ID){
			advance();
			PrimExpression_();
		} else if (lToken->lexeme == "this"){
			advance();
			PrimExpression_();
		}else if (lToken->lexeme == "new"){
			advance();
			match(ID, "");
			match(RB, "");
			match(LB, "");
			PrimExpression_();
		}else if (lToken->attribute == RB) {
			advance();
			Expression();
			match(LB, "");
			PrimExpression_();
		} else {
			error("Expressão prim inválida.");
		}
	}
}

//  PrimExpression' → . ID PrimExpression' | . ID ( (ExpressionsList)? ) PrimExpression'
void 
Parser::PrimExpression_()
{
	if (lToken->attribute == DOT){
		advance();
		match(ID, "");
		if (lToken->attribute == RB){
			advance();

			if (lToken->attribute != LB)
				ExpressionsList();

			match(LB, "");
			PrimExpression_();
		}
	} else {
		error("Expressão prim inválida.");
	}
}

// ExpressionsList → Expression (, Expression )∗
void 
Parser::ExpressionsList()
{
	Expression();
	match(RB, "");

	while(lToken->attribute == COMMA){
		advance();
		Expression();
	}
}	



bool
Parser::isType()
{
    return (lToken->lexeme == "int" || lToken->lexeme == "boolean" || lToken->lexeme == "void" || lToken->name == ID);
}
bool
Parser::isStatement()
{
	return (lToken->attribute == RCB || lToken->lexeme == "if" || lToken->lexeme == "while" || lToken->lexeme == "System.out.println" || lToken->name == ID);	
}

void
Parser::error(string str)
{
	cout << str << endl;

	exit(EXIT_FAILURE);
}