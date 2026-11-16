@if not exist "..\Songify_F26_Generator_Library\Songify_F26_Generator_Library.vcxproj" goto no_lib_project
@echo Copying the souce files from the Songify_F26_Generator project...
xcopy "..\Songify_F26_Generator_Library\cMusicGenerator.h" "*.*" /y /z /v
xcopy "..\Songify_F26_Generator_Library\cPersonGenerator.h" "*.*" /y /z /v
xcopy "..\Songify_F26_Generator_Library\cCallbackProgress.h" "*.*" /y /z /v
REM xcopy "..\Songify_F26_Generator_Library\cPerson.cpp" "StudentSolutionFiles\*.*" /y /z /v
REM xcopy "..\Songify_F26_Generator_Library\cPerson.h" "StudentSolutionFiles\*.*" /y /z /v
REM xcopy "..\Songify_F26_Generator_Library\cSong.h" "StudentSolutionFiles\*.*" /y /z /v
REM xcopy "..\Songify_F26_StarterCode\cSongify.h" "StudentSolutionFiles\*.*" /y /z /v
xcopy "..\x64\Debug\Songify_F26_Generator_Library.lib" "lib\Debug_x64\*.*" /y /z /v
xcopy "..\x64\Debug\Songify_F26_Generator_Library.pdb" "lib\Debug_x64\*.*" /y /z /v
tar -a -c -f "lib\Debug_x64\Debug_x64-Songify_F26_Generator_Library.lib.zip" "lib\Debug_x64\Songify_F26_Generator_Library.lib" "lib\Debug_x64\Songify_F26_Generator_Library.pdb" 
xcopy "..\x64\Release\Songify_F26_Generator_Library.lib" "lib\Release_x64\*.*" /y /z /v
xcopy "..\x64\Release\Songify_F26_Generator_Library.pdb" "lib\Release_x64\*.*" /y /z /v
tar -a -c -f "lib\Release_x64\Release_x64-Songify_F26_Generator_Library.lib.zip" "lib\Release_x64\Songify_F26_Generator_Library.lib" "lib\Release_x64\Songify_F26_Generator_Library.pdb" 
@goto copy_data_files
:no_lib_project
@echo Didn't find the Songify_F26_Generator_Library project, so nothing was copied.
@REM now the data files
:copy_data_files
@if not exist "..\Songify_F26_Generator\Data\Street Names (San Francisco)\Street_Names.csv" goto no_data_files
xcopy "..\Songify_F26_Generator\Data\*.*" "Data\*.*" /y /z /s /e /g /h /j
@goto all_done
:no_data_files
@echo Didn't find the Songify_F26_Generator_Library project data, so nothing was copied.
@goto all_done
:all_done
@echo All done.
pause



