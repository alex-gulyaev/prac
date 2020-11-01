//utf-16 to utf-8
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    //ввод и вывод
    FILE *f;
    if (argc == 2){
        if ((f = freopen(argv[1], "r", stdin)) == NULL){
            fprintf(stderr, "can't open file for reading\n");
            return 1;
        }
    }
    else {
        if (argc == 3){
           if ((f = freopen(argv[1], "r", stdin)) == NULL){
                fprintf(stderr, "can't open file for reading\n");
                return 1;
           } 
           if ((f = freopen(argv[2], "w", stdout)) == NULL){
                fprintf(stderr, "can't open file for writting\n");
                return 1;
           }
        }
    }

    //чтение BOM ! для LE -> flag = 0; для BE -> flag = 1 !
    unsigned char bom[2];
    int c, flag = 0;
    c = fread(bom, sizeof(unsigned char), 2, stdin);
    if (c != 2){
        //некорректное представление
        fseek(stdin, 0, SEEK_SET);
    }
    else {
        if (bom[0] == 0xfe && bom[1] == 0xff){
            flag = 1;
        }
        else {
            if (bom[0] == 0xff && bom[1] == 0xfe){
                flag = 0;
            }
            else {
                //файл не соответствует соглашению
                fseek(stdin, 0, SEEK_SET);
            }
        }
    }

    //перекодировка
    unsigned char utf16[2];
    unsigned char utf8[3];
    unsigned char glass;
    while ((c = fread(utf16, sizeof(unsigned char), 2, stdin)) == 2){
        if (flag == 0){
            glass = utf16[0];
            utf16[0] = utf16[1];
            utf16[1] = glass;
        }
        if (utf16[0] == 0){
            putchar(utf16[1]);
        }
        else {
            if (utf16[0] < 0x08){
                glass = utf16[0] << 2;
                utf8[0] = utf16[1] >> 6;
                utf8[0] = utf8[0] | 0xc0;
                utf8[0] = utf8[0] | glass;

                utf8[1] = utf16[1] & 0x3f;
                utf8[1] = utf8[1] | 0x80;

                fwrite(utf8, sizeof(unsigned char), 2, stdout);
            }
            else {
                utf8[0] = utf16[0] >> 4;
                utf8[0] = utf8[0] | 0xe0;

                glass = utf16[0] << 4;
                utf8[1] = glass >> 2;
                glass = utf16[1] >> 6;
                utf8[1] = utf8[1] | glass;
                utf8[1] = utf8[1] | 0x80;

                utf8[2] = utf16[1] & 0x3f;
                utf8[2] = utf8[2] | 0x80;

                fwrite(utf8, sizeof(unsigned char), 3, stdout);
            }
        }
    }
    if (c == 1){
        fprintf(stderr, "odd number of bytes, error in file");
        return 1;
    }
}