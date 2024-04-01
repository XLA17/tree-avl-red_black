/*
 * Composition :
 * - Lafosse Alexy
 * - Gillier Arthur
 * - Lahrouri Yasin
 * - Pineros Colin
 */

#include <string.h>
#include "tree-red-black.h"
#include "min-max.h"
#include <stdio.h>
#include <stdlib.h>

/*--------------------------------------------------------------------*/
Tree tree_new (){
  return NULL;
}

void tree_delete (Tree tree, void (*delete) (void *)){
  if (tree && !isSentinel(tree)){
    tree_delete (tree->left, delete);
    tree_delete (tree->right, delete);
    if (delete)
      delete (tree->data);
    free (tree);
  }
}


// Sentinelle commune à tous les arbres.
static Tree sentinel = NULL;

void initializeSentinel(){
  if (!sentinel){
    sentinel = malloc(sizeof(Tree));
    if (sentinel){
      sentinel->left = NULL;
      sentinel->right = NULL;
      sentinel->parent = NULL;
      sentinel->color = BLACK;
    }
  }
}

void freeSentinel(){
  if (!sentinel){
    printf("erreur : la sentinelle n'a pas été initialisée\n");
    return;
  }
  free(sentinel);
  sentinel = NULL;
}

bool isSentinel(Tree tree){
  return (tree->color == BLACK) && (tree->left == NULL) && (tree->right == NULL);
}

Tree tree_create (const void *data, size_t size){
  Tree tree = (Tree) malloc (3 * sizeof (Tree) + size + sizeof(Color));
  if (tree){
    tree->left = sentinel;
    tree->right = sentinel;
    tree->parent = NULL;
    tree->color = RED;
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
  if (tree && !isSentinel(tree)){
    func (tree->data, &tree->color, extra_data);
    tree_pre_order (tree->left, func, extra_data);
    tree_pre_order (tree->right, func, extra_data);
  }
}

void tree_in_order (Tree tree, void (*func) (void *, void *, void *), void *extra_data){
  if (tree && !isSentinel(tree)){
    tree_in_order (tree->left, func, extra_data);
    func (tree->data, &tree->color, extra_data);
    tree_in_order (tree->right, func, extra_data);
  }
}

void tree_post_order (Tree tree, void (*func) (void *, void *, void *), void *extra_data){
  if (tree && !isSentinel(tree)){
    tree_post_order (tree->left, func, extra_data);
    tree_post_order (tree->right, func, extra_data);
    func (tree->data, &tree->color, extra_data);
  }
}

size_t tree_height (Tree tree){
  if (tree && !isSentinel(tree))
    return 1 + MAX (tree_height (tree->left), tree_height (tree->right));
  else
    return 0;
}

size_t tree_size (Tree tree){
  if (tree && !isSentinel(tree))
    return 1 + tree_size (tree->left) + tree_size (tree->right);
  else
    return 0;
}

Tree tree_search (Tree tree, const void *data, int (*compare) (const void *, const void *)){
  if (tree && !isSentinel(tree)){
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

Tree grandparent (Tree n){
  if ((n != NULL) && (n->parent != NULL))
    return n->parent->parent;
  else
    return NULL;
}

Tree uncle (Tree n){
  Tree g = grandparent(n);
  if (g == NULL)
    return NULL;
  if (n->parent == g->left)
    return g->right;
  else
    return g->left;
}

void tree_rotate (Tree *root, Tree *tree, bool left){
  if (*tree){
    Tree tmp = (*tree);
    if (left) {
      // Rotation gauche
      if (tmp->parent == NULL){
        // pour le cas où l'on fait une rotation gauche sur la racine
        // pour que la variable de type Tree qui stocke la racine pointe sur la nouvelle racine
        *root = tmp->right;
      }
      else if (tmp->parent->left == tmp){
        tmp->parent->left = tmp->right;
      }
      else if (tmp->parent->right == tmp){
        tmp->parent->right = tmp->right;
      }
      tmp->right->parent = tmp->parent;
      tmp->parent = tmp->right;
      tmp->right = tmp->right->left;
      tmp->parent->left = tmp;
      tmp->right->parent = tmp;
    }
    else {
      // Rotation droite
      if (tmp->parent == NULL){
        // pour le cas où l'on fait une rotation droite sur la racine
        // pour que la variable de type Tree qui stocke la racine pointe sur la nouvelle racine
        *root = tmp->left;
      }
      else if (tmp->parent->left == tmp){
        tmp->parent->left = tmp->left;
      }
      else if (tmp->parent->right == tmp){
        tmp->parent->right = tmp->left;
      }
      tmp->left->parent = tmp->parent;
      tmp->parent = tmp->left;
      tmp->left = tmp->left->right;
      tmp->parent->right = tmp;
      tmp->left->parent = tmp;
    }
  }
}

void insert_case (Tree *root, Tree n){

  // cas 1
  // n est la racine
  if (n->parent == NULL){
    n->color = BLACK;
    return;
  }

  // cas 2
  // Le père de n est noir donc aucune modification à faire
  if (n->parent->color == BLACK){
    return;
  }

  // Le père de n est rouge donc le grand-père g est noir

  // cas 3
  // L'oncle u de n est rouge
  Tree u = uncle(n);
  Tree g = grandparent(n);
  if (u->color == RED){
    n->parent->color = BLACK;
    u->color = BLACK;
    g->color = RED;
    insert_case(root, g);
    return;
  }

  // L'oncle u de n est noir

  // cas 4
  // n est le fils droit de son père p et p est le fils gauche de son père g
  if ((n == n->parent->right) && (n->parent == g->left)){
    tree_rotate(root, &n->parent, true);
    n = n->left;
  }
  // n est le fils gauche de son père p et p est le fils droit de son père g
  else if ((n == n->parent->left) && (n->parent == g->right)){
    tree_rotate(root, &n->parent, false);
    n = n->right;
  }

  // cas 5
  g = grandparent(n);
  n->parent->color = BLACK;
  g->color = RED;
  // n est le fils gauche de son père p et p est le fils gauche de son père g
  if ((n == n->parent->left) && (n->parent == g->left)){
    tree_rotate(root, &g, false);
    return;
  }
  // n est le fils droit de son père p et p est le fils droit de son père g
  else {
    tree_rotate(root, &g, true);
    return;
  }
}

void tree_add_data (Tree *root, Tree *tree, const void *data, size_t size, int (*compare) (const void *, const void *)){
  if (*tree && !isSentinel(*tree)){
    switch (compare (data, (*tree)->data)){

      case -1: // si la donnée est plus petite que la donnée du noeud
        if (isSentinel((*tree)->left)){
          // on crée un nouveau noeud et on l'ajoute à gauche du noeud actuel
          Tree new = tree_create (data, size);
          if (new){
            (*tree)->left = new;
          }
          (*tree)->left->parent = *tree;
          insert_case(root, (*tree)->left); // on équilibre l'arbre
        }
        else {
          tree_add_data (root, &((*tree)->left), data, size, compare);
        }
        break;
      
      case 0: // si la donnée est égale à la donnée du noeud
        printf("erreur : la donnée est déjà présente dans l'arbre\n");
        break;

      case 1: // si la donnée est plus grande que la donnée du noeud
        if (isSentinel((*tree)->right)){
          // on crée un nouveau noeud et on l'ajoute à droite du noeud actuel
          Tree new = tree_create (data, size);
          if (new){
            (*tree)->right = new;
          }
          (*tree)->right->parent = *tree;
          insert_case(root, (*tree)->right); // on équilibre l'arbre
        }
        else {
          tree_add_data (root, &((*tree)->right), data, size, compare);
        }
        break;
      
      default:
        break;
    }
  }
  else{
    // si l'arbre est NULL ou si le noeud est une sentinelle
    // on crée un nouveau noeud et on l'ajoute
    Tree new = tree_create (data, size);
    if (new){
      *tree = new;
    }
    insert_case(root, *tree); // on équilibre l'arbre
  }
}

void delete_case (Tree *root, Tree n, Tree p){

  // cas 1
  // n est la racine => on ne fait rien
  if (!p){
    return;
  }
  Tree f, g, d;

  if ((p)->left == n){ // si n est le fils gauche de son père p
    f = p->right; // f est le frère de n
    g = f->left; // g est le fils gauche de f
    d = f->right; // d est le fils droit de f

    // cas 2
    // f est noir et ses deux fils sont noirs
    if (f->color == BLACK && g->color == BLACK && d->color == BLACK){
      f->color = RED;
      if (p->color == BLACK){
        delete_case(root, p, p->parent); // on recommence avec le père de p
      }
      else {
        p->color = BLACK;
      }
      return;
    }

    // cas 3
    // f est noir et d est rouge
    if (f->color == BLACK && d->color == RED){
      tree_rotate(root, &p, true);
      d->color = BLACK;
      if (p->color == RED){
        p->color = BLACK;
        f->color = RED;
      }
      return;
    }

    // cas 4
    // f est noir, g est rouge et d est noir
    if (f->color == BLACK && g->color == RED && d->color == BLACK){
      tree_rotate(root, &f, false);
      g->color = BLACK;
      f->color = RED;
      tree_rotate(root, &p, true);
      f->color = BLACK;
      if (p->color == RED){
        p->color = BLACK;
        g->color = RED;
      }
      return;
    }

    // cas 5
    // f est rouge
    if (f->color == RED){
      tree_rotate(root, &p, true);
      p->color = RED;
      f->color = BLACK;
      // on se retrouve dans la situation du cas 2
      delete_case(root, n, p);
    }
  }

  else { // si n est le fils droit de son père p
    f = p->left; // f est le frère de n
    g = f->left; // g est le fils gauche de f
    d = f->right; // d est le fils droit de f

    // cas 2
    // f est noir et ses deux fils sont noirs
    if (f->color == BLACK && g->color == BLACK && d->color == BLACK){
      f->color = RED;
      if (p->color == BLACK){
        delete_case(root, p, p->parent); // on recommence avec le père de p
      }
      else {
        p->color = BLACK;
      }
      return;
    }

    // cas 3
    // f est noir et g est rouge
    if (f->color == BLACK && g->color == RED){
      tree_rotate(root, &p, false);
      g->color = BLACK;
      if (p->color == RED){
        p->color = BLACK;
        f->color = RED;
      }
      return;
    }

    // cas 4
    // f est noir, d est rouge et g est noir
    if (f->color == BLACK && d->color == RED && g->color == BLACK){
      tree_rotate(root, &f, true);
      d->color = BLACK;
      f->color = RED;
      tree_rotate(root, &p, false);
      f->color = BLACK;
      if (p->color == RED){
        p->color = BLACK;
        d->color = RED;
      }
      return;
    }

    // cas 5
    // f est rouge
    if (f->color == RED){
      tree_rotate(root,&p, false);
      p->color = RED;
      f->color = BLACK;
      // on se retrouve dans la situation du cas 2
      delete_case(root, n, p);
    }
  }
}

void tree_delete_data (Tree *root, Tree *tree, const void *data, int (*compare) (const void *, const void *), void (*delete) (void *)){
  if (*tree && !isSentinel(*tree)){
    switch (compare (data, (*tree)->data)){

      case -1: // cas où la donnée est plus petite que la donnée du noeud
        tree_delete_data (root, &((*tree)->left), data, compare, delete);
        break;
      
      case 0: // cas où on a trouvé le noeud à supprimer
        if (isSentinel((*tree)->left) && isSentinel((*tree)->right)){
          Tree tmp = *tree;
          *tree = sentinel;
          if (tmp->color == BLACK){ // si le noeud à supprimer est noir => on équilibre l'arbre
            delete_case(root, *tree, tmp->parent);
          }
          if (delete){
            delete(tmp->data);
          }
          free(tmp);
          tmp = NULL;
        }
        else if (isSentinel((*tree)->left)){
          Tree tmp = *tree;
          *tree = (*tree)->right;
          (*tree)->parent = tmp->parent;
          if (tmp->color == BLACK){ // si le noeud à supprimer est noir
            if ((*tree)->color == BLACK){ // si le noeud remplaçant est noir => on équilibre l'arbre
              delete_case(root, *tree, (*tree)->parent);
            }
            else { // si le noeud remplaçant est rouge => on le met en noir
              (*tree)->color = BLACK;
            }
          }
          if (delete){
            delete(tmp->data);
          }
          free(tmp);
          tmp = NULL;
        }
        else if (isSentinel((*tree)->right)){
          Tree tmp = *tree;
          *tree = (*tree)->left;
          (*tree)->parent = tmp->parent;
          if (tmp->color == BLACK){ // si le noeud à supprimer est noir
              if ((*tree)->color == BLACK){ // si le noeud remplaçant est noir => on équilibre l'arbre
                  delete_case(root, *tree, (*tree)->parent);
              }
              else { // si le noeud remplaçant est rouge => on le met en noir
                  (*tree)->color = BLACK;
              }
          }
          if (delete){
              delete(tmp->data);
          }
          free(tmp);
          tmp = NULL;
        }
        else {
          Tree tmp = (*tree)->left;
          // on cherche le noeud le plus à droite du sous arbre gauche
          while (!isSentinel(tmp->right)){
            tmp = tmp->right;
          }
          // on remplace la donnée du noeud à supprimer par la donnée du noeud le plus à droite du sous arbre gauche
          memcpy ((*tree)->data, tmp->data, sizeof(tmp->data));
          // on supprime le noeud le plus à droite du sous arbre gauche
          tree_delete_data (root, &(*tree)->left, tmp->data, compare, delete);
        }
        break;

      case 1: // cas où la donnée est plus grande que la donnée du noeud
        tree_delete_data (root, &((*tree)->right), data, compare, delete);
        break;
      
      default:
        break;
    }
  }
}