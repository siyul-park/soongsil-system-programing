#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t get_file_size(char *name) {
    FILE *fp = fopen(name, "r");

    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);

    fclose(fp);

    return size;
}

void print_data(char *data, size_t size) {
    char maxPow = (char)(1 << (8-1));

    for (size_t i = 0; i < size; i++) {
        char current = data[i];
        for (size_t j = 0; j < 8; ++j) {
            printf("%u", current & maxPow ? 1 : 0);
            current = current << 1;
        }
        printf(" ");
    }
}

void *load_values(FILE *fp, size_t size) {
    size_t str_buffer_size = size * sizeof(char) * 8;

    char *str_buffer = calloc(str_buffer_size, sizeof(char));
    char *buffer = calloc(size, sizeof(char));

    fread(str_buffer, sizeof(char), str_buffer_size, fp);

    unsigned char byte = 0;
    for (size_t i = 0; i < str_buffer_size; i++) {
        byte = byte << 1;

        char currnet = str_buffer[i];
        if (currnet == '1') {
            byte += 1;
        }

        if (((i + 1) % 8) == 0) {
            memcpy(buffer + size - ((i + 1) / 8), &byte, sizeof(char));
            byte = 0;
        }
    }

    free(str_buffer);

    return buffer;
}

void *load_data(char *name, size_t unit_size) {
    size_t size = get_file_size(name);
    size_t buffer_size = size / sizeof(char) * 8;
    
    FILE *fp = fopen(name, "r");

    char *buffer = malloc(buffer_size);
    memset(buffer, 0, buffer_size);

    char *desk = buffer;
    while (feof(fp) == 0) {
        char *value_buffer = load_values(fp, unit_size);
        memcpy(desk, value_buffer, unit_size);
        free(value_buffer);

        desk += unit_size;
    }

    fclose(fp);

    return buffer;
}

int main() {
    char *name = "./input";
    size_t size = get_file_size(name);
    char *data = load_data(name, 8);

    print_data(data, size / 8);

    free(data);
    return 0;
}
