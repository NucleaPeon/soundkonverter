# Find KF6 CDDB libraries
# Once done this will define
#
#  KF6CDDB_FOUND       - system has kf6 cddb
#  KF6CDDB_INCLUDE_DIR - the kf6 cddb include directory
#

# Copyright (C) Daniel Kettle 2026
# Licenced under the GPL v2
#

INCLUDE(FindPackageHandleStandardArgs)


find_path( KF6CDDB_INCLUDE_DIR
		NAMES "kcddb.h" "kcddb_export.h"
		PATHS "/usr/include/KCddb6"
              "/usr/include/KCddb6/kcddb"
			  "/opt/local/include/KCddb6"
			  "/opt/local/include/KCddb6/kcddb"
			  "/usr/local/include/KCddb6"
			  "/usr/local/include/KCddb6/kcddb"
)

find_package_handle_standard_args(KF6Cddb
	REQUIRED_VARS
		KF6CDDB_INCLUDE_DIR
)
