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

void *load_data(char *name, size_t unit_size) {
    size_t size = get_file_size(name);
    size_t buffer_size = size / sizeof(char) * 8;
    
    FILE *fp = fopen(name, "r");

    char *buffer = malloc(buffer_size);
    char *unit_buffer = malloc(unit_size);

    memset(buffer, 0, buffer_size);

    char *desk = buffer;
    while (feof(fp) == 0) {
        memset(unit_buffer, 0, unit_size);
        load_values(fp, unit_buffer, unit_size);
        memcpy(desk, unit_size, unit_size);

        desk += unit_size;
    }

    fclose(fp);

    return buffer;
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
    printf("\n");
}

void print_signed_char(char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d", data[i]);
    }
    printf("\n");
}

void print_unsigned_char(unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%u", data[i]);
    }
    printf("\n");
}

void print_ascii(char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        char current = data[i];
        if (current >= 0 && current <= 127) {
            printf("%c", data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

void print_signed_int(int *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d", data[i]);
    }
    printf("\n");
}

void print_unsigned_int(unsigned int *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%u", data[i]);
    }
    printf("\n");
}

void print_signed_float(float *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%0.4f", data[i]);
    }
    printf("\n");
}

// ??
void print_unsigned_float(float *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%0.4f", data[i]);
    }
    printf("\n");
}

void print_signed_double(double *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        // ??
        printf("%0.4lf", data[i]);
    }
    printf("\n");
}

void print_unsigned_double(double *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        // ??
        printf("%0.4lf", data[i]);
    }
    printf("\n");
}

int main() {
    char *name = "/Users/siyual.park/Project/soongsil/soongsil-system-programing/assignments_1/input";
    
    size_t size = get_file_size(name);
    size_t data_size = size / 8;

    void *data = load_data(name, 8);

    print_data(data, data_size);

    print_signed_char(data, data_size);
    print_ascii(data, data_size);
    print_unsigned_char(data, data_size);

    print_signed_int(data, data_size / sizeof(int));
    print_unsigned_int(data, data_size / sizeof(int));

    print_signed_float(data, data_size / sizeof(float));
    print_unsigned_float(data, data_size / sizeof(float));

    print_signed_double(data, data_size / sizeof(double));
    print_unsigned_double(data, data_size / sizeof(double));

    free(data);
    return 0;
}
