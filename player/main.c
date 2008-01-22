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
  /* config variables */
  char *size = NULL;
  int cache_size = 50 * 1024; /* 50 MB - Swfdec default value */
  int garbage_size = 8 * 1024; /* 8 MB - Swfdec default value */

  GOptionEntry options[] = {
    { "size", 's', 0, G_OPTION_ARG_STRING, &size, "WIDTHxHEIGHT to specify a size or \'fullscreen\' to run fullscreen", "STRING" },
    { "cache-size", 'c', 0, G_OPTION_ARG_INT, &cache_size, "Amount of kB allowed as cache", "KB" },
    { "garbage-size", 'g', 0, G_OPTION_ARG_INT, &garbage_size, "Amount of kB before garbage collection runs", "KB" },
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
  player = swfdec_dfb_player_new_from_file (dfb, argv[1]);
  g_object_set (player, "cache-size", (gulong) cache_size * 1024, 
      "memory-until-gc", (gulong) garbage_size * 1024, NULL);

  dsc.flags = DSDESC_CAPS;
  dsc.caps = DSCAPS_DOUBLE | DSCAPS_PRIMARY;
  if (size) {
    if (g_ascii_strcasecmp (size, "fullscreen") == 0) {
      ERROR_CHECK (dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN));
    } else if (sscanf (size, "%ux%u", &w, &h) == 2) {
      dsc.flags |= DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT;
      dsc.width = w;
      dsc.height = h;
    } else {
      g_printerr ("invalid argument for --size specified\n");
      g_object_unref (player);
      return 1;
    }
  }

  ERROR_CHECK (dfb->CreateSurface (dfb, &dsc, &surface));
  //ERROR_CHECK (surface->GetSize (surface, &dsc.width, &dsc.height));
  //swfdec_player_set_size (player, dsc.width, dsc.height);
  renderer = swfdec_dfb_renderer_new (dfb, surface, player);

  swfdec_dfb_player_set_playing (SWFDEC_DFB_PLAYER (player), TRUE);
  swfdec_dfb_main ();

  g_object_unref (renderer);
  g_object_unref (player);
  surface->Release (surface);
  dfb->Release (dfb);
  return 0;
}

