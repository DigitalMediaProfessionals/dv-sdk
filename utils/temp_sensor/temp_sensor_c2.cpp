/*
 *  Copyright 2019 Digital Media Professionals Inc.

 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at

 *      http://www.apache.org/licenses/LICENSE-2.0

 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <cerrno>
#include <iostream>

int show_temp(volatile uint32_t *base_addr, unsigned interval = 1)
{
	volatile uint32_t *val_addr = base_addr + 1;
	while (1) {
		*base_addr = 1; // reset
		usleep(1);
		*base_addr = 0;
		usleep(1);

		*base_addr = 0x10; // clock enable
		sleep(interval);
		*base_addr = 0x00; // clock disable

		uint32_t val = *val_addr;
		if ((val >> 8) & 0x1) {
			uint32_t temp = (val & 0xff) - 128;
			std::cout << temp << " (C)" << std::endl;
		} else {
			std::cerr << "Failed to get temperature" << std::endl;
			return -1;
		}
	}

	return 0;
}

#define ERR(msg) { \
	cout << (msg) << " : " << strerror(errno) << endl;\
	ret = -1;\
	goto _err;\
}

int main(int argc, char const* argv[])
{
	using namespace std;
	int ret = 0;
	int fd = -1;
	char *mmem = reinterpret_cast<char*>(MAP_FAILED);
	static const unsigned long dev_addr = 0xff204004;

	long pgsz = sysconf(_SC_PAGE_SIZE);
	off_t off = dev_addr % pgsz;
	off_t base = dev_addr - off;
	unsigned interval = 1;

	if (argc > 1) {
		interval = std::atoi(argv[1]);
		if (interval <= 0) {
			ERR("An invalid argument is given");
		} else {
			cout << "Interval : " << interval << endl;
		}
	}
	if (pgsz == -1) {
		ERR("Failed to get page size");
	}
	if ((fd = open("/dev/mem", O_RDWR)) == -1) {
		ERR("Failed to open /dev/mem");
	}
	if ((mmem = reinterpret_cast<char*>(mmap(0, pgsz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, base))) == MAP_FAILED) {
		ERR("Failed to mmap /dev/mem");
	}

	ret = show_temp(reinterpret_cast<volatile uint32_t*>(mmem + off), interval);

_err:
	if (mmem != MAP_FAILED) {
		munmap(mmem, pgsz);
	}
	if (fd != -1) {
		close(fd);
	}

	return ret;
}
