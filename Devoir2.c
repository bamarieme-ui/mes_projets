#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include<time.h>
#include<ctype.h>



int** creationMatrice(int);//creation de matrice carree aleatoire
void affichage(int** , int);
void detruire(int** ,int);

void sauvegarde_matrice_binaire(int ,int**, int ) ;
void chargementFichierbin(int ,int);

void sauvegarde_matrice_texte(int ,int**, int );
void chargementFichiertexte(int ,int);



/* ******************************************************DEFINITIONS */

int**  creationMatrice( int taille )
 {
   int** matrix;
    matrix  = (int**)malloc(taille * sizeof(int*));
    if( !matrix )
      exit(-1);


    for (int i = 0; i < taille; i++)
    {
       matrix[i] = (int*) malloc( taille * sizeof(int));
        if( !matrix[i] )
           exit(-1);
    }
    srand(time(NULL));

    for (int i = 0; i < taille; i++)
    {
        for (int j = 0; j < taille; j++)
           matrix[i][j] = rand() % 101; // Valeurs aléatoires entre 0 et 100

    }
    return matrix;
}

 void detruire(int** mat, int taille)
 {
  if(mat)
  {
    for(int i= 0 ; i<taille ;i++ )

        free(mat[i]);

    free(mat);
  }
}

 void sauvegarde_matrice_binaire(int fd, int** mat, int taille) {

    for(int i = 0; i < taille; i++) {

        if(write( fd, mat[i], taille* sizeof(int) ) == -1)
        {
            perror("ERREUR D'ECRITURE");
            exit(1);    // Convention : utiliser 1 pour les erreurs
         }
      }
}

 void sauvegarde_matrice_texte(int fd, int** mat, int taille) {
    char tab[32];  // Plus grand buffer
    int n;

    for(int i = 0; i < taille; i++)
    {
        for(int j = 0; j < taille; j++)
        {
            // Ajoute un espace entre les nombres
            n = sprintf(tab, "%d ", mat[i][j]);

            if( write(fd, tab, n) == -1)
            {
                perror("Erreur d'ecriture");
                exit(1);
            }
        }
        // Ajoute un retour à la ligne à la fin de chaque ligne
        if(write(fd, "\n", 1) == -1)
        {
            perror("Erreur d'ecriture");
            exit(1);
         }
  }
}


 void chargementFichierbin(int fd  ,int taille)
 {
     int nombre;
    // Lecture de chaque ligne de la matrice
    for (int i = 0; i < taille; i++)
    {
      for(int j=0; j<taille ;j++)
      {
         if (read(fd, &nombre, sizeof(int) ) <= 0)
        {
            perror("Erreur lors de la lecture de la matrice");
            exit(-1);
        }
         printf("%d ",nombre);

      }
      printf("\n");

    }

 }
 void affichage(int** mat , int taille)
{
        for (int i = 0; i < taille; i++)
        {
          for (int j = 0; j < taille; j++)
                  printf("%d ", mat[i][j]);

        printf("\n");
      }

}

void chargementFichiertexte(int fd ,int taille)
{
  int i,j;
  char buffer[50];
  char car;

      for(i = 0; i < taille; i++)
      {
        for(j=0 ;j<taille ;j++)
        {
          while ( read(fd, &car, sizeof(char) ) >0 )
          {
            if( (car != ' ') && (car !='\n')  )
            {
              sprintf( buffer + strlen(buffer) ,"%c" ,car);
            }
            else
            {
              int nombre = atoi(buffer);
              printf("%d ", nombre);
              memset(buffer ,0,50);
              break;}
          }

        }
      printf("\n");
    }

}
extern char* optarg;
extern int optopt;


int main(int argc ,char* argv[])
{

     if(argc <2)
     {
       perror("Nombre arguments insuffissants\n");
       return 0;
    }
    int** N;
    int fd;
    int opt;

    int a_donne =0;
    int c_donne =0;
    int d_donne =0;
    int f_donne =0;
    int t_donne =0;
    int b_donne =0;

    int taille;//taille de la matrice
    char* nomfichier;

   while( (opt = getopt(argc ,argv , ":acd:f:tb")) != -1)
    {
      switch(opt)
      {
            case 'c':
                    c_donne =1;
                    break;
            case 'd':
                    d_donne =1; taille = atoi(optarg);
                    break;
            case 'f':
                   f_donne = 1;  nomfichier =strdup(optarg);
                  break;
            case 't' :
                  t_donne =1;
                   break;
            case 'b':
                   t_donne= 1;
                   break;
             case 'a':
                   a_donne= 1;
                   break;
            case ':' :
                   printf("option %c attend un argument",optopt);
                    break;
            case '?':
                    printf("option inconnue");
                    break;
      }
    }

    int bol =( (c_donne == 1 )&& (d_donne == 1) );

    if(  (b_donne) && (t_donne) )
    {
      printf("choisissez une seule option entre -t et -b");
      return 0;

    }

      if( bol && f_donne)
      {

        fd = open(nomfichier ,O_RDWR|O_CREAT|O_TRUNC,0644);
        if(fd == -1)
        {
          perror("erreur d'ouverture du fichier");
          exit(-1);
        }
          N = creationMatrice(taille);
            affichage(N ,taille);

        if(b_donne)
            sauvegarde_matrice_binaire(fd ,N,taille);

        if(t_donne)
          sauvegarde_matrice_texte(fd , N, taille);

        if( (!b_donne) && (!t_donne) )
           sauvegarde_matrice_binaire(fd ,N,taille);

         close(fd);
        detruire(N,taille);
    }



    if(  (a_donne) &&(d_donne)  && (f_donne) )
    {

      fd = open(nomfichier ,O_RDONLY ,0644);
        if(fd == -1)
        {
          perror("erreur d'ouverture du fichier");
          exit(-1);
        }

          if(b_donne)
            chargementFichierbin(fd ,taille);


          if(t_donne)
             chargementFichiertexte(fd , taille);

        if( (!b_donne) && (!t_donne) )
        {
           chargementFichierbin(fd ,taille);
           printf("\n");
        }

         close(fd);

    }


    return 0;
}
