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

#include "swfdec-directfb/swfdec_directfb_player.h"
#include "swfdec-directfb/swfdec_source.h"

enum {
  PROP_0,
  PROP_PLAYING,
  PROP_AUDIO,
  PROP_SPEED,
  PROP_DFB,
  PROP_HANDLE_EVENTS
};

/*** dfb-doc ***/

/**
 * SECTION:SwfdecDfbPlayer
 * @title: SwfdecDfbPlayer
 * @short_description: an improved #SwfdecPlayer
 *
 * The #SwfdecDfbPlayer adds common functionality to the rather barebones 
 * #SwfdecPlayer class, such as automatic playback and audio handling. Note 
 * that by default, the player will be paused, so you need to call 
 * swfdec_dfb_player_set_playing () on the new player.
 *
 * @see_also: SwfdecPlayer
 */

/**
 * SwfdecDfbPlayer:
 *
 * The structure for the Swfdec Dfb player contains no public fields.
 */

/*** SWFDEC_DFB_PLAYER ***/

G_DEFINE_TYPE (SwfdecDfbPlayer, swfdec_dfb_player, SWFDEC_TYPE_PLAYER)

static void
swfdec_dfb_player_get_property (GObject *object, guint param_id, GValue *value, 
    GParamSpec * pspec)
{
  SwfdecDfbPlayer *player = SWFDEC_DFB_PLAYER (object);
  
  switch (param_id) {
    case PROP_PLAYING:
      g_value_set_boolean (value, player->source != NULL);
      break;
    case PROP_AUDIO:
      g_value_set_boolean (value, player->audio_enabled);
      break;
    case PROP_SPEED:
      g_value_set_double (value, player->speed);
      break;
    case PROP_DFB:
      g_value_set_pointer (value, player->dfb);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
      break;
  }
}

static void
swfdec_dfb_player_set_property (GObject *object, guint param_id, const GValue *value,
    GParamSpec *pspec)
{
  SwfdecDfbPlayer *player = SWFDEC_DFB_PLAYER (object);
  
  switch (param_id) {
    case PROP_PLAYING:
      swfdec_dfb_player_set_playing (player, g_value_get_boolean (value));
      break;
    case PROP_AUDIO:
      swfdec_dfb_player_set_audio_enabled (player, g_value_get_boolean (value));
      break;
    case PROP_SPEED:
      swfdec_dfb_player_set_speed (player, g_value_get_double (value));
      break;
    case PROP_DFB:
      player->dfb = g_value_get_pointer (value);
      g_return_if_fail (player->dfb); 
      player->dfb->AddRef (player->dfb);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
      break;
  }
}

static void
swfdec_dfb_player_dispose (GObject *object)
{
  SwfdecDfbPlayer *player = SWFDEC_DFB_PLAYER (object);

  swfdec_dfb_player_set_playing (player, FALSE);
  if (player->dfb) {
    player->dfb->Release (player->dfb);
    player->dfb = NULL;
  }

  G_OBJECT_CLASS (swfdec_dfb_player_parent_class)->dispose (object);
}

static void
swfdec_dfb_player_class_init (SwfdecDfbPlayerClass * g_class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (g_class);

  object_class->dispose = swfdec_dfb_player_dispose;
  object_class->get_property = swfdec_dfb_player_get_property;
  object_class->set_property = swfdec_dfb_player_set_property;

  g_object_class_install_property (object_class, PROP_PLAYING,
      g_param_spec_boolean ("playing", "playing", "TRUE if the player is playing (d'oh)",
	  FALSE, G_PARAM_READWRITE));
  g_object_class_install_property (object_class, PROP_AUDIO,
      g_param_spec_boolean ("audio-enabled", "audio enabled", "TRUE if automatic audio handling is enabled",
	  TRUE, G_PARAM_READWRITE));
  g_object_class_install_property (object_class, PROP_SPEED,
      g_param_spec_double ("speed", "speed", "desired playback speed",
	  G_MINDOUBLE, G_MAXDOUBLE, 1.0, G_PARAM_READWRITE));
  g_object_class_install_property (object_class, PROP_DFB,
      g_param_spec_pointer ("directfb", "directfb", "The directfb object this player operates on",
	  G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static void
swfdec_dfb_player_init (SwfdecDfbPlayer * player)
{
  player->speed = 1.0;
  player->audio_enabled = TRUE;
}

/*** PUBLIC API ***/

/**
 * swfdec_dfb_player_new:
 * @debugger: %NULL or a #SwfdecAsDebugger to debug this player
 *
 * Creates a new Swfdec Dfb player.
 * This function calls swfdec_init () for you if it wasn't called before.
 *
 * Returns: The new player
 **/
SwfdecPlayer *
swfdec_dfb_player_new (IDirectFB *dfb, SwfdecAsDebugger *debugger)
{
  SwfdecPlayer *player;

  g_return_val_if_fail (dfb != NULL, NULL);

  swfdec_init ();
  player = g_object_new (SWFDEC_TYPE_DFB_PLAYER, "directfb", dfb, 
      "loader-type", SWFDEC_TYPE_FILE_LOADER, "socket-type", SWFDEC_TYPE_SOCKET,
      "debugger", debugger, 
      NULL);

  return player;
}

/**
 * swfdec_dfb_player_set_playing:
 * @player: a #SwfdecDfbPlayer
 * @playing: if the player should play or not
 *
 * Sets if @player should be playing or not. If the player is playing, a timer 
 * will be attached to the default main loop that periodically calls 
 * swfdec_player_advance().
 **/
void
swfdec_dfb_player_set_playing (SwfdecDfbPlayer *player, gboolean playing)
{
  g_return_if_fail (SWFDEC_IS_DFB_PLAYER (player));

  if (playing && player->source == NULL) {
    player->source = swfdec_iterate_source_new (SWFDEC_PLAYER (player), player->speed);
    g_source_attach (player->source, NULL);
  } else if (!playing && player->source != NULL) {
    g_source_destroy (player->source);
    g_source_unref (player->source);
    player->source = NULL;
  }
  g_object_notify (G_OBJECT (player), "playing");
}

/**
 * swfdec_dfb_player_get_playing:
 * @player: a #SwfdecDfbPlayer
 *
 * Queries if the player is playing.
 *
 * Returns: %TRUE if the player is playing
 **/
gboolean
swfdec_dfb_player_get_playing (SwfdecDfbPlayer *player)
{
  g_return_val_if_fail (SWFDEC_IS_DFB_PLAYER (player), FALSE);

  return player->source != NULL;
}

/**
 * swfdec_dfb_player_set_audio_enabled:
 * @player: a #SwfdecDfbPlayer
 * @enabled: %TRUE to enable audio
 *
 * Enables or disables automatic audio playback.
 **/
void
swfdec_dfb_player_set_audio_enabled (SwfdecDfbPlayer *player, gboolean enabled)
{
  g_return_if_fail (SWFDEC_IS_DFB_PLAYER (player));

  if (player->audio_enabled == enabled)
    return;
  player->audio_enabled = enabled;
  g_object_notify (G_OBJECT (player), "audio-enabled");
}

/**
 * swfdec_dfb_player_get_audio_enabled:
 * @player: a #SwfdecDfbPlayer
 *
 * Queries if audio playback for @player is enabled or not.
 *
 * Returns: %TRUE if audio playback is enabled
 **/
gboolean
swfdec_dfb_player_get_audio_enabled (SwfdecDfbPlayer *player)
{
  g_return_val_if_fail (SWFDEC_IS_DFB_PLAYER (player), FALSE);

  return player->audio_enabled;
}

/**
 * swfdec_dfb_player_set_speed:
 * @player: a #SwfdecDfbPlayer
 * @speed: the new playback speed
 *
 * Sets the new playback speed. 1.0 means the default speed, bigger values
 * make playback happen faster. Audio will only play back if the speed is 
 * 1.0. Note that if the player is not playing when calling this function,
 * it will not automatically start.
 **/
void
swfdec_dfb_player_set_speed (SwfdecDfbPlayer *player, double speed)
{
  g_return_if_fail (SWFDEC_IS_DFB_PLAYER (player));
  g_return_if_fail (speed > 0.0);

  player->speed = speed;
  if (player->source)
    swfdec_iterate_source_set_speed (player->source, player->speed);
  g_object_notify (G_OBJECT (player), "speed");
}

/**
 * swfdec_dfb_player_get_speed:
 * @player: a #SwfdecDfbPlayer
 *
 * Queries the current playback speed. If the player is currently paused, it
 * will report the speed that it would use if playing.
 *
 * Returns: the current playback speed.
 **/
double
swfdec_dfb_player_get_speed (SwfdecDfbPlayer *player)
{
  g_return_val_if_fail (SWFDEC_IS_DFB_PLAYER (player), FALSE);

  return player->speed;
}

