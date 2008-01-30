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

#ifndef _SWFDEC_DFB_PLAYER_H_
#define _SWFDEC_DFB_PLAYER_H_

#include <libswfdec/swfdec.h>
#include <directfb.h>

G_BEGIN_DECLS

typedef struct _SwfdecDfbPlayer SwfdecDfbPlayer;
typedef struct _SwfdecDfbPlayerClass SwfdecDfbPlayerClass;

#define SWFDEC_TYPE_DFB_PLAYER                    (swfdec_dfb_player_get_type())
#define SWFDEC_IS_DFB_PLAYER(obj)                 (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SWFDEC_TYPE_DFB_PLAYER))
#define SWFDEC_IS_DFB_PLAYER_CLASS(klass)         (G_TYPE_CHECK_CLASS_TYPE ((klass), SWFDEC_TYPE_DFB_PLAYER))
#define SWFDEC_DFB_PLAYER(obj)                    (G_TYPE_CHECK_INSTANCE_CAST ((obj), SWFDEC_TYPE_DFB_PLAYER, SwfdecDfbPlayer))
#define SWFDEC_DFB_PLAYER_CLASS(klass)            (G_TYPE_CHECK_CLASS_CAST ((klass), SWFDEC_TYPE_DFB_PLAYER, SwfdecDfbPlayerClass))
#define SWFDEC_DFB_PLAYER_GET_CLASS(obj)          (G_TYPE_INSTANCE_GET_CLASS ((obj), SWFDEC_TYPE_DFB_PLAYER, SwfdecDfbPlayerClass))

struct _SwfdecDfbPlayer
{
  SwfdecPlayer		player;

  IDirectFB *		dfb;		/* the DirectFB object we play on */

  /* properties */
  GSource *		source;		/* source if playing, NULL otherwise */
  gboolean		audio_enabled;	/* TRUE if audio should be played */
  double		speed;		/* desired playback speed */
};

struct _SwfdecDfbPlayerClass
{
  SwfdecPlayerClass   	player_class;
};

GType 		swfdec_dfb_player_get_type    	(void);

SwfdecPlayer *	swfdec_dfb_player_new		  	(IDirectFB *		dfb,
							 SwfdecAsDebugger *	debugger);

void		swfdec_dfb_player_set_playing	  	(SwfdecDfbPlayer *	player,
							 gboolean		playing);
gboolean	swfdec_dfb_player_get_playing	 	(SwfdecDfbPlayer *	player);
void		swfdec_dfb_player_set_audio_enabled   	(SwfdecDfbPlayer *	player,
							 gboolean		enabled);
gboolean	swfdec_dfb_player_get_audio_enabled	(SwfdecDfbPlayer *	player);
void		swfdec_dfb_player_set_speed		(SwfdecDfbPlayer *	player,
							 double			speed);
double		swfdec_dfb_player_get_speed		(SwfdecDfbPlayer *	player);

gboolean      	swfdec_dfb_player_handle_event		(SwfdecDfbPlayer *	player,
							 const DFBInputEvent *	event);
void		swfdec_dfb_player_set_handle_events	(SwfdecDfbPlayer *	player,
							 gboolean		handle_events);
gboolean      	swfdec_dfb_player_get_handle_events	(SwfdecDfbPlayer *	player);



G_END_DECLS
#endif
