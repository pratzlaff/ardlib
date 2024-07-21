#include <ardlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "ardlib_common.h"

int main(int argc, const char* argv[])
{

  const char* grating;
  const char* obsfile;
  int order;
  unsigned long i, n;

  Ardlib_Grating_Eff_Type *a;
  float *e, *eff;

  if (argc != 7 && argc != 4) {
    fprintf(stderr, "Usage: %s [options] obsfile grating order [emin emax n]\n", argv[0]);
    exit(1);
  }

  obsfile = argv[1];
  grating = argv[2];
  order = atoi(argv[3]);


  if (-1 == ardlib_initialize ("CHANDRA", obsfile)) {
    fprintf (stderr, "Failed to initialize ardlib\n");
    exit (1);
  }

  /* generate energies */
  if (argc == 7) {
    n = get_energies_from_args(argv[4], argv[5], argv[6], &e);
  }
  else if (argc == 4) {
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

  if (!(eff = malloc(n * sizeof(float)))) {
    perror("malloc");
    fprintf(stderr, "could not allocate memory, exiting\n");
    exit(1);
  }

  a = ardlib_open_grating_eff(grating, order);
  if (!a) {
    fprintf(stderr, "ardlib_open_grating_eff(%s) failed, exiting\n", grating);
    exit(1);
  }

  if (ardlib_compute_grating_eff(a, e, n, eff)) {
    fprintf(stderr, "ardlib_compute_grating_eff() failed, exiting\n");
    exit(1);
  }

  for (i=0; i<n; ++i)
    printf("%f\t%f\n", e[i], eff[i]);

  if (ardlib_close_grating_eff(a)) {
    fprintf(stderr, "ardlib_close_grating_eff() failed, exiting\n");
    exit(1);
  }

  free(e);
  free(eff);

  return 0;
}
