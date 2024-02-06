.POSIX:

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

all:

include generated.mk
include mk/portable.mk
include mk/unportable.mk

# For implementation that do not support pattern matching rules
.DEFAULT:
	+@set -e;\
	if printf '%s\n' "$@" | grep '^scalable' > /dev/null; then\
		sed < mk/unportable.mk '1s|%|'"$$(printf '%s\n' "$@" | sed 's|^scalable/\(.*\)\.svg$$|\1|')"'|g' | $(MAKE) -f - "$@";\
	elif printf '%s\n' "$@" | grep '\.png$$' > /dev/null; then\
		printf '%s\n' "$@" >&2 ;\
		$(MAKE) -f mk/make-stage-2.mk conv &&\
		f="$$(printf '%s\n' "$@" | sed 's|^[^/]*\/\(.*\)\.png$$|\1|')" &&\
		if test -n "$(DIR_SUFFIX)"; then \
			$(MAKE) -f mk/make-stage-2.mk scalable$(DIR_SUFFIX)/$$f.svg; \
		fi && \
		sed '/^[a-zA-Z0-9].*=/,$$d' < generated.mk | sed 's|%|'"$$f"'|g' | $(MAKE) -f - "$@";\
	else\
		printf 'No rule to make target %s\n' "$@" >&2;\
		exit 2;\
	fi

.PHONY: all all-fast all-fast-icons install uninstall clean
