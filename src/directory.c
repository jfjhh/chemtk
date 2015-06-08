/**
 * @file directory.c
 * @author Alex Striff
 *
 * @brief Provides a basic @c ls like utility.
 */

#include "directory.h"

char **get_files(const char *dir)
{
	char **files;
	DIR *directory;
	struct dirent *ent;
	int file = 0;

	if ((files = (char **) malloc(sizeof(char *) * GETFILES_FILE_MAX)) == NULL) {
		free(files);
		return NULL;
	}

	for (file = 0; file < GETFILES_FILE_MAX; file++)
		files[file] = NULL;

	if ((directory = opendir(dir)) != NULL) {
		file = 0;
		while ((ent = readdir(directory)) != NULL) {
			/* Check that ent->d_name is not "." or "..". */
			/* Check that it is definitely not a directory. */
			/* I apologize for this if statment. */
			if ((!((ent->d_name[0] == '.')
							&& ((ent->d_name[1] == '.')
								|| (ent->d_name[1] == '\0'))))
					&& (ent->d_type != DT_DIR)) {

				files[file] = strndup(ent->d_name, NAME_MAX);
				file++;
			}
		}
		files[file] = NULL; /* End of file list. */
		closedir(directory);

		return files;
	} else {
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

int test_directory(FILE *logfile)
{
	char **files;
	int file;

	/**
	 * @test Gets files from @c GETFILES_TESTDIR, expecting strings that are 5
	 * @c chars long, as each file name looks something like
	 * @code {'a', '.', 't', 'x', 't', '\0'} @endcode
	 * Test also fails if there are not 7 files found.
	 */
	fprintf(logfile, "=== Plain test ===\n");
	if (!(files = get_files(GETFILES_TESTDIR))) {
		goto fail;
	} else {
		for (file = 0; files[file] != NULL; file++) {
			fprintf(logfile, "Got file: `%s'.\n", files[file]);
			if (strlen(files[file]) != 5)
				goto fail;
		}
		if (file != 7)
			goto fail;
	}
	free_filelist(files);

	/**
	 * @test Gets files from @c GETFILES_TESTDIR, expecting strings that are 2
	 * @c chars long, as each file name looks something like
	 * @code {'a', '\0'} @endcode
	 * Test also fails if there are not 7 files found.
	 */
	fprintf(logfile, "=== Strip test ===\n");
	if (!(files = get_files_strip(GETFILES_TESTDIR))) {
		goto fail;
	} else {
		for (file = 0; files[file] != NULL; file++) {
			fprintf(logfile, "Got file: `%s'.\n", files[file]);
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

