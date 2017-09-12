/*-
 * Copyright (C) 2017  Dridi Boukelmoune
 * All rights reserved.
 *
 * Author: Dridi Boukelmoune <dridi.boukelmoune@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VOPT_DEFINITION
#define VOPT_INC "varnishtemplate_options.h"

#include <vapi/voptget.h>
#include <vapi/vsl.h>

#include <miniobj.h>
#include <vqueue.h>

#include <vas.h>
#include <vdef.h>
#include <vre.h>
#include <vut.h>

static struct VUT *vut;
static unsigned n_trans = 0;

static void __attribute__((__noreturn__))
usage(int status)
{
	const char **opt;

	fprintf(stderr, "Usage: %s <options>\n\n", vut->progname);
	fprintf(stderr, "Options:\n");
	for (opt = vopt_spec.vopt_usage; *opt != NULL; opt += 2)
		fprintf(stderr, " %-25s %s\n", *opt, *(opt + 1));
	exit(status);
}

static int __match_proto__(VSLQ_dispatch_f)
dispatch(struct VSL_data *vsl, struct VSL_transaction * const *pt, void *priv)
{
	struct VSL_transaction *t;
	enum VSL_tag_e tag;
	const char *str;
	uint32_t len;
	int i;

	AN(vsl);
	AN(pt);
	AZ(priv);

	for (t = *pt; t != NULL; t = *++pt) {
		if (t->type != VSL_t_req && t->type != VSL_t_bereq)
			continue;

		while (1) {
			i = VSL_Next(t->c);
			if (i < 0)
				return (i);
			if (i == 0)
				break;
			if (!VSL_Match(vsl, t->c))
				continue;
			tag = VSL_TAG(t->c->rec.ptr);
			if (tag != SLT_VCL_Log)
				continue;
			len = VSL_LEN(t->c->rec.ptr);
			str = VSL_CDATA(t->c->rec.ptr);
			if (len >= sizeof PACKAGE)
				len = sizeof PACKAGE - 1;
			if (!strncmp(PACKAGE, str, len)) {
				n_trans++;
				break;
			}
		}
	}

	return (0);
}

static void
sighandler(int sig)
{

	if (vut != NULL)
		VUT_Signaled(vut, sig);
}

int
main(int argc, char * const *argv)
{
	int opt;

	vut = VUT_InitProg(argc, argv, &vopt_spec);
	AN(vut);

	while ((opt = getopt(argc, argv, vopt_spec.vopt_optstring)) != -1) {
		switch (opt) {
		case 'h':
			usage(EXIT_SUCCESS);
			/* no return */
		case 'w':
			/* Write to file */
			INCOMPL();
			break;
		default:
			if (!VUT_Arg(vut, opt, optarg))
				usage(EXIT_FAILURE);
		}
	}

	if (optind != argc)
		usage(EXIT_FAILURE);

	vut->dispatch_f = dispatch;
	vut->dispatch_priv = NULL;

	VUT_Signal(sighandler);

	VUT_Setup(vut);
	(void)VUT_Main(vut);
	VUT_Fini(&vut);

	xxxassert(printf("%u\n", n_trans) > 0);
	XXXAZ(fflush(stdout));

	return (EXIT_SUCCESS);
}
