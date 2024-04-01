# Compiladores I (2024.1)

- compilador: g++

- como compilar o programa:

```
g++ token.h
g++ scanner.h
g++ -o scanner scanner.cpp principal.cpp
```

# Funcionalidades atuais
- Implementação do scanner.
  * Realiza a identificação dos tokens até ocorrer EOF.
  * Desconsideração dos espaços.
  * Reconhecimento de tokens inválidos (erro), porém sem tratamento.
