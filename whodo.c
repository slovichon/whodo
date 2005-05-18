/* $Id$ */

#include <sys/param.h>

#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

int	dohdr = 1;
int	dolong = 0;

__dead void	usage(void);
void		whodo(void);

int
main(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt(argc, argv, "hl")) != -1)
		switch (ch) {
		case 'h':
			dohdr = 0;
			break;
		case 'l':
			dolong = 1;
			break;
		default:
			usage();
			/* NOTREACHED */
		}

	if (dohdr) {
	}
	whodo();
	exit(0);
}

void
hdr(void)
{
	if (dolong) {
		printf("\n"
		    "%*s %*s %*s %*s %*s %*s %*s\n",
		    8, "User", 8, "tty",
		    7, "login@", 6, "idle",
		    6, "JCPU", 6, "PCPU",
		    5, "what");
	} else {
		char *stm, hostname[MAXHOSTNAMELEN];
		time_t tm;

		time(&tm);
		stm = ctime(&tm);
		if (gethostname(hostname, sizeof(hostname)) == -1)
			err(1, "gethostname");
		printf("%s%s\n", stm, hostname);
	}
}

void
whodo(void)
{
	struct utmp utmp;
	ssize_t siz;
	int fd;

	if (dohdr)
		hdr();
	if ((fd = open(_PATH_UTMP, O_RDONLY)) == -1)
		err(1, "%s", _PATH_UTMP);
	for (;;) {
		if ((siz = read(fd, &utmp, sizeof(utmp))) !=
		    sizeof(utmp))
			break;
		if (utmp.ut_name[0] == '\0')
			continue;
		if (dolong) {
			printf("%-*s %*s %*s\n",
			    8, utmp.ut_name,
			    8, utmp.ut_line,
			    8, utmp.ut_host);
		} else {
		}
	}
	if (siz == -1)
		err(1, "read");
	close(fd);
}

void
usage(void)
{
	extern char *__progname;

	fprintf(stderr, "usage: %s [-hl] [user]\n", __progname);
	exit(1);
}
