all: index.theme $(ALL_PNG_ICONS)

all-fast: index.theme all-fast-icons

all-fast-icons: $(ICONS:=.x)

$(ICONS:=.x): conv
	@+test -z "$(DIR_SUFFIX)" || $(MAKE) scalable$(DIR_SUFFIX)/$(@:.x=.svg)
	./conv scalable$(DIR_SUFFIX)/$(@:.x=.svg) $(SIZES)

index.theme: $(CONFIGFILE) mk/portable.mk mk/make-dir-info
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
			mk/make-dir-info $$s $$d;\
		done;\
	done >> index.theme

conv: mk/conv.c
	$(CC) -o $@ $< $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

install: index.theme $(ALL_PNG_ICONS)
	mkdir -p -- "$(DESTDIR)$(ICONPREFIX)"
	set -e;\
	for d in $(DIRS); do\
		for s in $(SIZES); do\
			mkdir -p -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/$${s}x$${s}/$${d}";\
		done;\
		mkdir -p -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/scalable/$${d}";\
	done
	head -n 1 < icons.mk | grep '^ICONS' > /dev/null
	set -e;\
	(sed '/[^\\]$$/q' | sed 's/\\$$//' | sed '1s/^.*=//' | sed 's/[[:space:]]//g' | sed '/^$$/d') < icons.mk |\
	while read i; do\
		for s in $(SIZES); do\
			cp -P -- "$${s}x$${s}$(DIR_SUFFIX)/$${i}.png" "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/$${s}x$${s}/$${i}.png";\
		done;\
		cp -P -- "scalable$(DIR_SUFFIX)/$${i}.svg" "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/scalable/$${i}.svg";\
	done
	cp -- index.theme "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/index.theme"

#`(sed ...) < icons.mk | while read i` is used instead of `for i in $(ICONS)` because $(ICONS) got too big for sh(1)
#The 

uninstall:
	-rm -rf -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)"

clean:
	+@$(MAKE) -f Makefile clean
