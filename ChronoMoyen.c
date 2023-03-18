/* Auteur : Gaël PAQUETTE & Aloui DHIA
 * Création : 17/02/2022
 * Dernière modification : 17/02/2022 */
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>

int intervalle_ms(struct timeval debut, struct timeval fin){
    /* Variables pour recueillir les mesures. */
    int sec, usec;

    sec = (fin.tv_sec - debut.tv_sec) * 1000;
    usec = (int)rint(0.001 * (fin.tv_usec - debut.tv_usec));

    return sec + usec;    
}

int nb_ms_vers_centiemes(int nb_ms){
    assert(nb_ms >= 0);

    return ((nb_ms / 10) % 100);
}

int nb_ms_vers_secondes(int nb_ms){
    assert(nb_ms >= 0);

    return ((nb_ms / 1000) % 60);
}

int nb_ms_vers_minutes(int nb_ms){
    assert(nb_ms >= 0);

    return ((nb_ms / (1000 * 60)) % 60);
}

int nb_ms_vers_heures(int nb_ms){
    assert(nb_ms >= 0);
    
    return ((nb_ms / (1000 * 60 * 60)) % 100);
}

void affichage(int time){
    int c, s, m, h;
    c = nb_ms_vers_centiemes(time);
    s = nb_ms_vers_secondes(time);
    m = nb_ms_vers_minutes(time);
    h = nb_ms_vers_heures(time);
    mvprintw(1, 10, "                   ");
    mvprintw(1, 10, "%02d:%02d:%02d:%02d", h, m, s, c);
    refresh();
}

int main(void) {
    initscr();
    int touche, etat_chrono = 0, duree_totale = 0;
    /* Variables de recueration du temps. */
    struct timeval temps__debut, temps_fin;
    
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    mousemask(ALL_MOUSE_EVENTS
        | REPORT_MOUSE_POSITION, NULL);
    
    /* Présentation */
    mvprintw(0, 5, "-- Chrono --");
    mvprintw(1, 0, "Runtime :");
    
    while (1) {
        touche = getch();
        if (touche == '\n'){
            refresh();
            endwin();
            exit(EXIT_SUCCESS);
        }
        if (touche == ' '){
            if (etat_chrono == 0){
                /* Mesure de temps initiale. */
                gettimeofday(&temps__debut, NULL);
                etat_chrono = 1;
            }
            else if (etat_chrono == 1)
                etat_chrono = 0;
        }
        usleep(50000); /* 500 mili-secondes */
        if (etat_chrono == 1){
            /* Mesure de temps finales. */
            gettimeofday(&temps_fin, NULL);
            duree_totale += intervalle_ms(temps__debut, temps_fin);
            temps__debut = temps_fin;
        }
        affichage(duree_totale);
        refresh();
    }
    
    refresh();
    getch();
    endwin();
    return 0;
}