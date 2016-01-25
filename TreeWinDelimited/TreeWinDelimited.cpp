#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <experimental/filesystem>
#include <openssl/md5.h>

#define SEPARATOR '*'
#define STRING_DELIM '"'

using namespace std;
namespace fs = std::experimental::filesystem;

string file_md5(const char * filename);


int main(int argc, char *argv[])
{

	if (argc != 3)
	{
		printf("\n%s - Copyright 2016 Konvergence srl\n", argv[0]);
		printf("\tUsage: %s <directory name> <prefix>\n", argv[0]);
		return (-1);
	}

	string prefix(argv[2]);
	// http://en.cppreference.com/w/cpp/experimental/fs/recursive_directory_iterator
	for (auto& p : fs::recursive_directory_iterator(argv[1])) {
		if (!fs::is_directory(p)) {

			auto ftime = fs::last_write_time(p.path());
			std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime); // assuming system_clock

			cout << STRING_DELIM << argv[2] << STRING_DELIM << SEPARATOR << STRING_DELIM << p.path().parent_path() << STRING_DELIM << SEPARATOR << STRING_DELIM << p.path().filename() << STRING_DELIM << SEPARATOR << fs::file_size(p.path())
				<< SEPARATOR << STRING_DELIM << std::put_time(std::localtime(&cftime), "%Y-%m-%d %H:%M:%S") << STRING_DELIM << SEPARATOR << STRING_DELIM << file_md5(p.path().string().c_str()) << STRING_DELIM << endl;
		}
	}

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
