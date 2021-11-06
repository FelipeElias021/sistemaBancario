#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>
#include <stdbool.h>
//#include <windows.h> //usar o metodo sleep

/*Consultar saldo - pix - depositar - Investimento - Sair*/

void login();
void menu();
void investimento();
float resgatarDinheiro(float tipo);

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
        float investimentoFix;
        float investimentoVar;
};

struct dados_cliente cliente[10];
int i_atual = 0, nClientes = 0;
bool mostrarRenda = true;
char nomeMaiusculo[41];

void login(){
    int escolhaLogin;
    bool flagEmail, flagSenha, flagValidacao;
    char validacaoUser[41],validacaoSenha[21];
    system("cls");
    printf("Bem vindo ao Banco ETI!\n");
    printf("> 1. Fazer login\n");
    printf("> 2. Cadastrar novo cliente\n> ");
    scanf("%d",&escolhaLogin);
    switch (escolhaLogin) {
        case 1:
            //pedir cpf/nome e senha
            if (cliente[0].conta == 0) { //se o client 0 n tiver conta ainda, vai pedir o cadrasto, pausando e voltando ao login
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
                for (int j = 0; cliente[j].conta != 0; j++) {//fazer o teste em todos os clientes cadastros, por isso quando conta != 0
                    if (isalpha(validacaoUser[0])) { //verificar se é nome ou cpf (vai verificar se o primeiro char é letra ou n)
                        for (int i = 0; validacaoUser[i] != '\0'; i++) { //para passar por todos os char da string
                            validacaoUser[i] = tolower(validacaoUser[i]);//deixar tudo minúsculo
                        }
                        if (strcmp(cliente[j].nome, validacaoUser) == 0) { //se o nome for igual, a flag fica false e sai do do...while
                            flagValidacao = false;
                            i_atual = j; //armazena o index para fazer as transações
                            if (strcmp(cliente[j].senha, validacaoSenha) != 0) {
                                flagValidacao = true;
                            } //se a senha for diferente a flag volta a true e repete o processo
                        }
                    } else {
                        int cont = 0; 
                        for(int i = 0;i <= 11; i++) {
                            if ((int)cliente[j].cpf[i] == (int)validacaoUser[i]) {
                                cont++;
                            }
                        }
                        if (cont == 11) {
                            flagValidacao = false;
                            i_atual = j;
                            if (strcmp(cliente[j].senha, validacaoSenha) != 0) {
                                flagValidacao = true;
                            } 
                        }
                        /*Vai passar por cada char, converter em int e comparar se é igual,
                        se for vai incrementar na variável contadora, e se esta for igual a 11
                        q é o número de números de um cpf é pq todos são iguais e roda*/
                    }
                }
            } while (flagValidacao);

            menu(); //leva ao menu
            break;
        case 2:
            //pedir informações como nome, senha, email e a senha de 4 dígitos
            setbuf(stdin, NULL);
            printf("\n> Nome: ");
            fgets(cliente[nClientes].nome,41,stdin);
            for (int i = 0; cliente[nClientes].nome[i] != '\0'; i++) {
                cliente[nClientes].nome[i] = tolower(cliente[nClientes].nome[i]);
            } //para deixar as  letras minúsculas

            do {
                printf("> CPF: ");
                scanf(" %s",&cliente[nClientes].cpf);
                strlen(cliente[nClientes].cpf) != 11 ? printf("\nCPF invalido, digite um valido !(CPF deve ter apenas 11 numeros)\n"):false;
            } while (strlen(cliente[nClientes].cpf) != 11); //comprimento (total de char) tem q ser igual a 11 para continuar
            
            flagEmail = true;
            do {
                printf("> E-mail: ");
                scanf(" %s",&cliente[nClientes].email);
                for (int i = 0;cliente[nClientes].email[i] != '\0'; i++) { //poderia ser usado strlen na segunda parte, mas assim para no final da string, que é \0, por isso deve declarar o chat com 1 a mais
                    if (cliente[nClientes].email[i] == '@') {
                        flagEmail = false;
                    }
                    cliente[nClientes].email[i] = tolower(cliente[nClientes].email[i]);
                }
                /*vai verificar cada char para ver se tem um @, se n tiver n é email
                e ainda vai deixar tudo minúsculo*/
                flagEmail ? printf("\nE-mail invalido!\n"):false;
            } while (flagEmail);

            printf("> Senha: ");
            scanf(" %s",&cliente[nClientes].senha);

            flagSenha = true;
            do {
                printf("> Senha de 4 digitos para transacoes: ");
                scanf("%d",&cliente[nClientes].senha4);
                if (cliente[nClientes].senha4 / 1000 < 1 || cliente[nClientes].senha4 / 1000 > 9) {
                    flagSenha ? printf("\nSenha deve conter apenas 4 digitos!\n"):false;
                } else {
                    flagSenha = false;
                }
                /*vai verificar se tem mesmo 4 digitos, se for digitado pro exemplo 444, a divisão
                desse número por 1000 é 0..., como a divisão é entre inteiro, e se for tipo 55555,
                vai ser 55 nesse caso, maior que 9*/
            } while (flagSenha);
            //os operadores ternários foram usados para imprimir mensagens quando está errado o campo

            cliente[nClientes].saldo = 0;
            cliente[nClientes].investimentoFix = 0;
            cliente[nClientes].investimentoVar = 0;
            
            srand(time(NULL));
            //o rand vai pegar um semnte multiplicar por 9999 e depois somar 100.000.000 para ter o mesmo comprimento todos
            cliente[nClientes].conta =  100000000 + rand() * 9999;

            nClientes++; //Incrementa para o proximo cadastro

            login(); //leva de volta a função login, para se fazer o login
            break;

        case 1024:
            //"Opção de desenvolvedor" - mostrar todos os dados;
            for (int i = 0; cliente[i].conta != 0;i++) {
                printf("\nNome: %s\n",cliente[i].nome);
                printf("Email: %s\n",cliente[i].email);
                printf("Senha: %s\n",cliente[i].senha);
                printf("Cpf: %s\n",cliente[i].cpf);
                printf("Conta: %d\n",cliente[i].conta);
                printf("Senha 4 digitos: %d\n",cliente[i].senha4);
                printf("Saldo: %f\n",cliente[i].saldo);
                printf("Investimento Fixo: %f\n",cliente[i].investimentoFix);
                printf("Investimento Variavel: %f\n",cliente[i].investimentoVar);
            }
            system("PAUSE");
            login(); //leva de volta a função login, para se fazer o login
            break;
        case 3:
            //criar clientes para testes
            strcpy(cliente[nClientes].nome,"felipe\n");
            strcpy(cliente[nClientes].email,"felipeelias@gmail.com");
            strcpy(cliente[nClientes].senha,"12345");
            strcpy(cliente[nClientes].cpf,"12312312344");
            cliente[nClientes].conta =  100000000 + rand() * 9999;
            cliente[nClientes].senha4 = 1234;
            cliente[nClientes].saldo = 0;
            cliente[nClientes].investimentoFix = 0;
            cliente[nClientes].investimentoVar = 0;
            nClientes++;
            strcpy(cliente[nClientes].nome,"paulo\n");
            strcpy(cliente[nClientes].email,"paulosergio@gmail.com");
            strcpy(cliente[nClientes].senha,"Pp12345");
            strcpy(cliente[nClientes].cpf,"12345678900");
            cliente[nClientes].conta =  100000000 + rand() * 9999;
            cliente[nClientes].senha4 = 1234;
            cliente[nClientes].saldo = 0;
            cliente[nClientes].investimentoFix = 0;
            cliente[nClientes].investimentoVar = 0;
            nClientes++;
            strcpy(cliente[nClientes].nome,"joao\n");
            strcpy(cliente[nClientes].email,"joaomanjabosco@gmail.com");
            strcpy(cliente[nClientes].senha,"Jj12345");
            strcpy(cliente[nClientes].cpf,"98765432100");
            cliente[nClientes].conta =  100000000 + rand() * 9999;
            cliente[nClientes].senha4 = 1234;
            cliente[nClientes].saldo = 0;
            cliente[nClientes].investimentoFix = 0;
            cliente[nClientes].investimentoVar = 0;
            nClientes++;

            login();
            break;
        default:
            printf("Opcao Invalida\n");
            system("PAUSE");
            login();
    }
    return;
}

void menu(){
    int escolhaMenu = 0;
    system("cls");
    printf("           Banco ETI\n\n");
    printf("Ola %s\n",nomeMaiusculo);
    printf("Conta: %d\n",cliente[i_atual].conta);
    printf("--------------------\n");
    printf("        Menu        \n\n");
    printf("> 1. Consultar Saldo\n");
    printf("> 2. Pix\n");
    printf("> 3. Depositar\n");
    printf("> 4. Investimento\n");
    printf("> 5. Sair\n> ");
    scanf("%d",&escolhaMenu);
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
        default:
            printf("Opcao Invalida\n");
            system("PAUSE");
            menu();
    }
    return;
}

void investimento() {
    int escolhaInvestimento;
    char escolhaResgate[16];
    system("cls");
    printf("Bem vindo ao IETi %s\n",cliente[i_atual].nome);
    printf("Aqui voce faz investimentos em ETC como seguranca\n\n");
    if (mostrarRenda == true) { //se o mostrar renda for true vai mostrar a renda 
        printf("Renda variavel: %.2f ETC\n",cliente[i_atual].investimentoVar);
        printf("Renda fixa: %.2f ETC\n",cliente[i_atual].investimentoFix);
    }
    printf("--------------------------------------------------\n");
    printf("> 1. Investir em renda Variavel\n");
    printf("> 2. Investir em renda Fixa\n");
    printf("> 3. Resgatar dinheiro\n");
    if (mostrarRenda == true) { //se for true vai mostrar o saldo e colocar opcção para ocultar
        printf("> 4. Ocultar saldo\n");
    } else {
        printf("> 4. Mostrar saldo\n");
    }
    printf("> 5. Voltar ao menu\n> ");
    scanf("%d",&escolhaInvestimento);
    switch (escolhaInvestimento) {
        case 1:
            //Renda Variavel();
            cliente[i_atual].investimentoVar += 50; //teste
            break;
        case 2:
            //Renda Fixa();
            cliente[i_atual].investimentoFix += 50; //teste
            break;
        case 3:
            //resgatarDinheiro();
            setbuf(stdin, NULL); //limpar para o fgets funcionar
            printf("\nDeseja resgatar da renda variavel ou fixa?\n");
            fgets(escolhaResgate, 16,stdin);
            for (int i = 0; escolhaResgate[i] != '\0'; i++) { //para passar por todos os char da string
                escolhaResgate[i] = tolower(escolhaResgate[i]);//deixar tudo minúsculo
            }
            if (strncmp(escolhaResgate,"variavel",3) == 0) {//se pelo menos as 3 primeiras letras fore var vai vir aqui
                cliente[i_atual].investimentoVar = resgatarDinheiro(cliente[i_atual].investimentoVar);//chama a funcao e armazena o return no valor do investimento
            } else if (strncmp(escolhaResgate,"fixa",3) == 0) {
                cliente[i_atual].investimentoFix = resgatarDinheiro(cliente[i_atual].investimentoFix);
            } else {
                printf("\n\nDigite variavel ou fixa.\n");
                system("PAUSE");
            }
            investimento();
            break;
        case 4:
            //trocar a visualização
            if (mostrarRenda == false) {
                mostrarRenda = true;
            } else {
                mostrarRenda = false;
            }
            investimento();
            break;
        case 5:
            menu();
            break;
        default:
            printf("Opcao Invalida\n");
            system("PAUSE");
    }
    investimento();
}

float resgatarDinheiro(float tipo) {
    float resgate;
    printf("\n\nValor do resgate: ");
    scanf("%f",&resgate);
    if (resgate > tipo) { //se o resgate for maior que o disponivel n funciona
        printf("\nValor maior de resgate maior do que disponivel\n");
        system("PAUSE");
        investimento();
    }
    tipo -= resgate; //o investimento que pode ser variavel e  fixo é diminuido do valor do resgate
    cliente[i_atual].saldo += resgate; //o saldo é acrescido do valor do resgate

    printf("Tranferencia realizada com sucesso!\n"); //FAZER UMA "NOTA FICAL" E QUEM SABE ENVIAR UM EMAIL
    system("PAUSE");
    return tipo; //retornar o tipo que pode ser variavel ou fixo
}