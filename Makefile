.NONPOSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

DIRS =\
	actions\
	apps\
	categories\
	devices\
	emblems\
	emotes\
	mimetypes\
	places\
	status

include icons.mk

ALL_PNG_ICONS = $(foreach S,$(SIZES),$(foreach I,$(ICONS),$(S)x$(S)$(DIR_SUFFIX)/$(I).png))

all: index.theme $(ALL_PNG_ICONS)

all-fast: index.theme all-fast-icons

index.theme: Makefile
	set -e;\
	printf '%s\n' \
		'[Icon Theme]'\
		'Name=$(THEME_NAME)'\
		'Comment=$(THEME_DESC)'\
		'Example=folder'\
		''\
		'# KDE specific stuff'\
		'DisplayDepth=32'\
		'LinkOverlay=link_overlay'\
		'LockOverlay=lock_overlay'\
		'ZipOverlay=zip_overlay'\
		'DesktopDefault=48'\
		'DesktopSizes='$$(printf ',%s' $(SIZES) | sed 's/^,//')\
		'ToolbarDefault=22'\
		'ToolbarSizes=8,16,22,32,48'\
		'MainToolbarDefault=22'\
		'MainToolbarSizes=8,16,22,32,48'\
		'SmallDefault=16'\
		'SmallSizes=16'\
		'PanelDefault=32'\
		'PanelSizes='$$(printf ',%s' $(SIZES) | sed 's/^,//')\
		''\
		> index.theme
	printf 'Directories=' >> index.theme
	set -e;\
	for s in $(SIZES); do\
		for d in $(DIRS); do\
			printf ',%sx%s/%s' $$s $$s $$d;\
		done;\
	done | sed 's/^,//' >> index.theme
	set -e;\
	for d in $(DIRS); do\
		printf ',scalable/%s' $$d;\
	done >> index.theme
	printf '\n' >> index.theme
	set -e;\
	for s in $(SIZES) scalable; do\
		for d in $(DIRS); do\
			printf '\n';\
			./make-dir-info $$s $$d;\
		done;\
	done >> index.theme

all-fast-icons: $(ICONS:=.x)

scalable-$(DIR_SUFFIX_)/%.svg: scalable/%.svg
	@mkdir -p -- "$$(dirname "$@")"
	if ! test "$@" = "scalable/$*.svg"; then\
		if test -L "scalable/$*.svg"; then\
			ln -s "$$(readlink -- "scalable/$*.svg")" $@;\
		else\
			sed < scalable/$*.svg > $@\
				-e 's/#[bB][eE][bB][eE][bB][eE]/#$(BASE_COLOUR)/g'\
				-e 's/#[eE][fF]2929/#$(ALARM_RED)/g'\
				-e 's/#[fF]57900/#$(ALARM_ORANGE)/g'\
				-e 's/#[cC][dD]656[cC]/#$(RED)/g'\
				-e 's/#[dD]69553/#$(ORANGE)/g'\
				-e 's/#[cC][cC][aA][dD]47/#$(YELLOW)/g'\
				-e 's/#32[aA]679/#$(GREEN)/g'\
				-e 's/#00[aA]09[fF]/#$(CYAN)/g'\
				-e 's/#2495[bB][eE]/#$(BLUE)/g'\
				-e 's/#[aA]46[eE][bB]0/#$(MAGENTA)/g'\
				-e 's/#000000/#$(OUTLINE)/g';\
		fi;\
	fi

%.x: conv
	@if test -n "$(DIR_SUFFIX)"; then make scalable$(DIR_SUFFIX)/$*.svg; fi
	@ # Does work as a dependeny in GNU make for some reason,
	@ # additionally, this lets us avoid starting make once
	@ # extra for every icon
	./conv scalable$(DIR_SUFFIX)/$*.svg $(SIZES)

conv: conv.c
	$(CC) -o $@ $< $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

8x8$(DIR_SUFFIX)/%.png: scalable$(DIR_SUFFIX)/%.svg conv
	./conv $@

16x16$(DIR_SUFFIX)/%.png: scalable$(DIR_SUFFIX)/%.svg conv
	./conv $@

22x22$(DIR_SUFFIX)/%.png: scalable$(DIR_SUFFIX)/%.svg conv
	./conv $@

24x24$(DIR_SUFFIX)/%.png: scalable$(DIR_SUFFIX)/%.svg conv
	./conv $@

32x32$(DIR_SUFFIX)/%.png: scalable$(DIR_SUFFIX)/%.svg conv
	./conv $@

36x36$(DIR_SUFFIX)/%.png: scalable$(DIR_SUFFIX)/%.svg conv
	./conv $@

48x48$(DIR_SUFFIX)/%.png: scalable$(DIR_SUFFIX)/%.svg conv
	./conv $@

install: index.theme $(ALL_PNG_ICONS)
	mkdir -p -- "$(DESTDIR)$(ICONPREFIX)"
	set -e;\
	for d in $(DIRS); do\
		for s in $(SIZES); do\
			mkdir -p -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/$${s}x$${s}/$${d}";\
		done;\
		mkdir -p -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/scalable/$${d}";\
	done
	set -e;\
	for i in $(ICONS); do\
		for s in $(SIZES); do\
			cp -P -- "$${s}x$${s}$(DIR_SUFFIX)/$${i}.png" "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/$${s}x$${s}/$${i}.png";\
		done;\
		cp -P -- "scalable$(DIR_SUFFIX)/$${i}.svg" "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/scalable/$${i}.svg";\
	done
	cp -- index.theme "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/index.theme"

uninstall:
	rm -rf -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)"

clean:
	-rm -f -- index.theme *.o *.su conv
	-if test -n "$(DIR_SUFFIX)"; then rm -rf -- scalable$(DIR_SUFFIX); fi
	-for s in $(SIZES); do printf "$${s}x$${s}$(DIR_SUFFIX)\n"; done | xargs rm -rf --

.PHONY: all all-fast all-fast-icons install uninstall clean
