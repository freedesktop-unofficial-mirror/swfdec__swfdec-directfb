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
#include <directfb.h>
#include <cairo-directfb.h>

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

  static const GOptionEntry options[] = {
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
  ERROR_CHECK (data.dfb->SetCooperativeLevel (data.dfb, DFSCL_NORMAL));

  player = swfdec_player_new_from_file (argv[1]);
  /* advance till initialized or fail */
  /* NB: This method only knows for file loading */
  while (!swfdec_player_is_initialized (player)) {
    long next_event = swfdec_player_get_next_event (player);
    if (next_event < 0) {
      g_object_unref (player);
      g_printerr ("\"%s\" is not a file Swfdec can play.\n", argv[1]);
      return 1;
    }
    swfdec_player_advance (player, next_event);
  }
  swfdec_player_get_default_size (player, &w, &h);

  dsc.flags = DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT;
  dsc.caps = DSCAPS_DOUBLE | DSCAPS_PRIMARY;
  dsc.width = w;
  dsc.height = h;
  ERROR_CHECK (data.dfb->CreateSurface (data.dfb, &dsc, &data.dfbsurface));
  data.surface = cairo_directfb_surface_create (data.dfb, data.dfbsurface);
  render (player, &data, 0, 0, w, h);
  g_signal_connect (player, "invalidate", G_CALLBACK (invalidate), &data);
  while (TRUE) {
    int next_event = swfdec_player_get_next_event (player);
    swfdec_player_advance (player, next_event);
  };

  return 0;
}

