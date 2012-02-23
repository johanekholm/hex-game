import debuglog

class BattleActionType:
    NEUTRAL     = 0
    ATTACK      = 1
    MOVEMENT    = 2
    DEFENSE     = 3
    BOOST       = 4

class BattleAction:
    MOVE =    0
    STRIKE =  1
    FIRE =    2
    GALE =    3
    BURN =    4
    HEAL =    5
    
class AdventureAction:
    MOVE            = 0
    FIGHT           = 1
    SHOP            = 6
    ENTERDUNGEON    = 7
    INVENTORY       = 8
    PARTYOPTIONS    = 9

class ActionTarget:    
    HEX =       1
    UNIT =      2
    PARTY =     3
    SELF =      4

class BattleActionHeal:
    """A class to heal units in battle"""
    id = 0
    unit = None
    name = "HEAL"
    cost = 20
    target = ActionTarget.UNIT
    type = BattleActionType.DEFENSE
    sound = "heal"
    
    def isAvailableToUnit(self):
        test = "hej"
    
    def doAction(self):
        debuglog.log("Action going!")
