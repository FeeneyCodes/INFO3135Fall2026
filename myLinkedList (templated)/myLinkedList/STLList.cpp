#include "cPerson.h"
#include <list>

void doSTL(void)
{
	std::list<cPerson> myList;

	cPerson Bob;
	Bob.Name = "Bob";
	Bob.Gender = 'M';
	Bob.Population = 18829;

	cPerson Sallly;
	Sallly.Name = "Sallly";
	Sallly.Gender = 'F';
	Sallly.Population = 3433;

	myList.insert(myList.begin(), Bob);
	myList.insert(myList.end(), Sallly);
	//myList.insert(myList.begin(), Terry);



	return;
}