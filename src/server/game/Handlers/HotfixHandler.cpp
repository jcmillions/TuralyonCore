/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "WorldSession.h"
#include "Containers.h"
#include "DB2Stores.h"
#include "HotfixPackets.h"
#include "Log.h"
#include "ObjectDefines.h"
#include "World.h"
#include "ObjectMgr.h"

void WorldSession::HandleDBQueryBulk(WorldPackets::Hotfix::DBQueryBulk& dbQuery)
{
    DB2StorageBase const* store = sDB2Manager.GetStorage(dbQuery.TableHash);
    if (!store)
    {
        TC_LOG_ERROR("network", "CMSG_DB_QUERY_BULK: %s requested unsupported unknown hotfix type: %u", GetPlayerInfo().c_str(), dbQuery.TableHash);
        return;
    }

    for (WorldPackets::Hotfix::DBQueryBulk::DBQueryRecord const& record : dbQuery.Queries)
    {
        WorldPackets::Hotfix::DBReply dbReply;
        dbReply.TableHash = dbQuery.TableHash;
        dbReply.RecordID = record.RecordID;

        if (dbReply.TableHash == 2442913102) //Hash of itemsparse.db2
        {
            if (const ItemTemplate* proto = sObjectMgr->GetItemTemplate(dbReply.RecordID))
            {
                WorldPacket l_Data(SMSG_DB_REPLY);

                l_Data << uint32(dbReply.TableHash);
                l_Data << uint32(dbReply.RecordID);
                l_Data << uint32(dbReply.Timestamp);
                l_Data.WriteBit(dbReply.Allow);

                l_Data.FlushBits();

                ByteBuffer l_Buff;

                l_Buff << uint32(proto->ItemId);
                l_Buff << int32(proto->AllowableRace);

                // item name
                std::string name = proto->Name1;
                l_Buff << uint16(name.length());
                if (name.length())
                    l_Buff << name;

                for (uint32 i = 0; i < 3; ++i) // other 3 names
                    l_Buff << uint16(0);

                std::string desc = proto->Description;
                l_Buff << uint16(desc.length());
                if (desc.length())
                    l_Buff << desc;

                l_Buff << int32(proto->Flags);
                l_Buff << int32(proto->Flags2);
                l_Buff << int32(proto->Flags3);
                l_Buff << int32(0);             //Flags 4
                l_Buff << int32(0); //Unk1
                l_Buff << int32(1); //Unk2
                l_Buff << int32(proto->BuyCount);
                l_Buff << int32(proto->BuyPrice);
                l_Buff << int32(proto->SellPrice);
                l_Buff << int32(proto->RequiredSpell);
                l_Buff << int32(proto->MaxCount);
                l_Buff << int32(proto->Stackable);

                for (int i = 0; i < 10; i++)
                    l_Buff << int32(proto->ItemStat[i].ItemScalingValue);

                for (int i = 0; i < 10; i++)
                    l_Buff << float(0.0f); //ItemStatSocketcostMultiplier

                l_Buff << float(proto->RangedModRange);
                l_Buff << int32(proto->BagFamily);
                l_Buff << float(proto->ArmorDamageModifier);
                l_Buff << int32(proto->Duration);
                l_Buff << int32(proto->StatScalingFactor);
                l_Buff << int32(proto->AllowableClass);
                l_Buff << int32(proto->ItemLevel);
                l_Buff << int32(proto->RequiredSkill);
                l_Buff << int32(proto->RequiredSkillRank);
                l_Buff << int32(proto->RequiredReputationFaction);

                for (int i = 0; i < 10; i++)
                    l_Buff << int32(proto->ItemStat[i].ItemStatValue);

                l_Buff << int32(proto->ScalingStatDistribution);
                l_Buff << int32(proto->Delay);
                l_Buff << int32(proto->PageText);
                l_Buff << int32(proto->StartQuest);
                l_Buff << int32(proto->LockID);
                l_Buff << int32(proto->RandomProperty);
                l_Buff << int32(proto->RandomSuffix);
                l_Buff << int32(proto->ItemSet);
                l_Buff << int32(proto->Area);
                l_Buff << int32(proto->Map);
                l_Buff << int32(proto->TotemCategory);
                l_Buff << int32(proto->socketBonus);
                l_Buff << int32(proto->GemProperties);
                l_Buff << int32(proto->ItemLimitCategory);
                l_Buff << int32(proto->HolidayId);
                l_Buff << int32(0); //RequiredTransmogHolidayId
                l_Buff << int32(proto->ItemId); //Item Name Description Id
                l_Buff << uint8(proto->Quality);
                l_Buff << uint8(proto->_InventoryType);
                l_Buff << uint8(proto->RequiredLevel);
                l_Buff << uint8(proto->RequiredHonorRank);
                l_Buff << uint8(proto->RequiredCityRank);
                l_Buff << uint8(proto->ContainerSlots);

                for (int i = 0; i < 10; i++)
                    l_Buff << uint8(proto->ItemStat[i].ItemStatType);

                l_Buff << uint8(proto->DamageType);
                l_Buff << uint8(proto->Bonding);
                l_Buff << uint8(proto->LanguageID);
                l_Buff << uint8(proto->PageMaterial);
                l_Buff << uint8(proto->Material);
                l_Buff << uint8(proto->Sheath);

                for (int i = 0; i < 3; i++)
                    l_Buff << uint8(proto->Socket[i].Color);

                l_Buff << uint8(proto->CurrencySubstitutionId);
                l_Buff << uint8(proto->CurrencySubstitutionCount);
                l_Buff << uint8(0); //ArtifactId
                l_Buff << uint8(0); //RequiredExpansion

                l_Data.append(l_Buff);
                l_Data << uint32(l_Buff.size());

                continue;
            }
        }

        if (dbReply.TableHash == 1344507586) //Hash of item.db2
        {
            if (const ItemTemplate* proto = sObjectMgr->GetItemTemplate(dbReply.RecordID))
            {
                WorldPacket l_Data(SMSG_DB_REPLY);

                ByteBuffer l_Buff;


                l_Data << uint32(dbReply.TableHash);
                l_Data << uint32(dbReply.RecordID);
                l_Data << uint32(dbReply.Timestamp);
                l_Data.WriteBit(dbReply.Allow);

                l_Data.FlushBits();

                l_Buff << uint32(proto->ItemId);
                l_Buff << uint32(proto->DisplayInfoID);
                l_Buff << uint8(proto->Class);
                l_Buff << uint8(proto->SubClass);
                l_Buff << uint8(proto->SoundOverrideSubclass);
                l_Buff << uint8(proto->Material);
                l_Buff << uint8(proto->_InventoryType);
                l_Buff << uint8(proto->Sheath);
                l_Buff << uint8(0);                     //GroupSoundsIds

                continue;
            }
        }

        if (store->HasRecord(record.RecordID))
        {
            dbReply.Allow = true;
            dbReply.Timestamp = sWorld->GetGameTime();
            store->WriteRecord(record.RecordID, GetSessionDbcLocale(), dbReply.Data);
        }
        else
        {
            TC_LOG_TRACE("network", "CMSG_DB_QUERY_BULK: %s requested non-existing entry %u in datastore: %u", GetPlayerInfo().c_str(), record.RecordID, dbQuery.TableHash);
            dbReply.Timestamp = time(NULL);
        }

        SendPacket(dbReply.Write());
    }
}

void WorldSession::SendAvailableHotfixes(int32 version)
{
    SendPacket(WorldPackets::Hotfix::AvailableHotfixes(version, sDB2Manager.GetHotfixData()).Write());
}

void WorldSession::HandleHotfixRequest(WorldPackets::Hotfix::HotfixRequest& hotfixQuery)
{
    std::map<uint64, int32> const& hotfixes = sDB2Manager.GetHotfixData();
    WorldPackets::Hotfix::HotfixResponse hotfixQueryResponse;
    hotfixQueryResponse.Hotfixes.reserve(hotfixQuery.Hotfixes.size());
    for (uint64 hotfixId : hotfixQuery.Hotfixes)
    {
        if (int32 const* hotfix = Trinity::Containers::MapGetValuePtr(hotfixes, hotfixId))
        {
            DB2StorageBase const* storage = sDB2Manager.GetStorage(PAIR64_HIPART(hotfixId));

            WorldPackets::Hotfix::HotfixResponse::HotfixData hotfixData;
            hotfixData.ID = hotfixId;
            hotfixData.RecordID = *hotfix;
            if (storage->HasRecord(hotfixData.RecordID))
            {
                hotfixData.Data = boost::in_place();
                storage->WriteRecord(hotfixData.RecordID, GetSessionDbcLocale(), *hotfixData.Data);
            }

            hotfixQueryResponse.Hotfixes.emplace_back(std::move(hotfixData));
        }
    }

    SendPacket(hotfixQueryResponse.Write());
}
