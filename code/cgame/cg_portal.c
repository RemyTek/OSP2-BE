/*
===========================================================================
cg_portal.c -- Threewave portal slot configstring handling.

Reads portal slot configstrings sent by the Threewave 1.7 server
(CS index = slot + CS_TW_PORTAL_BASE, 0-indexed) and remaps the
corresponding in-world portal surface texture to the destination
map's levelshot via trap_R_RemapShader.

Preview image preference:
  If levelshots/preview/<mapname>.tga|jpg|png exists it is used as-is
  (expected to be a small, square 1:1 thumbnail).
  Otherwise falls back to Threewave default behaviour:
    active   -> levelshots/<mapname>
    disabled -> levelshots/<mapname>_disabled
===========================================================================
*/

#include "cg_local.h"

/* CS index of the first portal slot on a Threewave 1.7 server (0-indexed). */
#define CS_TW_PORTAL_BASE   35

/* Total number of portal slots (0..68). */
#define TW_PORTAL_COUNT     69

/*
=================
CG_PortalSurfaceName

Builds the in-world surface shader name for portal slot N.
Threewave naming:
  slot 0..8  (num 1..9)  -> "textures/q3start/pm<num>"
  slot 9..68 (num 10..69)-> "textures/q3start/pm0<num>"
=================
*/
static void CG_PortalSurfaceName( char *buf, int buflen, int slot )
{
	Com_sprintf( buf, buflen, "textures/q3start/pm%02i", slot + 1 );
}

/*
=================
CG_PortalUpdateSlot

Parses the Threewave portal configstring for slot and issues
a trap_R_RemapShader call so the correct levelshot appears on
the portal room surface.
=================
*/
void CG_PortalUpdateSlot( int slot )
{
	const char  *csStr;
	const char  *val;
	char         mapname[MAX_QPATH];
	char         surfaceName[MAX_QPATH];
	char         shotpath[MAX_QPATH];
	int          active;
	qhandle_t    previewShader;
	int          i;

	static const char *s_exts[] = { "tga", "jpg", "png" };

	if ( slot < 0 || slot >= TW_PORTAL_COUNT )
		return;

	csStr = CG_ConfigString( CS_TW_PORTAL_BASE + slot );
	if ( !csStr || !csStr[0] )
		return;

	val = Info_ValueForKey( csStr, "m" );
	if ( !val || !val[0] )
		return;

	Q_strncpyz( mapname, val, sizeof( mapname ) );

	val    = Info_ValueForKey( csStr, "a" );
	active = ( val && val[0] ) ? atoi( val ) : 0;

	CG_PortalSurfaceName( surfaceName, sizeof( surfaceName ), slot );

	/* Blank / placeholder slot — clear the surface. */
	if ( !Q_stricmp( mapname, "none" ) )
	{
		trap_R_RemapShader( surfaceName, "textures/q3start/x.tga", "0.0" );
		return;
	}

	/*
	 * Try a small preview image first.  These are expected to be 1:1 square
	 * thumbnails placed in levelshots/preview/ so they display without
	 * aspect-ratio distortion on the portal surface.
	 */
	previewShader = 0;
	for ( i = 0; i < 3; i++ )
	{
		Com_sprintf( shotpath, sizeof( shotpath ),
		             "levelshots/preview/%s.%s", mapname, s_exts[i] );
		previewShader = trap_R_RegisterShaderNoMip( shotpath );
		if ( previewShader )
			break;
	}

	if ( previewShader )
	{
		/* shotpath already holds the successful preview path. */
		trap_R_RemapShader( surfaceName, shotpath, active ? "0.001" : "0.0" );
		return;
	}

	/* Threewave default: remap to the full levelshot. */
	if ( active )
		Com_sprintf( shotpath, sizeof( shotpath ), "levelshots/%s", mapname );
	else
		Com_sprintf( shotpath, sizeof( shotpath ), "levelshots/%s_disabled", mapname );

	trap_R_RemapShader( surfaceName, shotpath, active ? "0.001" : "0.0" );
}

/*
=================
CG_PortalInit

Processes all Threewave portal configstrings.  Called once from
CG_Init after the gamestate has been received.
=================
*/
void CG_PortalInit( void )
{
	int i;
	for ( i = 0; i < TW_PORTAL_COUNT; i++ )
		CG_PortalUpdateSlot( i );
}
