.POSIX:

APPS =\
	claws-mail

include ../mk/xdestdir.mk

.DEFAULT:
	+set -e; for a in $(APPS); do (cd -- "$$a" && $(MAKE) DESTDIR="$(XDESTDIR)" "$@"); done
