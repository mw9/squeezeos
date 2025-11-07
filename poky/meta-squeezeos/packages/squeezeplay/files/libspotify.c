#include <stdbool.h>
#include <libspotify/api.h>

SP_LIBEXPORT(sp_error) sp_session_create(const sp_session_config *config, sp_session **sess)
{
	return SP_ERROR_API_INITIALIZATION_FAILED;
}

SP_LIBEXPORT(sp_track *) sp_albumbrowse_track(sp_albumbrowse *alb, int index)
{
	return NULL;
}

SP_LIBEXPORT(void) sp_track_set_starred(sp_session *session, const sp_track **tracks, int num_tracks, bool star)
{
	return;
}

SP_LIBEXPORT(void) sp_track_release(sp_track *track)
{
	return;
}

SP_LIBEXPORT(sp_error) sp_albumbrowse_error(sp_albumbrowse *alb)
{
	return SP_ERROR_API_INITIALIZATION_FAILED;
}

SP_LIBEXPORT(void) sp_session_player_unload(sp_session *session)
{
	return;
}

SP_LIBEXPORT(sp_error) sp_session_logout(sp_session *session)
{
	return SP_ERROR_OK;
}

SP_LIBEXPORT(int) sp_albumbrowse_num_tracks(sp_albumbrowse *alb)
{
	return 0;
}

SP_LIBEXPORT(sp_track *) sp_link_as_track(sp_link *link)
{
	return NULL;
}

SP_LIBEXPORT(sp_albumbrowse *) sp_albumbrowse_create(sp_session *session, sp_album *album, albumbrowse_complete_cb *callback, void *userdata)
{
	return NULL;
}

SP_LIBEXPORT(void) sp_session_process_events(sp_session *session, int *next_timeout)
{
	return;
}

SP_LIBEXPORT(void) sp_track_add_ref(sp_track *track)
{
	return;
}

SP_LIBEXPORT(sp_linktype) sp_link_type(sp_link *link)
{
	return SP_LINKTYPE_INVALID;
}

SP_LIBEXPORT(bool) sp_track_is_loaded(sp_track *track)
{
	return false;
}

SP_LIBEXPORT(sp_error) sp_session_player_seek(sp_session *session, int offset)
{
	return SP_ERROR_API_INITIALIZATION_FAILED;
}

SP_LIBEXPORT(sp_link *) sp_link_create_from_string(const char *link)
{
	return NULL;
}

SP_LIBEXPORT(sp_error) sp_session_player_load(sp_session *session, sp_track *track)
{
	return SP_ERROR_API_INITIALIZATION_FAILED;
}

SP_LIBEXPORT(void) sp_albumbrowse_release(sp_albumbrowse *alb)
{
	return;
}

SP_LIBEXPORT(sp_error) sp_session_login(sp_session *session, const char *username, const char *password)
{
	return SP_ERROR_API_INITIALIZATION_FAILED;
}

const char message[] = "Unknown error";

SP_LIBEXPORT(const char*) sp_error_message(sp_error error)
{
	return message;
}

SP_LIBEXPORT(sp_error) sp_session_player_play(sp_session *session, bool play)
{
	return SP_ERROR_API_INITIALIZATION_FAILED;
}

SP_LIBEXPORT(sp_connectionstate) sp_session_connectionstate(sp_session *session)
{
	return SP_CONNECTION_STATE_UNDEFINED;
}

SP_LIBEXPORT(sp_user *) sp_session_user(sp_session *session)
{
	return NULL;
}

SP_LIBEXPORT(sp_album *) sp_link_as_album(sp_link *link)
{
	return NULL;
}

SP_LIBEXPORT(bool) sp_track_is_available(sp_session *session, sp_track *track)
{
	return false;
}

SP_LIBEXPORT(void) sp_link_release(sp_link *link)
{
	return;
}

const char empty[] = "";

SP_LIBEXPORT(const char *) sp_track_name(sp_track *track)
{
	return empty;
}

const char username[] = "User Name";

SP_LIBEXPORT(const char *) sp_user_canonical_name(sp_user *user)
{
	return username;
}

SP_LIBEXPORT(void) sp_session_preferred_bitrate(sp_session *session, sp_bitrate bitrate)
{
	return;
}
