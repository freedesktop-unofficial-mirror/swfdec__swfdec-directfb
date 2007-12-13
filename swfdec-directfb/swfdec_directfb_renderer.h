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

#ifndef _SWFDEC_DFB_RENDERER_H_
#define _SWFDEC_DFB_RENDERER_H_

#include <libswfdec/swfdec.h>
#include <directfb.h>

G_BEGIN_DECLS

typedef struct _SwfdecDfbRenderer SwfdecDfbRenderer;
typedef struct _SwfdecDfbRendererClass SwfdecDfbRendererClass;

#define SWFDEC_TYPE_DFB_RENDERER                    (swfdec_dfb_renderer_get_type())
#define SWFDEC_IS_DFB_RENDERER(obj)                 (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SWFDEC_TYPE_DFB_RENDERER))
#define SWFDEC_IS_DFB_RENDERER_CLASS(klass)         (G_TYPE_CHECK_CLASS_TYPE ((klass), SWFDEC_TYPE_DFB_RENDERER))
#define SWFDEC_DFB_RENDERER(obj)                    (G_TYPE_CHECK_INSTANCE_CAST ((obj), SWFDEC_TYPE_DFB_RENDERER, SwfdecDfbRenderer))
#define SWFDEC_DFB_RENDERER_CLASS(klass)            (G_TYPE_CHECK_CLASS_CAST ((klass), SWFDEC_TYPE_DFB_RENDERER, SwfdecDfbRendererClass))
#define SWFDEC_DFB_RENDERER_GET_CLASS(obj)          (G_TYPE_INSTANCE_GET_CLASS ((obj), SWFDEC_TYPE_DFB_RENDERER, SwfdecDfbRendererClass))

struct _SwfdecDfbRenderer
{
  GObject *		object;

  IDirectFB *		dfb;		/* the DirectFB object we play on */
  SwfdecPlayer *	player;		/* the player we display */

  IDirectFBSurface *	dfbsurface;	/* surface we render to (reference held by cairo surface) */
  cairo_surface_t *	surface;	/* same in cairo terms (we reference it) */
  SwfdecRectangle	repaint_area;	/* rectange that needs repainting */
  guint			repaint_id;	/* ID of source for repainting or 0 if none */
  /* cached values */
  int			width;
  int			height;
};

struct _SwfdecDfbRendererClass
{
  GObjectClass		object_class;
};

GType			swfdec_dfb_renderer_get_type    	(void);

SwfdecDfbRenderer *	swfdec_dfb_renderer_new			(IDirectFB *		dfb,
								 IDirectFBSurface *	surface,
								 SwfdecPlayer *		player);

void			swfdec_dfb_renderer_render		(SwfdecDfbRenderer *	renderer,
							         int			x,
							         int			y,
							         int			width,
							         int			height);


G_END_DECLS
#endif
