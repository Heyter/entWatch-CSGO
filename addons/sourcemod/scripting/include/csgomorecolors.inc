/**************************************************************************
 *                         CSGO More Colors (CMC)                         *
 *                         Colored Chat Functions                         *
 *       Author: exvel, Editor: Popoklopsi, Powerlord, Bara, Klexen       *
 *                           Version: CSGO 2.0	                          *
 *         Modified Specifically for CSGO Color Support by Klexen         *
 *                     18 color support by Ariistuujj                     *
 **************************************************************************/

#if defined _cmc_included
 #endinput
#endif
#define _cmc_included
 
#define MAX_LENGTH_CHAT 192
#define MAX_COLORS 18

#define SERVER_INDEX 0
#define NO_INDEX -1
#define NO_PLAYER -2

enum Colors
{
 	Color_White = 0, // White
	Color_DarkRed, // Red
	Color_Purple, // Light Blue-Violet
	Color_Yellow, // Buff
	Color_LightPurple, // Perano
	Color_DarkGreen, // Bright Green
	Color_LightGreen, // Tea Green
	Color_Green, // Green-Yellow
	Color_Red, // Coral Red
	Color_Gray, // Gray
	Color_LightYellow, // Khaki
	Color_LightBlue, // Gray-Blue
	Color_Blue, // Cornflower Blue
	Color_DarkBlue, // Neon Blue
	Color_GrayBlue, // Gray-Blue
	Color_DarkPurple, // Orchid
	Color_LightRed, // Chili Red
	Color_Orange // Gold
}

char CTag[][] =							{ "{WHITE}", "{DARKRED}", "{PURPLE}", "{YELLOW}", "{LIGHTPURPLE}", "{DARKGREEN}", "{LIGHTGREEN}", "{GREEN}", "{RED}", "{GRAY}", "{LIGHTYELLOW}", "{LIGHTBLUE}", "{BLUE}", "{DARKBLUE}", "{GRAYBLUE}", "{DARKPURPLE}", "{LIGHTRED}", "{ORANGE}" };
char CTagCode[][] =						{ "\x01",    "\x02",      "\x03",     "\x03",     "\x03",          "\x04",        "\x05",         "\x06",    "\x07",  "\x08",   "\x09",          "\x0A",        "\x0B",   "\x0C",       "\x0D",       "\x0E",         "\x0F",       "\x10" };
bool CTagReqSayText2[] = 				{ false,     false,       false,      true,       true,            false,         false,          false,     false,   false,    false,           false,         false,    false,        false,        false,          false,        false };
bool CEventIsHooked = false;
bool CSkipList[MAXPLAYERS+1] = { false, ... };
int CProfile_TeamIndex[MAX_COLORS+1] = 	{ NO_INDEX,  NO_INDEX,  SERVER_INDEX, CS_TEAM_T,  CS_TEAM_CT,      NO_INDEX, ... };

/**
 * Prints a message to a specific client in the chat area.
 * Supports color tags.
 *
 * @param client	  Client index.
 * @param szMessage   Message (formatting rules).
 * @return			  No return
 * 
 * On error/Errors:   If the client is not connected an error will be thrown.
 */
stock void CPrintToChat(int client, const char[] szMessage, any ...)
{
	if (!CIsValidClient(client, true, true))
		ThrowError("Invalid client index %d", client);
	
	char szBuffer[MAX_LENGTH_CHAT];
	char szCMessage[MAX_LENGTH_CHAT];

	SetGlobalTransTarget(client);
	
	Format(szBuffer, sizeof(szBuffer), "\x01%s", szMessage);
	VFormat(szCMessage, sizeof(szCMessage), szBuffer, 3);
	
	int index = CFormat(szCMessage, sizeof(szCMessage));
	
	if (index == NO_INDEX)
		PrintToChat(client, "%s", szCMessage);
	else
		CSayText2(client, index, szCMessage);
}

stock void CReplyToCommand(int client, const char[] szMessage, any ...)
{
	char szCMessage[MAX_LENGTH_CHAT];
	VFormat(szCMessage, sizeof(szCMessage), szMessage, 3);
	
	if (client == 0)
	{
		CRemoveTags(szCMessage, sizeof(szCMessage));
		PrintToServer("%s", szCMessage);
	}
	else if (GetCmdReplySource() == SM_REPLY_TO_CONSOLE)
	{
		CRemoveTags(szCMessage, sizeof(szCMessage));
		PrintToConsole(client, "%s", szCMessage);
	}
	else
	{
		CPrintToChat(client, "%s", szCMessage);
	}
}

/**
 * Prints a message to all clients in the chat area.
 * Supports color tags.
 *
 * @param client	  Client index.
 * @param szMessage   Message (formatting rules)
 * @return			  No return
 */
stock void CPrintToChatAll(const char[] szMessage, any ...)
{
	char szBuffer[MAX_LENGTH_CHAT];
	
	for (int i = 1; i <= MaxClients; i++)
	{
		if (CIsValidClient(i, false, true) && !CSkipList[i])
		{
			SetGlobalTransTarget(i);
			VFormat(szBuffer, sizeof(szBuffer), szMessage, 2);
			
			CPrintToChat(i, "%s", szBuffer);
		}
		
		CSkipList[i] = false;
	}
}

/**
 * Prints a message to a specific client in the chat area.
 * Supports color tags and teamcolor tag.
 *
 * @param client	  Client index.
 * @param author	  Author index whose color will be used for teamcolor tag.
 * @param szMessage   Message (formatting rules).
 * @return			  No return
 * 
 * On error/Errors:   If the client or author are not connected an error will be thrown.
 */
stock void CPrintToChatEx(int client, int author, const char[] szMessage, any ...)
{
	if (!CIsValidClient(client, true, true))
		ThrowError("Invalid client index %d", client);
	
	if (!CIsValidClient(client, true, true))
		ThrowError("Invalid client index %d", author);
	
	char szBuffer[MAX_LENGTH_CHAT];
	char szCMessage[MAX_LENGTH_CHAT];

	SetGlobalTransTarget(client);
	
	Format(szBuffer, sizeof(szBuffer), "\x01%s", szMessage);
	VFormat(szCMessage, sizeof(szCMessage), szBuffer, 4);
	
	int index = CFormat(szCMessage, sizeof(szCMessage), author);
	
	if (index == NO_INDEX)
		PrintToChat(client, "%s", szCMessage);
	else
		CSayText2(client, author, szCMessage);
}

/**
 * Prints a message to all clients in the chat area.
 * Supports color tags and teamcolor tag.
 *
 * @param author	  Author index whos color will be used for teamcolor tag.
 * @param szMessage   Message (formatting rules).
 * @return			  No return
 * 
 * On error/Errors:   If the author is not connected an error will be thrown.
 */
stock void CPrintToChatAllEx(int author, const char[] szMessage, any ...)
{
	if (!CIsValidClient(client, true, true))
		ThrowError("Invalid client index %d", author);
	
	char szBuffer[MAX_LENGTH_CHAT];
	
	for (int i = 1; i <= MaxClients; i++)
	{
		if (CIsValidClient(client, false, true) && !CSkipList[i])
		{
			SetGlobalTransTarget(i);
			VFormat(szBuffer, sizeof(szBuffer), szMessage, 3);
			
			CPrintToChatEx(i, author, "%s", szBuffer);
		}
		
		CSkipList[i] = false;
	}
}

/**
 * Removes color tags from the string.
 *
 * @param szMessage   String.
 * @return			  No return
 */
stock void CRemoveTags(char[] szMessage, int maxlength)
{
	for (int i = 0; i < MAX_COLORS; i++)
		ReplaceString(szMessage, maxlength, CTag[i], "", false);
	
	ReplaceString(szMessage, maxlength, "{teamcolor}", "", false);
}

/**
 * Replace the color with another color
 * Handle with care!
 *
 * @param color   			color to replace.
 * @param newColor   		color to replace with.
 * @noreturn
 */
stock void CReplaceColor(Colors color, Colors newColor)
{
	if (!CEventIsHooked)
		CEventIsHooked = true;
	
	CProfile_TeamIndex[color] = CProfile_TeamIndex[newColor];
	
	CTagReqSayText2[color] = CTagReqSayText2[newColor];
	Format(CTagCode[color], sizeof(CTagCode[]), CTagCode[newColor])
}

/**
 * This function should only be used right in front of
 * CPrintToChatAll or CPrintToChatAllEx and it tells
 * to those funcions to skip specified client when printing
 * message to all clients. After message is printed client will
 * no more be skipped.
 * 
 * @param client   Client index
 * @return		   No return
 */
stock void CSkipNextClient(int client)
{
	if (!CIsValidClient(client, true, true))
		ThrowError("Invalid client index %d", client);
	
	CSkipList[client] = true;
}

/**
 * Replaces color tags in a string with color codes
 *
 * @param szMessage   String.
 * @param maxlength   Maximum length of the string buffer.
 * @return			  Client index that can be used for SayText2 author index
 * 
 * On error/Errors:   If there is more then one team color is used an error will be thrown.
 */
stock int CFormat(char[] szMessage, int maxlength, int author=NO_INDEX)
{
	/* Hook event for auto profile setup on map start */
	if (!CEventIsHooked)
	{
		HookEvent("server_spawn", CEvent_MapStart, EventHookMode_PostNoCopy);
		CEventIsHooked = true;
	}
	
	int iRandomPlayer = NO_INDEX;
	
 	Format(szMessage, maxlength, " \x01\x0B\x01%s", szMessage);	// On CS:GO set invisible precolor
	ReplaceString(szMessage, maxlength, "{yellow}", "{TEAMCOLOR}", false);
	ReplaceString(szMessage, maxlength, "{lightpurple}", "{TEAMCOLOR}", false);
	
	/* If author was specified replace {teamcolor} tag */
	if (author != NO_INDEX)
	{
		ReplaceString(szMessage, maxlength, "{teamcolor}", "\x03", false);
		
		iRandomPlayer = author;
	}
	else
		ReplaceString(szMessage, maxlength, "{teamcolor}", "", false);

	/* For other color tags we need a loop */
	for (int i = 0; i < MAX_COLORS; i++)
	{
		/* If tag not found - skip */
		if (StrContains(szMessage, CTag[i], false) == -1)
			continue;
		/* If tag doesn't need saytext2 simply replace */
		else if (!CTagReqSayText2[i])
			ReplaceString(szMessage, maxlength, CTag[i], CTagCode[i], false);
		/* Tag needs saytext2 */
		else
		{
			/* If random player for tag wasn't specified replace tag and find player */
			if (iRandomPlayer == NO_INDEX)
			{
				/* Searching for valid client for tag */
				iRandomPlayer = CFindRandomPlayerByTeam(CProfile_TeamIndex[i]);
				
				/* If player not found replace tag with purple color tag */
				if (iRandomPlayer == NO_PLAYER)
					ReplaceString(szMessage, maxlength, CTag[i], CTagCode[Color_Purple], false);
				/* If player was found simply replace */
				else
					ReplaceString(szMessage, maxlength, CTag[i], CTagCode[i], false);
				
			}
			/* If found another team color tag throw error */
			else
			{
				ThrowError("Using two team colors in one message is not allowed");
			}
		}
	}
	
	return iRandomPlayer;
}

/**
 * Founds a random player with specified team
 *
 * @param color_team  Client team.
 * @return			  Client index or NO_PLAYER if no player found
 */
stock int CFindRandomPlayerByTeam(int color_team)
{
	if (color_team == SERVER_INDEX)
		return 0;
	else
	{
		for (int i = 1; i <= MaxClients; i++)
		{
			if (CIsValidClient(i, true, true) && GetClientTeam(i) == color_team)
				return i;
		}	
	}

	return NO_PLAYER;
}

/**
 * Sends a SayText2 usermessage to a client
 *
 * @param szMessage   Client index
 * @param maxlength   Author index
 * @param szMessage   Message
 * @return			  No return.
 */
stock void CSayText2(int client, int author, const char[] szMessage)
{
	Handle hBuffer = StartMessageOne("SayText2", client, USERMSG_RELIABLE|USERMSG_BLOCKHOOKS);
	
	if(GetFeatureStatus(FeatureType_Native, "GetUserMessageType") == FeatureStatus_Available && GetUserMessageType() == UM_Protobuf) 
	{
		PbSetInt(hBuffer, "ent_idx", author);
		PbSetBool(hBuffer, "chat", true);
		PbSetString(hBuffer, "msg_name", szMessage);
		PbAddString(hBuffer, "params", "");
		PbAddString(hBuffer, "params", "");
		PbAddString(hBuffer, "params", "");
		PbAddString(hBuffer, "params", "");
	}
	else
	{
		BfWriteByte(hBuffer, author);
		BfWriteByte(hBuffer, true);
		BfWriteString(hBuffer, szMessage);
	}
	
	EndMessage();
}

public Action CEvent_MapStart(Handle event, const char[] name, bool dontBroadcast)
{
	for (int i = 1; i <= MaxClients; i++)
		CSkipList[i] = false;
}

stock bool CIsValidClient(int iClient, bool bAllowBots = false, bool bAllowDead = true)
{
	if (!(1 <= iClient <= MaxClients) || !IsClientInGame(iClient) || (IsFakeClient(iClient) && !bAllowBots) || IsClientSourceTV(iClient) || IsClientReplay(iClient) || (!bAllowDead && !IsPlayerAlive(iClient)))
		return false;
	return true;
} 