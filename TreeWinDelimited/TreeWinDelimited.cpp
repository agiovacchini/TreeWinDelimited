#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <regex>
#include <experimental/filesystem>
#include <openssl/md5.h>
#include <inttypes.h>

#define SEPARATOR '*'
#define STRING_DELIM '"'

using namespace std;
namespace fs = std::experimental::filesystem;

string file_md5(const char * filename);


int main(int argc, char *argv[])
{

	if ((argc < 4) || (argc > 6))
	{
		printf("\n%s - Copyright 2016 Konvergence srl\n", argv[0]);
		printf("\tUsage: %s <directory name> <prefix> <outFileName> [exclusionRegex] [sizeLimitBytes]\n", argv[0]);
		printf("\n\tThis product includes software developed by the OpenSSL Project\n");
		printf("\tfor use in the OpenSSL Toolkit(http://www.openssl.org/)\n");
		return (-1);
	}

	string prefix(argv[2]);
	ofstream outFile(argv[3]);
	string md5("");
	string sFilesToAvoid("");
	uintmax_t fileSize(0ULL);
	uintmax_t maxFileSizeToCalc(UINTMAX_MAX);

	if (argc == 5) {
		sFilesToAvoid = argv[4];
	} else {
		sFilesToAvoid = "";
	}

	if (argc == 6) {
		maxFileSizeToCalc = strtoumax(argv[5], nullptr, 10);
	}

	cout << "sFilesToAvoid: " << sFilesToAvoid << ", maxFileSizeToCalc: " << maxFileSizeToCalc << endl;
	std::regex filesToAvoid(sFilesToAvoid, std::regex_constants::ECMAScript | std::regex_constants::icase);

	// http://en.cppreference.com/w/cpp/experimental/fs/recursive_directory_iterator
	for (auto& p : fs::recursive_directory_iterator(argv[1])) {
		if (!fs::is_directory(p)) {
			fileSize = fs::file_size(p.path());

			if ( (argc == 4) 
					|| ((argc >= 5) && (!regex_search(p.path().string(), filesToAvoid)) && (fileSize < maxFileSizeToCalc))
				) {
				md5 = file_md5(p.path().string().c_str());
			} else {
				md5 = "NO_CALC";
			}

			auto ftime = fs::last_write_time(p.path());
			std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime); // assuming system_clock
				
			outFile << STRING_DELIM << argv[2] << STRING_DELIM 
				<< SEPARATOR << STRING_DELIM << p.path().parent_path() << STRING_DELIM 
				<< SEPARATOR << STRING_DELIM << p.path().filename() << STRING_DELIM 
				<< SEPARATOR << fileSize
				<< SEPARATOR << STRING_DELIM 
				<< std::put_time(std::localtime(&cftime), "%Y-%m-%d %H:%M:%S") << STRING_DELIM 
				<< SEPARATOR << STRING_DELIM << md5 << STRING_DELIM 
				<< endl;			
		}
	}
	outFile.close();

	return 0;
}


/**
 * http://stackoverflow.com/questions/10324611/how-to-calculate-the-md5-hash-of-a-large-file-in-c
 */

string file_md5(const char * pFilename) {
	unsigned char caMd5[MD5_DIGEST_LENGTH];
	stringstream sMd5;
	MD5_CTX mdContext;
	size_t bytes;
	FILE *inFile = fopen(pFilename, "rb");

	sMd5 << std::hex << std::setfill('0');

	unsigned char data[1024];

	if (inFile == NULL) {
		printf("%s can't be opened.\n", pFilename);
		return NULL;
	}

	MD5_Init(&mdContext);
	
	while ((bytes = fread(data, 1, 1024, inFile)) != 0) {
		MD5_Update(&mdContext, data, bytes);
	}
	
	MD5_Final(caMd5, &mdContext);
	
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		sMd5 << std::setw(2) << static_cast<unsigned>(caMd5[i]);
	}

	fclose(inFile);
	return sMd5.str();
}
