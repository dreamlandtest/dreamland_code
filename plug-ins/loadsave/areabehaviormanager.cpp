/* $Id: areabehaviormanager.cpp,v 1.1.2.1 2009/09/19 00:53:18 rufina Exp $
 *
 * ruffina, 2003
 */

#include "areabehaviormanager.h"
#include "areabehavior.h"
#include "xmldocument.h"
#include "logstream.h"
#include "fread_utils.h"
#include "mercdb.h"
#include "merc.h"
#include "def.h"

void AreaBehaviorManager::parse( AREA_DATA * pArea, FILE *fp ) {
    char letter;
    char *word;
    
    letter = fread_letter( fp );
    ungetc( letter, fp );

    if (letter != '<') 
        return;
        
    word = fread_string( fp );

    try {
        std::basic_istringstream<char> istr( word );
        
        pArea->behavior.fromStream( istr );
        pArea->behavior->setArea( pArea );

    } catch (Exception e) {
        LogStream::sendError( ) << e.what( ) << endl;
    }
        
    free_string( word );
}

void AreaBehaviorManager::save( const AREA_DATA *pArea, FILE *fp ) {
    std::basic_ostringstream<char> ostr;
     
    if (!pArea->behavior)
        return;

    try {
        pArea->behavior.toStream( ostr );

        fprintf( fp, "%s~\n", ostr.str( ).c_str( ) );

    } catch (ExceptionXMLError e) {
        LogStream::sendError( ) << e.what( ) << endl;
    }
}

bool area_is_mansion(area_data *area)
{
    return !str_prefix("ht", area->area_file->file_name);
}

bool area_is_clan(area_data *area)
{
    static const DLString CLAN_AREA_TYPE = DLString("ClanArea");
    return area->behavior && CLAN_AREA_TYPE.strPrefix(area->behavior->getType());
}

bool area_is_hometown(area_data *area)
{
    return IS_SET(area->area_flag, AREA_HOMETOWN);
}

