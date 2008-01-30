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

/*** KEY TRANSLATION ***/

/* FIXME: add these
      return SWFDEC_KEY_CLEAR;
      return SWFDEC_KEY_HELP;
      return SWFDEC_KEY_GRAVE;
      return SWFDEC_KEY_APOSTROPHE;
      return SWFDEC_KEY_F13;
      return SWFDEC_KEY_F14;
      return SWFDEC_KEY_F15;
*/
SwfdecKey
swfdec_dfb_key_from_identifier (DFBInputDeviceKeyIdentifier id)
{
  switch (id) {
    case DIKI_A:
      return SWFDEC_KEY_A;
    case DIKI_B:
      return SWFDEC_KEY_B;
    case DIKI_C:
      return SWFDEC_KEY_C;
    case DIKI_D:
      return SWFDEC_KEY_D;
    case DIKI_E:
      return SWFDEC_KEY_E;
    case DIKI_F:
      return SWFDEC_KEY_F;
    case DIKI_G:
      return SWFDEC_KEY_G;
    case DIKI_H:
      return SWFDEC_KEY_H;
    case DIKI_I:
      return SWFDEC_KEY_I;
    case DIKI_J:
      return SWFDEC_KEY_J;
    case DIKI_K:
      return SWFDEC_KEY_K;
    case DIKI_L:
      return SWFDEC_KEY_L;
    case DIKI_M:
      return SWFDEC_KEY_M;
    case DIKI_N:
      return SWFDEC_KEY_N;
    case DIKI_O:
      return SWFDEC_KEY_O;
    case DIKI_P:
      return SWFDEC_KEY_P;
    case DIKI_Q:
      return SWFDEC_KEY_Q;
    case DIKI_R:
      return SWFDEC_KEY_R;
    case DIKI_S:
      return SWFDEC_KEY_S;
    case DIKI_T:
      return SWFDEC_KEY_T;
    case DIKI_U:
      return SWFDEC_KEY_U;
    case DIKI_V:
      return SWFDEC_KEY_V;
    case DIKI_W:
      return SWFDEC_KEY_W;
    case DIKI_X:
      return SWFDEC_KEY_X;
    case DIKI_Y:
      return SWFDEC_KEY_Y;
    case DIKI_Z:
      return SWFDEC_KEY_Z;
    case DIKI_0:
      return SWFDEC_KEY_0;
    case DIKI_1:
      return SWFDEC_KEY_1;
    case DIKI_2:
      return SWFDEC_KEY_2;
    case DIKI_3:
      return SWFDEC_KEY_3;
    case DIKI_4:
      return SWFDEC_KEY_4;
    case DIKI_5:
      return SWFDEC_KEY_5;
    case DIKI_6:
      return SWFDEC_KEY_6;
    case DIKI_7:
      return SWFDEC_KEY_7;
    case DIKI_8:
      return SWFDEC_KEY_8;
    case DIKI_9:
      return SWFDEC_KEY_9;
    case DIKI_F1:
      return SWFDEC_KEY_F1;
    case DIKI_F2:
      return SWFDEC_KEY_F2;
    case DIKI_F3:
      return SWFDEC_KEY_F3;
    case DIKI_F4:
      return SWFDEC_KEY_F4;
    case DIKI_F5:
      return SWFDEC_KEY_F5;
    case DIKI_F6:
      return SWFDEC_KEY_F6;
    case DIKI_F7:
      return SWFDEC_KEY_F7;
    case DIKI_F8:
      return SWFDEC_KEY_F8;
    case DIKI_F9:
      return SWFDEC_KEY_F9;
    case DIKI_F10:
      return SWFDEC_KEY_F10;
    case DIKI_F11:
      return SWFDEC_KEY_F11;
    case DIKI_F12:
      return SWFDEC_KEY_F12;
    case DIKI_SHIFT_L:
      return SWFDEC_KEY_SHIFT;
    case DIKI_SHIFT_R:
      return SWFDEC_KEY_SHIFT;
    case DIKI_CONTROL_L:
      return SWFDEC_KEY_CONTROL;
    case DIKI_CONTROL_R:
      return SWFDEC_KEY_CONTROL;
    case DIKI_ALT_L:
      return SWFDEC_KEY_ALT;
    case DIKI_ALT_R:
      return SWFDEC_KEY_ALT;
    case DIKI_META_L:
      return 0; /* FIXME */
    case DIKI_META_R:
      return 0; /* FIXME */
    case DIKI_SUPER_L:
      return 0; /* FIXME */
    case DIKI_SUPER_R:
      return 0; /* FIXME */
    case DIKI_HYPER_L:
      return 0; /* FIXME */
    case DIKI_HYPER_R:
      return 0; /* FIXME */
    case DIKI_CAPS_LOCK:
      return SWFDEC_KEY_CAPS_LOCK;
    case DIKI_NUM_LOCK:
      return SWFDEC_KEY_NUM_LOCK;
    case DIKI_SCROLL_LOCK:
      return 0; /* FIXME */
    case DIKI_ESCAPE:
      return SWFDEC_KEY_ESCAPE;
    case DIKI_LEFT:
      return SWFDEC_KEY_LEFT;
    case DIKI_RIGHT:
      return SWFDEC_KEY_RIGHT;
    case DIKI_UP:
      return SWFDEC_KEY_UP;
    case DIKI_DOWN:
      return SWFDEC_KEY_DOWN;
    case DIKI_TAB:
      return SWFDEC_KEY_TAB;
    case DIKI_ENTER:
      return SWFDEC_KEY_ENTER;
    case DIKI_SPACE:
      return SWFDEC_KEY_SPACE;
    case DIKI_BACKSPACE:
      return SWFDEC_KEY_BACKSPACE;
    case DIKI_INSERT:
      return SWFDEC_KEY_INSERT;
    case DIKI_DELETE:
      return SWFDEC_KEY_DELETE;
    case DIKI_HOME:
      return SWFDEC_KEY_HOME;
    case DIKI_END:
      return SWFDEC_KEY_END;
    case DIKI_PAGE_UP:
      return SWFDEC_KEY_PAGE_UP;
    case DIKI_PAGE_DOWN:
      return SWFDEC_KEY_PAGE_DOWN;
    case DIKI_PRINT:
      return 0; /* FIXME */
    case DIKI_PAUSE:
      return 0; /* FIXME */
    case DIKI_QUOTE_LEFT:
      return 0; /* FIXME */
    case DIKI_MINUS_SIGN:
      return SWFDEC_KEY_MINUS;
    case DIKI_EQUALS_SIGN:
      return SWFDEC_KEY_EQUAL;
    case DIKI_BRACKET_LEFT:
      return SWFDEC_KEY_LEFT_BRACKET;
    case DIKI_BRACKET_RIGHT:
      return SWFDEC_KEY_RIGHT_BRACKET;
    case DIKI_BACKSLASH:
      return SWFDEC_KEY_BACKSLASH;
    case DIKI_SEMICOLON:
      return SWFDEC_KEY_SEMICOLON;
    case DIKI_QUOTE_RIGHT:
      return 0; /* FIXME */
    case DIKI_COMMA:
      return 0; /* FIXME */
    case DIKI_PERIOD:
      return 0; /* FIXME */
    case DIKI_SLASH:
      return SWFDEC_KEY_SLASH;
    case DIKI_LESS_SIGN:
      return 0; /* FIXME */
    case DIKI_KP_DIV:
      return SWFDEC_KEY_NUMPAD_DIVIDE;
    case DIKI_KP_MULT:
      return SWFDEC_KEY_NUMPAD_MULTIPLY;
    case DIKI_KP_MINUS:
      return SWFDEC_KEY_NUMPAD_SUBTRACT;
    case DIKI_KP_PLUS:
      return SWFDEC_KEY_NUMPAD_ADD;
    case DIKI_KP_ENTER:
      return SWFDEC_KEY_ENTER;
    case DIKI_KP_SPACE:
      return SWFDEC_KEY_SPACE;
    case DIKI_KP_TAB:
      return SWFDEC_KEY_TAB;
    case DIKI_KP_F1:
      return SWFDEC_KEY_F1;
    case DIKI_KP_F2:
      return SWFDEC_KEY_F2;
    case DIKI_KP_F3:
      return SWFDEC_KEY_F3;
    case DIKI_KP_F4:
      return SWFDEC_KEY_F4;
    case DIKI_KP_EQUAL:
      return SWFDEC_KEY_EQUAL;
    case DIKI_KP_SEPARATOR:
      return 0; /* FIXME */
    case DIKI_KP_DECIMAL:
      return SWFDEC_KEY_NUMPAD_DECIMAL;
    case DIKI_KP_0:
      return SWFDEC_KEY_NUMPAD_0;
    case DIKI_KP_1:
      return SWFDEC_KEY_NUMPAD_1;
    case DIKI_KP_2:
      return SWFDEC_KEY_NUMPAD_2;
    case DIKI_KP_3:
      return SWFDEC_KEY_NUMPAD_3;
    case DIKI_KP_4:
      return SWFDEC_KEY_NUMPAD_4;
    case DIKI_KP_5:
      return SWFDEC_KEY_NUMPAD_5;
    case DIKI_KP_6:
      return SWFDEC_KEY_NUMPAD_6;
    case DIKI_KP_7:
      return SWFDEC_KEY_NUMPAD_7;
    case DIKI_KP_8:
      return SWFDEC_KEY_NUMPAD_8;
    case DIKI_KP_9:
      return SWFDEC_KEY_NUMPAD_9;
    case DIKI_UNKNOWN:
    case DIKI_KEYDEF_END:
    case DIKI_NUMBER_OF_KEYS:
    default:
       return 0;
  }
}

enum {
  PROP_0,
  PROP_PLAYING,
  PROP_AUDIO,
  PROP_SPEED,
  PROP_DFB,
  PROP_HANDLE_EVENTS
};

/*** GTK-DOC ***/

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
    case PROP_HANDLE_EVENTS:
      g_value_set_boolean (value, swfdec_dfb_player_get_handle_events (player));
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
    case PROP_HANDLE_EVENTS:
      swfdec_dfb_player_set_handle_events ((player), g_value_get_boolean (value));
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
  swfdec_dfb_player_set_handle_events (player, FALSE);
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
  g_object_class_install_property (object_class, PROP_HANDLE_EVENTS,
      g_param_spec_boolean ("handle-events", "handle events", "wether player has a global event handler installed",
	  FALSE, G_PARAM_READWRITE));
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
  g_return_val_if_fail (SWFDEC_IS_DFB_PLAYER (player), 0.0);

  return player->speed;
}

/**
 * swfdec_dfb_player_handle_input_event:
 * @player: the player that receives the event
 * @event: the event to handle
 *
 * Provides an input event to the @player. The player handles keyboard and mouse 
 * events in a generic way. If you want to use a more complex mechanism, you 
 * have to write your own event handling code.
 *
 * Returns: %TRUE if the event was handled, %FALSE otherwise.
 **/
gboolean
swfdec_dfb_player_handle_input_event (SwfdecDfbPlayer *player, const DFBInputEvent *event)
{
  g_return_val_if_fail (SWFDEC_IS_DFB_PLAYER (player), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  switch (event->type) {
    case DIET_KEYPRESS:
      {
	SwfdecKey key = swfdec_dfb_key_from_identifier (event->key_id);
	if (key == 0) {
	  g_printerr ("find the key for keycode %u", event->key_id);
	} else {
	  swfdec_player_key_press (SWFDEC_PLAYER (player), key, event->key_symbol);
	}
      }
      break;
    case DIET_KEYRELEASE:
      {
	SwfdecKey key = swfdec_dfb_key_from_identifier (event->key_id);
	if (key == 0) {
	  g_printerr ("find the key for keycode %u", event->key_id);
	} else {
	  swfdec_player_key_release (SWFDEC_PLAYER (player), key, event->key_symbol);
	}
      }
      break;
    case DIET_BUTTONPRESS:
      swfdec_player_mouse_press (SWFDEC_PLAYER (player), 
	  player->x, player->y, event->button + 1);
      break;
    case DIET_BUTTONRELEASE:
      swfdec_player_mouse_release (SWFDEC_PLAYER (player), 
	  player->x, player->y, event->button + 1);
      break;
    case DIET_AXISMOTION:
      if (event->axis == DIAI_X) {
	player->x = event->axisabs;
      } else if (event->axis == DIAI_Y) {
	player->y = event->axisabs;
      } else {
	return FALSE;
      }
      /* only emit mouse movements if no new mouse move event follows */
      if ((event->flags & DIEF_FOLLOW) == 0)
	swfdec_player_mouse_move (SWFDEC_PLAYER (player), player->x, player->y);
      break;
    case DIET_UNKNOWN:
    default:
      return FALSE;
  }
  return TRUE;
}

static gboolean
swfdec_dfb_player_event_io (GIOChannel *channel, GIOCondition condition, gpointer player)
{
  gsize i, n;
  DFBEvent events[32];

  if (g_io_channel_read_chars (channel, (gchar *) events, sizeof (events), &n, NULL) != G_IO_STATUS_NORMAL)
    return TRUE;

  n /= sizeof (DFBEvent);

  for (i = 0; i < n; i++) {
    if (events[i].clazz != DFEC_INPUT)
      continue;

    swfdec_dfb_player_handle_input_event (player, (DFBInputEvent *) &events[i]);
  }

  /* FIXME: need to reset here? */
  return TRUE;
}

gboolean
swfdec_dfb_player_set_handle_events (SwfdecDfbPlayer *player, gboolean handle_events)
{
  g_return_val_if_fail (SWFDEC_IS_DFB_PLAYER (player), FALSE);

  if (swfdec_dfb_player_get_handle_events (player) == handle_events)
    return TRUE;

  if (handle_events) {
    GIOChannel *channel;
    int fd;

    if (player->dfb->CreateInputEventBuffer (player->dfb, DICAPS_ALL, DFB_FALSE, &player->events))
      return FALSE;
    if (player->events->CreateFileDescriptor (player->events, &fd)) {
      player->events->Release (player->events);
      player->events = NULL;
      return FALSE;
    }
    
    channel = g_io_channel_unix_new (fd);
    g_io_channel_set_encoding (channel, NULL, NULL);
    g_io_channel_set_buffered (channel, FALSE);

    player->event_source = g_io_create_watch (channel, G_IO_IN);
    g_source_set_priority (player->event_source, G_PRIORITY_DEFAULT);
    g_source_set_can_recurse (player->event_source, TRUE);
    g_source_set_callback (player->event_source, (GSourceFunc) swfdec_dfb_player_event_io, player, NULL);
    g_source_attach (player->event_source, NULL);
    g_io_channel_unref (channel);
  } else {
    g_source_destroy (player->event_source);
    g_source_unref (player->event_source);
    player->event_source = NULL;
    player->events->Release (player->events);
    player->events = NULL;
  }
  g_object_notify (G_OBJECT (player), "handle-events");
  return TRUE;
}

/**
 * swfdec_dfb_player_get_handle_events:
 * @player: the player to query
 *
 * Queries if the given @player has a global event handler installed. See 
 * swfdec_dfb_player_set_handle_events() for what that means.
 *
 * Returns: %TRUE if a global event handler is installed
 **/
gboolean
swfdec_dfb_player_get_handle_events (SwfdecDfbPlayer *player)
{
  g_return_val_if_fail (SWFDEC_IS_DFB_PLAYER (player), FALSE);

  return player->events != NULL;
}

