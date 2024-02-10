scalable-$(DIR_SUFFIX_)/%.svg: scalable/%.svg
	@mkdir -p -- "$$(dirname -- "$@")"
	set -e;\
	F=$$(printf '%s\n' "$@" | cut -d / -f 2-);\
	if ! test "$@" = "scalable/$$F"; then\
		if test -L "scalable/$$F"; then\
			ln -sf "$$(readlink -- "scalable/$$F")" "$@";\
		else\
			sed < "scalable/$$F" > "$@"\
				-e 's/#[bB][eE][bB][eE][bB][eE]/#$(BASE_COLOUR)/g'\
				-e 's/#[eE][fF]2929/#$(ALARM_RED)/g'\
				-e 's/#[fF]57900/#$(ALARM_ORANGE)/g'\
				-e 's/#32[aA]678/#$(VERIFIED)/g'\
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
