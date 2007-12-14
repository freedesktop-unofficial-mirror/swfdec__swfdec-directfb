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
#include <swfdec-directfb/swfdec-directfb.h>
#include <directfb.h>
#include <stdio.h>

#define ERROR_CHECK(x) G_STMT_START{ \
  DFBResult err = (x); \
  if (err != DFB_OK) { \
    DirectFBErrorFatal ("fatal error", err); \
  } \
}G_STMT_END

int 
main (int argc, char *argv[])
{
  GError *error = NULL;
  GOptionContext *ctx;
  DFBSurfaceDescription dsc;
  SwfdecPlayer *player;
  SwfdecDfbRenderer *renderer;
  guint w, h;
  IDirectFB *dfb;
  IDirectFBSurface *surface;
  long next_event;
  GTimer *timer;
  int total = 0;
  /* config variables */
  int seconds = 10000;
  gboolean primary = FALSE;
  gboolean verbose = FALSE;

  GOptionEntry options[] = {
    { "primary", 'p', 0, G_OPTION_ARG_NONE, &primary, "use the primary display", NULL },
    { "time", 't', 0, G_OPTION_ARG_INT, &seconds, "number of milliseconds to advance (default: 10.000)", "mSECS" },
    { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "be verbose", NULL },
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
  
  ERROR_CHECK (DirectFBCreate (&dfb));
  player = swfdec_player_new_from_file (argv[1]);
  next_event = swfdec_player_get_next_event (player);
  while (next_event >= 0 && !swfdec_player_is_initialized (player)) {
    swfdec_player_advance (player, next_event);
    next_event = swfdec_player_get_next_event (player);
  }

  swfdec_player_get_default_size (player, &w, &h);
  dsc.flags = DSDESC_WIDTH | DSDESC_HEIGHT;
  dsc.width = w;
  dsc.height = h;
  if (primary) {
    dsc.flags |= DSDESC_CAPS;
    dsc.caps = DSCAPS_PRIMARY;
  }

  ERROR_CHECK (dfb->CreateSurface (dfb, &dsc, &surface));
  renderer = swfdec_dfb_renderer_new (dfb, surface, player);

  timer = g_timer_new ();
  while (g_main_context_iteration (NULL, FALSE));
  while (next_event >= 0 && total + next_event < seconds) {
    swfdec_player_advance (player, next_event);
    total += next_event;
    while (g_main_context_iteration (NULL, FALSE));

    if (verbose)
      g_print ("advanced %u.%03us in %.3gs\n", total / 1000, total % 1000, g_timer_elapsed (timer, NULL));
    next_event = swfdec_player_get_next_event (player);
  }
  g_timer_stop (timer);
  g_print ("advanced %u.%03us in %.3gs\n", seconds / 1000, seconds % 1000, g_timer_elapsed (timer, NULL));

  g_object_unref (renderer);
  g_object_unref (player);
  surface->Release (surface);
  dfb->Release (dfb);
  return 0;
}

