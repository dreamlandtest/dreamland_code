/* $Id: skillmanager.cpp,v 1.1.2.4.6.3 2010-09-05 13:57:11 rufina Exp $
 *
 * ruffina, 2004
 */

#include "skillmanager.h"
#include "skill.h"
#include "stringlist.h"

#include "pcharacter.h"

SkillManager* skillManager = 0;

SkillManager::SkillManager( ) 
{
    checkDuplicate( skillManager );
    skillManager = this;
    setRegistryName("skill");
    saveRegistryName();
}

SkillManager::~SkillManager( )
{
    eraseRegistryName();
    skillManager = 0;
}

int SkillManager::unstrictLookup( const DLString &constName, Character *ch ) const
{
    unsigned int i;
    Indexes::const_iterator iter;
    const Skill *skill;
    DLString name = constName.quote().getOneArgument();

    if (name.empty( ))
        return -1;
    
    // strict lookup
    iter = indexes.find( name );
    
    if (iter != indexes.end( )) {
        i = iter->second;
        skill = (Skill *)*table[i];

        if (!ch || skill->available( ch ))
            return i;
    }
    
    // unstrict lookup by partial name , e.g. 'cur po' for 'cure poison'
    StringList argList(name);

    for (i = 0; i < table.size( ); i++) {
        skill = (Skill *)*table[i];

        if (StringList(skill->getName()).superListOf(argList)
             || StringList(skill->getRussianName()).superListOf(argList))
        {
            if (!ch || skill->available( ch ))
                return i;
        }
    }

    return -1;
}

GlobalRegistryElement::Pointer SkillManager::getDumbElement( const DLString &name ) const
{
    return Skill::Pointer( NEW, name );
}

