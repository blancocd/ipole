
#include "par.h"

// sets default values for elements of params (if desired) and loads
// from par file 'fname'
void load_par (const char *fname, Params *params) {
 
  char line[256];
  FILE *fp = fopen(fname, "r");

  if (fp == NULL) {
    fprintf(stderr, "! unable to load parameter file '%s'. (%d: %s)\n", fname, errno, strerror(errno));
    exit(errno);
  }

  // set default values here
  params->counterjet = 0;
  params->tp_over_te = 3.;
  params->trat_small = 1.;
  params->trat_large = 40.;
  params->phicam = 0.;

  params->dump_skip = 1;
  params->restart_int = -1.;

  // modify parameters/types below
  while (fgets(line, 255, fp) != NULL) {

    if (line[0] == '#') continue;

    read_param(line, "counterjet", &(params->counterjet), TYPE_INT);

    read_param(line, "thetacam", &(params->thetacam), TYPE_DBL);
    read_param(line, "phicam", &(params->phicam), TYPE_DBL);
    read_param(line, "freqcgs", &(params->freqcgs), TYPE_DBL);
    read_param(line, "MBH", &(params->MBH), TYPE_DBL);
    read_param(line, "M_unit", &(params->M_unit), TYPE_DBL);
    read_param(line, "tp_over_te", &(params->tp_over_te), TYPE_DBL);
    read_param(line, "trat_small", &(params->trat_small), TYPE_DBL);
    read_param(line, "trat_large", &(params->trat_large), TYPE_DBL);

    read_param(line, "dump", (void *)(params->dump), TYPE_STR);
    read_param(line, "outfile", (void *)(params->outf), TYPE_STR);

    // for slow light
    read_param(line, "dump_min", &(params->dump_min), TYPE_INT);
    read_param(line, "dump_max", &(params->dump_max), TYPE_INT);
    read_param(line, "dump_skip", &(params->dump_skip), TYPE_INT);
    read_param(line, "img_cadence", &(params->img_cadence), TYPE_DBL);
    read_param(line, "restart_int", &(params->restart_int), TYPE_DBL);

  }

  fclose(fp);

  params->loaded = 1;

}

// allow command line arguments to overwrite parameters set in
// the par file. this should be called after load_par. 
void update_par(int argc, char *argv[], Params *params) {

  char *word, *value, *saveptr;

  for (int i=0; i<argc; ++i) {
    
    if (strlen(argv[i]) > 2 && argv[i][0] == '-' && argv[i][1] == '-') {

      

      word = strtok_r(argv[i]+2, "=", &saveptr);
      value = strtok_r(NULL, "=", &saveptr);

      set_by_word_val(word, value, "counterjet", &(params->counterjet), TYPE_INT);
      
      set_by_word_val(word, value, "thetacam", &(params->thetacam), TYPE_DBL);
      set_by_word_val(word, value, "phicam", &(params->phicam), TYPE_DBL);
      set_by_word_val(word, value, "freqcgs", &(params->freqcgs), TYPE_DBL);
      set_by_word_val(word, value, "MBH", &(params->MBH), TYPE_DBL);
      set_by_word_val(word, value, "M_unit", &(params->M_unit), TYPE_DBL);
      set_by_word_val(word, value, "tp_over_te", &(params->tp_over_te), TYPE_DBL);
      set_by_word_val(word, value, "trat_small", &(params->trat_small), TYPE_DBL);
      set_by_word_val(word, value, "trat_large", &(params->trat_large), TYPE_DBL);

      set_by_word_val(word, value, "dump", (void *)(params->dump), TYPE_STR);
      set_by_word_val(word, value, "outfile", (void *)(params->outf), TYPE_STR);

    }

  }

}

// loads value -> (type *)*val if line corresponds to (key,value)
void read_param (const char *line, const char *key, void *val, int type) {

  char word[256], value[256];

  sscanf(line, "%s %s", word, value);
  set_by_word_val(word, value, key, val, type);

}

// loads value -> (type *)*val if word==key
void set_by_word_val (const char *word, const char *value, const char *key, void *val, int type) {

  if (strcmp(word, key) == 0) {
    switch (type) {
    case TYPE_INT:
      sscanf(value, "%d", (int *)val);
      break;
    case TYPE_DBL:
      sscanf(value, "%lf", (double *)val);
      break;
    case TYPE_STR:
      sscanf(value, "%s", (char *)val);
      break;
    default:
      fprintf(stderr, "! attempt to load unknown type '%d'.\n", type);
    }
  }

}


