#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <stddef.h>
#include <Windows.h> 
#include <conio.h> 

int file_path_input(char **way);
int get_signature(unsigned char *si_re, int *smeshenie, FILE *f);
int chek_exefile(unsigned char *signature_for_chek, int displacement, int max_displacement, FILE *f, int *indicator);
int correct_file_type(FILE *f, int *indicator);

int main()
{
    if (SetConsoleCP(CP_UTF8)==0 || SetConsoleOutputCP(CP_UTF8)==0)
    {
        printf("Error! in function main: ошибка при подключении русского языка\n");
        getch();
        return 0;
    }
    int control, displacement, indicator=0, max_displacement;
    unsigned char base_signature[8], signature_for_chek[8];
    char *path_to_chekfile, *path_to_signature; 
    FILE *base_of_signature; 
    FILE *chek_file;
    control=printf("Введите путь к файлу с сигнатурной базой\n");
    if (control<0)
    {
        printf("Error! in fuction main: при выполнении функции printf\n");
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    control=file_path_input(&path_to_signature);
    if (control!=0) 
    {
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    control=printf("Введите путь к файлу, который нужно проверить\n");
    if (control<0)
    {
        printf("Error 2! in fuction main: при выполнении функции printf\n");
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    control=file_path_input(&path_to_chekfile);
    if (control!=0)
    {
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    } 

    chek_file=fopen(path_to_chekfile, "rb");
    if (chek_file==NULL)
    {
        printf("Error! in fuction main: при открытии проверяемого файла\n");
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    control=correct_file_type(chek_file, &indicator);
    if (control!=0) 
    {
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    if (indicator==1)
    {
        control=fclose(chek_file);
        if (control!=0)
        {
            printf("Error! in fuction main: при закрытии проверяемого файла\n");
            printf("Нажмите любую клавишу, чтобы завершить программу\n");
            getch();
            return 0;
        }
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    base_of_signature=fopen(path_to_signature, "r");
    if (base_of_signature==NULL)
    {
        printf("Error! in fuction main: при открытии базы сигнатур\n");
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    control=fseek(chek_file, 0, SEEK_END);
    if (control!=0)
    {
        printf("Error! in function main: при выполнении функции fseek \n");
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    max_displacement=ftell(chek_file);
    if (max_displacement==-1)
    {
        printf("Error! in fuction main: при выполнении функции ftell\n");
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    
    while(!feof(base_of_signature))
    {
        control=get_signature(base_signature, &displacement, base_of_signature);
        if (control!=0)
        {
            printf("Нажмите любую клавишу, чтобы завершить программу\n");
            getch();
            return 0;
        }
        control=chek_exefile(signature_for_chek, displacement, max_displacement, chek_file, &indicator);
        if (control!=0)
        {
            printf("Нажмите любую клавишу, чтобы завершить программу\n");
            getch();
            return 0;
        }
        if (indicator==1) continue;
        if(memcmp(signature_for_chek, base_signature, 8)==0) indicator=1;
        if (indicator==1)
        {
            control=printf("File is infected\n");
            if (control<0)
            {
                printf("Error! in fuction main: при выполнении функции printf");
                printf("Нажмите любую клавишу, чтобы завершить программу\n");
                getch();
                return 0;
            }
            control=printf("В файле %s обнаружена зараженная сигнатура по смещению %d\n", path_to_chekfile, displacement);
            if (control<0)
            {
                printf("Error! in fuction main: при выполнении функции printf");
                printf("Нажмите любую клавишу, чтобы завершить программу\n");
                getch();
                return 0;
            }
            for (int i=0; i<8; i++)
            {
                control=printf("%hhx ", signature_for_chek[i]);
                if (control<0)
                {
                    printf("Error! in fuction main: при выполнении функции printf");
                    printf("Нажмите любую клавишу, чтобы завершить программу\n");
                    getch();
                    return 0;
                }
            }
            printf("\n");
            break;
        }
    }
    if (indicator==0)
    {
        control=printf("File is clean\n");
        if (control<0)
        {
            printf("Error! in fuction main: при выполнении функции printf\n");
            printf("Нажмите любую клавишу, чтобы завершить программу\n");
            getch();
            return 0;
        }
    }
    control=fclose(chek_file);
    if (control!=0)
    {
        printf("Error! in fuction main: при закрытии проверяемого файла\n");
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    control=fclose(base_of_signature);
    if (control!=0)
    {
        printf("Error! in fuction main: при закрытии базы сигнатур\n");
        printf("Нажмите любую клавишу, чтобы завершить программу\n");
        getch();
        return 0;
    }
    free(path_to_signature);
    free(path_to_chekfile);
    printf("Нажмите любую клавишу, чтобы завершить программу\n");
    getch();
    return 0;
}

int chek_exefile(unsigned char *signature_for_chek, int displacement, int max_displacement, FILE *f, int *indicator)
{
    if (signature_for_chek==NULL)
    {
        printf("Error! in fuction chek_exefile: передача аргумента 1\n");
        return 1;
    }
    if (f==NULL)
    {
        printf("Error! in fuction chek_exefile: передача аргумента 4\n");
        return 4;
    }
    if (indicator==NULL)
    {
        printf("Error! in fuction chek_exefile: передача аргумента 5");
        return 5;
    }
    if (displacement<0)
    {
        printf("Error! in fuction chek_exefile: отрицательное смещение\n");
        return 2;
    }
    if ((max_displacement-7)<displacement)
    {
        *indicator=1;
        return 0;
    }
    *indicator=0;
    int control;
    control=fseek(f, displacement, SEEK_SET);
    if (control!=0)
    {
        printf("Error! in fuction chek_exefile: ошибка при выполнении функции fseek\n");
        return 6;
    }
    control=fread(signature_for_chek, sizeof(unsigned char), 8, f);
    if (control!=8)
    {
        printf("Error! in fuction chek_exefile: ошибка при выполнении функции fread\n");
        return 7;
    }
    return 0;
}

int get_signature(unsigned char *si_re, int *smeshenie, FILE *f)
{
    if (si_re==NULL)
    {
        printf("Error! in functuon get_signature: передача аргумента 1\n");
        return 1;
    }
    if (smeshenie==NULL)
    {
        printf("Error! in functuon get_signature: передача аргумента 2\n");
        return 2;
    }
    if (f==NULL)
    {
        printf("Error! in functuon get_signature: передача аргумента 3\n");
        return 3;
    }
    int control;
    for (int i=0; i<8; i++)
    {
        control=fscanf(f, "%x", &si_re[i]);
        if (control!=1)
        {
            printf("Error! in functuon get_signature: при считывании сигнатуры\n");
            return 4;
        }
    }
    control=fscanf(f, "%d", smeshenie);
    if (control!=1)
    {
        printf("Error! in functuon get_signature: при считываниb смещения\n");
        return 5;
    }
    return 0;
}

int file_path_input(char **way)
{
    if (way==NULL)
    {
        printf("Error! in function file_path_input: передача аргумента 1\n");
        return 1;
    }
    struct spisok
    {
        char simvol;
        struct spisok *next;
    };
    struct spisok *top, *p, *head;
    
    top=(struct spisok*)malloc(sizeof(struct spisok));
    if (top==NULL)
    {
        printf("Error! in function file_path_input: при выделении памяти\n");
        return 2;
    }
    int per;
    per=getchar();
    if (per==EOF)
    {
        printf("Error! in function file_path_input: при использовании getchar");
        return 3;
    }
    top->next=NULL;
    top->simvol=per;
    head=top;
    int n=0;
    per=getchar();
    if (per==EOF)
    {
        printf("Error! in function file_path_input: при использовании getchar");
        return 4;
    }
    while(per!='\n')
    {
        p=(struct spisok*)malloc(sizeof(struct spisok));
        if (p==NULL)
        {
            printf("Error! in function file_path_input: при выделении памяти\n");
            return 5;
        }
        p->simvol=per;
        p->next=NULL;
        top->next=p;
        top=p;
        n++;
        per=getchar();
        if (per==EOF)
        {
            printf("Error! in function file_path_input: при выполнении функции getchar");
            return 6;
        }
    }
    char *stroka;
    stroka=(char*)malloc(sizeof(char)*(n+1));
    if (stroka==NULL)
    {
        printf("Error! in function file_path_input: при выделении памяти\n");
        return 7;
    }
    top=head;
    int i=0;
    while(top)
    {
        stroka[i]=top->simvol;
        i++;
        top=top->next;
    }
    stroka[i]='\0';
    while(head)
    {
        p=head;
        head=head->next;
        free(p);
    }
    (*way)=stroka;
    return 0;
}

int correct_file_type(FILE *f, int *indicator)
{
    if (f==NULL)
    {
        printf("Error! in fuction correct_file_type: передача аргумента 1\n");
        return 1;
    }
    if (indicator==NULL)
    {
        printf("Error! in function correct_type_file: передача аргумента 2\n");
        return 2;
    }
    char type_file[2];
    int control;
    control=fread(type_file, sizeof(char), 2, f);
    if (control!=2)
    {
        printf("Error! in fuction correct_file_type: при выполнении функции fread\n");
        return 3;
    }
    if (type_file[0]!='M'||type_file[1]!='Z')
    {
        control=printf ("File isn't executable!\n");
        if (control<0)
        {
            printf("Error! in fuction correct_file_type: при выполнении printf\n");
            return 4;
        }
        *indicator=1;
    }
    return 0;
}