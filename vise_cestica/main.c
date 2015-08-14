#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define D 0.41 //: mikrometar na kvadrat / s
#define time_step 0.01 // mikrosekunde
#define sirina 0.02 //: mikrometri
#define visina 0.02 //: mikromtri
#define duzina 0.15 //:mikrometri
#define broj_iteracija 100
#define rate_constant 4.7
#define okolina 0.02
#define broj_receptora 300
#define broj_enzima 5
#define pocetni_broj 1000
int broj_cestica = 1000;
int broj_vezanih_cestica = 0;
double udaljenost_receptorax = duzina / 25;
double udaljenost_receptoraz = sirina / 12;

struct enzim_st
{
	double en_xcor;
	double en_ycor;
	double en_zcor;
	double pu;
} enzim, enzimi[broj_enzima];

struct receptor_st
{
	double xcor;
	double ycor;
	double zcor;
	double pb;
	int zauzeto;
} receptor, receptori[broj_receptora];

typedef struct xyz_st
{
	double x;
	double y;
	double z;
} koordinate;

koordinate reflektovanje(double x, double y, double z)
{
	koordinate xyz;
	double l;
	xyz.x = x;
	xyz.y = y;
	xyz.z = z;
	if (y >= (visina / 2))
	{
		l = abs(y - visina / 2);
		xyz.y = visina / 2 - l;
	}

	if (y <= (-visina / 2))
	{
		l = abs(-y - visina / 2);
		xyz.y = -visina / 2 + l;
	}

	if (x >= (duzina / 2))
	{
		l = abs(x - duzina / 2);
		xyz.x = duzina / 2 - l;
	}

	if (x <= (-duzina / 2))
	{
		l = abs(-x - duzina / 2);
		xyz.x = -duzina / 2 + l;
	}

	if (z >= (sirina / 2))
	{
		l = abs(x - sirina / 2);
		xyz.z = sirina / 2 - l;
	}

	if (z <= (-sirina / 2))
	{
		l = abs(-x - sirina / 2);
		xyz.z = -sirina / 2 + l;
	}

	return xyz;
}

double randn(double mu, double sigma)
{
	double U1, U2, W, mult;
	static double X1, X2;
	static int call = 0;

	if (call == 1)
	{
		call = !call;
		return (mu + sigma * (double)X2);
	}

	do
	{
		U1 = -1 + ((double)rand() / RAND_MAX) * 2;
		U2 = -1 + ((double)rand() / RAND_MAX) * 2;
		W = pow(U1, 2) + pow(U2, 2);
	} while (W >= 1 || W == 0);

	mult = sqrt((-2 * log(W)) / W);
	X1 = U1 * mult;
	X2 = U2 * mult;

	call = !call;

	return (mu + sigma * (double)X1);
}

void file(int x[], double y[])
{
	int j;
	FILE *fout;
	fout = fopen("output.txt", "w");

	for (j = 0; j < broj_iteracija; j++)
	{
		fprintf(fout, "%d %f \n", x[j], y[j]);
	}


}

int vezivanje(double x, double y, double z)
{
	int p = 0;
	double r = (double)rand() / RAND_MAX;
	double pb = 1.5774265663 * sqrt(time_step);

	for (p = 0; p < broj_receptora; p++)
	{
		receptori[p].xcor = -duzina / 2 + (p % 25) * udaljenost_receptorax;
		receptori[p].ycor = -visina / 2;
		receptori[p].zcor = -sirina / 2 + (p % 12) * udaljenost_receptoraz;

		if ((abs(z - receptori[p].zcor) <= okolina) && (r <= pb) && (abs(x - receptori[p].xcor) <= okolina) && (y <= receptori[p].ycor))
		{
			if (((receptori[p].zauzeto == 0) || (receptori[p].zauzeto == 1)) && (broj_vezanih_cestica < 2 * broj_receptora))
			{
				receptori[p].zauzeto = receptori[p].zauzeto + 1;
				broj_cestica = broj_cestica - 1;
				broj_vezanih_cestica = broj_vezanih_cestica + 1;
				return 1;

			}
			else if (receptori[p].zauzeto == 2)
			{
				receptori[p].zauzeto = 0;
				return 0;
			}
		}
		else return 0;
	}
	return 0;

}


void razlaganje (double x, double y, double z)
{
    int p = 0;
    double r = (double) rand()/RAND_MAX;
    enzim.pu = 0.5258088554 * sqrt (time_step);
    for (p = 0; p < broj_enzima; p++)
    {
        enzimi[p].en_xcor = -duzina/2 + (double) rand()/RAND_MAX*duzina;
        enzimi[p].en_zcor = -sirina/2 + (double) rand()/RAND_MAX*sirina;
        enzimi[p].en_ycor = visina/4;
        if ((abs(z -enzimi[p].en_zcor)<=okolina) && (r <= enzim.pu) && (abs(x-enzimi[p].en_xcor)<=okolina) && (y <= enzimi[p].en_ycor))
            {
                if ((receptori[p].zauzeto == 1) || (receptori[p].zauzeto == 2))
                        {
                        receptori[p].zauzeto = receptori[p].zauzeto - 1;
                        broj_vezanih_cestica = broj_vezanih_cestica - 1;
                        }

            }

    }
}
int main()
{
	srand((unsigned)time(NULL));

	double xkoordinate_cestica[pocetni_broj];
	double ykoordinate_cestica[pocetni_broj];
	double zkoordinate_cestica[pocetni_broj];

	double vezana_cestica[pocetni_broj];
	double xkoordinate_cestica_old[pocetni_broj];
	double ykoordinate_cestica_old[pocetni_broj];
	double zkoordinate_cestica_old[pocetni_broj];
	int i, j, vezan;

	for (i = 1; i <= pocetni_broj; i++)
	{
		xkoordinate_cestica_old[i] = 0;
		ykoordinate_cestica_old[i] = visina / 2;
		zkoordinate_cestica_old[i] = 0;
		vezana_cestica[i] = 0;
	}

	int kolicina[broj_iteracija];
	double vreme[broj_iteracija];
	//enzim.en_xcor = 0.3;
	//enzim.en_ycor = 0.3;
	//receptor.pb = 1.5774265663 * sqrt(time_step);
	for (j = 1; j <= broj_iteracija; j++)
	{
		for (i = 1; i <= pocetni_broj; i++)
		{
			if (vezana_cestica[i] == 1) continue;
			xkoordinate_cestica[i] = randn(xkoordinate_cestica_old[i], 2 * D * time_step);
			ykoordinate_cestica[i] = randn(ykoordinate_cestica_old[i], 2 * D * time_step);
			zkoordinate_cestica[i] = randn(zkoordinate_cestica_old[i], 2 * D * time_step);
			vezan = vezivanje(xkoordinate_cestica[i], ykoordinate_cestica[i], zkoordinate_cestica[i]);

			if (vezan == 0)
			{
				double l = abs(-ykoordinate_cestica[i] - visina / 2);
				ykoordinate_cestica[i] = -visina / 2 + l;
			}
			else vezana_cestica[i] = 1;

			koordinate temp = reflektovanje(xkoordinate_cestica[i], ykoordinate_cestica[i], zkoordinate_cestica[i]);
			xkoordinate_cestica[i] = temp.x;
			zkoordinate_cestica[i] = temp.z;
			razlaganje (xkoordinate_cestica[i], ykoordinate_cestica[i], zkoordinate_cestica[i]);

			xkoordinate_cestica_old[i] = xkoordinate_cestica[i];
			ykoordinate_cestica_old[i] = ykoordinate_cestica[i];
			zkoordinate_cestica_old[i] = zkoordinate_cestica[i];
		}
		printf("%d iterijacija: %d broj vezanih cestica \n", j, broj_vezanih_cestica);
		kolicina[j] = broj_cestica;
	}



	int s;
	for (s = 0; s < broj_iteracija; s++)
	{
		vreme[s] = s*time_step;
	}

	file(kolicina, vreme);

	return 0;
}
