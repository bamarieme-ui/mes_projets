#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void split(char *nomFichier, int taille) {
    int ficd = open(nomFichier, O_RDONLY);
    if (ficd == -1) {
        printf("Erreur : Impossible d'ouvrir le fichier source '%s'.\n", nomFichier);
        exit(-1);
    }

    char buf[1024];
    int cp = 1;

    while (1) {
        char nomPartie[256];
        snprintf(nomPartie, sizeof(nomPartie), "%s.part%d", nomFichier, cp);

        int ficdPartie = open(nomPartie, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (ficdPartie == -1) {
            printf("Erreur : Impossible de créer la partie '%s'.\n", nomPartie);
            close(ficd);
            exit(-1);
        }

        int nbrestant = taille;
        while (nbrestant > 0) {
            int aLire = (nbrestant < (int)sizeof(buf)) ? nbrestant : sizeof(buf);
            int lus = read(ficd, buf, aLire);
            if (lus == -1) {
                printf("Erreur : Lecture échouée sur le fichier source '%s'.\n", nomFichier);
                close(ficd);
                close(ficdPartie);
                exit(-1);
            }
            if (lus == 0) break; // Fin du fichier

            int ecrit = write(ficdPartie, buf, lus);
            if (ecrit == -1) {
                printf("Erreur : Écriture échouée dans la partie '%s'.\n", nomPartie);
                close(ficd);
                close(ficdPartie);
                exit(-1);
            }

            nbrestant -= ecrit;
        }

        close(ficdPartie);
        cp++;
        if (nbrestant > 0) break; // Fin du fichier
    }

    close(ficd);
    printf("Fichier '%s' découpé en %d parties.\n", nomFichier, cp - 1);
}

void join(const char *nomFichier, int nbParties, char *parties[]) {
    int ficdFinal = open(nomFichier, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (ficdFinal == -1) {
        printf("Erreur : Impossible de créer le fichier final '%s'.\n", nomFichier);
        exit(EXIT_FAILURE);
    }

    char buf[1024];
    for (int i = 0; i < nbParties; i++) {
        int ficdPartie = open(parties[i], O_RDONLY);
        if (ficdPartie == -1) {
            printf("Erreur : Impossible d'ouvrir la partie '%s'.\n", parties[i]);
            close(ficdFinal);
            exit(EXIT_FAILURE);
        }

        while (1) {
            int lus = read(ficdPartie, buf, sizeof(buf));
            if (lus == -1) {
                printf("Erreur : Lecture échouée sur la partie '%s'.\n", parties[i]);
                close(ficdPartie);
                close(ficdFinal);
                exit(EXIT_FAILURE);
            }
            if (lus == 0) break; // Fin de la partie

            int ecrit = write(ficdFinal, buf, lus);
            if (ecrit == -1) {
                printf("Erreur : Écriture échouée dans le fichier final '%s'.\n", nomFichier);
                close(ficdPartie);
                close(ficdFinal);
                exit(EXIT_FAILURE);
            }
        }

        close(ficdPartie);
    }

    close(ficdFinal);
    printf("Fichier '%s' recréé à partir des parties fournies.\n", nomFichier);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage pour découper : %s split nomFichier taille\n", argv[0]);
        printf("Usage pour assembler : %s join nomFichierFinal partie1 partie2 ...\n", argv[0]);
        exit(-1);
    }

    if (argc == 3)
    {
         char *nomFichier = argv[1];
         int taille = atoi(argv[2]);
        if (taille <= 0)
        {
            printf("Erreur : La taille doit être un entier positif.\n");
            exit(-1);
        }

        split(nomFichier, taille);
    }
    else
    {
        if(argc >3)
        {
            char *nomFichier = argv[1];
            int nombrepart = argc -2;
            join(nomFichier ,nombrepart ,&argv[2] );
        }
    }





    return 0;
}
