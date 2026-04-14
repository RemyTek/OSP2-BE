/*
===========================================================================
cg_portal.c -- Threewave portal slot configstring handling.

Reads portal slot configstrings sent by the Threewave 1.7 server
(CS index = slot + CS_TW_PORTAL_BASE, 0-indexed) and remaps the
corresponding in-world portal surface texture to the destination
map's levelshot via trap_R_RemapShader.

Both active and inactive portal slots are remapped when a levelshot
image can be found.  Inactive portals still have a valid destination
map set by the server; skipping them leaves the portal surface as a
raw black void.  If no levelshot image exists for a map the surface is
left unchanged.

Shader selection:
  We use the image path with an explicit extension (.tga then .jpg then
  .png) rather than the bare shader name (e.g. "levelshots/q3wcp1").
  Threewave's remaps*.shader files define the bare name with rgbGen vertex,
  which multiplies colours by the portal surface's vertex colours and
  produces a darkened result.  Requesting the path by explicit extension
  bypasses the named shader and causes the engine to create a full-
  brightness auto-shader (rgbGen identity) from the raw image instead.
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
Slots are 1-indexed and zero-padded to two digits:
  slot 0  -> "textures/q3start/pm01"
  slot 63 -> "textures/q3start/pm64"
=================
*/
static void CG_PortalSurfaceName( char *buf, int buflen, int slot )
{
Com_sprintf( buf, buflen, "textures/q3start/pm%02i", slot + 1 );
}

/*
=================
CG_PortalUpdateSlot

Parses the Threewave portal configstring for slot and issues a
trap_R_RemapShader call so the correct levelshot appears on the
portal room surface.

Both active and inactive slots are remapped when a levelshot image
can be found.  If no image is found the surface is left unchanged.
=================
*/
void CG_PortalUpdateSlot( int slot )
{
const char  *csStr;
const char  *val;
char         mapname[MAX_QPATH];
char         surfaceName[MAX_QPATH];
char         shotpath[MAX_QPATH];
qhandle_t    sh;
int          i;

static const char *s_exts[] = { "tga", "jpg", "png", NULL };

if ( slot < 0 || slot >= TW_PORTAL_COUNT )
return;

csStr = CG_ConfigString( CS_TW_PORTAL_BASE + slot );
if ( !csStr || !csStr[0] )
return;

val = Info_ValueForKey( csStr, "m" );
if ( !val || !val[0] )
return;

Q_strncpyz( mapname, val, sizeof( mapname ) );

/* Blank / placeholder slot -- do not remap. */
if ( !Q_stricmp( mapname, "none" ) )
return;

CG_PortalSurfaceName( surfaceName, sizeof( surfaceName ), slot );

/*
 * Request the levelshot by its explicit image extension rather than the
 * bare shader name.  Threewave's remaps*.shader scripts define
 * "levelshots/<mapname>" with rgbGen vertex, which multiplies the image
 * by the portal surface's vertex colours and produces a darkened result.
 * Using an explicit extension (e.g. "levelshots/q3wcp1.tga") causes the
 * engine to create an auto-shader with rgbGen identity instead, giving
 * correct full-brightness output.
 */
sh = 0;
for ( i = 0; s_exts[i]; i++ )
{
Com_sprintf( shotpath, sizeof( shotpath ),
             "levelshots/%s.%s", mapname, s_exts[i] );
sh = trap_R_RegisterShaderNoMip( shotpath );
if ( sh )
break;
}

if ( sh )
trap_R_RemapShader( surfaceName, shotpath, "0.001" );

/* No usable levelshot found -- leave portal surface as-is. */
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
