/**
 * @file command.c
 * @author Alex Striff
 *
 * @brief Runs commands that produce numbers to calculate with.
 *
 * An example of a command's output would be the atomic mass of an element from
 * the periodic table (with functionality coming from element.c).
 */

#include "command.h"

struct sc_command_tree *sc_commands;

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

static struct sc_command_tree *name_to_parent_tree(const char *name,
		struct sc_command_tree *tree)
{
	int i;
	struct sc_command_tree *result = NULL;

	if (tree == NULL)
		tree = sc_commands;

	if (tree && tree->entry) {
		// Check current node.
		if (strncmp(tree->entry->name, name, CMD_NAME_LEN) == 0)
			return tree;
		else if (tree->num_children > 0) // Check children.
			for (i = 0; i < tree->num_children; i++)
				if ((result = name_to_parent_tree(name, tree->children[i])))
					return result;
	}

	return NULL;
}

struct sc_command_tree *name_to_sc_parent_tree(const char *name)
{
	return name_to_parent_tree(name, sc_commands);
}

static void print_sc_command_entry(FILE *file, struct sc_command_entry *entry)
{
	if (entry && entry->delimiter && entry->name) {
		fprintf(file, "\u203a %s (%c)", entry->name, entry->delimiter);
		if (entry->input)
			fprintf(file, "\t%s%8s : %-d\n",
					(strlen(entry->name) > 8)
					? ((strlen(entry->name) > 16) ? "" : "\t")
					: "\t\t",
					entry->input->name, entry->input->length);
		else
			fputs("None\n", file);
	} else if (entry->delimiter || entry->name) {
		fputs("  Root\n", file);
	} else {
		fputs("[ NULL Entry ]\n", file);
	}
}

static void print_commands(FILE *file, struct sc_command_tree *tree, int depth)
{
	int i, c;

	if (depth == 0)
		fprintf(file, "[ Command Tree %p ]\n", (void *) tree);

	if (tree) {
		print_sc_command_entry(file, tree->entry);
		if (tree->num_children > 0) {
			for (c = 0; c < tree->num_children; c++) {
				// Extra vertical space.
				// for (i = 0; i < depth; i++)
				// 	fputs("  \u2502   ", file);
				// fputs("  \u2502\n", file);

				for (i = 0; i <= depth; i++) {
					if (i == depth) {
						if (depth == 0)
							fputs("  \u251c\u2500\u2500\u2500", file);
						else
							fprintf(file, "  %s\u2500\u2500\u2500",
									(c < tree->num_children - 1)
									? "\u251c" : "\u2514");
					} else {
						fputs("  \u2502   ", file);
					}
				}
				print_commands(file, tree->children[c], depth + 1);
			}

			if (depth == 0)
				fputs("  \u2514\u00a4", file);
		}
	}
}

void print_sc_commands(FILE *file)
{
	print_commands(file, sc_commands, 0);
}

static struct sc_command_entry *get_next_entry(struct sc_command_tree *tree,
		char *line)
{
	int i;
	char *next;
	struct sc_command_entry *result = NULL;

	if (line[0] == '\0')
		return NULL;

	if (tree && tree->entry) {
		fprintf(stderr, "get_next_entry: On line char '%c'.\n", line[0]);
		print_sc_command_entry(stderr, tree->entry);


		// Check current node.
		if (isalnum(line[0]) && tree->entry->delimiter == line[0]) {
			return tree->entry;
		} else if (tree->num_children > 0) { // Check children.
			// Check if this is the root tree (so we don't compare to '\0').
			next = (tree->entry->delimiter == '\0') ? line : line + 1;

			for (i = 0; i < tree->num_children; i++)
				if ((result = get_next_entry(tree->children[i], next)))
					return result;
		}
	}

	return NULL;
}

static inline struct sc_command_entry *get_sc_command(char *line)
{
	return get_next_entry(sc_commands, line);
}

struct sc_command_tree *alloc_command_tree(void) {
	struct sc_command_tree *tree;

	// Get memory for the tree.
	if (!(tree = malloc(sizeof(struct sc_command_tree)))) {
		perror("alloc_command_tree: could not allocate sc_command_tree.");
		return NULL;
	}

	// Initialize children to NULL.
	memset(tree->children, '\0',
			sizeof(struct sc_command_tree *) * CMD_CHILDREN);

	tree->entry        = NULL;
	tree->num_children = 0;
	tree->depth        = 0;

	return tree;
}

void free_command_tree(struct sc_command_tree *root)
{
	struct sc_command_tree *cur = root;
	int i = 0;

	if (root->entry)
		free(root->entry);

	while ((cur = root->children[i++]))
		free_command_tree(cur);

	free(root);
}

struct sc_command_entry *create_command_entry(char delimiter,
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
	strncpy(entry->name, name, CMD_NAME_LEN);
	entry->delimiter = delimiter;
	entry->input = name_to_input(input);
	entry->func  = strtofunc(func, handle);

	return entry;
}

struct sc_command_tree *add_command_entry(struct sc_command_entry *entry,
		struct sc_command_tree *tree)
{
	struct sc_command_tree *new;

	if (!tree || !entry
			|| tree->num_children == CMD_CHILDREN
			|| !(new = alloc_command_tree()))
		return NULL;

	new->entry = entry;
	new->depth = tree->depth + 1;

	fprintf(stderr,
			"add_command_entry: Adding entry to tree with %d children:\n"
			"\tParent Tree:\t<%p> (depth %d),\n"
			"\tEntry Tree:\t<%p> (depth %d).\n",
			tree->num_children,
			(void *) tree, tree->depth,
			(void *) new, new->depth);

	tree->num_children++;

	// tree->num_children is a count, not an offset, so subtract one to do
	// proper pointer indexing on tree->children.
	tree->children[tree->num_children - 1] = new;

	return new;
}

int is_sc_command(char *line)
{
	return get_sc_command(line) ? 1 : 0;
}

int run_sc_command(char *line, sc_token *out, FILE *logfile)
{
	struct sc_command_entry *cmd;

	if (!(cmd = get_sc_command(line))) {
		out->type = NONE;
	} else {
		if (cmd->func)
			return (cmd->func)(line, out, logfile);
		else
			fputs("No action (sc_command_func was NULL).\n", logfile);
	}

	return 0;
}

int test_sc_command(FILE *logfile)
{

	int status = 0;
	sc_token *test_token;

	// Global commands initialized by test_sc_parse().

	if (!(test_token = malloc(sizeof(sc_token))))
		fputs("Could not create a testing sc_token.\n", logfile);
	else if (!run_sc_command("T", test_token, logfile))
		fputs("Could not run_sc_command().\n", logfile);
	else if (SCT_TYPE(test_token) != VALUE) // Set by the command as a check.
		fputs("Test command executed wrong.\n", logfile);
	else
		status = 1;

	print_sc_token(test_token, logfile);

	free(test_token);
	/* Command tree freed in subsequent test. */

	return status;
}

