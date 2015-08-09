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
int broj_cestica = 50;

struct receptor_st
{
    double xcor;
    double ycor;
    double av;
    double pb;
    int zauzeto;
} receptor;

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
      printf("%lf %lf reflektovanje \n", x, y);
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
      int t;
    double r = (double) rand()/RAND_MAX;
    if ((y <= -boundaries/2 + 0.2) && (r <= 0.2))
    {
           for (t = 0; t <= broj_receptora -1; t++)
           {
            if (x <= - boundaries / 2 + t * 0.1 + 0.005)
            {
            if ((receptor.zauzeto == 0) || (receptor.zauzeto == 1))
                    {
                        printf("ulazi u vezivanje \n");
                        receptor.zauzeto = receptor.zauzeto + 1;
                        broj_cestica = broj_cestica - 1;

                    }
                    if (receptor.zauzeto == 2)
                    {
                        receptor.zauzeto = 0;
                    }
            }

           }
    }
      return receptor.zauzeto;

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
    receptor.av = 6.23 ;
    receptor.pb = (rate_constant * (1/ 2 * receptor.av * receptor.xcor*receptor.ycor)*(sqrt(pi*time_step/D))) ;
    for (j = 0; j <= broj_iteracija - 1; j++)
    {
    for (i = 1; i <= broj_cestica-1; i++)
    {
        xkoordinate_cestica[i] = randn(xkoordinate_cestica[i-1], 2 * D * time_step);
        ykoordinate_cestica[i] = randn(ykoordinate_cestica[i-1], 2 * D * time_step);
        printf("pre vezivanja %f %f \n", xkoordinate_cestica[i], ykoordinate_cestica[i]);
        vezan = vezivanje (xkoordinate_cestica[i], ykoordinate_cestica[i]);

        if (!((vezan == 1) || (vezan == 2)))
        {
           double l = abs(- ykoordinate_cestica[i]- boundaries /2);
            ykoordinate_cestica[i] = -boundaries/2 + l;
        }
        koordinate temp = reflektovanje(xkoordinate_cestica[i], ykoordinate_cestica[i]);
        xkoordinate_cestica[i] = temp.x;
        printf("posle vezivanja %f %f \n",temp.x, temp.y);
    }
    printf("%dbroj cestica \n", broj_cestica);
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
