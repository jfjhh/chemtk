#ifndef ELEMENT_COMMANDS_H
#define ELEMENT_COMMANDS_H

#include "command_interface.h"
#include "../element.h"

int element_information(char *line, sc_token *out, FILE *logfile);
int element_verbose_information(char *line, sc_token *out, FILE *logfile);
int element_periodic_table(char *line, sc_token *out, FILE *logfile);

#endif /* ELEMENT_COMMANDS_H */

