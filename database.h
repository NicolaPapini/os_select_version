#include <stdbool.h>
#include "data_structures/phone_directory.h"
#define MAXLINE 128
#define PHONE_DIRECTORY_FILE "/Users/nicola/CLionProjects/os_select_version/cmake-build-debug/phone_directory.txt"

bool serialize(const PhoneDirectory *phone_dir, const char *filename);
void deserialize(PhoneDirectory *phone_dir, const char *filename);

