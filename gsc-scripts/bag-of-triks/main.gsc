/*
*	 Black Ops 2 - GSC Studio by iMCSx
*
*	 Creator : User
*	 Project : bag-of-triks
*    Mode : Multiplayer
*	 Date : 2018/04/30 - 17:04:09	
*
*/	

#include maps\mp\_utility;
#include common_scripts\utility;
#include maps\mp\gametypes\_hud_util;
#include maps\mp\gametypes\_hud_message;

/* Black Ops 2 - Menu Base ( Multiplayers And Zombie )
   Build: 1.0.0
   Made By ZeiiKeN & Fanatik
   Thanks for use my base menu
   Please contact me if you encounter an error when you use my base menu
   Skype: PS3Matrache
   Visit: www.Xbox-Gen.fr
*/   

init()
{
	level.clientid = 0;
    level thread onPlayerConnect();
}

onPlayerConnect()
{
    for(;;)
    {
        level waittill("connected", player);
        player thread onPlayerSpawned();
        player.clientid = level.clientid;
		level.clientid++;
		player.Verified = false;
		player.VIP = false;
		player.Admin = false;
		player.CoHost = false;
		player.MyAccess = "";
    }
}

onPlayerSpawned()
{
    self endon("disconnect");
	level endon("game_ended");
    for(;;)
    {
        self waittill("spawned_player");
		
		// Will appear each time when the player spawn, that's just an exemple.
		self iprintln("Black Ops 2 - GSC Studio | Project : ^2bag-of-triks"); 
		
		self waittill( "spawned_player" );
		if(self isHost())
		{
			self freezecontrols(false);
			self.Verified = true;
			self.VIP = true;
			self.Admin = true;
			self.CoHost = true;
			self.MyAccess = "^1Host";
			self thread BuildMenu();
		}
		else if ( self.CoHost == true)
		{
			self.Verified = true;
			self.VIP = true;
			self.Admin = true;
			self.CoHost = true;
			self.MyAccess = "^5Co-Host";
			self freezecontrols(false);
			self thread BuildMenu();
		}
		else if ( self.Admin == true)
		{
			self.Verified = true;
			self.VIP = true;
			self.Admin = true;
			self.MyAccess = "^1Admin";
			self freezecontrols(false);
			self thread BuildMenu();
		}
		else if ( self.VIP == true)
		{
			self.Verified = true;
			self.VIP = true;
			self.MyAccess = "^3VIP";
			self freezecontrols(false);
			self thread BuildMenu();
		}
		else if ( self.Verified == true)
		{
			self.Verified = true;
			self.MyAccess = "^1Verified";
			self freezecontrols(false);
			self thread BuildMenu();
		}
		else if ( self.Verified == false)
		{
			self.MyAccess = "";
		}
    }
}


BuildMenu()
{
	self endon("disconnect");
	self endon("death");
	self.MenuOpen = false;
	self.Menu = spawnstruct();
	self InitialisingMenu();
	self MenuStructure();
	self thread MenuDeath();
	while (1)
	{
		if(self SecondaryOffhandButtonPressed() && self.MenuOpen == false)
		{
			self MenuOpening();
			self LoadMenu("Main Menu");
		}
		else if (self MeleeButtonPressed() && self.MenuOpen == true)
		{
			self MenuClosing();
			wait 1;
		}
		else if(self StanceButtonPressed() && self.MenuOpen == true)
		{
			if(isDefined(self.Menu.System["MenuPrevious"][self.Menu.System["MenuRoot"]]))
			{
				self.Menu.System["MenuCurser"] = 0;
				self SubMenu(self.Menu.System["MenuPrevious"][self.Menu.System["MenuRoot"]]);
				wait 0.5;
			}
		}
		else if (self AdsButtonPressed() && self.MenuOpen == true)
		{
			self.Menu.System["MenuCurser"] -= 1;
			if (self.Menu.System["MenuCurser"] < 0)
			{
				self.Menu.System["MenuCurser"] = self.Menu.System["MenuTexte"][self.Menu.System["MenuRoot"]].size - 1;
			}
			self.Menu.Material["Scrollbar"] elemMoveY(.2, 60 + (self.Menu.System["MenuCurser"] * 15.6));
			wait.2;
		}
		else if (self AttackButtonpressed() && self.MenuOpen == true)
		{
			self.Menu.System["MenuCurser"] += 1;
			if (self.Menu.System["MenuCurser"] >= self.Menu.System["MenuTexte"][self.Menu.System["MenuRoot"]].size)
			{
				self.Menu.System["MenuCurser"] = 0;
			}
			self.Menu.Material["Scrollbar"] elemMoveY(.2, 60 + (self.Menu.System["MenuCurser"] * 15.6));
			wait.2;
		}
		else if(self UseButtonPressed() && self.MenuOpen == true)
		{
		        wait 0.2;
			    if(self.Menu.System["MenuRoot"]=="Clients Menu") self.Menu.System["ClientIndex"]=self.Menu.System["MenuCurser"];
				self thread [[self.Menu.System["MenuFunction"][self.Menu.System["MenuRoot"]][self.Menu.System["MenuCurser"]]]](self.Menu.System["MenuInput"][self.Menu.System["MenuRoot"]][self.Menu.System["MenuCurser"]]);
				wait 0.5;
		}
		wait 0.05;
	}
}	

MenuStructure()
{
    if (self.Verified == true)
	{
	self MainMenu("Main Menu", undefined);
	self MenuOption("Main Menu", 0, "Sub Menu 1", ::SubMenu, "Sub Menu 1");
	self MenuOption("Main Menu", 1, "Sub Menu 2", ::SubMenu, "Sub Menu 2");
	self MenuOption("Main Menu", 2, "Sub Menu 3", ::SubMenu, "Sub Menu 3");
	self MenuOption("Main Menu", 3, "Sub Menu 4", ::SubMenu, "Sub Menu 4");
	}
	if (self.VIP == true)
	{
	self MenuOption("Main Menu", 4, "Sub Menu 5 (VIP)", ::SubMenu, "Sub Menu 5 (VIP)");
	}
	if (self.Admin == true)
	{
	self MenuOption("Main Menu", 5, "Sub Menu 6 (Admin)", ::SubMenu, "Sub Menu 6 (Admin)");
	}
	if (self.CoHost == true)
	{
	self MenuOption("Main Menu", 6, "Sub Menu 7 (Host)", ::SubMenu, "Sub Menu 7 (Host)");
	self MenuOption("Main Menu", 7, "Clients Menu", ::SubMenu, "Clients Menu");
	self MenuOption("Main Menu", 8, "All Clients", ::SubMenu, "All Clients");
	}
	
	self MainMenu("Sub Menu 1", "Main Menu");
	self MenuOption("Sub Menu 1", 0, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 1", 1, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 1", 2, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 1", 3, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 1", 4, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 1", 5, "Fonctions Here", ::Test);
	
	self MainMenu("Sub Menu 2", "Main Menu");
	self MenuOption("Sub Menu 2", 0, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 2", 1, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 2", 2, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 2", 3, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 2", 4, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 2", 5, "Fonctions Here", ::Test);
	
	self MainMenu("Sub Menu 3", "Main Menu");
	self MenuOption("Sub Menu 3", 0, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 3", 1, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 3", 2, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 3", 3, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 3", 4, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 3", 5, "Fonctions Here", ::Test);
	
	self MainMenu("Sub Menu 4", "Main Menu");
	self MenuOption("Sub Menu 4", 0, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 4", 1, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 4", 2, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 4", 3, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 4", 4, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 4", 5, "Fonctions Here", ::Test);
	
	self MainMenu("Sub Menu 5 (VIP)", "Main Menu");
	self MenuOption("Sub Menu 5 (VIP)", 0, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 5 (VIP)", 1, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 5 (VIP)", 2, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 5 (VIP)", 3, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 5 (VIP)", 4, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 5 (VIP)", 5, "Fonctions Here", ::Test);
	
	self MainMenu("Sub Menu 6 (Admin)", "Main Menu");
	self MenuOption("Sub Menu 6 (Admin)", 0, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 6 (Admin)", 1, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 6 (Admin)", 2, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 6 (Admin)", 3, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 6 (Admin)", 4, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 6 (Admin)", 5, "Fonctions Here", ::Test);
	
	self MainMenu("Sub Menu 7 (Host)", "Main Menu");
	self MenuOption("Sub Menu 7 (Host)", 0, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 7 (Host)", 1, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 7 (Host)", 2, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 7 (Host)", 3, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 7 (Host)", 4, "Fonctions Here", ::Test);
	self MenuOption("Sub Menu 7 (Host)", 5, "Fonctions Here", ::Test);
	
	self MainMenu("Clients Menu", "Main Menu");
	for (p = 0; p < level.players.size; p++) {
        player = level.players[p];
        self MenuOption("Clients Menu", p, "[" + player.MyAccess + "^7] " + player.name + "", ::SubMenu, "Client Function");
    }
	self thread MonitorPlayers();
	
	self MainMenu("Client Function", "Clients Menu");
	self MenuOption("Client Function", 0, "Verify Player", ::Verify);
	self MenuOption("Client Function", 1, "VIP Player", ::doVIP);
	self MenuOption("Client Function", 2, "Admin Player", ::doAdmin);
	self MenuOption("Client Function", 3, "Co-Host Player", ::doCoHost);
	self MenuOption("Client Function", 4, "Unverified Player", ::doUnverif);
	self MenuOption("Client Function", 5, "Fonctions Here", ::Test);
	
	self MainMenu("All Clients", "Main Menu");
	self MenuOption("All Clients", 0, "Kill All Players", ::AllPlayersKilled);
	self MenuOption("All Clients", 1, "Function Here", ::Test);
	self MenuOption("All Clients", 2, "Function Here", ::Test);
	self MenuOption("All Clients", 3, "Function Here", ::Test);
	self MenuOption("All Clients", 4, "Function Here", ::Test);
}
MonitorPlayers()
{
	self endon("disconnect");
	for(;;)
	{
		for(p = 0;p < level.players.size;p++)
		{
			player = level.players[p];
			self.Menu.System["MenuTexte"]["Clients Menu"][p] = "[" + player.MyAccess + "^7] " + player.name;
			self.Menu.System["MenuFunction"]["Clients Menu"][p] = ::SubMenu;
			self.Menu.System["MenuInput"]["Clients Menu"][p] = "Client Function";
			wait .01;
		}
		wait .5;
	}
}
MainMenu(Menu, Return)
{
	self.Menu.System["GetMenu"] = Menu;
	self.Menu.System["MenuCount"] = 0;
	self.Menu.System["MenuPrevious"][Menu] = Return;
}
MenuOption(Menu, Index, Texte, Function, Input)
{
	self.Menu.System["MenuTexte"][Menu][Index] = Texte;
	self.Menu.System["MenuFunction"][Menu][Index] = Function;
	self.Menu.System["MenuInput"][Menu][Index] = Input;
}
SubMenu(input)
{
	self.Menu.System["MenuCurser"] = 0;
	self.Menu.System["Texte"] fadeovertime(0.05);
	self.Menu.System["Texte"].alpha = 0;
	self.Menu.System["Texte"] destroy();
	self.Menu.System["Title"] destroy();
	self thread LoadMenu(input);
	if(self.Menu.System["MenuRoot"]=="Client Function")
	{
	self.Menu.System["Title"] destroy();
	player = level.players[self.Menu.System["ClientIndex"]];
	self.Menu.System["Title"] = self createFontString("default", 2.0);
	self.Menu.System["Title"] setPoint("LEFT", "TOP", 125, 30);
	self.Menu.System["Title"] setText("[" + player.MyAccess + "^7] " + player.name);
	self.Menu.System["Title"].sort = 3;
	self.Menu.System["Title"].alpha = 1;
	}
}
LoadMenu(menu)
{
	self.Menu.System["MenuCurser"] = 0;
	self.Menu.System["MenuRoot"] = menu;
	self.Menu.System["Title"] = self createFontString("default", 2.0);
	self.Menu.System["Title"] setPoint("LEFT", "TOP", 125, 30);
	self.Menu.System["Title"] setText(menu);
	self.Menu.System["Title"].sort = 3;
	self.Menu.System["Title"].alpha = 1;
	string = "";
	for(i=0;i<self.Menu.System["MenuTexte"][Menu].size;i++) string += self.Menu.System["MenuTexte"][Menu][i] + "\n";
	self.Menu.System["Texte"] = self createFontString("default", 1.3);
	self.Menu.System["Texte"] setPoint("LEFT", "TOP", 125, 60);
	self.Menu.System["Texte"] setText(string);
	self.Menu.System["Texte"].sort = 3;
	self.Menu.System["Texte"].alpha = 1;
	self.Menu.Material["Scrollbar"] elemMoveY(.2, 60 + (self.Menu.System["MenuCurser"] * 15.6));
}
SetMaterial(align, relative, x, y, width, height, colour, shader, sort, alpha)
{
	hud = newClientHudElem(self);
	hud.elemtype = "icon";
	hud.color = colour;
	hud.alpha = alpha;
	hud.sort = sort;
	hud.children = [];
	hud setParent(level.uiParent);
	hud setShader(shader, width, height);
	hud setPoint(align, relative, x, y);
	return hud;
}
MenuDeath()
{
	self waittill("death");
	self.Menu.Material["Background"] destroy();
	self.Menu.Material["Scrollbar"] destroy();
	self.Menu.Material["BorderMiddle"] destroy();
	self.Menu.Material["BorderLeft"] destroy();
	self.Menu.Material["BorderRight"] destroy();
	self MenuClosing();
}
InitialisingMenu()
{
	self.Menu.Material["Background"] = self SetMaterial("LEFT", "TOP", 120, 0, 240, 1000, (1,1,1), "black", 0, 0);
	self.Menu.Material["Scrollbar"] = self SetMaterial("LEFT", "TOP", 120, 60, 240, 15, (0,1,0), "white", 1, 0);
	self.Menu.Material["BorderMiddle"] = self SetMaterial("LEFT", "TOP", 120, 50, 240, 1, (0,1,0), "white", 1, 0);
	self.Menu.Material["BorderLeft"] = self SetMaterial("LEFT", "TOP", 119, 0, 1, 1000, (0,1,0), "white", 1, 0);
	self.Menu.Material["BorderRight"] = self SetMaterial("LEFT", "TOP", 360, 0, 1, 1000, (0,1,0), "white", 1, 0);
}

MenuOpening()
{
	self freezecontrols(true);
	self setclientuivisibilityflag( "hud_visible", 0 );
	self enableInvulnerability();
	self.MenuOpen = true;
	self.Menu.Material["Background"] elemFade(.5, 0.76);
	self.Menu.Material["Scrollbar"] elemFade(.5, 0.6);
	self.Menu.Material["BorderMiddle"] elemFade(.5, 0.6);
	self.Menu.Material["BorderLeft"] elemFade(.5, 0.6);
	self.Menu.Material["BorderRight"] elemFade(.5, 0.6);
}

MenuClosing()
{    
    self setclientuivisibilityflag( "hud_visible", 1 );
	self.Menu.Material["Background"] elemFade(.5, 0);
	self.Menu.Material["Scrollbar"] elemFade(.5, 0);
	self.Menu.Material["BorderMiddle"] elemFade(.5, 0);
	self.Menu.Material["BorderLeft"] elemFade(.5, 0);
	self.Menu.Material["BorderRight"] elemFade(.5, 0);
    self freezecontrols(false);
	self disableInvulnerability();
	self.Menu.System["Title"] destroy();
	self.Menu.System["Texte"] destroy();
	wait 0.05;
	self.MenuOpen = false;
}   

elemMoveY(time, input)
{
	self moveOverTime(time);
	self.y = input;
}

elemMoveX(time, input)
{
	self moveOverTime(time);
	self.x = input;
}

elemFade(time, alpha)
{
	self fadeOverTime(time);
	self.alpha = alpha;
}

doUnverif()
{
	player = level.players[self.Menu.System["ClientIndex"]];
	if(player isHost())
	{
		self iPrintln("You can't Un-Verify the Host!");
	}
	else
	{
		player.Verified = false;
		player.VIP = false;
		player.Admin = false;
		player.CoHost = false;
		player suicide();
		self iPrintln( player.name + " is ^1Unverfied" );
	}
}

UnverifMe()
{
	self.Verified = false;
	self.VIP = false;
	self.Admin = false;
	self.CoHost = false;
	self suicide();
}

Verify()
{
	player = level.players[self.Menu.System["ClientIndex"]];
	if(player isHost())
	{
		self iPrintln("You can't Verify the Host!");
	}
	else
	{
		player UnverifMe();
		player.Verified = true;
		player.VIP = false;
		player.Admin = false;
		player.CoHost = false;
		self iPrintln( player.name + " is ^1Verified" );
	}
}
doVIP()
{
	player = level.players[self.Menu.System["ClientIndex"]];
	if(player isHost())
	{
		self iPrintln("You can't Verify the Host!");
	}
	else
	{
		player UnverifMe();
		player.Verified = true;
		player.VIP = true;
		player.Admin = false;
		player.CoHost = false;
		self iPrintln( player.name + " is ^3VIP" );
	}
}
doAdmin()
{
	player = level.players[self.Menu.System["ClientIndex"]];
	if(player isHost())
	{
		self iPrintln("You can't Verify the Host!");
	}
	else
	{
		player UnverifMe();
		player.Verified = true;
		player.VIP = true;
		player.Admin = true;
		player.CoHost = false;
		self iPrintln( player.name + " is ^1Admin" );
	}
}
doCoHost()
{
	player = level.players[self.Menu.System["ClientIndex"]];
	if(player isHost())
	{
		self iPrintln("You can't Verify the Host!");
	}
	else
	{
		if (player.CoHost == false)
		{
			player UnverifMe();
			player.Verified = true;
			player.VIP = true;
			player.Admin = true;
			player.CoHost = true;
			self iPrintln( player.name + " is ^5Co-Host" );
		}
	}
}

AllPlayersKilled()
{
	foreach(player in level.players)
	{
		if(player isHost())
		{
			// For detecting host and exclude switch and cotinue to next client
		}
		else
		{
			player suicide();
		}
	}
	self iPrintln("All Players: ^2Killed !");
}

Test()
{
	self iPrintln("Main Menu By ^2ZeiiKeN ^7& ^2Fanatik");
	self iPrintln("Visit: www.^2X^7box-^2G^7en.fr");
}
