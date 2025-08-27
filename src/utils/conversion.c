#include "conversion.h"

int byteToKB(int bytes) {
    return bytes / 1024;
}
int byteToMB(int bytes) {
    return (bytes / 1024) / 1024;
}
int kBToMB(int kB) {
    return kB / 1024;
}

int byteToKB_64u(uint64_t bytes) {
    return udiv64(bytes, 1024);
}
int byteToMB_64u(uint64_t bytes) {
    return udiv64(udiv64(bytes, 1024), 1024);
}
int kBToMB_64u(uint64_t kB) {
    return udiv64(kB, 1024);
}
