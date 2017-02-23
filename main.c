#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

int main()
{
    /*deschid fişierele*/
    FILE *input = fopen("tm.in","r");
    FILE *banda = fopen("tape.in","r");
    FILE *output = fopen("tape.out","w");

    /*verific daca citirea a reusit*/
    if(input == NULL)
        exit(-1);

    if(banda == NULL)
        exit(-1);

    /*funcţie cu care voi citi din fişier linie cu linie*/
    size_t getline(char **lineptr, size_t *n, FILE *stream) {
        char *bufptr = NULL;
        char *p = bufptr;
        size_t size;
        int c;

        if (lineptr == NULL) {
            return -1;
        }
        if (stream == NULL) {
            return -1;
        }
        if (n == NULL) {
            return -1;
        }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
    	return -1;
    }
    if (bufptr == NULL) {
    	bufptr = malloc(128);
    	if (bufptr == NULL) {
    		return -1;
    	}
    	size = 128;
    }
    p = bufptr;
    while(c != EOF) {
    	if ((p - bufptr) > (size - 1)) {
    		size = size + 128;
    		bufptr = realloc(bufptr, size);
    		if (bufptr == NULL) {
    			return -1;
    		}
    	}
    	*p++ = c;
    	if (c == '\n') {
    		break;
    	}
    	c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}

    int N, i = 0;
    ssize_t read;
	size_t len = 0;
    char *line = NULL;
	char **stariInitiale = malloc(902 * sizeof(char*));

	/*fac citirea din fişier, linie cu linie, despart în tokens
	şi îmi salvez datele*/
	char *token;
    read = getline(&line, &len, input);
	token = strtok(line, " '\n'");
	N = atoi(token);
	token = strtok(NULL, " '\n'");

	for(i = 0 ; i < N ; i++) {
        stariInitiale[i] = malloc(sizeof(char) * 10);
    }

    i = 0;

    while(token) {
        strcpy(stariInitiale[i], token);
        token = strtok(NULL, " '\n'");
        i++;
    }

    read = getline(&line, &len, input);
    token = strtok(line, " '\n'");
    int M;
    M = atoi(token);

    char **stariFinale = malloc(902 * sizeof(char*));
    for(i = 0 ; i < M ; i++) {
        stariFinale[i] = malloc(sizeof(char) * 10);
    }

    i = 0;

    token = strtok(NULL, " '\n'");

    while(token) {
        strcpy(stariFinale[i], token);
        token = strtok(NULL, " ");
        i++;
    }

    read = getline(&line, &len, input);
    token = strtok(line," '\n'");
    char *stareCurenta = malloc(sizeof(char) * 10);
    strcpy(stareCurenta, token);
    token = strtok(NULL, " '\n'");

    read = getline(&line, &len, input);
    token = strtok(line, " '\n'");
    int P = atoi(token);
    token = strtok(NULL, " '\n'");

    /*citesc banda şi o stochez în variabila codBanda, apoi o
    completez cu # până la 1000*/
    char codBanda[1002];
    size_t n = 0;
    fscanf(banda, "%s", codBanda);

    n = strlen(codBanda) - 1;
    while(n < 1000)
        codBanda[n++] = '#';
    codBanda[n] = '\0';

    char **stariTranzitii = malloc(9002 * sizeof(char*));
    for(i = 0 ; i < P ; i++) {
        stariTranzitii[i] = malloc(sizeof(char) * 10);
    }

    char **stariFinTranz = malloc(9002 * sizeof(char*));
    for(i = 0 ; i < P ; i++) {
        stariFinTranz[i] = malloc(sizeof(char) * 10);
    }

    char **simbolBanda = malloc(9002 * sizeof(char*));
    for(i = 0 ; i < P ; i++) {
        simbolBanda[i] = malloc(sizeof(char) * 2);
    }

    char **simbolTranzitie = malloc(9002 * sizeof(char*));
    for(i = 0 ; i < P ; i++) {
        simbolTranzitie[i] = malloc(sizeof(char) * 2);
    }

    char **pozitie = malloc(9002 * sizeof(char*));
    for(i = 0 ; i < P ; i++) {
        pozitie[i] = malloc(sizeof(char) * 2);
    }

    i = 0;

    while((read = getline(&line, &len, input)) != -1) {
	    token = strtok(line, " '\n'");
        strcpy(stariTranzitii[i], token);

        token = strtok(NULL, " '\n'");
        strcpy(simbolBanda[i], token);

        token = strtok(NULL, " '\n'");
        strcpy(stariFinTranz[i], token);

        token = strtok(NULL, " '\n'");
        strcpy(simbolTranzitie[i], token);

        token = strtok(NULL, " '\n'");
        strcpy(pozitie[i], token);

        i++;
	}

	int j, k, indexBanda = 1, ok = 0;
    i = -1; //încep cu i-ul de la -1 ca să îmi considere şi
            //prima tranziţie la începutul while-ului

    /*parcurg tranzitiile, incepand din stareaCurenta, actualizand
    mereu codBanda si stareCurenta cand gaseste o tranziţie ce indeplineste
    conditiile*/
	while(i < P) {
        ok = 0;
        i++;
        if(i == P)
            break;
        if(!strcmp(stareCurenta, stariTranzitii[i])) {
            for(j = i ; j < P && !strcmp(stareCurenta,stariTranzitii[j]) ; j++) {
                if(simbolBanda[j][0] == codBanda[indexBanda]) {
                    i = -1;
                    strcpy(stareCurenta,stariFinTranz[j]);
                    strncpy(&codBanda[indexBanda], simbolTranzitie[j], 1);
                    if(strncmp(pozitie[j], "R", 1) == 0)
                        indexBanda++;
                    else if(strncmp(pozitie[j], "L", 1) == 0)
                        indexBanda--;
                    for(k = 0 ; k < M ; k++) {
                        if(!strcmp(stareCurenta, stariFinale[k]))  {
                            fprintf(output, "%s", codBanda);
                            fclose(input);
                            fclose(banda);
                            return 0;
                        }
                    }
                break;
                }
            }
        }
	}
    fprintf(output, "Se agata!"); //în caz că se agaţă
    fclose(input);
    fclose(banda);


    /*eliberez memoria*/
    for(i = 0 ; i < N ; i++) {
        free(stariInitiale[i]);
    }

    free(stariInitiale);

    for(i = 0 ; i < M ; i++) {
        free(stariFinale[i]);
    }

    for(i = 0 ; i < P; i++) {
        free(stariTranzitii[i]);
        free(stariFinTranz[i]);
        free(simbolTranzitie[i]);
        free(simbolBanda[i]);
        free(pozitie[i]);
    }

    free(stareCurenta);
    free(stariFinTranz);
    free(stariTranzitii);
    free(simbolTranzitie);
    free(simbolBanda);
    free(pozitie);

    return 0;
}
