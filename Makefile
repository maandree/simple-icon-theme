.POSIX:

DEVCHECK = @:
UNIMPORTANT_CHECK =

CONFIGFILE = config.mk
include $(CONFIGFILE)

all:

generated.mk: $(CONFIGFILE) icons.mk Makefile find-errors check-icon-listing
	$(DEVCHECK) ./find-errors
	$(DEVCHECK) check/find-unlisted-icons
	$(DEVCHECK) check/find-duplicates
	$(DEVCHECK) ./check-icon-listing
	@test ! -e $@ || chmod -- u+w $@
	printf '\043 %s\n' 'This file is generated from Makefile' > $@
	printf '\n%1i$$(DIR_SUFFIX)/%%.png: scalable$$(DIR_SUFFIX)/%%.svg conv\n\t./conv $$@\n' $(SIZES) |\
		sed 's/^[0-9]\+/&x&/' >> $@
	set -e; \
	for size in $(SIZES); do\
		printf '\nALL_SIZE_%i_PNG_ICONS =\\\n' $$size >> $@;\
		sed '/^\s*\('"$$(printf '\043')"'.*\|\)$$/d' < icons.mk | sed 1d | sed 's/\\\?$$/\.png&/' |\
			sed 's/^\s*/&'"$${size}x$${size}"'\$$(DIR_SUFFIX)\//' >> $@;\
	done
	printf '\nALL_PNG_ICONS =' >> $@
	printf ' $$(ALL_SIZE_%i_PNG_ICONS)' $(SIZES) >> $@
	printf '\n' >> $@
	@chmod -- a-w $@

find-errors: check/find-errors.c
	$(CC) -o $@ $< $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

check-icon-listing: check/check-icon-listing.c
	$(CC) -o $@ $< $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

check: find-errors check-icon-listing
	$(UNIMPORTANT_CHECK) ./find-errors
	$(UNIMPORTANT_CHECK) check/find-unlisted-icons
	$(UNIMPORTANT_CHECK) check/find-duplicates
	$(UNIMPORTANT_CHECK) ./check-icon-listing
	+cd apps && $(MAKE) check

clean:
	-rm -rf -- index.theme *.o *.su conv generated.mk scalable-"$(DIR_SUFFIX_)"
	-rm -f -- find-errors check-icon-listing
	-for s in $(SIZES); do printf "$${s}x$${s}$(DIR_SUFFIX)\n"; done | xargs rm -rf --
	-+cd apps && $(MAKE) clean

# These are just added so autocompletion works with them
all all-fast all-fast-icons all-apps install uninstall index.theme conv: generated.mk
	+@$(MAKE) -f mk/make-stage-2.mk $@

.DEFAULT:
	+@$(MAKE) generated.mk
	+@$(MAKE) -f mk/make-stage-2.mk $@

.PHONY: all check all-fast all-fast-icons all-apps install uninstall clean
