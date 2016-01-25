#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <experimental/filesystem>
#include <openssl/md5.h>

using namespace std;
namespace fs = std::experimental::filesystem;

string file_md5(const char * filename);


int main(int argc, char *argv[])
{

	if (argc != 3)
	{
		printf("\nUsage: %s <directory name> <prefix>\n", argv[0]);
		return (-1);
	}

	string prefix(argv[2]);
	// http://en.cppreference.com/w/cpp/experimental/fs/recursive_directory_iterator
	for (auto& p : fs::recursive_directory_iterator(argv[1])) {
		if (!fs::is_directory(p)) {

			auto ftime = fs::last_write_time(p.path());
			std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime); // assuming system_clock

			cout << "'" << argv[2] << "', '" << p.path().parent_path() << "', '" << p.path().filename() << "', " << fs::file_size(p.path()) 
				<< ", '" << std::put_time(std::localtime(&cftime), "%Y-%m-%d %H:%M:%S") << "', '" << file_md5(p.path().string().c_str()) << "'" << endl;
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
