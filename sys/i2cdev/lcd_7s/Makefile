include ../common/linux.mk

GIT_VERSION ?= $(shell git describe --always --dirty)
GIT_USR ?= $(shell  git config --get-all user.name)
CFLAGS += -D__GIT_USR__="\"${GIT_USR}\"" -D__GIT_VER__="\"${GIT_VERSION}\""