#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define broj_receptora 10000
#define broj_iteracija 50000
#define broj_enzima 500
#define pocetni_broj_cestica 3000
#define koeficijent_difuzije 0.41 /*mikrometar na kvadrat po sekundi*/
#define time_step 0.0001

const double OKOLINA = 0.002; //razmotriti razlicite vrednosti za okolinu
const double VISINA = 0.02; //: mikrometri
const double DUZINA = 0.15; //:mikrometri
const double SIRINA = 0.02; //mikrometri
//const double PB = 1.5774265663 * 0.1; //veorvatnoca vezivanja za receptor
const double PB = 1;

int broj_rasporedjenih_receptora = 0;
int broj_rasporedjenih_cestica = 0;
int broj_reflektovanih_cestica = 0;
int broj_cestica = pocetni_broj_cestica;
double kolicina[broj_iteracija];
double q;

// definisem enzim
struct enzim_st
{
	double x;
	double y;
	double z;
	double pu;
} enzim, enzimi[broj_enzima];

// struktura receptor
typedef struct receptor_st
{
	double x;
	double y;
	int zauzeto;
} TReceptor;

TReceptor receptori[broj_receptora];

//struktura ACh
typedef struct ACh_st
{
	double x;
	double y;
	double z;
	int reflektovana;
	int vezana;
} ACh;

//	procedura za generisanje random broja iz gausove raspodele
double randn(double mu, double sigma);

// procedura za ispisivanje u fajl
void writeToFile(double x[], double y[]);

// funkcija koja rasporedjuje receptore po donjoj ploci
void rasporedjivanje_receptora();

// funkcija koja racuna novu poziju cestice za sledeci vremenski trenutak
// ulazni parametar je pocetna pozicija povratna vrednost je nova pozicija
ACh kretanje(ACh cestica);

// funckija koja vraca 1 ukoliko prosledjena cestica moze da se veze
// preduslov za ovu funkciju je da su receptori rasporedjeni
// potrebno je da je cestica.z < OKOLINA, cestica je reflektovana ako je f-ja vratila 0
int vezivanje(ACh cestica);

//sve cestice polaze iz iste tacke
void inicijalizacijaPolozajaCestica(ACh cestice[]);

// cestice se rasporedjuju po gornjoj ploci ravnomerno
// kao kod receptora
void ravnomernoRasporedjivanjeCestica(ACh cestice[]);

// sacuvaj poziciju cestica
void sacuvajPozicijuCestica(ACh cestice[], char *imeFajla);

// sacuvaj poziciju slobodnih receptora
void sacuvajPozicijuSlobodnihReceptora();

int main()
{
	//srand((unsigned)time(NULL));

	int s, i;
	double vreme[broj_iteracija];
	ACh cestice[pocetni_broj_cestica];

	rasporedjivanje_receptora();
	ravnomernoRasporedjivanjeCestica(cestice);

	sacuvajPozicijuCestica(cestice, "cestice_rasporedjene.txt");

	for (s = 0; s < broj_iteracija; s++)
	{
		vreme[s] = s*time_step;

		for (i = 0; i < broj_rasporedjenih_cestica; i++)
		{
			if (cestice[i].vezana || cestice[i].reflektovana)
			{
				continue;
			}

			cestice[i] = kretanje(cestice[i]);
			//printf("Koordinate cesice iz main-a %.5lf %.5lf %.5lf\n",cestica.x, cestica.y, cestica.z);

			if (cestice[i].z < OKOLINA)
			{
				cestice[i].vezana = vezivanje(cestice[i]);
				if(cestice[i].vezana == 0)
				{
					if (cestice[i].z < 0) {
						cestice[i].reflektovana = 1;
						broj_reflektovanih_cestica++;
					}
				}
			}
		}

		kolicina[s] = broj_cestica;
	}

	sacuvajPozicijuSlobodnihReceptora();
	sacuvajPozicijuCestica(cestice, "cestice_na_kraju.txt");

	writeToFile(kolicina, vreme);

	printf("Broj reflektovanih cestica %d\n", broj_reflektovanih_cestica);

	system("PAUSE");

	return 0;
}

void sacuvajPozicijuSlobodnihReceptora()
{
	int j;
	FILE *fout = NULL;
	fout = fopen("receptori.txt", "w");

	for (j = 0; j < broj_rasporedjenih_receptora; j++)
	{
		if (receptori[j].zauzeto != 0) {
			fprintf(fout, "%lf %lf 0.00\n", receptori[j].x, receptori[j].y);
		}
	}

	fclose(fout);


}

// sacuvaj poziciju cestica
void sacuvajPozicijuCestica(ACh cestice[], char *imeFajla)
{
	int j;
	FILE *fout = NULL;
	fout = fopen(imeFajla, "w");

	for (j = 0; j < broj_rasporedjenih_cestica; j++)
	{
		if (cestice[j].reflektovana == 1) {
			fprintf(fout, "%lf %lf %lf\n", cestice[j].x, cestice[j].y, cestice[j].z);
		}
	}

	fclose(fout);
}

//sve cestice polaze iz iste tacke
void inicijalizacijaPolozajaCestica(ACh cestice[])
{
	int i;

	for (i = 0; i < pocetni_broj_cestica; i++)
	{
		cestice[i].x = ((double)rand() / RAND_MAX) * SIRINA;
		cestice[i].y = ((double)rand() / RAND_MAX) * DUZINA;
		cestice[i].z = VISINA;
		cestice[i].reflektovana = 0;
		cestice[i].vezana = 0;
	}
}

// cestice se rasporedjuju po gornjoj ploci ravnomerno
// kao kod receptora
void ravnomernoRasporedjivanjeCestica(ACh cestice[])
{
	double pozx, pozy;
	int p = 0;
	double h = sqrt((SIRINA * DUZINA) / pocetni_broj_cestica);

	pozy = h;
	while (pozy<SIRINA) {
		pozx = h;
		while (pozx<DUZINA) {
			cestice[p].x = pozx;
			cestice[p].y = pozy;
			cestice[p].z = VISINA;
			cestice[p].reflektovana = 0;
			cestice[p].vezana = 0;
			p++;
			pozx += h;
		}
		pozy += h;
	}

	broj_rasporedjenih_cestica = p;
	broj_cestica = p;

}


// funkcija koja racuna novu poziju cestice za sledeci vremenski trenutak
//ulazni parametar je pocetna pozicija izlazni sledeca
ACh kretanje(ACh cestica)
{
	ACh pomocna_cestica = cestica;

	//reflektovane cestice se ne krecu
	if (cestica.reflektovana)
		return cestica;


	pomocna_cestica.x = randn(cestica.x, 2 * koeficijent_difuzije * time_step);
	if (pomocna_cestica.x < 0)
		pomocna_cestica.x *= -1;
	else if (pomocna_cestica.x > SIRINA)
		pomocna_cestica.x = 2 * SIRINA - pomocna_cestica.x;

	pomocna_cestica.y = randn(cestica.y, 2 * koeficijent_difuzije * time_step);
	if (pomocna_cestica.y < 0)
		pomocna_cestica.y *= -1;
	else if (pomocna_cestica.y > DUZINA)
		pomocna_cestica.y = 2 * DUZINA - pomocna_cestica.y;


	pomocna_cestica.z = cestica.z - fabs(randn(cestica.z, 2 * koeficijent_difuzije * time_step) - cestica.z);
	// posmatram razliku novog polozaja i starog, nema potrebe za ispitivanjem slucajeva, ako je nova vrednost manja od nule
	// napravi polje reflektovana cestica
	// mozda se vezala dok je prolazila
	//if (pomocna_cestica.z < 0)
	//	pomocna_cestica.reflektovana = 1;

	return pomocna_cestica;
}

// funckija koja vraca 1 ukoliko prosledjena cestica moze da se veze
// preduslov za ovu funkciju je da su receptori rasporedjeni
// potrebno je da je cestica.z < OKOLINA, cestica je reflektovana ako je f-ja vratila 0
int vezivanje(ACh cestica)
{
	int p = 0;
	double r = (double)rand() / RAND_MAX;

	if (cestica.vezana)
		return 1;

	for (p = 0; p < broj_rasporedjenih_receptora; p++)
	{
		//ispisati r i rb i pratiti njihov odnos
		//printf("Uslovi u vezivanju %d %d %d\n", fabs(cestica.y - receptori[p].y) <= OKOLINA, fabs(cestica.x - receptori[p].x) <= OKOLINA, (r <= pb) );
		if ((fabs(cestica.y - receptori[p].y) <= OKOLINA) && (fabs(cestica.x - receptori[p].x) <= OKOLINA))
		{
			if (receptori[p].zauzeto != 0 && r <= PB)
			{
				//printf("Cestica se vezala za %lf %lf receptor\n",receptori[p].x, receptori[p].y);
				receptori[p].zauzeto -= 1;
				broj_cestica = broj_cestica - 1;
				return 1;
			}
		}
	}

	return 0;
}

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


// procedura za ispisivanje u fajl
void writeToFile(double x[], double y[])
{
	int j;
	FILE *fout = NULL;
	fout = fopen("output.txt", "w");
	//fopen_s(fout, "output.txt", "w");

	for (j = 0; j < broj_iteracija; j++)
	{
		fprintf(fout, "%lf %lf \n", x[j], y[j]);
	}

	fclose(fout);
}

// funkcija koja rasporedjuje receptore po donjoj ploci
void rasporedjivanje_receptora()
{
	double pozx, pozy;
	int p = 0;
	double h = sqrt((SIRINA * DUZINA) / broj_receptora);

	pozy = h;
	while (pozy<SIRINA) {
		pozx = h;
		while (pozx<DUZINA) {
			receptori[p].x = pozx;
			receptori[p].y = pozy;
			receptori[p].zauzeto = 2;
			p++;
			pozx += h;
		}
		pozy += h;
	}

	broj_rasporedjenih_receptora = p;
}
