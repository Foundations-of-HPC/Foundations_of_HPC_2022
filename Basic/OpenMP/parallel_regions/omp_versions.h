
#define _OMPv__STR(x) #x
#define _OMPv_STR(x) _OMPv__STR(x)


typedef struct omp_versions {
  char _openmp_value[20];
  int  major, minor; } omp_version_t;

#define _OPENMP_KNOWN_VERSIONS 8
omp_version_t omp_versions[_OPENMP_KNOWN_VERSIONS] = {
  {"199808", 1, 0},
  {"200203", 2, 0},
  {"200505", 2, 5},
  {"200805", 3, 0},
  {"201107", 3, 1},
  {"201307", 4, 0},
  {"201511", 4, 5},
  {"201611", 5, 0} };
