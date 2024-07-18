# :gear: Analisador léxico e sintático

O seguinte repositório tem como objetivo armazenar códigos referentes à implementação de um analisador léxico e sintático para a linguagem de programação *MiniJava*, um subconjunto de Java. Ambas etapas fazem parte da construção de um compilador.

O analisador léxico é capaz de reconhecer e retornar os *tokens* da linguagem e o analisador sintático percorre o programa fonte, detectando e reportando erros. 

# :gear: Análise léxica

A análise léxica é feita utilizando os seguintes *tokens*:

1. **Identificadores (ID)**: uma sequência de letras, dígitos e underscores, começando sempre com uma letra. A linguagem distingue letras maiúsculas e minúsculas.

2. **Números inteiros (INTEGER_LITERAL)**: uma sequência de dígitos decimais, que denotam o número inteiro correspondente.

3. **Operadores (OP)**: &&, <, >, +, −, ∗, /, =, ==, ! = e !.

4. **Separadores (SEP)**: (, ), [, ], {, }, ;, . e ,.

5. **Palavras reservadas**: boolean, class, else, extends, false, if, int, length, main, new, public, return, static, String, System.out.println, this, true, void e while.

6. **Comentários**: os comentários devem ser ignorados. Existem dois tipos: comentário de linha e comentário de bloco.

7. **Espaços em branco**: devem ser ignorados. São eles \n, \t, \r e \f.

## :desktop_computer: Execução
A compilação pode ser realizada da seguinte forma:
```
g++ -o scanner scanner.cpp principal_lex.cpp

./scanner teste.mj
```

O analisador será responsável por percorrer o arquivo teste e retornar quais os *tokens* identificados. Caso não seja encontrada correspondência com os *tokens* da linguagem, será retornado um erro.

Os arquivos dentro da pasta *testes_scanner* podem ser utilizados.


# :gear: Análise sintática

A análise sintática trabalha em conjunto com a análise léxica. Os *tokens* da linguagem serão analisados utilizando as produções gramaticais como base para gerar um analisador sintático descendente preditivo.

A gramática original apresentava recursão à esquerda em suas produções, portanto, as recursões foram removidas, resultando nas produções finais, como demonstrado na tabela:
| **Produção original**                                                                 | **Produção final**                                                |
|------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------|
| **Expression** → Expression && RelExpression <br> \| RelExpression                                            | **Expression** → RelExpression Expression' <br><br> **Expression'** → && RelExpression Expression' <br> \| **ε** |
| **RelExpression** → RelExpression < AddExpression <br> \| RelExpression > AddExpression <br> \| RelExpression == AddExpression <br> \| RelExpression != AddExpression <br> \| AddExpression | **RelExpression** → AddExpression RelExpression' <br><br> **RelExpression'** → < AddExpression RelExpression' <br> \| > AddExpression RelExpression' <br> \| == AddExpression RelExpression' <br> \| != AddExpression RelExpression' <br> \| ε |
| **AddExpression** → AddExpression + MultExpression <br> \| AddExpression - MultExpression <br> \| MultExpression | **AddExpression** → MultExpression AddExpression' <br><br> **AddExpression'** → + MultExpression AddExpression' <br> \| - MultExpression AddExpression' <br> \| ε |
| **MultExpression** → MultExpression * UnExpression <br> \| MultExpression / UnExpression <br> \| UnExpression  | **MultExpression** → UnExpression MultExpression' <br><br> **MultExpression'** → * UnExpression MultExpression' <br> \| / UnExpression MultExpression' <br> \| ε |
| **PrimExpression** → ID <br> \| this <br> \| new ID ( ) <br> \| ( Expression ) <br> \| PrimExpression . ID <br> \| PrimExpression . ID ( (ExpressionsList)? ) <br> \| PrimExpression . length <br> \| PrimExpression [ Expression ] | **PrimExpression** → ID PrimExpression' <br> \| this PrimExpression' <br> \| new ID ( ) PrimExpression' <br> \| ( Expression ) PrimExpression' <br><br> **PrimExpression'** → . ID PrimExpression' <br> \| . ID ( (ExpressionsList)? ) PrimExpression' <br> \| ε |



## :desktop_computer: Execução
A compilação pode ser realizada da seguinte forma:
```
g++ -o parser scanner.cpp parser.cpp principal.cpp

./parser teste.mj
```

O analisador será responsável por percorrer o arquivo teste e retornar quais os produções identificadas. Caso não seja encontrada correspondência com as produções da linguagem, uma mensagem de erro será retornada, indicando qual a linha em que erro ocorreu e sua descrição.

Os arquivos dentro da pasta *testes_parser* podem ser utilizados.

##
O código desenvolvido foi submetido no projeto prático da disciplina de Compiladores I/2024, na UFMS.
