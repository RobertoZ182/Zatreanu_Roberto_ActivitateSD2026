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
typedef struct Telefon Telefon;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

Telefon citireTelefonDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);

    Telefon t;
    char* aux;

    aux = strtok(buffer, sep);
    t.id = atoi(aux);

    t.capacitateBaterie = atoi(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    t.marca = malloc(strlen(aux) + 1);
    strcpy(t.marca, aux);

    t.pret = atof(strtok(NULL, sep));

    t.model = *strtok(NULL, sep);

    return t;
}

void afisareTelefon(Telefon t) {
    printf("Id: %d\n", t.id);
    printf("Capacitate baterie: %d\n", t.capacitateBaterie);
    printf("Marca: %s\n", t.marca);
    printf("Pret: %.2f\n", t.pret);
    printf("Model: %c\n\n", t.model);
}

void afisareListaTelefoane(Nod* cap) {
    while (cap != NULL) {
        afisareTelefon(cap->info);
        cap = cap->next;
    }
}

void adaugaTelefonInLista(Nod** cap, Telefon t) {
    Nod* nou = malloc(sizeof(Nod));
    nou->info = t;
    nou->next = NULL;

    if (*cap == NULL) {
        *cap = nou;
    }
    else {
        Nod* aux = *cap;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = nou;
    }
}

HashTable initializareHashTable(int dim) {
    HashTable ht;
    ht.dim = dim;
    ht.vector = malloc(sizeof(Nod*) * dim);

    for (int i = 0; i < dim; i++) {
        ht.vector[i] = NULL;
    }

    return ht;
}

int calculeazaHash(int id, int dim) {
    return id % dim;
}

void inserareTelefonInTabela(HashTable ht, Telefon t) {
    int poz = calculeazaHash(t.id, ht.dim);
    adaugaTelefonInLista(&(ht.vector[poz]), t);
}

HashTable citireTelefoaneDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");

    HashTable ht = initializareHashTable(5);

    if (f != NULL) {
        while (!feof(f)) {
            Telefon t = citireTelefonDinFisier(f);
            inserareTelefonInTabela(ht, t);
        }
        fclose(f);
    }
    else {
        printf("Fisierul nu a fost gasit!\n");
    }

    return ht;
}

void afisareTabela(HashTable ht) {
    for (int i = 0; i < ht.dim; i++) {
        printf("\n--- Cluster %d ---\n", i);
        afisareListaTelefoane(ht.vector[i]);
    }
}

void dezalocareLista(Nod** cap) {
    while (*cap != NULL) {
        Nod* temp = *cap;

        free(temp->info.marca);

        *cap = (*cap)->next;
        free(temp);
    }
}

void dezalocareTabela(HashTable* ht) {
    for (int i = 0; i < ht->dim; i++) {
        dezalocareLista(&(ht->vector[i]));
    }

    free(ht->vector);
    ht->vector = NULL;
    ht->dim = 0;
}

float* calculeazaPretMediu(HashTable ht, int* nrClustere) {
    *nrClustere = 0;

    for (int i = 0; i < ht.dim; i++) {
        if (ht.vector[i] != NULL) {
            (*nrClustere)++;
        }
    }

    float* medii = malloc(sizeof(float) * (*nrClustere));
    int index = 0;

    for (int i = 0; i < ht.dim; i++) {
        if (ht.vector[i] != NULL) {
            float suma = 0;
            int nr = 0;

            Nod* aux = ht.vector[i];
            while (aux != NULL) {
                suma += aux->info.pret;
                nr++;
                aux = aux->next;
            }

            medii[index++] = suma / nr;
        }
    }

    return medii;
}

Telefon cautaTelefon(HashTable ht, int id) {
    int poz = calculeazaHash(id, ht.dim);

    Nod* aux = ht.vector[poz];
    while (aux != NULL) {
        if (aux->info.id == id) {
            return aux->info;
        }
        aux = aux->next;
    }

    Telefon t;
    t.id = -1;
    t.marca = NULL;
    return t;
}


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

void afisareTelefon(struct Telefon telefon) {
    printf("Id: %d\n", telefon.id);
    printf("Capacitate baterie: %d\n", telefon.capacitateBaterie);
    printf("Marca: %s\n", telefon.marca);
    printf("Pret: %.2f\n", telefon.pret);
    printf("Model: %c\n\n", telefon.model);
}

void afisareVectorTelefoane(struct Telefon* telefoane, int nrTelefoane) {
    for (int i = 0; i < nrTelefoane; i++) {
        afisareTelefon(telefoane[i]);
    }
}

void adaugaTelefonInVector(struct Telefon** telefoane, int* nrTelefoane, struct Telefon telefonNou) {
    struct Telefon* aux = (struct Telefon*)malloc(sizeof(struct Telefon) * ((*nrTelefoane) + 1));

    for (int i = 0; i < *nrTelefoane; i++) {
        aux[i] = (*telefoane)[i];
    }

    aux[*nrTelefoane] = copiaza(telefonNou);

    free(*telefoane);
    *telefoane = aux;
    (*nrTelefoane)++;
}

struct Telefon citireTelefonFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";

    struct Telefon t;
    t.id = -1;
    t.capacitateBaterie = 0;
    t.marca = NULL;
    t.pret = 0;
    t.model = '-';

    if (fgets(buffer, 100, file) != NULL) {
        char* aux;
        aux = strtok(buffer, sep);
        if (aux != NULL) {
            t.id = atoi(aux);
            t.capacitateBaterie = atoi(strtok(NULL, sep));

            aux = strtok(NULL, sep);
            t.marca = (char*)malloc(strlen(aux) + 1);
            strcpy_s(t.marca, strlen(aux) + 1, aux);

            t.pret = (float)atof(strtok(NULL, sep));
            t.model = *strtok(NULL, sep);
        }
    }

    return t;
}

struct Telefon* citireVectorTelefoaneFisier(const char* numeFisier, int* nrTelefoaneCitite) {
    FILE* file = fopen(numeFisier, "r");
    struct Telefon* vector = NULL;
    *nrTelefoaneCitite = 0;

    if (file) {
        while (!feof(file)) {
            struct Telefon t = citireTelefonFisier(file);
            if (t.id != -1) {
                adaugaTelefonInVector(&vector, nrTelefoaneCitite, t);
                free(t.marca);
            }
        }
        fclose(file);
    }

    return vector;
}

void dezalocareVectorTelefoane(struct Telefon** vector, int* nrTelefoane) {
    if (*vector) {
        for (int i = 0; i < *nrTelefoane; i++) {
            free((*vector)[i].marca);
        }
        free(*vector);
        *vector = NULL;
        *nrTelefoane = 0;
    }
}
Telefon citireTelefonDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    Telefon t;

    t.id = -1;
    t.capacitateBaterie = 0;
    t.marca = NULL;
    t.pret = 0;
    t.model = '-';

    if (fgets(buffer, 100, file) != NULL) {
        char* aux;

        aux = strtok(buffer, sep);
        if (aux != NULL) {
            t.id = atoi(aux);

            aux = strtok(NULL, sep);
            if (aux != NULL) {
                t.capacitateBaterie = atoi(aux);
            }

            aux = strtok(NULL, sep);
            if (aux != NULL) {
                t.marca = (char*)malloc(strlen(aux) + 1);
                if (t.marca != NULL) {
                    strcpy_s(t.marca, strlen(aux) + 1, aux);
                }
            }

            aux = strtok(NULL, sep);
            if (aux != NULL) {
                t.pret = (float)atof(aux);
            }

            aux = strtok(NULL, sep);
            if (aux != NULL) {
                t.model = aux[0];
            }
        }
    }

    return t;
}

void afisareTelefon(Telefon telefon) {
    printf("Id: %d\n", telefon.id);
    printf("Capacitate baterie: %d\n", telefon.capacitateBaterie);
    printf("Marca: %s\n", telefon.marca);
    printf("Pret: %.2f\n", telefon.pret);
    printf("Model: %c\n\n", telefon.model);
}

void afisareListaTelefoane(Nod* lista) {
    while (lista) {
        afisareTelefon(lista->info);
        lista = lista->next;
    }
}

void adaugaTelefonInLista(Nod** lista, Telefon telefonNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = telefonNou;   // shallow copy, exact ca la profesor
    nou->next = NULL;

    if (*lista == NULL) {
        *lista = nou;
    }
    else {
        Nod* p = *lista;
        while (p->next) {
            p = p->next;
        }
        p->next = nou;
    }
}

void adaugaLaInceputInLista(Nod** lista, Telefon telefonNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = telefonNou;   // shallow copy
    nou->next = *lista;
    *lista = nou;
}

Nod* citireListaTelefoaneDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* lista = NULL;

    if (f != NULL) {
        while (!feof(f)) {
            Telefon t = citireTelefonDinFisier(f);
            if (t.id != -1) {
                adaugaTelefonInLista(&lista, t);
            }
        }
        fclose(f);
    }

    return lista;
}

void dezalocareListaTelefoane(Nod** lista) {
    while (*lista) {
        free((*lista)->info.marca);
        Nod* p = *lista;
        *lista = (*lista)->next;
        free(p);
    }
}

float calculeazaPretMediu(Nod* lista) {
    float suma = 0;
    int nr = 0;

    while (lista) {
        suma += lista->info.pret;
        nr++;
        lista = lista->next;
    }

    if (nr > 0) {
        return suma / nr;
    }
    return 0;
}

void stergeTelefoaneDinModel(Nod** lista, char modelCautat) {
    while (*lista && (*lista)->info.model == modelCautat) {
        Nod* temp = *lista;
        *lista = (*lista)->next;
        free(temp->info.marca);
        free(temp);
    }

    if (*lista == NULL) {
        return;
    }

    Nod* p = *lista;
    while (p->next) {
        if (p->next->info.model == modelCautat) {
            Nod* temp = p->next;
            p->next = temp->next;
            free(temp->info.marca);
            free(temp);
        }
        else {
            p = p->next;
        }
    }
}

float calculeazaPretulTelefoanelorUneiMarci(Nod* lista, const char* marcaCautata) {
    float suma = 0;

    while (lista) {
        if (strcmp(lista->info.marca, marcaCautata) == 0) {
            suma += lista->info.pret;
        }
        lista = lista->next;
    }

    return suma;
}
struct Nod {
    Telefon info;
    Nod* next;
    Nod* prev;
};

struct ListaDubla {
    Nod* prim;
    Nod* ultim;
};

Telefon initializareTelefon(int id, int capacitateBaterie, const char* marca, float pret, char model) {
    Telefon t;
    t.id = id;
    t.capacitateBaterie = capacitateBaterie;
    t.pret = pret;
    t.model = model;

    if (marca != NULL) {
        t.marca = (char*)malloc(strlen(marca) + 1);
        strcpy(t.marca, marca);
    }
    else {
        t.marca = NULL;
    }

    return t;
}

Telefon copiazaTelefon(Telefon t) {
    return initializareTelefon(t.id, t.capacitateBaterie, t.marca, t.pret, t.model);
}

void afisareTelefon(Telefon telefon) {
    printf("Id: %d\n", telefon.id);
    printf("Capacitate baterie: %d\n", telefon.capacitateBaterie);
    printf("Marca: %s\n", telefon.marca);
    printf("Pret: %.2f\n", telefon.pret);
    printf("Model: %c\n\n", telefon.model);
}

Telefon citireTelefonDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    Telefon t;

    t.id = -1;
    t.capacitateBaterie = 0;
    t.marca = NULL;
    t.pret = 0;
    t.model = '-';

    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        char* aux = strtok(buffer, sep);
        if (aux != NULL) {
            t.id = atoi(aux);

            aux = strtok(NULL, sep);
            if (aux != NULL) {
                t.capacitateBaterie = atoi(aux);
            }

            aux = strtok(NULL, sep);
            if (aux != NULL) {
                t.marca = (char*)malloc(strlen(aux) + 1);
                strcpy(t.marca, aux);
            }

            aux = strtok(NULL, sep);
            if (aux != NULL) {
                t.pret = (float)atof(aux);
            }

            aux = strtok(NULL, sep);
            if (aux != NULL) {
                t.model = aux[0];
            }
        }
    }

    return t;
}

void afisareListaTelefoane(ListaDubla lista) {
    Nod* p = lista.prim;
    while (p != NULL) {
        afisareTelefon(p->info);
        p = p->next;
    }
}

void adaugaTelefonInLista(ListaDubla* lista, Telefon telefonNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = copiazaTelefon(telefonNou);
    nou->next = NULL;
    nou->prev = lista->ultim;

    if (lista->prim == NULL) {
        lista->prim = nou;
        lista->ultim = nou;
    }
    else {
        lista->ultim->next = nou;
        lista->ultim = nou;
    }
}

void adaugaLaInceputInLista(ListaDubla* lista, Telefon telefonNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = copiazaTelefon(telefonNou);
    nou->prev = NULL;
    nou->next = lista->prim;

    if (lista->prim == NULL) {
        lista->prim = nou;
        lista->ultim = nou;
    }
    else {
        lista->prim->prev = nou;
        lista->prim = nou;
    }
}

ListaDubla citireLDTelefoaneDinFisier(const char* numeFisier) {
    ListaDubla lista;
    lista.prim = NULL;
    lista.ultim = NULL;

    FILE* f = fopen(numeFisier, "r");
    if (f != NULL) {
        while (!feof(f)) {
            Telefon t = citireTelefonDinFisier(f);
            if (t.id != -1) {
                adaugaTelefonInLista(&lista, t);
                free(t.marca);
            }
        }
        fclose(f);
    }

    return lista;
}

void dezalocareLDTelefoane(ListaDubla* lista) {
    Nod* p = lista->prim;
    while (p != NULL) {
        Nod* temp = p;
        p = p->next;
        free(temp->info.marca);
        free(temp);
    }

    lista->prim = NULL;
    lista->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
    float suma = 0;
    int nr = 0;

    Nod* p = lista.prim;
    while (p != NULL) {
        suma += p->info.pret;
        nr++;
        p = p->next;
    }

    if (nr == 0) {
        return 0;
    }

    return suma / nr;
}

void stergeTelefonDupaID(ListaDubla* lista, int id) {
    Nod* p = lista->prim;

    while (p != NULL) {
        if (p->info.id == id) {
            if (p->prev == NULL && p->next == NULL) {
                lista->prim = NULL;
                lista->ultim = NULL;
            }
            else if (p->prev == NULL) {
                lista->prim = p->next;
                lista->prim->prev = NULL;
            }
            else if (p->next == NULL) {
                lista->ultim = p->prev;
                lista->ultim->next = NULL;
            }
            else {
                p->prev->next = p->next;
                p->next->prev = p->prev;
            }

            free(p->info.marca);
            free(p);
            return;
        }
        p = p->next;
    }
}
char* getMarcaTelefonScump(ListaDubla lista) {
    if (lista.prim == NULL) {
        return NULL;
    }

    Nod* p = lista.prim;
    Nod* max = lista.prim;

    while (p != NULL) {
        if (p->info.pret > max->info.pret) {
            max = p;
        }
        p = p->next;
    }

    char* rezultat = (char*)malloc(strlen(max->info.marca) + 1);
    strcpy(rezultat, max->info.marca);
    return rezultat;
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

    printf("\n==============================\n");
    printf("1. afisareTelefon()\n");
    printf("==============================\n");
    afisareTelefon(v[0]);

    printf("\n==============================\n");
    printf("2. afisareVectorTelefoane()\n");
    printf("==============================\n");
    afisareVectorTelefoane(v, n);

    printf("\n==============================\n");
    printf("3. adaugaTelefonInVector()\n");
    printf("==============================\n");
    struct Telefon telefonNou = initializare(5, 4500, "Huawei", 220.0f, 'H');
    adaugaTelefonInVector(&v, &n, telefonNou);
    afisareVectorTelefoane(v, n);
    free(telefonNou.marca);

    printf("\n==============================\n");
    printf("4, 5, 6. Citire din fisier + afisare + dezalocare\n");
    printf("==============================\n");

    int nrTelefoaneFisier = 0;
    struct Telefon* vectorFisier = citireVectorTelefoaneFisier("telefoane.txt", &nrTelefoaneFisier);

    if (vectorFisier != NULL) {
        afisareVectorTelefoane(vectorFisier, nrTelefoaneFisier);
        dezalocareVectorTelefoane(&vectorFisier, &nrTelefoaneFisier);
    }
    else {
        printf("Fisierul telefoane.txt nu a putut fi deschis sau nu contine date.\n");
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

    ListaDubla lista;
    lista.prim = NULL;
    lista.ultim = NULL;

    Telefon t1 = initializareTelefon(1, 5000, "Samsung", 2200.5f, 'S');
    Telefon t2 = initializareTelefon(2, 4200, "Apple", 4500.0f, 'A');
    Telefon t3 = initializareTelefon(3, 6000, "Xiaomi", 1800.0f, 'X');
    Telefon t4 = initializareTelefon(4, 4800, "Huawei", 2100.0f, 'H');

    adaugaTelefonInLista(&lista, t1);
    adaugaTelefonInLista(&lista, t2);
    adaugaTelefonInLista(&lista, t3);
    adaugaLaInceputInLista(&lista, t4);

    printf("Lista initiala:\n\n");
    afisareListaTelefoane(lista);

    printf("Pret mediu: %.2f\n\n", calculeazaPretMediu(lista));

    char* marcaScump = getMarcaTelefonScump(lista);
    if (marcaScump != NULL) {
        printf("Marca telefonului cu pretul cel mai mare este: %s\n\n", marcaScump);
        free(marcaScump);
    }

    printf("Stergem telefonul cu id = 2\n\n");
    stergeTelefonDupaID(&lista, 2);
    afisareListaTelefoane(lista);

    dezalocareLDTelefoane(&lista);

    free(t1.marca);
    free(t2.marca);
    free(t3.marca);
    free(t4.marca);

    HashTable ht = citireTelefoaneDinFisier("telefoane.txt");

    afisareTabela(ht);

    printf("\nCautare telefon:\n");
    Telefon t = cautaTelefon(ht, 101);

    if (t.id != -1) {
        afisareTelefon(t);
    }
    else {
        printf("Telefonul nu a fost gasit!\n");
    }

    int nr;
    float* medii = calculeazaPretMediu(ht, &nr);

    printf("\nPret mediu pe clustere:\n");
    for (int i = 0; i < nr; i++) {
        printf("%.2f\n", medii[i]);
    }

    free(medii);

    dezalocareTabela(&ht);

    return 0;
    return 0;
}