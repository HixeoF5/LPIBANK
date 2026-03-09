#include <iostream>
#include <ctime>
#include "CLEAR.h"
#include "CLASSES.h"

using namespace std;

int main() {
    BancoManager banco;
    string cpfSuper = "08607341456";
    string senhaSuper = "08607341456";

    int OPCAO;
    string CPF, SENHA, NOME;
    Operador* operadorLogado = nullptr;

    while (true) {
        limparTela();
        cout << "----------------------------------" << endl;
        cout << "***********LPIBANK****************" << endl;
        cout << "----------------------------------" << endl;
        cout << "1 - LOGIN" << endl;
        cout << "2 - NOVO OPERADOR" << endl;
        cout << "3 - ACESSO ROOT (MASTER)" << endl;
        cout << "4 - DESLIGAR SISTEMA" << endl;
        cout << "Escolha: ";
        cin >> OPCAO;
        limparTela();

        if (OPCAO == 1) {
            cout << "DIGITE SEU CPF: "; cin >> CPF;
            cout << "DIGITE SUA SENHA: "; cin >> SENHA;
            
            operadorLogado = banco.buscarOperadorCpf(CPF);
            if (operadorLogado != nullptr && operadorLogado->autenticar(SENHA)) {
                break; // Vai pro Caixa
            } else {
                cout << "\nUsuario nao cadastrado ou senha incorreta!" << endl;
                cin.ignore(); cin.get();
            }
        } 
        else if (OPCAO == 2) {
            cout << "--- NOVO CADASTRO DE OPERADOR ---" << endl;
            if (banco.contarOperadores() >= 5) {
                cout << "ERRO: Limite maximo de 5 operadores atingido!" << endl;
            } else {
                int d, m, a;
                cout << "NOME (Sem espacos): "; cin >> NOME;
                cout << "CPF: "; cin >> CPF;
                cout << "DIA de nascimento: "; cin >> d;
                cout << "MES de nascimento: "; cin >> m;
                cout << "ANO de nascimento: "; cin >> a;
                cout << "SENHA: "; cin >> SENHA;
                
                banco.inserirOperador(NOME, CPF, SENHA, d, m, a);
                cout << "\nOperador cadastrado com sucesso!" << endl;
            }
            cin.ignore(); cin.get();
        }
        else if (OPCAO == 3) {
            cout << "--- ACESSO ROOT ---" << endl;
            cout << "CPF MASTER: "; cin >> CPF;
            cout << "SENHA MASTER: "; cin >> SENHA;
            
            if (CPF == cpfSuper && SENHA == senhaSuper) {
                int opRoot = 0;
                while(opRoot != 5) {
                    limparTela();
                    cout << "--- PAINEL ROOT ---" << endl;
                    cout << "1. Listar Todos" << endl;
                    cout << "2. Alterar por Nome" << endl;
                    cout << "3. Remover por Nome" << endl;
                    cout << "4. Relatorio de Transacoes do Dia" << endl;
                    cout << "5. Sair do Root" << endl;
                    cout << "Escolha: "; cin >> opRoot;
                    limparTela();

                    if(opRoot == 1) banco.listarTodos();
                    else if(opRoot == 2) {
                        string n; cout << "Nome para alterar: "; cin >> n;
                        banco.alterar(n);
                    }
                    else if(opRoot == 3) {
                        string n; cout << "Nome para remover: "; cin >> n;
                        banco.remover(n);
                    }
                    else if(opRoot == 4) {
                        string nomeOp; cout << "Nome do Operador do turno: "; cin >> nomeOp;
                        limparTela();
                        banco.exibirRelatorioTurno(nomeOp);
                    }
                    if(opRoot != 5) { cout << "\nPressione Enter..."; cin.ignore(); cin.get(); }
                }
            } else {
                cout << "Acesso Negado!" << endl;
                cin.ignore(); cin.get();
            }
        }
        else if (OPCAO == 4) return 0; 
    }

    // --- SAUDAÇÃO E ABERTURA ---
    time_t tzero = time(0);
    tm* tempoLocal = localtime(&tzero);
    int hora = tempoLocal->tm_hour;

    limparTela();
    if (hora >= 5 && hora < 12) cout << "Bom dia, " << operadorLogado->getNome() << "!" << endl;
    else if (hora >= 12 && hora < 18) cout << "Boa tarde, " << operadorLogado->getNome() << "!" << endl;
    else cout << "BOA NOITE, " << operadorLogado->getNome() << "!" << endl;
    
    double CEDULAS, MOEDAS;
    cout << "\nDECLARE OS VALORES DE ABERTURA DO SEU CAIXA" << endl;
    cout << "VALOR DAS CEDULAS: R$ "; cin >> CEDULAS;
    cout << "VALOR DAS MOEDAS: R$ "; cin >> MOEDAS;
    
    banco.setAbertura(CEDULAS + MOEDAS);
    
    limparTela();
    cout << "VALOR TOTAL DO CAIXA: R$ " << banco.getAbertura() << endl;
    cout << "TENHA UM OTIMO TRABALHO!" << endl;
    cout << "Pressione Enter para continuar...";
    cin.ignore(); cin.get();

    // --- MENU DE SERVIÇOS ---
    int OPCAO_SERV;
    while (true) {
        limparTela();
        cout << "====== MENU DE SERVICOS ======" << endl;
        cout << "1 - EXTRATO" << endl;
        cout << "2 - DEPOSITO" << endl;
        cout << "3 - SAQUE" << endl;
        cout << "4 - FECHAMENTO DE CAIXA (SAIR)" << endl;
        cout << "==============================" << endl;
        cout << "CAIXA ATUAL: R$ " << banco.getCaixa() << endl;
        cout << "Escolha: "; cin >> OPCAO_SERV;
        limparTela();

        if (OPCAO_SERV == 1) { 
            string numCartao, senhaCartao;
            cout << "--- EXTRATO ---" << endl;
            cout << "Numero do cartao: "; cin >> numCartao;
            cout << "Senha: "; cin >> senhaCartao;
            cout << "\n[COMPROVANTE] SALDO DISPONIVEL: R$ 1000000.00" << endl;
        } 
        else if (OPCAO_SERV == 2) { 
            string cpfDep, contaDest; double valorDep;
            cout << "--- DEPOSITO ---" << endl;
            cout << "CPF do Depositante: "; cin >> cpfDep;
            cout << "Conta Destinatario: "; cin >> contaDest;
            cout << "Valor: R$ "; cin >> valorDep;
            
            cout << "\nCONFERIU AS CEDULAS FISICAS? (1-SIM / 2-NAO): ";
            int conf; cin >> conf;
            if (conf == 1) {
                banco.registrarTransacao("DEPOSITO", valorDep);
                cout << "Deposito computado!" << endl;
            } else cout << "Operacao Cancelada." << endl;
        }
        else if (OPCAO_SERV == 3) { 
            double valorSaque;
            cout << "--- SAQUE ---" << endl;
            cout << "Valor: R$ "; cin >> valorSaque;
            if (valorSaque > banco.getCaixa()) cout << "ERRO: Valor em caixa insuficiente." << endl;
            else if (valorSaque > 1000.00) cout << "ERRO: Saldo do cliente insuficiente." << endl;
            else {
                banco.registrarTransacao("SAQUE", valorSaque);
                cout << "Saque realizado! Entregue o dinheiro ao cliente." << endl;
            }
        }
        else if (OPCAO_SERV == 4) { 
            double cedFim, moeFim;
            cout << "--- FECHAMENTO DE CAIXA ---" << endl;
            cout << "Conte o dinheiro fisico na gaveta." << endl;
            cout << "Total em Cedulas: R$ "; cin >> cedFim;
            cout << "Total em Moedas: R$ "; cin >> moeFim;
            
            double totalFisico = cedFim + moeFim;
            double esperado = banco.getCaixa();
            double diferenca = totalFisico - esperado;

            limparTela();
            cout << "=== RELATORIO DE FECHAMENTO ===" << endl;
            cout << "Operador: " << operadorLogado->getNome() << endl;
            cout << "Abertura: R$ " << banco.getAbertura() << endl;
            cout << "Esperado no Sistema: R$ " << esperado << endl;
            cout << "Total Fisico Informado: R$ " << totalFisico << endl;
            cout << "-------------------------------" << endl;
            
            if (diferenca < 0) cout << "[ALERTA] FALTOU DINHEIRO NO CAIXA: R$ " << diferenca * -1 << endl;
            else if (diferenca > 0) cout << "[ALERTA] SOBROU DINHEIRO NO CAIXA: R$ " << diferenca << endl;
            else cout << "[SUCESSO] O CAIXA BATEU PERFEITAMENTE!" << endl;
            
            cout << "\nFechamento concluido. Sistema encerrando..." << endl;
            break; 
        }
        
        cout << "\nPressione Enter para continuar...";
        cin.ignore(); cin.get();
    }
    return 0;
}