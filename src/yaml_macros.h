/**
 * @file yaml_macros.h
 * @author Alex Striff
 *
 * @brief Convenience macros for parsing YAML with libyaml.
 */

#ifndef YAML_MACROS_H
#define YAML_MACROS_H

#include <yaml.h>

/**
 * Converts from a YAML style enum value to a string.
 */
#define EVENT_STYLE(A) \
	((A == YAML_ANY_SCALAR_STYLE) ? \
	 "YAML_ANY_SCALAR_STYLE" : ( \
		 (A == YAML_PLAIN_SCALAR_STYLE) ? \
		 "YAML_PLAIN_SCALAR_STYLE" : ( \
			 (A == YAML_SINGLE_QUOTED_SCALAR_STYLE) ? \
			 "YAML_SINGLE_QUOTED_SCALAR_STYLE" : ( \
				 (A == YAML_DOUBLE_QUOTED_SCALAR_STYLE) ? \
				 "YAML_DOUBLE_QUOTED_SCALAR_STYLE" : ( \
					 (A == YAML_LITERAL_SCALAR_STYLE) ? \
					 "YAML_LITERAL_SCALAR_STYLE" : ( \
						 (A == YAML_FOLDED_SCALAR_STYLE) ? \
						 "YAML_FOLDED_SCALAR_STYLE" : \
						 "UNKNOWN STYLE" ))))))

/**
 * Converts from a YAML type enum value to a string.
 */
#define EVENT_TYPE(A) \
	((A == YAML_NO_EVENT) ? \
	 "YAML_NO_EVENT" : ( \
		 (A == YAML_STREAM_START_EVENT) ? \
		 "YAML_STREAM_START_EVENT" : ( \
			 (A == YAML_STREAM_END_EVENT) ? \
			 "YAML_STREAM_END_EVENT" : ( \
				 (A == YAML_DOCUMENT_START_EVENT) ? \
				 "YAML_DOCUMENT_START_EVENT" : ( \
					 (A == YAML_DOCUMENT_END_EVENT) ? \
					 "YAML_DOCUMENT_END_EVENT" : ( \
						 (A == YAML_SEQUENCE_START_EVENT) ? \
						 "YAML_SEQUENCE_START_EVENT" : ( \
							 (A == YAML_SEQUENCE_END_EVENT) ? \
							 "YAML_SEQUENCE_END_EVENT" : ( \
								 (A == YAML_MAPPING_START_EVENT) ? \
								 "YAML_MAPPING_START_EVENT" : ( \
									 (A == YAML_MAPPING_END_EVENT) ? \
									 "YAML_MAPPING_END_EVENT" : ( \
										 (A == YAML_ALIAS_EVENT) ? \
										 "YAML_ALIAS_EVENT" : ( \
											 (A == YAML_SCALAR_EVENT) ? \
											 "YAML_SCALAR_EVENT" : \
											 "UNKNOWN EVENT" )))))))))))

/**
 * Default destination encoding.
 */
#define TO_ENCODING "ASCII//"

/**
 * Converts from a YAML encoding enum value to a locale string.
 */
#define EVENT_ENCODING(A) \
	((A == YAML_ANY_ENCODING) ? \
	 TO_ENCODING : ( \
		 (A == YAML_UTF8_ENCODING) ? \
		 "UTF8//" : ( \
			 (A == YAML_UTF16LE_ENCODING) ? \
			 "UTF16LE" : ( \
				 (A == YAML_UTF16BE_ENCODING) ? \
				 "UTF16BE" : \
				 TO_ENCODING ))))

/**
 * Escapes a special character.
 */
#define ESCAPE_CHAR(A) \
	((A == 'a') ?  \
	 '\a' : ( \
		 (A == 'b') ?  \
		 '\b' : ( \
			 (A == 't') ?  \
			 '\t' : ( \
				 (A == 'n') ? \
				 '\n' : ( \
					 (A == 'v') ? \
					 '\v' : ( \
						 (A == 'f') ? \
						 '\f' : ( \
							 (A == 'r') ? \
							 '\r' : ( \
								 (A == '\\') ? \
								 '\\' : \
								 '\0' ))))))))

/**
 * Finds the minimum value.
 */
#define MIN(A, B)	((A < B) ? A : B)

/**
 * Finds the minimum length of a scalar literal (e.g. @c PARENT_SCALAR)
 * and a scalar length.
 */
#define SCALAR_MIN_LEN(A, B)	(MIN(strlen(A), B))

/**
 * Syntactic sugar for scalar literal / event scalar comparisions.
 */
#define SCALAR_CMP(A, B)	(strncmp(A, (char *) B.value, \
			SCALAR_MIN_LEN(A, B.length)))

#endif /* YAML_MACROS_H */

