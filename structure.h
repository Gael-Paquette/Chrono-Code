/* Auteur : Gaël PAQUETTE & Aloui DHIA
 * Création : 18/02/2022
 * Dernière modification : 18/02/2022 */
#include <stdio.h>
#include <sys/time.h>

#define TAILLE_MAX 30

typedef struct{
    int etat;
    int duree_totale;
    int avertissement;
    int dernier_tour;
    int nb_tour;
    int tab[TAILLE_MAX];
}Chronometre;