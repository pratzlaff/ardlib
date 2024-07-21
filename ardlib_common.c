#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>

unsigned long get_energies_from_args(const char* eminstr,
				     const char* emaxstr,
				     const char* nstr,
				     float** b)
{
  

  float emin, emax;
  float logemin, logemax, logestep, loge;
  unsigned long i, n;
  float *e;

  emin = atof(eminstr);
  emax = atof(emaxstr);
  n = atol(nstr);

  if (emin <= 0 || emax <= 0) {
    fprintf(stderr, "emin=%f, emax=%f, exiting\n", emin, emax);
    exit(1);
  }

  if (n > 1e6) {
    fprintf(stderr, "n = %lu is too large, exiting\n", n);
    exit(1);
  }

  if (! (e = malloc(n * sizeof(float))) ) {
    fprintf(stderr, "could not allocate memory, exiting\n");
    exit(1);
  }

  *b = e;

  logemin = log(emin);
  logemax = log(emax);
  logestep = (logemax - logemin) / n;
  loge = logemin;

  for (i=0; i<n; ++i, loge+=logestep)
    e[i] = exp(loge);

  return n;
}

unsigned long get_energies_from_file(const char* filename, float** b) {

  float *buf;
  float f;
  unsigned long nbuf = 1024;
  unsigned long n = 0;
  FILE* file;

  if (!strcmp("-", filename)) {
    file = stdin;
  }
  else if (! (file = fopen(filename, "r"))) {
    perror("fopen");
    fprintf(stderr, "could not open %s for reading, exiting\n", filename);
    exit(1);
  }

  if (! (buf = malloc(nbuf * sizeof(f)))) {
    perror("malloc");
    fprintf(stderr, "could not allocate memory, exiting\n");
    exit(1);
  }

  *b = buf;

  while (fscanf(file, "%f", &f) == 1) {
    ++n;

    /* allocate more space */
    if (n > nbuf) {

      if ( ! (realloc(buf, sizeof(float) * (nbuf + 2*nbuf))) ) {
	perror("realloc");
	fprintf(stderr, "could not realloc memory, exiting\n");
	exit(1);
      }

      nbuf += 2*nbuf;

    }

    buf[n-1] = f;
  }

  if (! feof(file))
    fprintf(stderr, "scanning stopped prematurely due to non-float input\n");

  if (fclose(file)) {
    perror("fclose");
    fprintf(stderr, "error closing file\n");
  }

  return n;
  
}

