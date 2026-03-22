#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Telefon {
    int id;
    int capacitateBaterie;
    char* marca;
    float pret;
    char model;
};

struct Telefon initializare(int id, int capacitate, const char* marca, float pret, char model) {
    struct Telefon t;
    t.id = id;
    t.capacitateBaterie = capacitate;

    t.marca = (char*)malloc(strlen(marca) + 1);
    if (t.marca == NULL) {
        
        t.marca = NULL;
    }
    else {
        strcpy_s(t.marca, strlen(marca) + 1, marca);
    }

    t.pret = pret;
    t.model = model;
    return t;
}

void afisare(struct Telefon t) {
    printf("%d. Telefonul %s model %c are un pret de %5.2f si o baterie cu capacitatea de %d.\n",
        t.id, t.marca ? t.marca : "(null)", t.model, t.pret, t.capacitateBaterie);
}

void modificare_Pret_Reducere(struct Telefon* t) {
    t->pret *= 0.9f;
}

void dezalocare(struct Telefon** vector, int* nrElemente) {
    if (vector == NULL || *vector == NULL || nrElemente == NULL) return;

    for (int i = 0; i < *nrElemente; i++) {
        free((*vector)[i].marca);
        (*vector)[i].marca = NULL;
    }

    free(*vector);
    *vector = NULL;
    *nrElemente = 0;
}
struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
    if (nrElementeCopiate < nrElemente) {
        		struct Telefon* vectorNou = malloc(sizeof(Telefon) * nrElementeCopiate);
        		for (int i = 0; i < nrElementeCopiate; i++) {
        			vectorNou[i] = copiaza(vector[i]);
        		}
        		return vectorNou;
        	}
        	else {
        		return NULL;
        	}
}
void copiazaAnumiteElemente(struct Telefon* vector, char nrElemente, float prag, struct Telefon** vectorNou, int* dimensiune) {
    	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].suprafata < prag) {
			(*dimensiune)++;
		}
	}
	*vectorNou = malloc(sizeof(Telefon) * (*dimensiune));
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].suprafata < prag) {
			(*vectorNou)[*dimensiune] = copiaza(vector[i]);
			(*dimensiune)++;
		}
	}
}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
        Telefon s;
    	s.id = -1;
    	s.denumire = NULL;
    	for (int i = 0; i < nrElemente; i++) {
    		if (strcmp(numeCautat, vector[i].denumire)==0) {
    			s = copiaza(vector[i]);
    		}
    	}
    	return s;
}
int main() {
   
    int n = 1;
    struct Telefon* v = (struct Telefon*)malloc(sizeof(struct Telefon) * n);
    v[0] = initializare(1, 2000, "Samsung", 200.00f, 'S');

    afisare(v[0]);

    dezalocare(&v, &n);  
    return 0;
}