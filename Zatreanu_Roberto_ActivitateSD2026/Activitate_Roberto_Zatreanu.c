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

struct Telefon copiaza(struct Telefon t) {
    return initializare(t.id, t.capacitateBaterie, t.marca, t.pret, t.model);
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
    if (nrElementeCopiate <= nrElemente && nrElementeCopiate > 0) {
        struct Telefon* vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
        if (vectorNou == NULL) {
            return NULL;
        }

        for (int i = 0; i < nrElementeCopiate; i++) {
            vectorNou[i] = copiaza(vector[i]);
        }
        return vectorNou;
    }
    else {
        return NULL;
    }
}

void copiazaAnumiteElemente(struct Telefon* vector, int nrElemente, float prag, struct Telefon** vectorNou, int* dimensiune) {
    *dimensiune = 0;

    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].pret < prag) {
            (*dimensiune)++;
        }
    }

    *vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * (*dimensiune));
    if (*vectorNou == NULL && *dimensiune > 0) {
        *dimensiune = 0;
        return;
    }

    int k = 0;
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].pret < prag) {
            (*vectorNou)[k] = copiaza(vector[i]);
            k++;
        }
    }
}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
    struct Telefon s;
    s.id = -1;
    s.capacitateBaterie = 0;
    s.marca = NULL;
    s.pret = 0;
    s.model = '-';

    for (int i = 0; i < nrElemente; i++) {
        if (strcmp(conditie, vector[i].marca) == 0) {
            s = copiaza(vector[i]);
            return s;
        }
    }

    return s;
}

int main() {
    int n = 4;
    struct Telefon* v = (struct Telefon*)malloc(sizeof(struct Telefon) * n);

    if (v == NULL) {
        return 1;
    }

    v[0] = initializare(1, 2000, "Samsung", 200.00f, 'S');
    v[1] = initializare(2, 3000, "Apple", 500.00f, 'A');
    v[2] = initializare(3, 2500, "Samsung", 150.00f, 'M');
    v[3] = initializare(4, 4000, "Xiaomi", 180.00f, 'X');

    printf("Vector initial:\n");
    for (int i = 0; i < n; i++) {
        afisare(v[i]);
    }

    printf("\nPrimele 2 elemente copiate:\n");
    struct Telefon* primele2 = copiazaPrimeleNElemente(v, n, 2);
    if (primele2 != NULL) {
        for (int i = 0; i < 2; i++) {
            afisare(primele2[i]);
        }
    }

    printf("\nTelefoane cu pret mai mic decat 250:\n");
    struct Telefon* vectorFiltrat = NULL;
    int dimFiltrat = 0;
    copiazaAnumiteElemente(v, n, 250.0f, &vectorFiltrat, &dimFiltrat);
    for (int i = 0; i < dimFiltrat; i++) {
        afisare(vectorFiltrat[i]);
    }

    printf("\nPrimul telefon cu marca Samsung:\n");
    struct Telefon t = getPrimulElementConditionat(v, n, "Samsung");
    if (t.id != -1) {
        afisare(t);
        free(t.marca);
    }
    else {
        printf("Nu s-a gasit telefonul cautat.\n");
    }

    if (primele2 != NULL) {
        for (int i = 0; i < 2; i++) {
            free(primele2[i].marca);
        }
        free(primele2);
    }

    if (vectorFiltrat != NULL) {
        for (int i = 0; i < dimFiltrat; i++) {
            free(vectorFiltrat[i].marca);
        }
        free(vectorFiltrat);
    }

    dezalocare(&v, &n);

    return 0;
}