#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double time_step = 0.01; //mikrosekunde
int broj_iteracija = 200;
double okolina = 0.02;
int pocetni_broj_cestica = 3000;
int broj_vezanih_cestica = 0;
int broj_zauzetih_receptora = 0;
double visina = 0.02; //: mikromtri
double duzina = 0.15; //:mikrometri
double sirina = 0.02; //mikrometri
int broj_receptora = 1000;
int broj_enzima = 200;

// definisem enzim
struct enzim_st
{
	double enx;
	double eny;
	double enz;
	double pu;
} enzim;

// struktura receptor
struct receptor_st
{
	double rx;
	double ry;
	double rz;
	double pb;
	int zauzeto;
} receptor;

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

/*void rasporedjivanje_receptora(duzina, sirina, broj_receptora)
{
    int p;
    receptori receptor1;
    double step_x, step_y;

    int odnos_duzina = (int)floor(broj_receptora/duzina);
    int odnos_sirina = (int)floor(broj_receptora/sirina);

    step_x = sqrt(broj_receptora * sirina / duzina);
    step_y = sqrt(broj_receptora * sirina / duzina) * sirina/duzina;

    for (p = 0; p < broj_receptora; p++)
    {
        receptor1.rx = -sirina / 2 + (p % odnos_sirina) * step_x;
		receptor1.ry = -duzina / 2 + (p % odnos_duzina) * step_y;
		receptor1.rz = -visina / 2;
    }

}
*/

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

int main()
{
    srand((unsigned)time(NULL));
    double kolicina[broj_iteracija];
	double vreme[broj_iteracija];

	int s;
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
    //vezivanje(cestica);
    kolicina[s] = pom.cy;
    FILE *isidora;
	isidora = fopen("nesto.txt", "a");
	fprintf(isidora, "%f %f %f \n", pom.cx, pom.cy, pom.cz);
	fclose(isidora);
	}

	file(kolicina, vreme);

    return 0;
}
