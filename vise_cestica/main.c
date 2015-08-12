#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define D 0.41 //: mikrometar na kvadrat / milisekund
#define time_step 0.1 // milisekunde
#define boundaries 1.000000 //: mikrometri
#define pi 3.14
#define broj_iteracija 50
#define rate_constant 4.7
#define broj_receptora 5
#define udaljenost_receptora 0.1
#define okolina 0.2
#define broj_enzima 5
#define C 0.4

int broj_cestica = 100;
int broj_vezanih_cestica = 0;

struct enzim_st
{
    double en_xcor;
    double en_ycor;
} enzim, enzimi [broj_enzima];

struct receptor_st
{
    double xcor;
    double ycor;
    double av;
    double pb;
    int zauzeto;
} receptor, receptori [broj_receptora];

typedef struct xy_st
{
    double x;
    double y;
} koordinate;

koordinate reflektovanje (double x, double y)
{
    koordinate xy;
    double l;
       if (y >= (boundaries / 2))
        {
            l = abs(y - boundaries/2);
            y = boundaries/2 - l;
        }

       if (y <= (-boundaries/2))
        {
            l = abs(- y - boundaries /2);
            y = -boundaries/2 + l;
        }

        if (x >= (boundaries / 2))
        {
            l = abs(x - boundaries/2);
            x = boundaries/2 - l;
        }

        if (x <= (-boundaries/2))
        {
            l = abs(- x - boundaries /2);
            x = - boundaries/2 + l;
        }
     // printf("%lf %lf reflektovanje \n", x, y);
      xy.x = x;
      xy.y = y;
    return xy;
}

double randn (double mu, double sigma)
{
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;

  if (call == 1)
    {
      call = !call;
      return (mu + sigma * (double) X2);
    }

  do
    {
      U1 = -1 + ((double) rand () / RAND_MAX)* 2;
      U2 = -1 + ((double) rand () / RAND_MAX)* 2;
      W = pow (U1, 2) + pow (U2, 2);
    }
  while (W >= 1 || W == 0);

  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;

  call = !call;

  return (mu + sigma * (double) X1);
}

void file (int x[], double y[])
{
    int j;
    FILE *fout;
    fout = fopen ("output.txt", "w");

        for (j = 0; j < broj_iteracija - 1; j++)
        {
            fprintf(fout, "%d %f \n", x[j], y[j]);
        }


}

int vezivanje(double x, double y)
{
    int t = 0;
    //receptor receptori [broj_receptora];
    double r = (double) rand()/RAND_MAX;
    for (t = 0; t <= broj_receptora - 1; t++)
    {
       receptori[t].xcor = -boundaries/2 + t * udaljenost_receptora;
    if (((y <= -boundaries/2 + okolina) && (r <= 0.0135)) && (x <= (receptori[t].xcor + okolina)) && (x>= receptori[t].xcor - okolina))
    {
            if (((receptori[t].zauzeto == 0) || (receptori[t].zauzeto == 1)) && (broj_vezanih_cestica < 2 * broj_receptora))
                    {
                        printf("ulazi u vezivanje \n");
                        receptori[t].zauzeto = receptori[t].zauzeto + 1;
                        broj_cestica = broj_cestica - 1;
                        broj_vezanih_cestica = broj_vezanih_cestica + 1;
                        return 1;

                    }
                   else if (receptori[t].zauzeto == 2)
                    {
                        receptori[t].zauzeto = 0;
                        return 0;
                    }

         }
    }

}

void razlaganje (double x, double y)
{
    int t = 0, i = 0;
    double r = (double) rand()/RAND_MAX;
    // koordinatni pocetak se nalazi u centru
    for (t = 1; t <= broj_enzima; t++)
    {
        if ((x <= enzimi[t].en_xcor / 2) && (x >= -enzimi[t].en_xcor / 2) && (y <= enzimi[t].en_ycor / 2) && (y >= -enzimi[t].en_ycor / 2) && (r <= 0.2))
        {
              while ((receptori[i].zauzeto == 1) || (receptori[i].zauzeto == 2))
              {
                  receptori[i].zauzeto = receptori[i].zauzeto - 1;
                  broj_vezanih_cestica = broj_vezanih_cestica - 1;
                  i++;
              }
        }
    }
}

int main()
{
    srand((unsigned)time(NULL));
    double xkoordinate_cestica[1000];
    double ykoordinate_cestica[1000];
    xkoordinate_cestica[0] = 0;
    ykoordinate_cestica[0] = 0;
    int i, j, vezan;
    int kolicina[broj_iteracija];
    double vreme[broj_iteracija];
    enzim.en_xcor = 0.3;
    enzim.en_ycor = 0.3;
    receptor.av = 6.23 ;
    receptor.pb = (rate_constant * (1/ 2 * receptor.av * receptor.xcor*receptor.ycor)*(sqrt(pi*time_step/D))) ;
    for (j = 0; j <= broj_iteracija - 1; j++)
    {
    for (i = 0; i <= broj_cestica-1; i++)
    {
        xkoordinate_cestica[i] = randn(xkoordinate_cestica[i-1], 2 * D * time_step);
        ykoordinate_cestica[i] = randn(ykoordinate_cestica[i-1], 2 * D * time_step);
        //printf("pre vezivanja %f %f \n", xkoordinate_cestica[i], ykoordinate_cestica[i]);
        vezan = vezivanje (xkoordinate_cestica[i], ykoordinate_cestica[i]);

        if (vezan == 0)
        {
            double l = abs(- ykoordinate_cestica[i]- boundaries /2);
            ykoordinate_cestica[i] = -boundaries/2 + l;
        }
        koordinate temp = reflektovanje(xkoordinate_cestica[i], ykoordinate_cestica[i]);
        xkoordinate_cestica[i] = temp.x;
        razlaganje (xkoordinate_cestica[i], ykoordinate_cestica[i]);
       // printf("posle vezivanja %f %f \n",temp.x, temp.y);
    }
    printf("%d broj vezanih cestica \n", broj_vezanih_cestica);
    kolicina[j] = broj_cestica;
    }


    int s;
    for (s = 0; s <= broj_iteracija - 1; s++)
    {
        vreme[s] = s*time_step;
    }

    file(kolicina,vreme);
    return 0;
}
