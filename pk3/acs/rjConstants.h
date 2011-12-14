#define RJPACK_BASE             692


#define RJPACK_SPAWN_COMMON     RJPACK_BASE - 101

#define RJPACK_OPEN             RJPACK_BASE - 1
#define RJPACK_ENTER            RJPACK_BASE - 2
#define RJPACK_RESPAWN          RJPACK_BASE - 3
#define RJPACK_DEATH            RJPACK_BASE - 4
#define RJPACK_DISCONNECT       RJPACK_BASE - 5


#define RJPACK_SETSPAWN         RJPACK_BASE + 1
#define RJPACK_CHECKAMMO        RJPACK_BASE + 2
#define RJPACK_JOIN             RJPACK_BASE + 3
#define RJPACK_GETPOWER         RJPACK_BASE + 4
#define RJPACK_PROJ_CHECKAMMO   RJPACK_BASE + 5
#define RJPACK_GOTOSPAWN        RJPACK_BASE + 6
#define RJPACK_TOGGLEPICKUP     RJPACK_BASE + 7
#define RJPACK_MAKEBEACON       RJPACK_BASE + 8
#define RJPACK_REMOVEBEACONS    RJPACK_BASE + 9



#define BEACON_COUNT            9
#define MAX_BEACONS             3


int BEACONNAMES[BEACON_COUNT][2] = {
                                    {"RedBeacon",       "Red"}, 
                                    {"OrangeBeacon",    "Orange"},
                                    {"YellowBeacon",    "Yellow"},
                                    {"GreenBeacon",     "Green"},
                                    {"TurquoiseBeacon", "Turquoise"},
                                    {"BlueBeacon",      "Blue"},
                                    {"MagentaBeacon",   "Magenta"},
                                    {"WhiteBeacon",     "White"},
                                    {"BlackBeacon",     "Black"},
                                   };
