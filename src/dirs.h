/**
 * @file dirs.h
 * @author Alex Striff
 *
 * @brief Various macros for the project.
 *
 * Defines the directory for project data files to be stored (@c DATA_DIR),
 * macros for various buffer sizes, and various directory locations for use
 * within the project, relative to the data directory.
 */

#ifndef DIRS_H
#define DIRS_H

#ifndef DATA_DIR
#error "DATA_DIR UNDEFINED!"
/**
 * The directory for all associated project test files and data to be
 * decompressed to upon installation.
 *
 * @c DATA_DIR is defined to be either within the installation directory, which
 * is the default, or within the source repository.
 *
 * @note
 * The initial, installation directory value for @c DATA_DIR is defined at
 * compilation time to be AutoMake's @c pkgdatadir.
 *
 * @see
 * https://www.gnu.org/savannah-checkouts/gnu/autoconf/manual/autoconf-2.69/html_node/Installation-Directory-Variables.html#Installation-Directory-Variables
 * @see
 * https://www.gnu.org/software/automake/manual/html_node/Uniform.html#Uniform
 */
#define DATA_DIR
#endif /* DATA_DIR */

/*******************************************************************************
 * Macros for buffer sizes for various data structures and printing widths.
 ******************************************************************************/

/**
 * Length of ELEMENT_INFO_DIR, including terminating @c NULL byte.
 */
#define ELEMENT_INFO_DIR_LEN    strlen(ELEMENT_INFO_DIR) +  1

/**
 * Length of an element path, which is ELEMENT_SYM_LEN combined with
 * ELEMENT_PTABLE_DIR_LEN.
 */
#define ELEMENT_PATH_LEN        (ELEMENT_SYM_LEN +  1 +  ELEMENT_PTABLE_DIR_LEN)

/**
 * Length of ELEMENT_PTABLE_DIR, including the terminating @c NULL byte.
 */
#define ELEMENT_PTABLE_DIR_LEN  strlen(ELEMENT_PTABLE_DIR) +  1

/**
 * Maximum number of files that may be read from a directory by @c
 * "directory.h".
 */
#define GETFILES_FILE_MAX       256

/**
 * Maximum length of a file name, including the terminating @c NULL byte.
 * @note @c NAME_MAX along with other macros are defined by the POSIX.1
 * standard.
 * @see https://www.gnu.org/software/libc/manual/html_node/Limits-for-Files.html
 */
#define GETFILES_NAME_MAX       (NAME_MAX + 1)

/**
 * Length of a buffer for @c "directory.h".
 */
#define GETFILES_BUFLEN         (GETFILES_FILE_MAX *  GETFILES_NAME_MAX)

/*******************************************************************************
 * Directory locations for things like scientific constant, information, help
 * files, etc.
 ******************************************************************************/

/**
 * The file that contains all constants to be looked up by @c "constants.h".
 */
#define CONST_FILE              DATA_DIR     "/constants.txt"

/**
 * The direcory that contains all element info @c "elements.h".
 */
#define ELEMENT_DIR             DATA_DIR     "/elements"

/**
 * The directory that contains all long, detailed element information to be looked
 * up by @c "elements.h".
 */
#define ELEMENT_INFO_DIR        ELEMENT_DIR  "/info"

/**
 * The directory that contains all short, easily-parsable element information to
 * be looked up by @c "elements.h".
 */
#define ELEMENT_PTABLE_DIR      ELEMENT_DIR  "/ptable"

/**
 * The directory that is used for testing by @c "directory.h".
 */
#define GETFILES_TESTDIR        DATA_DIR     "/get_files"

/**
 * The file that is used by @c "elements.h" to print an ASCII periodic table.
 * @see doc/legal/fair_use_periodni.pdf
 */
#define PTABLE_FILE             ELEMENT_DIR  "/ptable.txt"

/**
 * The file that provides help and usage information for @c "scicalc.h".
 */
#define SCICALC_DOCFILE         DATA_DIR     "/scicalc.txt"

/**
 * The file that is used by @c "pager.h" for testing.
 */
#define TEST_PAGER_FILE         DATA_DIR     "/pager.txt"

/**
 * The file that all the commands are read from.
 */
#define COMMAND_FILE            DATA_DIR     "/commands.yml"

/**
 * The virtual directory that provides access to the process file descriptors.
 * @see proc(5) http://linux.die.net/man/5/proc
 */
#define SC_SELFFD_DIR           "/proc/self/fd/"

#endif /* DIRS_H */

