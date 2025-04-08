#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100

typedef struct Node {
    char nome[MAX_NAME_LENGTH];
    char genero;
    double salario;
    struct Node* anterior;
    struct Node* proximo;
} Node;

Node* criarNo(const char* nome, char genero, double salario);
void inserirNo(Node** cabeca, Node* novoNo);
void ordenarLista(Node** cabeca);
void lerDoArquivo(const char* nomeArquivo, Node** cabeca);
void exibirCrescente(Node* cabeca);
void exibirDecrescente(Node* cauda);
void liberarLista(Node* cabeca);

int main() {
    Node* cabeca = NULL;
    int escolha;

    lerDoArquivo("file_example-ed.txt", &cabeca);

    ordenarLista(&cabeca);

    do {
        printf("1 - mostrar dados em ordem crescente\n");
        printf("2 - mostrar dados em ordem decrescente\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                exibirCrescente(cabeca);
                break;
            case 2:
                {
                    Node* cauda = cabeca;
                    while (cauda && cauda->proximo) {
                        cauda = cauda->proximo;
                    }
                    exibirDecrescente(cauda);
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (escolha != 0);

    liberarLista(cabeca);

    return 0;
}

Node* criarNo(const char* nome, char genero, double salario) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
    if (novoNo == NULL) {
        fprintf(stderr, "Falha na alocacao de memoria\n");
        exit(1);
    }
    strcpy(novoNo->nome, nome);
    novoNo->genero = genero;
    novoNo->salario = salario;
    novoNo->anterior = NULL;
    novoNo->proximo = NULL;
    return novoNo;
}

void inserirNo(Node** cabeca, Node* novoNo) {
    if (*cabeca == NULL) {
        *cabeca = novoNo;
    } else {
        Node* atual = *cabeca;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoNo;
        novoNo->anterior = atual;
    }
}

void ordenarLista(Node** cabeca) {
    if (*cabeca == NULL || (*cabeca)->proximo == NULL) {
        return;
    }

    Node* atual;
    Node* indice;
    char nomeTemporario[MAX_NAME_LENGTH];
    char generoTemporario;
    double salarioTemporario;

    for (atual = *cabeca; atual->proximo != NULL; atual = atual->proximo) {
        for (indice = atual->proximo; indice != NULL; indice = indice->proximo) {
            if (strcmp(atual->nome, indice->nome) > 0) {
                strcpy(nomeTemporario, atual->nome);
                strcpy(atual->nome, indice->nome);
                strcpy(indice->nome, nomeTemporario);

                generoTemporario = atual->genero;
                atual->genero = indice->genero;
                indice->genero = generoTemporario;

                salarioTemporario = atual->salario;
                atual->salario = indice->salario;
                indice->salario = salarioTemporario;
            }
        }
    }
}

void lerDoArquivo(const char* nomeArquivo, Node** cabeca) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        exit(1);
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        char nome[MAX_NAME_LENGTH];
        char genero;
        double salario;

        sscanf(linha, "%[^F^M]%c%lf", nome, &genero, &salario);

        int comprimento = strlen(nome);
        while (comprimento > 0 && nome[comprimento-1] == ' ') {
            nome[--comprimento] = '\0';
        }

        Node* novoNo = criarNo(nome, genero, salario);
        inserirNo(cabeca, novoNo);
    }

    fclose(arquivo);
}

void exibirCrescente(Node* cabeca) {
    Node* atual = cabeca;
    while (atual != NULL) {
        printf("-----------------------------------------------------------------\n");
        printf("Nome: %s\n", atual->nome);
        printf("Sexo: %c\n", atual->genero);
        printf("Salario: %.2f\n", atual->salario);
        atual = atual->proximo;
    }
    printf("-----------------------------------------------------------------\n");
}

void exibirDecrescente(Node* cauda) {
    Node* atual = cauda;
    while (atual != NULL) {
        printf("-----------------------------------------------------------------\n");
        printf("Nome: %s\n", atual->nome);
        printf("Sexo: %c\n", atual->genero);
        printf("Salario: %.2f\n", atual->salario);
        atual = atual->anterior;
    }
    printf("-----------------------------------------------------------------\n");
}

void liberarLista(Node* cabeca) {
    Node* atual = cabeca;
    while (atual != NULL) {
        Node* temporario = atual;
        atual = atual->proximo;
        free(temporario);
    }
}