XDESTDIR =\
	$$(if test -n "$(DESTDIR)" && (! printf '%s\n' "$(DESTDIR)" | grep '^/' >/dev/null); then\
		printf '../%s\n' "$(DESTDIR)";\
	else\
		printf '%s\n' "$(DESTDIR)";\
	fi)
