#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


   double f(int n)
   {
       double p=1;
       for(int i=0;i<n;i++)
              p *=2;
            return p;
    }
   int g(int n)
   {
    return 2*n;
   }
 extern char* optarg;
 extern int optopt;


int main(int argc , char *argv[])
{

       if(argc<3 ||  argc> 4)
    {
        printf("Erreur sur le nombres d'arguments\n");
        return 0;
    }



    int opt;
    int f_First =-1;//  1: -f d'abord, 0: -g d'abord
    int f_Arg=-1 , g_Arg =-1; // recuperation des arguments de f et g
    int f_fournie=-1 ,g_fournie =-1 ; //-1 pas d'argument  : 1:argument

    while( (opt = getopt(argc ,argv,":f:g:") ) != -1 )
    {
         switch(opt)
         {
             case 'f':

                    f_Arg = atoi(optarg);
                    f_First = 1;
                    f_fournie = 1;
                    break;

             case 'g':

                   g_Arg =atoi(optarg);
                   f_First = 0;
                    g_fournie =1;
                     break;

             case  ':' :
                  if(f_fournie && optopt != 'f')
                            g_fournie = 1;

                   if(g_fournie && optopt != 'g')
                       f_fournie = 1;
                   break;

             case '?':
                 printf("option inconnue %c\n",optopt);
                 break;
         }

    }

    if(argc == 3)
    {
              if( (f_Arg !=-1 ) && (g_Arg == -1) && (f_fournie ==1 ) ) // seulement la fonction f est applée -> l'option g n'est pas choisie
                   printf("f(%d) = 2^%d =%f\n",f_Arg ,f_Arg ,f(f_Arg));

              if( (f_Arg == -1) && (g_Arg != -1) && (g_fournie) == 1) // seulement la fonction g est applée -> l'option f n'est pas choisie
                  printf("g(%d) = 2*%d =%d\n",g_Arg ,g_Arg ,g(g_Arg));

    }else
    {
        if( (f_fournie ==1 ) && (g_fournie ==1 ) )
        {
               if(f_First == 1 &&  f_Arg !=  -1) // -f est choisie en premier lieu
              {
                  int n = g(f_Arg);

                 printf("la composition fog(%d) =%f",f_Arg,f(n));
              }

               if( (f_First == 0 ) && (g_Arg != -1) )// -g est choisie en premier lieu et f n'admet pas de parametre'
              {
                 double n = f(g_Arg);
                printf("la composition gof(%d) = %d :",g_Arg,g((int)n));

              }

          }
          else
              printf("la meme option est fournie encore\n");

     }






    return 0;
}
