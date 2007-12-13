/* Swfdec
 * Copyright (C) 2007 Benjamin Otte <otte@gnome.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <libswfdec/swfdec.h>
#include <swfdec-directfb/swfdec-directfb.h>
#include <directfb.h>
#include <cairo-directfb.h>
#include <stdio.h>

#define ERROR_CHECK(x) G_STMT_START{ \
  DFBResult err = (x); \
  if (err != DFB_OK) { \
    DirectFBErrorFatal ("fatal error", err); \
  } \
}G_STMT_END

typedef struct {
  IDirectFB *dfb;
  IDirectFBSurface *dfbsurface;
  cairo_surface_t *surface;
} Data;

static void
render (SwfdecPlayer *player, Data *data, guint x, guint y, guint w, guint h)
{
  /* region include the bottom right pixel */
  DFBRegion region = { x, y, x + w - 1, y + h - 1 };
  cairo_t *cr;

  cr = cairo_create (data->surface);
  swfdec_player_render (player, cr, x, y, w, h);
  cairo_destroy (cr);
  data->dfbsurface->Flip (data->dfbsurface, &region, DSFLIP_ONSYNC);
}

static void
invalidate (SwfdecPlayer *player, SwfdecRectangle *extents, SwfdecRectangle *rects, guint n_rects, Data *data)
{
  render (player, data, extents->x, extents->y, extents->width, extents->height);
}

int 
main (int argc, char *argv[])
{
  GError *error = NULL;
  GOptionContext *ctx;
  DFBSurfaceDescription dsc;
  SwfdecPlayer *player;
  guint w, h;
  Data data;
  /* config variables */
  char *size = NULL;

  GOptionEntry options[] = {
    { "size", 's', 0, G_OPTION_ARG_STRING, &size, "WIDTHxHEIGHT to specify a size or \'fullscreen\' to run fullscreen", "STRING" },
    { NULL }
  };

  ERROR_CHECK (DirectFBInit (&argc, &argv));
  swfdec_init ();

  ctx = g_option_context_new ("");
  g_option_context_add_main_entries (ctx, options, "options");
  g_option_context_parse (ctx, &argc, &argv, &error);
  g_option_context_free (ctx);

  if (error) {
    g_printerr ("Error parsing command line arguments: %s\n", error->message);
    g_error_free (error);
    return 1;
  }

  if (argc < 2) {
    g_printerr ("Usage: %s [OPTIONS] filename\n", argv[0]);
    return 1;
  }
  
  ERROR_CHECK (DirectFBCreate (&data.dfb));
  player = swfdec_dfb_player_new_from_file (data.dfb, argv[1]);

  if (size) {
    if (g_ascii_strcasecmp (size, "fullscreen") == 0) {
      ERROR_CHECK (data.dfb->SetCooperativeLevel (data.dfb, DFSCL_FULLSCREEN));
      dsc.flags = DSDESC_CAPS;
    } else if (sscanf (size, "%ux%u", &w, &h) == 2) {
      dsc.flags = DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT;
    } else {
      g_printerr ("invalid argument for --size specified\n");
      g_object_unref (player);
      return 1;
    }
  } else {
    dsc.flags = DSDESC_CAPS;
  }

  dsc.caps = DSCAPS_DOUBLE | DSCAPS_PRIMARY;
  dsc.width = w;
  dsc.height = h;
  ERROR_CHECK (data.dfb->CreateSurface (data.dfb, &dsc, &data.dfbsurface));
  data.surface = cairo_directfb_surface_create (data.dfb, data.dfbsurface);

  ERROR_CHECK (data.dfbsurface->GetSize (data.dfbsurface, &dsc.width, &dsc.height));
  swfdec_player_set_size (player, dsc.width, dsc.height);
  render (player, &data, 0, 0, dsc.width, dsc.height);

  g_signal_connect (player, "invalidate", G_CALLBACK (invalidate), &data);
  swfdec_dfb_player_set_playing (SWFDEC_DFB_PLAYER (player), TRUE);
  swfdec_dfb_main ();

  return 0;
}

