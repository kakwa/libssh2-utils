# Name of the package
NAME = libssh2-utils

# Version
VERSION=$(shell git describe --tags --dirty | sed 's/-/./g')

# Revision number 
RELEASE = 1

# URL of the project 
URL=https://github.com/kakwa/$(NAME)

# short summary of what the package provides
SUMMARY=companion tools for libssh2

# long version of the summary, but I'm lazy
DESCRIPTION=$(SUMMARY)

# License of the upstream project
LICENSE=MIT

###############################################################################
# Rule #
########


# Including common rules and targets 
include buildenv/Makefile.common

# preparation hook for rpm package
# source archives must recovered here
# and their content put inside $(BUILD_DIR)/src
#
# $(WGS) should be the prefered way to recover archives
# (add checksum against a manifest file).
# use 'make manifest' to generate/update this manifest file.

$(SOURCE_ARCHIVE): $(SOURCE_DIR) $(CACHE) Makefile MANIFEST
	rm -rf $(SOURCE_DIR)
	rsync -ap --ignore-errors --force --exclude pkg \
		--exclude CMakeFiles \
		--exclude Makefile  \
		--exclude libssh2-*  \
		--exclude CMakeCache.txt  \
		--exclude cmake_install.cmake  \
		../ $(SOURCE_DIR)
	$(SOURCE_TAR_CMD)
