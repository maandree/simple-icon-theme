#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static int exitstatus = 0;
static char *path = NULL;
static size_t pathsize = 0;
static size_t pathlen = 0;
static char *buf = NULL;
static size_t bufsize = 0;


static void
pushname(const char *name, size_t *old_len_out)
{
	size_t len = pathlen + strlen(name) + 1;

	if (pathsize < len + 1) {
		pathsize = len + 1;
		path = realloc(path, pathsize);
		if (!path) {
			perror("find-errors");
			exit(1);
		}
	}

	*old_len_out = pathlen;
	pathlen = (size_t)(stpcpy(pathlen ? stpcpy(&path[pathlen], "/") : path, name) - path);

}


static void
popname(size_t old_len)
{
	pathlen = old_len;
	path[pathlen] = '\0';
}


static void
checkfile(const char *name)
{
	size_t old;
	int fd;
	size_t len = 0;
	ssize_t r;
	size_t off;
	size_t i;

	pushname(name, &old);

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "find-errors: open %s O_RDONLY: %s\n", path, strerror(errno));
		exit(1);
	}
	for (;;) {
		if (len == bufsize) {
			bufsize += 1024;
			buf = realloc(buf, bufsize);
			if (!buf) {
				perror("find-errors");
				exit(1);
			}
		}
		r = read(fd, &buf[len], bufsize - len);
		if (r <= 0) {
			if (!r)
				break;
			fprintf(stderr, "find-errors: read %s: %s\n", path, strerror(errno));
			exit(1);
		}
		len += (size_t)r;
	}
	close(fd);

	if (len < 7)
		goto out;

	for (off = 0; off < len - 6; off++) {
		if (buf[off] != '#')
			continue;
		for (i = 1; i <= 6; i++) {
			if ('A' <= buf[off + i] && buf[off + i] <= 'F')
				buf[off + i] ^= 'A' ^ 'a';
			else if (!(('a' <= buf[off + i] && buf[off + i] <= 'f') ||
			           ('0' <= buf[off + i] && buf[off + i] <= '9')))
				goto next;
		}
		if (!strncmp(&buf[off + 1], "bebebe", 6) ||
		    !strncmp(&buf[off + 1], "ef2929", 6) ||
		    !strncmp(&buf[off + 1], "f57900", 6) ||
		    !strncmp(&buf[off + 1], "32a678", 6) ||
		    !strncmp(&buf[off + 1], "cd656c", 6) ||
		    !strncmp(&buf[off + 1], "d69553", 6) ||
		    !strncmp(&buf[off + 1], "ccad47", 6) ||
		    !strncmp(&buf[off + 1], "32a679", 6) ||
		    !strncmp(&buf[off + 1], "00a09f", 6) ||
		    !strncmp(&buf[off + 1], "2495be", 6) ||
		    !strncmp(&buf[off + 1], "a46eb0", 6) ||
		    !strncmp(&buf[off + 1], "000000", 6))
			continue;
		fprintf(stderr, "%s uses colour not defined in theme: %.7s\n", path, &buf[off]);
		exitstatus = 1;
	next:;
	}

out:
	popname(old);
}


static void
checkdir(const char *name)
{
	size_t old;
	DIR *dir;
	static size_t old2;
	static struct dirent *f;
	static struct stat st;

	pushname(name, &old);

	dir = opendir(path);
	if (!dir) {
		fprintf(stderr, "find-errors: opendir %s: %s\n", path, strerror(errno));
		exit(1);
	}

	errno = 0;
	while ((f = readdir(dir))) {
		if (f->d_name[0] == '.')
			continue;
		if (f->d_type == DT_DIR) {
			goto dir;
		} else if (f->d_type == DT_REG) {
			goto reg;
		} else if (f->d_type == DT_UNKNOWN) {
			pushname(f->d_name, &old2);
			if (lstat(path, &st)) {
				fprintf(stderr, "find-errors: lstat %s: %s\n", path, strerror(errno));
				exit(1);
			}
			popname(old2);
			if (S_ISDIR(st.st_mode)) {
			dir:
				checkdir(f->d_name);
			} else if (S_ISREG(st.st_mode)) {
			reg:
				if (strlen(f->d_name) > 4 && !memcmp(&f->d_name[strlen(f->d_name) - 4], ".svg", 4))
					checkfile(f->d_name);
			}
		}
	}

	if (errno) {
		fprintf(stderr, "find-errors: readdir %s: %s\n", path, strerror(errno));
		exit(1);
	}

	closedir(dir);
	popname(old);
}


int
main(void)
{
	checkdir("scalable");
	free(path);
	free(buf);
	return exitstatus;
}
