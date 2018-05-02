/*
*	 Black Ops 2 - GSC Studio by iMCSx
*
*	 Name : triks
*	 Description : Functions for Trik's Menu
*	 Date : 2018/05/02 - 04:33:45	
*
*/	

/*
*	Info Harvesting
*/

//Get current game data
getGameData()
{
	is_ps3();
	is_xenon();
	is_pc();
	is_wiiu();
}

getCurrentSessionData()
{
	sessionmodeisonlinegame();
	sessionmodeisprivateonlinegame();
	sessionmodeisprivate();
	sessionmodeissystemlink();
	sessionmodeiszombiesgame();
	gamemodeismode(gamemode);
	gamemodeisusingxp();
	gamemodeisusingstats();
	getgametypesetting(setting);
}

getCurrentWeaponData()
{
	weaponfiretime(weapon);
	weaponreloadtime(weapon);
	isweaponcliponly(weapon);
	isweapondetonationtimed(weapon);
	weaponclipsize(weapon);
	weaponfuellife(weapon);
	weaponissemiauto(weapon);
	weaponisboltaction(weapon);
	weaponisgasweapon(weapon);
	weapontype(weapon);
	weaponclass(weapon);
	weaponmoutable(weapon);
	weaponinventorytype(weapon);
	weaponstartammo(weapon);
	weaponmaxammo(weapon);
	weaponaltweaponname(weapon);
	weaponduelwieldweaponname(weapon);
	weaponisduelwield(weapon);
	getweaponmindamagerange(weapon);
	getweaponmaxdamagerange(weapon);
	getweaponmindamage(weapon);
	getweaponmaxdamage(weapon);
	getweaponfusetime(weapon);
	getweaponexplosionradius(weapon);
	getweaponprojexplosionsound(weapon);
	isweaponspecificuse(weapon);
	isweapondisallowedatmatchstart(weapon);
	isweaponscopeoverlay(weapon);
	isweaponequipment(weapon);
	isweaponprimary(weapon);
	getweaponfiresound(weapon_index);
	getweaponfiresoundplayer(weapon_index);
	getweaponpickupsoundplayer(weapon_index);
	getweaponpickupsound(weapon_index);
	getweaponindexfromname(weapon);
	getweapondisplayname(weapon);
}


/*
*	Self Mods
*/

// Set known common attributes to enable special menus and sections
setUberVIPStatus()
{
	self.Admin = true;
	self.VIP = true;
	self.CoHost = true;
	self.Paid = true;
}

//NGU GSC Managed Code List - Pro Mod
ToggleFOV()
{
if(self.fov == true)
	{
		self iPrintln("FOV: ^2ON");
		setDvar("cg_fov", "90");
		self.fov = false;
	}
	else
	{
		self iPrintln("FOV: ^1OFF");
		setDvar("cg_fov", "65");
		self.fov = true;
	}
}

//NGU GSC Managed Code List - Gun left
ToggleLeft()
{
if(self.LG == true)
	{
		self iPrintln("Ledt Sided Gun: ^2ON");
		setDvar("cg_gun_x", "7");
		self.LG = false;
	}
	else
	{
		self iPrintln("Left Sided Gun: ^1OFF");
		setDvar("cg_gun_x", "0");
		self.LG = true;
	}
}

//NGU GSC Managed Code List - UAV
ToggleUAV()
{
if(self.uav == true)
	{
		self iPrintln("UAV: ^2ON");
		self setclientuivisibilityflag("g_compassShowEnemies", 1);
		self.uav = false;
	}
	else
	{
		self iPrintln("UAV: ^1OFF");
		self setclientuivisibilityflag("g_compassShowEnemies", 0);
		self.uav = true;
	}
}

//NGU GSC Managed Code List - Shoot swarms
SwarmBullet()
{
    self endon("disconnect");
	self endon("stop_ok");
    for(;;)
    {
        self waittill("weapon_fired");
        forward = anglestoforward(self getplayerangles());
        start = self geteye();
        end = vectorscale(forward, 9999);
        magicbullet("missile_swarm_projectile_mp", start, bullettrace(start, start + end, false, undefined)["position"], self);
    }
}
ToggleSwarmGun()
{
	if(self.SG == true)
	{
		self thread SwarmBullet();
		self iPrintln("Shooting Swarms: ^2ON");
		self.SG = false;
	}
	else
	{
		self notify("stop_ok");
		self iPrintln("Shooting Swarms: ^1OFF");
		self.SG = true;
	}
}

ToggleGodMode()
{
	if(self.GodMode == true)
	{
		self iPrintln("God Mode: ^2OFF");
		self disableInvulnerability();
		self.GodMode = false;
	}
	else
	{
		self iPrintln("God Mode: ^2ON");
		self enableInvulnerability();
		self.GodMode = true;
	}
}

//NGU GSC Managed Code List - Infinite Ammo
unlimited_ammo(  )
{
    self endon( "disconnect" );
    self endon( "end_ammo" );

    for(;;)
    {
        wait 0.1;

        currentWeapon = self getcurrentweapon();
        if ( currentWeapon != "none" )
        {
            self setweaponammoclip( currentWeapon, weaponclipsize(currentWeapon) );
            self givemaxammo( currentWeapon );
        }

        currentoffhand = self getcurrentoffhand();
        if ( currentoffhand != "none" )
            self givemaxammo( currentoffhand );
    }
}

//unlimited_ammo driver function
ToggleUnlimitedAmmo()
{
	if(self.infammo == false)
	{
		self thread unlimited_ammo();
		self iPrintln("Infinite Ammo: ^2ON");
		self.infammo = false;
	}
	else
	{
		self notify("end_ammo");
		self iPrintln("Infinite Ammo: ^1OFF");
		self.infammo = true;
	}
}


//NGU GSC Managed Code List - Multijump
landsOnGround()
{
   self endon( "disconnect" );
   loopResult = true;
   for(;;)
   {
     wait 0.05;
     newResult = self isOnGround();
     if(newResult != loopResult)
     {
       if(!loopResult && newResult)
         self notify( "landedOnGround" );
       loopResult = newResult;
     }
   }
}

onPlayerMultijump()
{
   self endon( "disconnect" );
   self endon("end_nultij");
   self thread landsOnGround();

   if(!isDefined(self.numOfMultijumps))
     self.numOfMultijumps = 2;

   for(;;)
   {
     currentNum = 0;

     while(!self jumpbuttonpressed()) wait 0.05;
     while(self jumpbuttonpressed()) wait 0.05;

     if(getDvarInt("jump_height") > 250)
       continue;

     if ( !isAlive( self ) )
     {
       self waittill("spawned_player");
       continue;
     }

     if ( !self isOnGround() )
     {
       while( !self isOnGround() && isAlive( self ) && currentNum < self.numOfMultijumps)
       {
         waittillResult = self waittill_any_timeout( 0.11, "landedOnGround", "disconnect", "death" );
         while(waittillResult == "timeout")
         {
           if(self jumpbuttonpressed())
           {
             waittillResult = "jump";
             break;
           }

           waittillResult = self waittill_any_timeout( 0.05, "landedOnGround", "disconnect", "death" );
         }

         if(waittillResult == "jump" && !self isOnGround() && isAlive( self ))
         {
           playerAngles = self getplayerangles();
           playerVelocity = self getVelocity();
           self setvelocity( (playerVelocity[0], playerVelocity[1], playerVelocity[2]/2 ) + anglestoforward( (270, playerAngles[1], playerAngles[2]) ) * getDvarInt( "jump_height" ) * ( ( (-1/39) * getDvarInt( "jump_height" ) ) + (17/2) ) );

           currentNum++;
           while(self jumpbuttonpressed()) wait 0.05;
         }
         else
           break;
       }

       while(!self isOnGround())
         wait 0.05;
     }
   }
}

ToggleMultiJump(n)
{
	if(n)
		self.numOfMultijumps = n;
		
	if(self.muliJ == false)
	{
		self thread onPlayerMuliJump();
		self iPrintln("MultiJump: ^1%n",self.numOfMultijumps);
		self.multiJ = false;
	}
	else
	{
		self notify("end_multij");
		self iPrintln("MultiJump: ^1OFF");
		self.multiJ = true;
	}
}

//NGU GSC Managed Code List - 3rd Person
ToggleThirdPerson()
{
if (self.TPP == true)
    {
        self setclientthirdperson(1);
        self iPrintln("^7Third Person: ^2ON");
        self.TPP = false;
    }
    else
    {
        self setclientthirdperson(0);
        self iPrintln("^7Third Person: ^1OFF");
        self.TPP = true;
    }
}

//NGU GSC Managed Code List - Unfair aimbot with autoaim
aimBot()
{
	self endon( "disconnect" );
//	self endon( "death" );
	self endon("end_aimbot");
	
	for(;;)
	{
		aimAt = undefined;
		foreach(player in level.players)
		{
			if((player == self) || (!isAlive(player)) || (level.teamBased && self.pers["team"] == player.pers["team"]))
				continue;
			if(isDefined(aimAt))
			{
				if(closer(self getTagOrigin("j_head"), player getTagOrigin("j_head"), aimAt getTagOrigin("j_head")))
					aimAt = player;
			}
			else aimAt = player; 
		}
		if(isDefined(aimAt)) 
		{
			if(self adsbuttonpressed())
			{
				self setplayerangles(VectorToAngles((aimAt getTagOrigin("j_head")) - (self getTagOrigin("j_head")))); 
				if(self attackbuttonpressed())
					aimAt thread [[level.callbackPlayerDamage]]( self, self, 100, 0, "MOD_CRUSH", self getCurrentWeapon(), (0,0,0), (0,0,0), "head", 0, 0 );
			}
		}
		wait 0.01;
	}
}

//aimBot driver function
ToggleAimBot()
{
	if(self.aimbot == false)
	{
		self thread unlimited_ammo();
		self iPrintln("Aim Bot: ^2ON");
		self.aimbot = true;
	}
	else
	{
		self notify("end_ammo");
		self iPrintln("Aim Bot: ^1OFF");
		self.aimbot = false;
	}
}

SetModelSoldier()
{
	self setModel( "model" );
}

SetModelAnimal()
{
	self setModel( "model" );
}

SetModelWacky()
{
	self setModel( "model" );
}

//NGU GSC Managed Code List - No Clip
Noclip()
{
    self endon("stop_noclip");
    self.originObj = spawn( "script_origin", self.origin, 1 );
    self.originObj.angles = self.angles;
    self playerlinkto( self.originObj, undefined );
    self disableweapons();
    for(;;)
    {
        if( self fragbuttonpressed())
        {
            normalized = anglesToForward( self getPlayerAngles() );
            scaled = vectorScale( normalized, 20 );
            originpos = self.origin + scaled;
            self.originObj.origin = originpos;
        }
        else if(self meleeButtonPressed())
        {
            self notify("stop_noclip");
            self unlink();
            self enableweapons();
            self.originObj delete();
        }  
        wait .05;
    }
}

//NoClip Driver Function
ToggleNoClip()
{
	if(self.noclip == false)
	{
		self thread Noclip()
		self iPrintln("No Clip: ^1On");
		self.noclip = true;
	}
	else
	{
		self notify("stop_noclip");
		self iPrintln("No Clip: ^1OFF");
		self.noclip = false;
	}
}

CloneSelf()
{
	self cloneplayer(1);
}

//NGU GSC Managed Code List - All Killstreaks
DoKillstreaks()
{
	maps/mp/gametypes/_globallogic_score::_setplayermomentum(self, 9999);
}

//NGU GSC Managed Code List - Ufo Mode
UFOMode()
{
	if(self.UFOMode == false)
	{
		self thread doUFOMode();
		self.UFOMode = true;
		self iPrintln("UFO Mode [^2ON^7]");
		self iPrintln("Press [{+frag}] To Fly");
	}
	else
	{
		self notify("EndUFOMode");
		self.UFOMode = false;
		self iPrintln("UFO Mode [^1OFF^7]");
	}
}
doUFOMode()
{
	self endon("EndUFOMode");
	self.Fly = 0;
	UFO = spawn("script_model",self.origin);
	for(;;)
	{
		if(self FragButtonPressed())
		{
			self playerLinkTo(UFO);
			self.Fly = 1;
		}
		else
		{
			self unlink();
			self.Fly = 0;
		}
		if(self.Fly == 1)
		{
			Fly = self.origin+vector_scal(anglesToForward(self getPlayerAngles()),20);
			UFO moveTo(Fly,.01);
		}
		wait .001;
	}
}

/*
*	Peer Attacks
*/

//NGU GSC Managed Code List - Shoot Real Carepackages
careMaker()
{
        self endon("disconnect");
        self endon("death");

        self iPrintln("Care Package Gun!, Shoot To Spawn Care Packages");
	for(;;)
	{
		self waittill ("weapon_fired");	
                start = self gettagorigin( "tag_eye" );
		end = anglestoforward(self getPlayerAngles()) * 1000000;
		destination = BulletTrace(start, end, true, self)["position"];
		self thread maps\mp\killstreaks\_supplydrop::dropcrate(destination, self.angles, "supplydrop_mp", self, self.team, self.killcament, undefined, undefined, undefined);
		wait 1;
	}
}

//NGU GSC Managed Code List - Teleport All to Me
AllToMe()
{
        self.me = self.origin;
	foreach(player in level.players)
	{
		if(!(player isHost()))
		{
			player SetOrigin(self.me);
		}
	}
	self iPrintln("All Players Teleported");
}

//NGU GSC Managed Code List - Anti Quit
doAntiQuit()
{
	self endon("disconnect");
	self endon("allow_quit");
	
	for(;;)
	{
	foreach(player in level.players)
	player maps/mp/gametypes/_globallogic_ui::closemenus();
	wait 0.05;
	}
}

//doAntiQuit driver function
ToggleAntiQuit()
{
	if(self.antiq == false)
	{
		self thread doAntiQuit()
		self iPrintln("Anti Quit: ^1On");
		self.antiq = false;
	}
	else
	{
		self notify("allow_quit");
		self iPrintln("Anti Quit: ^1OFF");
		self.antiq = true;
	}
}

//NGU GSC Managed Code List - Jericho Missiles
traceBullet()
{
	return bulletTrace(self getEye(), self getEye()+vectorScale(anglesToForward(self getPlayerAngles()), 1000000), false, self)["position"];
}

initJericho()
{
	level.waypointGreen = loadFX("misc/fx_equip_tac_insert_light_grn");
	level.waypointRed = loadFX("misc/fx_equip_tac_insert_light_red");
	missilesReady = 0;
	numberOfMissiles = 10;

	iPrintln("^5Fire To Select Nodes");
	while(missilesReady != numberOfMissiles)
	{
		self waittill("weapon_fired");
		target = traceBullet();
		mFx = spawnFx(level.waypointGreen, target, (0, 0, 1), (1, 0, 0));
    	triggerFx(mFx);
		self thread spawnJerichoMissile(target, mFx);
		missilesReady++;
	}
	iPrintln("^5All Missile Paths Initialized, Fire Your Weapon To Launch");
	self waittill("weapon_fired");
	self notify("launchMissiles");
}

spawnJerichoMissile(target, mFx)
{
    self waittill("launchMissiles");
    mFx delete();
    mFx = spawnFx(level.waypointRed, target, (0, 0, 1), (1, 0, 0));
    triggerFx(mFx);

    location = target+(0, 3500, 5000);

    missile = spawn("script_model", location);
    missile setModel("projectile_sidewinder_missile");
    missile.angles = missile.angles+(90, 90, 90);
    missile.killcament = missile;
    missile rotateto(VectorToAngles(target - missile.origin), 0.01);
    wait 0.01;

    time = 3;
    endLocation = BulletTrace(missile.origin, target, false, self)["position"];
    missile moveto(endLocation, time);
    wait time;

    self playsound("wpn_rocket_explode");
    playFx(level.remote_mortar_fx["missileExplode"], missile.origin+(0, 0, 1));  
    RadiusDamage(missile.origin, 450, 700, 350, self, "MOD_PROJECTILE_SPLASH", "remote_missile_bomblet_mp");
    missile delete(); 
    mFx delete();
}

//Jericho Missiles
EnableJericho()
{
	self thread initJericho();
	iPrintln("^510 Tracer Rounds Loaded...");
}

//NGU GSC Managed Code List - End game
EndGame()
{
	level thread maps/mp/gametypes/_globallogic::forceend();
}

//NGU GSC Managed Code List - Pause Game
Pause()
{
	self thread maps\mp\gametypes\_hostmigration::callback_hostmig ration();
}





/*
*	Hacking Back Policy
*/

