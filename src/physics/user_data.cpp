#include "user_data.hpp"
#include <algorithm>
#include <sstream>

namespace jadeite
{
ObjectData::ObjectData( const std::string& tag, const std::string& group, bool collider,
				bool trigger, bool isFriendly, std::uint32_t entity_id )
	: tag{ tag }
	, group{ group }
	, bCollider{ collider }
	, bTrigger{ trigger }
	, bIsFriendly{ isFriendly }
	, entityID{ entity_id }
{
	
}
				
std::string ObjectData::toString() const
{
	std::stringstream ss;
	ss << "========= ObjectData =========\n"
		<< std::boolalpha << "Tag: " << tag << "\n"
		<< "Group: " << group << "\n"
		<< "bCollider: " << bCollider << "\n"
		<< "bTrigger: " << bTrigger << "\n"
		<< "bIsFriendly: " << bIsFriendly << "\n"
		<< "EntityID: " << entityID << "\n";
		
	return ss.str();
}

bool ObjectData::AddContact(const ObjectData* objectData)
{
	if ( tag.empty() && group.empty() )
		return false;
	
	if ( objectData->tag.empty() && objectData->group.empty() )
		return false;
	
	if ( objectData->tag == tag && objectData->group == group )
		return false;
	
	if ( bIsFriendly && objectData->bIsFriendly && bTrigger && objectData->bTrigger )
		return false;
	
	auto contactItr = std::ranges::find_if(
		contactEntities,
		[&](const ObjectData* contactInfo )
		{
			return *contactInfo == *objectData;
		}
	);
	
	if ( contactItr != contactEntities.end() )
		return false;
	
	contactEntities.push_back( objectData );
	return true;
}

bool ObjectData::RemoveContact(const ObjectData* objectData)
{
	if ( objectData->tag.empty() && objectData->group.empty() )
		return false;
	
	auto contactItr = std::remove_if( 
		contactEntities.begin(), contactEntities.end(),
		[&](const ObjectData* contactInfo )
		{
			return *contactInfo == *objectData;
		}
	);
	
	if (contactItr == contactEntities.end())
		return false;
	
	contactEntities.erase( contactItr, contactEntities.end() );
	return true;
}

bool operator==(const ObjectData& a, const ObjectData& b)
{
	return a.bCollider == b.bCollider && a.bTrigger == b.bTrigger && 
			a.bIsFriendly == b.bIsFriendly && a.tag == b.tag && 
			a.group == b.group && a.entityID == b.entityID;
}

} // jadeite