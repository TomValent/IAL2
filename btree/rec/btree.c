/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    bst_node_t *tmp = tree;
    if(!tmp)
        return false;

    if(tmp->key == key)                     //found
    {
        *value = tmp->value;
        return true;
    }
    else if(tmp->key > key)                 //lavy podstrom
    {
        if(tmp->left != NULL)
            bst_search(tmp->left, key, value);
        else
            return false;
    }

    else if(tmp->key < key)                 //pravy podstrom
    {
        if(tmp->right != NULL)
            bst_search(tmp->right, key, value);
        else
            return false;
    }
    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t *tmp = *tree;
    bst_node_t *new = malloc(sizeof(bst_node_t));

    new->left = NULL;
    new->right = NULL;
    new->key = key;
    new->value = value;

    if(!tmp)
    {
        *tree = new;
        return;
    }

    if(tmp->key == key)             //found
    {
        tmp->value = value;
        free(new);
        return;
    }
    else if(tmp->key > key)         //lavy podstrom
    {
        if(tmp->left != NULL)
        {
            free(new);
            bst_insert(&tmp->left, key, value);
        }
        else
        {
            tmp->left = new;
            return;
        }
    }
    else if(tmp->key < key)         //pravy podstrom
    {
        if(tmp->right != NULL)
        {
            free(new);
            bst_insert(&tmp->right, key, value);
        }
        else
        {
            tmp->right = new;
            return;
        }
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *tmp = *tree;
    bst_node_t *prev = target;

    if(!tmp)
        return;

    if(tmp->right->right)
    {
        bst_replace_by_rightmost(target, &tmp->right);
        return;
    }
    if(tmp->right)      //aspon 1 pravy uzol tam je - aj ak sa nedostane do prvej podmienky
    {
        if(tmp->right->left == NULL)        //najdeny najpravejsi nema laveho syna
        {
            prev = tmp;
            tmp = tmp->right;

            target->value = tmp->value;
            target->key = tmp->key;

            free(tmp);
            prev->right = NULL;
            return;
        }
        else                                //najdeny najpravejsi ma laveho syna
        {
            prev = tmp;
            tmp = tmp->right;
            target->value = tmp->value;
            target->key = tmp->key;

            prev->right = tmp->left;
            free(tmp);
        }
    }
    else        //hned nalavo pod targetom je najpravejsi uzol - ak sa nedostane do prvej podmienky vobec
    {
        target->value = tmp->value;
        target->key = tmp->key;

        if(tmp->left)                      //strom tmp ma lavy podstrom
            prev->right = tmp->left;
        else                               //strom tmp ma len 1 prvok
            prev->right = NULL;
        free(tmp);
    }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    if(!*tree)                                //koren je NULL
        return;

    if((*tree)->key == key)                     //found
    {
        if(!(*tree)->left && !(*tree)->right)                                                   //no kids
        {
            free((*tree));
            *tree = NULL;
        }
        else if((!(*tree)->left && (*tree)->right) || ((*tree)->left && !(*tree)->right))       //1 child
        {
            if((*tree)->left)           //existuje len lavy podstrom
            {
                bst_node_t *to_delete = (*tree);
                (*tree) = (*tree)->left;
                free(to_delete);
            }
            else//(*tree)->right        //existuje len pravy podstrom
            {
                bst_node_t *to_delete = (*tree);
                (*tree) = (*tree)->right;
                free(to_delete);
            }
        }
        else if((*tree)->left && (*tree)->right)                                                //2 children
        {
            bst_replace_by_rightmost((*tree), &(*tree)->left);
            return;
        }
    }
    else if((*tree)->key > key)                 //lavy podstrom
    {
        if((*tree)->left != NULL)
        {
            bst_delete(&(*tree)->left, key);
            return;
        }
        else
            return;
    }
    else if((*tree)->key < key)                 //pravy podstrom
    {
        if((*tree)->right != NULL)
        {
            bst_delete(&(*tree)->right, key);
            return;
        }
        else
            return;
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if(*tree != NULL)
    {
        bst_dispose(&(*tree)->left);
        bst_dispose(&(*tree)->right);
        free(*tree);
        (*tree) = NULL;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    bst_node_t *tmp = tree;
    if (tmp) {
        bst_print_node(tmp);
        bst_preorder(tmp->left);
        bst_preorder(tmp->right);
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    bst_node_t *tmp = tree;
    if(tmp)
    {
        bst_inorder(tmp->left);
        bst_print_node(tmp);
        bst_inorder(tmp->right);
    }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    bst_node_t *tmp = tree;
    if(tmp)
    {
        bst_postorder(tmp->left);
        bst_postorder(tmp->right);
        bst_print_node(tmp);
    }
}
