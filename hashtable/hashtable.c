/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    for(int i = 0; i < MAX_HT_SIZE; i++){
        (*table)[i] = NULL;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  for(int i = 0; i < MAX_HT_SIZE; i++)
  {
      if((*table)[i] != NULL)
      {
          if((*table)[i]->key == key)
              return (*table[i]);
      }
  }
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    struct ht_item *tmp = ht_search(table, key);
    if(tmp)
        tmp->value = value;
    else
    {
        struct ht_item *new = malloc(sizeof(struct ht_item));
        if(!new)
            return;
        new->key = key;
        new->value = value;
        new->next = NULL;
        int hashCode = get_hash(key);

        tmp = (*table)[hashCode];
        if((*table)[hashCode])
            new->next = tmp;

        (*table)[hashCode] = new;
    }
}


/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    struct ht_item *tmp = ht_search(table, key);
    if(tmp != NULL)
        return &(tmp->value);

    return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    int hash_code = get_hash(key);
    if((*table)[hash_code] != NULL)
    {
        struct ht_item *tmp = *table[hash_code];
        struct ht_item *next;

        while (tmp->next != NULL)
        {
            next = tmp->next;
            free(tmp);
            tmp = next;
        }
        free(tmp);
    }
    (*table)[hash_code] = NULL;
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
    struct ht_item *tmp, *next;
    for(int pos = 0; pos < MAX_HT_SIZE; pos++)
    {
        if((*table)[pos] != NULL)
        {
            next = (*table)[pos];
            while(next->next != NULL)
            {
                tmp = next;
                next = next->next;
                free(tmp);
            }
            free(next);
            (*table)[pos] = NULL;
        }
    }
}