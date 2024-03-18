#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int set_max_size(int max_size, int numLetters) {
// set a new max length of word
    if (numLetters > max_size) {
        max_size = numLetters;
    }
    return max_size;
}

bool have_separate(char text_str) {
// check if symbol is separator
    char separator[] = ",=+/?\\{}[]#.';:!&()_><";
    bool have_separator = false;
        for (int i = 0; i < strlen(separator); i++) {
            if (separator[i] == text_str) {
                have_separator = true;
                break;
            }
        }
    return have_separator;
}

void replace_separator(const char *filename) {
// replace all kinds of separators to ' ' and writes changed text to file temp.txt
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Can't open file.\n");
        exit(1);
    }

    char tempName[] = "temp.txt";
    FILE *tempFile = fopen(tempName, "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Can't create temp file.\n");
        exit(1);
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (have_separate(ch)) {
            fputc(' ', tempFile);
        } else {
            fputc(ch, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);
}

void write_new_fin_file(char line[]){
// delete all words in fin.txt and write a new word
    FILE *fin_file = fopen("fin.txt", "w");
    fprintf(fin_file, "%s\n", line);
    fclose(fin_file);
}

bool word_already_appended(char line[]){
// check if the word has been written into the file fin.txt
    char fin_line[30];
    FILE *fin_file = fopen("fin.txt", "r+");
    while (fscanf(fin_file, "%s", fin_line) == 1){
        if (strcmp(line, fin_line) == 0){
            fclose(fin_file);
            return true;}
    }
    fclose(fin_file);
    return false;
}

void append_to_fin_file(char line[]){
// append word to file fin.txt
    FILE *fin_file = fopen("fin.txt", "a");
    if (!word_already_appended(line)){
        fprintf(fin_file, "%s\n", line);
    }
    fclose(fin_file);
}

int main() {
    replace_separator("text.txt");

    FILE *file = fopen("temp.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[30];
    int max_size = 1;
    int old_max_size;
    int num_of_words = 0;

    while (fscanf(file, "%s", line) == 1){
        old_max_size = max_size;
        int current_word_length = strlen(line);
        max_size = set_max_size(max_size, current_word_length);

        if(current_word_length > old_max_size){
            write_new_fin_file(line);
            num_of_words = 0;}
        else if(current_word_length == max_size){
            append_to_fin_file(line);
            num_of_words += 1;}
    }
    fclose(file);
    printf("No problem.\n");
    printf("Length of the longest word: %d", max_size);

    return 0;
}
