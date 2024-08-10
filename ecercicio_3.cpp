#include <stdio.h>olo
#include <stdlib.h>
#include <string.h>

// Struct para representar um pixel RGB
typedef struct {
    unsigned char r, g, b;
} Pixel;

int main() {
    char nome_arquivo[100];
    printf("Digite o nome do arquivo PPM (Não esquecer do .ppm): ");
    scanf("%s", nome_arquivo);

    FILE *fp_entrada, *fp_saida;

    // Abrir o arquivo PPM para leitura
    fp_entrada = fopen(nome_arquivo, "rb");
    if (fp_entrada == NULL) {
        printf("Erro na abertura do arquivo <%s>\n", nome_arquivo);
        return EXIT_FAILURE;
    }

    // Determinar o nome do arquivo de saída (sem a extensão .ppm)
    char nome_sem_extensao[100];
    strncpy(nome_sem_extensao, nome_arquivo, strlen(nome_arquivo) - 4); // Remove os últimos 4 caracteres (.ppm)
    nome_sem_extensao[strlen(nome_arquivo) - 4] = '\0'; // Adiciona terminador nulo

    // Abrir arquivo de saída para escrita (substituir o arquivo existente)
    fp_saida = fopen(strcat(nome_sem_extensao, ".txt"), "w");
    if (fp_saida == NULL) {
        printf("Erro na abertura do arquivo de saída\n");
        fclose(fp_entrada);
        return EXIT_FAILURE;
    }

    char id[3];
    int colunas, linhas, maximo;

    fscanf(fp_entrada, "%s", id);
    fscanf(fp_entrada, "%d", &colunas);
    fscanf(fp_entrada, "%d", &linhas);
    fscanf(fp_entrada, "%d", &maximo);

    fseek(fp_entrada, 1, SEEK_CUR); // Consumir o caractere de quebra de linha

    // Aloca matriz dinâmica para armazenar os pixels da imagem
    Pixel **imagem = (Pixel **)malloc(linhas * sizeof(Pixel *));
    for (int i = 0; i < linhas; i++) {
        imagem[i] = (Pixel *)malloc(colunas * sizeof(Pixel));
    }

    // Lê pixels da imagem
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fread(&imagem[i][j], sizeof(Pixel), 1, fp_entrada);
        }
    }

    // Conjunto de caracteres para representar diferentes tons de cinza
    char ascii_chars[] = " .:-=+*#%@";
    int num_chars = strlen(ascii_chars);

    // Converte pixels para ASCII Art e escreve no arquivo de saída
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            unsigned char gray = (imagem[i][j].r + imagem[i][j].g + imagem[i][j].b) / 3;
            int index = ((double)gray / 255) * (num_chars - 1);
            char ascii_char = ascii_chars[index];
            fprintf(fp_saida, "%c", ascii_char); // Escreve no arquivo de saída
            printf("%c", ascii_char); // Imprime no console
        }
        fprintf(fp_saida, "\n"); // Nova linha no arquivo de saída
        printf("\n"); // Nova linha no console
    }

    // Fecha os arquivos e libera memória alocada
    fclose(fp_entrada);
    fclose(fp_saida);
    for (int i = 0; i < linhas; i++) {
        free(imagem[i]);
    }
    free(imagem);

    return 0;
}