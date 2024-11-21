#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define TABLE_SIZE 10
#define PRIME 11  //deve ser primo maior que TABLE_SIZE

// Estrutura da tabela hash
typedef struct {
    int* table;
    bool* occupied;  // Array para rastrear posicoes ocupadas
    int size;
    int a, b;  
} HashTable;

//inicializa a tabela com hash universal
HashTable* createUniversalHashTable(int size) {
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->table = (int*)calloc(size, sizeof(int));
    hashTable->occupied = (bool*)calloc(size, sizeof(bool));
    hashTable->size = size;
    
    // inicializa os parametros a e b aleatoriamente
    srand(time(NULL));
    hashTable->a = rand() % (PRIME - 1) + 1;  // 1 <= a < prime
    hashTable->b = rand() % PRIME;  // 0 <= b < prime
    
    return hashTable;
}

// Funcao de hash universal
int universalHash(int key, HashTable* hashTable) {
    // h(k) = ((ak + b) mod prime) mod table_size
    return ((hashTable->a * key + hashTable->b) % PRIME) % hashTable->size;
}

// inserir um elemento na tabela hash usando tentativa quadratica
bool insert(HashTable* hashTable, int key) {
    // Encontra o indice inicial
    int index = universalHash(key, hashTable);
    int originalIndex = index;
    int k = 0;  // conta tentaticas

    // Tenta inserir usando sondagem quadratica
    while (k < hashTable->size) {
        // Se a posicao estiver vazia, insere
        if (!hashTable->occupied[index]) {
            hashTable->table[index] = key;
            hashTable->occupied[index] = true;
            printf("Chave %d inserida no indice %d (tentativa %d)\n", key, index, k);
            return true;
        }
        
        // Calcula o proximo indice usando tentativ quadratica
        // h(x,k) = (h(x,0) + k^2) % m
        k++;
        index = (originalIndex + k * k) % hashTable->size;
    }

    
    printf("Erro: Tabela hash cheia. Nao foi possivel inserir a chave %d\n", key);
    return false;
}

// Buscar um elemento na tabela hash
int search(HashTable* hashTable, int key) {
    // Encontra o indice inicial
    int index = universalHash(key, hashTable);
    int originalIndex = index;
    int k = 0;  //k = tentativas

    // encontrar com tentativa quadratica
    while (k < hashTable->size) {
        // Se a posicao estiver ocupada e o valor for igual
        if (hashTable->occupied[index] && hashTable->table[index] == key) {
            return index;
        }
        
        // se encontrar uma posicao nao ocupada, para de buscar
        if (!hashTable->occupied[index]) {
            break;
        }

        // Calcula o proximo indice
        k++;
        index = (originalIndex + k * k) % hashTable->size;
    }

    return -1;  // Elemento nao encontrado
}

// Liberar memoria da tabela hash
void freeHashTable(HashTable* hashTable) {
    free(hashTable->table);
    free(hashTable->occupied);
    free(hashTable);
}

// Funcao para imprimir a tabela hash
void printHashTable(HashTable* hashTable) {
    printf("\n--- Tabela Hash ---\n");
    for (int i = 0; i < hashTable->size; i++) {
        if (hashTable->occupied[i]) {
            printf("Indice %d: %d\n", i, hashTable->table[i]);
        }
    }
    printf("------------------\n");
}

// Funcao para limpar o buffer de entrada
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void displayMenu() {
    printf("\n--- Menu de Hashing Universal ---\n");
    printf("1. Inserir elemento\n");
    printf("2. Buscar elemento\n");
    printf("3. Imprimir Tabela Hash\n");
    printf("4. Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    HashTable* hashTable = createUniversalHashTable(TABLE_SIZE);
    int choice, key, result;
    
    printf("Bem-vindo ao Sistema de Hashing Universal!\n");
    printf("Parametros da Funcao Hash:\n");
    printf("Tamanho da Tabela: %d\n", TABLE_SIZE);
    printf("Numero Primo: %d\n", PRIME);
    printf("Parametros a e b gerados aleatoriamente\n");
    printf("Metodo de Colisao: Sondagem Quadratica\n");
    
    while (1) {
        displayMenu();
        
        // Verificar se a leitura foi bem sucedida
        if (scanf("%d", &choice) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            clearInputBuffer();
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Digite a chave a ser inserida: ");
                if (scanf("%d", &key) != 1) {
                    printf("Entrada invalida.\n");
                    clearInputBuffer();
                    break;
                }
                insert(hashTable, key);
                break;
            
            case 2:
                printf("Digite a chave a ser buscada: ");
                if (scanf("%d", &key) != 1) {
                    printf("Entrada invalida.\n");
                    clearInputBuffer();
                    break;
                }
                result = search(hashTable, key);
                if (result != -1) {
                    printf("Chave %d encontrada no indice %d\n", key, result);
                } else {
                    printf("Chave %d nao encontrada\n", key);
                }
                break;
            
            case 3:
                printHashTable(hashTable);
                break;
            
            case 4:
                printf("Encerrando o programa...\n");
                freeHashTable(hashTable);
                return 0;
            
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
        
        // Limpar buffer de entrada
        clearInputBuffer();
    }
    
    return 0;
}