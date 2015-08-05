#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>

#define D 0.41
#define time_step 0.01 // milisekunde
#define boundaries 0.5 //: nanometri
#define pi 3.14

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

        for (j = 0; j < 10; j++)
        {
            fprintf(fout, "%f %f \n", x[j], y[j]);
        }


}
int main()
{
    srand((unsigned)time(NULL));
    double x[1000];
    double y[1000];
    int i;
    double k, ugao, presek_x, presek_y, hipotenuza, rastojanje, a = 0, b = 0;
    x[0] = 0;
    y[0] = 0;
    for (i = 1; i<= 9; i++)
    {
      x[i] = randn(a, sqrt(2 * D * time_step));
      y[i] = randn(a, sqrt(2 * D * time_step));

      if ((x[i] < boundaries) && (x[i] > (boundaries * (-1))) && (y[i] < boundaries) && (y[i] > (boundaries * (-1))))
{
     a = x[i];
     b = y[i];
}

         if ((y[i] > (boundaries/2)) || (y[i] < ((boundaries/2) * (-1))))
         {
            k = (x[i] - a)/(y[i] - b);
            ugao = atan(k);
            presek_x = (k*a - b - boundaries/2) / k;
            rastojanje = sqrt(((x[i] - a)*(x[i] - a)) + ( (y[i] - b)*(y[i] - b)));
            hipotenuza = sqrt(((presek_x - a)*(presek_x - a)) + (( boundaries/2 - b)*(boundaries/2 - b)));
            //: da li je ugao ostar ili tup, moze li da se nalazi u III ili IV kvadrantu???
            if (ugao < pi/2)
            {
                x[i] = a + rastojanje*cos(ugao);
                y[i] = boundaries/2 - ((rastojanje - hipotenuza) * sin(ugao));
            }
            if (ugao > pi/2)
            {
                x[i] = a - rastojanje*cos(ugao);
                y[i] = boundaries/2 - ((rastojanje - hipotenuza) * sin(ugao));
            }
         }
         if(((x[i] > (boundaries/2)) || (x[i] < ((boundaries/2) * (-1)))))
         {
            k = (x[i] - a)/(y[i] - b);
            ugao = atan(k);
            rastojanje = sqrt(((x[i] - a)*(x[i] - a)) + ( (y[i] - b)*(y[i] - b)));
            hipotenuza = sqrt(((presek_x - a)*(presek_x - a)) + (( boundaries/2 - b)*(boundaries/2 - b)));
            presek_y = b - k*a + k* boundaries/2;
            hipotenuza = sqrt((a * a) + ((presek_y - b)*(presek_y - b)));
            //: da li je ugao ostar ili tup, moze li da se nalazi u III ili IV kvadrantu???
            if (ugao < pi/2)
            {
                x[i] = boundaries/2 - ((rastojanje - hipotenuza)*sin(ugao));
                y[i] = b + rastojanje*cos(ugao);
            }
            if (ugao > pi/2)
            {
                x[i] = boundaries/2 - ((rastojanje - hipotenuza)*sin(ugao));
                y[i] = b - rastojanje*cos(ugao);
            }

         }

          printf("%f % f\n", x[i], y[i]);

    }
    file(x, y);
    return 0;
}
