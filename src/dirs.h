#ifndef DIRS_H
#define DIRS_H

/* Define DATA_DIR to be either the installation dir (default), or to be the
 * local, source repository location */

#ifdef __has_include
#	if __has_include("data/data.h")
#		pragma message("(__has_include) repo-local data dir exists (using it)")
#		undef DATA_DIR
#		define DATA_DIR "./data"
#	endif
#else /* Just try to include it anyways, even if it may not exist. */
#	include "data/data.h"
#	ifdef DATA_H
#		pragma message("(no __has_include) repo-local data dir exists (using it)")
#		undef DATA_DIR
#		define DATA_DIR "./data"
#	endif
#endif

/* Directory locations for things like scientific constants, information, help
 * files, etc. */

#define CONST_FILE DATA_DIR "/constants.txt"

#define GETFILES_TESTDIR DATA_DIR "/get_files"
#define GETFILES_FILE_MAX 256
#define GETFILES_NAME_MAX (NAME_MAX + 1)
#define GETFILES_BUFLEN (GETFILES_FILE_MAX * GETFILES_NAME_MAX)

#define ELEMENT_DIR DATA_DIR "/elements"

#define ELEMENT_INFO_DIR ELEMENT_DIR "/info"
#define ELEMENT_INFO_DIR_LEN strlen(ELEMENT_INFO_DIR) + 1
#define ELEMENT_PTABLE_DIR ELEMENT_DIR "/ptable"
#define ELEMENT_PTABLE_DIR_LEN strlen(ELEMENT_PTABLE_DIR) + 1

#define PTABLE_FILE ELEMENT_DIR "/ptable.txt"

#define ELEMENT_PATH_LEN (ELEMENT_SYM_LEN + 1 + ELEMENT_PTABLE_DIR_LEN)

#define SCICALC_DOCFILE DATA_DIR "/scicalc.txt"

#define TEST_PAGER_FILE DATA_DIR "/pager.txt"

#endif /* DIRS_H */

