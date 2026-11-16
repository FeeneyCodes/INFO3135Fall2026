# Songify F26 tester

This is the terster helper tool I'll be using the mark your submissions. 

NOTE and DISCLAIMER: 
* It is **NOT** a complete grading tool, but is just a guide. 
* It *WILL* have bugs in it, almost certainly
* If you code has bugs or missing features, who knows what it'll do
* It tries a bunch of 'usual' cases (Add song, get song, etc.) then
   will try a bunch of 'edge' cases (like empty play lists, missing data, etc.)
   (there is where a lot of the bugs in this will be)
* This is meant as a helper for you to see if your code works somewhat as intended and to 
  catch errors earlier, particularly catastrophic (ex. crashing) and trivial ones 
  (ex. mixing last name with first name). If this is provided to you at all, it is as a ***courtesy***.
* This code is ***not*** guaranteed to be error free or even be fit for use.

   
How it's set up and how you use it:

- Copy your files into the Songify_F26_TesterExerciser/StudentSolutionFiles folder
  You'll see the original  cPerson, cSong, and cSpotify files there. 
  Overwrite them with yours. 
- Add any other files that you require to the appropraite folder (either the
  "StudentSolutionFiles" or the parent file, then use Add Item in the 
  "Songify_F26_TesterExerciser" project to add these the VS project. 
  NOTE: if you *DON'T* add the .cpp files to Visual Studio, it won't find them.
- "Songify_F26_TesterExerciser" depends on the other two projects, so when you 
  build it, it'll check if the other projects are built or not.
  If they haven't been built (or have been edited), then it will first built them, then 
  run a custom build step will run "CopyLibraryAndHeaderFiles.bat" which will copy
  the library (.lib), debug (.pdb), and data files, updating the project. 
  
Open the "Songify_F26_TesterExerciser_NO_Solution" solution file
(If you only open the project file, it won't set up the dependancies, etc.)


How to use (when marking a student):

1. Place all the student's source files into the "StudentSolutionFiles" folder. 
   (Overwrite whatever is there)
   Note: the tester assumes the cSongify class is called "cSongify".
   (not whatever the student might have called it)
   It also assumes that the student's files all reference themselves relatively, 
   i.e. that they can all be placed in the one folder and they are A-OK. 

2. In the Visual Studio project, add any ADDITIONAL student files to the "StudentSolutionFiles" filter.
   It already has links to the provided files (cPerson, cSongify, etc.).
   (If you add the same file again, VS will just assume they are the same and ignore the add)
   (i.e. it won't add them twice - if they are in the same folder that is...)
   If they have additional files, you'll need to add them here, too. 

If you want a specific file name, change the setStudentInfo() parameter:
eg. pTester->setStudentInfo("Bob Student SN:123456");

The files are generated based on this, but removes 'forbidden' characters.

You can put whatever text you want text you want in that parameter; it doesn't have to be like it is above.


3. Build and run. A log file based on the student's info will be generated. 
   Every time it's run, a new log and CSV summary file is generated.

   NOTE: If you have all the set up in the tree (above), then note there is 
         a "Pre-Build Event" (under project properties, "Build Events")
         that runs "CopyLibraryAndHeaderFiles.bat" which copies the original
         data AND project files into the StudentSolutionFiles directory!
		 If you don't have the projects set up like the tree (above) it 
		 doesn't find the files and doesn't do anything. 
		 If you want to disable this, change the "Build Events" or just
		 rename/delete that .bat file.
		 
		 This can be an issue if the student has added stuff to the original files.
   
The logfile will look like this: "2025-11-18 [12-14-22] (Tuesday, November 18, 2026) Bob Student SN-123456.log"


FYI: The "cSongify_EMPTY_IMPLEMENTATION.cpp" file is set as "Excluded from Build". 
It was only there so you could build *just* the tester (with no student files), if you wanted. 
(If you don't add this, you'll get a ton of linker errors, of course.)


 