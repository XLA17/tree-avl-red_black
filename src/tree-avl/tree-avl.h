#ifndef _TREE_H_
#define _TREE_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct _TreeNode *Tree;

struct _TreeNode
  {
    Tree left;
    Tree right;
    int balance;
    char data[1];
  };

// Fonction pour créer un nouvel arbre vide.
Tree tree_new ();

// Fonction pour supprimer tous les éléments d'un arbre.
void tree_delete (Tree tree, void (*delete) (void *));

// Fonction pour créer un nouveau noeud avec des données de taille spécifiée.
Tree tree_create (const void *data, size_t size);

// Fonction pour obtenir le sous-arbre gauche d'un arbre donné.
Tree tree_get_left (Tree tree);

// Fonction pour obtenir le sous-arbre droit d'un arbre donné.
Tree tree_get_right (Tree tree);

// Fonction pour obtenir les données d'un arbre donné.
void *tree_get_data (Tree tree);

// Fonction pour parcourir l'arbre en pre-order et appliquer une fonction à chaque nœud.
void tree_pre_order (Tree tree, void (*func) (void *, void *, void *), void *extra_data);

// Fonction pour parcourir l'arbre en in-order et appliquer une fonction à chaque nœud.
void tree_in_order (Tree tree, void (*func) (void *, void *, void *), void *extra_data);

// Fonction pour parcourir l'arbre en post-order et appliquer une fonction à chaque nœud.
void tree_post_order (Tree tree, void (*func) (void *, void *, void *), void *extra_data);

// Fonction pour calculer la hauteur maximale d'un arbre.
size_t tree_height (Tree tree);

// Fonction pour calculer la taille d'un arbre (nombre de nœuds).
size_t tree_size (Tree tree);

// Fonction pour rechercher des données dans un arbre.
Tree tree_search (Tree tree, const void *data, int (*compare) (const void *, const void *));

// Fonction pour calculer le balance d'un noeud d'un arbre.
void tree_set_balance (Tree tree);

// Fonction pour faire une rotation d'un arbre.
void tree_rotate (Tree *tree, bool left);

// Fonction pour équilibrer un arbre.
void tree_balance (Tree *tree);

// Fonction pour ajouter un noeud dans un arbre.
void tree_add_data (Tree *tree, const void *data, size_t size, int (*compare) (const void *, const void *));

// Fonction pour supprimer un noeud dans un arbre.
void tree_delete_data (Tree *tree, const void *data, int (*compare) (const void *, const void *), void (*delete) (void *));

// Fonction pour équilibrer un arbre de manière itérative.
void tree_iterative_balance (Tree *tree);

#endif
