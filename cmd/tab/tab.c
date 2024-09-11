#include <u.h>
#include <libc.h>
#include <bio.h>

static int nstop = 8;
static int minspace;
static Biobuf bout;

static void
untab(Biobuf *b)
{
	long c;
	int pos;

	pos = 0;
	while((c = Bgetrune(b)) != Beof) {
		if(c == '\t') {
			do
				Bputrune(&bout, ' ');
			while(++pos % nstop);
		} else {
			Bputrune(&bout, c);
			if(c == '\n')
				pos = 0;
			else
				++pos;
		}
	}
}

static void
putspaces(int pos, int n)
{
	int epos;

	if(n < minspace) {
		while(n-- > 0)
			Bputrune(&bout, ' ');
		return;
	}

	epos = pos + n;
	while(pos/nstop != epos/nstop) {
		Bputrune(&bout, '\t');
		pos += nstop - pos % nstop;
	}
	while(pos++ < epos)
		Bputrune(&bout, ' ');
}

static void
tab(Biobuf *b)
{
	int c, nspace, pos;

	pos = 0;
	nspace = 0;
	while((c = Bgetrune(b)) != Beof) {
		switch(c) {
		case ' ':
			nspace++;
			pos++;
			continue;
		case '\t':
			nspace += nstop - pos % nstop;
			pos += nstop - pos % nstop;
			continue;
		}

		putspaces(pos-nspace, nspace);
		nspace = 0;
		Bputrune(&bout, c);

		if(c == '\n')
			pos = 0;
		else
			pos++;
	}
}

void
usage(void)
{
	fprint(2, "usage: tab [-n tabstop] [-u] [file...]\n");
	exits("usage");
}

void
main(int argc, char **argv)
{
	int i, err;
	Biobuf *b, bin;
	void (*cvt)(Biobuf *b);

	cvt = tab;
	ARGBEGIN{
	case 'm':
		minspace = atoi(ARGF());
		break;
	case 'n':
		nstop = atoi(ARGF());
		break;
	case 'u':
		cvt = untab;
		break;
	default:
		usage();
	}ARGEND;

	if(minspace == 0)
		minspace = 2;

	Binit(&bout, 1, OWRITE);
	if(argc > 0) {
		err = 0;
		for(i=0; i<argc; i++) {
			b = Bopen(argv[i], OREAD);
			if(b == nil) {
				fprint(2, "error opening %s: %r\n", argv[i]);
				err = 1;
				continue;
			}
			cvt(b);
			Bterm(b);
		}
		if(err)
			exits("errors");
		exits(0);
	} else {
		Binit(&bin, 0, OREAD);
		cvt(&bin);
		exits(0);
	}
}
