# - Try to find the KCDDB libraries
# Once done this will define
#
#  KCDDB_FOUND       - system has cddb
#  KCDDB_INCLUDE_DIR         - the cddb include directory
#  KCDDB_LIBRARIES   - Link these to use cddb
#

# Copyright (c) 2006, Richard Laerkaeng, <richard@goteborg.utfors.se>
# Copyright (c) 2007, Allen Winter, <winter@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(FindPackageHandleStandardArgs)

set(KCDDB_FOUND FALSE)

find_library(KCDDB_LIBRARIES NAMES KCddb6 KCddb kcddb.h)

find_path( KCDDB_INCLUDE_DIR
		NAMES "KCDDB"
		PATHS "/usr/include/KCddb6/"
)

find_package_handle_standard_args(KCddb6
	REQUIRED_VARS
		KCDDB_INCLUDE_DIR

)
