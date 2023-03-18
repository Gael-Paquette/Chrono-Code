/* Auteur : Gaël PAQUETTE & Aloui DHIA
 * Création : 18/02/2022
 * Dernière modification : 22/02/2022 */
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>

#ifndef __STRUCT__
#define __STRUCT__
#include "structure.h"
#endif

Chronometre initialiser_chronometre(){
    /* Fonction qui permet d'initialiser le chronomètre. C'est-à-dire, la duree du chronomètre est égale à 0,
     * L'avertissement est paramètré à 25s, les tours sont initialiser à 0. etc... */
    int i;
    Chronometre chrono;
    chrono.etat = 0;
    chrono.duree_totale = 0;
    chrono.avertissement = 25000;
    chrono.dernier_tour = 0;
    chrono.nb_tour = 0;
    for (i = 0; i < TAILLE_MAX; i++)
        chrono.tab[i] = 0;
    return chrono;
}

int intervalle_ms(struct timeval debut, struct timeval fin){
    /* Fonction qui prend en paramètres des arguments qui sont des structures temps. ET qui renvoie la différence entre ses deux
     * arguments en une valeur entière en mili-secondes. */
    /* Variables pour recueillir les mesures. */
    int sec, usec;

    sec = (fin.tv_sec - debut.tv_sec) * 1000;
    usec = (int)rint(0.001 * (fin.tv_usec - debut.tv_usec));

    return sec + usec;    
}

int nb_ms_vers_centiemes(int nb_ms){
    /* Fonction qui prend en paramètres une valeur entière en mili-secondes.
     * Et qui renvoie une valeur en centiemes. */
    assert(nb_ms >= 0);

    return ((nb_ms / 10) % 100);
}

int nb_ms_vers_secondes(int nb_ms){
    /* Fonction qui prend en paramètres une valeur entière en mili-secondes.
     * Et qui renvoie une valeur en secondes. */
    assert(nb_ms >= 0);

    return ((nb_ms / 1000) % 60);
}

int nb_ms_vers_minutes(int nb_ms){
    /* Fonction qui prend en paramètres une valeur entière en mili-secondes.
     * Et qui renvoie une valeur en minutes. */
    assert(nb_ms >= 0);

    return ((nb_ms / (1000 * 60)) % 60);
}

int nb_ms_vers_heures(int nb_ms){
    /* Fonction qui prend en paramètres une valeur entière en mili-secondes.
     * Et qui renvoie une valeur en heures. */
    assert(nb_ms >= 0);
    
    return ((nb_ms / (1000 * 60 * 60)) % 100);
}

void afficher_duree(int y, int x, int nb_ms){
    /* Fonction qui prend en paramètres des coordonnées sur l'écran et une valeur entière en mili-secondes.
     * Et qui affiche le chronomètres selon le fomat h : m : s : c aux coordonées données. */
    assert(y >= 0 && y <= LINES);
    assert(x >= 0 && x <= COLS);
    assert(nb_ms >= 0);

    int c, s, m, h;
    c = nb_ms_vers_centiemes(nb_ms);
    s = nb_ms_vers_secondes(nb_ms);
    m = nb_ms_vers_minutes(nb_ms);
    h = nb_ms_vers_heures(nb_ms);
    mvprintw(y, x, "                           ");
    mvprintw(y, x, "%02d : %02d : %02d : %02d\r", h, m, s, c);
    refresh();
}

void afficher_flash(int y, int x, int avertissement){
    /* Fonction qui prend en paramètres des coordonnées de l'écran et une valeur entière en mili-secondes.
     * Et qui affiche en clignotant en rouge la valeur au format h : m : s : c aux coordonnées données. */
    assert(y >= 0 && y <= LINES);
    assert(x >= 0 && x <= COLS);
    assert(avertissement == 25000);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    attron(COLOR_PAIR(1)); /* Activation de la couleur */
    attron(A_BLINK); /* Activation du clignotement pour le warning. */
    mvprintw(y, x, "Avertissement : 00 : 00 : %02d : 00\r", nb_ms_vers_secondes(avertissement));
    attroff(A_BLINK); /* Désactivation du clignotement pour le warning. */
    attroff(COLOR_PAIR(1)); /* Désactivation de la couleur. */
    refresh();
}

void afficher_avertissement(int y, int x, int duree_parallele, int avertissement){
    /* Fonction qui prend en paramètres des coordonnées à l'ecran, et deux valeur entière en mili-secondes pour la durée en cours et la limite.
     * Cette fonction affiche en blan la limite sous le format h : m : s : c ou appelle la fonction afficher_flash(). */
    assert(y >= 0 && y <= LINES);
    assert(x >= 0 && x <= COLS);
    assert(duree_parallele >= 0);
    assert(avertissement == 25000);

    mvprintw(y, x, "                                   ");
    if (duree_parallele < avertissement)
        mvprintw(y, x, "Avertissement : 00 : 00 : %02d : 00\r", nb_ms_vers_secondes(avertissement));
    else
        afficher_flash(y, x, avertissement);
    refresh();
}

void ajouter_tour(Chronometre *chrono){
    /* Fonction qui prend en paramètres une chronomètres par pointeur.
     * Et qui ajoute une duree au tableur tab. Puis incremente les variables dernier_tour et nb_tour de 1.*/
    assert(chrono->dernier_tour >= 0);
    assert(chrono->nb_tour >= 0);
    assert(chrono->etat == 1); /* Car, selon les règles du jeu, il est impossible d'ajouter un tour lors que le chrono est arrêter. */

    int i = 0;
    chrono->dernier_tour += 1;
    chrono->nb_tour += 1;
    while (chrono->tab[i] != 0 && i < (TAILLE_MAX - 1))
        i++;
    chrono->tab[i] = chrono->duree_totale;
}

void affiche_tour(int y, int x, Chronometre chrono){
    /* Fonction qui prend en paramètres des coordonnées à l'écran, ainsi que un chronometre.
     * Et qui affiche les durées présentes dans le tableau tab dans le format h : m : s: c ainsi que leurs positions dans le tableau. */
    assert(y >= 0);
    assert(x >= 0);
    assert(chrono.dernier_tour >= 0);
    assert(chrono.nb_tour >= 0);
    assert(chrono.etat == 1 || chrono.etat == 0);

    int indice_tour = 0;
    int c, s, m, h;

    while (chrono.tab[indice_tour] != 0 && chrono.tab[indice_tour] != '\n'){
        if (y == 8)
            y = 2;
        c = nb_ms_vers_centiemes(chrono.tab[indice_tour]);
        s = nb_ms_vers_secondes(chrono.tab[indice_tour]);
        m = nb_ms_vers_minutes(chrono.tab[indice_tour]);
        h = nb_ms_vers_heures(chrono.tab[indice_tour]);
        if (indice_tour + 1 >= 10)
            mvprintw(y, x - 1, "Tour %d     : %02d : %02d : %02d : %02d\r", indice_tour + 1, h, m, s, c);
        else
            mvprintw(y, x, "Tour %d     : %02d : %02d : %02d : %02d\r", indice_tour + 1, h, m, s, c);
        y += 2;
        indice_tour++;
    }
    refresh();
}

void efface_tour(){
    /* Fonction qui permet d'effacer les tours incrits à l'écran. */
    int y = 2, x = (COLS / 2) - 13;
    assert(y >= 0 && y <= LINES);
    assert(x >= 0 && x <= COLS);

    mvprintw(y, x, "                                ");
    mvprintw(y + 2, x, "                                ");
    mvprintw(y + 4, x, "                                ");
    refresh();
}

void afficher_interface(Chronometre chrono, int duree_parallele){
    /* Fonction qui prend en paramètre un chronometre et une duree en mili-seconde.
     * Et qui s'occupe de gérer l'affiche à l'écran. */
    assert(duree_parallele >= 0);
    assert(chrono.duree_totale >= 0);
    assert(chrono.etat == 0 || chrono.etat == 1);
    assert(chrono.avertissement == 25000);
    assert(chrono.dernier_tour >= 0);
    assert(chrono.nb_tour >= 0);

    /* Cette fonction permet de faire l'affichage graphique du projet */
    int x_chrono, y_chrono, x_tour, y_tour, x_avertissement, y_avertissement, x_option, y_option;
    mvprintw(0, (COLS / 2), "== Chronometre ==");

    /* Positionnement des marquages des tours */
    y_tour = 2;
    x_tour = (COLS / 2) - 12;
    affiche_tour(y_tour, x_tour, chrono);

    /* Positionnement de l'affichage du Chronomètre */
    x_chrono = (COLS / 2);
    y_chrono = 9;
    afficher_duree(y_chrono, x_chrono, chrono.duree_totale);

    /* Positionnement de l'avertissement */
    y_avertissement = 11;
    x_avertissement = (COLS / 2) - 16;
    afficher_avertissement(y_avertissement, x_avertissement, duree_parallele, chrono.avertissement);
    refresh();

    /* Affichage des options */
    x_option = 0;
    y_option = (LINES / 2);
    mvprintw(y_option, x_option, "---------------------------------------------------------------------------------------------------------------------------------------------------------");
    mvprintw(y_option + 2, 0, "Espace : lancer / mettre en pause");
    mvprintw(y_option + 4, 0, "r      : rénitialiser");
    mvprintw(y_option + 6, 0, "t      : marquer tour");
    mvprintw(y_option + 8, 0, "F1/F2  : incrementer / decrementer heure avertissement");
    mvprintw(y_option + 10, 0, "F3/F4  : incrementer / decrementer minute avertissement");
    mvprintw(y_option + 12, 0, "F5/F6  : incrementer / decrementer seconde avertissement");
    mvprintw(y_option + 14, 0, "q      : quitter");
}

int main(void) {
    initscr(); /* Initialisation de la fenêtre */
    Chronometre chrono; 
    chrono = initialiser_chronometre();
    int touche, mise_a_zero, duree_parallele = 0;
    /* Variables de recueration du temps. */
    struct timeval temps_debut, temps_fin;
    
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    mousemask(ALL_MOUSE_EVENTS
        | REPORT_MOUSE_POSITION, NULL);
    
    afficher_interface(chrono, duree_parallele); /* Présentation à l'écran */
    
    while (1) {
        touche = getch(); /* Récupération des touches préssées */
        if (touche == 'q'){
            refresh();
            endwin();
            exit(EXIT_SUCCESS); /* On sort du programme proprement */
        }
        if (touche == 'r'){
            refresh();
            chrono.duree_totale = 0; /* Remise à zéro de la durée en cours */
            duree_parallele = 0; /* Remise à zéro de la durée parallèle */
            chrono.etat = 0; /* Arret du chronomètre */
            for (mise_a_zero = 0; mise_a_zero < TAILLE_MAX; mise_a_zero++)
                chrono.tab[mise_a_zero] = 0; /* Remise à éero du tableau d'entiers */
            efface_tour();
            afficher_interface(chrono, duree_parallele);
        }
        if (touche == 't'){
            duree_parallele = 0;
            ajouter_tour(&chrono);
        }
        if (touche == KEY_F(1)){ /*Augmentation du chronomètre d'une heure */
            chrono.duree_totale += 3600000; 
            duree_parallele += 3600000;
        }
        if (touche == KEY_F(2)){ /* Diminution du chronomètre d'une heure */
            chrono.duree_totale -= 3600000;
            duree_parallele -= 3600000;
        }
        if (touche == KEY_F(3)){ /* Augmentation du chronomètre d'une minute */
            chrono.duree_totale += 60000;
            duree_parallele += 60000;
        }
        if (touche == KEY_F(4)){ /* Diminution du chronomètre d'une minute */
            chrono.duree_totale -= 60000;
            duree_parallele -= 60000;
        }
        if (touche == KEY_F(5)){ /* Augmentation du chronomètre d'une seconde */
            chrono.duree_totale += 1000;
            duree_parallele += 1000;
        }
        if (touche == KEY_F(6)){ /* Diminution du chronomètre d'une seconde */
            chrono.duree_totale -= 1000;
            duree_parallele -= 1000;
        }
        if (touche == ' '){
            if (chrono.etat == 0){
                /* Mesure de temps initiale. */
                gettimeofday(&temps_debut, NULL);
                chrono.etat = 1;
            }
            else if (chrono.etat == 1)
                chrono.etat = 0;
        }
        usleep(50000); /* 500 mili-secondes */
        if (chrono.etat == 1){
            /* Mesure de temps finales. */
            gettimeofday(&temps_fin, NULL);
            chrono.duree_totale += intervalle_ms(temps_debut, temps_fin);
            duree_parallele += intervalle_ms(temps_debut, temps_fin);
            temps_debut = temps_fin;
        }
        afficher_interface(chrono, duree_parallele);
        refresh();
    }
    
    refresh();
    getch();
    endwin();
    return 0;
}