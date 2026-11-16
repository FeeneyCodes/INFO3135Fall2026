@if not exist "..\Songify_F25_Generator\Songify_F25_Generator.vcxproj" goto no_gen_project
@echo Copying the souce files from the Songify_F25_Generator project...
xcopy "..\Songify_F25_Generator\cCallbackProgress.cpp" "cCallbackProgress.cpp" /y /z /v
xcopy "..\Songify_F25_Generator\cCallbackProgress.h" "cCallbackProgress.h" /y /z /v
xcopy "..\Songify_F25_Generator\cMersenneTwister.cpp" "cMersenneTwister.cpp" /y /z /v
xcopy "..\Songify_F25_Generator\cMersenneTwister.h" "cMersenneTwister.h" /y /z /v
xcopy "..\Songify_F25_Generator\cMusicGenerator.cpp" "cMusicGenerator.cpp" /y /z /v
xcopy "..\Songify_F25_Generator\cMusicGenerator.h" "cMusicGenerator.h" /y /z /v
xcopy "..\Songify_F25_Generator\cMusicGenerator_Imp.cpp" "cMusicGenerator_Imp.cpp" /y /z /v
xcopy "..\Songify_F25_Generator\cMusicGenerator_Imp.h" "cMusicGenerator_Imp.h" /y /z /v
xcopy "..\Songify_F25_Generator\cPerson.cpp" "cPerson.cpp" /y /z /v
xcopy "..\Songify_F25_Generator\cPerson.h" "cPerson.h" /y /z /v
xcopy "..\Songify_F25_Generator\cPersonGenerator.cpp" "cPersonGenerator.cpp" /y /z /v
xcopy "..\Songify_F25_Generator\cPersonGenerator.h" "cPersonGenerator.h" /y /z /v
xcopy "..\Songify_F25_Generator\cPersonGenerator_Imp.cpp" "cPersonGenerator_Imp.cpp" /y /z /v
xcopy "..\Songify_F25_Generator\cPersonGenerator_Imp.h" "cPersonGenerator_Imp.h" /y /z /v
xcopy "..\Songify_F25_Generator\cSong.h" "cSong.h" /y /z /v
xcopy "..\Songify_F25_Generator\sCanadianCityInfo.h" "sCanadianCityInfo.h" /y /z /v
xcopy "..\Songify_F25_Generator\sCanadianCityInfo_Data.h" "sCanadianCityInfo_Data.h" /y /z /v
@goto all_done
@echo Didn't find the Songify_F25_Generator_Library project, so nothing was copied.
:no_gen_project
:all_done
@echo All done.
@REM pause



