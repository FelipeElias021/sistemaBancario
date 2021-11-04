#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>
#include <stdbool.h>
#include <windows.h> //usar o metodo sleep

/*Consultar saldo - pix - depositar - Investimento - Sair*/
/*Struct: nome - senha - email  - saldo - investimento - senha4*/

void login();
void menu();
void investimento();

int main() {
    //apenas começa o código levando ao login
    login();
    return 0;
}

struct dados_cliente {
        char nome[41];
        char senha[21];
        char email[41];
        char cpf[12];
        int conta;
        int senha4;
        float saldo;
        float investimento;
    };

struct dados_cliente cliente;


void login(){
    int escolhaLogin;
    bool flagEmail, flagSenha, flagValidacao;
    char validacaoUser[41],validacaoSenha[21];
    system("cls");
    printf("Bem vindo ao Banco ETI!\n");
    printf("> 1. Fazer login\n");
    printf("> 2. Cadastrar novo cliente\n> ");
    do { //equanto o numero for diferente das opções vai pedir
        scanf("%d",&escolhaLogin);
    } while (escolhaLogin != 1 && escolhaLogin != 2 && escolhaLogin != 1024);
    switch (escolhaLogin) {
        case 1:
            //pedir cpf/nome e senha
            if (cliente.conta == 0) { //se o cliente n tiver conta ainda, vai pedir o cadrasto, pausando e voltando ao login
                printf("\nCadastre primeiro\n");
                system("PAUSE");
                login();
            }

            flagValidacao = true; //deixando a flag true dnovo
            do {
                setbuf(stdin, NULL); //limpar para o fgets funcionar
                printf("\nNome / CPF: ");
                fgets(validacaoUser,41,stdin);
                printf("Senha: ");
                scanf("%s",&validacaoSenha);
                if (isalpha(validacaoUser[0])) { //verificar se é nome ou cpf (vai verificar se o primeiro char é letra ou n)
                    for (int i = 0; validacaoUser[i] != '\0'; i++) { //para passar por todos os char da string
                        if (i == 0 || cliente.nome[i - 1] == ' ') { //no primeiro char,e se no ultimo for um espaço[i - 1] vai deixar o char upper
                            validacaoUser[i] = toupper(validacaoUser[i]);
                        } else {
                            validacaoUser[i] = tolower(validacaoUser[i]);
                        }
                    }
                    if (strcmp(cliente.nome, validacaoUser) == 0) { //se o nome for igual, a flag fica false e sai do do...while
                        flagValidacao = false;
                    }
                } else {
                    int cont = 0; 
                    for(int i = 0;i <= 11; i++) {
                        if ((int)cliente.cpf[i] == (int)validacaoUser[i]) {
                            cont++;
                        }
                    }
                    if (cont == 11) {
                        flagValidacao = false;
                    }
                    /*Vai passar por cada char, converter em int e comparar se é igual,
                    se for vai incrementar na variável contadora, e se esta for igual a 11
                    q é o número de números de um cpf é pq todos são iguais e roda*/
                }

                if (strcmp(cliente.senha, validacaoSenha) != 0) {
                    flagValidacao = true;
                } //se a senha for diferente a flag volta a true e repete o processo
            } while (flagValidacao);

            menu(); //leva ao menu
            break;
        case 2:
            //pedir informações como nome, senha, email e a senha de 4 dígitos
            setbuf(stdin, NULL);
            printf("\n> Nome: ");
            fgets(cliente.nome,41,stdin);
            for (int i = 0; cliente.nome[i] != '\0'; i++) {
                if (i == 0 || cliente.nome[i - 1] == ' ') {
                    cliente.nome[i] = toupper(cliente.nome[i]);
                } else {
                    cliente.nome[i] = tolower(cliente.nome[i]);
                }
            } //para deixar as primeiras letras maiúsculas e as outras minúsculas

            do {
                printf("> CPF: ");
                scanf(" %s",&cliente.cpf);
                strlen(cliente.cpf) != 11 ? printf("\nCPF invalido, digite um valido !(CPF deve ter apenas 11 numeros)\n"):false;
            } while (strlen(cliente.cpf) != 11); //comprimento (total de char) tem q ser igual a 11 para continuar
            
            flagEmail = true;
            do {
                printf("> E-mail: ");
                scanf(" %s",&cliente.email);
                for (int i = 0;cliente.email[i] != '\0'; i++) { //poderia ser usado strlen na segunda parte, mas assim para no final da string, que é \0, por isso deve declarar o chat com 1 a mais
                    if (cliente.email[i] == '@') {
                        flagEmail = false;
                    }
                    cliente.email[i] = tolower(cliente.email[i]);
                }
                /*vai verificar cada char para ver se tem um @, se n tiver n é email
                e ainda vai deixar tudo minúsculo*/
                flagEmail ? printf("\nE-mail invalido!\n"):false;
            } while (flagEmail);

            printf("> Senha: ");
            scanf(" %s",&cliente.senha);

            flagSenha = true;
            do {
                printf("> Senha de 4 digitos para transacoes: ");
                scanf("%d",&cliente.senha4);
                if (cliente.senha4 / 1000 < 1 || cliente.senha4 / 1000 > 9) {
                    flagSenha ? printf("\nSenha deve conter apenas 4 digitos!\n"):false;
                } else {
                    flagSenha = false;
                }
                /*vai verificar se tem mesmo 4 digitos, se for digitado pro exemplo 444, a divisão
                desse número por 1000 é 0..., como a divisão é entre inteiro, e se for tipo 55555,
                vai ser 55 nesse caso, maior que 9*/
            } while (flagSenha);

            //os operadores ternários foram usados para imprimir mensagens quando está errado o camppo
            cliente.saldo = 0;
            cliente.investimento = 0;
            
            srand(time(NULL));
            //o rand vai pegar um semnte multiplicar por 9999 e depois somar 100.000.000 para ter o mesmo comprimento todos
            cliente.conta =  100000000 + rand() * 9999;

            login(); //leva de volta a função login, para se fazer o login
            break;

        case 1024:
            //"Opção de desenvolvedor" - mostrar todos os dados;
            printf("\nNome: %s\n",cliente.nome);
            printf("Email: %s\n",cliente.email);
            printf("Senha: %s\n",cliente.senha);
            printf("Cpf: %s\n",cliente.cpf);
            printf("Conta: %d\n",cliente.conta);
            printf("Senha 4 digitos: %d\n",cliente.senha4);
            printf("Saldo: %f\n",cliente.saldo);
            printf("Investimento: %f\n",cliente.investimento);
            system("PAUSE");
            login(); //leva de volta a função login, para se fazer o login
            break;
    }
    return;
}

void menu(){
    int escolhaMenu = 0;
    system("cls");
    printf("           Banco ETI\n\n");
    printf("Ola %s\n",cliente.nome);
    printf("Conta: %d\n",cliente.conta);
    printf("--------------------\n");
    printf("        Menu        \n\n");
    printf("> 1. Consultar Saldo\n");
    printf("> 2. Pix\n");
    printf("> 3. Depositar\n");
    printf("> 4. Investimento\n");
    printf("> 5. Sair\n> ");
    do {
        scanf("%d",&escolhaMenu);
    } while (escolhaMenu <  1 || escolhaMenu > 6);
    switch (escolhaMenu) {
        case 1:
            //consultar();
            break;
        case 2:
            //pix();
            break;
        case 3:
            //depositar();
            break;
        case 4:
            investimento();
            break;
        case 5:
            login();
            break;
    }
    return;
}

void investimento() {
    system("cls");
    printf("AREA INDISPONIVEL NO MOMENTO\n\nESPERE POR MAIS ATUALIZACOES\n\nMAS TOME AQUI 5 ETICOINS\n");
    cliente.investimento += 5;
    for (int i = 5; i >= 0; i--) {
            printf("\nAguarde %d segundos",i);
            Sleep(1000);
    }
    menu();
}