/*
* Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
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

#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "Creature.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "World.h"
#include "Spell.h"
#include "WorldSession.h"
#include "ScriptedCreature.h"

#define HEARTH             "Hearthstone"

#define ALLIANCE_LOCATIONS "Alliance Locations"
#define LOC_SW             "Stormwind"
#define LOC_DARN           "Darnassus"
#define LOC_IRONFORGE      "Ironforge"
#define LOC_EXODAR         "The Exodar"

#define HORDE_LOCATIONS    "Horde Locations"
#define LOC_ORG            "Orgrimmar"
#define LOC_UNDERCITY      "Undercity"
#define LOC_SILVERMOON     "Silvermoon City"
#define LOC_THUNDERBLUFF   "Thunder Bluff"

#define NEUTRAL_LOCATIONS  "Neutral Locations"
#define DALARAN_NORTHREND  "Dalaran (Northrend)"
#define DALARAN_BI         "Dalaran (Broken Isles)"

#define DUNGEONS           "Dungeons"
#define RAIDS              "Raids"

#define RAGEFIRE           "Ragefire Chasm"
#define MALL               "Player Mall"

class playerScript_josh : public PlayerScript
{
	public:
		playerScript_josh() : PlayerScript("playerScript_josh") { }

		void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg)
		{
			if (msg == "goodbye")
			{
				player->ModifyHealth(100000000);
			}
		}
};

class teleporter_ItemGossip : public ItemScript
{
public:
	teleporter_ItemGossip() : ItemScript("teleporter_ItemGossip") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/, ObjectGuid /*castId*/) override
	{
        ClearGossipMenuFor(player);
		AddGossipItemFor(player, GOSSIP_ICON_CHAT, MALL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, ALLIANCE_LOCATIONS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, HORDE_LOCATIONS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, NEUTRAL_LOCATIONS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, DUNGEONS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, RAIDS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
		SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
		
		return true;
	}

    void OnGossipSelect(Player* player, Item* item, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF + 1:
            ClearGossipMenuFor(player);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, LOC_SW, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, LOC_DARN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, LOC_IRONFORGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, LOC_EXODAR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            ClearGossipMenuFor(player);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, LOC_ORG, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, LOC_UNDERCITY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, LOC_SILVERMOON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, LOC_THUNDERBLUFF, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            ClearGossipMenuFor(player);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, DALARAN_NORTHREND, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, DALARAN_BI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:
            ClearGossipMenuFor(player);
			AddGossipItemFor(player, GOSSIP_ICON_CHAT, RAGEFIRE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:
            player->TeleportTo(0, -8833.07f, 622.778f, 93.9317f, 0.6771f); //Stormwind
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 7:
            player->TeleportTo(1, 1569.97f, -4397.41f, 16.0472f, 0.543025f); //Orgrimmar
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 8:
            player->TeleportTo(0, 1584.07f, 241.987f, -52.1534f, 0.049647f); //Undercity
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:
            player->TeleportTo(530, 9487.69f, -7279.2f, 14.2866f, 6.16478f); //Silvermoon City
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 10:
            player->TeleportTo(1, -1277.37f, 124.804f, 131.287f, 5.22274f); //Thunder Bluff
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 11:
            player->TeleportTo(1, 9949.56f, 2284.21f, 1341.4f, 1.59587f); //Darnassus
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 12:
            player->TeleportTo(0, -4918.88f, -940.406f, 501.564f, 5.42347f); //Ironforge
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 13:
            player->TeleportTo(530, -3965.7f, -11653.6f, -138.844f, 0.852154f); //The Exodar
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 14:
            player->TeleportTo(571, 5804.15f, 624.771f, 647.767f, 1.64f); //Dalaran (Northrend)
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 15:
            player->TeleportTo(1220, -828.835f, 4371.91f, 738.636f, 1.88158f); //Dalaran (Broken Isles)
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 16:
            player->TeleportTo(389, 0.797643f, -8.234290f, -15.528800f, 4.712389f); //Ragefire Chasm
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 17:
            player->TeleportTo(1220, 1711.617798f, 4633.925293f, 124.005684f, 5.251531f); //Player Mall
            CloseGossipMenuFor(player);
            break;
        default:
            break;
        }
    }
};

void AddSC_custom_npcs()
{
    new playerScript_josh();
    new teleporter_ItemGossip();
}
