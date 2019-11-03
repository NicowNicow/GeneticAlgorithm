#include <stdlib.h> // For exit()
#include <stdio.h> // For printf()
#include <math.h> // For sin() and cos()
#include "GfxLib.h" // To do simple graphics
#include "ESLib.h" // For valeurAleatoire()

#ifndef M_PI
#define M_PI 3.141592654
#endif

// Default width and height
#define WindowWidth 800
#define WindowHeight 600

#define IterationsPerRound 5000

#define PopulationMaxSize 720
#define PopulationSize MaxBound

typedef unsigned int Gene;

double deg2rad(unsigned int degrees)
{
	return degrees*M_PI/180.;
}

int fitness(Gene gene)
{
	return (int)100*(cos(deg2rad(gene))+1.);
}

Gene population[PopulationMaxSize];
int values[PopulationMaxSize];
int sumOfValues = 0;
int MaxBound = 180;

void initializePopulation(void)
{
	int index;
	sumOfValues = 0;
	for (index = 0; index < PopulationSize; ++index)
	{
		population[index] = valeurAleatoire()*MaxBound;
		values[index] = fitness(population[index]);
		sumOfValues += values[index];
	}
}

Gene crossover(Gene gene1, Gene gene2)
{
	const int mask = valeurAleatoire()*65536;
	return (gene1 & mask) | (gene2 & ~mask);
}

Gene mutation(Gene gene)
{
	const int bitNumber = valeurAleatoire()*32;
	return ((gene ^ (1<<bitNumber))&1023)%MaxBound;
}

int chooseGene(void)
{
	const int randomLevel = valeurAleatoire()*sumOfValues;
	int index = 0;
	int partialSum = 0;
	for(;;)
	{
		partialSum += values[index];
		if (randomLevel < partialSum)
			break;
		++index;
	}
	return index;
}

void replaceMin(Gene gene)
{
	int index;
	int indexMin = 0;
	for (index = 0; index < PopulationSize; ++index)
		if (values[index] < values[indexMin])
			indexMin = index;
	sumOfValues -= values[indexMin];
	population[indexMin] = gene;
	values[indexMin] = fitness(gene);
	sumOfValues += values[indexMin];
}

#define HistogramSize PopulationMaxSize
int histogram[HistogramSize];

void computeHistogram(void)
{
	int index;
	for (index = 0; index < HistogramSize; ++index)
		histogram[index] = 0;
	for (index = 0; index < PopulationSize; ++index)
		++histogram[population[index]];
}

int main(int argc, char *argv[])
{
	initialiseGfx(argc, argv);

	prepareFenetreGraphique(argv[argc-1], WindowWidth, WindowHeight);
	lanceBoucleEvenements();

	return 0;
}


// To draw a circle
void circle(float xCenter, float yCenter, float radius)
{
	const int Steps = 8; // Number of sectors to draw a circle
	const double AngularSteps = 2.*M_PI/Steps;
	int index;

	for (index = 0; index < Steps; ++index) // For each sector
	{
		const double angle = 2.*M_PI*index/Steps; // Compute the starting angle
		triangle(xCenter, yCenter,
                xCenter+radius*cos(angle), yCenter+radius*sin(angle),
			     xCenter+radius*cos(angle+AngularSteps), yCenter+radius*sin(angle+AngularSteps));
	}
}

void drawCosine(void)
{
	const int yCenter = hauteurFenetre()/2;
	int index;

	epaisseurDeTrait(1);
	couleurCourante(255, 255, 255);
	ligne(0, 0, 0, hauteurFenetre()-1);
	ligne(0, yCenter, largeurFenetre()-1, yCenter);
	couleurCourante(255, 255, 0);
	for (index = 0; index < HistogramSize; ++index)
		ligne(index, yCenter*(1+cos(deg2rad(index))), index+1, yCenter*(1+cos(deg2rad(index+1))));
}

void drawHistogram(void)
{
	const int yCenter = hauteurFenetre()/2;
	int index;
	couleurCourante(0, 255, 0);
	for (index = 0; index < HistogramSize; ++index)
	{
		if (histogram[index] != 0)
		{
			epaisseurDeTrait(1);
			ligne(index, hauteurFenetre()/2, index, hauteurFenetre()/2+3*histogram[index]);
			epaisseurDeTrait(8);
			point(index, hauteurFenetre()/2);
			point(index, yCenter*(1+cos(deg2rad(index))));
		}
	}
}

void drawValues(void)
{
	int index;
	couleurCourante(0, 255, 0);
	epaisseurDeTrait(1);
	for (index = 0; index < HistogramSize; ++index)
	{
		ligne(index, hauteurFenetre()/2, index, hauteurFenetre()/2+3*histogram[index]);
		circle(index, hauteurFenetre()/2, histogram[index]);
	}
}



void gestionEvenement(EvenementGfx event)
{
	static int counter;

	switch (event)
	{
		case Initialisation:
			initializePopulation();
			computeHistogram();
			counter = 0;
			demandeTemporisation(20);
			break;

		case Temporisation:
			if (counter > 0)
			{
				int index;
				for (index = 0; index < 10; ++index)
				{
					Gene nouveau = crossover(chooseGene(), chooseGene());
					nouveau = mutation(nouveau);
					replaceMin(nouveau);
				}
				computeHistogram();
				counter -= index;

				rafraichisFenetre();
			}
			break;

		case Affichage:
			effaceFenetre (0, 0, 0);
			drawCosine();
			drawHistogram();
			epaisseurDeTrait(2);
			couleurCourante(0, 200, 255);
			afficheChaine("1 : 180° ; 7 : 720° ; i : iterate ; r : reset", 16, 5, 5);
			break;

		case Clavier:
			switch (caractereClavier())
			{
				case '1':
					MaxBound = 180;
					initializePopulation();
					computeHistogram();
					rafraichisFenetre();
					break;

				case '7':
					MaxBound = 720;
					initializePopulation();
					computeHistogram();
					rafraichisFenetre();
					break;

				case 'Q':
				case 'q':
					termineBoucleEvenements();
					break;

				case 'i':
				case 'I':
					counter = IterationsPerRound;
					rafraichisFenetre();
					break;

				case 'R':
				case 'r':
					initializePopulation();
					computeHistogram();
					counter = 0;
					rafraichisFenetre();
					break;
			}
			break;

		case ClavierSpecial:
			break;
		case BoutonSouris:
			break;
		case Souris:
			break;
		case Inactivite:
			break;
		case Redimensionnement:
			break;
	}
}
