#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tree-avl.h"
#include <time.h>

// Fonction d'affichage pour les entiers.
void monPrintFInt (void * a, void * b, void * c){
    printf("Balance : %d  --  Valeur : %d\n", *(int*)b, *(int*)a);
}

// Fonction d'affichage pour les chaines de caractères.
void monPrintFString (void * a, void * b, void * c){
    printf("Balance : %d  --  Valeur : %s\n", *(int*)b, (char*)a);
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

// Fonction qui retourne la taille d'une chaine de caractères.
size_t sizeString (const void * a){
    return strlen((const char*)a)+1;
}

void testArbresAVL(void){

    // création d'un tableau d'entiers avec les valeurs de 1 à DATA.
    const int DATA = 127;
    int data[DATA];
    for (int i = 0; i < DATA; i++) {
        data[i] = i;
    }
    
    size_t sizeInt = sizeof(int); // taille d'un entier en octets



    // tests ajout avec rotations

    printf("\n-- Test ajout --\n");

    Tree arbre = tree_new(); // création d'un arbre vide

    tree_add_data(&arbre, &data[50], sizeInt, monCompareInt);

    assert(tree_height(arbre) == 1);
    assert(tree_size(arbre) == 1);
    assert(arbre->data[0] == data[50]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test ajout + rotation simple droite --\n");

    tree_add_data(&arbre, &data[25], sizeInt, monCompareInt);
    tree_add_data(&arbre, &data[12], sizeInt, monCompareInt);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->data[0] == data[25]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test ajout + rotation simple gauche --\n");

    tree_add_data(&arbre, &data[56], sizeInt, monCompareInt);
    tree_add_data(&arbre, &data[62], sizeInt, monCompareInt);

    assert(tree_height(arbre) == 3);
    assert(tree_size(arbre) == 5);
    assert(arbre->right->data[0] == data[56]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test ajout + rotation double droite --\n");

    tree_delete_data(&arbre, &data[62], monCompareInt, NULL);
    tree_delete_data(&arbre, &data[56], monCompareInt, NULL);
    tree_delete_data(&arbre, &data[50], monCompareInt, NULL);
    tree_add_data(&arbre, &data[18], sizeInt, monCompareInt);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->data[0] == data[18]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test ajout + rotation double gauche --\n");

    tree_delete_data(&arbre, &data[12], monCompareInt, NULL);
    tree_add_data(&arbre, &data[20], sizeInt, monCompareInt);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->data[0] == data[20]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test ajout données existantes --\n");
    // aucune donnée ne doit être ajoutée

    tree_add_data(&arbre, &data[25], sizeInt, monCompareInt);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");



    // tests suppression

    printf("\n-- Test suppression feuille --\n");

    tree_delete_data(&arbre, &data[25], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 2);
    assert(arbre->right == NULL);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppression noeud avec un fils gauche --\n");

    tree_add_data(&arbre, &data[25], sizeInt, monCompareInt);
    tree_add_data(&arbre, &data[12], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &data[18], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->left->data[0] == data[12]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppression noeud avec un fils droit --\n");

    tree_add_data(&arbre, &data[18], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &data[12], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->left->data[0] == data[18]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppression noeud avec un deux fils --\n");

    tree_delete_data(&arbre, &data[20], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 2);
    assert(arbre->right->data[0] == data[25]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");



    // tests suppression avec rotations

    printf("\n-- Test suppression + rotation gauche --\n");

    tree_add_data(&arbre, &data[12], sizeInt, monCompareInt);
    tree_add_data(&arbre, &data[50], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &data[12], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->left->data[0] == data[18]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppression + rotation droite --\n");

    tree_add_data(&arbre, &data[12], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &data[50], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->left->data[0] == data[12]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppression + rotation gauche double --\n");

    tree_add_data(&arbre, &data[20], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &data[12], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->data[0] == data[20]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppression + rotation droite double --\n");

    tree_add_data(&arbre, &data[19], sizeInt, monCompareInt);
    tree_delete_data(&arbre, &data[25], monCompareInt, NULL);

    assert(tree_height(arbre) == 2);
    assert(tree_size(arbre) == 3);
    assert(arbre->data[0] == data[19]);

    printf("\nTri post-order : \n");
    tree_post_order(arbre, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    tree_delete(arbre, NULL);

    
    
    // tests itératifs

    printf("\n-- Test ajouts --\n");

    Tree arbre2 = tree_new();

    // ajout des DATA valeurs
    for (int i = 0; i < DATA; i++) {
        tree_add_data(&arbre2, &data[i], sizeInt, monCompareInt);
    }

    assert(tree_height(arbre2) == 7); // car 2^6 (= 64) < DATA < 2^7 (= 128)
    assert(tree_size(arbre2) == DATA);

    printf("\nTri post-order : \n");
    tree_post_order(arbre2, monPrintFInt, NULL);
    printf("\nTests OK ...\n");

    printf("\n-- Test suppressions --\n");

    // suppression des 50 premières valeurs
    for (int i = 0; i < 50; i++) {
        tree_delete_data(&arbre2, &data[i], monCompareInt, NULL);
    }

    assert(tree_height(arbre2) == 7);
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

    printf("\n-- Test chaines de caractères --\n");

    Tree arbre3 = tree_new();

    char * str[] = {"fonction", "TEA", "programmation", "C", "code", };

    for (int i = 0; i < (sizeof(str) / sizeof(str[0])); i++){
        tree_add_data(&arbre3, str[i], sizeString(str[i]), monCompareString);
    }

    assert(tree_height(arbre3) == 3);
    assert(tree_size(arbre3) == 5);
    assert(strcmp((char*)arbre3->data, "fonction") == 0);

    printf("\nTri post-order : \n");
    tree_post_order(arbre3, monPrintFString, NULL);
    printf("\nTests OK ...\n");

    tree_delete(arbre3, NULL);



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

            for (int k = 0; k < nbDonnees; k++) {
                donnees[k] = rand(); // Génère un nombre pseudo-aléatoire
            }
            start = clock(); // Début du chronomètre
            for (int k = 0; k < nbDonnees; k++) {
                tree_add_data(&arbre5, &donnees[k], sizeInt, monCompareInt);
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
                tree_delete_data(&arbre5, &donnees[k], monCompareInt, NULL);
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
}


int main(){

    testArbresAVL();

    return EXIT_SUCCESS;
}
