/*Test program za iscitavanje fajlova iz odredjenog direktorijuma.

Obrati paznju ako postoji komentar u obliku "komentar 1", "komentar 2" itd prelistaj
na kraj fajla da bi procitao sadrzaj komentara.

Ne zanemari sto pisem cas engleski cas srpski, kad je neko objasnjenje u pitanju
cesce cu pisati na engelskom, a kad je u pitanju neki bug ili nesto malo ozbiljnije
onda cu pisati na srpskom.*/

#include <dirent.h>
#include <stdio.h>
#include <string.h>

typedef struct { 
    char id[50]; 
    char author[50]; 
    char name[50]; 
    char year[5]; 
} Books;

int fill_struct(Books* niz_knjiga, int max_knjiga, int* books_num) { 

    DIR *dir;   /*pointer to a file stream inside a directory*/
    struct dirent *de; /*struct that stores the characteristicts of a file*/
    FILE* fp;
    int c;
    int i, j, k = 0; /*local counters, for books, parsing of header and parsing of id*/

    char path[20]= "./biblioteka/";
    char header[20] = ""; 

    if((dir = opendir("biblioteka")) != NULL) { /*opendir returns a pointer to the first file inside a directory*/
        printf("Uspesno otvoren dir\n");
    } else {
        printf("could not open dir");
        return -1;
    }

    while(dir) {    /*goes around the directory until it gets a NULL value signaling it's at the end of directory if no errors have occured*/
        de = readdir(dir);  /*fills the dirent struct, mainly d_name and d_type, others might not be supported..*/
        if (!de) break;     /*if the pointer value equals to NULL, end of dir, break*/
        if(*(de->d_name) != '.') {  /*komentar 1*/
            if(i > 99) break;   /*safety restriction*/
            while(de->d_name[k] != '.') { /*while is used so that only the id is stored without .txt*/
                niz_knjiga[i].id[k] = de->d_name[k];
                printf("%s\n", niz_knjiga[i].id);
                k++;
            }
            k = 0;
            strcat(path, de->d_name);

            fp = fopen(path, "r");
            while(1) {
                c = fgetc(fp); /*takes byte by byte, that is char by char in ascii format*/
                if(c != '/' && j < 3) {
                    strcat(header, &c);
                } else {
                    if(j == 0) {
                        printf("\n%s\n", header);
                        strcpy(niz_knjiga[i].author, header);
                        strcpy(header, "");
                        j++;
                    } else if(j == 1) {
                        printf("\n%s\n", header);
                        strcpy(niz_knjiga[i].name, header);
                        strcpy(header, "");
                        j++;
                    } else if(j == 2){
                        printf("\n%s\n", header);
                        strcpy(niz_knjiga[i].year, header);
                        strcpy(header, "");
                        j++;
                    }
                }
             
                if(feof(fp)) {
                    break;
                }
                printf("%c", c);
            }
            fclose(fp);
            printf("\n");

            strcpy(path, "./biblioteka/");  //path reset for next cat
            j = 0;
            i++;
        }
    }
    *books_num = i;
    i = 0;  /*reset counter for future use*/
    closedir(dir);

    return 0;
}

void searchAll(Books* niz_knjiga, char* param, char* ret_string, int books_num) {   
    for(int i = 0; i < books_num; i++) {
        strcat(ret_string, "ID: ");
        strcat(ret_string, niz_knjiga[i].id);
        strcat(ret_string, "\n");
        strcat(ret_string, "AUTHOR: ");
        strcat(ret_string, niz_knjiga[i].author);
        strcat(ret_string, "\n");
        strcat(ret_string, "NAME: ");
        strcat(ret_string, niz_knjiga[i].name);
        strcat(ret_string, "YEAR: ");
        strcat(ret_string, "\n");
        strcat(ret_string, niz_knjiga[i].year);
        strcat(ret_string, "\n\n");
    }
    return;
}

void searchByOne(){} /*search by one parameter*/
void searchByAll(){} /*Za razliku od searchAll(), ovde trazi po parametrima odredjenu knjigu*/

int main() {
    Books niz_knjiga[100];
    int books_num = 0;  /*number of all books*/
    int max = 100;
    char param[50];
    char* ret_string[1000];
    fill_struct(niz_knjiga, max, &books_num);

    /*TODO Komentar 4*/
    /*TODO Komentar 3*/
    //scanf("%s", param);  /*TODO komentar 2*/
    scanf ("%[^\n]%*c", param); /*obican scanf prekida ucitavanje posle whitespace char????? moj zivot je laz*/

    printf("\n**************%s**********\n" , param);
    if(!strcmp("SearchAll", param)) {
        searchAll(niz_knjiga, param, ret_string, books_num);
        printf("%s\n" ,ret_string);
    }

    //TODO omoguci searchByOne i searchByAll

    return 0;
}

/*
Komentar 1: 
    Iz nekog razloga kada se prolazi kroz direktorijum i iscitavaju imena svih fajlova
    unutar direktorijuma desava se da se ispise . ili .. pored ostalih imena.
    Ovaj bug ne narusava ostale fajlove tako da je dovoljno samo ukloniti te
    tacke iz kriterijuma izlistavanja direktorijuma.
Komentar 2:
    Navodjenje parametara za pretragu, upisati u param klijentsku poruku umesto scanf
Komentar 3:
    Realizovati Login i Logout funkcionalnost odmah posle fill_struct tako da 
    program tj komunikacija klijenta i servera ne moze da se nastavi u koliko
    se ne poklapaju informacije trenutnog korisnika sa jednim clanom iz baze
    podataka svih clanova. Preporucujem upotrebu strukture user npr:

        typedef struct{
            char username[20];
            char password[20];
        }users;
    
    i u mainu deklarisati:
        users korisnici[10];
Komentar 4:
    Realizovati klijent server komunikaciju sve do prve regularne komunikacije
*/
