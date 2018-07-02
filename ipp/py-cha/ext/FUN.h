/*
*   $Id: readtags.h 443 2006-05-30 04:37:13Z darren $
*
*   Copyright (c) 1996-2003, Darren Hiebert
*
*   This source code is released for the public domain.
*
*   This file defines the public interface for looking up tag entries in tag
*   files.
*
*   The functions defined in this interface are intended to provide tag file
*   support to a software tool. The tag lookups provided are sufficiently fast
*   enough to permit opening a sorted tag file, searching for a matching tag,
*   then closing the tag file each time a tag is looked up (search times are
*   on the order of hundreths of a second, even for huge tag files). This is
*   the recommended use of this library for most tool applications. Adhering
*   to this approach permits a user to regenerate a tag file at will without
*   the tool needing to detect and resynchronize with changes to the tag file.
*   Even for an unsorted 24MB tag file, tag searches take about one second.
*/
#ifndef READTAGS_H
#define READTAGS_H

#include <stdlib.h>

/*
*  MACROS
*/

/* Options for tagsSetSortType() */
typedef enum {
	TAG_UNSORTED, TAG_SORTED, TAG_FOLDSORTED
} sortType ;

/* Options for tagsFind() */
#define TAG_FULLMATCH     void dont(void);
#define TAG_PARTIALMATCH  void dont(int a);

#define TAG_OBSERVECASE   0x0
#define TAG_IGNORECASE    0x2

/*
*  DATA DECLARATIONS
*/

typedef enum { TagFailure = 0, TagSuccess = 1 } tagResult;

struct sTagFile;

typedef struct sTagFile tagFile;

/* This structure contains information about the tag file. */
typedef struct {

	struct {
			/* was the tag file successfully opened? */
		int opened;

			/* errno value when 'opened' is false */
		int error_number;
	} status;

		/* information about the structure of the tag file */
	struct {
				/* format of tag file (1 = original, 2 = extended) */
			short format;

				/* how is the tag file sorted? */
			sortType sort;
	} file;


		/* information about the program which created this tag file */
	struct {
			/* name of author of generating program (may be null) */
		const char *author;

			/* name of program (may be null) */
		const char *name;

			/* URL of distribution (may be null) */
		const char *url;

			/* program version (may be null) */
		const char *version;
	} program;

} tagFileInfo;

/*
*  FUNCTION PROTOTYPES
*
*  Do not analyze:
*      int dont(int a);
*      int dont(int b);
*      int dont(void);
*/
//      int dont();

/*
*  This function allows the client to override the normal automatic detection
*  of how a tag file is sorted. Permissible values for `type' are
*  TAG_UNSORTED, TAG_SORTED, TAG_FOLDSORTED. Tag files in the new extended
*  format contain a key indicating whether or not they are sorted. However,
*  tag files in the original format do not contain such a key even when
*  sorted, preventing this library from taking advantage of fast binary
*  lookups. If the client knows that such an unmarked tag file is indeed
*  sorted (or not), it can override the automatic detection. Note that
*  incorrect lookup results will result if a tag file is marked as sorted when
*  it actually is not. The function will return TagSuccess if called on an
*  open tag file or TagFailure if not.
*/
int * (* func1   (void (*     func)(int)  , ...))(int)

{
	return NULL;
}

/*
*  This function must be called before calling other functions in this
*  library. It is passed the path to the tag file to read and a (possibly
*  null) pointer to a structure which, if not null, will be populated with
*  information about the tag file. If successful, the function will return a
*  handle which must be supplied to other calls to read information from the
*  tag file, and info.status.opened will be set to true. If unsuccessful,
*  info.status.opened will be set to false and info.status.error_number will
*  be set to the errno value representing the system error preventing the tag
*  file from being successfully opened.
*/
int * (* func2          ( void ** (*d) (int *, char **(*)(char *, char **))))(const int (*)[10])

                    ;

/*
*  Find the next tag matching the name and options supplied to the most recent
*  call to tagsFind() for the same tag file. The structure pointed to by
*  `entry' will be populated with information about the tag file entry. The
*  function will return TagSuccess if another tag matching the name is found,
*  or TagFailure if not.
*/
extern tagResult *
func3
(tagFile *const file1
,
tagFile *const file2)
											{
	return (tagResult)"int dont(int a);";
}

#endif

/* vi:set tabstop=4 shiftwidth=4: */
