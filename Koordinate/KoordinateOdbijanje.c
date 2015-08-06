#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define D 0.41 //: mikrometar na kvadrat / milisekund
#define time_step 0.03 // milisekunde
#define boundaries 1.000000 //: mikrometri
#define pi 3.14
#define broj_iteracija 1000

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
    double l = 0;
    for (i = 1; i <= broj_iteracija-1; i++)
    {
        x[i] = randn(x[i-1], 2 * D * time_step);
        y[i] = randn(y[i-1], 2 * D * time_step);

        if (y[i] > (boundaries / 2))
        {
            l = abs(y[i] - boundaries/2);
            y[i] = boundaries/2 - l;
        }

        if (y[i] < (-boundaries/2))
        {
            l = abs(- y[i] - boundaries /2);
            y[i] = -boundaries/2 + l;
        }

        if (x[i] > (boundaries / 2))
        {
            l = abs(x[i] - boundaries/2);
            x[i] = boundaries/2 - l;
        }

        if (x[i] < (-boundaries/2))
        {
            l = abs(- x[i] - boundaries /2);
            x[i] = - boundaries/2 + l;
        }
        printf("%f %f \n", x[i], y[i]);
    }
    file (x, y);
    return 0;
}
