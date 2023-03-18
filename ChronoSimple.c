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
    fprintf(stderr, "%02d:%02d:%02d:%02d\r", h, m, s, c);
}

int main(void) {
    /* Variables de recuperation du temps. */
    struct timeval temps_debut, temps_fin;

    int ms = 0;
    /* Mesure de temps initiale. */
    gettimeofday(&temps_debut, NULL);


    while(1){
        usleep(50000); /* 500 mili-secondes */
        /* Mesure de temps finales. */
        gettimeofday(&temps_fin, NULL);
        ms = intervalle_ms(temps_debut, temps_fin);
        affichage(ms);
    }
    return 0;
}