#include "directory.h"

char **get_files(const char *dir)
{
	char **files;
	DIR *directory;
	struct dirent *ent;
	int file = 0;

	/* fprintf(stderr, "Inside get_files(). NAME_MAX is %d.\n", NAME_MAX); */

	if ((files = (char **) malloc(sizeof(char *) * GETFILES_FILE_MAX)) == NULL) {
		/* fprintf(stderr, "Not enough memory for files buffer.\n"); */
		free(files);
		return NULL;
	}

	for (file = 0; file < GETFILES_FILE_MAX; file++)
		files[file] = NULL;

	if ((directory = opendir(dir)) != NULL) {
		/* fprintf(stderr, "opendir(dir) succeeded.\n"); */
		file = 0;
		while ((ent = readdir(directory)) != NULL) {
			/* fprintf(stderr, */
			/* 		"readdir(directory) to get struct dirent * succeeded.\n"); */

			/* Check that ent->d_name is not "." or "..". */
			/* Check that it is definitely not a directory. */
			/* I apologize for this if statment. */
			if ((!((ent->d_name[0] == '.')
							&& ((ent->d_name[1] == '.')
								|| (ent->d_name[1] == '\0'))))
					&& (ent->d_type != DT_DIR)) {

				files[file] = strndup(ent->d_name, NAME_MAX);
				/* fprintf(stderr, "succeeded in doing strndup();\n"); */
				/* fprintf(stderr, "inputted ent->d_name is `%s'.\n", ent->d_name); */
				file++;
			}

			/* fprintf(stderr, "=== DONE processing an ent (%d) ===\n", file); */
		}
		files[file] = NULL; /* End of file list. */
		closedir(directory);
		/* fprintf(stderr, "closedir(dir) succeeded.\n\n"); */

		return files;
	} else {
		/* fprintf(stderr, "opendir(dir) failed.\n"); */
		free_filelist(files);

		return NULL;
	}
}

char **get_files_strip(const char *dir)
{
	char **files;
	int file, i;

	if ((files = get_files(dir)) != NULL) {
		for (file = 0; files[file] != NULL; file++) {
			i = 0;

			/* Find the first "." in a file name */
			for (i = 0; (files[file][i] != '.') && (files[file][i] != '\0'); i++)
				;

			/* Zero to the end of the string. */
			while (files[file][i] != '\0')
				files[file][i++] = '\0';
		}
		return files;
	} else {
		free(files);
		return NULL;
	}
}

void free_filelist(char **files)
{
	int file;

	if (files)
		for (file = 0; file < GETFILES_FILE_MAX; file++)
			if (files[file])
				free(files[file]);

	free(files);
}

int test_directory(WINDOW *outwin)
{
	char **files;
	int file;

	/* TODO: write test_directory(). */
	wprintw(outwin, "Write test_directory().\n");

	wprintw(outwin, "=== Plain test ===\n");
	if (!(files = get_files(GETFILES_TESTDIR))) {
		goto fail;
	} else {
		for (file = 0; files[file] != NULL; file++) {
			wprintw(outwin, "Got file: `%s'.\n", files[file]);
			if (strlen(files[file]) != 5)
				goto fail;
		}
		if (file != 7)
			goto fail;
	}
	free_filelist(files);

	wprintw(outwin, "=== Strip test ===\n");
	if (!(files = get_files_strip(GETFILES_TESTDIR))) {
		goto fail;
	} else {
		for (file = 0; files[file] != NULL; file++) {
			wprintw(outwin, "Got file: `%s'.\n", files[file]);
			if (strlen(files[file]) != 1)
				goto fail;
		}
		if (file != 7)
			goto fail;
	}

	free_filelist(files);
	return 1;

fail:
	free_filelist(files);
	return 0;
}

