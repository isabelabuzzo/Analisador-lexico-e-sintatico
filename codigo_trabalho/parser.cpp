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
	else {
		cout << "(int match) Current token: " << Token::returnTokenName(lToken->name) << endl;
		error("Erro no match int");
		}
}

void
Parser::match(int t, string lexeme)
{
    //cout << "(lexeme match) Current token: " << Token::returnTokenName(lToken->name) << endl;
	// " with lexeme: " << lToken->lexeme <<
	if (lToken->name == t && (lToken->lexeme == lexeme || lToken->lexeme == ""))
        advance();
    else{
		cout << "Current token: " << Token::returnTokenName(lToken->name) << endl;
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
		cout << "Entrei no Class Declaration do Program" << endl;
		ClassDeclaration();
	}
	cout << "Saí do Class Declaration" << endl;
	match(END_OF_FILE, "");
	cout << "Compilação concluída com sucesso." << endl;

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
	cout << "Chegou no Statement do MainClass" << endl;
	Statement();
	cout << "Saiu do Statement do MainClass" << endl;
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
	advance();
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
	
	while (lToken->name == COMMA)
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
		while(lToken->name == RSB)
		{
			advance();
			match(LSB, "");
		}
	} else if (lToken->name == RESERVED && lToken->lexeme == "boolean"){
		advance();
	} else if (lToken->name == ID) {
		advance();
	} else {
		error("Type error: tipo inválido.");
	}
}

//  Statement
void 
Parser::Statement()
{
	cout << "Dentro de statement" << endl;
	// { (Statement)∗ }
	if (lToken->name == RCB){
		advance();
		while (isStatement()){
			Statement();
		}
		match(LCB, "");
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
		} else if (lToken->name == RSB) {
			advance();
			Expression();
			match(LSB, "");
			match(ASSIGN, "");
			Expression();
			match(SC, "");
		} else {
			error("Statement error: atribuição de ID inválida.");
		}
	} else {
		error("Statement error: nenhum if casou.");
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
	} else if (lToken->lexeme == "new"){
		advance();
		match(RESERVED, "int"); // BUG AKI
		match(RSB, "");
		Expression();
		match(LSB, "");
	} else {
		PrimExpression();
		if (lToken->name == DOT){
			advance();
			match(RESERVED, "length");
		}
		if (lToken->name == RSB){
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
	if (lToken->name == ID) {
		advance();
		PrimExpression_();
	} else if (lToken->lexeme == "this"){
		advance();
		PrimExpression_();
	} else if(lToken->lexeme == "new"){
		advance();
		match(ID, "");
		match(RB, "");
		match(LB, "");
		PrimExpression_();
	} else if(lToken->name == RB){
		advance();
		Expression();
		match(LB, "");
		PrimExpression_();
	}
}

//  PrimExpression' → . ID PrimExpression' | . ID ( (ExpressionsList)? ) PrimExpression'
void 
Parser::PrimExpression_()
{
	if (lToken->name == DOT){
		advance();
		match(ID, "");

		if(lToken->name == RB){
			advance();
			if(isExpression()){
				ExpressionsList();
			}
			match(LB, "");
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
	cout << endl << "LINHA " << scanner->getLine() << ": " << str << endl << endl;

	exit(EXIT_FAILURE);
}