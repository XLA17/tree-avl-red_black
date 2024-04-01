#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tree-red-black.h"
#include <stdlib.h>
#include <time.h>

typedef struct {
    char *mot;
    char *def;
}       Couple;

// Fonction d'affichage pour les entiers.
void monPrintFInt (void * a, void * b, void * c){
    char * color;
    if (*(Color*)b == RED){
        color = "rouge";
    }
    else{
        color = "noir";
    }
    printf("Color : %s  --  Valeur : %d\n", color, *(int*)a);
}

// Fonction d'affichage pour les chaines de caractères.
void monPrintFString (void * a, void * b, void * c){
    char * color;
    if (*(Color*)b == RED){
        color = "rouge";
    }
    else{
        color = "noir";
    }
    printf("Color : %s  --  Valeur : %s\n", color, (char*)a);
}

// Fonction d'affichage pour le type Couple.
void monPrintFCouple(void * a, void * b, void * c){
    char * color;
    if (*(Color*)b == RED){
        color = "rouge";
    }
    else{
        color = "noir";
    }
    printf("Color : %s  --  Valeur : %s:%s\n", color, ((Couple *) a)->mot, ((Couple *) a)->def);
}

// Fonction de comparaison pour les entiers.
int monCompareInt (const void * a, const void * b){
    if (*(int*)a < *(int*)b){
        return -1;
    }
    else if (*(int*)a > *(int*)b){
        return 1;
    }
    else{
        return 0;
    }
}

// Fonction de comparaison pour les chaines de caractères.
int monCompareString (const void * a, const void * b){
    int res = strcmp((const char*)a, (const char*)b);
    if (res < 0){
        return -1;
    }
    else if (res > 0){
        return 1;
    }
    else{
        return 0;
    }
}

// Fonction de comparaison pour le type Couple.
int monCompareCouple(const void *a, const void *b){
    int res = strcmp(((Couple *) a)->mot, ((Couple *) b)->mot);
    if (res < 0){
        return -1;
    }
    else if (res > 0){
        return 1;
    }
    else{
        return 0;
    }
}

// Fonction qui retourne la taille d'une chaine de caractères.
size_t sizeString (const void * a){
    return strlen((const char*)a)+1;
}

void testArbresRougeNoir(void){
    initializeSentinel(); // Initialisation de la sentinelle.

    const int DATA = 127;
    int data[DATA];
    for (int i = 0; i < DATA; i++) {
        data[i] = i;
    }
    
    size_t sizeInt = sizeof(int);



    // tests ajouts

    Tree arbre = tree_new();

    printf("\n-- Test ajout cas 1 --\n");

    tree_add_data(&arbre, &arbre, &data[50], sizeInt, monCompareInt);

    assert(tree_height(arbre) == 1);
    assert(tree_size(arbre) == 1);
    assert(arbre->data[0] == data[50]);
    assert(arbre->color == BLACK);
    assert(isSentinel(arbre->left));
    assert(isSentinel(arbre->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test ajout cas 2 --\n");

    tree_add_data(&arbre, &arbre, &data[25], sizeInt, monCompareInt);
    tree_add_data(&arbre, &arbre, &data[75], sizeInt, monCompareInt);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->left->data[0] == data[25]);
    assert(arbre->right->data[0] == data[75]);
    assert(arbre->left->color == RED);
    assert(arbre->right->color == RED);
    assert(isSentinel(arbre->left->left));
    assert(isSentinel(arbre->left->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test ajout cas 3 --\n");

    tree_add_data(&arbre, &arbre, &data[12], sizeInt, monCompareInt);

    assert(tree_height(arbre) == 3);
    assert(tree_size(arbre) == 4);
    assert(arbre->left->left->data[0] == data[12]);
    assert(arbre->left->left->color == RED);
    assert(arbre->left->color == BLACK);
    assert(arbre->right->color == BLACK);
    assert(arbre->color == BLACK);
    assert(isSentinel(arbre->left->left->left));
    assert(isSentinel(arbre->left->left->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test ajout cas 4 & 5 --\n");

    tree_delete_data(&arbre, &arbre, &data[25], monCompareInt, NULL);
    tree_delete_data(&arbre, &arbre, &data[75], monCompareInt, NULL);
    tree_add_data(&arbre, &arbre, &data[25], sizeInt, monCompareInt);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->data[0] == data[25]);
    assert(arbre->left->data[0] == data[12]);
    assert(arbre->right->data[0] == data[50]);
    assert(arbre->color == BLACK);
    assert(arbre->left->color == RED);
    assert(arbre->right->color == RED);
    assert(isSentinel(arbre->left->left));
    assert(isSentinel(arbre->left->right));
    assert(isSentinel(arbre->right->left));
    assert(isSentinel(arbre->right->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");



    // tests suppression

    printf("\n-- Test suppression feuille (aucun fils) --\n");

    tree_delete_data(&arbre, &arbre, &data[50], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 2);
    assert(isSentinel(arbre->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppression noeud (un fils droit) --\n");

    tree_add_data(&arbre, &arbre, &data[50], sizeInt, monCompareInt);
    tree_add_data(&arbre, &arbre, &data[75], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &arbre, &data[50], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->right->data[0] == data[75]);
    assert(arbre->left->color == BLACK);
    assert(arbre->right->color == BLACK);
    assert(isSentinel(arbre->right->left));
    assert(isSentinel(arbre->right->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppression noeud (un fils gauche) --\n");

    tree_add_data(&arbre, &arbre, &data[50], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &arbre, &data[75], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->right->data[0] == data[50]);
    assert(arbre->right->color == BLACK);
    assert(isSentinel(arbre->right->left));
    assert(isSentinel(arbre->right->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppression noeud (deux fils) --\n");

    tree_delete_data(&arbre, &arbre, &data[25], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 2);
    assert(arbre->data[0] == data[12]);
    assert(arbre->color == BLACK);
    assert(arbre->right->color == RED);
    assert(isSentinel(arbre->left));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");




    // Tests suppression noeud noir => rééquilibrage

    printf("\n-- Test rééquilibrage suppression cas 1 et 2 (gauche) --\n");

    tree_add_data(&arbre, &arbre, &data[5], sizeInt, monCompareInt);
    tree_add_data(&arbre, &arbre, &data[75], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &arbre, &data[75], monCompareInt, NULL);
    tree_delete_data(&arbre, &arbre, &data[5], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 2);
    assert(arbre->data[0] == data[12]);
    assert(arbre->color == BLACK);
    assert(arbre->right->color == RED);
    assert(isSentinel(arbre->left));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test rééquilibrage suppression cas 3 (gauche) --\n");

    tree_add_data(&arbre, &arbre, &data[5], sizeInt, monCompareInt);
    tree_add_data(&arbre, &arbre, &data[75], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &arbre, &data[5], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->data[0] == data[50]);
    assert(arbre->left->data[0] == data[12]);
    assert(arbre->right->data[0] == data[75]);
    assert(arbre->color == BLACK);
    assert(arbre->left->color == BLACK);
    assert(arbre->right->color == BLACK);
    assert(isSentinel(arbre->left->left));
    assert(isSentinel(arbre->left->right));
    assert(isSentinel(arbre->right->left));
    assert(isSentinel(arbre->right->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test rééquilibrage suppression cas 4 (gauche) --\n");

    tree_add_data(&arbre, &arbre, &data[60], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &arbre, &data[12], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->data[0] == data[60]);
    assert(arbre->left->data[0] == data[50]);
    assert(arbre->right->data[0] == data[75]);
    assert(arbre->color == BLACK);
    assert(arbre->left->color == BLACK);
    assert(arbre->right->color == BLACK);
    assert(isSentinel(arbre->left->left));
    assert(isSentinel(arbre->left->right));
    assert(isSentinel(arbre->right->left));
    assert(isSentinel(arbre->right->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test rééquilibrage suppression cas 5 (gauche) --\n");

    tree_add_data(&arbre, &arbre, &data[70], sizeInt, monCompareInt);
    tree_add_data(&arbre, &arbre, &data[80], sizeInt, monCompareInt);
    tree_add_data(&arbre, &arbre, &data[90], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &arbre, &data[90], monCompareInt, NULL);
    tree_delete_data(&arbre, &arbre, &data[50], monCompareInt, NULL);

    assert(tree_height(arbre) == 3);
    assert(tree_size(arbre) == 4);
    assert(arbre->data[0] == data[75]);
    assert(arbre->left->data[0] == data[60]);
    assert(arbre->left->right->data[0] == data[70]);
    assert(arbre->right->data[0] == data[80]);
    assert(arbre->color == BLACK);
    assert(arbre->left->color == BLACK);
    assert(arbre->left->right->color == RED);
    assert(arbre->right->color == BLACK);
    assert(isSentinel(arbre->left->left));
    assert(isSentinel(arbre->left->right->left));
    assert(isSentinel(arbre->left->right->right));
    assert(isSentinel(arbre->right->left));
    assert(isSentinel(arbre->right->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test rééquilibrage suppression cas 1 et 2 (droit) --\n");

    tree_delete_data(&arbre, &arbre, &data[70], monCompareInt, NULL);
    tree_delete_data(&arbre, &arbre, &data[80], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 2);
    assert(arbre->data[0] == data[75]);
    assert(arbre->color == BLACK);
    assert(arbre->left->color == RED);
    assert(isSentinel(arbre->right));
    assert(isSentinel(arbre->left->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test rééquilibrage suppression cas 3 (droit) --\n");

    tree_add_data(&arbre, &arbre, &data[80], sizeInt, monCompareInt);
    tree_add_data(&arbre, &arbre, &data[50], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &arbre, &data[80], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->data[0] == data[60]);
    assert(arbre->left->data[0] == data[50]);
    assert(arbre->right->data[0] == data[75]);
    assert(arbre->color == BLACK);
    assert(arbre->left->color == BLACK);
    assert(arbre->right->color == BLACK);
    assert(isSentinel(arbre->left->left));
    assert(isSentinel(arbre->left->right));
    assert(isSentinel(arbre->right->left));
    assert(isSentinel(arbre->right->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test rééquilibrage suppression cas 4 (droit) --\n");

    tree_add_data(&arbre, &arbre, &data[55], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &arbre, &data[75], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->data[0] == data[55]);
    assert(arbre->left->data[0] == data[50]);
    assert(arbre->right->data[0] == data[60]);
    assert(arbre->color == BLACK);
    assert(arbre->left->color == BLACK);
    assert(arbre->right->color == BLACK);
    assert(isSentinel(arbre->left->left));
    assert(isSentinel(arbre->left->right));
    assert(isSentinel(arbre->right->left));
    assert(isSentinel(arbre->right->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test rééquilibrage suppression cas 5 (droit) --\n");

    tree_add_data(&arbre, &arbre, &data[40], sizeInt, monCompareInt);
    tree_add_data(&arbre, &arbre, &data[52], sizeInt, monCompareInt);
    tree_add_data(&arbre, &arbre, &data[30], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &arbre, &data[30], monCompareInt, NULL);
    tree_delete_data(&arbre, &arbre, &data[60], monCompareInt, NULL);

    assert(tree_height(arbre) == 3);
    assert(tree_size(arbre) == 4);
    assert(arbre->data[0] == data[50]);
    assert(arbre->left->data[0] == data[40]);
    assert(arbre->right->data[0] == data[55]);
    assert(arbre->right->left->data[0] == data[52]);
    assert(arbre->color == BLACK);
    assert(arbre->left->color == BLACK);
    assert(arbre->right->color == BLACK);
    assert(arbre->right->left->color == RED);
    assert(isSentinel(arbre->right->left->left));
    assert(isSentinel(arbre->right->left->right));
    assert(isSentinel(arbre->right->right));
    assert(isSentinel(arbre->left->left));
    assert(isSentinel(arbre->left->right));

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    tree_delete(arbre, NULL);



    // tests itératifs

    printf("\n-- Test ajout de données --\n");

    Tree arbre2 = tree_new();

    // ajout des DATA valeurs
    for (int i = 0; i < DATA; i++) {
        tree_add_data(&arbre2, &arbre2, &data[i], sizeInt, monCompareInt);
    }

    assert(tree_size(arbre2) == DATA);

    printf("\nTri post-order : \n");
    tree_post_order(arbre2, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppression de données --\n");

    // suppression des 50 premières valeurs
    for (int i = 0; i < 50; i++) {
        tree_delete_data(&arbre2, &arbre2, &data[i], monCompareInt, NULL);
    }

    assert(tree_size(arbre2) == (DATA - 50));

    printf("\nTri post-order : \n");
    tree_post_order(arbre2, monPrintFInt, NULL);
    printf("\nTests OK ...\n");



    // test recherche

    assert(tree_search(arbre2, &data[0], monCompareInt) == NULL);
    assert(tree_search(arbre2, &data[49], monCompareInt) == NULL);
    assert(tree_search(arbre2, &data[50], monCompareInt)->data[0] == data[50]);
    assert(tree_search(arbre2, &data[95], monCompareInt)->data[0] == data[95]);
    assert(tree_search(arbre2, &data[126], monCompareInt)->data[0] == data[126]);

    tree_delete(arbre2, NULL);



    // test chaines de caractères

    printf("\n-- Test avec chaines de caractères --\n");

    Tree arbre3 = tree_new();

    char * str[] = {"fonction", "TEA", "programmation", "C", "code", };

    for (int i = 0; i < (sizeof(str) / sizeof(str[0])); i++){
        tree_add_data(&arbre3, &arbre3, str[i], sizeString(str[i]), monCompareString);
    }

    printf("\nTri post-order : \n");
    tree_post_order(arbre3, monPrintFString, NULL);
    printf("\nTests OK ...\n");

    tree_delete(arbre3, NULL);

    printf("\n-- Test avec type dictionnaire --\n");
    // Fonctionne 1/2 fois à l'exécution (je ne sais pas pourquoi)

    Tree arbre4 = tree_new();

    int nbMots = 4;
    Couple *c = malloc(nbMots * sizeof(Couple));

    for (int i = 0; i < nbMots; i++) {
        c[i].mot = malloc(10);
        c[i].def = malloc(10);
        sprintf(c[i].mot, "mot%d", i + 1);
        sprintf(c[i].def, "def%d", i + 1);
        tree_add_data(&arbre4, &arbre4, &c[i], sizeof(Couple), monCompareCouple);
    }

    printf("\nTri in-order : \n");
    tree_post_order(arbre4, monPrintFCouple, NULL);
    printf("\nTests OK ...\n");
    
    tree_delete(arbre4, NULL);



    // calcul temps d'exécution

    /*
    clock_t start, end;
    double tpsAdd, tpsSearch, tpsDelete;
    const int nbTests = 20;
    const int nbCycles = 20;
    int nbDonnees = 50;
    int *donnees = malloc(nbDonnees * sizeof(int));

    srand(time(NULL));

    FILE *fichier = fopen("donnees_tps", "w");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nbCycles; i++){
        for (int j = 0; j < nbTests; j++){

            Tree arbre5 = tree_new();

            start = clock(); // Début du chronomètre
            for (int k = 0; k < nbDonnees; k++) {
                donnees[k] = rand(); // Génère un nombre pseudo-aléatoire
                tree_add_data(&arbre5, &arbre5, &donnees[k], sizeof(int), monCompareInt);
            }
            end = clock(); // Fin du chronomètre
            assert(tree_size(arbre5) == nbDonnees);
            tpsAdd += ((double) (end - start)) / CLOCKS_PER_SEC; // Calcul du temps d'exécution

            start = clock();
            for (int k = 0; k < nbDonnees; k++) {
                tree_search(arbre5, &donnees[k], monCompareInt);
            }
            end = clock();
            tpsSearch += ((double) (end - start)) / CLOCKS_PER_SEC;

            start = clock();
            for (int k = 0; k < nbDonnees; k++) {
                tree_delete_data(&arbre5, &arbre5, &donnees[k], monCompareInt, NULL);
            }
            end = clock();
            tpsDelete += ((double) (end - start)) / CLOCKS_PER_SEC;

            tree_delete(arbre5, NULL);

        }
        tpsAdd /= nbTests;
        tpsSearch /= nbTests;
        tpsDelete /= nbTests;
        fprintf(fichier, "%f;%f;%f\n", tpsAdd, tpsSearch, tpsDelete);
        nbDonnees += 50;
    }

    fclose(fichier);
    */

    freeSentinel(); // Libération de la sentinelle
}

int main(){

    testArbresRougeNoir();

    return EXIT_SUCCESS;
}
