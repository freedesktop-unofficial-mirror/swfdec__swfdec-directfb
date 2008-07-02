/* Swfdec
 * Copyright (C) 2007-2008 Benjamin Otte <otte@gnome.org>
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
  if (renderer->repaint_area) {
    g_array_free (renderer->repaint_area, TRUE);
    renderer->repaint_area = NULL;
  }
  if (renderer->repaint_id) {
    g_source_remove (renderer->repaint_id);
    renderer->repaint_id = 0;
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
  renderer->repaint_area = g_array_new (FALSE, FALSE, sizeof (SwfdecRectangle));
}

/*** PUBLIC API ***/

static gboolean
swfdec_dfb_renderer_do_render (gpointer rendererp)
{
  SwfdecDfbRenderer *renderer = rendererp;

  renderer->repaint_id = 0;
  swfdec_dfb_renderer_render (renderer, (const SwfdecRectangle *) renderer->repaint_area->data,
      renderer->repaint_area->len);
  g_array_set_size (renderer->repaint_area, 0);
  return FALSE;
}

static void
swfdec_dfb_renderer_queue_repaint (SwfdecDfbRenderer *renderer, 
    const SwfdecRectangle *rects, guint n_rects)
{
  if (!renderer->repaint_id) {
    renderer->repaint_id = g_idle_add (swfdec_dfb_renderer_do_render, renderer);
  }
  g_array_append_vals (renderer->repaint_area, rects, n_rects);
}

SwfdecDfbRenderer *
swfdec_dfb_renderer_new (IDirectFB *dfb, IDirectFBSurface *surface,
    SwfdecPlayer *player)
{
  SwfdecDfbRenderer *renderer;
  SwfdecRectangle rect;

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
  g_signal_connect_swapped (player, "invalidate", G_CALLBACK (swfdec_dfb_renderer_queue_repaint), renderer);
  rect.x = rect.y = 0;
  rect.width = renderer->width;
  rect.height = renderer->height;
  swfdec_dfb_renderer_queue_repaint (renderer, &rect, 1);

  return renderer;
}

void
swfdec_dfb_renderer_render (SwfdecDfbRenderer *renderer, 
    const SwfdecRectangle *areas, guint n_areas)
{
  DFBRegion region;
  cairo_t *cr;
  guint i;

  g_return_if_fail (SWFDEC_IS_DFB_RENDERER (renderer));
  g_return_if_fail (n_areas > 0);

  cr = cairo_create (renderer->surface);
  /* FIXME: don't just accumulate, create a proper region here */
  region.x1 = areas->x;
  region.y1 = areas->y;
  region.x2 = areas->x + areas->width - 1;
  region.y2 = areas->y + areas->height - 1;
  for (i = 1; i < n_areas; i++) {
    region.x1 = MIN (region.x1, areas[i].x);
    region.y1 = MIN (region.y1, areas[i].y);
    region.x2 = MAX (region.x2, areas[i].x + areas[i].width - 1);
    region.y2 = MAX (region.y2, areas[i].y + areas[i].height - 1);
  }
  cairo_rectangle (cr, region.x1, region.y1, region.x2 + 1 - region.x1, region.y2 + 1 - region.y1);
  cairo_clip (cr);
  swfdec_player_render (renderer->player, cr);
  cairo_destroy (cr);
  renderer->dfbsurface->Flip (renderer->dfbsurface, &region, DSFLIP_ONSYNC);
}
