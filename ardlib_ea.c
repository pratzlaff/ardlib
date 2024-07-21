#include <ardlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "ardlib_common.h"

int main(int argc, const char* argv[])
{

  const char* obsfile;
  const char* mirror;
  double theta, phi;
  unsigned long i, n;

  Ardlib_Mirror_EA_Type *a;
  float *e, *ea;

  if (argc != 8 && argc != 5) {
    fprintf(stderr, "Usage: %s [options] obsfile mirror theta phi [emin emax n]\n", argv[0]);
    exit(1);
  }

  obsfile = argv[1];
  mirror = argv[2];
  theta = atof(argv[3]);
  phi = atof(argv[4]);

  if (-1 == ardlib_initialize ("CHANDRA", obsfile)) {
    fprintf (stderr, "Failed to initialize ardlib\n");
    exit (1);
  }

  /* generate energies */
  if (argc == 8) {
    n = get_energies_from_args(argv[5], argv[6], argv[7], &e);
  }
  else if (argc == 5) {
    n = get_energies_from_file("-", &e);
  }
  else {
    fprintf(stderr, "programmer error, exiting\n");
    exit(1);
  }
  
  if (!n) {
    fprintf(stderr, "number of energies is zero, exiting\n");
    exit(1);
  }

  if (!(ea = malloc(n * sizeof(float)))) {
    perror("malloc");
    fprintf(stderr, "could not allocate memory, exiting\n");
    exit(1);
  }

  a = ardlib_open_mirror_ea(mirror);
  if (!a) {
    fprintf(stderr, "ardlib_open_mirror_ea(%s) failed, exiting\n", mirror);
    exit(1);
  }

  if (ardlib_compute_mirror_ea(a, theta, phi, e, n, ea)) {
    fprintf(stderr, "ardlib_compute_mirror_ea() failed, exiting\n");
    exit(1);
  }

  for (i=0; i<n; ++i)
    printf("%f\t%f\n", e[i], ea[i]);

  if (ardlib_close_mirror_ea(a)) {
    fprintf(stderr, "ardlib_close_mirror_ea() failed\n");
    exit(1);
  }

  free(e);
  free(ea);

  return 0;
}
