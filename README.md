# Projeto LPIBANK - Sistema Bancário

#ESSE PROJETO FOI UMA RECRIAÇÃO PILOTO DE UMA FERRAMENTE DE TRABALHO QUE USEI DURANTE ALGUNS ANOS 

## Modelagem do Sistema (UML)

```mermaid
classDiagram
    class Pessoa {
        <<abstract>>
        #string nome
        #string cpf
        +getNome() string
        +getCpf() string
        +exibirDados()* void
        +getTipo()* string
    }
    class Operador {
        -string senha
        +autenticar(string s) bool
        +exibirDados() void
    }
    class Cliente {
        -double saldo
        +exibirDados() void
    }
    class BancoManager {
        -vector~Pessoa*~ cadastros
        +inserirOperador() bool
        +salvarDados()
    }
    Pessoa <|-- Operador
    Pessoa <|-- Cliente
    BancoManager o-- Pessoa
