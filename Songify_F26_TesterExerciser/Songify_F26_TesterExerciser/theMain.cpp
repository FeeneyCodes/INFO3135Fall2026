
#include "cSongifyTesterExerciser.h"

// Include student's cSongify header here
//
// #include "cSongify_SOLUTION_Feeney.h"
// Be sure to rename it "cSongify" as the tester expects that.
// 
// eg. Not "cSongify_SOLUTION_Feeney.h" 
//     but--> "cSongify.h"
// 
#include "StudentSolutionFiles/cSongify.h"

// Note: This assumes that the student's cSongify header 
//       includes whatever other header files they need.


int main(int argc, char* argv[])
{
	cSongifyTesterExerciser* pTester = new cSongifyTesterExerciser();
	// Change this to whatever you'd like: 
	// (the file is generated based on this, but removes 'forbidden' characters)
	pTester->setStudentInfo("Bob Student SN:123456");

	pTester->startLog();

	if (!pTester->initializeTestData())
	{
		std::cout << "Error: Can't load test data generator." << std::endl;
		std::cout << "See if the test data is in the correct folder." << std::endl;
		std::cout << "Sorry it didn't work out; we had so much going for each other..." << std::endl;
		delete pTester;
		return -1;
	}


	cSongify* pStudentsSongify = new cSongify();
	pTester->setSongifyInstance(pStudentsSongify);

	pTester->RunAllTests();

	// Get rid of it, to be sure we start 100% fresh and clean
	pTester->setSongifyInstance(nullptr);
	delete pStudentsSongify;



	pTester->Log("--------------------------------------------");
	pTester->Log("Re-running tests with large number of users.");
	pTester->Log("--------------------------------------------");

	// Now regenerate data and run a stress test
	pTester->regenerateTestData(100, 500);

	pStudentsSongify = new cSongify();
	pTester->setSongifyInstance(pStudentsSongify);


	// Less detailed log output
	pTester->RunAllTests(false);


	// You can also run specific tests if you'd like
	// Hopefully the methods are somewhat clear...
	//	pTester->RunBasicUserTests();
	//	pTester->RunBasicSongTests();
	//	pTester->RunPlaylistTests();

	pTester->setSongifyInstance(nullptr);
	delete pStudentsSongify;

	delete pTester;

	return 0;
}
