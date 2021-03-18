#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME "/Users/siyual.park/Project/soongsil/soongsil-system-programing/assignments_1/input"


size_t get_file_size(char *name) {
    FILE *fp = fopen(name, "r");

    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);

    fclose(fp);

    return size;
}

void load_values(FILE *fp, char *buffer, size_t size) {
    size_t str_buffer_size = size * sizeof(char) * 8;

    char *str_buffer = calloc(str_buffer_size, sizeof(char));
    fread(str_buffer, sizeof(char), str_buffer_size, fp);

    unsigned char byte = 0;
    for (size_t i = 0; i < str_buffer_size; i++) {
        byte = byte << 1;

        char currnet = str_buffer[i];
        if (currnet == '1') {
            byte += 1;
        }

        if (((i + 1) % 8) == 0) {
            char *desk = buffer + size - ((i + 1) / 8);
            memcpy(desk, &byte, sizeof(char));
            byte = 0;
        }
    }

    free(str_buffer);
}

void *load_data(FILE *fp, size_t size, size_t unit_size) {
    size_t buffer_size = size / sizeof(char) * 8;
    
    char *buffer = malloc(buffer_size);
    char *unit_buffer = malloc(unit_size);

    memset(buffer, 0, buffer_size);

    char *desk = buffer;
    while (feof(fp) == 0) {
        memset(unit_buffer, 0, unit_size);
        load_values(fp, unit_buffer, unit_size);
        memcpy(desk, unit_buffer, unit_size);

        desk += unit_size;
    }

    free(unit_buffer);

    return buffer;
}


void print_data() {
    size_t size = get_file_size(NAME);
    size_t data_size = size / 8;

    FILE *fp = fopen(NAME, "r");
    
    char *data = load_data(fp, size, sizeof(char));

    fclose(fp);

    char maxPow = (char)(1 << (8-1));

    for (size_t i = 0; i < data_size; i++) {
        char current = data[i];
        for (size_t j = 0; j < 8; ++j) {
            printf("%u", current & maxPow ? 1 : 0);
            current = current << 1;
        }
        printf(" ");
    }
    printf("\n");
}

void print_signed_char() {
    size_t size = get_file_size(NAME);
    size_t data_size = size / 8;

    FILE *fp = fopen(NAME, "r");
    
    char *data = load_data(fp, size, sizeof(char));

    fclose(fp);

    for (size_t i = 0; i < data_size / sizeof(char); i++) {
        printf("%d", data[i]);
    }
    printf("\n");
}

void print_unsigned_char() {
    size_t size = get_file_size(NAME);
    size_t data_size = size / 8;

    FILE *fp = fopen(NAME, "r");
    
    unsigned char *data = load_data(fp, size, sizeof(unsigned char));

    fclose(fp);

    for (size_t i = 0; i < data_size / sizeof(unsigned char); i++) {
        printf("%u", data[i]);
    }
    printf("\n");
}

void print_ascii() {
    size_t size = get_file_size(NAME);
    size_t data_size = size / 8 / sizeof(char);

    FILE *fp = fopen(NAME, "r");
    
    char *data = load_data(fp, size, sizeof(char));

    fclose(fp);

    for (size_t i = 0; i < data_size; i++) {
        char current = data[i];
        if (current >= 0 && current <= 127) {
            printf("%c", data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");

    free(data);
}

void print_signed_int() {
    size_t size = get_file_size(NAME);
    size_t data_size = size / 8 / sizeof(int);

    FILE *fp = fopen(NAME, "r");
    
    int *data = load_data(fp, size, sizeof(int));

    fclose(fp);

    for (size_t i = 0; i < data_size; i++) {
        printf("%d", data[i]);
    }
    printf("\n");

    free(data);
}

void print_unsigned_int() {
    size_t size = get_file_size(NAME);
    size_t data_size = size / 8 / sizeof(unsigned int);

    FILE *fp = fopen(NAME, "r");
    
    unsigned int *data = load_data(fp, size, sizeof(unsigned int));

    fclose(fp);

    for (size_t i = 0; i < data_size; i++) {
        printf("%u", data[i]);
    }
    printf("\n");
    
    free(data);
}

void print_signed_float() {
    size_t size = get_file_size(NAME);
    size_t data_size = size / 8 / sizeof(float);

    FILE *fp = fopen(NAME, "r");
    
    float *data = load_data(fp, size, sizeof(float));

    fclose(fp);

    for (size_t i = 0; i < data_size; i++) {
        printf("%0.4f", data[i]);
    }
    printf("\n");
    
    free(data);
}

// ??
void print_unsigned_float() {
    size_t size = get_file_size(NAME);
    size_t data_size = size / 8 / sizeof(float);

    FILE *fp = fopen(NAME, "r");
    
    float *data = load_data(fp, size, sizeof(float));

    fclose(fp);

    for (size_t i = 0; i < data_size; i++) {
        printf("%0.4f", data[i]);
    }
    printf("\n");

    free(data);
}

void print_signed_double() {
    size_t size = get_file_size(NAME);
    size_t data_size = size / 8 / sizeof(double);

    FILE *fp = fopen(NAME, "r");
    
    double *data = load_data(fp, size, sizeof(double));

    fclose(fp);

    for (size_t i = 0; i < data_size; i++) {
        printf("%0.4lf", data[i]);
    }
    printf("\n");
    
    free(data);
}

void print_unsigned_double() {
    size_t size = get_file_size(NAME);
    size_t data_size = size / 8 / sizeof(double);

    FILE *fp = fopen(NAME, "r");
    
    double *data = load_data(fp, size, sizeof(double));

    fclose(fp);

    for (size_t i = 0; i < data_size; i++) {
        // ??
        printf("%0.4lf", data[i]);
    }
    printf("\n");

    free(data);
}

int main() {
    print_data();

    print_signed_char();
    print_ascii();
    print_unsigned_char();

    print_signed_int();
    print_unsigned_int();

    print_signed_float();
    print_unsigned_float();

    print_signed_double();
    print_unsigned_double();

    return 0;
}
