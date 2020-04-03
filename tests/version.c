/*
  Part of: CCPathnames
  Contents: test for version functions
  Date: Feb 25, 2018

  Abstract

	Test file for version functions.

  Copyright (C) 2018, 2020 Marco Maggi <mrc.mgg@gmail.com>

  See the COPYING file.
*/

#include <ccpathnames.h>
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  ccptn_library_init();
  printf("version number string: %s\n", ccptn_version_string());
  printf("libtool version number: %d:%d:%d\n",
	 ccptn_version_interface_current(),
	 ccptn_version_interface_revision(),
	 ccptn_version_interface_age());
  exit(EXIT_SUCCESS);
}

/* end of file */
