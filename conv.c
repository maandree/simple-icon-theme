#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static char *argv0;


static int
single(int argc, char *argv[])
{
	char size[10], target[4096], source[4096];
	char *p;
	ssize_t r;
	int fd;

	p = strchr(argv[0], 'x');
	if (!p)
		return 1;
	*p = '\0';
	stpcpy(size, argv[0]);
	*p = 'x';

	p = strchr(p, '/');
	if (!p)
		return 1;
	p = stpcpy(stpcpy(source, "scalable"), p);
	if (strcmp(&p[-4], ".png"))
		return 1;
	stpcpy(&p[-4], ".svg");

	for (p = argv[0]; (p = strchr(p, '/')); p++) {
		*p = '\0';
		if (mkdir(argv[0], 0777) && errno != EEXIST) {
			fprintf(stderr, "%s: mkdir %s 0777: %s\n", argv0, argv[0], strerror(errno));
			return 1;
		}
		*p = '/';
	}

	r = readlink(source, target, sizeof(target) - 1);
	if (r >= 0)
		target[r] = 0;
	if (r >= 0 && (r >= sizeof(target) - 1 || r < 4 || strcmp(&target[r - 4], ".svg"))) {
		return 1;
	} else if (r < 0 && errno == EINVAL) {
		fd = open(argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd < 0) {
			fprintf(stderr, "%s: open %s O_WRONLY|O_CREAT|O_TRUNC 0666: %s\n", argv0, argv[0], strerror(errno));
			return 1;
		}
		if (fd != STDOUT_FILENO) {
			if (dup2(fd, STDOUT_FILENO) != STDOUT_FILENO)
				fprintf(stderr, "%s: dup2 %i 1: %s\n", argv0, fd, strerror(errno));
			close(fd);
		}
		execlp("rsvg-convert", "rsvg-convert", "-w", size, "-h", size, "-f", "png", source, NULL);
		fprintf(stderr, "%s: execvp rsvg-convert: %s\n", argv0, strerror(errno));
		return 1;
	} else if (r < 0) {
		fprintf(stderr, "%s: readlink %s: %s\n", argv0, source, strerror(errno));
		return 1;
	} else {
		stpcpy(&target[r - 4], ".png");
		if (symlink(target, argv[0])) {
			if (errno == EEXIST) {
				unlink(argv[0]);
				if (!symlink(target, argv[0]))
					return 0;
			}
			fprintf(stderr, "%s: symlink %s %s: %s\n", argv0, target, argv[0], strerror(errno));
			return 1;
		}
		return 0;
	}
}


static int
multi(int argc, char *argv[])
{
	char *source, output[4096], target[4096];
	char *p, *q;
	pid_t pid;
	int status, fd, i;
	ssize_t r;

	source = *argv++, argc--;

	q = strchr(source, '/');
	if (!q)
		return 1;

	for (i = 0; i < argc; i++) {
		stpcpy(stpcpy(stpcpy(stpcpy(output, argv[i]), "x"), argv[i]), q);
		for (p = output; (p = strchr(p, '/')); p++) {
			*p = '\0';
			if (mkdir(output, 0777) && errno != EEXIST) {
				fprintf(stderr, "%s: mkdir %s 0777: %s\n", argv0, output, strerror(errno));
				return 1;
			}
			*p = '/';
		}
	}

	r = readlink(source, target, sizeof(target) - 1);
	if (r >= 0)
		target[r] = 0;
	if (r >= 0 && (r >= sizeof(target) - 1 || r < 4 || strcmp(&target[r - 4], ".svg"))) {
		return 1;
	} else if (r < 0 && errno == EINVAL) {
		p = stpcpy(stpcpy(stpcpy(stpcpy(output, argv[0]), "x"), argv[0]), q);
		stpcpy(&p[-4], ".png");
		for (; *argv; argv++, argc--) {
			pid = argc == 1 ? 0 : fork();
			if (pid == -1) {
				fprintf(stderr, "%s: fork: %s\n", argv0, strerror(errno));
				return 1;
			}
			if (pid) {
				p = stpcpy(stpcpy(stpcpy(stpcpy(output, argv[1]), "x"), argv[1]), q);
				stpcpy(&p[-4], ".png");
				if (waitpid(pid, &status, 0) != pid) {
					fprintf(stderr, "%s: waitpid rsvg-convert: %s\n", argv0, strerror(errno));
					return 1;
				}
				if (status)
					return 1;
				continue;
			}
			fd = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if (fd < 0) {
				fprintf(stderr, "%s: open %s O_WRONLY|O_CREAT|O_TRUNC 0666: %s\n", argv0, output, strerror(errno));
				return 1;
			}
			if (fd != STDOUT_FILENO) {
				if (dup2(fd, STDOUT_FILENO) != STDOUT_FILENO)
					fprintf(stderr, "%s: dup2 %i 1: %s\n", argv0, fd, strerror(errno));
				close(fd);
			}
			execlp("rsvg-convert", "rsvg-convert", "-w", *argv, "-h", *argv, "-f", "png", source, NULL);
			fprintf(stderr, "%s: execvp rsvg-convert: %s\n", argv0, strerror(errno));
			return 1;
		}
	} else if (r < 0) {
		fprintf(stderr, "%s: readlink %s: %s\n", argv0, source, strerror(errno));
		return 1;
	} else {
		stpcpy(&target[r - 4], ".png");
		for (; *argv; argv++) {
			p = stpcpy(stpcpy(stpcpy(stpcpy(output, *argv), "x"), *argv), q);
			stpcpy(&p[-4], ".png");
			if (symlink(target, output)) {
				if (errno == EEXIST) {
					unlink(output);
					if (!symlink(target, output))
						continue;
				}
				fprintf(stderr, "%s: symlink %s %s: %s\n", argv0, target, output, strerror(errno));
				return 1;
			}
		}
		return 0;
	}
}


int
main(int argc, char *argv[])
{
	argv0 = *argv++, argc--;
	if (argc == 1)
		return single(argc, argv);
	if (argc > 1)
		return multi(argc, argv);
	return 1;
}
