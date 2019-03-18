#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <vector>
#include <cctype>
#include <cerrno>
#include <cstring>

using namespace std;

static const unsigned long base_address = 0xff180000;
static const int MIO_MIN = 0;
static const int MIO_MAX = 77;

#define ERR(msg_stream) do { cerr << msg_stream << ": " \
                << strerror(errno) << endl; } while (0) 

int intvec_max(vector<int> l)
{
  int v = l[0];
  for (int _v : l) {
    if (_v > v) {
      v = _v;
    }
  }
  return v;
}

class Main
{
  public:
    Main() {
      fd = -1;
      mem = nullptr;
    }
    ~Main() {
      if (mem != nullptr && mem != MAP_FAILED) {
        munmap(mem, memlen);
      }
      if (fd != -1) {
        close(fd);
      }
    }
    int operator()(vector<int> &l) {
      fd = open("/dev/mem", O_RDWR);
      if (fd == -1) {
        ERR("Failed to open /dev/mem");
        return -1;
      }
      int pgsz = getpagesize();
      unsigned long base_addr_off = base_address % pgsz;
      unsigned long true_base_addr = base_address - base_addr_off;
      memlen = max(intvec_max(l) * 4, pgsz) + base_addr_off;
      mem = reinterpret_cast<char*>(mmap(0, memlen, PROT_READ | PROT_WRITE, MAP_SHARED, fd, true_base_addr));
      if (mem == MAP_FAILED) {
        ERR("Failed to mmap /dev/mem");
        return -1;
      }

      for(int v : l) {
        volatile char *mio_addr = mem + base_addr_off + v * 4;
        cout << "Reseting MIO " << v << endl;
        *mio_addr = 0x00;
      }
      return 0;
    }
  private:
    int fd;
    char *mem;
    size_t memlen;
};

bool isnumber(const char* str)
{
  const char *p = str;
  while (*p) {
    if (!isdigit(*p)) {
      return false;
    }
    p++;
  }
  return true;
}

void usage()
{
  cerr << "./reset_mio MIO_NUMBER..." << endl;
  cerr << endl;
  cerr << "NOTE: run with root privilege" << endl;
}

int main(int argc, char const* argv[])
{
  vector<int> l {};
  for (int i = 1; i < argc; i++) {
    if (!isnumber(argv[i])) {
      cerr << "[Warning] An invalid MIO number :"
          << argv[i] << endl;
    } else {
      l.push_back(atoi(argv[i]));
    }
  }
  for (auto it = l.begin(); it != l.end(); ) {
    if (*it > MIO_MAX || *it < MIO_MIN ) {
      cerr << "[Warning] Too large MIO number :"
        << *it << endl;
      it = l.erase(it);
    } else {
      it++;
    }
  }
  if (l.empty()) {
    usage();
    return -1;
  }
  
  return Main()(l);
}
