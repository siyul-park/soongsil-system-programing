#include <stdio.h>
#include "mystdio.h"
#include <assert.h>

#define TEST_BUF_SIZE 200
#define TEST_FILE_PATH "/Users/siyual.park/Project/soongsil/soongsil-system-programing/assignments_5/dump"

void test_prepare_alphabet(){
    MFILE *prepare = m_fopen(TEST_FILE_PATH, "w");
    m_fwrite("ABCDEFG", 1, 6, prepare);
    m_fclose(prepare);
}

void test_get_current_all_string(char* data){
    MFILE *read_data = m_fopen(TEST_FILE_PATH, "r");
    m_fread(data, sizeof(char), TEST_BUF_SIZE, read_data);
    m_fclose(read_data);
}

void test_fread(){
    test_prepare_alphabet();
    MFILE *file_ptr;
    char arr1[4] = {'1', '2', '3', '4'};
    char arr2[5] = {'5', '6', '7', '8', '9'};
    size_t iCount;

    file_ptr = m_fopen(TEST_FILE_PATH, "r");

    iCount = m_fread(arr1, sizeof(char), sizeof(arr1), file_ptr);
    assert(iCount == 4);
    assert(arr1[0] == 'A');
    assert(arr1[1] == 'B');
    assert(arr1[2] == 'C');
    assert(arr1[3] == 'D');

    iCount = m_fread(arr2, sizeof(char), sizeof(arr2), file_ptr);
    assert(iCount == 2);
    assert(arr2[0] == 'E');
    assert(arr2[1] == 'F');
    assert(arr2[2] == '7');
    assert(arr2[3] == '8');
    assert(arr2[4] == '9');
    assert(m_feof(file_ptr) == -1);
    m_fclose(file_ptr); 
    
    printf("[test] test_fread [success]\n");
}

void test_m_fwrite(){
    MFILE *file_ptr;
    char arr1[4] = {'1', '2', '3', '4'};
    char arr2[5] = {'5', '6', '7', '8', '9'};

    file_ptr = m_fopen(TEST_FILE_PATH, "w");

    int icount = m_fwrite(arr1, sizeof(char), sizeof(arr1), file_ptr);
    assert(icount == 4);

    icount = m_fwrite(arr2, sizeof(char), sizeof(arr2), file_ptr);
    assert(icount == 5);
    char data[200] = {'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'};
    test_get_current_all_string(data);
    assert(data[0] == '1');
    assert(data[1] == '2');
    assert(data[2] == '3');
    assert(data[3] == '4');
    assert(data[4] == '5');
    assert(data[5] == '6');
    assert(data[6] == '7');
    assert(data[7] == '8');
    assert(data[8] == 'c');

    assert(m_fclose(file_ptr) != EOF);

    char data2[200];
    test_get_current_all_string(data2);
    assert(data2[0] == '1');
    assert(data2[1] == '2');
    assert(data2[2] == '3');
    assert(data2[3] == '4');
    assert(data2[4] == '5');
    assert(data2[5] == '6');
    assert(data2[6] == '7');
    assert(data2[7] == '8');
    assert(data2[8] == '9');

    printf("[test] test_read_write [success]\n");
}

void test_read_write(){
    test_prepare_alphabet();
    MFILE *file_ptr;
    char arr0[1] = {'1'};
    char arr1[4] = {'a', 'b', 'c', 'd'};
    char arr2[3] = {'5', '6', '7'};
    char arr3[2] = {'5', '6'};

    file_ptr = m_fopen(TEST_FILE_PATH, "r+");

    int icount;

    icount = m_fread(arr0, sizeof(char), sizeof(arr0), file_ptr);
    assert(icount == 1);
    assert(arr0[0] == 'A');

    icount = m_fwrite(arr3, sizeof(char), sizeof(arr3), file_ptr);
    assert(icount == 2);

    m_fclose(file_ptr);

    char data[200];
    test_get_current_all_string(data);
    assert(data[0] == 'A');
    assert(data[1] == '5');
    assert(data[2] == '6');
    assert(data[3] == 'D');
    assert(data[4] == 'E');
    assert(data[5] == 'F');

    printf("[test] test_read_write [success]\n");
}

void test_write_read(){
    test_prepare_alphabet();
    MFILE *file_ptr;
    char arr0[1] = {'1'};
    char arr1[4] = {'a', 'b', 'c', 'd'};
    char arr2[3] = {'5', '6', '7'};
    char arr3[2] = {'5', '6'};

    file_ptr = m_fopen(TEST_FILE_PATH, "r+");

    int icount;

    icount = m_fwrite(arr2, sizeof(char), sizeof(arr2), file_ptr);
    assert(icount == 3);

    icount = m_fread(arr0, sizeof(char), sizeof(arr0), file_ptr);

    assert(icount == 1);
    assert(arr0[0] == 'D');
    assert(m_feof(file_ptr) != -1);

    icount = m_fread(arr3, sizeof(char), sizeof(arr3), file_ptr);

    assert(icount == 2);
    assert(arr3[0] == 'E');
    assert(arr3[1] == 'F');
    assert(m_feof(file_ptr) != -1);

    icount = m_fread(arr0, sizeof(char), sizeof(arr0), file_ptr);

    assert(icount == 0);
    assert(arr0[0] == 'D');
    assert(m_feof(file_ptr) == -1);

    m_fclose(file_ptr);

    char data[200];
    test_get_current_all_string(data);
    assert(data[0] == '5');
    assert(data[1] == '6');
    assert(data[2] == '7');
    assert(data[3] == 'D');
    assert(data[4] == 'E');
    assert(data[5] == 'F');

    printf("[test] test_write_read [success]\n");
}

void test_getc_putc(){
    test_prepare_alphabet();
    MFILE *file_ptr;
    char arr0[1] = {'1'};
    char arr1[4] = {'a', 'b', 'c', 'd'};
    char arr2[3] = {'5', '6', '7'};
    char arr3[1] = {'5'};

    file_ptr = m_fopen(TEST_FILE_PATH, "r+");

    int icount;

    icount = m_fgetc(file_ptr);
    assert(icount == 'A');

    for(int i=0; i<2; i++){
        icount = m_fputc(arr1[i], file_ptr);
    }
    
    m_fflush(file_ptr);

    m_fclose(file_ptr);

    char data[200];
    test_get_current_all_string(data);
    assert(data[0] == 'A');
    assert(data[1] == 'a');
    assert(data[2] == 'b');
    assert(data[3] == 'D');
    assert(data[4] == 'E');
    assert(data[5] == 'F');


    printf("[test] test_getc_putc [success]\n");
}

void test_putc_getc(){
    test_prepare_alphabet();
    MFILE *file_ptr;
    char arr0[1] = {'1'};
    char arr1[4] = {'a', 'b', 'c', 'd'};
    char arr2[3] = {'5', '6', '7'};
    char arr3[1] = {'5'};

    file_ptr = m_fopen(TEST_FILE_PATH, "r+");

    int icount;

    for(int i=0; i<3; i++){
        icount = m_fputc(arr1[i], file_ptr);
    }

    icount = m_fgetc(file_ptr);
    assert(icount == 'D');

    m_fclose(file_ptr);

    char data[200];
    test_get_current_all_string(data);
    assert(data[0] == 'a');
    assert(data[1] == 'b');
    assert(data[2] == 'c');
    assert(data[3] == 'D');
    assert(data[4] == 'E');
    assert(data[5] == 'F');

    printf("[test] test_putc_getc [success]\n");
}

void test_fgets(){
    test_prepare_alphabet();
    MFILE *file_ptr;
    file_ptr = m_fopen(TEST_FILE_PATH, "r");

    assert(m_fgetc(file_ptr) == 'A');
    assert(m_fgetc(file_ptr) == 'B');
    assert(m_fgetc(file_ptr) == 'C');
    assert(m_fgetc(file_ptr) == 'D');
    assert(m_fgetc(file_ptr) == 'E');
    assert(m_fgetc(file_ptr) == 'F');
    assert(m_feof(file_ptr) != -1);
    m_fgetc(file_ptr);
    assert(m_feof(file_ptr) == -1);
    m_fclose(file_ptr); 
    printf("[test] test_fgets [success]\n");
}

void test_fputs(){
    MFILE *file_ptr;
    file_ptr = m_fopen(TEST_FILE_PATH, "w");

    m_fputc('1', file_ptr);
    m_fputc('2', file_ptr);
    m_fputc('3', file_ptr);
    char data[200] = {'a'};
    test_get_current_all_string(data);
    assert(data[0] == 'a');

    m_fputc('4', file_ptr);
    m_fflush(file_ptr);
    
    char data2[200] = {'a'};
    test_get_current_all_string(data2);
    assert(data2[0] == '1');
    assert(data2[1] == '2');
    assert(data2[2] == '3');
    assert(data2[3] == '4');

    m_fputc('5', file_ptr);
    m_fputc('6', file_ptr);
    m_fputc('7', file_ptr);
    m_fputc('a', file_ptr);
    m_fputc('9', file_ptr);

    char data3[200] = {'c','c','c','c','c','c','c','c','c','c','c','c'};
    test_get_current_all_string(data3);
    assert(data3[0] == '1');
    assert(data3[1] == '2');
    assert(data3[2] == '3');
    assert(data3[3] == '4');
    assert(data3[4] == '5');
    assert(data3[5] == '6');
    assert(data3[6] == '7');
    assert(data3[7] == 'a');
    assert(data3[8] == 'c');

    m_fclose(file_ptr); 

    char data4[200] = {'c','c','c','c','c','c','c','c','c','c','c','c'};
    test_get_current_all_string(data4);
    assert(data4[0] == '1');
    assert(data4[1] == '2');
    assert(data4[2] == '3');
    assert(data4[3] == '4');
    assert(data4[4] == '5');
    assert(data4[5] == '6');
    assert(data4[6] == '7');
    assert(data4[7] == 'a');
    assert(data4[8] == '9');
    printf("[test] test_fputs [success]\n");
}

void test_fseek(){
    test_prepare_alphabet();
    MFILE *file_ptr;
    char arr0[1] = {'1'};
    char arr1[4] = {'a', 'b', 'c', 'd'};
    char arr2[3] = {'5', '6', '7'};
    char arr3[1] = {'5'};

    file_ptr = m_fopen(TEST_FILE_PATH, "r+");

    int icount;

    icount = m_fread(arr0, sizeof(char), sizeof(arr0), file_ptr);
    assert(icount == 1);
    assert(arr0[0] == 'A');
    m_fseek(file_ptr, -1, SEEK_CUR);

    icount = m_fread(arr0, sizeof(char), sizeof(arr0), file_ptr);
    assert(icount == 1);
    assert(arr0[0] == 'A');
    
    m_fclose(file_ptr);
    printf("[test] test_fseek [success]\n");
}

void test_read_fseek_write(){
    test_prepare_alphabet();
    MFILE *file_ptr;
    char arr0[1] = {'1'};
    char arr1[4] = {'a', 'b', 'c', 'd'};
    char arr2[3] = {'5', '6', '7'};
    char arr3[1] = {'5'};

    file_ptr = m_fopen(TEST_FILE_PATH, "r+");

    int icount;

    icount = m_fread(arr0, sizeof(char), sizeof(arr0), file_ptr);
    assert(icount == 1);
    assert(arr0[0] == 'A');
    m_fseek(file_ptr, 1, SEEK_CUR);

    icount = m_fwrite(arr1, sizeof(char), sizeof(arr1), file_ptr);
    m_fflush(file_ptr);
    assert(icount == 4);

    char data[TEST_BUF_SIZE];
    test_get_current_all_string(data);
    
    assert(data[0] == 'A');
    assert(data[1] == 'B');
    assert(data[2] == 'a');
    assert(data[3] == 'b');
    assert(data[4] == 'c');
    assert(data[5] == 'd');

    m_fclose(file_ptr);
    printf("[test] test_read_fseek_write [success]\n");
}

void test_write_fseek_read(){
    test_prepare_alphabet();
    MFILE *file_ptr;
    char arr0[1] = {'1'};
    char arr1[4] = {'a', 'b', 'c', 'd'};
    char arr2[3] = {'5', '6', '7'};
    char arr3[1] = {'5'};

    file_ptr = m_fopen(TEST_FILE_PATH, "r+");

    int icount;

    icount = m_fwrite(arr1, sizeof(char), sizeof(arr1), file_ptr);
    
    assert(icount == 4);

    m_fseek(file_ptr, 1, SEEK_CUR);


    icount = m_fread(arr0, sizeof(char), sizeof(arr0), file_ptr);
    assert(icount == 1);
    assert(arr0[0] == 'F');

    m_fclose(file_ptr);
    printf("[test] test_write_fseek_read [success]\n");
}

void test_getc_fseek_putc(){
    test_prepare_alphabet();

    MFILE *file_ptr;
    char arr0[1] = {'1'};
    char arr1[4] = {'a', 'b', 'c', 'd'};
    char arr2[3] = {'5', '6', '7'};
    char arr3[1] = {'5'};

    file_ptr = m_fopen(TEST_FILE_PATH, "r+");

    int icount;

    icount = m_fgetc(file_ptr);

    assert(icount == 'A');
    m_fseek(file_ptr, 1, SEEK_CUR);

    for(int i=0; i<3; i++){
        icount = m_fputc(arr1[i], file_ptr);
    }

    m_fflush(file_ptr);

    m_fclose(file_ptr);
    printf("[test] test_getc_fseek_putc [success]\n");
}

void test_putc_fseek_getc(){
    test_prepare_alphabet();

    MFILE *file_ptr;
    char arr0[1] = {'1'};
    char arr1[4] = {'a', 'b', 'c', 'd'};
    char arr2[3] = {'5', '6', '7'};
    char arr3[1] = {'5'};

    file_ptr = m_fopen(TEST_FILE_PATH, "r+");

    int icount;

    for(int i=0; i<3; i++){
        icount = m_fputc(arr1[i], file_ptr);
    }

    m_fflush(file_ptr);
    
    m_fseek(file_ptr, -1, SEEK_CUR);
    icount = m_fgetc(file_ptr);
    assert(icount == 'c');
    m_fclose(file_ptr);

    printf("[test] test_putc_fseek_getc [success]\n");
}

void test_feof(){
    test_prepare_alphabet();
    MFILE *file_ptr;
    char arr1[6];
    char arr2[2];
    size_t iCount;

    file_ptr = m_fopen(TEST_FILE_PATH, "r");

    iCount = m_fread(arr1, sizeof(char), sizeof(arr1), file_ptr);
    assert(iCount == 6);
    assert(m_feof(file_ptr) == 0);

    iCount = m_fread(arr2, sizeof(char), sizeof(arr2), file_ptr);
    assert(m_feof(file_ptr) == -1);
    m_fclose(file_ptr); 
    printf("[test] test_feof [success]\n");
}

void test_append_read_write(){
    test_prepare_alphabet();

    MFILE *file_ptr;
    char arr1[4] = {'1', '2', '3', '4'};
    char arr2[4] = {'1', '2', '3', '4'};
    size_t iCount;

    file_ptr = m_fopen(TEST_FILE_PATH, "a+");
    iCount = m_fread(arr1, sizeof(char), sizeof(arr1), file_ptr);
    assert(iCount == 4);
    assert(arr1[0] == 'A');
    assert(arr1[1] == 'B');
    assert(arr1[2] == 'C');
    assert(arr1[3] == 'D');

    iCount = m_fwrite(arr2, sizeof(char), sizeof(arr2), file_ptr);
    m_fflush(file_ptr);
    char data[TEST_BUF_SIZE];
    test_get_current_all_string(data);

    assert(iCount == 4);
    assert(arr1[0] == 'A');
    assert(arr1[1] == 'B');
    assert(arr1[2] == 'C');
    assert(arr1[3] == 'D');
    assert(arr1[4] == '1');
    assert(arr1[5] == '2');
    assert(arr1[6] == '3');
    assert(arr1[7] == '4');
    m_fclose(file_ptr);
    printf("[test] test_append_read_write [success]\n");
}


int main(int args, char *argv[]){
    test_fread();
    test_m_fwrite();
    test_read_write();
    test_write_read();
    test_getc_putc();
    test_putc_getc();
    test_fgets();
    test_fputs();
    test_fseek();
    test_read_fseek_write();
    test_write_fseek_read();
    test_getc_fseek_putc();
    test_putc_fseek_getc();
    test_feof();
    test_append_read_write();

    return 0;
}