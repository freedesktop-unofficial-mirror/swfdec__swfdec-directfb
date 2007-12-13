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

#include "swfdec-directfb/swfdec_directfb_renderer.h"
#include <cairo-directfb.h>

G_DEFINE_TYPE (SwfdecDfbRenderer, swfdec_dfb_renderer, G_TYPE_OBJECT)

static void
swfdec_dfb_renderer_dispose (GObject *object)
{
  SwfdecDfbRenderer *renderer = SWFDEC_DFB_RENDERER (object);

  if (renderer->dfb) {
    renderer->dfb->Release (renderer->dfb);
    renderer->dfb = NULL;
  }
  if (renderer->surface) {
    cairo_surface_destroy (renderer->surface);
    renderer->surface = NULL;
    renderer->dfbsurface = NULL;
  }
  if (renderer->player) {
    g_object_unref (renderer->player);
    renderer->player = NULL;
  }

  G_OBJECT_CLASS (swfdec_dfb_renderer_parent_class)->dispose (object);
}

static void
swfdec_dfb_renderer_class_init (SwfdecDfbRendererClass * g_class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (g_class);

  object_class->dispose = swfdec_dfb_renderer_dispose;
}

static void
swfdec_dfb_renderer_init (SwfdecDfbRenderer * renderer)
{
}

/*** PUBLIC API ***/

static gboolean
swfdec_dfb_renderer_do_render (gpointer rendererp)
{
  SwfdecDfbRenderer *renderer = rendererp;

  renderer->repaint_id = 0;
  swfdec_dfb_renderer_render (renderer, 
      renderer->repaint_area.x, renderer->repaint_area.y,
      renderer->repaint_area.width, renderer->repaint_area.height);
  return FALSE;
}

static void
swfdec_dfb_renderer_queue_repaint (SwfdecDfbRenderer *renderer, const SwfdecRectangle *rect)
{
  if (renderer->repaint_id) {
    swfdec_rectangle_union (&renderer->repaint_area, &renderer->repaint_area, rect);
  } else {
    renderer->repaint_area = *rect;
    renderer->repaint_id = g_idle_add (swfdec_dfb_renderer_do_render, renderer);
  }
}

static void
swfdec_dfb_renderer_invalidate_cb (SwfdecPlayer *player, const SwfdecRectangle *extents,
    const SwfdecRectangle *rects, guint n_rects, SwfdecDfbRenderer *renderer)
{
  swfdec_dfb_renderer_queue_repaint (renderer, extents);
}

SwfdecDfbRenderer *
swfdec_dfb_renderer_new (IDirectFB *dfb, IDirectFBSurface *surface,
    SwfdecPlayer *player)
{
  SwfdecDfbRenderer *renderer;

  g_return_val_if_fail (dfb != NULL, NULL);
  g_return_val_if_fail (surface != NULL, NULL);
  g_return_val_if_fail (SWFDEC_IS_PLAYER (player), NULL);

  renderer = g_object_new (SWFDEC_TYPE_DFB_RENDERER, NULL);
  dfb->AddRef (dfb);
  renderer->dfb = dfb;
  renderer->dfbsurface = surface;
  renderer->surface = cairo_directfb_surface_create (dfb, surface);
  surface->GetSize (surface, &renderer->width, &renderer->height);
  renderer->player = g_object_ref (player);
  g_signal_connect (player, "invalidate", G_CALLBACK (swfdec_dfb_renderer_invalidate_cb), renderer);

  return renderer;
}

void
swfdec_dfb_renderer_render (SwfdecDfbRenderer *renderer, int x, int y,
    int width, int height)
{
  DFBRegion region = { x, y, x + width - 1, y + height - 1 };
  cairo_t *cr;

  g_return_if_fail (SWFDEC_IS_DFB_RENDERER (renderer));
  g_return_if_fail (x >= 0);
  g_return_if_fail (y >= 0);
  g_return_if_fail (width > 0);
  g_return_if_fail (height > 0);
  g_return_if_fail (x + width <= renderer->width);
  g_return_if_fail (y + height <= renderer->height);

  cr = cairo_create (renderer->surface);
  swfdec_player_render (renderer->player, cr, x, y, width, height);
  cairo_destroy (cr);
  renderer->dfbsurface->Flip (renderer->dfbsurface, &region, DSFLIP_ONSYNC);
}
