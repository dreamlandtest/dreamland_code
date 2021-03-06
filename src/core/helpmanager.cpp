/* $Id$
 *
 * ruffina, 2004
 */
#include "helpmanager.h"
#include "character.h"

template class XMLStub<HelpArticle>;

const DLString HelpArticle::ATTRIBUTE_KEYWORD = "keyword";
const DLString HelpArticle::ATTRIBUTE_LEVEL = "level";
const DLString HelpArticle::ATTRIBUTE_REF = "ref";
const DLString HelpArticle::ATTRIBUTE_REFBY = "refby";
const DLString HelpArticle::ATTRIBUTE_LABELS = "labels";

long lastID = 0;

HelpArticle::HelpArticle( ) 
               : areafile( NULL ),
                 level( -1 )
{
}

DLString HelpArticle::getText( Character * ) const
{
    return *this;
}

const DLString & HelpArticle::getKeyword( ) const
{
    return fullKeyword;
}

const StringSet & HelpArticle::getKeywords() const
{
    return keywords;
}

int HelpArticle::getLevel( ) const
{
    return level;
}

void HelpArticle::setLevel( int level )
{
    this->level = level;
}

void HelpArticle::setID(int id) 
{
    this->id = id;
}

int HelpArticle::getID() const
{
    return id;
}

DLString HelpArticle::getTitle(const DLString &label) const 
{
    return getKeyword();
}


void HelpArticle::setText( const DLString &text )
{
    assign( text );
}

void HelpArticle::addKeyword( const DLString &add )
{
    // 'add' can contain a list of keywords, parse them into keywords set.
    keywords.fromString(add.toLower());
    // Regenerate concatenated string.
    fullKeyword = keywords.toString().toUpper();
}

const DLString &HelpArticle::getKeywordAttribute() const
{
    return keyword;
}

void HelpArticle::setKeywordAttribute(const DLString &keyword)
{
    this->keyword = keyword;
    addKeyword( keyword );
}

const StringSet & HelpArticle::getLabels() const
{
    return labels;
}

void HelpArticle::addLabel(const DLString &label) 
{
    labels.insert(label);
}

void HelpArticle::setLabelAttribute(const DLString &attribute)
{
    this->labelAttribute = attribute;
    StringSet newLabels;
    newLabels.fromString(labelAttribute);
    labels.insert(newLabels.begin(), newLabels.end());
}

bool HelpArticle::visible( Character *ch ) const
{
    return ch->get_trust( ) >= level;
}

bool HelpArticle::toXML( XMLNode::Pointer &parent ) const
{
    XMLStringNoEmpty xmlString( *this );

    if (!xmlString.toXML( parent ))
        if (keyword.empty( ) && ref.empty( ) && refby.empty( ))
            return false;
    
    if (!keyword.empty( ))
        parent->insertAttribute( ATTRIBUTE_KEYWORD, keyword );

    if (level >= -1)
        parent->insertAttribute( ATTRIBUTE_LEVEL, DLString( level ) );
    
    if (!ref.empty( ))
        parent->insertAttribute( ATTRIBUTE_REF, ref.toString( ) );

    if (!refby.empty( ))
        parent->insertAttribute( ATTRIBUTE_REFBY, refby.toString( ) );

    if (!labelAttribute.empty())
        parent->insertAttribute(ATTRIBUTE_LABELS, labelAttribute);

    return true;
}

void HelpArticle::fromXML( const XMLNode::Pointer &parent ) throw( ExceptionBadType )
{
    XMLStringNoEmpty xmlString;
    
    xmlString.fromXML( parent );
    assign( xmlString );

    keyword = parent->getAttribute( ATTRIBUTE_KEYWORD );
    addKeyword( keyword );

    if (parent->hasAttribute( ATTRIBUTE_LEVEL ))
        level = parent->getAttribute( ATTRIBUTE_LEVEL ).toInt( );

    ref.fromString( parent->getAttribute( ATTRIBUTE_REF ) );
    refby.fromString( parent->getAttribute( ATTRIBUTE_REFBY ) );
    labelAttribute = parent->getAttribute(ATTRIBUTE_LABELS);
    labels.fromString(labelAttribute);
}

/*-----------------------------------------------------------------------
 * HelpManager
 *-----------------------------------------------------------------------*/
HelpManager * helpManager = NULL;

HelpManager::HelpManager( )
{
    checkDuplicate( helpManager );
    helpManager = this;
}

HelpManager::~HelpManager( )
{
    helpManager = NULL;
}


void HelpManager::registrate( HelpArticle::Pointer art )
{
    articles.push_back( art );
    art->setID(++lastID);
}

void HelpManager::unregistrate( HelpArticle::Pointer art )
{
    articles.remove( art );
    art->setID(-1);
}


