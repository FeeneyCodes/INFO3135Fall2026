#include "cSongifyTesterExerciser.h"
#include "formattingHelperFunctions.h"
#include <algorithm>

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

bool cSongifyTesterExerciser::RunBasicUserTests(bool bVerboseLogOutput /*=true*/)
{
	if (!this->m_pLog)
	{
		this->m_pLog = new cLogger(this->m_studentInfo);
		this->m_pLog->bAddTimeToLogLines = true;
	}

	this->m_bVerboseLogOutput = bVerboseLogOutput;

	this->m_pLog->LogPrintLineBreak(60);
	this->m_pLog->Log("RunBasicUserTests() starting.");
	if (!this->m_bVerboseLogOutput)
	{
		this->m_pLog->Log("(Generating limited/non-verbose output)");
	}
	this->m_pLog->LogPrintLineBreak(60);

	// Add a bunch of users
	this->m_pLog->Log("Adding a bunch of users with AddUser()");
	this->m_addTestResult("AddUser()");
	std::string errorString = "";
	unsigned int errorCount = 0;
	for (unsigned int count = 0; count != this->m_vecSomePeople.size(); count++)
	{
		if (this->m_pSongify->AddUser(this->m_vecSomePeople[count], errorString))
		{
			if (this->m_bVerboseLogOutput)
			{
				this->m_pLog->Log("(CORRECT) Added user: " + printablePerson(this->m_vecSomePeople[count], true));
				this->m_testResult_Success("AddUser()");
			}
		}
		else
		{
			this->m_pLog->Log("Error: Failed to add user: " + printablePerson(this->m_vecSomePeople[count], true));
			this->m_pLog->Log(" ..returned this error: " + errorString);
			this->m_testResult_Failure("AddUser()");
			errorCount++;
		}
	}
	this->m_pLog->appendText("Added ");
	this->m_pLog->appendNumber(this->m_vecSomePeople.size() - errorCount);
	this->m_pLog->appendText("users.");
	this->m_pLog->Log_OutputAppendedText();
	if (errorCount > 0)
	{
		this->m_pLog->appendText("Error: Failed to add ");
		this->m_pLog->appendNumber(errorCount);
		this->m_pLog->appendText(" users.");
		this->m_pLog->Log_OutputAppendedText();
	}


	this->m_pLog->LogPrintLineBreak();

	// ------------------------------
	// Add SAME user - check return
	cPerson* pDuplicate = this->m_getRandom_pUser();
	this->m_pLog->Log("Attempting to add this person again:");
	this->m_pLog->Log(printablePerson(*pDuplicate, true));
	if (!this->m_pSongify->AddUser(*pDuplicate, errorString))
	{
		this->m_pLog->Log("(CORRECT) duplicate user rejected with error: " + errorString);
		this->m_testResult_Success("AddUser()");
	}
	else
	{
		this->m_pLog->Log("Error: A duplicate user may have been added - AddUser() returned true.");
		this->m_testResult_Failure("AddUser()");

	}

	this->m_pLog->LogPrintLineBreak();

	// ------------------------------
	// Testing GetUsers() but ignoring the order
	{
		this->m_pLog->Log("Checking if all users were added using GetUsers(), ignoring order.");

		// For testing we are NOT going to delete this in case student is doing something odd with it
		cPerson* pAllUsers = nullptr;
		unsigned int allUsersArraySize = 0;
		if (!this->m_pSongify->GetUsers(pAllUsers, allUsersArraySize, true))
		{
			this->m_pLog->Log("Error/warning: GetUsers() returned false (should have returned true).");
		}
		std::vector<std::string> vecCompareErrorDetails;
		unsigned int numUsersMissing = 0;
		if (this->compareUserLists(this->m_vecSomePeople, pAllUsers, allUsersArraySize, numUsersMissing, vecCompareErrorDetails))
		{
			this->m_pLog->Log("(CORRECT) GetUers() returned all the expected users.");
			this->m_testResult_Success("GetUsers()");
		}
		else
		{
			this->m_pLog->Log("Error: GetUsers() did NOT return the expected users:");
			this->m_testResult_Failure("GetUsers()");
			if (this->m_bVerboseLogOutput)
			{
				this->m_pLog->Log("Here are the error details:");
				for (std::string line : vecCompareErrorDetails)
				{
					this->m_pLog->Log(line);
				}
			}
		}//if ( this->compareUserLists()
	}//// Testing GetUsers() but ignoring the order


	// 
	this->m_pLog->LogPrintLineBreak();

	// ------------------------------
	// Testing GetUsersByID(), compare and check order (by SongifyID)
	{
		this->m_pLog->Log("Checking GetUsersByID().");
		this->m_addTestResult("GetUsersByID()");

		// For testing we are NOT going to delete this in case student is doing something odd with it
		cPerson* pAllUsers = nullptr;
		unsigned int allUsersArraySize = 0;
		if (!this->m_pSongify->GetUsersByID(pAllUsers, allUsersArraySize))
		{
			this->m_pLog->Log("Error/warning: GetUsersByID() returned false (should have returned true).");
			this->m_testResult_Failure("GetUsersByID()");
		}

		// Copy reference people and sort
		std::vector<cPerson> vecRefPeopleInOrder;
		for (cPerson A : this->m_vecSomePeople)
		{
			vecRefPeopleInOrder.push_back(A);
		}
		this->sortPeopleBySongifyID(vecRefPeopleInOrder);

		std::vector<std::string> vecCompareErrorDetails;
		if (this->compareUserListsInOrder(vecRefPeopleInOrder, pAllUsers, allUsersArraySize, vecCompareErrorDetails))
		{
			this->m_pLog->Log("(CORRECT) GetUsersByID() returned all the expected users in order.");
			this->m_testResult_Success("GetUsersByID()");
		}
		else
		{
			this->m_pLog->Log("Error: GetUsersByID() did NOT return the expected users:");
			this->m_testResult_Failure("GetUsersByID()");
			if (this->m_bVerboseLogOutput)
			{
				this->m_pLog->Log("Here are the error details:");
				for (std::string line : vecCompareErrorDetails)
				{
					this->m_pLog->Log(line);
				}
			}
		}//if ( this->compareUserLists()
	}// Testing GetUsersByID(), compare and check order (by SongifyID)

	this->m_pLog->LogPrintLineBreak();

	// ------------------------------
	// Testing GetUsers() in ascending order...
	{
		this->m_pLog->Log("Checking GetUsers() in ascending order:");
		this->m_addTestResult("GetUers()");

		// For testing we are NOT going to delete this in case student is doing something odd with it
		cPerson* pAllUsers = nullptr;
		unsigned int allUsersArraySize = 0;
		if (!this->m_pSongify->GetUsers(pAllUsers, allUsersArraySize, true))
		{
			this->m_pLog->Log("Error/warning: GetUsers() returned false (should have returned true).");
			this->m_testResult_Failure("GetUers()", "GetUers() warning: return falue wrong");
		}

		// Copy reference people and sort
		std::vector<cPerson> vecRefPeopleInOrder;
		for (cPerson A : this->m_vecSomePeople)
		{
			vecRefPeopleInOrder.push_back(A);
		}
		this->sortPeopleByName(vecRefPeopleInOrder, true);		// <-- Ascending order

		std::vector<std::string> vecCompareErrorDetails;
		if (this->compareUserListsInOrder(vecRefPeopleInOrder, pAllUsers, allUsersArraySize, vecCompareErrorDetails))
		{
			this->m_pLog->Log("(CORRECT) GetUsers() in ascending order returned all the expected users in order.");
			this->m_testResult_Success("GetUers()");
		}
		else
		{
			this->m_pLog->Log("Error: GetUsers() in ascending order did NOT return the expected users:");
			this->m_testResult_Failure("GetUers()");
			if (this->m_bVerboseLogOutput)
			{
				this->m_pLog->Log("Here are the error details:");
				for (std::string line : vecCompareErrorDetails)
				{
					this->m_pLog->Log(line);
				}
			}
		}//if ( this->compareUserLists()
	}// Testing GetUsers() in ascending order...

	this->m_pLog->LogPrintLineBreak();

	// ------------------------------
	// Testing GetUsers() in descending order...
	{
		this->m_pLog->Log("Checking GetUsers() in descending order:");

		// For testing we are NOT going to delete this in case student is doing something odd with it
		cPerson* pAllUsers = nullptr;
		unsigned int allUsersArraySize = 0;
		if (!this->m_pSongify->GetUsers(pAllUsers, allUsersArraySize, false))
		{
			this->m_pLog->Log("Error/warning: GetUsers() returned false (should have returned true).");
			this->m_testResult_Failure("GetUers()");
		}

		// Copy reference people and sort
		std::vector<cPerson> vecRefPeopleInOrder;
		for (cPerson A : this->m_vecSomePeople)
		{
			vecRefPeopleInOrder.push_back(A);
		}
		this->sortPeopleByName(vecRefPeopleInOrder, false);		// <-- Descending order

		std::vector<std::string> vecCompareErrorDetails;
		if (this->compareUserListsInOrder(vecRefPeopleInOrder, pAllUsers, allUsersArraySize, vecCompareErrorDetails))
		{
			this->m_pLog->Log("(CORRECT) GetUsers() in descending order returned all the expected users in order.");
			this->m_testResult_Success("GetUers()");
		}
		else
		{
			this->m_pLog->Log("Error: GetUsers() in descending order did NOT return the expected users:");
			this->m_testResult_Failure("GetUers()");
			if (this->m_bVerboseLogOutput)
			{
				this->m_pLog->Log("Here are the error details:");
				for (std::string line : vecCompareErrorDetails)
				{
					this->m_pLog->Log(line);
				}
			}
		}//if ( this->compareUserLists()
	}// Testing GetUsers() in descending order...

	this->m_pLog->LogPrintLineBreak();

	{// FindUserBySIN() - Looking for a person

		this->m_pLog->Log("Checking FindUserBySIN():");
		this->m_addTestResult("FindUserBySIN()");

		cPerson* pToFind = this->m_getRandom_pUser();

		cPerson foundPerson;
		if (!this->m_pSongify->FindUserBySIN(pToFind->SIN, foundPerson))
		{
			this->m_pLog->Log("Error: FindUserBySIN() returned false, but should have returned true.");
			this->m_testResult_Failure("FindUserBySIN()");
		}
		else
		{
			this->m_pLog->Log("(CORRECT) FindUserBySIN() returned true.");
			this->m_testResult_Success("FindUserBySIN()");
		}

		std::vector<std::string> vecDifferences;
		if (this->comparePeople(*pToFind, foundPerson, vecDifferences))
		{
			this->m_pLog->Log("(CORRECT) The person returned matches the one we searched for.");
			this->m_testResult_Success("FindUserBySIN()");
		}
		else
		{
			this->m_pLog->Log("Error: The person returned did NOT match the one we searched for:");
			this->m_pLog->Log("- We looked for: " + printablePerson(*pToFind, true));
			this->m_pLog->Log("- We got this: " + printablePerson(foundPerson));
			this->m_testResult_Failure("FindUserBySIN()");
			if (this->m_bVerboseLogOutput)
			{
				this->m_pLog->Log("Here is a list of differences:");
				for (std::string line : vecDifferences)
				{
					this->m_pLog->Log("\t" + line);
				}
			}//if ( this->m_bVerboseLogOutput )
		}// if ( this->comparePeople()

		this->m_pLog->Log("Now looking for a invalid person (i.e. not added):");
		cPerson missingPerson;
		unsigned int SIN_NotInLost = this->getInvalidSIN();
		this->m_pLog->appendText("Looking for this SIN: ");
		this->m_pLog->appendNumber(SIN_NotInLost);
		this->m_pLog->Log_OutputAppendedText();
		if (this->m_pSongify->FindUserBySIN(SIN_NotInLost, missingPerson))
		{
			this->m_pLog->Log("Error: FindUserBySIN() returned true, but should have returned false.");
			this->m_pLog->Log("It returned this person: " + printablePerson(missingPerson, true));
			this->m_testResult_Failure("FindUserBySIN()");
		}
		else
		{
			this->m_pLog->Log("(CORRECT) FindUserBySIN() returned false.");
			this->m_testResult_Success("FindUserBySIN()");
			// Did they load anything into the found person?
			// (they shouldn't have)
			cPerson blankPerson;
			if (!this->comparePeople(blankPerson, missingPerson, true))
			{
				this->m_pLog->Log("Error: But returned a person (shouldn't have):");
				this->m_pLog->Log("It returned this person: " + printablePerson(missingPerson, true));
				this->m_testResult_Failure("FindUserBySIN()", "FindUserBySIN() updated person when it shouldn't have");
			}
		}

	}// FindUserBySIN(), compare

	this->m_pLog->LogPrintLineBreak();

	{// FindUserBySongifyID(), compare

		this->m_pLog->Log("Checking FindUserBySongifyID():");
		this->m_addTestResult("FindUserBySongifyID()");

		cPerson* pToFind = this->m_getRandom_pUser();

		cPerson foundPerson;
		if (!this->m_pSongify->FindUserBySongifyID(pToFind->getSongifyUniqueUserID(), foundPerson))
		{
			this->m_pLog->Log("Error: FindUserBySongifyID() returned false, but should have returned true.");
			this->m_testResult_Failure("FindUserBySongifyID()");
		}
		else
		{
			this->m_pLog->Log("(CORRECT) FindUserBySongifyID() returned true.");
			this->m_testResult_Success("FindUserBySongifyID()");
		}

		std::vector<std::string> vecDifferences;
		if (this->comparePeople(*pToFind, foundPerson, vecDifferences))
		{
			this->m_pLog->Log("(CORRECT) The person returned matches the one we searched for.");
			this->m_testResult_Success("FindUserBySongifyID()");
		}
		else
		{
			this->m_pLog->Log("Error: The person returned did NOT match the one we searched for:");
			this->m_pLog->Log("- We looked for: " + printablePerson(*pToFind, true));
			this->m_pLog->Log("- We got this: " + printablePerson(foundPerson));
			this->m_testResult_Failure("FindUserBySongifyID()");
			if (this->m_bVerboseLogOutput)
			{
				this->m_pLog->Log("Here is a list of differences:");
				for (std::string line : vecDifferences)
				{
					this->m_pLog->Log("\t" + line);
				}
			}//if ( this->m_bVerboseLogOutput )
		}// if ( this->comparePeople()

		this->m_pLog->Log("Now looking for a invalid person (i.e. not added):");
		cPerson missingPerson;
		unsigned int ID_NotInLost = this->getInvalidSongifyID();
		this->m_pLog->appendText("Looking for this SongifyID: ");
		this->m_pLog->appendNumber(ID_NotInLost);
		this->m_pLog->Log_OutputAppendedText();
		if (this->m_pSongify->FindUserBySongifyID(ID_NotInLost, missingPerson))
		{
			this->m_pLog->Log("Error: FindUserBySongifyID() returned true, but should have returned false.");
			this->m_pLog->Log("It returned this person: " + printablePerson(missingPerson, true));
			this->m_testResult_Failure("FindUserBySongifyID()");
		}
		else
		{
			this->m_pLog->Log("(CORRECT) FindUserBySongifyID() returned false.");
			this->m_testResult_Success("FindUserBySongifyID()");
			// Did they load anything into the found person?
			// (they shouldn't have)
			cPerson blankPerson;
			if (!this->comparePeople(blankPerson, missingPerson, true))
			{
				this->m_pLog->Log("Error: But returned a person (shouldn't have):");
				this->m_pLog->Log("It returned this person: " + printablePerson(missingPerson, true));
				this->m_testResult_Failure("FindUserBySongifyID()", "FindUserBySongifyID() updated person when it should not have");
			}
		}
	}// FindUserBySongifyID(), compare


	this->m_pLog->LogPrintLineBreak();

	{// FindUser() is the same as FindUserBySongifyID()

		this->m_pLog->Log("Checking FindUser():");
		this->m_pLog->Log("   (...which is the same as FindUserBySongifyID() )");
		this->m_addTestResult("FindUser()");

		cPerson* pToFind = this->m_getRandom_pUser();

		cPerson foundPerson;
		if (!this->m_pSongify->FindUser(pToFind->getSongifyUniqueUserID(), foundPerson))
		{
			this->m_pLog->Log("Error: FindUser() returned false, but should have returned true.");
			this->m_testResult_Failure("FindUser()");
		}
		else
		{
			this->m_pLog->Log("(CORRECT) FindUser() returned true.");
			this->m_testResult_Success("FindUser()");
		}

		std::vector<std::string> vecDifferences;
		if (this->comparePeople(*pToFind, foundPerson, vecDifferences))
		{
			this->m_pLog->Log("(CORRECT) The person returned matches the one we searched for.");
			this->m_testResult_Success("FindUser()");
		}
		else
		{
			this->m_pLog->Log("Error: The person returned did NOT match the one we searched for:");
			this->m_pLog->Log("- We looked for: " + printablePerson(*pToFind, true));
			this->m_pLog->Log("- We got this: " + printablePerson(foundPerson));
			this->m_testResult_Failure("FindUser()");
			if (this->m_bVerboseLogOutput)
			{
				this->m_pLog->Log("Here is a list of differences:");
				for (std::string line : vecDifferences)
				{
					this->m_pLog->Log("\t" + line);
				}
			}//if ( this->m_bVerboseLogOutput )
		}// if ( this->comparePeople()

		this->m_pLog->Log("Now looking for a invalid person (i.e. not added):");
		cPerson missingPerson;
		unsigned int ID_NotInLost = this->getInvalidSongifyID();
		this->m_pLog->appendText("Looking for this SongifyID: ");
		this->m_pLog->appendNumber(ID_NotInLost);
		this->m_pLog->Log_OutputAppendedText();
		if (this->m_pSongify->FindUserBySongifyID(ID_NotInLost, missingPerson))
		{
			this->m_pLog->Log("Error: FindUser() returned true, but should have returned false.");
			this->m_pLog->Log("It returned this person: " + printablePerson(missingPerson, true));
			this->m_testResult_Failure("FindUser()");
		}
		else
		{
			this->m_pLog->Log("(CORRECT) FindUser() returned false.");
			this->m_testResult_Success("FindUser()");
			// Did they load anything into the found person?
			// (they shouldn't have)
			cPerson blankPerson;
			if (!this->comparePeople(blankPerson, missingPerson, true))
			{
				this->m_pLog->Log("Error: But returned a person (shouldn't have):");
				this->m_pLog->Log("It returned this person: " + printablePerson(missingPerson, true));
				this->m_testResult_Failure("FindUser()");
			}
		}
	}// FindUserBySongifyID(), compare



	this->m_pLog->LogPrintLineBreak();

	// UpdateUser() - various data, then FindUser(), compare
	{// UpdateUser() 

		this->m_pLog->Log("Checking UpdateUser():");
		this->m_addTestResult("UpdateUser()");

		cPerson* pTheUsers = nullptr;
		unsigned int numUsers = 0;
		this->m_pLog->Log("Calling GetUers()...");
		this->m_pSongify->GetUsers(pTheUsers, numUsers, true);
		if (numUsers == 0)
		{
			this->m_pLog->Log("Error: GetUsers() didn't return any users.");
			this->m_pLog->Log("(so we can't do the rest of this test)");
			this->m_testResult_Failure("GetUsers()");
		}
		else
		{
			this->m_pLog->appendText("GetUsers() returned ");
			this->m_pLog->appendNumber(numUsers);
			this->m_pLog->appendText(" users.");
			this->m_pLog->Log_OutputAppendedText();
			this->m_testResult_Success("GetUsers()");

			// Pick a user in the middle of the list (aka randomly)
			unsigned int indexUserToUpdate = numUsers / 2;
			this->m_pLog->Log("Updating this users: " + printablePerson(pTheUsers[indexUserToUpdate], true));
			cPerson personToUpdate = pTheUsers[indexUserToUpdate];

			// Make a new users will all different stuff
			cPerson newPerson = this->m_pThePeople->generateRandomPerson();

			// Change everything BUT the SIN and SongifyID
			personToUpdate.first = newPerson.first;
			personToUpdate.middle = newPerson.middle;
			personToUpdate.last = newPerson.last;
			personToUpdate.gender = newPerson.gender;
			personToUpdate.streetNumber = newPerson.streetNumber;
			personToUpdate.streetName = newPerson.streetName;
			personToUpdate.streetType = newPerson.streetType;
			personToUpdate.streetDirection = newPerson.streetDirection;
			personToUpdate.city = newPerson.city;
			personToUpdate.province = newPerson.province;
			memcpy(personToUpdate.postalCode, newPerson.postalCode, 6);
			memcpy(personToUpdate.phoneNumbers[0], newPerson.phoneNumbers[0], 11);
			memcpy(personToUpdate.phoneNumbers[1], newPerson.phoneNumbers[1], 11);
			memcpy(personToUpdate.phoneNumbers[2], newPerson.phoneNumbers[2], 11);
			memcpy(personToUpdate.phoneNumbers[3], newPerson.phoneNumbers[3], 11);

			this->m_pLog->Log("User's info now this: " + printablePerson(personToUpdate, true));

			std::string errorString;
			if (this->m_pSongify->UpdateUser(personToUpdate, errorString))
			{
				this->m_pLog->Log("(CORRECT) UpdateUser() returned true.");
				this->m_testResult_Success("UpdateUser()");

				cPerson updatedPerson;
				if (this->m_pSongify->FindUser(personToUpdate.getSongifyUniqueUserID(), updatedPerson))
				{
					std::vector<std::string> vecDifferences;
					if (this->comparePeople(personToUpdate, updatedPerson, vecDifferences))
					{
						this->m_pLog->Log("(CORRECT) And the user was updated correctly.");
						this->m_testResult_Success("UpdateUser()");
					}
					else
					{
						this->m_pLog->Log("Error: The user was not updated correctly (found user didn't match updates).");
						this->m_pLog->Log("Original user: ");
						this->m_pLog->Log("   " + printablePerson(pTheUsers[indexUserToUpdate], true));
						this->m_pLog->Log("Updated user: ");
						this->m_pLog->Log("   " + printablePerson(personToUpdate, true));
						this->m_pLog->Log("Person found after update: ");
						this->m_pLog->Log("   " + printablePerson(updatedPerson, true));
						this->m_testResult_Failure("UpdateUser()");
						if (this->m_bVerboseLogOutput)
						{
							this->m_pLog->Log("Here is a detailed list of the differences:");
							for (std::string line : vecDifferences)
							{
								this->m_pLog->Log("- " + line);
							}
						}
					}//if ( this->comparePeople()
				}
				else
				{
					this->m_pLog->Log("Error: ...but FindUser() didn't find updated user (returned false).");
				}//if ( this->m_pSongify->FindUserBySIN()
			}
			else
			{
				this->m_pLog->Log("Error: UpdateUser() returned false (should have returned true).");
			}//if ( this->m_pSongify->UpdateUser()


		}//if ( numUsers == 0 )

		// UpdateUser() INVALID user - check return

		this->m_pLog->Log("           ----");
		this->m_pLog->Log("Calling UpdateUser() with an invalid (missing) user: ");

		cPerson invalidUser = this->m_pThePeople->generateRandomPerson();
		errorString = "";
		if (!this->m_pSongify->UpdateUser(invalidUser, errorString))
		{
			this->m_pLog->Log("(CORRECT) UpdateUser() returned false for invalid user.");
			this->m_testResult_Success("UpdateUser()");
		}
		else
		{
			this->m_pLog->Log("Error: UpdateUser() returned true for invalid user (should be false)");
			this->m_testResult_Failure("UpdateUser()");

			cPerson foundPerson;
			if (this->m_pSongify->FindUser(invalidUser.getSongifyUniqueUserID(), foundPerson))
			{
				this->m_pLog->Log("Error: FindUser() returned true for users that isn't there (should be false)");
				this->m_testResult_Failure("FindUser()");
				// Did they load anything into the found person?
				// (they shouldn't have)
				cPerson blankPerson;
				if (!this->comparePeople(blankPerson, foundPerson, true))
				{
					this->m_pLog->Log("...and returned a person (shouldn't have):");
					this->m_pLog->Log("It returned this person: " + printablePerson(foundPerson, true));
				}
			}
		}//if ( ! this->m_pSongify->UpdateUser(invalidUser
	}// UpdateUser() 


	this->m_pLog->LogPrintLineBreak();

	{// DeleteUser() 

	// DeleteUser(), then GetUsers() to compare
		this->m_pLog->Log("Checking DeleteUser():");
		this->m_addTestResult("DeleteUser()");

		cPerson* pTheUsers = nullptr;
		unsigned int numUsers = 0;
		this->m_pLog->Log("Calling GetUsers()...");
		this->m_pSongify->GetUsers(pTheUsers, numUsers, true);
		if (numUsers == 0)
		{
			this->m_pLog->Log("Error: GetUsers() didn't return any users.");
			this->m_pLog->Log("(so we can't do the rest of this test)");
			this->m_testResult_Failure("GetUsers()");
		}
		else
		{
			this->m_pLog->appendText("GetUsers() returned ");
			this->m_pLog->appendNumber(numUsers);
			this->m_pLog->appendText(" users.");
			this->m_pLog->Log_OutputAppendedText();

			// Pick a user in the middle of the list (aka randomly)
			unsigned int indexUserToDelete = numUsers / 2;
			this->m_pLog->Log("Deleting this user: " + printablePerson(pTheUsers[indexUserToDelete], true));

			// Make a copy because we'll add them back later
			cPerson deletedUserCopy = pTheUsers[indexUserToDelete];

			std::string errorString;
			if (this->m_pSongify->DeleteUser(deletedUserCopy.getSongifyUniqueUserID(), errorString))
			{
				this->m_pLog->Log("(CORRECT) DeleteUser() returned true.");
				this->m_testResult_Success("DeleteUser()");

				cPerson foundPerson;
				if (!this->m_pSongify->FindUserBySongifyID(deletedUserCopy.getSongifyUniqueUserID(), foundPerson))
				{
					this->m_pLog->Log("(CORRECT) FindUser() didn't find the deleted user (returned false)");
					this->m_testResult_Success("DeleteUser()");
				}
				else
				{
					this->m_pLog->Log("Error: FindUser() returned true looking for a deleted user.");
					this->m_pLog->Log("..and returned this user: " + printablePerson(foundPerson, true));
				}
			}//if ( this->m_pSongify->DeleteUser(

			// Add the deleted person back
			this->m_pLog->Log("Adding the deleted person back:");
			if (this->m_pSongify->AddUser(deletedUserCopy, errorString))
			{
				this->m_pLog->Log("(CORRECT) Person added back OK (AddUser() returned true).");
				this->m_testResult_Success("AddUser()");
			}
			else
			{
				this->m_pLog->Log("Error: AddUser() returned false when adding back deleted user.");
				this->m_pLog->Log("(this might be because they were never deleted)");
				this->m_testResult_Failure("AddUser()");
			}

		}//if ( numUsers == 0 )

		// DeleteUser() INVALID user - check return

		this->m_pLog->Log("Now attempting to delete an invalid (missing) user:");

		cPerson invalidPerson = this->m_pThePeople->generateRandomPerson();
		std::string errorString;
		if (!this->m_pSongify->DeleteUser(invalidPerson.getSongifyUniqueUserID(), errorString))
		{
			this->m_pLog->Log("(CORRECT) DeleteUser() returned false trying to delete an invalid user (correct)");
			this->m_testResult_Success("DeleteUser()");
		}
		else
		{
			this->m_pLog->Log("Error: DeleteUser() returned true trying to delete an invalid user (should be false)");
			this->m_testResult_Failure("DeleteUser()");
		}


	}//// DeleteUser() 



	this->m_pLog->Log("RunBasicUserTests() completed.");

	return true;
}


bool cSongifyTesterExerciser::comparePeople(cPerson& A, cPerson& B, std::vector<std::string>& vecDifferences, bool bIgnoreID /*=false*/)
{
	// returns false of vecDifferences isn't zero
	vecDifferences.clear();	// Assume they are the same

	if (A.first != B.first)
	{
		std::stringstream ssError;
		ssError << "First names don't match: [" << A.first << "] != [" << B.first << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (A.middle != B.middle)
	{
		std::stringstream ssError;
		ssError << "Middle names don't match: [" << A.middle << "] != [" << B.middle << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (A.last != B.last)
	{
		std::stringstream ssError;
		ssError << "Middle names don't match: [" << A.middle << "] != [" << B.middle << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (A.gender != B.gender)
	{
		std::stringstream ssError;
		ssError << "Genders don't match: [" << A.getGenderAsString() << "] != [" << B.getGenderAsString() << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (A.age != B.age)
	{
		std::stringstream ssError;
		ssError << "Ages don't match: [" << A.age << "] != [" << B.age << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (A.streetNumber != B.streetNumber)
	{
		std::stringstream ssError;
		ssError << "Street Numbers don't match: [" << A.streetNumber << "] != [" << B.streetNumber << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (A.streetName != B.streetName)
	{
		std::stringstream ssError;
		ssError << "Street Names don't match: [" << A.streetName << "] != [" << B.streetName << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (A.streetType != B.streetType)
	{
		std::stringstream ssError;
		ssError << "Street Types don't match: [" << A.streetType << "] != [" << B.streetType << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (A.streetDirection != B.streetDirection)
	{
		std::stringstream ssError;
		ssError << "Street Directions don't match: [" << A.streetDirection << "] != [" << B.streetDirection << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (A.city != B.city)
	{
		std::stringstream ssError;
		ssError << "Cities don't match: [" << A.city << "] != [" << B.city << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (A.province != B.province)
	{
		std::stringstream ssError;
		ssError << "Provinces don't match: [" << A.province << "] != [" << B.province << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (!this->compareCharBuffers(A.postalCode, B.postalCode, 6))
	{
		std::stringstream ssError;
		ssError << "Postal Codes don't match: [" << printablePostalCode(A.postalCode) << "] != [" << printablePostalCode(B.postalCode) << "]";
		vecDifferences.push_back(ssError.str());
	}
	for (unsigned int phoneIndex = 0; phoneIndex < 4; phoneIndex++)
	{
		if (!this->compareCharBuffers(A.phoneNumbers[phoneIndex], B.phoneNumbers[phoneIndex], 11))
		{
			std::stringstream ssError;
			ssError << "Phone number [" << phoneIndex << "] don't match: ["
				<< printablePhoneNumber(A.phoneNumbers[phoneIndex]) << "] != ["
				<< printablePhoneNumber(B.phoneNumbers[phoneIndex]) << "]";
			vecDifferences.push_back(ssError.str());
		}
	}
	if (A.SIN != B.SIN)
	{
		std::stringstream ssError;
		ssError << "SINs don't match: [" << A.SIN << "] != [" << B.SIN << "]";
		vecDifferences.push_back(ssError.str());
	}
	if (!bIgnoreID)
	{
		if (A.getSongifyUniqueUserID() != B.getSongifyUniqueUserID())
		{
			std::stringstream ssError;
			ssError << "SongifyIDs don't match: [" << A.getSongifyUniqueUserID() << "] != [" << B.getSongifyUniqueUserID() << "]";
			vecDifferences.push_back(ssError.str());
		}
	}//if ( ! bIgnoreID )
	// If differences is empty, no errors
	return (vecDifferences.empty());
}

// Uses above, then ignores vecDifferences
bool cSongifyTesterExerciser::comparePeople(cPerson& A, cPerson& B, bool bIgnoreID /*=false*/)
{
	std::vector<std::string> vecDifferences;
	return this->comparePeople(A, B, vecDifferences, bIgnoreID);
}


void cSongifyTesterExerciser::sortPeopleBySIN(std::vector<cPerson>& vecPeople)
{
	class cAscendingBySIN
	{
	public:
		bool operator()(const cPerson& A, const cPerson& B)
		{
			return (A.SIN < B.SIN);
		}
	};

	std::sort(vecPeople.begin(), vecPeople.end(), cAscendingBySIN());

	return;
}

void cSongifyTesterExerciser::sortPeopleBySongifyID(std::vector<cPerson>& vecPeople)
{
	class cAscendingBySongifyID
	{
	public:
		bool operator()(cPerson& A, cPerson& B)
		{
			return (A.getSongifyUniqueUserID() < B.getSongifyUniqueUserID());
		}
	};

	std::sort(vecPeople.begin(), vecPeople.end(), cAscendingBySongifyID());
}

void cSongifyTesterExerciser::sortPeopleByName(std::vector<cPerson>& vecPeople, bool bAscending)
{
	class cSortedByName
	{
	public:
		cSortedByName(bool bAscending)
		{
			this->m_bAscending = bAscending;
		}
		bool operator()(cPerson& A, cPerson& B)
		{
			if (this->m_bAscending)
			{
				return this->bIsA_LessThan_B(A, B);
			}
			// else is Descending
			// ...reverse A and B
			return this->bIsA_LessThan_B(B, A);
		}
		bool bIsA_LessThan_B(cPerson& A, cPerson& B)
		{
			if (A.last < B.last)
			{
				return true;
			}
			if (A.last > B.last)
			{
				return false;
			}
			// Last names are the same
			if (A.first < B.first)
			{
				return true;
			}
			if (A.first > B.first)
			{
				return false;
			}
			// Last AND first names are the same
			if (A.middle < B.middle)
			{
				return true;
			}
			if (A.middle > B.middle)
			{
				return false;
			}
			// First AND last AND middle are the same
			// (so *not* less than)
			return false;
		}
	private:
		bool m_bAscending = true;
	};

	std::sort(vecPeople.begin(), vecPeople.end(), cSortedByName(bAscending));
	return;
}

// Picks values that aren't in the list of people
unsigned int cSongifyTesterExerciser::getInvalidSongifyID(void)
{
	cPerson testPerson;
	bool bKeepLooking = true;
	while (bKeepLooking)
	{
		testPerson = this->m_pThePeople->generateRandomPerson();
		// Assume this person isn't in list, so we can stop looking
		bKeepLooking = false;
		for (cPerson& p : this->m_vecSomePeople)
		{
			if (p.getSongifyUniqueUserID() == testPerson.getSongifyUniqueUserID())
			{
				// Found a match, so keep looking
				bKeepLooking = true;
			}
		}
	};
	// ID isn't in vector
	return testPerson.getSongifyUniqueUserID();
}

unsigned int cSongifyTesterExerciser::getInvalidSIN(void)
{
	cPerson testPerson;
	bool bKeepLooking = true;
	while (bKeepLooking)
	{
		testPerson = this->m_pThePeople->generateRandomPerson();
		// Assume this person isn't in list, so we can stop looking
		bKeepLooking = false;
		for (cPerson& p : this->m_vecSomePeople)
		{
			if (p.SIN == testPerson.SIN)
			{
				// Found a match, so keep looking
				bKeepLooking = true;
			}
		}
	};
	// ID isn't in vector
	return testPerson.SIN;
}


bool cSongifyTesterExerciser::compareUserLists(
	std::vector<cPerson>& vecPeopleReference, cPerson* pUserArray, unsigned int userArraySize,
	unsigned int& numUsersMissing, std::vector<std::string>& vecErrorDetails)
{
	// Will add to vecErrorDetails if errors

	if (pUserArray == nullptr)
	{
		vecErrorDetails.push_back("pUserArray == NULL, invalid array. Can't continue.");
		return false;
	}

	// Make a copy of the array...
	std::vector<cPerson> vecUsersCopy;
	for (unsigned int index = 0; index != userArraySize; index++)
	{
		vecUsersCopy.push_back(pUserArray[index]);
	}

	if (vecPeopleReference.size() != userArraySize)
	{
		vecErrorDetails.push_back("Wrong number of users");
		std::stringstream ssError;
		ssError << "Expected " << vecPeopleReference.size() << " users, but got "
			<< vecUsersCopy.size() << " users.";
		vecErrorDetails.push_back(ssError.str());
	}

	// All users present? 
	// Look for matching user in the vecUsersCopy, then delete.
	// If any are left, then there's "extra" people (or didn't exactly match)
	numUsersMissing = 0;
	for (unsigned int refIndex = 0; refIndex < vecPeopleReference.size(); refIndex++)
	{
		bool bFoundMatch = false;
		for (std::vector<cPerson>::iterator itCandidate = vecUsersCopy.begin();
			itCandidate != vecUsersCopy.end(); itCandidate++)
		{
			if (this->comparePeople(vecPeopleReference[refIndex], *itCandidate))
			{
				// It's a match
				vecUsersCopy.erase(itCandidate);
				bFoundMatch = true;
				// Early exit out of loop
				break;
			}
		}
		// Found a match?
		if (!bFoundMatch)
		{
			numUsersMissing++;
			vecErrorDetails.push_back("Did not find a match for user: " + printablePerson(vecPeopleReference[refIndex], true));
		}

	}//for ( unsigned int refIndex
	// If everyone got matched up, then vecUsersCopy should be empty
	if (!vecUsersCopy.empty())
	{
		vecErrorDetails.push_back("Extra people present in returned list of people:");
		for (std::vector<cPerson>::iterator itCandidate = vecUsersCopy.begin();
			itCandidate != vecUsersCopy.end(); itCandidate++)
		{
			vecErrorDetails.push_back(" - " + printablePerson(*itCandidate, true));
		}
	}


	// If errors, print out both lists for comparison
	if (!vecErrorDetails.empty())
	{
		vecErrorDetails.push_back("User lists DON'T match. Here's the two lists:");
		vecErrorDetails.push_back("Reference list:");
		for (unsigned int index = 0; index < vecPeopleReference.size(); index++)
		{
			std::stringstream ssInfo;
			ssInfo << "  [" << index << "] : " << printablePerson(vecPeopleReference[index], true);
			vecErrorDetails.push_back(ssInfo.str());
		}
		vecErrorDetails.push_back("\nReturned list:");
		for (unsigned int index = 0; index < vecUsersCopy.size(); index++)
		{
			std::stringstream ssInfo;
			ssInfo << "  [" << index << "] : " << printablePerson(vecUsersCopy[index], true);
			vecErrorDetails.push_back(ssInfo.str());
		}

	}//if ( ! vecErrorDetails.empty() )


	return (vecErrorDetails.empty());
}


bool cSongifyTesterExerciser::compareUserListsInOrder(
	std::vector<cPerson>& vecPeopleReference, cPerson* pUserArray, unsigned int userArraySize,
	std::vector<std::string>& vecErrorDetails)
{
	// Will add to vecErrorDetails if errors

	if (pUserArray == nullptr)
	{
		vecErrorDetails.push_back("pUserArray == NULL, invalid array. Can't continue.");
		return false;
	}

	if (vecPeopleReference.size() != userArraySize)
	{
		vecErrorDetails.push_back("Wrong number of users");
		std::stringstream ssError;
		ssError << "Expected " << vecPeopleReference.size() << " users, but got "
			<< userArraySize << " users.";
		vecErrorDetails.push_back(ssError.str());
	}

	// Testing in order, so compare corresponding indices
	for (unsigned int refIndex = 0;
		refIndex < userArraySize &&			// More expected users than returned users
		refIndex < vecPeopleReference.size();
		refIndex++)
	{
		if (!this->comparePeople(vecPeopleReference[refIndex], pUserArray[refIndex]))
		{
			std::stringstream ssError;
			ssError << "  Users @[" << refIndex << "] don't match.";
			vecErrorDetails.push_back(ssError.str());
		}

	}//for ( unsigned int refIndex


	// If errors, print out both lists for comparison
	if (!vecErrorDetails.empty())
	{
		vecErrorDetails.push_back("User lists DON'T match. Here's the two lists:");
		vecErrorDetails.push_back("Reference list:");
		for (unsigned int index = 0; index < vecPeopleReference.size(); index++)
		{
			std::stringstream ssInfo;
			ssInfo << "  [" << index << "] : " << printablePerson(vecPeopleReference[index], true);
			vecErrorDetails.push_back(ssInfo.str());
		}
		vecErrorDetails.push_back("\nReturned list:");
		for (unsigned int index = 0; index < userArraySize; index++)
		{
			std::stringstream ssInfo;
			ssInfo << "  [" << index << "] : " << printablePerson(pUserArray[index], true);
			vecErrorDetails.push_back(ssInfo.str());
		}

	}//if ( ! vecErrorDetails.empty() )


	return (vecErrorDetails.empty());
}