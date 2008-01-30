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
#include <swfdec/swfdec.h>
#include <swfdec-directfb/swfdec-directfb.h>
#include <directfb.h>
#include <directfb/directfb_strings.h>
#include <stdio.h>

#define ERROR_CHECK(x) G_STMT_START{ \
  DFBResult err = (x); \
  if (err != DFB_OK) { \
    DirectFBErrorFatal ("fatal error", err); \
  } \
}G_STMT_END

static DFBSurfacePixelFormat pixel_format = DSPF_UNKNOWN;

static gboolean
parse_pixel_format (const gchar *option_name, const gchar *value, gpointer data,
    GError **error)
{
  static const DirectFBPixelFormatNames (names)
  guint i;

  for (i = 0; i < G_N_ELEMENTS (names); i++) {
    if (g_ascii_strcasecmp (names[i].name, value) == 0) {
      pixel_format = names[i].format;
      return TRUE;
    }
  }
  g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
      "%s is not a valid pixel format", value);
  return FALSE;
}

int 
main (int argc, char *argv[])
{
  GError *error = NULL;
  GOptionContext *ctx;
  DFBSurfaceDescription dsc;
  SwfdecPlayer *player;
  SwfdecURL *url;
  SwfdecDfbRenderer *renderer;
  guint w, h;
  IDirectFB *dfb;
  IDirectFBSurface *surface;
  /* config variables */
  gboolean no_background = FALSE;
  char *size = NULL;
  int cache_size = 50 * 1024; /* 50 MB - Swfdec default value */
  int garbage_size = 8 * 1024; /* 8 MB - Swfdec default value */

  GOptionEntry options[] = {
    { "cache-size", 'c', 0, G_OPTION_ARG_INT, &cache_size, "Maxmimum amount of memory to be used as cache (default: 50MB)", "KB" },
    { "format", 'f', 0, G_OPTION_ARG_CALLBACK, parse_pixel_format, "pixel format to use for the outpt window", "FORMAT" },
    { "garbage-size", 'g', 0, G_OPTION_ARG_INT, &garbage_size, "Amount of kB before garbage collection runs (default: 8MB)", "KB" },
    { "no-background", 'b', 0, G_OPTION_ARG_NONE, &no_background, "Disable rendering of the background (allows transparency)", NULL },
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
  
  ERROR_CHECK (DirectFBCreate (&dfb));
  player = swfdec_dfb_player_new (dfb, NULL);
  g_object_set (player, "cache-size", (gulong) cache_size * 1024, 
      "memory-until-gc", (gulong) garbage_size * 1024, NULL);
  if (no_background)
    g_object_set (player, "background-color", 0, NULL);

  dsc.flags = DSDESC_CAPS;
  dsc.caps = DSCAPS_DOUBLE | DSCAPS_PRIMARY;
  if (pixel_format != DSPF_UNKNOWN) {
    dsc.flags |= DSDESC_PIXELFORMAT;
    dsc.pixelformat = pixel_format;
  }
  if (size) {
    if (g_ascii_strcasecmp (size, "fullscreen") == 0) {
      ERROR_CHECK (dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN));
    } else if (sscanf (size, "%ux%u", &w, &h) == 2) {
      dsc.flags |= DSDESC_WIDTH | DSDESC_HEIGHT;
      dsc.width = w;
      dsc.height = h;
    } else {
      g_printerr ("invalid argument for --size specified\n");
      g_object_unref (player);
      return 1;
    }
  } else {
    ERROR_CHECK (dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN));
  }

  ERROR_CHECK (dfb->CreateSurface (dfb, &dsc, &surface));
  //ERROR_CHECK (surface->GetSize (surface, &dsc.width, &dsc.height));
  //swfdec_player_set_size (player, dsc.width, dsc.height);
  renderer = swfdec_dfb_renderer_new (dfb, surface, player);

  url = swfdec_url_new_from_input (argv[1]);
  swfdec_player_set_url (player, url);
  swfdec_url_free (url);
  swfdec_dfb_player_set_playing (SWFDEC_DFB_PLAYER (player), TRUE);
  swfdec_dfb_main ();

  g_object_unref (renderer);
  g_object_unref (player);
  surface->Release (surface);
  dfb->Release (dfb);
  return 0;
}

