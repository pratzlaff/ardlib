#include <ardlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "ardlib_common.h"

int main(int argc, const char* argv[])
{

  const char* obsfile;
  const char* detsubsys;
  double x, y;
  unsigned long i, n;

  Ardlib_Det_QE_Type *a;
  float *e, *qe;

  if (argc != 8 && argc != 5) {
    fprintf(stderr, "Usage: %s [options] obsfile detsubsys x y [emin emax n]\n", argv[0]);
    exit(1);
  }

  obsfile = argv[1];
  detsubsys = argv[2];
  x = atof(argv[3]);
  y = atof(argv[4]);

  if (-1 == ardlib_initialize ("CHANDRA", obsfile)) {
    fprintf (stderr, "Failed to initialize ardlib\n");
    exit (1);
  }

  /* generate energies */
  if (argc == 8) {
    n = get_energies_from_args(argv[5], argv[6], argv[7], &e);
  }
  else if(argc == 5) {
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

  if (!(qe = malloc(n * sizeof(float)))) {
    perror("malloc");
    fprintf(stderr, "could not allocate memory, exiting\n");
    exit(1);
  }

  a = ardlib_open_det_qe(detsubsys);
  if (!a) {
    fprintf(stderr, "ardlib_open_det_qe(%s) failed, exiting\n", detsubsys);
    exit(1);
  }

  if (ardlib_compute_det_qe(a, x, y, e, n, qe)) {
    fprintf(stderr, "ardlib_compute_det_qe() failed, exiting\n");
    exit(1);
  }

  for (i=0; i<n; ++i)
    printf("%f\t%f\n", e[i], qe[i]);

  if (ardlib_close_det_qe(a)) {
    fprintf(stderr, "ardlib_close_det_qe() failed, exiting\n");
    exit(1);
  }

  free(e);
  free(qe);

  return 0;
}
