/*
 * Composition :
 * - Lafosse Alexy
 * - Gillier Arthur
 * - Lahrouri Yasin
 * - Pineros Colin
 */

#include <string.h>
#include "tree-avl.h"
#include "min-max.h"

/*--------------------------------------------------------------------*/

Tree tree_new (){
  return NULL;
}

void tree_delete (Tree tree, void (*delete) (void *)){
  if (tree){
    tree_delete (tree->left, delete);
    tree_delete (tree->right, delete);
    if (delete)
      delete (tree->data);
    free (tree);
  }
}

Tree tree_create (const void *data, size_t size){
  Tree tree = (Tree) malloc (sizeof(int) + 2 * sizeof (Tree) + size);
  if (tree){
    tree->left = NULL;
    tree->right = NULL;
    tree->balance = 0;
    memcpy (tree->data, data, size);
  }
  return tree;
}

Tree tree_get_left (Tree tree){
  if (tree)
    return tree->left;
  else
    return NULL;
}

Tree tree_get_right (Tree tree){
  if (tree)
    return tree->right;
  else
    return NULL;
}

void * tree_get_data (Tree tree){
  if (tree)
    return tree->data;
  else
    return NULL;
}

void tree_pre_order (Tree tree, void (*func) (void *, void *, void *), void *extra_data){
  if (tree){
    func (tree->data, &tree->balance, extra_data);
    tree_pre_order (tree->left, func, extra_data);
    tree_pre_order (tree->right, func, extra_data);
  }
}

void tree_in_order (Tree tree, void (*func) (void *, void *, void *), void *extra_data){
  if (tree){
    tree_in_order (tree->left, func, extra_data);
    func (tree->data, &tree->balance, extra_data);
    tree_in_order (tree->right, func, extra_data);
  }
}

void tree_post_order (Tree tree, void (*func) (void *, void *, void *), void *extra_data){
  if (tree){
    tree_post_order (tree->left, func, extra_data);
    tree_post_order (tree->right, func, extra_data);
    func (tree->data, &tree->balance, extra_data);
  }
}

size_t tree_height (Tree tree){
  if (tree)
    return 1 + MAX (tree_height (tree->left),
    tree_height (tree->right));
  else
    return 0;
}

size_t tree_size (Tree tree){
  if (tree)
    return 1 + tree_size (tree->left) + tree_size (tree->right);
  else
    return 0;
}

Tree tree_search (Tree tree, const void *data, int (*compare) (const void *, const void *)){
  if (tree){
    switch (compare (data, tree->data)){
    case -1:
      return tree_search (tree->left, data, compare);
    case 0:
      return tree;
    case 1:
      return tree_search (tree->right, data, compare);
    default:
        return NULL;
    }
  }
  else
    return NULL;
}

void tree_set_balance (Tree tree){
  if (tree){
    tree->balance = tree_height (tree->right) - tree_height (tree->left);
  }
}

void tree_rotate (Tree *tree, bool left){
  if (*tree){
    Tree tmp = *tree;
    if (left){
      // Rotation gauche
      *tree = (*tree)->right;
      tmp->right = (*tree)->left;
      (*tree)->left = tmp;
    }
    else {
      // Rotation droite
      *tree = (*tree)->left;
      tmp->left = (*tree)->right;
      (*tree)->right = tmp;
    }
  }
}

void tree_balance (Tree *tree){
  if (*tree){
    if ((*tree)->balance < -1){
      if (tree_height ((*tree)->left->left) < tree_height ((*tree)->left->right)){
        // On fait une rotation gauche du sous arbre gauche
        // On met à jour le balance des noeuds
        (*tree)->left->balance = (*tree)->left->balance - 1 - MAX((*tree)->left->right->balance, 0);
        (*tree)->left->right->balance = (*tree)->left->right->balance - 1 + MIN((*tree)->left->balance, 0);
        tree_rotate (&(*tree)->left, true);
      }
      // On fait une rotation droite
      // On met à jour le balance des noeuds
      (*tree)->balance = (*tree)->balance + 1 - MIN((*tree)->left->balance, 0);
      (*tree)->left->balance = (*tree)->left->balance + 1 + MAX((*tree)->balance, 0);
      tree_rotate (tree, false);
    }
    else if ((*tree)->balance > 1){
      if (tree_height ((*tree)->right->right) < tree_height ((*tree)->right->left)){
        // On fait une rotation droite du sous arbre droit
        // On met à jour le balance des noeuds
        (*tree)->right->balance = (*tree)->right->balance + 1 - MIN((*tree)->right->left->balance, 0);
        (*tree)->right->left->balance = (*tree)->right->left->balance + 1 + MAX((*tree)->right->balance, 0);
        tree_rotate (&(*tree)->right, false);
      }
      // On fait une rotation gauche
      // On met à jour le balance des noeuds
      (*tree)->balance = (*tree)->balance - 1 - MAX((*tree)->right->balance, 0);
      (*tree)->right->balance = (*tree)->right->balance - 1 + MIN((*tree)->balance, 0);
      tree_rotate (tree, true);
    }
  }
}

void tree_add_data (Tree *tree, const void *data, size_t size, int (*compare) (const void *, const void *)){
  if (*tree){
    switch (compare (data, (*tree)->data)){

      case -1: // cas où la donnée est plus petite que la donnée du noeud
        tree_add_data (&(*tree)->left, data, size, compare); // on fais un appel récursif dans le sous arbre gauche
        tree_set_balance (*tree); // on met à jour le balance du noeud
        tree_balance (tree); // on équilibre l'arbre si besoin
        break;

      case 1: // cas où la donnée est plus grande que la donnée du noeud
        tree_add_data (&(*tree)->right, data, size, compare); // on fais un appel récursif dans le sous arbre droit
        tree_set_balance (*tree); // on met à jour le balance du noeud
        tree_balance (tree); // on équilibre l'arbre si besoin
        break;

      default: // cas où la donnée est égale à la donnée du noeud
        break;
    }
  }
  else { // si l'arbre est null
    // ajout de la nouvelle donnée
    Tree new = tree_create (data, size);
    if (new){
      *tree = new;
    }
  }
}

void tree_delete_data (Tree *tree, const void *data, int (*compare) (const void *, const void *), void (*delete) (void *)){
  if (*tree){
    switch (compare (data, (*tree)->data)){

      case -1: // cas où la donnée est plus petite que la donnée du noeud
        tree_delete_data (&(*tree)->left, data, compare, delete); // on fais un appel récursif dans le sous arbre gauche
        tree_set_balance (*tree); // on met à jour le balance du noeud
        tree_balance (tree); // on équilibre l'arbre
        break;

      case 0: // cas où la donnée est égale à la donnée du noeud
        if ((*tree)->left && (*tree)->right){ // si le noeud a deux fils
          Tree tmp = (*tree)->left;
          // on cherche le noeud le plus à droite du sous arbre gauche
          while (tmp->right){
            tmp = tmp->right;
          }
          // on remplace la donnée du noeud à supprimer par la donnée du noeud le plus à droite du sous arbre gauche qu'on supprime ensuite
          memcpy ((*tree)->data, tmp->data, sizeof(tmp->data));
          tree_delete_data (&(*tree)->left, tmp->data, compare, delete);
        }
        else if ((*tree)->left || (*tree)->right){ // si le noeud a un fils
          Tree tmp = *tree;
          if ((*tree)->left){ 
            *tree = (*tree)->left;
          }
          else {
            *tree = (*tree)->right;
          }
          if (delete)
            delete (tmp->data);
          free (tmp);
          tmp = NULL;
        }
        else { // si le noeud n'a pas de fils
          if (delete)
            delete ((*tree)->data);
          free (*tree);
          *tree = NULL;
        }
        break;

      case 1: // si la donnée est plus grande que la donnée du noeud
        tree_delete_data (&(*tree)->right, data, compare, delete); // On fais un appel récursif dans le sous arbre droit
        tree_set_balance (*tree); // On met à jour le balance du noeud
        tree_balance (tree); // On équilibre l'arbre
        break;

      default:
        break;
    }
  }
}

void tree_iterative_balance (Tree *tree){
  if (*tree){
    tree_iterative_balance (&(*tree)->left);
    tree_iterative_balance (&(*tree)->right);
    tree_balance (tree);
  }
}