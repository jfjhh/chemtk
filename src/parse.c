/**
 * @file parse.c
 * @author Alex Striff
 *
 * @brief Parses the command (and input) specification YAML file, and generates
 * the global datastructures used to access the commands and their inputs.
 */

#include "parse.h"

extern void *sc_handle;

static int parse_inputs(yaml_parser_t *parser, yaml_event_t *event,
		FILE *logfile)
{
	int scalar_num = 0;

	fputs("\n\tParsing inputs!\n", logfile);

	if (!(sc_inputs = alloc_input_table()))
		return 1;

	yaml_event_delete(event); /* Remove the old "root" scalar event. */
	if (!yaml_parser_parse(parser, event)) {
		fprintf(stderr, "Parser error %d\n", parser->error);
		return 2;
	} else if (event->type != YAML_SEQUENCE_START_EVENT) {
		fputs("To parse inputs, parser must be on the sequence event "
				"(preferably tagged !!omap)\n", stderr);
		return 3;
	}
	yaml_event_delete(event);

	do {
		if (!yaml_parser_parse(parser, event)) {
			fprintf(stderr, "Parser error %d\n", parser->error);
			return 1;
		}

		switch (event->type) {
			case YAML_NO_EVENT:
				fputs("No event!\n", logfile);
				break;

			case YAML_MAPPING_START_EVENT: /* Start of an input. */
				fprintf(logfile,
						" *** Start Mapping *** (anchor: %s)(tag: %s)\n",
						event->data.mapping_start.anchor,
						event->data.mapping_start.tag);
				break;
			case YAML_MAPPING_END_EVENT: /* End of an input. */
				scalar_num = 0;
				sc_inputs->count++;
				fputs(" *** End Mapping *** \n", logfile);
				break;

			case YAML_SCALAR_EVENT:
				scalar_num++;

				if (scalar_num == 1) { /* The name of the symbol. */
					snprintf(cur_input()->name, CMD_INPUT_LEN, "%s",
							event->data.scalar.value);
				} else if (scalar_num == 2) { /* The value of the symbol. */
					cur_input()->length =
						atoin((char *) event->data.scalar.value,
								event->data.scalar.length);
				}

				fprintf(logfile, "Got scalar (value: %s)(anchor: %s)(tag: %s)"
						"(col: %d)\n",
						event->data.scalar.value,
						event->data.scalar.anchor,
						event->data.scalar.tag,
						(int) event->start_mark.column);
				break;

			case YAML_SEQUENCE_END_EVENT: /* End of all inputs. */
				fputs(" *** End Sequence *** \n", logfile);
				break;

			default: /* Some weird event that shouldn't be there. */
				yaml_event_delete(event);
				fputs("Wtf is this weird event doing in the sc_inputs?\n",
						stderr);
				return 4;
				break;
		}

		if (event->type != YAML_SEQUENCE_END_EVENT)
			yaml_event_delete(event);

	} while (event->type != YAML_SEQUENCE_END_EVENT);

	yaml_event_delete(event);
	return 0;
}

static int parse_commands(yaml_parser_t *parser, yaml_event_t *event,
		FILE *logfile)
{
	/* Command field variables: */
	char delimiter, name[CMD_NAME_LEN], func[CMD_NAME_LEN], input[CMD_NAME_LEN];
	struct sc_command_entry *cur;
	struct sc_command_tree  *parent;

	int scalar_num, status, attrib, len;

	cur    = NULL;
	parent = NULL;
	status = scalar_num = attrib = len = 0;
	delimiter  = '\0';

	memset(name,  '\0', CMD_NAME_LEN);
	memset(func,  '\0', CMD_NAME_LEN);
	memset(input, '\0', CMD_NAME_LEN);

	fputs("\n\tParsing commands!\n", logfile);

	/* Open commands library for access to function symbols. */
	if (!(sc_handle = dlopen(COMMAND_LIB,
					RTLD_NOW || RTLD_GLOBAL || RTLD_NODELETE))) {
		/* Open local for testing. */
		if (!(sc_handle = dlopen("./commands/.libs/libsccommands.so",
						RTLD_NOW || RTLD_GLOBAL || RTLD_NODELETE))) {
			/* Open self for access to function symbols. */
			if (!(sc_handle = dlopen(NULL, RTLD_LAZY))) {
				fprintf(stderr,
						"init_sc_commands: could not open opaque dl handle.\n");
				status = 1;
				goto exit;
			}
		}
	}

	/* Allocate root / global command tree. */
	if (!(sc_commands = alloc_command_tree())) {
		status = 2;
		goto exit;
	}
	fprintf(logfile, " *** sc_commands is <%p> ***\n", (void *) sc_commands);

	yaml_event_delete(event); /* Remove the old "root" scalar event. */
	if (!yaml_parser_parse(parser, event)) {
		fprintf(stderr, "Parser error %d\n", parser->error);
		status = 3;
		goto exit;
	} else if (event->type != YAML_SEQUENCE_START_EVENT) {
		fputs("To parse inputs, parser must be on the sequence event "
				"(preferably tagged !!omap)\n", stderr);
		status = 4;
		goto exit;
	}
	yaml_event_delete(event);

	do {
		if (!yaml_parser_parse(parser, event)) {
			fprintf(stderr, "Parser error %d\n", parser->error);
			status = 5;
			goto exit;
		}

		switch (event->type) {
			case YAML_SEQUENCE_END_EVENT: /* Command sequence exit condition. */
				goto exit;
				break;

			case YAML_NO_EVENT:
				fputs("No event!\n", logfile);
				break;

			case YAML_MAPPING_START_EVENT: /* Start of a command. */
				snprintf(name, CMD_NAME_LEN,
						"%s", event->data.mapping_start.anchor);

				fprintf(logfile, "Name is '%s'.\n", name);

				scalar_num = 1;

				fprintf(logfile,
						" *** Start Mapping *** (anchor: %s)(tag: %s)\n",
						event->data.mapping_start.anchor,
						event->data.mapping_start.tag);
				break;
			case YAML_MAPPING_END_EVENT: /* End of a command. */
				scalar_num = 0;
				if (!(cur = create_command_entry(delimiter, name,
								func, input, sc_handle))) {
					fputs("Could not create the command entry from data\n",
							stderr);
					status = 6;
					goto exit;
				} else if(cur->func == NULL && func[0] != '\0'
						&& parent != NULL) {
					/* Check for unfound symbols (not intentionally NULL). */
					fprintf(stderr, "Could not find symbol: '%s'.\n", func);
					status = 7;
					goto exit;
				}

				fprintf(logfile, "Created command entry <%p>.\n", (void *) cur);

				if (parent == NULL) { /* This is sc_commands' entry. */
					sc_commands->entry = cur;
					fputs("\tAdded root entry to sc_commands!\n", logfile);
				} else if (!add_command_entry(cur, parent)) {
					fprintf(stderr, "add_command_entry failed! "
							"cur: [%p], parent: [%p]\n",
							(void *) cur, (void *) parent);
					status = 8;
					goto exit;
				}

				fputs(" *** End Mapping *** \n\n", logfile);
				break;

			case YAML_SCALAR_EVENT:
				if (scalar_num == 1) { /* an "attribute" scalar. */
					if (SCALAR_CMP(PARENT_SCALAR,
								event->data.scalar) == 0) { /* Parent. */
						attrib = 1;
					} else if (SCALAR_CMP(DELIMITER_SCALAR,
								event->data.scalar) == 0) { /* Delimiter. */
						attrib = 2;
					} else if (SCALAR_CMP(FUNCTION_SCALAR,
								event->data.scalar) == 0) { /* Function. */
						attrib = 3;
					} else if (SCALAR_CMP(INPUT_SCALAR,
								event->data.scalar) == 0) { /* Input. */
						attrib = 4;
					} else {
						fputs("This attribute scalar doesn't belong in a "
								"sc_command! Go fix it!\n", stderr);
						status = 9;
						goto exit;
					}
					fprintf(logfile, "Got attribute scalar (#%02d).\n", attrib);

					scalar_num++;
				} else if (scalar_num == 2) { /* a "value" scalar. */
					fprintf(logfile,
							"Got value scalar (from attrib #%02d)(%s).\n",
							attrib, event->data.scalar.value);
					switch (attrib) {
						case 1: /* Parent *scalar* value: only if NULL. */
							if (event->data.scalar.value[0] == '\0') {
								parent = NULL;
							} else {
								fputs("Got a scalar instead of an anchor for "
										"the parent attribute's value!\n",
										stderr);
								status = 10;
							}
							break;
						case 2: /* Delimiter. */
							delimiter = (char) event->data.scalar.value[0];
							if (delimiter == '\\')
								delimiter = ESCAPE_CHAR((char)
										event->data.scalar.value[1]);
							fprintf(logfile,
									"Delimiter is '%c'.\n", delimiter);
							break;
						case 3: /* Function. `!!null` is allowed as the
								   function, to specify none. */
							if (event->data.scalar.value[0] != '\0') {
								snprintf(func, CMD_NAME_LEN,
										"%s", event->data.scalar.value);
							}
							fprintf(logfile,
									"Function (string) is '%s'.\n", func);
							break;
						case 4: /* Input. */
							snprintf(input, CMD_NAME_LEN,
									"%s", event->data.scalar.value);

							fprintf(logfile,
									"Input (string) is '%s'.\n", input);
							break;
						default:
							fputs("This shouldn't be here!\n", stderr);
							status = 11;
							goto exit;
							break;
					}
					scalar_num--;
				}

				fprintf(logfile, "Got scalar (value: %s)(anchor: %s)(tag: %s)"
						"(col: %d)\n",
						event->data.scalar.value,
						event->data.scalar.anchor,
						event->data.scalar.tag,
						(int) event->start_mark.column);
				break;

			case YAML_ALIAS_EVENT:
				if (attrib != 1) { /* Not value of a parent "attribute" scalar.
									  */
					fputs("Found YAML_ALIAS_EVENT (ref to command) but not in "
							"a parent attribute!\n", stderr);
				}
				parent =
					name_to_sc_parent_tree((char *) event->data.alias.anchor);
				fprintf(logfile, "parent is <%p>.\n", (void *) parent);

				scalar_num = 1; /* allow parsing of following "attributes." */
				break;

			default: /* Some weird event that shouldn't be there. */
				fprintf(stderr,
						"Wtf is this weird event doing in a command (%s)?\n",
						EVENT_TYPE(event->type));
				status = 12;
				goto exit;
				break;
		}

		if (event->type != YAML_SEQUENCE_END_EVENT)
			yaml_event_delete(event);

	} while (event->type != YAML_SEQUENCE_END_EVENT);

exit: /* Fallthrough on success. */
	if (status != 0)
		free(sc_commands);

	/* Intentionally leaving syms unclosed so later functions can use syms. */
	yaml_event_delete(event);

	return status;
}

int parse_command_file(const char *file, FILE *logfile)
{
	FILE *fh;
	yaml_parser_t parser;
	yaml_event_t  event;
	int status, parse_depth;

	status = parse_depth = 0;

	/* Initialize parser */
	if(!yaml_parser_initialize(&parser)) {
		fputs("Failed to initialize parser!\n", stderr);
		return 1;
	}

	/* Open the file */
	if (!(fh = fopen(file, "r"))) {
		fputs("Failed to open file to parse!\n", stderr);
		return 2;
	}

	/* Set input file */
	yaml_parser_set_input_file(&parser, fh);

	do {
		if (!yaml_parser_parse(&parser, &event)) {
			fprintf(stderr, "Parser error %d\n", parser.error);
			return 3;
		}

		switch (event.type) {
			case YAML_NO_EVENT:
				fputs("No event!\n", logfile);
				break;

			case YAML_STREAM_START_EVENT: /* Stream start / end */
				fprintf(logfile, "STREAM START (encoding: %s)\n",
						EVENT_ENCODING(event.data.stream_start.encoding));
				break;
			case YAML_STREAM_END_EVENT:
				fputs("STREAM END\n", logfile);
				break;

			case YAML_DOCUMENT_START_EVENT: /* Block delimeters */
				fputs(" *** Start Document *** \n", logfile);
				break;
			case YAML_DOCUMENT_END_EVENT:
				fputs(" *** End Document *** \n", logfile);
				break;
			case YAML_SEQUENCE_START_EVENT:
				fprintf(logfile,
						" *** Start Sequence *** (anchor: %s)(tag: %s)\n",
						event.data.sequence_start.anchor,
						event.data.sequence_start.tag);
				parse_depth++;
				break;
			case YAML_SEQUENCE_END_EVENT:
				fputs(" *** End Sequence *** \n", logfile);
				parse_depth--;
				break;
			case YAML_MAPPING_START_EVENT:
				fprintf(logfile,
						" *** Start Mapping *** (anchor: %s)(tag: %s)\n",
						event.data.mapping_start.anchor,
						event.data.mapping_start.tag);
				parse_depth++;
				break;
			case YAML_MAPPING_END_EVENT:
				fputs(" *** End Mapping *** \n", logfile);
				parse_depth--;
				break;

			case YAML_ALIAS_EVENT: /* Data */
				fprintf(logfile,
						"Got alias (anchor: %s)\n", event.data.alias.anchor);
				break;
			case YAML_SCALAR_EVENT:
				if (parse_depth == 1) { /* A "root" scalar. */
					if (strncmp((char *) event.data.scalar.value,
								ROOT_INPUTS_SCALAR,
								strlen(ROOT_INPUTS_SCALAR)) == 0) {
						/* Parse the inputs sequence that follows. */
						if ((status = parse_inputs(&parser, &event, logfile))
								!= 0) {
							fprintf(stderr,
									"parse_inputs failed with code %d!\n",
									status);
							goto exit;
						}
					} else if (strncmp((char *) event.data.scalar.value,
								ROOT_COMMANDS_SCALAR,
								strlen(ROOT_COMMANDS_SCALAR)) == 0) {
						/* Parse the command sequence that follows. */
						if ((status = parse_commands(&parser, &event, logfile))
								!= 0) {
							fprintf(stderr,
									"parse_commands failed with code %d!\n",
									status);
							goto exit;
						}
					}
				} else {
					fprintf(logfile,
							"Got scalar (value: %s)(anchor: %s)(tag: %s)"
							"(col: %d)\n",
							event.data.scalar.value,
							event.data.scalar.anchor,
							event.data.scalar.tag,
							(int) event.start_mark.column);
				}
				break;
		}

		if (event.type != YAML_STREAM_END_EVENT)
			yaml_event_delete(&event);

	} while (event.type != YAML_STREAM_END_EVENT);

exit: /* Cleanup */
	yaml_event_delete(&event);
	yaml_parser_delete(&parser);
	fclose(fh);
	return 0;
}

void free_command_info(void)
{
	free(sc_inputs);
	free_command_tree(sc_commands);
	if (sc_handle)
		dlclose(sc_handle);
}

int test_sc_parse(FILE *logfile)
{
	/**
	 * @test Tests if @c COMMAND_FILE can be parsed into a @c sc_command_tree.
	 */

	int i, status;
	status = parse_command_file(COMMAND_FILE, logfile);

	if (!sc_inputs) {
		fputs("sc_inputs should not be NULL!\n", logfile);
		return 0;
	}

	fprintf(logfile, "\n\tSC_INPUTS (%d)\n\n", sc_inputs->count);
	for (i = 0; i < sc_inputs->count; i++)
		fprintf(logfile, " [%02d]%10s => %d.\n",
				i, sc_inputs->inputs[i].name, sc_inputs->inputs[i].length);
	fputc('\n', logfile);

	if (!sc_commands) {
		fputs("sc_commands should not be NULL!\n", logfile);
		free(sc_inputs);
		return 0;
	}

	if (status == 0) /* Ensure commands not corrupted from bad test. */
		print_sc_commands(logfile);

	/* Leave alloc'd for test_sc_command(). */

	return (status == 0) ? 1 : 0;
}

