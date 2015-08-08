#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define D 0.41 //: [mikrometar na kvadrat / milisekund]
#define time_step 0.01 // milisekunde
#define boundaries 1.0000000 //: mikrometri
#define pi 3.14
#define broj_iteracija 1000
#define rate_constant 0.0047

struct receptor_st
{
    double xcor;
    double ycor;
    double av;
    double pb;
    int zauzeto;

} receptor;

void vezivanje(double x, double y)
{
    double r = (double) rand()/RAND_MAX;
    if (y == -boundaries/2)
    {
    if (r < receptor.pb)
    {       if (receptor.zauzeto == 0)
                    {
                        receptor.zauzeto = 1;
                    }
            if (receptor.zauzeto == 1)
                    {
                        receptor.zauzeto = 2;
                    }
            if (receptor.zauzeto == 2)
                    {
                      reflektovanje (receptor.xcor, receptor.ycor);
                    }

    } else
    {
        reflektovanje (receptor.xcor, receptor.ycor);
    }
}
}

void reflektovanje (double x, double y)
{
    double l = 0;
     if (y > (boundaries / 2))
        {
            l = abs(y - boundaries/2);
            y = boundaries/2 - l;
        }

        if (y < (-boundaries/2))
        {
            l = abs(- y - boundaries /2);
            y = -boundaries/2 + l;
        }

        if (x > (boundaries / 2))
        {
            l = abs(x - boundaries/2);
            x = boundaries/2 - l;
        }

        if (x < (-boundaries/2))
        {
            l = abs(- x - boundaries /2);
            x = - boundaries/2 + l;
        }

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

void file (double x[], double y[])
{
    int j;
    FILE *fout;
    fout = fopen ("output.txt", "w");

        for (j = 0; j < broj_iteracija - 1; j++)
        {
            fprintf(fout, "%f %f \n", x[j], y[j]);
        }


}


int main()
{
    srand((unsigned)time(NULL));
    double x[1000];
    double y[1000];
    x[0] = 0;
    y[0] = 0;
    int i;
    double a, b;
    //receptor.zauzeto = 0;
    //receptor.av = 6.23 * pow(10, 2);
    //receptor.pb = (rate_constant * (1/ 2 * receptor.av * receptor.xcor*receptor.ycor)*(sqrt(pi*time_step/D))) ;
    for (i = 1; i <= broj_iteracija-1; i++)
    {
        x[i] = randn(x[i-1], 2 * D * time_step);
        y[i] = randn(y[i-1], 2 * D * time_step);
        reflektovanje(x[i], y[i]);
        printf("%f %f \n", x[i], y[i]);
    }
    file (x, y);
    return 0;
}
