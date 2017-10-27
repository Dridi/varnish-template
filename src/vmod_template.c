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

#include <cache/cache.h>
#include <vdef.h>
#include <vcl.h>

#include "vcc_template_if.h"
#include "VSC_template.h"

static pthread_mutex_t mtx;
static struct VSC_template *vsc_template;

int __match_proto__(vmod_event_f)
event(VRT_CTX, struct vmod_priv *priv, enum vcl_event_e evt)
{

	CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
	AN(priv);

	switch (evt) {
	case VCL_EVENT_LOAD:
		AZ(pthread_mutex_init(&mtx, NULL));
		AZ(vsc_template);
		vsc_template = VSC_template_New("");
		AN(vsc_template);
		break;
	case VCL_EVENT_DISCARD:
		AZ(pthread_mutex_destroy(&mtx));
		AN(vsc_template);
		VSC_template_Destroy(&vsc_template);
		break;
	default:
		break;
	}

	return (0);
}

VCL_VOID __match_proto__(td_template_notice_me)
vmod_notice_me(VRT_CTX)
{

	CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
	AN(ctx->vsl);
	VSLb(ctx->vsl, SLT_VCL_Log, PACKAGE_STRING " was here!");
	AZ(pthread_mutex_lock(&mtx));
	if (ctx->http_req != NULL)
		vsc_template->client++;
	if (ctx->http_bereq != NULL)
		vsc_template->backend++;
	AZ(pthread_mutex_unlock(&mtx));
}
