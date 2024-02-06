.POSIX:

DEVCHECK = @:

CONFIGFILE = config.mk
include $(CONFIGFILE)

all:

generated.mk: $(CONFIGFILE) icons.mk Makefile
	$(DEVCHECK) check/find-errors
	$(DEVCHECK) check/find-unlisted-icons
	$(DEVCHECK) check/check-icons-listing
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

clean:
	-rm -rf -- index.theme *.o *.su conv generated.mk scalable-"$(DIR_SUFFIX_)"
	-for s in $(SIZES); do printf "$${s}x$${s}$(DIR_SUFFIX)\n"; done | xargs rm -rf --

# These are just added so autocompletion works with them
all all-fast all-fast-icons install uninstall index.theme conv: generated.mk
	+@$(MAKE) -f mk/make-stage-2.mk $@

.DEFAULT:
	+@$(MAKE) generated.mk
	+@$(MAKE) -f mk/make-stage-2.mk $@

.PHONY: all all-fast all-fast-icons install uninstall clean
