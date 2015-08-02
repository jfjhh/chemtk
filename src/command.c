/**
 * @file command.c
 * @author Alex Striff
 *
 * @brief Runs commands that produce numbers to calculate with.
 *
 * An example of a command's output would be the atomic mass of an element from
 * the periodic table.
 */

#include "command.h"

struct sc_command_tree  *sc_commands = NULL;
struct sc_input_table   *sc_inputs   = NULL;

static struct sc_command_tree *alloc_command_tree(void)
{
	struct sc_command_tree *tree, *children;

	// Get memory for the tree.
	if (!(tree = malloc(sizeof(struct sc_command_tree)))) {
		perror("alloc_sc_command_tree: could not allocate sc_command_tree.");
		return NULL;
	}

	// Get memory for the root tree's children.
	if (!(children = malloc(sizeof(struct sc_command_tree *) * CMD_CHILDREN))) {
		perror("alloc_sc_command_tree: could not allocate sc_command_tree.");
		free(tree);
	}

	tree->entry        = NULL;
	tree->children     = children;
	tree->num_children = 0;

	return tree;
}

static inline void free_command_tree(struct sc_command_tree *tree)
{
	// Assumes tree and tree->children are allocated memory. free(NULL) is OK.
	free(tree->children);
	free(tree);
}

static sc_command_func strtofunc(const char *str, void *handle)
{
	sc_command_func sym = NULL;

	dlerror();
	if (str)
		// ISO C disallows casting of `void *` to function pointer:
		// sym = dlsym(handle, "test_dlsym"); // Supported in `POSIX.1-2013`.
		// ... so use this messy casting, instead.
		*(void **) (&sym) = dlsym(handle, str);

	if (dlerror())
		sym = NULL;

	return sym;
}

static struct sc_command_input *get_sc_command_input(const char *str)
{
	int i;

	// str can be NULL, signifying an implied input from higher in the tree.
	if (!str)
		return NULL;

	for (i = 0; i < sc_inputs->count; i++)
		if (strncmp(str, sc_inputs->inputs[i].name, CMD_NAME_LEN) == 0)
			return (sc_inputs->inputs + i);

	return NULL;
}

static struct sc_command_tree *add_command_child(struct sc_command_entry *entry,
		struct sc_command_tree *tree)
{
	struct sc_command_tree *new;

	if (tree->num_children == CMD_CHILDREN) // Already full.
		return NULL;

	new = tree->children + (tree->num_children - 1);
	tree->num_children++;

	if (!(new = alloc_command_tree()))
		return NULL;
	new->entry = entry;

	return new;
}

static struct sc_command_entry *create_command_entry(char token,
		const char *name, const char *func, const char *input, void *handle)
{
	struct sc_command_entry *entry;

	// Get memory for the entry.
	if (!(entry = malloc(sizeof(struct sc_command_entry)))) {
		perror("parse_command_tree: "
				"could not allocate sc_command_entry.");
		return NULL;
	}

	// Add command fields to entry.
	entry->token = token;
	strncpy(entry->name, name, CMD_STRLEN);
	entry->input = get_sc_command_input(input); // May be NULL for children.
	entry->func = strtofunc(func, handle);

	return entry;
}

static inline void command_syntax_error(int line, const char *msg, ...)
{
	va_list ap;

	fprintf(stderr, "Syntax error while generating command tree:\n"
			"\t['" COMMAND_FILE "', line %03d]: \n\t\t", line);

	va_start(ap, msg);
	vfprintf(stderr, msg, ap);
	va_end(ap);

	fprintf(stderr, ".\n");
}

static void print_sc_commands(void)
{
	/**
	 * @todo Have @c print_sc_commands() pretty-print the entire command tree.
	 */
}

static int parse_command_tree(const char *data, void *handle)
{
	/**
	 * @todo: Finish writing @c parse_command_tree().
	 * @todo: Refactor @c parse_command_tree() into smaller chunks.
	 */
	int i, input_value, line, depth, status = 0;
	struct sc_command_tree  *cur_child;
	struct sc_command_tree  *subtree   = NULL;
	struct sc_command_tree  *parent    = NULL;
	struct sc_command_entry *entry     = NULL;
	struct sc_stack         *parents   = NULL;
	char token = '\0';
	char name[CMD_STRLEN]  = { '\0' },
		 func[CMD_STRLEN]  = { '\0' },
		 input[CMD_STRLEN] = { '\0' };

	if (!(parents = new_sc_stack())) {
		fprintf(stderr, "parse_command_tree: Could not allocate a sc_stack for "
				"command tree generation.\n");
		goto exit;
	}

	cur_child = sc_commands->children;
	while (cur_child++)
		;

	for (i = depth = 0, line = 1; data[i] != '\0'; i++) {
		if (data[i] == '#') {
			// Comment.
			while (data[i++] != '\n')
				;
			line++;
		} else if (data[i] == '\n') {
			// Empty line.
			line++;
		} else {
			// Valid line.
			if (sscanf(data + i, "%s, %d\n", name, &input_value) == 2) {
				// Command Input.
				sc_inputs->count++;
				if (sc_inputs->count > CMD_NAME_LEN) { // Too many inputs.
					command_syntax_error(line, "Too many command inputs");
					goto exit;
				}

				// Copy over the input fields to the global table.
				sc_inputs->inputs[sc_inputs->count - 1].length = input_value;
				strncpy(sc_inputs->inputs[sc_inputs->count - 1].name,
						input, CMD_STRLEN);

				line++;
			} else if (sscanf(data + i, "%c, %s, %s, %s\n", &token, name, func,
						input) == 4) {
				// Parent command (start of command family).

				// Add parent entry to root tree.
				entry = create_command_entry(token, name, func, input, handle);
				if(!entry
						|| !(subtree = add_command_child(entry, sc_commands))) {
					fprintf(stderr, "parse_command_tree: could not add parent"
							"command entry to sc_commands tree.\n");
					free(entry);
					goto exit;
				}

				push_sc_stack(&parents, entry);
				line++;

			} else if (data[i] == CMD_DEPTH_DELIM) {
				// Prepare thyself, intrepid reader...

				// Get depth.
				depth++;
				while (data[i++] == CMD_DEPTH_DELIM)
					depth++;

				// Check a parent for the child even exists.
				if (!(parent = peek_sc_stack(&parents))) {
					command_syntax_error(line, "Could not add child to tree, "
							"because it had no parent.");
					goto exit;
				}

				//
				// If the child is less deep than the parent at the top
				// of the stack, then pop parents until they are at the
				// same level.
				//
				// [ Diagram ]: CommandGroup Tree (Parent and Children)
				//              ---------------------------------------
				//
				//    root (depth 0). <- parent at bottom of stack.
				//    |
				//    :-- child (depth 1). <- parent at middle of stack.
				//    |   |
				//    |   :-- child (depth 2). <- parent at top of stack.
				//    |   |   |
				//    |   |   :-- NULL
				//    |   |
				//    |   :-- NULL
				//    |
				//    :-- child <- now (depth 1).
				//    |   |
				//    |   :-- NULL
				//    |
				//    :-- NULL
				//
				// === Result ===
				// The stack is popped from twice.
				//
				// Now, stack depth is 1, like the child's depth, and the
				// top of the stack is now 'root.'
				//
				while (depth < sc_stack_depth(parents)) {
					if (!(parent = pop_sc_stack(&parents))) {
						fprintf(stderr, "parse_command_tree: accidentally "
								"reached top of stack when popping up a "
								"depth level from the stack.\n");
						return 0;
					}
				}

				// Get child data.
				if (sscanf(data + i, "%c, %s, %s\n", &token, name, func) != 3) {
					command_syntax_error(line, "Malformed child, depth %d.",
							depth);
					goto exit;
				}

				// Add the child entry to the tree.
				if (!(subtree = add_command_child(create_command_entry(
									token, name, func, NULL, handle),
								parent))) {
					fprintf(stderr, "parse_command_tree: could not add "
							"child command entry to parent tree.\n");
					free(entry);
					goto exit;
				}

				// Push the child to the top of the stack, it now becomes a
				// parent for future iterations.
				if(!push_sc_stack(&parents, subtree)) {
					fprintf(stderr, "parse_command_tree: could not "
							"push subtree to parent stack.\n");
					free(entry);
					return 0;
				}

				//
				// Wow, it's finally over.
				// Note to self: Please make function less indented.
				//
				// Please.
				//     Please.
				//         Please.
				//
			} else {
				// An unknown syntax error occured.
				command_syntax_error(line, "Unknown syntax error, %d bytes into"
						" file, char was '%c'", i, data[i]);
				goto exit;
			}

			// Skip to the end of this line.
			while (data[i++] != '\n')
				;
		}
	}

	status = 1; // Everything was OK.

exit: // Falls through on success.
	delete_sc_stack(parents, NULL);
	return status;
}

static struct sc_command_entry *get_next_entry(struct sc_command_tree *tree,
		const char *line)
{
	struct sc_command_tree *child;
	struct sc_command_entry *result = NULL;

	// Check current node.
	if (tree->entry
			&& tree->entry->token == line[0]
			&& !isspace(line[0]))
		return tree->entry;
	else if ((child = tree->children)) // Check children.
		while (child)
			if ((result = get_next_entry(child, line + 1)))
				return result;
			else
				child++;

	return NULL;
}

static inline struct sc_command_entry *get_sc_command(const char *line)
{
	return get_next_entry(sc_commands, line);
}

static int test_sc_command_fun(const char *line, sc_token *token, FILE *logfile)
{
	fprintf(logfile, "test_sc_command_fun():\n");
	fprintf(logfile, "\tGot line: '%s'\n", line);
	fprintf(logfile, "\tGot token: '%p'\n", (void *) token);

	SCT_TYPE(token) = VALUE;
	fprintf(logfile, "\tSet token type to VALUE.\n");

	return 1;
}

int init_sc_commands(void)
{
	FILE *cmd_file = NULL;
	char *cmd_info = NULL;
	void *handle   = NULL;
	long length    = 0L;
	int  status    = 0;

	// Open self for access to function symbols.
	if (!(handle = dlopen(NULL, RTLD_LAZY))) {
		fprintf(stderr, "init_sc_commands: could not open opaque dl handle "
				"for self (dlfcn).");
		goto exit;
	}

	// Command file.
	if (!(cmd_file = fopen(COMMAND_FILE, "rb"))) {
		perror("init_sc_commands: '" COMMAND_FILE "' could not be opened");
		goto exit;
	}

	// Length of file.
	fseek(cmd_file, 0L, SEEK_END);
	length = ftell(cmd_file);
	rewind(cmd_file);

	// Get memory for file buffer.
	if (!(cmd_info = malloc(sizeof(char) * (length + 1)))) { // +1 for '\0'.
		perror("init_sc_commands: could not allocate buffer for file");
		goto exit;
	}
	cmd_info[length] = '\0'; // The 'length + 1'-th element.

	// Read the entire file into the buffer.
	if (fread(cmd_info, sizeof(char), length, cmd_file) != (size_t) length) {
		fprintf(stderr, "init_sc_commands: could not read entire file%s%s",
				(feof(cmd_file) ? " (reached EOF)" : ""),
				(ferror(cmd_file)
				 ? (clearerr(cmd_file),
					 " (error indicator was set; now cleared)")
				 : "" ));
		goto exit;
	}

	// Get memory for the root tree.
	if (!(sc_commands = malloc(sizeof(struct sc_command_tree)))) {
		perror("init_sc_commands: could not allocate sc_command_tree.");
		goto exit;
	}

	// Get memory for the input list.
	if (!(sc_inputs = malloc(sizeof(struct sc_input_table)))) {
		perror("init_sc_commands: could not allocate sc_input_table.");
		goto exit;
	}
	sc_inputs->count = 0;

	// Defaults for uninitialized tree.
	sc_commands->entry        = NULL;
	sc_commands->children     = NULL;
	sc_commands->num_children = 0;

	// Create the tree.
	if (!parse_command_tree(cmd_info, handle)) {
		fprintf(stderr, "init_sc_commands: error while creating parse tree.");
		goto exit;
	}

	// Close the self dl handle.
	if (dlclose(handle) != 0)
		fprintf(stderr, "init_sc_commands: error closing the self dl handle "
				"(meh, it'll probably be OK).");

	status = 1;

exit: // Success falls through.

	if (cmd_file)
		fclose(cmd_file);

	if (handle && dlclose(handle) != 0)
		fprintf(stderr, "init_sc_commands: error closing the self dl handle "
				"(meh, it'll probably be OK).");

	if (cmd_info)
		free(cmd_info);

	return status;
}

static void free_sc_command_tree(struct sc_command_tree *root)
{
	struct sc_command_tree *cur = root;
	int i = 0;

	if (root->entry)
		free(root->entry);

	while ((cur = (root->children) + (i++)))
		free_sc_command_tree(cur);

	free_command_tree(root);
}

void free_sc_commands(void)
{
	free_sc_command_tree(sc_commands);
	free(sc_inputs);
}

int is_sc_command(const char *line)
{
	return (get_sc_command(line) ? 1 : 0 );
}

int run_sc_command(const char *line, sc_token *out, FILE *logfile)
{
	/**
	 * @todo Write run_sc_command().
	 */
	struct sc_command_entry *cmd;

	if (!(cmd = get_sc_command(line))) {
		out->type = NONE;
		return 0;
	} else {
		(cmd->func)(line, out, logfile);
		return 1;
	}
}

int test_sc_command(FILE *logfile)
{
	int status = 0;
	sc_token *test_token = malloc(sizeof(sc_token));

	if (!test_token)
		fprintf(logfile, "Could not create a testing sc_token.\n");
	else if (!init_sc_commands())
		fprintf(logfile, "Could not init_sc_commands().\n");
	else if (!run_sc_command("test", test_token, logfile))
		fprintf(logfile, "Could not run_sc_command() [failed].\n");
	else if (SCT_TYPE(test_token) != VALUE) /* Set by the command as a check. */
		fprintf(logfile, "Test command executed wrong.\n");
	else
		status = 1;

	print_sc_commands();

	free_sc_commands();
	free(test_token);

	return status;
}

