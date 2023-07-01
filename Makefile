.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

all:

generated-rules.mk: $(CONFIGFILE)
	@test ! -e $@ || chmod -- u+w $@
	printf '# %s\n' 'This file is generated from Makefile' > $@
	printf '\n%1i$$(DIR_SUFFIX)/%%.png: scalable$$(DIR_SUFFIX)/%%.svg conv\n\t./conv $$@\n' $(SIZES) | \
		sed 's/^[0-9]\+/&x&/' >> $@
	@chmod -- a-w $@

clean:
	-rm -f -- index.theme *.o *.su conv generated-rules.mk
	-if test -n "$(DIR_SUFFIX)"; then rm -rf -- scalable$(DIR_SUFFIX); fi
	-for s in $(SIZES); do printf "$${s}x$${s}$(DIR_SUFFIX)\n"; done | xargs rm -rf --

# These are just added so autocompletion works with them
all all-fast all-fast-icons install uninstall index.theme conv: generated-rules.mk
	+@$(MAKE) -f make-stage-2.mk $@

.DEFAULT:
	+@$(MAKE) generated-rules.mk
	+@$(MAKE) -f make-stage-2.mk $@

.PHONY: all all-fast all-fast-icons install uninstall clean
