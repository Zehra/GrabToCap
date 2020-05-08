// GrabToCap.cpp

#include "bzfsAPI.h"

// Custom Utility functions
void resetPlayer(int playerID) {
    bz_resetFlag(playerID);
    bz_killPlayer(playerID, true, BZ_SERVER);
    bz_incrementPlayerLosses(playerID, -1);
}

bz_eTeamType flagToTeamValue(const char* flagType) {
    if (strcmp("R*", flagType) == 0) {
        return eRedTeam;
    } else if (strcmp("G*", flagType) == 0) {
        return eGreenTeam;
    } else if (strcmp("B*", flagType) == 0) {
        return eBlueTeam;
    } else if (strcmp("P*", flagType) == 0) {
        return ePurpleTeam;
    }  else {
        return eNoTeam;
    }
}

class GrabToCap : public bz_Plugin
{
public:
  virtual const char* Name () {return "GrabToCap";}
  virtual void Init(const char* config);
  virtual void Event(bz_EventData *eventData);
};

BZ_PLUGIN(GrabToCap)

void GrabToCap::Init( const char* /*commandLine*/ ) {
  Register(bz_eFlagGrabbedEvent);
}

void GrabToCap::Event ( bz_EventData *eventData )
{
    if (eventData->eventType != bz_eFlagGrabbedEvent)
      return;

    bz_FlagGrabbedEventData_V1* grabData = (bz_FlagGrabbedEventData_V1*)eventData;

    int playerHoldingFlag = grabData->playerID;
    bz_eTeamType flagTeam = flagToTeamValue(grabData->flagType);

    // Proceeding, if we have a team flag.
    if (flagTeam != eNoTeam) {
        bz_eTeamType playerTeam = bz_getPlayerTeam(grabData->playerID);
        // If we have an actual team, we may be ready to score.
        if ((playerTeam == eRedTeam) || (playerTeam == eGreenTeam) || (playerTeam == eBlueTeam) || (playerTeam == ePurpleTeam)) {
            // Checking if teams are not the same, if they are not, we trigger a score change at this point, as well as anything else specified.
            if (flagTeam != playerTeam) {
                // View custom functions for details of code.
                bz_triggerFlagCapture(playerHoldingFlag, playerTeam, flagTeam);
                resetPlayer(playerHoldingFlag);
                //bz_incrementPlayerWins(playerHoldingFlag, bz_getTeamCount(flagTeam));
            }
        }
    }
    // End of event trigger
}
