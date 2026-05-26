 The reason why all file handling is handled through objects is so that debugging and handling the directory is done
all at once

since the project is OOP and has headers and classes separate, a simple 

	ifstream filename("filename.txt")

will not work since the file should be relative to the build (.exe) instead, adter different tested methods, the filesystem library
was used to get the directory relative to the output build

	std::filesystem::path exeDir = std::filesystem::path(argv[0]).parent_path();

this returns the directoy relative to the output exe. the filename is appended to the end of the directory (of exeDir)
to get the path of the file.

thus the necessary files needed by the program shuold alway be located in the output dir of the porject specfically:
	
	Wordle\x64\Release

the group tried to make the porject as portable as possible, however, short of tinkering with build configs and cmake lists,
some setup is still required

