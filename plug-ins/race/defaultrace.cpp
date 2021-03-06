/* $Id$
 *
 * ruffina, 2004
 */
#include <set>

#include "defaultrace.h"
#include "defaultpcrace.h"
#include "raceflags.h"

#include "class.h"
#include "grammar_entities_impl.h"

#include "skillgroup.h"                                                       
#include "skill.h"
#include "skillmanager.h"
#include "pcharacter.h"
#include "wearlocation.h"
#include "alignment.h"
#include "logstream.h"

#include "dreamland.h"
#include "merc.h"
#include "def.h"

using namespace std;
/*-------------------------------------------------------------------
 * RaceHelp 
 *------------------------------------------------------------------*/
static const DLString LABEL_RACE = "race";
const DLString RaceHelp::TYPE = "RaceHelp";
GROUP(ancient_languages);

void RaceHelp::setRace( Race::Pointer race )
{
    this->race = race;
    
    if (!keyword.empty( ))
        keywords.fromString( keyword.toLower() );

    keywords.insert( race->getName( ) );
    keywords.insert( race->getMaleName( ).ruscase( '1' ) );
    keywords.insert( race->getFemaleName( ).ruscase( '1' ) );
    keywords.insert( race->getMltName( ).ruscase( '1' ) );
    fullKeyword = keywords.toString( ).toUpper( );
    addLabel(LABEL_RACE);

    helpManager->registrate( Pointer( this ) );
}

void RaceHelp::unsetRace( )
{
    helpManager->unregistrate( Pointer( this ) );
    race.clear( );
    keywords.clear();
    fullKeyword = "";
}

DLString RaceHelp::getTitle(const DLString &label) const
{
    if (race)
        return race->getMltName().ruscase('1') + ", " + race->getName();
    return HelpArticle::getTitle(label);
}

struct CommaSet : public set<string> {
    void print( ostream &buf ) const {
        bool found = false;
        for (const_iterator i = begin( ); i != end( ); i++) {
            if (found)
                buf << ", ";
            buf << *i;
            found = true;
        }
    }
};
inline ostream& operator << ( ostream& ostr, const CommaSet& cset )
{
    cset.print( ostr );
    return ostr;
}

void RaceHelp::getRawText( Character *ch, ostringstream &in ) const
{
    DLString nameF = race->getFemaleName( ).ruscase( '1' ); 
    DLString nameM = race->getMaleName( ).ruscase( '1' ); 

    in << "Раса {C" << (ch->getSex( ) == SEX_FEMALE ? nameF : nameM) << "{x";
    if (nameF != nameM)
        in << " ({C" << (ch->getSex( ) == SEX_FEMALE ? nameM : nameF) << "{x)";
    in << " или {C" << race->getName( ) << "{x" << endl << endl;

    in << *this << endl;

    const PCRace *r = (const_cast<Race *>(race.getPointer( )))->getPC( );
    if (r) {
        
        in << "{cХарактер{x  : " << align_name_for_range( r->getMinAlign( ), r->getMaxAlign( ) ) << endl;
        if (!r->getStats( ).empty( )) {
            bool found = false;

            in << "{cПараметры{x : ";
            for (int i = 0; i < stat_table.size; i++) {
                int stat = r->getStats( )[i];
                if (stat != 0) {
                    if (found) 
                        in << ", ";
                    in << (stat > 0 ? "+" : "") << stat << " к " << stat_table.message( i, '3' );
                    found = true;
                }
            }
            if (!found)
                in << "без изменений";
            in << endl;
        }
        in << "{cРазмер{x    : " << size_table.message( r->getSize( ) ) << endl; 
        
        in << "{cПрофессии{x : ";
        bool found = false;
        for (int i = 0; i < professionManager->size( ); i++) {
            Profession *prof = professionManager->find( i );
            if (const_cast<PCRace *>(r)->getClasses( )[prof->getIndex( )] > 0) {
                if (found)
                  in << ", ";  
                in << prof->getRusName( ).ruscase( '1' );
                found = true;
            }
        }
        in << endl;

        if (r->getPoints( ) > 0) {
            in << "{cДоп. опыт{x : " << r->getPoints( ) << endl;
        }

        DLString res = imm_flags.messages( r->getRes( ), true, '1' );
        if (!res.empty( )) {
            in << "{cУстойчивы{x : к " << res << endl;
        }
        DLString vuln = imm_flags.messages( r->getVuln( ), true, '1' );
        if (!vuln.empty( )) {
            in << "{cУязвимы{x   : к " << vuln << endl;
        }

        DLString aff = r->getAff( ).messages( true, '1' );
        if (!aff.empty( )) {
            in << "{cАффекты{x   : " << aff << endl;
        }
    }

    in << endl;

    // Pretend we have a dummy character of this race. Find out
    // all available race aptitude, professional and
    // non-professioal bonuses.
    CommaSet prof100, noprof100, raceApt;

    for (int sn = 0; sn < skillManager->size( ); sn++) {
        Skill *skill = skillManager->find( sn );
        DLString sname = "{sR" + skill->getRussianName( ) + "{sE" + skill->getName( ) + "{sx";
        PCharacter dummy;
        dummy.setRace( race->getName( ) );
        dummy.setLevel( 100 );

        // Collect skills available even for profession 'none',
        // such as race aptitudes or non-professional bonuses.
        if (skill->visible( &dummy )) {
            if (skill->getLearned( &dummy ) >= 100)
                noprof100.insert( sname );
            else if (skill->getGroup( ) != group_ancient_languages)
                raceApt.insert( sname );
        }            

        // Pretend we're every profession in a row and check
        // for professional 100% learned bonus.
        for (int i = 0; i < professionManager->size( ); i++) {
            Profession *prof = professionManager->find( i );
            if (!prof->isPlayed( ))
                continue;
            dummy.setProfession( prof->getName( ) );
            if (skill->visible( &dummy )) {
                if (skill->getLearned( &dummy ) >= 100)
                    if (raceApt.count( sname ) == 0 && noprof100.count( sname ) == 0)
                        prof100.insert( sname );
            }
        }
    }
    
    if (!raceApt.empty( )) {
        in << "{WРасовые cпособности{x: " << raceApt << endl;
    }
    if (!prof100.empty( )) {
        in << "{WБонусы на профессиональные умения{x: " << prof100 << endl;
    }
    if (!noprof100.empty( )) {
        in << "{WБонусы независимо от профессии{x: " << noprof100 << endl;
    }

    in << endl << "Подробнее обо всех параметрах читай в %H% [(race stats,раса характеристики)]" << endl;
}

/* ------------------------------------------------------------------
 * DefaultRace
 *------------------------------------------------------------------*/
DefaultRace::DefaultRace( ) : 
                det( 0, &detect_flags ),
                act( 0, &act_flags ),
                aff( 0, &affect_flags ),
                off( 0, &off_flags ),
                imm( 0, &imm_flags ),
                res( 0, &res_flags ),
                vuln( 0, &vuln_flags ),
                form( 0, &form_flags ),
                parts( 0, &part_flags ),
                size( SIZE_MEDIUM, &size_table ),
                wearloc( wearlocationManager ),
                hunts( raceManager ),
                donates( raceManager )
{
}

const DLString & DefaultRace::getName( ) const
{
    return Race::getName( );
}

bool DefaultRace::isValid( ) const
{
    return true;
}

void DefaultRace::setName( const DLString &name )
{
    this->name = name;
}

bool DefaultRace::matchesStrict( const DLString &str ) const
{
    return Race::matchesStrict( str ); /* TODO */
}

bool DefaultRace::matchesUnstrict( const DLString &str ) const
{
    return Race::matchesUnstrict( str ); /* TODO */
}


void DefaultRace::loaded( )
{
    raceManager->registrate( Pointer( this ) );

    if (help)
        help->setRace( Pointer( this ) );
}

void DefaultRace::unloaded( )
{
    if (help)
        help->unsetRace( );

    raceManager->unregistrate( Pointer( this ) );
}

const Flags & DefaultRace::getDet( ) const
{
    return det;
}
const Flags & DefaultRace::getAct( ) const
{
    return act;
}
const Flags & DefaultRace::getAff( ) const
{
    return aff;
}
const Flags & DefaultRace::getOff( ) const
{
    return off;
}
const Flags & DefaultRace::getImm( ) const
{
    return imm;
}
const Flags & DefaultRace::getRes( ) const
{
    return res;
}
const Flags & DefaultRace::getVuln( ) const
{
    return vuln;
}
const Flags & DefaultRace::getForm( ) const
{
    return form;
}
const Flags & DefaultRace::getParts( ) const
{
    return parts;
}
const GlobalBitvector & DefaultRace::getWearloc( ) const
{
    return wearloc;
}

const Enumeration & DefaultRace::getSize( ) const
{
    return size;
}

const DLString & DefaultRace::getMaleName( ) const
{
    return nameMale.getValue( );
}
const DLString & DefaultRace::getNeuterName( ) const
{
    return nameNeuter.empty( ) ? nameMale : nameNeuter;
}
const DLString & DefaultRace::getFemaleName( ) const
{
    return nameFemale.getValue( );
}
const DLString & DefaultRace::getMltName( ) const
{
    return nameMlt.getValue( );
}
DLString DefaultRace::getNameFor( Character *looker, Character *me, const Grammar::Case &c ) const
{
    if (looker && me && looker->getConfig( )->rucommands) {
        if (me->getSex( ) == SEX_MALE)
            return getMaleName( ).ruscase( c );
        if (me->getSex( ) == SEX_FEMALE)
            return getFemaleName( ).ruscase( c );
        if (me->getSex( ) == SEX_NEUTRAL)
            return getNeuterName( ).ruscase( c );
    }
    
    return getName( );
}

Flags DefaultRace::getAttitude( const Race &race ) const
{
    Flags att( 0, &race_flags );

    if (getForm( ).isSet( FORM_CANINE ) && race.getForm( ).isSet( FORM_FELINE ))
        att.setBit( RACE_HATES );

    if (getForm( ).isSet( FORM_FELINE ) && race.getForm( ).isSet( FORM_CANINE ))
        att.setBit( RACE_HATES );

    return att;
}

