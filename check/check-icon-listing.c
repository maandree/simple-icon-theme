#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum filetype {
	ERROR,
	NO_SUCH_FILE,
	REGULAR,
	SYMLINK,
	OTHER_TYPE
};

static size_t lineno = 0;
static char *text = NULL;
static size_t textlen = 0;
static int exitstatus = 0;
static char *filebuf = NULL;
static char *targetbuf = NULL;
static char *relbuf = NULL;

static void
loadicons(void)
{
	int fd;
	size_t size = 0;
	ssize_t r;

	fd = open("icons.mk", O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open icons.mk: %s\n", strerror(errno));
		exit(1);
	}

	for (;;) {
		if (textlen == size) {
			size += 128UL << 10;
			text = realloc(text, size);
			if (!text) {
				fprintf(stderr, "Failed to allocate enough memory to load icons.mk\n");
				exit(1);
			}
		}

		r = read(fd, &text[textlen], size - textlen);
		if (r < 0) {
			fprintf(stderr, "Failed to read icons.mk: %s\n", strerror(errno));
			exit(1);
		} else if (!r) {
			break;
		} else {
			textlen += (size_t)r;
		}
	}

	close(fd);
}

static char *
nextline(void)
{
	static size_t pos = 0;
	char *line = &text[pos];

	if (pos == textlen)
		return NULL;

	lineno += 1;

	for (; pos < textlen; pos++) {
		if (!text[pos]) {
			fprintf(stderr, "Line %zu in icons.mk contains NUL byte\n", lineno);
			exitstatus = 1;
			text[pos] = '^';
		} else if (text[pos] == '\n') {
			text[pos++] = '\0';
			break;
		}
	}

	return line;
}

static int
iscomment(char *line)
{
	while (*line == ' ' || *line == '\t')
		line++;
	return *line == '#' || !*line;
}

static void
remove_line_continuation(char *line)
{
	size_t n = strlen(line);
	if (n && line[n - 1] == '\\')
		line[n - 1] = '\0';
}

static void
rstrip(char *line)
{
	size_t n = strlen(line);
	while (n && (line[n - 1] == ' ' || line[n - 1] == '\t'))
		line[--n] = '\0';
}

static size_t
getindent(char **linep)
{
	size_t indent = 0;
	char *line = *linep;
	int warned_sp = 0;

	for (;; line++) {
		if (*line == ' ') {
			if (!warned_sp) {
				fprintf(stderr, "Line %zu in icons.mk contains SP character instead of HT\n", lineno);
				warned_sp = 1;
			}
			exitstatus = 1;
			indent += 1;
		} else if (*line == '\t') {
			indent += 8;
		} else {
			break;
		}
	}

	*linep = line;
	return (indent + 7) / 8;
}

static char *
getfile(char *icon)
{
	static size_t size = 0;
	size_t req = strlen(icon) + sizeof("scalable/.svg");
	if (req > size) {
		size = req;
		filebuf = realloc(filebuf, size);
		if (!filebuf) {
			fprintf(stderr, "Failed to allocate enough memory to validate icons.mk\n");
			exit(1);
		}
	}
	stpcpy(stpcpy(stpcpy(filebuf, "scalable/"), icon), ".svg");
	return filebuf;
}

static enum filetype
getlink(char *file, char **target_out)
{
	static size_t size = 0;
	ssize_t r;
	struct stat st;

	if (!size) {
	grow_and_try_again:
		size += 4096;
		targetbuf = realloc(targetbuf, size);
		if (!targetbuf) {
			fprintf(stderr, "Failed to allocate enough memory to validate icons.mk\n");
			exit(1);
		}
	}

	r = readlink(file, targetbuf, size - 1);
	if (r >= 0) {
		if (r >= size - 2)
			goto grow_and_try_again;
		while (r && !targetbuf[r - 1])
			r -= 1;
		targetbuf[r] = '\0';
		*target_out = targetbuf;
		return SYMLINK;
	} else if (errno == ENOENT) {
		*target_out = NULL;
		return NO_SUCH_FILE;
	} else if (errno == EINVAL) {
		*target_out = NULL;
		if (!lstat(file, &st))
			return S_ISREG(st.st_mode) ? REGULAR : OTHER_TYPE;
		fprintf(stderr, "Failure at line %zu in icons.mk: lstat %s: %s\n", lineno, file, strerror(errno));
		exitstatus = 1;
		return ERROR;
	} else {
		fprintf(stderr, "Failure at line %zu in icons.mk: readlink %s: %s\n", lineno, file, strerror(errno));
		exitstatus = 1;
		*target_out = NULL;
		return ERROR;
	}
}

static char *
rel(char *to, char *from)
{
	static size_t size = 0;
	size_t i, req, up = 0;
	char *p;

	size_t off = 0;
	for (i = 0; to[i]; i++) {
		if (to[i] != from[i])
			break;
		if (to[i] == '/')
			off = i + 1;
	}
	to = &to[off];
	from = &from[off];

	while ((from = strchr(from, '/'))) {
		from = &from[1];
		up += 1;
	}

	if (!up)
		return to;

	req = up * 3 + strlen(to) + 1;
	if (req > size) {
		size += 4096;
		relbuf = realloc(relbuf, size = req);
		if (!relbuf) {
			fprintf(stderr, "Failed to allocate enough memory to validate icons.mk\n");
			exit(1);
		}
	}

	p = relbuf;
	while (up--)
		p = stpcpy(p, "../");
	stpcpy(p, to);
	return relbuf;
}

int
main(void)
{
	char *line;
	char *file;
	char *target;
	char *goal;
	int ret = 0;
	size_t count = 0;
	size_t indent;
	char **stack = NULL;
	size_t stacksize = 0;
	size_t stacklen = 0;
	struct stat st1, st2;
	const char *diff;
	size_t len;
	int fixed;

	loadicons();

	while ((line = nextline())) {
		remove_line_continuation(line);
		if (iscomment(line))
			continue;
		break;
	}

	while ((line = nextline())) {
		remove_line_continuation(line);
		if (iscomment(line))
			continue;

		count += 1;
		rstrip(line);
		indent = getindent(&line);
		if (strchr(line, ' ') || strchr(line, '\t')) {
			fprintf(stderr, "Line %zu in icons.mk contains unexpected whitespace\n", lineno);
			exitstatus = 1;
		}

		if (!indent) {
			fprintf(stderr, "Line %zu in icons.mk is not indented\n", lineno);
			exitstatus = 1;
			break;
		}
		indent -= 1;
		if (indent > stacklen) {
			fprintf(stderr, "Line %zu in icons.mk (%s) is overindented\n", lineno, line);
			exitstatus = 1;
			break;
		}
		if (stacksize <= indent) {
			stacksize += 32;
			stack = realloc(stack, stacksize * sizeof(*stack));
			if (!stack) {
				fprintf(stderr, "Failed to allocate enough memory to validate icons.mk\n");
				exit(1);
			}
		}
		stack[indent] = line;
		stacklen = indent + 1;

		file = getfile(line);
		switch (getlink(file, &target)) {
		case ERROR:
			continue;
		case NO_SUCH_FILE:
			fprintf(stderr, "%s is listed but does not exist\n", line);
			exitstatus = 1;
			continue;
		case REGULAR:
			if (indent) {
				fprintf(stderr, "%s is not a symlink but is listed as linking to %s\n", line, stack[indent - 1]);
				exitstatus = 1;
				continue;
			}
			break;
		case SYMLINK:
			if (!indent) {
				fprintf(stderr, "%s is a symlink but not indented\n", line);
				exitstatus = 1;
			}
			len = strlen(target);
			if (len < 5 || strcmp(&target[len - 4], ".svg")) {
				fprintf(stderr, "target of %s (%s) is not a .svg-file\n", line, target);
				exitstatus = 1;
				continue;
			}
			target[len -= 4] = '\0';
			break;
		case OTHER_TYPE:
		default:
			fprintf(stderr, "%s is listed as an icon but is not a regular file\n", line);
			exitstatus = 1;
			continue;
		}

		if (indent) {
			if (stat(file, &st1)) {
				if (errno == ENOENT) {
					fprintf(stderr, "%s is a dangling symlink\n", line);
				} else {
					fprintf(stderr, "Failure at line %zu in icons.mk: stat %s: %s\n",
					        lineno, line, strerror(errno));
				}
				exitstatus = 1;
				continue;
			}

			file = getfile(stack[indent - 1]);
			if (stat(file, &st2)) {
				fprintf(stderr, "Failure at line %zu in icons.mk (%s): stat %s: %s\n",
				        lineno, line, file, strerror(errno));
			}

			if (st1.st_dev == st2.st_dev && st1.st_ino == st2.st_ino)
				diff = "same real file";
			else
				diff = "different real file";

			fixed = 0;
			while (target[0] == '.' && target[1] == '/') {
				target = &target[2];
				fixed = 1;
			}
			goal = rel(stack[indent - 1], line);
			if (strcmp(target, goal)) {
				fprintf(stderr, "%s links to %s.svg but should link to %s.svg [%s] (%s)\n",
					line, target, goal, stack[indent - 1], diff);
				exitstatus = 1;
				continue;
			}
			if (fixed) {
				fprintf(stderr, "Fixing symlink %s\n", line);
				file = getfile(line);
				*strchr(target, '\0') = '.'; /* restore ".svg" at the end of the string */
				if (unlink(file)) {
					fprintf(stderr, "... failed to unlink\n");
				} else if (symlink(target, file)) {
					fprintf(stderr, "... failed to create symlink (%s -> %s)\n", file, target);
					exitstatus = 1;
					break;
				}
			}
		}
	}

	if (!count) {
		fprintf(stderr, "No icons are listed in icons.mk\n");
		exitstatus = 1;
	}

	free(stack);
	free(text);
	free(filebuf);
	free(targetbuf);
	free(relbuf);
	return exitstatus;
}
