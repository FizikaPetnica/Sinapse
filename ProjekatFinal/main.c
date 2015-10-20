#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int broj_receptora = 1000;
double time_step = 0.01; //mikrosekunde
int broj_iteracija = 500;
double okolina = 0.02;
int pocetni_broj_cestica = 3000;
int broj_vezanih_cestica = 0;
int broj_cestica = 3000;
int broj_zauzetih_receptora = 0;
int broj_razlozenih_cestica = 0;
double visina = 0.02; //: mikromtri
double duzina = 0.15; //:mikrometri
double sirina = 0.02; //mikrometri
int broj_enzima = 200;
double kolicina[200];

// definisem enzim
struct enzim_st
{
	double x;
	double y;
	double z;
	double pu;
} enzim, enzimi[1000];

// struktura receptor
struct receptor_st
{
	double x;
	double y;
	double z;
	double pb;
	int zauzeto;
} receptor, receptori[1000]; // iz nekog razloga ovde se mora staviti konkretan broj, u suprotnom javi gresku

//struktura ACh
typedef struct ACh_st
{
	double cx;
	double cy;
	double cz;
	double Dx;  //: mikrometar na kvadrat / s
	double Dy;  //: mikrometar na kvadrat / s
	double Dz;  //: mikrometar na kvadrat / s
} ACh;

//procedura za generisanje random broja iz gausove raspodele
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

// procedura za ucitavanje u fajl
void file(double x[], double y[])
{
	int j;
	FILE *fout;
	fout = fopen("output.txt", "w");

	for (j = 0; j < broj_iteracija; j++)
	{
		fprintf(fout, "%f %f \n", x[j], y[j]);
	}


}

void rasporedjivanje_receptora(duzina, sirina, broj_receptora)
{
    int p;
    double step_x, step_y;

    int odnos_duzina = (int)floor(broj_receptora/duzina);
    int odnos_sirina = (int)floor(broj_receptora/sirina);

    step_x = sqrt(broj_receptora * sirina / duzina);
    step_y = sqrt(broj_receptora * sirina / duzina) * sirina/duzina;

    for (p = 0; p < broj_receptora; p++)
    {
        receptori[p].x = -sirina / 2 + (p % odnos_sirina) * step_x;
		receptori[p].y = -duzina / 2 + (p % odnos_duzina) * step_y;
		receptori[p].z = -visina / 2;
		receptori[p].zauzeto = 0;
		receptori[p].pb = 1.5774265663 * sqrt(time_step);
    }

}

void rasporedjivanje_enzima(duzina, sirina, visina, broj_enzima)
{
    int g;
    for (g = 0; g < broj_enzima; g++)
    {
                enzimi[g].x = ((double)rand() / RAND_MAX) * sirina/2;
                enzimi[g].y = ((double)rand() / RAND_MAX) * duzina/2;
                enzimi[g].z = ((double)rand() / RAND_MAX) * visina/2;
                enzimi[g].pu = 1.5774265663 * sqrt(time_step);
    }
}


ACh kretanje(ACh cestica)
{
        ACh pomocna_cestica;

        pomocna_cestica.cx = randn(cestica.cx, 2 * cestica.Dx * time_step);
        if ((pomocna_cestica.cx >= sirina/2) || (pomocna_cestica.cx< -sirina/2))
        {
            double l = abs(pomocna_cestica.cx - sirina/2);
            pomocna_cestica.cx = sirina/2 - l;
            pomocna_cestica.Dx = pomocna_cestica.Dx * (-1);
        }

        pomocna_cestica.cy = randn(cestica.cy, 2 * cestica.Dy * time_step);
        if ((pomocna_cestica.cy >= duzina/2) || (pomocna_cestica.cy< -duzina/2))
        {
            double l = abs(pomocna_cestica.cy - duzina/2);
            pomocna_cestica.cy = duzina/2 - l;
            pomocna_cestica.Dy = pomocna_cestica.Dy * (-1);
        }

        pomocna_cestica.cz = randn(cestica.cz, 2 * cestica.Dz * time_step);
        if ((pomocna_cestica.cz >= visina/2) || (pomocna_cestica.cz< -visina/2))
        {
            double l = abs(pomocna_cestica.cz - visina/2);
            pomocna_cestica.cz = visina/2 - l;
            pomocna_cestica.Dz = pomocna_cestica.Dz * (-1);
        }

        return pomocna_cestica;

}

void vezivanje(ACh cestica)
{
    int p;
	double r = (double)rand() / RAND_MAX;
	double pb = 1.5774265663 * sqrt(time_step);

		if ((abs(cestica.cy - receptor.y) <= okolina) && (r <= pb) && (abs(cestica.cx - receptor.x) <= okolina) && (cestica.cz >= 0) && (cestica.cz < visina))
		{
			if (receptor.zauzeto != 2)
			{
				receptor.zauzeto += 1;
				broj_vezanih_cestica += 1;
				broj_cestica = broj_cestica - 1;

			}
			else
			{
			    receptor.zauzeto = 0;
				kretanje(cestica);
			}
        }

}

void razlaganje (ACh cestica)
{
    double r = (double)rand() / RAND_MAX;
    enzim.pu = 1.5774265663 * sqrt(time_step);
    if ((abs(cestica.cz - enzim.z) <= okolina) && (abs(cestica.cx - enzim.x) <= okolina) && (cestica.cy <= enzim.y) && (r <= enzim.pu))
            {

                       broj_cestica = broj_cestica - 1;
                       broj_razlozenih_cestica = broj_razlozenih_cestica + 1;
            }

}
int main()
{
    srand((unsigned)time(NULL));
	double vreme[broj_iteracija];

	int s, i;
    ACh cestica;
    ACh pom;
	cestica.cx = 0;
	cestica.cy = 0;
	cestica.cz = visina/2;
	cestica.Dx = 0.41;
	cestica.Dy = 0.41;
	cestica.Dz = -0.41;

	for (s = 0; s < broj_iteracija; s++)
	{
    vreme[s] = s*time_step;
    pom = kretanje(cestica);
    vezivanje(cestica);
    razlaganje(cestica);
    //FILE *isidora;
	//isidora = fopen("nesto.txt", "a");
	//fprintf(isidora, "%f %f %f \n", pom.cx, pom.cy, pom.cz);
	//fclose(isidora);
	kolicina[s] = broj_cestica;
	}
    printf("%d", broj_razlozenih_cestica);
	file(kolicina, vreme);

    return 0;
}
