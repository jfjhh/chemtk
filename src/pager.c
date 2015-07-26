/**
 * @file pager.c
 * @author Alex Striff
 *
 * @brief Pages files.
 */

#include "pager.h"

int page_file_stream(FILE *path)
{
	struct rlimit  self_rlim;
	char           *filename, *page_path;
	int            fd, fd_charlen, fd_totlen, status;
	unsigned long  hard_fd_lim; /* should fit rlim_t */

	/*
	 * In order to figure out the max size of the str used to get the filename
	 * of path, the following string must be generated:
	 * '/proc/self/fd/FOO'

	 * We must know the length of FOO, and thus the hard limit of file descriptors
	 * for this process. This program probably won't be ran as superuser, and even
	 * if the hard limit changed, it would be accounted for.
	 *
	 * This is done using getrlimit(2).
	 */

	if (getrlimit(RLIMIT_NOFILE, &self_rlim) != 0) {
		perror("page_file_stream: failed to get max. process file descriptors");
		return 0;
	}

	fd          =          fileno(path); /* Determine fd from FILE *. */
	hard_fd_lim =          self_rlim.rlim_max - (rlim_t) 1;
	fd_charlen  = (int)    log10l(hard_fd_lim) + 1;
	fd_totlen   =          strlen(SC_SELFFD_DIR) + fd_charlen + 1;
	filename    = malloc(sizeof(char) * fd_totlen);
	page_path   = malloc(sizeof(char) * PATH_MAX);

	if (!filename || !page_path) {
		return 0;
	} else {
		snprintf(filename, fd_totlen, "%s%d", SC_SELFFD_DIR, fd);
	}

	if (readlink(filename, page_path, PATH_MAX) == -1) {
		free(filename);
		free(page_path);
		perror("page_file_stream: readlink to get file name failed");
		return 0;
	}

	fprintf(stderr, "DEBUG: Got filename '%s'.\n", page_path); /* DEBUG */

	free(filename);
	status = page_file(page_path);
	free(page_path);

	return status;
}

int page_file_exit(const char *path, enum page_exit auto_exit)
{
	int status;
	pid_t child_pid;

	/* Test file can be read. */
	if (access(path, R_OK) == -1) {
		perror("page_file: cannot read from file");
		return 0;
	}

	if ((child_pid = fork()) >= 0) { /* fork() successful. */
		if (child_pid == 0) { /* Child process. */
			if (auto_exit)
				return execlp(SC_PAGER, SC_PAGER, "-E", path, (char *) NULL);
			else
				return execlp(SC_PAGER, SC_PAGER, path, (char *) NULL);
		} else { /* Parent process. */
			wait(&status);
			if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
				return 1;
			else
				return 0;
		}
	} else { /* fork() failed. */
		perror("page_file: cannot fork for pager");
		exit(0);
	}
}

int test_pager(FILE *logfile)
{
	/**
	 * @test
	 * Tests to see if @c TEST_PAGER_FILE can be paged.
	 */
	fprintf(logfile, "Testing page_file with '" TEST_PAGER_FILE "'.\n");
	return page_auto(TEST_PAGER_FILE);
}

