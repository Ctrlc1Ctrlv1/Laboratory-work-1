#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

bool is_separator(char ch) {
    char separators[] = ",=+/?\\{}[]#.';:!&()_>< ";
    return strchr(separators, ch) != NULL || ch == '\n' || ch == EOF;
}

bool process_word(char* word, int* max_size) {
    int len = strlen(word);
    if (len > 30) {
        printf("Text has a word with the length more than 30 letters: %s\nThe program was stopped", word);
        return false; // Word exceeds maximum length
    }
    if (len > *max_size) {
        *max_size = len;
    }
    return true;
}

int main() {
    system("chcp 65001");
    setlocale(LC_ALL, "Ukrainian");

    const char* filename = "Test5.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char word[31];
    int word_len = 0;
    int max_size = 0;
    char ch;

    // First pass: Find the maximum length of the words
    while ((ch = fgetc(file)) != EOF) {
        if (is_separator(ch)) {
            if (word_len > 0) {
                word[word_len] = '\0';
                if (!process_word(word, &max_size)) {
                    fclose(file);
                    return EXIT_FAILURE;
                }
                word_len = 0;
            }
        } else {
            if (word_len < sizeof(word) - 1) {
                word[word_len++] = ch;
            }
        }
    }
    // Check if the last word needs to be processed
    if (word_len > 0) {
        word[word_len] = '\0';
        process_word(word, &max_size);
    }

    fclose(file);

    // Reopen the file for the second pass
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error reopening file");
        return EXIT_FAILURE;
    }

    FILE *fout = fopen("fin.txt", "w");
    if (fout == NULL) {
        perror("Error opening output file");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Second pass: Write all words with the maximum length to the output file
    word_len = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (is_separator(ch)) {
            if (word_len > 0) {
                word[word_len] = '\0';
                if (strlen(word) == max_size) {
                    fprintf(fout, "%s\n", word);
                }
                word_len = 0;
            }
        } else {
            if (word_len < sizeof(word) - 1) {
                word[word_len++] = ch;
            }
        }
    }
    // Again, check if the last word needs to be processed
    if (word_len > 0) {
        word[word_len] = '\0';
        if (strlen(word) == max_size) {
            fprintf(fout, "%s\n", word);
        }
    }

    fclose(file);
    fclose(fout);

    printf("Longest words written to fin.txt\n");
    printf("Length of the longest word: %d\n", max_size);

    return 0;
}
