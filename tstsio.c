/* tstsio.c - test synchronous i/o */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sio.h"

double rate = 48000;            // sampling rate

void
save_result(float *a, int ns, int cnt, double sr, char *dn)
{
    float  aab, apk = 0, tpk;
    int i, ipk = 0;
    FILE *fp;
    static char *fn = "tstsio.txt";

    for (i = 0; i < ns; i++) {
        aab = (float)fabs(a[i]);
        if (apk < aab) {
            apk = aab;
            ipk = i;
        }
    }
    tpk = ipk * 1000 / (float) sr;
    printf("impulse response peak at %.2f ms\n", tpk);
    fp = fopen(fn, "wt");
    if (fp) {
        fprintf(fp, "; %s\n", fn);
        fprintf(fp, ";\n");
        fprintf(fp, "tlabel=I/O device: %s\n", dn);
	fprintf(fp, "size=%d\n", ns);
	fprintf(fp, "count=%d\n", cnt);
	fprintf(fp, "rate=%.0f\n", sr);
	fprintf(fp, "tpk=%.3f\n", tpk);
        fprintf(fp, ";\n");
	for (i = 0; i < ns; i++) {
	    fprintf(fp, "%9.3f\n", a[i] * 1000);
	}
        fclose(fp);
    }
}

void 
device_list()
{
    char    msg[80], *select;
    int     curdev, i, j;

    printf("-- sio device list --\n");
    curdev = sio_set_device(-1);	// get current device number
    for (i = 1; i < 30; i++) {
	j = sio_set_device(i);
	if (j != i) {
	    if (i == 1) {
		printf("No devices!\n");
	    }
	    break;
	}
	select = (i == curdev) ? ">" : " ";
        sio_get_device(msg);
	printf("%s %2d: %s\n", select, i, msg);
    }
}

void
usage()
{
    printf("usage: tstio [-option] [device_number]\n");
    printf("options:\n");
    printf("-h   print usage\n");
    printf("-l   list devices\n");
    printf("-rN  set sampling rate to N\n");
    printf("-v   version\n");
    printf("\n");
    printf("Note: soundcard should be in loopback configuration.\n");
    exit(0);
}

int
tstsio(int ac, char **av)
{
    char devnam[40];
    float *inn[2], *out[2], *avg[2];
    int devnum, cnt, inc[2];
    int nsk = 0;                    // number of samples skipped
    int nsw = 100;                  // number of averages
    int nav = 100;                  // number of averages
    int nrj = 100;                  // number of rejects
    int nbf = 1;                    // number of buffers per channel
    int nch = 2;                    // number of channels
    int nsm = 4800;                 // number of samples in buffers

    if (ac > 1) {
        if (av[1][0] == '-') {
            if (av[1][1] == 'h') {
                usage();
            } else if (av[1][1] == 'l') {
		device_list();
		return (0);
            } else if (av[1][1] == 'r') {
                rate = atof(&av[1][2]);
            } else if (av[1][1] == 'v') {
                printf("%s\n", sio_version());
                exit(0);
            }
	} else {
	    devnum = atoi(av[1]);
	    sio_set_device(devnum);
	}
    }
    inc[0] = inc[1] = 1;
    inn[0] = inn[1] = NULL;                         // unused input buffers
    out[1] = avg[1] = NULL;                         // unused out & avg buffers
    avg[0] = (float *) calloc(nsm, sizeof(float));  // average buffer
    out[0] = (float *) calloc(nsm, sizeof(float));  // stimulus buffer
    out[0][0] = 1;                                  // click amplitude = 1V
    if (sio_open()) {				    // open the sio device
	sio_get_device(devnam);                     // get device name
	printf("i/o device: %s\n", devnam);
	sio_set_rate(rate);                         // set sampling rate
	sio_set_size(nsm, 0, 0);                    // set buffer size
	sio_set_input(nch, nbf, inc, inn);          // specify input buffers
	sio_set_output(nch, nbf, inc, out);         // specify output buffers
	sio_set_average(avg, &cnt);                 // specify average buffers
	sio_io(nsk, nsw, nav, nrj);                 // initiate i/o
	sio_close();                                // close sio device
	save_result(avg[0], nsm, cnt, rate, devnam);
    } else {
	printf("can't open i/o device\n");
    }
    free(avg[0]);
    free(out[0]);
    return (0);
}

int 
main(int ac, char **av)
{
    tstsio(ac, av);
    return (0);
}

