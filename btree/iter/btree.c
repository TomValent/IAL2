/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    bst_node_t *tmp = tree;
    while(tmp)
    {
        if(tmp->key == key)     //found     //prva iteracia tmp == root
        {
            *value = tmp->value;
            return true;
        }
        if(tmp->key < key)      //lavy podstrom
        {
            if(tmp->left != NULL)
                tmp = tmp->left;
            else
                return false;
        }
        else                    //pravy podstrom
        {
            if(tmp->right != NULL)
                tmp = tmp->right;
            else
                return false;
        }
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t *tmp = *tree;
    bst_node_t *new = malloc(sizeof(bst_node_t));
    if(!new)
        return;

    new->key = key;
    new->value = value;

    while(tmp)
    {
        if(tmp->key == key)     //found == nahradi hodnotu
        {
            tmp->value = value;
            break;
        }
        if(tmp->key < key)      //lavy podstrom
        {
            if(tmp->left != NULL)
                tmp = tmp->left;
            else
            {
                tmp->left = new;
                break;
            }
        }
        else                    //pravy podstrom
        {
            if(tmp->right != NULL)
                tmp = tmp->right;
            else
            {
                tmp->right = new;
                break;
            }
        }
    }
    *tree = new;
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *tmp = *tree;
    bst_node_t *prev = target;
    while(tmp->right)
    {
        prev = tmp;
        tmp = tmp->right;
    }
    if(tmp->left == NULL)
    {
        target->value = tmp->value;
        target->key = tmp->key;
        free(tmp);
    }
    else
    {
        target->value = tmp->value;
        target->key = tmp->key;

        prev->right = tmp->left;
        free(tmp->left);
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
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    bst_node_t *tmp = *tree;
    bst_node_t *prev = NULL;
    while(tmp)
    {
        if(tmp->key == key)                     //found
        {
            if(!tmp->left && !tmp->right)                                           //no childs
            {
                free(tmp);
                break;
            }
            else if((!tmp->left && tmp->right) || (tmp->left && !tmp->right))       //1 child
            {
                if(prev != NULL)        //tmp neni root
                {
                    if(prev->left == tmp)
                    {
                        if(tmp->left != NULL)
                        {
                            prev->left = tmp->left;
                        }
                        else if(tmp->right != NULL)
                        {
                            prev->left = tmp->right;
                        }
                    }
                    else if(prev->right == tmp)
                    {
                        if(tmp->left != NULL)
                        {
                            prev->right = tmp->left;
                        }
                        else if(tmp->right != NULL)
                        {
                            prev->right = tmp->right;
                        }
                    }
                    free(tmp);
                    break;
                }
                else                //tmp je root
                {
                    if(tmp->left != NULL)
                    {
                        *tree = tmp->left;
                    }
                    else if(tmp->right != NULL)
                    {
                        *tree = tmp->right;
                    }
                    free(tmp);
                    break;
                }
            }
            else if(tmp->left && tmp->right)                                        //both children
            {
                bst_replace_by_rightmost(tmp, &(tmp->left));
            }
        }
        if(tmp->key < key)                      //lavy podstrom
        {
            if(tmp->left != NULL)
            {
                prev = tmp;
                tmp = tmp->left;
            }
            else
                break;
        }
        else                                    //pravy podstrom
        {
            if(tmp->right != NULL)
            {
                prev = tmp;
                tmp = tmp->right;
            }
            else
                break;
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);

    stack_bst_push(&stack, *tree);
    while(!stack_bst_empty(&stack))
    {
        bst_node_t *tmp = stack_bst_pop(&stack);
        if(!tmp)
            continue;

        stack_bst_push(&stack, tmp->left);
        stack_bst_push(&stack, tmp->right);

        free(tmp);
    }
    *tree= NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
}
