#include <stdlib.h> // For exit()
#include <stdio.h> // For printf()
#include <math.h> // For sin() and cos()
#include "GfxLib.h" // To do simple graphics
#include "ESLib.h" // For valeurAleatoire()


#ifndef M_PI	// D�finition of a PI variable
#define M_PI 3.141592654
#endif


// Default width and height
#define WindowWidth 1024
#define WindowHeight 600


bool iterate = false;	//Declaration of an iterate variable


#define PopulationSize 100	//definition of a PopulationSize Variable


typedef unsigned int Gene;	//definition of a gene variable


double range2rad(unsigned int degrees) //Function to convert degrees into radians
{
	return degrees*M_PI/256;
}


bool phased = false;	//declaration of a phased Variable
double phase = 0.;	//Declaration of a phase variable


double function(double input)	//Function to apply cos to an input and a phase
{
	return cos(input-phase);
}


int fitness(Gene gene)	//Function chosen by the user. Here, it's in order to find the maximal value of the cos function
{
	return (int)(100*(function(range2rad(gene))+1));
}


Gene population[PopulationSize];	//Declaration of a array of genes, which size is PopulationSize
int values[PopulationSize];			//Declaration of a values array of the same size, used in computeValues
int sumOfValues = 0;				//Declaration of a sumOfValues variable, used in computeValues and chooseGene
int MaxBound = 256;					//Declaration of a maxBound variable, used in initializePopulation and Mutation


void computeValues(void)	//Function that apply the fitness function to every value stored in population and store the returned value in the values array
{
	sumOfValues = 0;
	for (int index = 0; index < PopulationSize; ++index)
	{
		values[index] = fitness(population[index]);
		sumOfValues += values[index];
	}
}


void initializePopulation(void)	//Function that initialize each value of the population array to a random value between 0 and 256 and then start computeValues()
{
	for (int index = 0; index < PopulationSize; ++index)
		population[index] = (Gene)(valeurAleatoire()*MaxBound);
	computeValues();
}


Gene crossover(Gene gene1, Gene gene2) //Function that apply OR and AND logical operators between two genes and a randomly created mask
{
	const unsigned int mask = valeurAleatoire()*65536; //16bits unsigned=> integer between 0 and 65536 stored in binary (because everything is stored in binary it's a computer ffs)
	return (Gene)((gene1 & mask) | (gene2 & ~mask)); // & and | are logical operators between two binary number. The ~ is the NOT logical operator
}


Gene mutation(const Gene gene) //Function that randomly choose to apply a mutation on a gene or not
{
	switch ((int)(valeurAleatoire()*3))
	{
		case 0: // random value
			return valeurAleatoire()*MaxBound;
		case 1: // close value
		{
			const int number = (valeurAleatoire()-.5)*MaxBound/10.;
			const int mutated = (int)gene + number;
			return (Gene)(mutated < 0 ? 0 : (mutated >= MaxBound ? MaxBound-1 : mutated)); //if mutated<0, return 0; else (if mutated>=MaxBound, return MaxBound-1; else return mutated)
		}
		default: //no mutations
			return gene;
	}
}


int chooseGene(void)	//Function used to randomly choose a gene to modify
{
	const int randomLevel = valeurAleatoire()*sumOfValues;
	int index = 0;
	int partialSum = 0;
	for(;;) //For loop without looping condition for it to stop
	{
		partialSum += values[index];
		if (randomLevel < partialSum)
			break;
		++index;
	}
	return index;
}


void replaceMin(Gene gene)  //Function that find and replace the minimal value within a gene
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


#define HistogramSize WindowWidth //Definition of a HistogramSize variable which value is WindowWidth=1024
int histogram[HistogramSize];	//Declaration of a histogram array, which size is HistogramSize


void computeHistogram(void)	//function used to set the histogram values to 0 and then put to 1 the value to which index is population[index]
{
	int index;
	for (index = 0; index < HistogramSize; ++index)
		histogram[index] = 0;
	for (index = 0; index < PopulationSize; ++index)
		++histogram[population[index]];
}


int main(int argc, char *argv[]) //Main function for gfxLib
{
	initialiseGfx(argc, argv);

	prepareFenetreGraphique(argv[argc-1], WindowWidth, WindowHeight);
	lanceBoucleEvenements();

	return 0;
}


void drawFunc(void)	//Used to draw the cosinus function on the screen
{
	const int yCenter = hauteurFenetre()/2;
	int index;

	epaisseurDeTrait(1);
	couleurCourante(255, 255, 255);
	ligne(0, 0, 0, hauteurFenetre()-1);
	ligne(0, yCenter, largeurFenetre()-1, yCenter);
	couleurCourante(255, 255, 0);
	for (index = 0; index < MaxBound; ++index)
		ligne(index, yCenter*(1+function(range2rad(index))), index+1, yCenter*(1+function(range2rad(index+1))));
}


void drawHistogram(void)	//Used to draw the histogram on on the x-axis
{
	const int yCenter = hauteurFenetre()/2;
	int index;
	couleurCourante(0, 255, 0);
	for (index = 0; index < HistogramSize; ++index)
	{
		if (histogram[index] != 0)
		{
			epaisseurDeTrait(1);
			ligne(index, yCenter, index, yCenter+3*histogram[index]);
			epaisseurDeTrait(8);
			point(index, yCenter);
			point(index, yCenter*(1+function(range2rad(index))));
		}
	}
}


void drawMaxFitness(void)	//Used to draw the best fitness
{
	int index;
	int indexMax = 0;
	for (index = 1; index < PopulationSize; ++index)
		if (values[index] > values[indexMax])
			indexMax = index;
	couleurCourante(255, 0, 0);
	const int yCenter = hauteurFenetre()/2;
	for (index = 0; index < PopulationSize; ++index)
		if (values[index] == values[indexMax])
		{
			point(population[index], yCenter);
			point(population[index], yCenter*(1+function(range2rad(population[index]))));
		}	
}


void gestionEvenement(EvenementGfx event)
{
	switch (event)
	{
		case Initialisation:
			initializePopulation();
			computeHistogram();
			iterate = false;
			phase = false;
			demandeTemporisation(20);
			break;

		case Temporisation:
			if (iterate)
			{
				if (phased)
				{
					phase += 1./128.; 
					computeValues();
				}
				int index;
				for (index = 0; index < 10; ++index)
				{
					Gene nouveau = crossover(population[chooseGene()], population[chooseGene()]);
					nouveau = mutation(nouveau);
					replaceMin(nouveau);
				}
				computeHistogram();
				
				rafraichisFenetre();
			}
			break;

		case Affichage:
			effaceFenetre (0, 0, 0);
			drawFunc();
			drawHistogram();
			drawMaxFitness();
			epaisseurDeTrait(2);
			couleurCourante(0, 200, 255);
			afficheChaine("1 : 180° ; 7 : 720° ; i : iterate ; p : phase ; r : reset", 16, 5, 5);
			break;

		case Clavier:
			switch (caractereClavier())
			{
				case '1':
					MaxBound = 256;
					initializePopulation();
					computeHistogram();
					rafraichisFenetre();
					break;

				case '7':
					MaxBound = 1024;
					initializePopulation();
					computeHistogram();
					rafraichisFenetre();
					break;

				case 'i':
				case 'I':
					iterate = !iterate;
					rafraichisFenetre();
					break;

				case 'P':
				case 'p':
					phased = !phased;
					rafraichisFenetre();
					break;
					
				case 'Q':
				case 'q':
					termineBoucleEvenements();
					break;

				case 'R':
				case 'r':
					initializePopulation();
					computeHistogram();
					rafraichisFenetre();
					break;

				case 's':
				case 'S':
					iterate = !iterate;
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
