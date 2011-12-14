#include "zcommon.acs"
#library "rjpack"

#include "rjConstants.h"
#include "rjFunctions.c"

int startPoints[32][5];
int spawnPoints[32][6];
int beaconTIDs[32][BEACON_COUNT][MAX_BEACONS];
int joinScriptRunning[32];

script RJPACK_OPEN open
{
    if (GetCVar("rjpack_version") != 1)
    {
        ConsoleCommand("set rjpack_version 1");
        ConsoleCommand("set rjpack_power   128");

        ConsoleCommand("archivecvar rjpack_version");
        ConsoleCommand("archivecvar rjpack_power");
    }
}

script RJPACK_JOIN enter
{
    ACS_ExecuteAlways(RJPACK_SETSPAWN, 0, 2,0,0);
    ACS_ExecuteAlways(RJPACK_SPAWN_COMMON, 0, 0,0,0);
}

script RJPACK_RESPAWN respawn
{
    ACS_ExecuteAlways(RJPACK_GOTOSPAWN, 0, 1,0,0);
    ACS_ExecuteAlways(RJPACK_SPAWN_COMMON, 0, 0,0,0);
}


script RJPACK_SPAWN_COMMON (void)
{
    int pln = PlayerNumber();
    int rjn = CheckInventory("RJForever");
    int i;
    int onGround; int oldGround;
    
    if (joinScriptRunning[pln] || (rjn == 0) )
    {
        terminate;
    }


    joinScriptRunning[pln] = 1;

    while (PlayerInGame(pln) && CheckInventory("RJForever") == rjn)
    {
        if (GetActorProperty(0, APROP_Health) <= 0)
        {
            break;
        }
        
        GiveInventory("OneSecondInvuln", 1);

        if (rjn == 1)
        {
            GiveInventory("Rocket~Launcher", 1);
        }
        else
        {
            GiveInventory("Instant-Rocket~Launcher", 1);
        }
        
        GiveInventory("Super~Chaingun", 1);
        GiveInventory("Super~Railgun", 1);
        GiveInventory("Beacon~Layer", 1);

        oldGround = onGround;
        onGround = (GetActorZ(0) - GetActorFloorZ(0) == 0);

        if ((oldGround != onGround) && !CheckInventory("NoSmartSwitch"))
        {
            TakeInventory("SpawnSetter", 1);
            TakeInventory("SpawnItem", 1);
            
            if (onGround == 1)
            {
                GiveInventory("SpawnSetter", 1);
                GiveInventory("SpawnItem", 1);
            }
            else
            {
                GiveInventory("SpawnItem", 1);
                GiveInventory("SpawnSetter", 1);
            }
            
            TakeInventory("RemoveBeacons", 1);
            TakeInventory("ToggleSwitch", 1);
            TakeInventory("TogglePickups", 1);
            TakeInventory("SpawnUnsetter", 1);
            GiveInventory("SpawnUnsetter", 1);
            GiveInventory("TogglePickups", 1);
            GiveInventory("ToggleSwitch", 1);
            GiveInventory("RemoveBeacons", 1);
        }
        else
        {
            GiveInventory("ToggleSwitch", 1);
            GiveInventory("TogglePickups", 1);
            GiveInventory("SpawnItem", 1);
            GiveInventory("SpawnSetter", 1);
            GiveInventory("SpawnUnsetter", 1);
            GiveInventory("RemoveBeacons", 1);
        }

        if (i % 3 == 0)
        {
            GiveInventory("SuperRailCounter", 1);
            i = 0;
        }
        
        if (GetActorProperty(0, APROP_Health) == 0)
        {
            break;
        }
        Delay(1);
        i++;
    }
    
    joinScriptRunning[pln] = 0;
}

script RJPACK_DEATH death
{
    TakeInventory("OneSecondInvuln", 1);
}

script RJPACK_DISCONNECT (int pln) disconnect
{
    joinScriptRunning[pln] = 0;
    ACS_ExecuteAlways(RJPACK_SETSPAWN, 0, 3,0,0);
    ACS_ExecuteAlways(RJPACK_MAKEBEACON, 0, 3,0,0);
}



script RJPACK_SETSPAWN (int whichMode)
{
    int pln = PlayerNumber();

    switch (whichMode)
    {
        case 0:
            if (GetActorZ(0) - GetActorFloorZ(0) != 0)
            {
                Print(s:"You can only set your spawn point when on the ground");
                break;
            }
            
            spawnPoints[pln][0] = 1;
            spawnPoints[pln][1] = GetActorX(0);
            spawnPoints[pln][2] = GetActorY(0);
            spawnPoints[pln][3] = GetActorZ(0);
            spawnPoints[pln][4] = GetActorAngle(0);
            spawnPoints[pln][5] = GetActorPitch(0);

            Print(s:"Spawn point set");
            break;

        case 1:
            spawnPoints[pln][0] = 0;
            spawnPoints[pln][1] = 0;
            spawnPoints[pln][2] = 0;
            spawnPoints[pln][3] = 0;
            spawnPoints[pln][4] = 0;
            spawnPoints[pln][5] = 0;
            
            Print(s:"Spawn point unset");
            break;

        case 2:
            startPoints[pln][0] = GetActorX(0);
            startPoints[pln][1] = GetActorY(0);
            startPoints[pln][2] = GetActorZ(0);
            startPoints[pln][3] = GetActorAngle(0);
            startPoints[pln][4] = GetActorPitch(0);
            break;

        case 3:
            spawnPoints[pln][0] = 0;
            spawnPoints[pln][1] = 0;
            spawnPoints[pln][2] = 0;
            spawnPoints[pln][3] = 0;
            spawnPoints[pln][4] = 0;
            spawnPoints[pln][5] = 0;
            break;
    }
}


script RJPACK_CHECKAMMO (int whichClass)
{
    switch (whichClass)
    {
        case 0:
            SetResultValue(CheckInventory("SuperRailCounter") );
            break;
    }
}

script RJPACK_GETPOWER (void)
{
    SetResultValue(GetCVar("rjpack_power"));
}

script RJPACK_PROJ_CHECKAMMO (int which)
{
    SetActivatorToTarget(0);
    SetResultValue(ACS_ExecuteWithResult(RJPACK_CHECKAMMO, which) );
}

script RJPACK_GOTOSPAWN (int spawned)
{
    int pln = PlayerNumber();
    
    if (spawnPoints[pln][0] == 1)
    {
        SetActorVelocity(0, 0,0,0, 0, 0);
        SetActorAngle(0, spawnPoints[pln][4]);
        SetActorPitch(0, spawnPoints[pln][5]);
        SetActorPosition(0, spawnPoints[pln][1], spawnPoints[pln][2], spawnPoints[pln][3], 1);
    }
    else if (!spawned)
    {
        SetActorVelocity(0, 0,0,0, 0, 0);
        SetActorAngle(0, startPoints[pln][3]);
        SetActorPitch(0, startPoints[pln][4]);
        SetActorPosition(0, startPoints[pln][0], startPoints[pln][1], startPoints[pln][2], 1);
    }
}

script RJPACK_MAKEBEACON (int choice, int beaconType)
{
    int i; int j; int tid;
    int pln = PlayerNumber();
    
    switch (choice)
    {
    case 0:
        if (beaconType < 0 || beaconType >= BEACON_COUNT)
        {
            terminate;
        }
        
        int nx; int ny; int nz;
        int cont;
        
        int newTID = unusedTID(5000, 10000);

        int x = GetActorX(0);
        int y = GetActorY(0);
        int z = GetActorZ(0);
        int angle = GetActorAngle(0);
        int pitch = GetActorPitch(0);

        
        for (i = 0; i < MAX_BEACONS; i++)
        {
            if (beaconTIDs[pln][beaconType][i] == 0)
            {
                cont = 1;
                break;
            }
        }

        nx = x + (FixedMul(cos(angle), cos(pitch)) * 80);
        ny = y + (FixedMul(sin(angle), cos(pitch)) * 80);
        nz = max(z + (-sin(pitch) * 80), GetActorFloorZ(0));
        
        Spawn(BEACONNAMES[beaconType][0], nx, ny, nz, newTID, 0);

        if (ThingCount(0, newTID) == 0)
        {
            terminate;
        }

        if (cont == 0)
        {
            SpawnSpot("DecorativeExplosion", beaconTIDs[pln][beaconType][0]);
            Thing_Remove(beaconTIDs[pln][beaconType][0]);

            for (i = 0; i < MAX_BEACONS; i++)
            {
                beaconTIDs[pln][beaconType][i] = beaconTIDs[pln][beaconType][i+1];
            }
             
            beaconTIDs[pln][beaconType][MAX_BEACONS-1] = newTID;
        }
        else
        {
            beaconTIDs[pln][beaconType][i] = newTID;
        }
        break;

    case 1:
        SetResultValue(CheckInventory("BeaconCounter"));
        break;

    case 2:
        i = CheckInventory("BeaconCounter");
        j = modulo(i + beaconType, BEACON_COUNT);

        TakeInventory("BeaconCounter", i);
        GiveInventory("BeaconCounter", j);

        Print(s:"Switched to beacon color ", s:BEACONNAMES[j][1]);
        break;

    case 3:
        for (i = 0; i < BEACON_COUNT; i++)
        {
            for (j = 0; j < MAX_BEACONS; j++)
            {
                tid = beaconTIDs[pln][i][j];

                if (tid != 0)
                {
                    SpawnSpot("DecorativeExplosion", tid);
                    Thing_Remove(tid);
                    beaconTIDs[pln][i][j] = 0;
                }
            }
        }
    }
}
