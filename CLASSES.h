#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// Estrutura para o Relatório do Turno
struct Transacao {
    string tipo;
    double valor;
};

// =========================================================
// REQUISITO 3: COMPOSIÇÃO 
// =========================================================
class Data {
public:
    int dia, mes, ano;
    Data(int d = 1, int m = 1, int a = 2000) : dia(d), mes(m), ano(a) {}
};

// =========================================================
// REQUISITO 4 e 7: SUPERCLASSE E POLIMORFISMO
// =========================================================
class Pessoa {
protected:
    string nome;
    string cpf;
public:
    Pessoa(string n, string c) : nome(n), cpf(c) {}
    virtual ~Pessoa() {}
    
    string getNome() { return nome; }
    string getCpf() { return cpf; }
    void setNome(string n) { nome = n; }
    
    virtual void exibirDados() = 0; 
    virtual string getTipo() = 0;
};

class Operador : public Pessoa {
private:
    string senha;
    Data dataNascimento; 
public:
    Operador(string n, string c, string s, Data d) : Pessoa(n, c), senha(s), dataNascimento(d) {}
    
    bool autenticar(string s) { return senha == s; }
    string getSenha() { return senha; }
    void setSenha(string s) { senha = s; }
    
    void exibirDados() override {
        cout << "[Operador] Nome: " << nome << " | CPF: " << cpf << endl;
    }
    string getTipo() override { return "Operador"; }
};

class Cliente : public Pessoa {
private:
    double saldo;
public:
    Cliente(string n, string c, double s) : Pessoa(n, c), saldo(s) {}
    
    void exibirDados() override {
        cout << "[Cliente] Nome: " << nome << " | CPF: " << cpf << " | Saldo: R$" << saldo << endl;
    }
    string getTipo() override { return "Cliente"; }
};

// =========================================================
// REQUISITO 8: CLASSE QUE GERENCIA O CRUD
// =========================================================
class BancoManager {
private:
    vector<Pessoa*> cadastros; 
    vector<Transacao> historicoTurno;
    double valorCaixa;
    double valorAbertura;

public:
    BancoManager() {
        valorCaixa = 0.0;
        valorAbertura = 0.0;
        carregarDados(); 
    }
    
    ~BancoManager() {
        salvarDados(); 
        for(auto p : cadastros) delete p;
    }

    void setAbertura(double v) { valorAbertura = v; valorCaixa = v; }
    double getAbertura() { return valorAbertura; }
    double getCaixa() { return valorCaixa; }
    
    void registrarTransacao(string tipo, double valor) {
        if(tipo == "DEPOSITO") valorCaixa += valor;
        else if(tipo == "SAQUE") valorCaixa -= valor;
        historicoTurno.push_back({tipo, valor});
    }

    int contarOperadores() {
        int cont = 0;
        for(auto p : cadastros) {
            if(p->getTipo() == "Operador") cont++;
        }
        return cont;
    }

    bool inserirOperador(string nome, string cpf, string senha, int d, int m, int a) {
        if (contarOperadores() >= 5) return false;
        cadastros.push_back(new Operador(nome, cpf, senha, Data(d, m, a)));
        return true;
    }

    void inserirCliente(string nome, string cpf, double saldo) {
        cadastros.push_back(new Cliente(nome, cpf, saldo));
    }

    void listarTodos() {
        cout << "--- LISTA DE REGISTROS ---" << endl;
        for(size_t i = 0; i < cadastros.size(); i++) {
            cout << "Indice [" << i << "] - ";
            cadastros[i]->exibirDados();
        }
    }

    // =========================================================
    // REQUISITO 1 (OBS): PESQUISA POR NOME RETORNANDO ÍNDICE
    // Lida com múltiplos usuários com o mesmo nome
    // =========================================================
    int pesquisar(string nomeBusca) {
        vector<int> indicesEncontrados;
        
        for(size_t i = 0; i < cadastros.size(); i++) {
            if(cadastros[i]->getNome() == nomeBusca) {
                indicesEncontrados.push_back(i);
            }
        }

        if(indicesEncontrados.empty()) {
            return -1; // Não encontrou
        } else if(indicesEncontrados.size() == 1) {
            return indicesEncontrados[0]; // Retorna direto se só tiver 1
        } else {
            // Se tiver mais de um, exibe as opções pelo índice
            cout << "\nForam encontrados multiplos registros com o nome '" << nomeBusca << "':" << endl;
            for(int idx : indicesEncontrados) {
                cout << "Indice [" << idx << "] - ";
                cadastros[idx]->exibirDados();
            }
            int indiceEscolhido;
            cout << "Digite o indice correspondente ao que deseja selecionar: ";
            cin >> indiceEscolhido;
            return indiceEscolhido;
        }
    }

    void alterar(string nome) {
        int idx = pesquisar(nome);
        if(idx != -1) {
            string novoNome;
            cout << "Novo Nome: "; cin >> novoNome;
            cadastros[idx]->setNome(novoNome);
            
            if(cadastros[idx]->getTipo() == "Operador") {
                string novaSenha;
                cout << "Nova Senha: "; cin >> novaSenha;
                dynamic_cast<Operador*>(cadastros[idx])->setSenha(novaSenha);
            }
            cout << "Dados alterados com sucesso!" << endl;
        } else cout << "Registro nao encontrado!" << endl;
    }

    void remover(string nome) {
        int idx = pesquisar(nome);
        if(idx != -1) {
            delete cadastros[idx];
            cadastros.erase(cadastros.begin() + idx);
            cout << "Removido com sucesso!" << endl;
        } else cout << "Registro nao encontrado!" << endl;
    }

    void exibirRelatorioTurno(string nomeOp) {
        cout << "--- RELATORIO DE TURNO: " << nomeOp << " ---" << endl;
        cout << "Valor de Abertura: R$ " << valorAbertura << endl;
        cout << "Transacoes Realizadas:" << endl;
        for(auto t : historicoTurno) cout << " > " << t.tipo << ": R$ " << t.valor << endl;
        cout << "Valor Final Esperado no Sistema: R$ " << valorCaixa << endl;
    }

    Operador* buscarOperadorCpf(string cpf) {
        for(auto p : cadastros) {
            if(p->getTipo() == "Operador" && p->getCpf() == cpf) {
                return dynamic_cast<Operador*>(p);
            }
        }
        return nullptr;
    }

    // =========================================================
    // REQUISITO 11: SALVANDO DUAS CLASSES NO MESMO ARQUIVO
    // =========================================================
    void salvarDados() {
        ofstream arq("dados_banco.txt");
        if(arq.is_open()) {
            for(auto p : cadastros) {
                if(p->getTipo() == "Operador") {
                    Operador* op = dynamic_cast<Operador*>(p);
                    arq << "OP;" << op->getNome() << ";" << op->getCpf() << ";" << op->getSenha() << "\n";
                } else if(p->getTipo() == "Cliente") {
                    Cliente* cli = dynamic_cast<Cliente*>(p);
                    arq << "CL;" << cli->getNome() << ";" << cli->getCpf() << ";1000\n";
                }
            }
            arq.close();
        }
    }

    void carregarDados() {
        ifstream arq("dados_banco.txt");
        string linha, tipo, nome, cpf, extra;
        if(arq.is_open()) {
            while(getline(arq, linha)) {
                stringstream ss(linha);
                getline(ss, tipo, ';');
                getline(ss, nome, ';');
                getline(ss, cpf, ';');
                getline(ss, extra, ';');
                
                if(tipo == "OP") inserirOperador(nome, cpf, extra, 1, 1, 2000);
                else if(tipo == "CL") inserirCliente(nome, cpf, stod(extra));
            }
            arq.close();
        }
    }
};

#endif