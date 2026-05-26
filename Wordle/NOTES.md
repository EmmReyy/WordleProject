 The reason why all file handling is handled through objects is so that debugging and handling the directory is done
all at once

since the project is OOP and has headers and classes separate, a simple 

	ifstream filename("filename.txt")

will not work since the file should be relative to the build (.exe) instead, adter different tested methods, the filesystem library
was used to get the directory relative to the output build

	std::filesystem::path dataDir = std::filesystem::path("Assets");

gets the directory of the assets folder. the filenames are appended to that string (dataDir) and passsed as arguments

