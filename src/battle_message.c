#include "global.h"
#include "gflib.h"
#include "battle_string_ids.h"
#include "battle.h"
#include "battle_anim.h"
#include "strings.h"
#include "battle_message.h"
#include "link.h"
#include "event_scripts.h"
#include "event_data.h"
#include "item.h"
#include "battle_tower.h"
#include "trainer_tower.h"
#include "battle_setup.h"
#include "field_specials.h"
#include "new_menu_helpers.h"
#include "battle_controllers.h"
#include "graphics.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/trainers.h"

struct BattleWindowText
{
    u8 fillValue;
    u8 fontId;
    u8 x;
    u8 y;
    u8 letterSpacing;
    u8 lineSpacing;
    u8 speed;
    u8 fgColor;
    u8 bgColor;
    u8 shadowColor;
};

static EWRAM_DATA u8 sBattlerAbilities[MAX_BATTLERS_COUNT] = {};
static EWRAM_DATA struct BattleMsgData *sBattleMsgDataPtr = NULL;

static void ChooseMoveUsedParticle(u8 *textPtr);
static void ChooseTypeOfMoveUsedString(u8 *textPtr);
static void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst);

static const u8 sText_Empty1[] = _("");
static const u8 sText_Trainer1LoseText[] = _("{B_TRAINER1_LOSE_TEXT}");
static const u8 sText_Trainer2Class[] = _("{B_TRAINER2_CLASS}");
static const u8 sText_Trainer1RecallPkmn1[] = _("{B_TRAINER1_NAME}: ¡{B_OPPONENT_MON1_NAME}, regresa!");
static const u8 sText_Trainer1WinText[] = _("{B_TRAINER1_WIN_TEXT}");
static const u8 sText_Trainer1RecallPkmn2[] = _("{B_TRAINER1_NAME}: ¡{B_OPPONENT_MON2_NAME}, regresa!");
static const u8 sText_Trainer1RecallBoth[] = _("{B_TRAINER1_NAME}: ¡{B_OPPONENT_MON1_NAME} y\n{B_OPPONENT_MON2_NAME}, regresen!");
static const u8 sText_Trainer2Name[] = _("{B_TRAINER2_NAME}");
static const u8 sText_PkmnGainedEXP[] = _("¡{B_BUFF1} ganó{B_BUFF2}\n{B_TRAINER2_LOSE_TEXT} Puntos de EXP.!\p");
static const u8 sText_EmptyString4[] = _("");
static const u8 sText_ABoosted[] = _(" unos aumentados");
static const u8 sText_PkmnGrewToLv[] = _("¡{B_BUFF1} creció al\nNV. {B_BUFF2}!{WAIT_SE}\p");
static const u8 sText_PkmnLearnedMove[] = _("¡{B_BUFF1} aprendió\n{B_BUFF2}!{WAIT_SE}\p");
static const u8 sText_TryToLearnMove1[] = _("{B_BUFF1} intenta\naprender {B_BUFF2}.\p");
static const u8 sText_TryToLearnMove2[] = _("Pero, {B_BUFF1} no puede\naprender más de cuatro ataques.\p");
static const u8 sText_TryToLearnMove3[] = _("¿Remplazar un movimiento\ncon {B_BUFF2}?");
static const u8 sText_PkmnForgotMove[] = _("{B_BUFF1} olvidó\n{B_BUFF2}.\p");
static const u8 sText_StopLearningMove[] = _("{PAUSE 32}¿Dejar de aprender\n{B_BUFF2}?");
static const u8 sText_DidNotLearnMove[] = _("{B_BUFF1} no aprendió\n{B_BUFF2}.\p");
static const u8 sText_UseNextPkmn[] = _("¿Usar al siguiente POKéMON?");
static const u8 sText_AttackMissed[] = _("¡Ataque de {B_ATK_NAME_WITH_PREFIX}\nfalló!");
static const u8 sText_PkmnProtectedItself[] = _("¡{B_DEF_NAME_WITH_PREFIX}\nse protegió!");
static const u8 sText_AvoidedDamage[] = _("¡{B_DEF_NAME_WITH_PREFIX} evitó el\ndaño con {B_DEF_ABILITY}!");
static const u8 sText_PkmnMakesGroundMiss[] = _("¡{B_DEF_NAME_WITH_PREFIX} evita los movimientos\nde tipo TIERRA con {B_DEF_ABILITY}!");
static const u8 sText_PkmnAvoidedAttack[] = _("¡{B_DEF_NAME_WITH_PREFIX} esquivó\nel ataque!");
static const u8 sText_ItDoesntAffect[] = _("No afecta a\n{B_DEF_NAME_WITH_PREFIX}…");
static const u8 sText_AttackerFainted[] = _("¡{B_ATK_NAME_WITH_PREFIX}\nse debilitó!\p");
static const u8 sText_TargetFainted[] = _("¡{B_DEF_NAME_WITH_PREFIX}\nse debilitó!\p");
static const u8 sText_PlayerGotMoney[] = _("¡{B_PLAYER_NAME} obtuvo ¥{B_BUFF1}\ncomo premio!\p");
static const u8 sText_PlayerWhiteout[] = _("¡{B_PLAYER_NAME} se quedo\nsin POKéMON!\p");
static const u8 sText_PlayerPanicked[] = _("{B_PLAYER_NAME} entro en pánico y perdió\n¥{B_BUFF1}…\p… … … …\p{B_PLAYER_NAME} esta fuera de combate!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerWhiteoutAgainstTrainer[] = _("¡{B_PLAYER_NAME} se quedo\nsin POKéMON!\p¡Perdiste contra\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerPaidAsPrizeMoney[] = _("{B_PLAYER_NAME} pagó ¥{B_BUFF1} como\npremio…\p… … … …\p¡{B_PLAYER_NAME} esta fuera de combate!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerWhiteout2[] = _("¡{B_PLAYER_NAME} esta fuera de combate!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PreventsEscape[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} prevents\nescape with {B_SCR_ACTIVE_ABILITY}!\p");
static const u8 sText_CantEscape2[] = _("¡No puedes escapar!\p");
static const u8 sText_AttackerCantEscape[] = _("¡{B_ATK_NAME_WITH_PREFIX} no puede escapar!");
static const u8 sText_HitXTimes[] = _("Hit {B_BUFF1} time(s)!");
static const u8 sText_PkmnFellAsleep[] = _("{B_EFF_NAME_WITH_PREFIX}\nfell asleep!");
static const u8 sText_PkmnMadeSleep[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade {B_EFF_NAME_WITH_PREFIX} sleep!");
static const u8 sText_PkmnAlreadyAsleep[] = _("{B_DEF_NAME_WITH_PREFIX} is\nalready asleep!");
static const u8 sText_PkmnAlreadyAsleep2[] = _("{B_ATK_NAME_WITH_PREFIX} is\nalready asleep!");
static const u8 sText_PkmnWasntAffected[] = _("{B_DEF_NAME_WITH_PREFIX}\nwasn't affected!");
static const u8 sText_PkmnWasPoisoned[] = _("¡{B_EFF_NAME_WITH_PREFIX}\nfue envenenado!");
static const u8 sText_PkmnPoisonedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\npoisoned {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnHurtByPoison[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt\nby poison!");
static const u8 sText_PkmnAlreadyPoisoned[] = _("{B_DEF_NAME_WITH_PREFIX} is already\npoisoned.");
static const u8 sText_PkmnBadlyPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX} is badly\npoisoned!");
static const u8 sText_PkmnEnergyDrained[] = _("{B_DEF_NAME_WITH_PREFIX} had its\nenergy drained!");
static const u8 sText_PkmnWasBurned[] = _("{B_EFF_NAME_WITH_PREFIX} was burned!");
static const u8 sText_PkmnBurnedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nburned {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnHurtByBurn[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt\nby its burn!");
static const u8 sText_PkmnAlreadyHasBurn[] = _("{B_DEF_NAME_WITH_PREFIX} already\nhas a burn.");
static const u8 sText_PkmnWasFrozen[] = _("{B_EFF_NAME_WITH_PREFIX} was\nfrozen solid!");
static const u8 sText_PkmnFrozenBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nfroze {B_EFF_NAME_WITH_PREFIX} solid!");
static const u8 sText_PkmnIsFrozen[] = _("{B_ATK_NAME_WITH_PREFIX} is\nfrozen solid!");
static const u8 sText_PkmnWasDefrosted[] = _("{B_DEF_NAME_WITH_PREFIX} was\ndefrosted!");
static const u8 sText_PkmnWasDefrosted2[] = _("{B_ATK_NAME_WITH_PREFIX} was\ndefrosted!");
static const u8 sText_PkmnWasDefrostedBy[] = _("{B_ATK_NAME_WITH_PREFIX} was\ndefrosted by {B_CURRENT_MOVE}!");
static const u8 sText_PkmnWasParalyzed[] = _("{B_EFF_NAME_WITH_PREFIX} is paralyzed!\nIt may be unable to move!");
static const u8 sText_PkmnWasParalyzedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nparalyzed {B_EFF_NAME_WITH_PREFIX}!\lIt may be unable to move!");
static const u8 sText_PkmnIsParalyzed[] = _("{B_ATK_NAME_WITH_PREFIX} is paralyzed!\nIt can't move!");
static const u8 sText_PkmnIsAlreadyParalyzed[] = _("{B_DEF_NAME_WITH_PREFIX} is\nalready paralyzed!");
static const u8 sText_PkmnHealedParalysis[] = _("{B_DEF_NAME_WITH_PREFIX} was\nhealed of paralysis!");
static const u8 sText_PkmnDreamEaten[] = _("{B_DEF_NAME_WITH_PREFIX}'s\ndream was eaten!");
static const u8 sText_StatsWontIncrease[] = _("¡{B_BUFF1} de {B_ATK_NAME_WITH_PREFIX}\nno subira más!");
static const u8 sText_StatsWontDecrease[] = _("¡{B_BUFF1} de {B_DEF_NAME_WITH_PREFIX}\nno bajara más!");
static const u8 sText_TeamStoppedWorking[] = _("Your team's {B_BUFF1}\nstopped working!");
static const u8 sText_FoeStoppedWorking[] = _("The foe's {B_BUFF1}\nstopped working!");
static const u8 sText_PkmnIsConfused[] = _("{B_ATK_NAME_WITH_PREFIX} is\nconfused!");
static const u8 sText_PkmnHealedConfusion[] = _("{B_ATK_NAME_WITH_PREFIX} snapped\nout of confusion!");
static const u8 sText_PkmnWasConfused[] = _("{B_EFF_NAME_WITH_PREFIX} became\nconfused!");
static const u8 sText_PkmnAlreadyConfused[] = _("{B_DEF_NAME_WITH_PREFIX} is\nalready confused!");
static const u8 sText_PkmnFellInLove[] = _("{B_DEF_NAME_WITH_PREFIX}\nfell in love!");
static const u8 sText_PkmnInLove[] = _("{B_ATK_NAME_WITH_PREFIX} is in love\nwith {B_SCR_ACTIVE_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnImmobilizedByLove[] = _("{B_ATK_NAME_WITH_PREFIX} is\nimmobilized by love!");
static const u8 sText_PkmnBlownAway[] = _("{B_DEF_NAME_WITH_PREFIX} was\nblown away!");
static const u8 sText_PkmnChangedType[] = _("{B_ATK_NAME_WITH_PREFIX} transformed\ninto the {B_BUFF1} type!");
static const u8 sText_PkmnFlinched[] = _("{B_ATK_NAME_WITH_PREFIX} flinched!");
static const u8 sText_PkmnRegainedHealth[] = _("{B_DEF_NAME_WITH_PREFIX} regained\nhealth!");
static const u8 sText_PkmnHPFull[] = _("{B_DEF_NAME_WITH_PREFIX}'s\nHP is full!");
static const u8 sText_PkmnRaisedSpDef[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised SP. DEF!");
static const u8 sText_PkmnRaisedSpDefALittle[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised SP. DEF a little!");
static const u8 sText_PkmnRaisedDef[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised DEFENSE!");
static const u8 sText_PkmnRaisedDefALittle[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised DEFENSE a little!");
static const u8 sText_PkmnCoveredByVeil[] = _("{B_ATK_PREFIX2}'s party is covered\nby a veil!");
static const u8 sText_PkmnUsedSafeguard[] = _("{B_DEF_NAME_WITH_PREFIX}'s party is protected\nby SAFEGUARD!");
static const u8 sText_PkmnSafeguardExpired[] = _("{B_ATK_PREFIX3}'s party is no longer\nprotected by SAFEGUARD!");
static const u8 sText_PkmnWentToSleep[] = _("{B_ATK_NAME_WITH_PREFIX} went\nto sleep!");
static const u8 sText_PkmnSleptHealthy[] = _("{B_ATK_NAME_WITH_PREFIX} slept and\nbecame healthy!");
static const u8 sText_PkmnWhippedWhirlwind[] = _("{B_ATK_NAME_WITH_PREFIX} whipped\nup a whirlwind!");
static const u8 sText_PkmnTookSunlight[] = _("{B_ATK_NAME_WITH_PREFIX} took\nin sunlight!");
static const u8 sText_PkmnLoweredHead[] = _("{B_ATK_NAME_WITH_PREFIX} lowered\nits head!");
static const u8 sText_PkmnIsGlowing[] = _("{B_ATK_NAME_WITH_PREFIX} is glowing!");
static const u8 sText_PkmnFlewHigh[] = _("{B_ATK_NAME_WITH_PREFIX} flew\nup high!");
static const u8 sText_PkmnDugHole[] = _("{B_ATK_NAME_WITH_PREFIX} dug a hole!");
static const u8 sText_PkmnHidUnderwater[] = _("{B_ATK_NAME_WITH_PREFIX} hid\nunderwater!");
static const u8 sText_PkmnSprangUp[] = _("{B_ATK_NAME_WITH_PREFIX} sprang up!");
static const u8 sText_PkmnSqueezedByBind[] = _("{B_DEF_NAME_WITH_PREFIX} was squeezed by\n{B_ATK_NAME_WITH_PREFIX}'s BIND!");
static const u8 sText_PkmnTrappedInVortex[] = _("{B_DEF_NAME_WITH_PREFIX} was trapped\nin the vortex!");
static const u8 sText_PkmnTrappedBySandTomb[] = _("{B_DEF_NAME_WITH_PREFIX} was trapped\nby SAND TOMB!");
static const u8 sText_PkmnWrappedBy[] = _("{B_DEF_NAME_WITH_PREFIX} was WRAPPED by\n{B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnClamped[] = _("{B_ATK_NAME_WITH_PREFIX} CLAMPED\n{B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnHurtBy[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt\nby {B_BUFF1}!");
static const u8 sText_PkmnFreedFrom[] = _("{B_ATK_NAME_WITH_PREFIX} was freed\nfrom {B_BUFF1}!");
static const u8 sText_PkmnCrashed[] = _("{B_ATK_NAME_WITH_PREFIX} kept going\nand crashed!");
const u8 gBattleText_MistShroud[] = _("{B_ATK_PREFIX2} became\nshrouded in MIST!");
static const u8 sText_PkmnProtectedByMist[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is protected\nby MIST!");
const u8 gBattleText_GetPumped[] = _("{B_ATK_NAME_WITH_PREFIX} is getting\npumped!");
static const u8 sText_PkmnHitWithRecoil[] = _("{B_ATK_NAME_WITH_PREFIX} is hit\nwith recoil!");
static const u8 sText_PkmnProtectedItself2[] = _("{B_ATK_NAME_WITH_PREFIX} protected\nitself!");
static const u8 sText_PkmnBuffetedBySandstorm[] = _("{B_ATK_NAME_WITH_PREFIX} is buffeted\nby the sandstorm!");
static const u8 sText_PkmnPeltedByHail[] = _("{B_ATK_NAME_WITH_PREFIX} is pelted\nby HAIL!");
static const u8 sText_PkmnsXWoreOff[] = _("{B_ATK_PREFIX1}'s {B_BUFF1}\nwore off!");
static const u8 sText_PkmnSeeded[] = _("{B_DEF_NAME_WITH_PREFIX} was seeded!");
static const u8 sText_PkmnEvadedAttack[] = _("{B_DEF_NAME_WITH_PREFIX} evaded\nthe attack!");
static const u8 sText_PkmnSappedByLeechSeed[] = _("{B_ATK_NAME_WITH_PREFIX}'s health is\nsapped by LEECH SEED!");
static const u8 sText_PkmnFastAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} is fast\nasleep.");
static const u8 sText_PkmnWokeUp[] = _("{B_ATK_NAME_WITH_PREFIX} woke up!");
static const u8 sText_PkmnUproarKeptAwake[] = _("But {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s UPROAR\nkept it awake!");
static const u8 sText_PkmnWokeUpInUproar[] = _("{B_ATK_NAME_WITH_PREFIX} woke up\nin the UPROAR!");
static const u8 sText_PkmnCausedUproar[] = _("{B_ATK_NAME_WITH_PREFIX} caused\nan UPROAR!");
static const u8 sText_PkmnMakingUproar[] = _("{B_ATK_NAME_WITH_PREFIX} is making\nan UPROAR!");
static const u8 sText_PkmnCalmedDown[] = _("{B_ATK_NAME_WITH_PREFIX} calmed down.");
static const u8 sText_PkmnCantSleepInUproar[] = _("But {B_DEF_NAME_WITH_PREFIX} can't\nsleep in an UPROAR!");
static const u8 sText_PkmnStockpiled[] = _("{B_ATK_NAME_WITH_PREFIX} STOCKPILED\n{B_BUFF1}!");
static const u8 sText_PkmnCantStockpile[] = _("{B_ATK_NAME_WITH_PREFIX} can't\nSTOCKPILE any more!");
static const u8 sText_PkmnCantSleepInUproar2[] = _("But {B_DEF_NAME_WITH_PREFIX} can't\nsleep in an UPROAR!");
static const u8 sText_UproarKeptPkmnAwake[] = _("But the UPROAR kept\n{B_DEF_NAME_WITH_PREFIX} awake!");
static const u8 sText_PkmnStayedAwakeUsing[] = _("{B_DEF_NAME_WITH_PREFIX} stayed awake\nusing its {B_DEF_ABILITY}!");
static const u8 sText_PkmnStoringEnergy[] = _("{B_ATK_NAME_WITH_PREFIX} is storing\nenergy!");
static const u8 sText_PkmnUnleashedEnergy[] = _("{B_ATK_NAME_WITH_PREFIX} unleashed\nenergy!");
static const u8 sText_PkmnFatigueConfusion[] = _("{B_ATK_NAME_WITH_PREFIX} became\nconfused due to fatigue!");
static const u8 sText_PkmnPickedUpItem[] = _("{B_PLAYER_NAME} picked up\n¥{B_BUFF1}!\p");
static const u8 sText_PkmnUnaffected[] = _("{B_DEF_NAME_WITH_PREFIX} is\nunaffected!");
static const u8 sText_PkmnTransformedInto[] = _("{B_ATK_NAME_WITH_PREFIX} transformed\ninto {B_BUFF1}!");
static const u8 sText_PkmnMadeSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX} made\na SUBSTITUTE!");
static const u8 sText_PkmnHasSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX} already\nhas a SUBSTITUTE!");
static const u8 sText_SubstituteDamaged[] = _("The SUBSTITUTE took damage\nfor {B_DEF_NAME_WITH_PREFIX}!\p");
static const u8 sText_PkmnSubstituteFaded[] = _("{B_DEF_NAME_WITH_PREFIX}'s\nSUBSTITUTE faded!\p");
static const u8 sText_PkmnMustRecharge[] = _("{B_ATK_NAME_WITH_PREFIX} must\nrecharge!");
static const u8 sText_PkmnRageBuilding[] = _("{B_DEF_NAME_WITH_PREFIX}'s RAGE\nis building!");
static const u8 sText_PkmnMoveWasDisabled[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\nwas disabled!");
static const u8 sText_PkmnMoveDisabledNoMore[] = _("{B_ATK_NAME_WITH_PREFIX} is disabled\nno more!");
static const u8 sText_PkmnGotEncore[] = _("{B_DEF_NAME_WITH_PREFIX} got\nan ENCORE!");
static const u8 sText_PkmnEncoreEnded[] = _("{B_ATK_NAME_WITH_PREFIX}'s ENCORE\nended!");
static const u8 sText_PkmnTookAim[] = _("{B_ATK_NAME_WITH_PREFIX} took aim\nat {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnSketchedMove[] = _("{B_ATK_NAME_WITH_PREFIX} SKETCHED\n{B_BUFF1}!");
static const u8 sText_PkmnTryingToTakeFoe[] = _("{B_ATK_NAME_WITH_PREFIX} is trying\nto take its foe with it!");
static const u8 sText_PkmnTookFoe[] = _("{B_DEF_NAME_WITH_PREFIX} took\n{B_ATK_NAME_WITH_PREFIX} with it!");
static const u8 sText_PkmnReducedPP[] = _("Reduced {B_DEF_NAME_WITH_PREFIX}'s\n{B_BUFF1} by {B_BUFF2}!");
static const u8 sText_PkmnStoleItem[] = _("{B_ATK_NAME_WITH_PREFIX} stole\n{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}!");
static const u8 sText_TargetCantEscapeNow[] = _("{B_DEF_NAME_WITH_PREFIX} can't\nescape now!");
static const u8 sText_PkmnFellIntoNightmare[] = _("{B_DEF_NAME_WITH_PREFIX} fell into\na NIGHTMARE!");
static const u8 sText_PkmnLockedInNightmare[] = _("{B_ATK_NAME_WITH_PREFIX} is locked\nin a NIGHTMARE!");
static const u8 sText_PkmnLaidCurse[] = _("{B_ATK_NAME_WITH_PREFIX} cut its own HP and\nlaid a CURSE on {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnAfflictedByCurse[] = _("{B_ATK_NAME_WITH_PREFIX} is afflicted\nby the CURSE!");
static const u8 sText_SpikesScattered[] = _("SPIKES were scattered all around\nthe opponent's side!");
static const u8 sText_PkmnHurtBySpikes[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is hurt\nby SPIKES!");
static const u8 sText_PkmnIdentified[] = _("{B_ATK_NAME_WITH_PREFIX} identified\n{B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnPerishCountFell[] = _("{B_ATK_NAME_WITH_PREFIX}'s PERISH count\nfell to {B_BUFF1}!");
static const u8 sText_PkmnBracedItself[] = _("{B_ATK_NAME_WITH_PREFIX} braced\nitself!");
static const u8 sText_PkmnEnduredHit[] = _("{B_DEF_NAME_WITH_PREFIX} ENDURED\nthe hit!");
static const u8 sText_MagnitudeStrength[] = _("MAGNITUDE {B_BUFF1}!");
static const u8 sText_PkmnCutHPMaxedAttack[] = _("{B_ATK_NAME_WITH_PREFIX} cut its own HP\nand maximized ATTACK!");
static const u8 sText_PkmnCopiedStatChanges[] = _("{B_ATK_NAME_WITH_PREFIX} copied\n{B_DEF_NAME_WITH_PREFIX}'s stat changes!");
static const u8 sText_PkmnGotFree[] = _("{B_ATK_NAME_WITH_PREFIX} got free of\n{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}!");
static const u8 sText_PkmnShedLeechSeed[] = _("{B_ATK_NAME_WITH_PREFIX} shed\nLEECH SEED!");
static const u8 sText_PkmnBlewAwaySpikes[] = _("{B_ATK_NAME_WITH_PREFIX} blew away\nSPIKES!");
static const u8 sText_PkmnFledFromBattle[] = _("{B_ATK_NAME_WITH_PREFIX} fled from\nbattle!");
static const u8 sText_PkmnForesawAttack[] = _("{B_ATK_NAME_WITH_PREFIX} foresaw\nan attack!");
static const u8 sText_PkmnTookAttack[] = _("{B_DEF_NAME_WITH_PREFIX} took the\n{B_BUFF1} attack!");
static const u8 sText_PkmnChoseXAsDestiny[] = _("{B_ATK_NAME_WITH_PREFIX} chose\n{B_CURRENT_MOVE} as its destiny!");
static const u8 sText_PkmnAttack[] = _("¡Ataque de {B_BUFF1}!");
static const u8 sText_PkmnCenterAttention[] = _("{B_ATK_NAME_WITH_PREFIX} became the\ncenter of attention!");
static const u8 sText_PkmnChargingPower[] = _("{B_ATK_NAME_WITH_PREFIX} began\ncharging power!");
static const u8 sText_NaturePowerTurnedInto[] = _("NATURE POWER turned into\n{B_CURRENT_MOVE}!");
static const u8 sText_PkmnStatusNormal[] = _("{B_ATK_NAME_WITH_PREFIX}'s status\nreturned to normal!");
static const u8 sText_PkmnSubjectedToTorment[] = _("{B_DEF_NAME_WITH_PREFIX} was subjected\nto TORMENT!");
static const u8 sText_PkmnTighteningFocus[] = _("{B_ATK_NAME_WITH_PREFIX} is tightening\nits focus!");
static const u8 sText_PkmnFellForTaunt[] = _("{B_DEF_NAME_WITH_PREFIX} fell for\nthe TAUNT!");
static const u8 sText_PkmnReadyToHelp[] = _("{B_ATK_NAME_WITH_PREFIX} is ready to\nhelp {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnSwitchedItems[] = _("{B_ATK_NAME_WITH_PREFIX} switched\nitems with its opponent!");
static const u8 sText_PkmnObtainedX[] = _("{B_ATK_NAME_WITH_PREFIX} obtained\n{B_BUFF1}.");
static const u8 sText_PkmnObtainedX2[] = _("{B_DEF_NAME_WITH_PREFIX} obtained\n{B_BUFF2}.");
static const u8 sText_PkmnObtainedXYObtainedZ[] = _("{B_ATK_NAME_WITH_PREFIX} obtained\n{B_BUFF1}.\p{B_DEF_NAME_WITH_PREFIX} obtained\n{B_BUFF2}.");
static const u8 sText_PkmnCopiedFoe[] = _("{B_ATK_NAME_WITH_PREFIX} copied\n{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}!");
static const u8 sText_PkmnMadeWish[] = _("{B_ATK_NAME_WITH_PREFIX} made a WISH!");
static const u8 sText_PkmnWishCameTrue[] = _("{B_BUFF1}'s WISH\ncame true!");
static const u8 sText_PkmnPlantedRoots[] = _("{B_ATK_NAME_WITH_PREFIX} planted its roots!");
static const u8 sText_PkmnAbsorbedNutrients[] = _("{B_ATK_NAME_WITH_PREFIX} absorbed\nnutrients with its roots!");
static const u8 sText_PkmnAnchoredItself[] = _("{B_DEF_NAME_WITH_PREFIX} anchored\nitself with its roots!");
static const u8 sText_PkmnWasMadeDrowsy[] = _("{B_ATK_NAME_WITH_PREFIX} made\n{B_DEF_NAME_WITH_PREFIX} drowsy!");
static const u8 sText_PkmnKnockedOff[] = _("{B_ATK_NAME_WITH_PREFIX} knocked off\n{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}!");
static const u8 sText_PkmnSwappedAbilities[] = _("{B_ATK_NAME_WITH_PREFIX} swapped abilities\nwith its opponent!");
static const u8 sText_PkmnSealedOpponentMove[] = _("{B_ATK_NAME_WITH_PREFIX} sealed the\nopponent's moveシsス!");
static const u8 sText_PkmnWantsGrudge[] = _("{B_ATK_NAME_WITH_PREFIX} wants the\nopponent to bear a GRUDGE!");
static const u8 sText_PkmnLostPPGrudge[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1} lost\nall its PP due to the GRUDGE!");
static const u8 sText_PkmnShroudedItself[] = _("{B_ATK_NAME_WITH_PREFIX} shrouded\nitself in {B_CURRENT_MOVE}!");
static const u8 sText_PkmnMoveBounced[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE}\nwas bounced back by MAGIC COAT!");
static const u8 sText_PkmnWaitsForTarget[] = _("{B_ATK_NAME_WITH_PREFIX} waits for its foe\nto make a move!");
static const u8 sText_PkmnSnatchedMove[] = _("{B_DEF_NAME_WITH_PREFIX} SNATCHED\n{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s move!");
static const u8 sText_ElectricityWeakened[] = _("Electricity's power was\nweakened!");
static const u8 sText_FireWeakened[] = _("Fire's power was\nweakened!");
static const u8 sText_XFoundOneY[] = _("{B_ATK_NAME_WITH_PREFIX} found\none {B_LAST_ITEM}!");
static const u8 sText_SoothingAroma[] = _("A soothing aroma wafted\nthrough the area!");
static const u8 sText_ItemsCantBeUsedNow[] = _("Items can't be used now.{PAUSE 64}");
static const u8 sText_ForXCommaYZ[] = _("For {B_SCR_ACTIVE_NAME_WITH_PREFIX},\n{B_LAST_ITEM} {B_BUFF1}");
static const u8 sText_PkmnUsedXToGetPumped[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} used\n{B_LAST_ITEM} to hustle!");
static const u8 sText_PkmnLostFocus[] = _("{B_ATK_NAME_WITH_PREFIX} lost its\nfocus and couldn't move!");
static const u8 sText_PkmnWasDraggedOut[] = _("{B_DEF_NAME_WITH_PREFIX} was\ndragged out!\p");
static const u8 sText_TheWallShattered[] = _("The wall shattered!");
static const u8 sText_ButNoEffect[] = _("But it had no effect!");
static const u8 sText_PkmnHasNoMovesLeft[] = _("{B_ACTIVE_NAME_WITH_PREFIX} has no\nmoves left!\p");
static const u8 sText_PkmnMoveIsDisabled[] = _("{B_ACTIVE_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE}\nis disabled!\p");
static const u8 sText_PkmnCantUseMoveTorment[] = _("{B_ACTIVE_NAME_WITH_PREFIX} can't use the same\nmove in a row due to the TORMENT!\p");
static const u8 sText_PkmnCantUseMoveTaunt[] = _("{B_ACTIVE_NAME_WITH_PREFIX} can't use\n{B_CURRENT_MOVE} after the TAUNT!\p");
static const u8 sText_PkmnCantUseMoveSealed[] = _("{B_ACTIVE_NAME_WITH_PREFIX} can't use the\nsealed {B_CURRENT_MOVE}!\p");
static const u8 sText_PkmnMadeItRain[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade it rain!");
static const u8 sText_PkmnRaisedSpeed[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nraised its SPEED!");
static const u8 sText_PkmnProtectedBy[] = _("{B_DEF_NAME_WITH_PREFIX} was protected\nby {B_DEF_ABILITY}!");
static const u8 sText_PkmnPreventsUsage[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents {B_ATK_NAME_WITH_PREFIX}\lfrom using {B_CURRENT_MOVE}!");
static const u8 sText_PkmnRestoredHPUsing[] = _("{B_DEF_NAME_WITH_PREFIX} restored HP\nusing its {B_DEF_ABILITY}!");
static const u8 sText_PkmnsXMadeYUseless[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade {B_CURRENT_MOVE} useless!");
static const u8 sText_PkmnChangedTypeWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade it the {B_BUFF1} type!");
static const u8 sText_PkmnPreventsParalysisWith[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents paralysis!");
static const u8 sText_PkmnPreventsRomanceWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents romance!");
static const u8 sText_PkmnPreventsPoisoningWith[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents poisoning!");
static const u8 sText_PkmnPreventsConfusionWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents confusion!");
static const u8 sText_PkmnRaisedFirePowerWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nraised its FIRE power!");
static const u8 sText_PkmnAnchorsItselfWith[] = _("{B_DEF_NAME_WITH_PREFIX} anchors\nitself with {B_DEF_ABILITY}!");
static const u8 sText_PkmnCutsAttackWith[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncuts {B_DEF_NAME_WITH_PREFIX}'s ATTACK!");
static const u8 sText_PkmnPreventsStatLossWith[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nprevents stat loss!");
static const u8 sText_PkmnHurtsWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nhurt {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnTraced[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} TRACED\n{B_BUFF1}'s {B_BUFF2}!");
static const u8 sText_PkmnsXPreventsBurns[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY}\nprevents burns!");
static const u8 sText_PkmnsXBlocksY[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nblocks {B_CURRENT_MOVE}!");
static const u8 sText_PkmnsXBlocksY2[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nblocks {B_CURRENT_MOVE}!");
static const u8 sText_PkmnsXRestoredHPALittle2[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nrestored its HP a little!");
static const u8 sText_PkmnsXWhippedUpSandstorm[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nwhipped up a sandstorm!");
static const u8 sText_PkmnsXIntensifiedSun[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nintensified the sun's rays!");
static const u8 sText_PkmnsXPreventsYLoss[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nprevents {B_BUFF1} loss!");
static const u8 sText_PkmnsXInfatuatedY[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\ninfatuated {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnsXMadeYIneffective[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade {B_CURRENT_MOVE} ineffective!");
static const u8 sText_PkmnsXCuredYProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncured its {B_BUFF1} problem!");
static const u8 sText_ItSuckedLiquidOoze[] = _("It sucked up the\nLIQUID OOZE!");
static const u8 sText_PkmnTransformed[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} transformed!");
static const u8 sText_PkmnsXTookAttack[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\ntook the attack!");
const u8 gText_PkmnsXPreventsSwitching[] = _("{B_BUFF1}'s {B_LAST_ABILITY}\nprevents switching!\p");
static const u8 sText_PreventedFromWorking[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevented {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s\l{B_BUFF1} from working!");
static const u8 sText_PkmnsXMadeItIneffective[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade it ineffective!");
static const u8 sText_PkmnsXPreventsFlinching[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY}\nprevents flinching!");
static const u8 sText_PkmnsXPreventsYsZ[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nprevents {B_DEF_NAME_WITH_PREFIX}'s\l{B_DEF_ABILITY} from working!");
static const u8 sText_PkmnsXCuredItsYProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncured its {B_BUFF1} problem!");
static const u8 sText_PkmnsXHadNoEffectOnY[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nhad no effect on {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_TooScaredToMove[] = _("¡{B_ATK_NAME_WITH_PREFIX} quedó inmóvil del miedo!");
static const u8 sText_GetOutGetOut[] = _("FANTASMA: Salgan…… Salgan……");
static const u8 sText_StatSharply[] = _("mucho ");
const u8 gBattleText_Rose[] = _("subió!");
static const u8 sText_StatHarshly[] = _("mucho ");
static const u8 sText_StatFell[] = _("bajó!");
static const u8 sText_AttackersStatRose[] = _("¡{B_BUFF1} de {B_ATK_NAME_WITH_PREFIX}\n{B_BUFF2}");
const u8 gText_DefendersStatRose[] = _("¡{B_BUFF1} de {B_DEF_NAME_WITH_PREFIX}\n{B_BUFF2}");
static const u8 sText_UsingItemTheStatOfPkmnRose[] = _("Usando {B_LAST_ITEM}, {B_BUFF1}\nde {B_SCR_ACTIVE_NAME_WITH_PREFIX} {B_BUFF2}");
static const u8 sText_AttackersStatFell[] = _("¡{B_BUFF1} de {B_ATK_NAME_WITH_PREFIX}\n{B_BUFF2}");
static const u8 sText_DefendersStatFell[] = _("¡{B_BUFF1} de {B_DEF_NAME_WITH_PREFIX}\n{B_BUFF2}");
static const u8 sText_StatsWontIncrease2[] = _("{B_ATK_NAME_WITH_PREFIX}'s stats won't\ngo any higher!");
static const u8 sText_StatsWontDecrease2[] = _("{B_DEF_NAME_WITH_PREFIX}'s stats won't\ngo any lower!");
static const u8 sText_CriticalHit[] = _("¡Es un golpe crítico!");
static const u8 sText_OneHitKO[] = _("It's a one-hit KO!");
static const u8 sText_123Poof[] = _("{PAUSE 32}1, {PAUSE 15}2, y{PAUSE 15}… {PAUSE 15}… {PAUSE 15}… {PAUSE 15}{PLAY_SE SE_BALL_BOUNCE_1}¡Poof!\p");
static const u8 sText_AndEllipsis[] = _("Y…\p");
static const u8 sText_HMMovesCantBeForgotten[] = _("HM moves can't be\nforgotten now.\p");
static const u8 sText_NotVeryEffective[] = _("No es muy eficaz…");
static const u8 sText_SuperEffective[] = _("¡Es super eficaz!");
static const u8 sText_GotAwaySafely[] = _("{PLAY_SE SE_FLEE}¡Escapaste a salvo!\p");
static const u8 sText_PkmnFledUsingIts[] = _("{PLAY_SE SE_FLEE}¡{B_ATK_NAME_WITH_PREFIX} huyó\nusando {B_LAST_ITEM}!\p");
static const u8 sText_PkmnFledUsing[] = _("{PLAY_SE SE_FLEE}¡{B_ATK_NAME_WITH_PREFIX} huyó\nusando {B_ATK_ABILITY}!\p");
static const u8 sText_WildPkmnFled[] = _("{PLAY_SE SE_FLEE}¡{B_BUFF1} salvaje huyó!");
static const u8 sText_PlayerDefeatedLinkTrainer[] = _("Player defeated\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_TwoLinkTrainersDefeated[] = _("Player beat {B_LINK_OPPONENT1_NAME}\nand {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_PlayerLostAgainstLinkTrainer[] = _("Player lost against\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerLostToTwo[] = _("Player lost to {B_LINK_OPPONENT1_NAME}\nand {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_PlayerBattledToDrawLinkTrainer[] = _("Player battled to a draw against\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerBattledToDrawVsTwo[] = _("Player battled to a draw against\n{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_WildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} fled!");
static const u8 sText_TwoWildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} and\n{B_LINK_OPPONENT2_NAME} fled!");
static const u8 sText_NoRunningFromTrainers[] = _("¡No! ¡No hay escape de una\nbatalla entre ENTRENADORES!\p");
static const u8 sText_CantEscape[] = _("¡No puedes escapar!\p");
static const u8 sText_EmptyString5[] = _("");   //sText_DontLeaveBirch in pokeem
static const u8 sText_ButNothingHappened[] = _("¡Pero no paso nada!");
static const u8 sText_ButItFailed[] = _("¡Pero falló!");
static const u8 sText_ItHurtConfusion[] = _("¡Se hirió a si mismo en su confusión!");
static const u8 sText_MirrorMoveFailed[] = _("¡El MOVIMIENTO ESPEJO falló!");
static const u8 sText_StartedToRain[] = _("¡Empezó a llover!");
static const u8 sText_DownpourStarted[] = _("A downpour started!"); // corresponds to DownpourText in pokegold and pokecrystal and is used by Rain Dance in GSC
static const u8 sText_RainContinues[] = _("Sigue lloviendo.");
static const u8 sText_DownpourContinues[] = _("The downpour continues."); // unused
static const u8 sText_RainStopped[] = _("La lluvia se detuvo.");
static const u8 sText_SandstormBrewed[] = _("A sandstorm brewed!");
static const u8 sText_SandstormRages[] = _("The sandstorm rages.");
static const u8 sText_SandstormSubsided[] = _("The sandstorm subsided.");
static const u8 sText_SunlightGotBright[] = _("The sunlight got bright!");
static const u8 sText_SunlightStrong[] = _("The sunlight is strong.");
static const u8 sText_SunlightFaded[] = _("The sunlight faded.");
static const u8 sText_StartedHail[] = _("It started to hail!");
static const u8 sText_HailContinues[] = _("Hail continues to fall.");
static const u8 sText_HailStopped[] = _("The hail stopped.");
static const u8 sText_FailedToSpitUp[] = _("But it failed to SPIT UP\na thing!");
static const u8 sText_FailedToSwallow[] = _("But it failed to SWALLOW\na thing!");
static const u8 sText_WindBecameHeatWave[] = _("The wind turned into a\nHEAT WAVE!");
static const u8 sText_StatChangesGone[] = _("All stat changes were\neliminated!");
static const u8 sText_CoinsScattered[] = _("Coins scattered everywhere!");
static const u8 sText_TooWeakForSubstitute[] = _("It was too weak to make\na SUBSTITUTE!");
static const u8 sText_SharedPain[] = _("The battlers shared\ntheir pain!");
static const u8 sText_BellChimed[] = _("A bell chimed!");
static const u8 sText_FaintInThree[] = _("All affected POKéMON will\nfaint in three turns!");
static const u8 sText_NoPPLeft[] = _("¡No quedan PP para este\nmovimiento!\p");
static const u8 sText_ButNoPPLeft[] = _("¡Pero no quedaban PP para\nel movimiento!");
static const u8 sText_PkmnIgnoresAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} ignored\norders while asleep!");
static const u8 sText_PkmnIgnoredOrders[] = _("{B_ATK_NAME_WITH_PREFIX} ignored\norders!");
static const u8 sText_PkmnBeganToNap[] = _("{B_ATK_NAME_WITH_PREFIX} began to nap!");
static const u8 sText_PkmnLoafing[] = _("{B_ATK_NAME_WITH_PREFIX} is\nloafing around!");
static const u8 sText_PkmnWontObey[] = _("{B_ATK_NAME_WITH_PREFIX} won't\nobey!");
static const u8 sText_PkmnTurnedAway[] = _("{B_ATK_NAME_WITH_PREFIX} turned away!");
static const u8 sText_PkmnPretendNotNotice[] = _("{B_ATK_NAME_WITH_PREFIX} pretended\nnot to notice!");
static const u8 sText_EnemyAboutToSwitchPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} está\napunto de usar a {B_BUFF2}.\p¿Cambiarás de\nPOKéMON?");
static const u8 sText_PkmnLearnedMove2[] = _("¡{B_ATK_NAME_WITH_PREFIX} aprendió\n{B_BUFF1}!");
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("¡Derrotaste a\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!\p");
static const u8 sText_ThrewARock[] = _("{B_PLAYER_NAME} threw a ROCK\nat the {B_OPPONENT_MON1_NAME}!");
static const u8 sText_ThrewSomeBait[] = _("{B_PLAYER_NAME} threw some BAIT\nat the {B_OPPONENT_MON1_NAME}!");
static const u8 sText_PkmnWatchingCarefully[] = _("{B_OPPONENT_MON1_NAME} is watching\ncarefully!");
static const u8 sText_PkmnIsAngry[] = _("{B_OPPONENT_MON1_NAME} is angry!");
static const u8 sText_PkmnIsEating[] = _("{B_OPPONENT_MON1_NAME} is eating!");
static const u8 sText_OutOfSafariBalls[] = _("{PLAY_SE SE_DING_DONG}ANNOUNCER: You're out of\nSAFARI BALLS! Game over!\p");
static const u8 sText_WildPkmnAppeared[] = _("¡{B_OPPONENT_MON1_NAME} salvaje aparece!\p");
static const u8 sText_WildPkmnAppeared2[] = _("¡{B_OPPONENT_MON1_NAME} salvaje aparece!\p");
static const u8 sText_WildPkmnAppearedPause[] = _("¡{B_OPPONENT_MON1_NAME} salvaje aparece!{PAUSE 127}");
static const u8 sText_TwoWildPkmnAppeared[] = _("¡{B_OPPONENT_MON1_NAME} y\n{B_OPPONENT_MON2_NAME} salvajes aparecen!\p");
static const u8 sText_GhostAppearedCantId[] = _("¡El FANTASMA apareció!\p¡Rayos!\n¡No puedes identificarlo!\p");
static const u8 sText_TheGhostAppeared[] = _("¡El FANTASMA apareció!\p");
static const u8 sText_SilphScopeUnveil[] = _("¡El VISOR SILPH reveló la\nidentidad del FANTASMA!");
static const u8 sText_TheGhostWas[] = _("¡Se trataba de MAROWAK!\p\n");
static const u8 sText_Trainer1WantsToBattle[] = _("¡{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nquiere luchar!\p");
static const u8 sText_LinkTrainerWantsToBattle[] = _("¡{B_LINK_OPPONENT1_NAME}\nquiere luchar!");
static const u8 sText_TwoLinkTrainersWantToBattle[] = _("¡{B_LINK_OPPONENT1_NAME} y {B_LINK_OPPONENT2_NAME}\nquieren luchar!");
static const u8 sText_Trainer1SentOutPkmn[] = _("¡{B_TRAINER1_CLASS} {B_TRAINER1_NAME} envió\na {B_OPPONENT_MON1_NAME}!{PAUSE 60}");
static const u8 sText_Trainer1SentOutTwoPkmn[] = _("¡{B_TRAINER1_CLASS} {B_TRAINER1_NAME} envió\na {B_OPPONENT_MON1_NAME} y {B_OPPONENT_MON2_NAME}!{PAUSE 60}");
static const u8 sText_Trainer1SentOutPkmn2[] = _("¡{B_TRAINER1_CLASS} {B_TRAINER1_NAME} envió\na {B_BUFF1}!");
static const u8 sText_LinkTrainerSentOutPkmn[] = _("¡{B_LINK_OPPONENT1_NAME} envió a\n{B_OPPONENT_MON1_NAME}!");
static const u8 sText_LinkTrainerSentOutTwoPkmn[] = _("{B_LINK_OPPONENT1_NAME} envió a\n{B_OPPONENT_MON1_NAME} y {B_OPPONENT_MON2_NAME}!");
static const u8 sText_TwoLinkTrainersSentOutPkmn[] = _("¡{B_LINK_OPPONENT1_NAME} envió a {B_LINK_OPPONENT_MON1_NAME}!\n¡{B_LINK_OPPONENT2_NAME} envió a {B_LINK_OPPONENT_MON2_NAME}!");
static const u8 sText_LinkTrainerSentOutPkmn2[] = _("¡{B_LINK_OPPONENT1_NAME} envió a\n{B_BUFF1}!");
static const u8 sText_LinkTrainerMultiSentOutPkmn[] = _("¡{B_LINK_SCR_TRAINER_NAME} envió a\n{B_BUFF1}!");
static const u8 sText_GoPkmn[] = _("¡Vamos, {B_PLAYER_MON1_NAME}!");
static const u8 sText_GoTwoPkmn[] = _("¡Vamos, {B_PLAYER_MON1_NAME} y\n{B_PLAYER_MON2_NAME}!");
static const u8 sText_GoPkmn2[] = _("¡Vamos, {B_BUFF1}!");
static const u8 sText_DoItPkmn[] = _("¡Hazlo, {B_BUFF1}!");
static const u8 sText_GoForItPkmn[] = _("¡Tú puedes, {B_BUFF1}!");
static const u8 sText_YourFoesWeakGetEmPkmn[] = _("¡Tu enemigo está débil!\n¡Derrotalo, {B_BUFF1}!");
static const u8 sText_LinkPartnerSentOutPkmnGoPkmn[] = _("¡{B_LINK_PARTNER_NAME} envió a {B_LINK_PLAYER_MON2_NAME}!\n¡Vamos! ¡{B_LINK_PLAYER_MON1_NAME}!");
static const u8 sText_PkmnThatsEnough[] = _("¡{B_BUFF1}, eso fue suficiente!\n¡Regresa!");
static const u8 sText_PkmnComeBack[] = _("¡{B_BUFF1}, regresa!");
static const u8 sText_PkmnOkComeBack[] = _("¡Bien, {B_BUFF1}!\n¡Regresa!");
const u8 sText_PkmnGoodComeBack[] = _("¡Bien hecho, {B_BUFF1}!\n¡Regresa!");
static const u8 sText_Trainer1WithdrewPkmn[] = _("¡{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nretiró a {B_BUFF1}!");
static const u8 sText_LinkTrainer1WithdrewPkmn[] = _("¡{B_LINK_OPPONENT1_NAME} retiró a\n{B_BUFF1}!");
static const u8 sText_LinkTrainer2WithdrewPkmn[] = _("¡{B_LINK_SCR_TRAINER_NAME} retiró a\n{B_BUFF1}!");
static const u8 sText_WildPkmnPrefix[] = _(" salvaje");
static const u8 sText_FoePkmnPrefix[] = _(" enemigo");
static const u8 sText_FoePkmnPrefix2[] = _("enemigo");
static const u8 sText_AllyPkmnPrefix[] = _("Ally");
static const u8 sText_FoePkmnPrefix3[] = _("enemigo");
static const u8 sText_AllyPkmnPrefix2[] = _("Ally");
static const u8 sText_FoePkmnPrefix4[] = _("enemigo");
static const u8 sText_AllyPkmnPrefix3[] = _("Ally");
static const u8 sText_AttackerUsedX[] = _("¡{B_ATK_NAME_WITH_PREFIX} usó\n{B_BUFF2}");
static const u8 sText_ExclamationMark[] = _("!");
static const u8 sText_ExclamationMark2[] = _("!");
static const u8 sText_ExclamationMark3[] = _("!");
static const u8 sText_ExclamationMark4[] = _("!");
static const u8 sText_ExclamationMark5[] = _("!");

static const u8 sText_HP2[] = _("PS");
static const u8 sText_Attack2[] = _("ATAQUE");
static const u8 sText_Defense2[] = _("DEFENSA");
static const u8 sText_Speed[] = _("VELOCIDAD");
static const u8 sText_SpAtk2[] = _("ATQ. ESP.");
static const u8 sText_SpDef2[] = _("DEF. ESP.");
static const u8 sText_Accuracy[] = _("precisión");
static const u8 sText_Evasiveness[] = _("evasión");

const u8 *const gStatNamesTable[] = {
    sText_HP2,
    sText_Attack2,
    sText_Defense2,
    sText_Speed,
    sText_SpAtk2,
    sText_SpDef2,
    sText_Accuracy,
    sText_Evasiveness
};

static const u8 sText_PokeblockWasTooSpicy[] = _("was too spicy!"); //
static const u8 sText_PokeblockWasTooDry[] = _("was too dry!");
static const u8 sText_PokeblockWasTooSweet[] = _("was too sweet!");
static const u8 sText_PokeblockWasTooBitter[] = _("was too bitter!");
static const u8 sText_PokeblockWasTooSour[] = _("was too sour!");

const u8 *const gPokeblockWasTooXStringTable[] = {
    sText_PokeblockWasTooSpicy,
    sText_PokeblockWasTooDry,
    sText_PokeblockWasTooSweet,
    sText_PokeblockWasTooBitter,
    sText_PokeblockWasTooSour
};

static const u8 sText_PlayerUsedItem[] = _("¡{B_PLAYER_NAME} usó\n{B_LAST_ITEM}!");
static const u8 sText_OldManUsedItem[] = _("¡El señor mayor usó\n{B_LAST_ITEM}!");
static const u8 sText_PokedudeUsedItem[] = _("The POKé DUDE used\n{B_LAST_ITEM}!");
static const u8 sText_Trainer1UsedItem[] = _("¡{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nusó {B_LAST_ITEM}!");
static const u8 sText_TrainerBlockedBall[] = _("¡La BOLA fue bloqueada!");
static const u8 sText_DontBeAThief[] = _("¡No seas un{A} ladr{ONONA}!");
static const u8 sText_ItDodgedBall[] = _("¡Esquivó la BOLA!\n¡No puedes atrapar a este POKéMON!");
static const u8 sText_YouMissedPkmn[] = _("¡Fallaste, no le diste al POKéMON!");
static const u8 sText_PkmnBrokeFree[] = _("¡Oh, no!\n¡El POKéMON escapó!");
static const u8 sText_ItAppearedCaught[] = _("¡Aww!\n¡Parecía estar atrapado!");
static const u8 sText_AarghAlmostHadIt[] = _("¡Aargh!\n¡Casi lo tenías!");
static const u8 sText_ShootSoClose[] = _("¡Rayos!\n¡Y estaba tan cerca!");
const u8 gUnknown_83FD78A[] = _("よけられた!\nこいつは つかまりそうにないぞ!");
static const u8 sText_GotchaPkmnCaught[] = _("Gotcha!\n{B_OPPONENT_MON1_NAME} was caught!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p");
static const u8 sText_GotchaPkmnCaught2[] = _("Gotcha!\n{B_OPPONENT_MON1_NAME} was caught!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}{PAUSE 127}");
static const u8 sText_GiveNicknameCaptured[] = _("Give a nickname to the\ncaptured {B_OPPONENT_MON1_NAME}?");
static const u8 sText_PkmnSentToPC[] = _("{B_OPPONENT_MON1_NAME} fue enviado a la PC\nde {B_PC_CREATOR_NAME}.");
static const u8 sText_Someones[] = _("alguien");
static const u8 sText_Bills[] = _("BILL");
static const u8 sText_PkmnDataAddedToDex[] = _("Los datos de {B_OPPONENT_MON1_NAME} fueron\nañadidos a la POKéDEX.\p");
static const u8 sText_ItIsRaining[] = _("It is raining."); // used only in RSE when a battle starts in a rainy area
static const u8 sText_SandstormIsRaging[] = _("A sandstorm is raging.");
static const u8 sText_BoxIsFull[] = _("¡La CAJA está llena!\n¡No puedes guardar más!\p");
static const u8 sText_EnigmaBerry[] = _("ENIGMA BERRY");
static const u8 sText_BerrySuffix[] = _(" BAYA");
static const u8 gUnknown_83FD8B6[] = _("ナゾ");
static const u8 sText_PkmnsItemCuredParalysis[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\ncuró la parálisis!");
static const u8 sText_PkmnsItemCuredPoison[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\ncuró el envenenamiento!");
static const u8 sText_PkmnsItemHealedBurn[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\ncuró su quemadura!");
static const u8 sText_PkmnsItemDefrostedIt[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nlo descongeló!");
static const u8 sText_PkmnsItemWokeIt[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nlo despertó de su sueño!");
static const u8 sText_PkmnsItemSnappedOut[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nlo sacó de su confusión!");
static const u8 sText_PkmnsItemCuredProblem[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\ncuró su problema de {B_BUFF1}!");
static const u8 sText_PkmnsItemNormalizedStatus[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nnormalizó su estado!");
static const u8 sText_PkmnsItemRestoredHealth[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nrestauró salud!");
static const u8 sText_PkmnsItemRestoredPP[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nrestauró los PP de {B_BUFF1}!");
static const u8 sText_PkmnsItemRestoredStatus[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nrestauró su estado!");
static const u8 sText_PkmnsItemRestoredHPALittle[] = _("¡{B_LAST_ITEM} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nrestauró un poco de sus PS!");
static const u8 sText_ItemAllowsOnlyYMove[] = _("¡El efecto de {B_LAST_ITEM} solo\npermite usar {B_CURRENT_MOVE}!\p");
static const u8 sText_PkmnHungOnWithX[] = _("¡{B_DEF_NAME_WITH_PREFIX} resistió\nusando su {B_LAST_ITEM}!");
const u8 gText_EmptyString3[] = _("");
static const u8 sText_PlayedFluteCatchyTune[] = _("{B_PLAYER_NAME} played the {B_LAST_ITEM}.\pNow, that's a catchy tune!");
static const u8 sText_PlayedThe[] = _("{B_PLAYER_NAME} played the\n{B_LAST_ITEM}.");
static const u8 sText_PkmnHearingFluteAwoke[] = _("The POKéMON hearing the FLUTE\nawoke!");
static const u8 sText_YouThrowABallNowRight[] = _("You throw a BALL now, right?\nI… I'll do my best!");
const u8 gText_ForPetesSake[] = _("OAK: Oh, pero bueno…\nTan agresivo como siempre.\p{B_PLAYER_NAME}.\p¿Nunca has tenido una batalla\nPOKéMON antes, cierto?\pUna batalla POKéMON es cuando los\nENTRENADORES enfrentan a sus\lPOKéMON entre sí.\p");
const u8 gText_TheTrainerThat[] = _("El ENTRENADOR o ENTRENADORA que\nderrote a los POKéMON del oponente\lal bajar sus PS a “0,” gana.\p");
const u8 gText_TryBattling[] = _("Pero en vez de hablar sobre ello,\naprenderás más con la experiencia.\pIntenta luchar por tu cuenta.\p");
const u8 gText_InflictingDamageIsKey[] = _("OAK: Infligir daño al oponente es\nla clave de cualquier batalla.\p");
const u8 gText_LoweringStats[] = _("OAK: Reducir las estadísticas del\noponente te dará una ventaja.\p");
const u8 gText_KeepAnEyeOnHP[] = _("OAK: Mantén los ojos en los PS de\ntus POKéMON.\pSe debilitarán si sus PS bajan a\n“0.”\p");
const u8 gText_OakNoRunningFromATrainer[] = _("OAK: ¡No! ¡No hay escape de una\nbatalla entre ENTRENADORES!\p");
const u8 gText_WinEarnsPrizeMoney[] = _("OAK: ¡Hm! ¡Excelente!\p¡Si ganas, obtendrás dinero, y tus\nPOKéMON crecerán!\p¡Lucha con otros ENTRENADORES y\nhaz fuertes a tus POKéMON!\p");
const u8 gText_HowDissapointing[] = _("OAK: Hm…\nQué decepcionante…\pSi ganas, obtendrás dinero, y tus\nPOKéMON crecerán.\pPero si pierdes, {B_PLAYER_NAME},\ntendrás que pagar dinero…\pSin embargo, cómo esta vez no lo\nsabías, pagaré por ti.\pPero las cosas no serán así cuando\nestés fuera de estas puertas.\pPor eso debes luchar con POKéMON\nsalvajes para hacerte más fuerte.\p");

const u8 *const gBattleStringsTable[] = {
    [STRINGID_TRAINER1LOSETEXT - BATTLESTRINGS_TABLE_START]              = sText_Trainer1LoseText,
    [STRINGID_PKMNGAINEDEXP - BATTLESTRINGS_TABLE_START]                 = sText_PkmnGainedEXP,
    [STRINGID_PKMNGREWTOLV - BATTLESTRINGS_TABLE_START]                  = sText_PkmnGrewToLv,
    [STRINGID_PKMNLEARNEDMOVE - BATTLESTRINGS_TABLE_START]               = sText_PkmnLearnedMove,
    [STRINGID_TRYTOLEARNMOVE1 - BATTLESTRINGS_TABLE_START]               = sText_TryToLearnMove1,
    [STRINGID_TRYTOLEARNMOVE2 - BATTLESTRINGS_TABLE_START]               = sText_TryToLearnMove2,
    [STRINGID_TRYTOLEARNMOVE3 - BATTLESTRINGS_TABLE_START]               = sText_TryToLearnMove3,
    [STRINGID_PKMNFORGOTMOVE - BATTLESTRINGS_TABLE_START]                = sText_PkmnForgotMove,
    [STRINGID_STOPLEARNINGMOVE - BATTLESTRINGS_TABLE_START]              = sText_StopLearningMove,
    [STRINGID_DIDNOTLEARNMOVE - BATTLESTRINGS_TABLE_START]               = sText_DidNotLearnMove,
    [STRINGID_PKMNLEARNEDMOVE2 - BATTLESTRINGS_TABLE_START]              = sText_PkmnLearnedMove2,
    [STRINGID_ATTACKMISSED - BATTLESTRINGS_TABLE_START]                  = sText_AttackMissed,
    [STRINGID_PKMNPROTECTEDITSELF - BATTLESTRINGS_TABLE_START]           = sText_PkmnProtectedItself,
    [STRINGID_STATSWONTINCREASE2 - BATTLESTRINGS_TABLE_START]            = sText_StatsWontIncrease2,
    [STRINGID_AVOIDEDDAMAGE - BATTLESTRINGS_TABLE_START]                 = sText_AvoidedDamage,
    [STRINGID_ITDOESNTAFFECT - BATTLESTRINGS_TABLE_START]                = sText_ItDoesntAffect,
    [STRINGID_ATTACKERFAINTED - BATTLESTRINGS_TABLE_START]               = sText_AttackerFainted,
    [STRINGID_TARGETFAINTED - BATTLESTRINGS_TABLE_START]                 = sText_TargetFainted,
    [STRINGID_PLAYERGOTMONEY - BATTLESTRINGS_TABLE_START]                = sText_PlayerGotMoney,
    [STRINGID_PLAYERWHITEOUT - BATTLESTRINGS_TABLE_START]                = sText_PlayerWhiteout,
    [STRINGID_PLAYERWHITEOUT2 - BATTLESTRINGS_TABLE_START]               = sText_PlayerPanicked,
    [STRINGID_PREVENTSESCAPE - BATTLESTRINGS_TABLE_START]                = sText_PreventsEscape,
    [STRINGID_HITXTIMES - BATTLESTRINGS_TABLE_START]                     = sText_HitXTimes,
    [STRINGID_PKMNFELLASLEEP - BATTLESTRINGS_TABLE_START]                = sText_PkmnFellAsleep,
    [STRINGID_PKMNMADESLEEP - BATTLESTRINGS_TABLE_START]                 = sText_PkmnMadeSleep,
    [STRINGID_PKMNALREADYASLEEP - BATTLESTRINGS_TABLE_START]             = sText_PkmnAlreadyAsleep,
    [STRINGID_PKMNALREADYASLEEP2 - BATTLESTRINGS_TABLE_START]            = sText_PkmnAlreadyAsleep2,
    [STRINGID_PKMNWASNTAFFECTED - BATTLESTRINGS_TABLE_START]             = sText_PkmnWasntAffected,
    [STRINGID_PKMNWASPOISONED - BATTLESTRINGS_TABLE_START]               = sText_PkmnWasPoisoned,
    [STRINGID_PKMNPOISONEDBY - BATTLESTRINGS_TABLE_START]                = sText_PkmnPoisonedBy,
    [STRINGID_PKMNHURTBYPOISON - BATTLESTRINGS_TABLE_START]              = sText_PkmnHurtByPoison,
    [STRINGID_PKMNALREADYPOISONED - BATTLESTRINGS_TABLE_START]           = sText_PkmnAlreadyPoisoned,
    [STRINGID_PKMNBADLYPOISONED - BATTLESTRINGS_TABLE_START]             = sText_PkmnBadlyPoisoned,
    [STRINGID_PKMNENERGYDRAINED - BATTLESTRINGS_TABLE_START]             = sText_PkmnEnergyDrained,
    [STRINGID_PKMNWASBURNED - BATTLESTRINGS_TABLE_START]                 = sText_PkmnWasBurned,
    [STRINGID_PKMNBURNEDBY - BATTLESTRINGS_TABLE_START]                  = sText_PkmnBurnedBy,
    [STRINGID_PKMNHURTBYBURN - BATTLESTRINGS_TABLE_START]                = sText_PkmnHurtByBurn,
    [STRINGID_PKMNWASFROZEN - BATTLESTRINGS_TABLE_START]                 = sText_PkmnWasFrozen,
    [STRINGID_PKMNFROZENBY - BATTLESTRINGS_TABLE_START]                  = sText_PkmnFrozenBy,
    [STRINGID_PKMNISFROZEN - BATTLESTRINGS_TABLE_START]                  = sText_PkmnIsFrozen,
    [STRINGID_PKMNWASDEFROSTED - BATTLESTRINGS_TABLE_START]              = sText_PkmnWasDefrosted,
    [STRINGID_PKMNWASDEFROSTED2 - BATTLESTRINGS_TABLE_START]             = sText_PkmnWasDefrosted2,
    [STRINGID_PKMNWASDEFROSTEDBY - BATTLESTRINGS_TABLE_START]            = sText_PkmnWasDefrostedBy,
    [STRINGID_PKMNWASPARALYZED - BATTLESTRINGS_TABLE_START]              = sText_PkmnWasParalyzed,
    [STRINGID_PKMNWASPARALYZEDBY - BATTLESTRINGS_TABLE_START]            = sText_PkmnWasParalyzedBy,
    [STRINGID_PKMNISPARALYZED - BATTLESTRINGS_TABLE_START]               = sText_PkmnIsParalyzed,
    [STRINGID_PKMNISALREADYPARALYZED - BATTLESTRINGS_TABLE_START]        = sText_PkmnIsAlreadyParalyzed,
    [STRINGID_PKMNHEALEDPARALYSIS - BATTLESTRINGS_TABLE_START]           = sText_PkmnHealedParalysis,
    [STRINGID_PKMNDREAMEATEN - BATTLESTRINGS_TABLE_START]                = sText_PkmnDreamEaten,
    [STRINGID_STATSWONTINCREASE - BATTLESTRINGS_TABLE_START]             = sText_StatsWontIncrease,
    [STRINGID_STATSWONTDECREASE - BATTLESTRINGS_TABLE_START]             = sText_StatsWontDecrease,
    [STRINGID_TEAMSTOPPEDWORKING - BATTLESTRINGS_TABLE_START]            = sText_TeamStoppedWorking,
    [STRINGID_FOESTOPPEDWORKING - BATTLESTRINGS_TABLE_START]             = sText_FoeStoppedWorking,
    [STRINGID_PKMNISCONFUSED - BATTLESTRINGS_TABLE_START]                = sText_PkmnIsConfused,
    [STRINGID_PKMNHEALEDCONFUSION - BATTLESTRINGS_TABLE_START]           = sText_PkmnHealedConfusion,
    [STRINGID_PKMNWASCONFUSED - BATTLESTRINGS_TABLE_START]               = sText_PkmnWasConfused,
    [STRINGID_PKMNALREADYCONFUSED - BATTLESTRINGS_TABLE_START]           = sText_PkmnAlreadyConfused,
    [STRINGID_PKMNFELLINLOVE - BATTLESTRINGS_TABLE_START]                = sText_PkmnFellInLove,
    [STRINGID_PKMNINLOVE - BATTLESTRINGS_TABLE_START]                    = sText_PkmnInLove,
    [STRINGID_PKMNIMMOBILIZEDBYLOVE - BATTLESTRINGS_TABLE_START]         = sText_PkmnImmobilizedByLove,
    [STRINGID_PKMNBLOWNAWAY - BATTLESTRINGS_TABLE_START]                 = sText_PkmnBlownAway,
    [STRINGID_PKMNCHANGEDTYPE - BATTLESTRINGS_TABLE_START]               = sText_PkmnChangedType,
    [STRINGID_PKMNFLINCHED - BATTLESTRINGS_TABLE_START]                  = sText_PkmnFlinched,
    [STRINGID_PKMNREGAINEDHEALTH - BATTLESTRINGS_TABLE_START]            = sText_PkmnRegainedHealth,
    [STRINGID_PKMNHPFULL - BATTLESTRINGS_TABLE_START]                    = sText_PkmnHPFull,
    [STRINGID_PKMNRAISEDSPDEF - BATTLESTRINGS_TABLE_START]               = sText_PkmnRaisedSpDef,
    [STRINGID_PKMNRAISEDDEF - BATTLESTRINGS_TABLE_START]                 = sText_PkmnRaisedDef,
    [STRINGID_PKMNCOVEREDBYVEIL - BATTLESTRINGS_TABLE_START]             = sText_PkmnCoveredByVeil,
    [STRINGID_PKMNUSEDSAFEGUARD - BATTLESTRINGS_TABLE_START]             = sText_PkmnUsedSafeguard,
    [STRINGID_PKMNSAFEGUARDEXPIRED - BATTLESTRINGS_TABLE_START]          = sText_PkmnSafeguardExpired,
    [STRINGID_PKMNWENTTOSLEEP - BATTLESTRINGS_TABLE_START]               = sText_PkmnWentToSleep,
    [STRINGID_PKMNSLEPTHEALTHY - BATTLESTRINGS_TABLE_START]              = sText_PkmnSleptHealthy,
    [STRINGID_PKMNWHIPPEDWHIRLWIND - BATTLESTRINGS_TABLE_START]          = sText_PkmnWhippedWhirlwind,
    [STRINGID_PKMNTOOKSUNLIGHT - BATTLESTRINGS_TABLE_START]              = sText_PkmnTookSunlight,
    [STRINGID_PKMNLOWEREDHEAD - BATTLESTRINGS_TABLE_START]               = sText_PkmnLoweredHead,
    [STRINGID_PKMNISGLOWING - BATTLESTRINGS_TABLE_START]                 = sText_PkmnIsGlowing,
    [STRINGID_PKMNFLEWHIGH - BATTLESTRINGS_TABLE_START]                  = sText_PkmnFlewHigh,
    [STRINGID_PKMNDUGHOLE - BATTLESTRINGS_TABLE_START]                   = sText_PkmnDugHole,
    [STRINGID_PKMNSQUEEZEDBYBIND - BATTLESTRINGS_TABLE_START]            = sText_PkmnSqueezedByBind,
    [STRINGID_PKMNTRAPPEDINVORTEX - BATTLESTRINGS_TABLE_START]           = sText_PkmnTrappedInVortex,
    [STRINGID_PKMNWRAPPEDBY - BATTLESTRINGS_TABLE_START]                 = sText_PkmnWrappedBy,
    [STRINGID_PKMNCLAMPED - BATTLESTRINGS_TABLE_START]                   = sText_PkmnClamped,
    [STRINGID_PKMNHURTBY - BATTLESTRINGS_TABLE_START]                    = sText_PkmnHurtBy,
    [STRINGID_PKMNFREEDFROM - BATTLESTRINGS_TABLE_START]                 = sText_PkmnFreedFrom,
    [STRINGID_PKMNCRASHED - BATTLESTRINGS_TABLE_START]                   = sText_PkmnCrashed,
    [STRINGID_PKMNSHROUDEDINMIST - BATTLESTRINGS_TABLE_START]            = gBattleText_MistShroud,
    [STRINGID_PKMNPROTECTEDBYMIST - BATTLESTRINGS_TABLE_START]           = sText_PkmnProtectedByMist,
    [STRINGID_PKMNGETTINGPUMPED - BATTLESTRINGS_TABLE_START]             = gBattleText_GetPumped,
    [STRINGID_PKMNHITWITHRECOIL - BATTLESTRINGS_TABLE_START]             = sText_PkmnHitWithRecoil,
    [STRINGID_PKMNPROTECTEDITSELF2 - BATTLESTRINGS_TABLE_START]          = sText_PkmnProtectedItself2,
    [STRINGID_PKMNBUFFETEDBYSANDSTORM - BATTLESTRINGS_TABLE_START]       = sText_PkmnBuffetedBySandstorm,
    [STRINGID_PKMNPELTEDBYHAIL - BATTLESTRINGS_TABLE_START]              = sText_PkmnPeltedByHail,
    [STRINGID_PKMNSEEDED - BATTLESTRINGS_TABLE_START]                    = sText_PkmnSeeded,
    [STRINGID_PKMNEVADEDATTACK - BATTLESTRINGS_TABLE_START]              = sText_PkmnEvadedAttack,
    [STRINGID_PKMNSAPPEDBYLEECHSEED - BATTLESTRINGS_TABLE_START]         = sText_PkmnSappedByLeechSeed,
    [STRINGID_PKMNFASTASLEEP - BATTLESTRINGS_TABLE_START]                = sText_PkmnFastAsleep,
    [STRINGID_PKMNWOKEUP - BATTLESTRINGS_TABLE_START]                    = sText_PkmnWokeUp,
    [STRINGID_PKMNUPROARKEPTAWAKE - BATTLESTRINGS_TABLE_START]           = sText_PkmnUproarKeptAwake,
    [STRINGID_PKMNWOKEUPINUPROAR - BATTLESTRINGS_TABLE_START]            = sText_PkmnWokeUpInUproar,
    [STRINGID_PKMNCAUSEDUPROAR - BATTLESTRINGS_TABLE_START]              = sText_PkmnCausedUproar,
    [STRINGID_PKMNMAKINGUPROAR - BATTLESTRINGS_TABLE_START]              = sText_PkmnMakingUproar,
    [STRINGID_PKMNCALMEDDOWN - BATTLESTRINGS_TABLE_START]                = sText_PkmnCalmedDown,
    [STRINGID_PKMNCANTSLEEPINUPROAR - BATTLESTRINGS_TABLE_START]         = sText_PkmnCantSleepInUproar,
    [STRINGID_PKMNSTOCKPILED - BATTLESTRINGS_TABLE_START]                = sText_PkmnStockpiled,
    [STRINGID_PKMNCANTSTOCKPILE - BATTLESTRINGS_TABLE_START]             = sText_PkmnCantStockpile,
    [STRINGID_PKMNCANTSLEEPINUPROAR2 - BATTLESTRINGS_TABLE_START]        = sText_PkmnCantSleepInUproar2,
    [STRINGID_UPROARKEPTPKMNAWAKE - BATTLESTRINGS_TABLE_START]           = sText_UproarKeptPkmnAwake,
    [STRINGID_PKMNSTAYEDAWAKEUSING - BATTLESTRINGS_TABLE_START]          = sText_PkmnStayedAwakeUsing,
    [STRINGID_PKMNSTORINGENERGY - BATTLESTRINGS_TABLE_START]             = sText_PkmnStoringEnergy,
    [STRINGID_PKMNUNLEASHEDENERGY - BATTLESTRINGS_TABLE_START]           = sText_PkmnUnleashedEnergy,
    [STRINGID_PKMNFATIGUECONFUSION - BATTLESTRINGS_TABLE_START]          = sText_PkmnFatigueConfusion,
    [STRINGID_PLAYERPICKEDUPMONEY - BATTLESTRINGS_TABLE_START]              = sText_PkmnPickedUpItem,
    [STRINGID_PKMNUNAFFECTED - BATTLESTRINGS_TABLE_START]                = sText_PkmnUnaffected,
    [STRINGID_PKMNTRANSFORMEDINTO - BATTLESTRINGS_TABLE_START]           = sText_PkmnTransformedInto,
    [STRINGID_PKMNMADESUBSTITUTE - BATTLESTRINGS_TABLE_START]            = sText_PkmnMadeSubstitute,
    [STRINGID_PKMNHASSUBSTITUTE - BATTLESTRINGS_TABLE_START]             = sText_PkmnHasSubstitute,
    [STRINGID_SUBSTITUTEDAMAGED - BATTLESTRINGS_TABLE_START]             = sText_SubstituteDamaged,
    [STRINGID_PKMNSUBSTITUTEFADED - BATTLESTRINGS_TABLE_START]           = sText_PkmnSubstituteFaded,
    [STRINGID_PKMNMUSTRECHARGE - BATTLESTRINGS_TABLE_START]              = sText_PkmnMustRecharge,
    [STRINGID_PKMNRAGEBUILDING - BATTLESTRINGS_TABLE_START]              = sText_PkmnRageBuilding,
    [STRINGID_PKMNMOVEWASDISABLED - BATTLESTRINGS_TABLE_START]           = sText_PkmnMoveWasDisabled,
    [STRINGID_PKMNMOVEISDISABLED - BATTLESTRINGS_TABLE_START]            = sText_PkmnMoveIsDisabled,
    [STRINGID_PKMNMOVEDISABLEDNOMORE - BATTLESTRINGS_TABLE_START]        = sText_PkmnMoveDisabledNoMore,
    [STRINGID_PKMNGOTENCORE - BATTLESTRINGS_TABLE_START]                 = sText_PkmnGotEncore,
    [STRINGID_PKMNENCOREENDED - BATTLESTRINGS_TABLE_START]               = sText_PkmnEncoreEnded,
    [STRINGID_PKMNTOOKAIM - BATTLESTRINGS_TABLE_START]                   = sText_PkmnTookAim,
    [STRINGID_PKMNSKETCHEDMOVE - BATTLESTRINGS_TABLE_START]              = sText_PkmnSketchedMove,
    [STRINGID_PKMNTRYINGTOTAKEFOE - BATTLESTRINGS_TABLE_START]           = sText_PkmnTryingToTakeFoe,
    [STRINGID_PKMNTOOKFOE - BATTLESTRINGS_TABLE_START]                   = sText_PkmnTookFoe,
    [STRINGID_PKMNREDUCEDPP - BATTLESTRINGS_TABLE_START]                 = sText_PkmnReducedPP,
    [STRINGID_PKMNSTOLEITEM - BATTLESTRINGS_TABLE_START]                 = sText_PkmnStoleItem,
    [STRINGID_TARGETCANTESCAPENOW - BATTLESTRINGS_TABLE_START]           = sText_TargetCantEscapeNow,
    [STRINGID_PKMNFELLINTONIGHTMARE - BATTLESTRINGS_TABLE_START]         = sText_PkmnFellIntoNightmare,
    [STRINGID_PKMNLOCKEDINNIGHTMARE - BATTLESTRINGS_TABLE_START]         = sText_PkmnLockedInNightmare,
    [STRINGID_PKMNLAIDCURSE - BATTLESTRINGS_TABLE_START]                 = sText_PkmnLaidCurse,
    [STRINGID_PKMNAFFLICTEDBYCURSE - BATTLESTRINGS_TABLE_START]          = sText_PkmnAfflictedByCurse,
    [STRINGID_SPIKESSCATTERED - BATTLESTRINGS_TABLE_START]               = sText_SpikesScattered,
    [STRINGID_PKMNHURTBYSPIKES - BATTLESTRINGS_TABLE_START]              = sText_PkmnHurtBySpikes,
    [STRINGID_PKMNIDENTIFIED - BATTLESTRINGS_TABLE_START]                = sText_PkmnIdentified,
    [STRINGID_PKMNPERISHCOUNTFELL - BATTLESTRINGS_TABLE_START]           = sText_PkmnPerishCountFell,
    [STRINGID_PKMNBRACEDITSELF - BATTLESTRINGS_TABLE_START]              = sText_PkmnBracedItself,
    [STRINGID_PKMNENDUREDHIT - BATTLESTRINGS_TABLE_START]                = sText_PkmnEnduredHit,
    [STRINGID_MAGNITUDESTRENGTH - BATTLESTRINGS_TABLE_START]             = sText_MagnitudeStrength,
    [STRINGID_PKMNCUTHPMAXEDATTACK - BATTLESTRINGS_TABLE_START]          = sText_PkmnCutHPMaxedAttack,
    [STRINGID_PKMNCOPIEDSTATCHANGES - BATTLESTRINGS_TABLE_START]         = sText_PkmnCopiedStatChanges,
    [STRINGID_PKMNGOTFREE - BATTLESTRINGS_TABLE_START]                   = sText_PkmnGotFree,
    [STRINGID_PKMNSHEDLEECHSEED - BATTLESTRINGS_TABLE_START]             = sText_PkmnShedLeechSeed,
    [STRINGID_PKMNBLEWAWAYSPIKES - BATTLESTRINGS_TABLE_START]            = sText_PkmnBlewAwaySpikes,
    [STRINGID_PKMNFLEDFROMBATTLE - BATTLESTRINGS_TABLE_START]            = sText_PkmnFledFromBattle,
    [STRINGID_PKMNFORESAWATTACK - BATTLESTRINGS_TABLE_START]             = sText_PkmnForesawAttack,
    [STRINGID_PKMNTOOKATTACK - BATTLESTRINGS_TABLE_START]                = sText_PkmnTookAttack,
    [STRINGID_PKMNATTACK - BATTLESTRINGS_TABLE_START]                    = sText_PkmnAttack,
    [STRINGID_PKMNCENTERATTENTION - BATTLESTRINGS_TABLE_START]           = sText_PkmnCenterAttention,
    [STRINGID_PKMNCHARGINGPOWER - BATTLESTRINGS_TABLE_START]             = sText_PkmnChargingPower,
    [STRINGID_NATUREPOWERTURNEDINTO - BATTLESTRINGS_TABLE_START]         = sText_NaturePowerTurnedInto,
    [STRINGID_PKMNSTATUSNORMAL - BATTLESTRINGS_TABLE_START]              = sText_PkmnStatusNormal,
    [STRINGID_PKMNHASNOMOVESLEFT - BATTLESTRINGS_TABLE_START]            = sText_PkmnHasNoMovesLeft,
    [STRINGID_PKMNSUBJECTEDTOTORMENT - BATTLESTRINGS_TABLE_START]        = sText_PkmnSubjectedToTorment,
    [STRINGID_PKMNCANTUSEMOVETORMENT - BATTLESTRINGS_TABLE_START]        = sText_PkmnCantUseMoveTorment,
    [STRINGID_PKMNTIGHTENINGFOCUS - BATTLESTRINGS_TABLE_START]           = sText_PkmnTighteningFocus,
    [STRINGID_PKMNFELLFORTAUNT - BATTLESTRINGS_TABLE_START]              = sText_PkmnFellForTaunt,
    [STRINGID_PKMNCANTUSEMOVETAUNT - BATTLESTRINGS_TABLE_START]          = sText_PkmnCantUseMoveTaunt,
    [STRINGID_PKMNREADYTOHELP - BATTLESTRINGS_TABLE_START]               = sText_PkmnReadyToHelp,
    [STRINGID_PKMNSWITCHEDITEMS - BATTLESTRINGS_TABLE_START]             = sText_PkmnSwitchedItems,
    [STRINGID_PKMNCOPIEDFOE - BATTLESTRINGS_TABLE_START]                 = sText_PkmnCopiedFoe,
    [STRINGID_PKMNMADEWISH - BATTLESTRINGS_TABLE_START]                  = sText_PkmnMadeWish,
    [STRINGID_PKMNWISHCAMETRUE - BATTLESTRINGS_TABLE_START]              = sText_PkmnWishCameTrue,
    [STRINGID_PKMNPLANTEDROOTS - BATTLESTRINGS_TABLE_START]              = sText_PkmnPlantedRoots,
    [STRINGID_PKMNABSORBEDNUTRIENTS - BATTLESTRINGS_TABLE_START]         = sText_PkmnAbsorbedNutrients,
    [STRINGID_PKMNANCHOREDITSELF - BATTLESTRINGS_TABLE_START]            = sText_PkmnAnchoredItself,
    [STRINGID_PKMNWASMADEDROWSY - BATTLESTRINGS_TABLE_START]             = sText_PkmnWasMadeDrowsy,
    [STRINGID_PKMNKNOCKEDOFF - BATTLESTRINGS_TABLE_START]                = sText_PkmnKnockedOff,
    [STRINGID_PKMNSWAPPEDABILITIES - BATTLESTRINGS_TABLE_START]          = sText_PkmnSwappedAbilities,
    [STRINGID_PKMNSEALEDOPPONENTMOVE - BATTLESTRINGS_TABLE_START]        = sText_PkmnSealedOpponentMove,
    [STRINGID_PKMNCANTUSEMOVESEALED - BATTLESTRINGS_TABLE_START]         = sText_PkmnCantUseMoveSealed,
    [STRINGID_PKMNWANTSGRUDGE - BATTLESTRINGS_TABLE_START]               = sText_PkmnWantsGrudge,
    [STRINGID_PKMNLOSTPPGRUDGE - BATTLESTRINGS_TABLE_START]              = sText_PkmnLostPPGrudge,
    [STRINGID_PKMNSHROUDEDITSELF - BATTLESTRINGS_TABLE_START]            = sText_PkmnShroudedItself,
    [STRINGID_PKMNMOVEBOUNCED - BATTLESTRINGS_TABLE_START]               = sText_PkmnMoveBounced,
    [STRINGID_PKMNWAITSFORTARGET - BATTLESTRINGS_TABLE_START]            = sText_PkmnWaitsForTarget,
    [STRINGID_PKMNSNATCHEDMOVE - BATTLESTRINGS_TABLE_START]              = sText_PkmnSnatchedMove,
    [STRINGID_PKMNMADEITRAIN - BATTLESTRINGS_TABLE_START]                = sText_PkmnMadeItRain,
    [STRINGID_PKMNRAISEDSPEED - BATTLESTRINGS_TABLE_START]               = sText_PkmnRaisedSpeed,
    [STRINGID_PKMNPROTECTEDBY - BATTLESTRINGS_TABLE_START]               = sText_PkmnProtectedBy,
    [STRINGID_PKMNPREVENTSUSAGE - BATTLESTRINGS_TABLE_START]             = sText_PkmnPreventsUsage,
    [STRINGID_PKMNRESTOREDHPUSING - BATTLESTRINGS_TABLE_START]           = sText_PkmnRestoredHPUsing,
    [STRINGID_PKMNCHANGEDTYPEWITH - BATTLESTRINGS_TABLE_START]           = sText_PkmnChangedTypeWith,
    [STRINGID_PKMNPREVENTSPARALYSISWITH - BATTLESTRINGS_TABLE_START]     = sText_PkmnPreventsParalysisWith,
    [STRINGID_PKMNPREVENTSROMANCEWITH - BATTLESTRINGS_TABLE_START]       = sText_PkmnPreventsRomanceWith,
    [STRINGID_PKMNPREVENTSPOISONINGWITH - BATTLESTRINGS_TABLE_START]     = sText_PkmnPreventsPoisoningWith,
    [STRINGID_PKMNPREVENTSCONFUSIONWITH - BATTLESTRINGS_TABLE_START]     = sText_PkmnPreventsConfusionWith,
    [STRINGID_PKMNRAISEDFIREPOWERWITH - BATTLESTRINGS_TABLE_START]       = sText_PkmnRaisedFirePowerWith,
    [STRINGID_PKMNANCHORSITSELFWITH - BATTLESTRINGS_TABLE_START]         = sText_PkmnAnchorsItselfWith,
    [STRINGID_PKMNCUTSATTACKWITH - BATTLESTRINGS_TABLE_START]            = sText_PkmnCutsAttackWith,
    [STRINGID_PKMNPREVENTSSTATLOSSWITH - BATTLESTRINGS_TABLE_START]      = sText_PkmnPreventsStatLossWith,
    [STRINGID_PKMNHURTSWITH - BATTLESTRINGS_TABLE_START]                 = sText_PkmnHurtsWith,
    [STRINGID_PKMNTRACED - BATTLESTRINGS_TABLE_START]                    = sText_PkmnTraced,
    [STRINGID_STATSHARPLY - BATTLESTRINGS_TABLE_START]                   = sText_StatSharply,
    [STRINGID_STATROSE - BATTLESTRINGS_TABLE_START]                      = gBattleText_Rose,
    [STRINGID_STATHARSHLY - BATTLESTRINGS_TABLE_START]                   = sText_StatHarshly,
    [STRINGID_STATFELL - BATTLESTRINGS_TABLE_START]                      = sText_StatFell,
    [STRINGID_ATTACKERSSTATROSE - BATTLESTRINGS_TABLE_START]             = sText_AttackersStatRose,
    [STRINGID_DEFENDERSSTATROSE - BATTLESTRINGS_TABLE_START]             = gText_DefendersStatRose,
    [STRINGID_ATTACKERSSTATFELL - BATTLESTRINGS_TABLE_START]             = sText_AttackersStatFell,
    [STRINGID_DEFENDERSSTATFELL - BATTLESTRINGS_TABLE_START]             = sText_DefendersStatFell,
    [STRINGID_CRITICALHIT - BATTLESTRINGS_TABLE_START]                   = sText_CriticalHit,
    [STRINGID_ONEHITKO - BATTLESTRINGS_TABLE_START]                      = sText_OneHitKO,
    [STRINGID_123POOF - BATTLESTRINGS_TABLE_START]                       = sText_123Poof,
    [STRINGID_ANDELLIPSIS - BATTLESTRINGS_TABLE_START]                   = sText_AndEllipsis,
    [STRINGID_NOTVERYEFFECTIVE - BATTLESTRINGS_TABLE_START]              = sText_NotVeryEffective,
    [STRINGID_SUPEREFFECTIVE - BATTLESTRINGS_TABLE_START]                = sText_SuperEffective,
    [STRINGID_GOTAWAYSAFELY - BATTLESTRINGS_TABLE_START]                 = sText_GotAwaySafely,
    [STRINGID_WILDPKMNFLED - BATTLESTRINGS_TABLE_START]                  = sText_WildPkmnFled,
    [STRINGID_NORUNNINGFROMTRAINERS - BATTLESTRINGS_TABLE_START]         = sText_NoRunningFromTrainers,
    [STRINGID_CANTESCAPE - BATTLESTRINGS_TABLE_START]                    = sText_CantEscape,
    [STRINGID_DONTLEAVEBIRCH - BATTLESTRINGS_TABLE_START]                = sText_EmptyString5,
    [STRINGID_BUTNOTHINGHAPPENED - BATTLESTRINGS_TABLE_START]            = sText_ButNothingHappened,
    [STRINGID_BUTITFAILED - BATTLESTRINGS_TABLE_START]                   = sText_ButItFailed,
    [STRINGID_ITHURTCONFUSION - BATTLESTRINGS_TABLE_START]               = sText_ItHurtConfusion,
    [STRINGID_MIRRORMOVEFAILED - BATTLESTRINGS_TABLE_START]              = sText_MirrorMoveFailed,
    [STRINGID_STARTEDTORAIN - BATTLESTRINGS_TABLE_START]                 = sText_StartedToRain,
    [STRINGID_DOWNPOURSTARTED - BATTLESTRINGS_TABLE_START]               = sText_DownpourStarted,
    [STRINGID_RAINCONTINUES - BATTLESTRINGS_TABLE_START]                 = sText_RainContinues,
    [STRINGID_DOWNPOURCONTINUES - BATTLESTRINGS_TABLE_START]             = sText_DownpourContinues,
    [STRINGID_RAINSTOPPED - BATTLESTRINGS_TABLE_START]                   = sText_RainStopped,
    [STRINGID_SANDSTORMBREWED - BATTLESTRINGS_TABLE_START]               = sText_SandstormBrewed,
    [STRINGID_SANDSTORMRAGES - BATTLESTRINGS_TABLE_START]                = sText_SandstormRages,
    [STRINGID_SANDSTORMSUBSIDED - BATTLESTRINGS_TABLE_START]             = sText_SandstormSubsided,
    [STRINGID_SUNLIGHTGOTBRIGHT - BATTLESTRINGS_TABLE_START]             = sText_SunlightGotBright,
    [STRINGID_SUNLIGHTSTRONG - BATTLESTRINGS_TABLE_START]                = sText_SunlightStrong,
    [STRINGID_SUNLIGHTFADED - BATTLESTRINGS_TABLE_START]                 = sText_SunlightFaded,
    [STRINGID_STARTEDHAIL - BATTLESTRINGS_TABLE_START]                   = sText_StartedHail,
    [STRINGID_HAILCONTINUES - BATTLESTRINGS_TABLE_START]                 = sText_HailContinues,
    [STRINGID_HAILSTOPPED - BATTLESTRINGS_TABLE_START]                   = sText_HailStopped,
    [STRINGID_FAILEDTOSPITUP - BATTLESTRINGS_TABLE_START]                = sText_FailedToSpitUp,
    [STRINGID_FAILEDTOSWALLOW - BATTLESTRINGS_TABLE_START]               = sText_FailedToSwallow,
    [STRINGID_WINDBECAMEHEATWAVE - BATTLESTRINGS_TABLE_START]            = sText_WindBecameHeatWave,
    [STRINGID_STATCHANGESGONE - BATTLESTRINGS_TABLE_START]               = sText_StatChangesGone,
    [STRINGID_COINSSCATTERED - BATTLESTRINGS_TABLE_START]                = sText_CoinsScattered,
    [STRINGID_TOOWEAKFORSUBSTITUTE - BATTLESTRINGS_TABLE_START]          = sText_TooWeakForSubstitute,
    [STRINGID_SHAREDPAIN - BATTLESTRINGS_TABLE_START]                    = sText_SharedPain,
    [STRINGID_BELLCHIMED - BATTLESTRINGS_TABLE_START]                    = sText_BellChimed,
    [STRINGID_FAINTINTHREE - BATTLESTRINGS_TABLE_START]                  = sText_FaintInThree,
    [STRINGID_NOPPLEFT - BATTLESTRINGS_TABLE_START]                      = sText_NoPPLeft,
    [STRINGID_BUTNOPPLEFT - BATTLESTRINGS_TABLE_START]                   = sText_ButNoPPLeft,
    [STRINGID_PLAYERUSEDITEM - BATTLESTRINGS_TABLE_START]                = sText_PlayerUsedItem,
    [STRINGID_OLDMANUSEDITEM - BATTLESTRINGS_TABLE_START]                = sText_OldManUsedItem,
    [STRINGID_TRAINERBLOCKEDBALL - BATTLESTRINGS_TABLE_START]            = sText_TrainerBlockedBall,
    [STRINGID_DONTBEATHIEF - BATTLESTRINGS_TABLE_START]                  = sText_DontBeAThief,
    [STRINGID_ITDODGEDBALL - BATTLESTRINGS_TABLE_START]                  = sText_ItDodgedBall,
    [STRINGID_YOUMISSEDPKMN - BATTLESTRINGS_TABLE_START]                 = sText_YouMissedPkmn,
    [STRINGID_PKMNBROKEFREE - BATTLESTRINGS_TABLE_START]                 = sText_PkmnBrokeFree,
    [STRINGID_ITAPPEAREDCAUGHT - BATTLESTRINGS_TABLE_START]              = sText_ItAppearedCaught,
    [STRINGID_AARGHALMOSTHADIT - BATTLESTRINGS_TABLE_START]              = sText_AarghAlmostHadIt,
    [STRINGID_SHOOTSOCLOSE - BATTLESTRINGS_TABLE_START]                  = sText_ShootSoClose,
    [STRINGID_GOTCHAPKMNCAUGHT - BATTLESTRINGS_TABLE_START]              = sText_GotchaPkmnCaught,
    [STRINGID_GOTCHAPKMNCAUGHT2 - BATTLESTRINGS_TABLE_START]             = sText_GotchaPkmnCaught2,
    [STRINGID_GIVENICKNAMECAPTURED - BATTLESTRINGS_TABLE_START]          = sText_GiveNicknameCaptured,
    [STRINGID_PKMNSENTTOPC - BATTLESTRINGS_TABLE_START]                  = sText_PkmnSentToPC,
    [STRINGID_PKMNDATAADDEDTODEX - BATTLESTRINGS_TABLE_START]            = sText_PkmnDataAddedToDex,
    [STRINGID_ITISRAINING - BATTLESTRINGS_TABLE_START]                   = sText_ItIsRaining,
    [STRINGID_SANDSTORMISRAGING - BATTLESTRINGS_TABLE_START]             = sText_SandstormIsRaging,
    [STRINGID_CANTESCAPE2 - BATTLESTRINGS_TABLE_START]                   = sText_CantEscape2,
    [STRINGID_PKMNIGNORESASLEEP - BATTLESTRINGS_TABLE_START]             = sText_PkmnIgnoresAsleep,
    [STRINGID_PKMNIGNOREDORDERS - BATTLESTRINGS_TABLE_START]             = sText_PkmnIgnoredOrders,
    [STRINGID_PKMNBEGANTONAP - BATTLESTRINGS_TABLE_START]                = sText_PkmnBeganToNap,
    [STRINGID_PKMNLOAFING - BATTLESTRINGS_TABLE_START]                   = sText_PkmnLoafing,
    [STRINGID_PKMNWONTOBEY - BATTLESTRINGS_TABLE_START]                  = sText_PkmnWontObey,
    [STRINGID_PKMNTURNEDAWAY - BATTLESTRINGS_TABLE_START]                = sText_PkmnTurnedAway,
    [STRINGID_PKMNPRETENDNOTNOTICE - BATTLESTRINGS_TABLE_START]          = sText_PkmnPretendNotNotice,
    [STRINGID_ENEMYABOUTTOSWITCHPKMN - BATTLESTRINGS_TABLE_START]        = sText_EnemyAboutToSwitchPkmn,
    [STRINGID_THREWROCK - BATTLESTRINGS_TABLE_START]                     = sText_ThrewARock,
    [STRINGID_THREWBAIT - BATTLESTRINGS_TABLE_START]                     = sText_ThrewSomeBait,
    [STRINGID_PKMNWATCHINGCAREFULLY - BATTLESTRINGS_TABLE_START]         = sText_PkmnWatchingCarefully,
    [STRINGID_PKMNANGRY - BATTLESTRINGS_TABLE_START]                     = sText_PkmnIsAngry,
    [STRINGID_PKMNEATING - BATTLESTRINGS_TABLE_START]                    = sText_PkmnIsEating,
    [STRINGID_DUMMY288 - BATTLESTRINGS_TABLE_START]                      = sText_Empty1,
    [STRINGID_DUMMY289 - BATTLESTRINGS_TABLE_START]                      = sText_Empty1,
    [STRINGID_OUTOFSAFARIBALLS - BATTLESTRINGS_TABLE_START]              = sText_OutOfSafariBalls,
    [STRINGID_PKMNSITEMCUREDPARALYSIS - BATTLESTRINGS_TABLE_START]       = sText_PkmnsItemCuredParalysis,
    [STRINGID_PKMNSITEMCUREDPOISON - BATTLESTRINGS_TABLE_START]          = sText_PkmnsItemCuredPoison,
    [STRINGID_PKMNSITEMHEALEDBURN - BATTLESTRINGS_TABLE_START]           = sText_PkmnsItemHealedBurn,
    [STRINGID_PKMNSITEMDEFROSTEDIT - BATTLESTRINGS_TABLE_START]          = sText_PkmnsItemDefrostedIt,
    [STRINGID_PKMNSITEMWOKEIT - BATTLESTRINGS_TABLE_START]               = sText_PkmnsItemWokeIt,
    [STRINGID_PKMNSITEMSNAPPEDOUT - BATTLESTRINGS_TABLE_START]           = sText_PkmnsItemSnappedOut,
    [STRINGID_PKMNSITEMCUREDPROBLEM - BATTLESTRINGS_TABLE_START]         = sText_PkmnsItemCuredProblem,
    [STRINGID_PKMNSITEMRESTOREDHEALTH - BATTLESTRINGS_TABLE_START]       = sText_PkmnsItemRestoredHealth,
    [STRINGID_PKMNSITEMRESTOREDPP - BATTLESTRINGS_TABLE_START]           = sText_PkmnsItemRestoredPP,
    [STRINGID_PKMNSITEMRESTOREDSTATUS - BATTLESTRINGS_TABLE_START]       = sText_PkmnsItemRestoredStatus,
    [STRINGID_PKMNSITEMRESTOREDHPALITTLE - BATTLESTRINGS_TABLE_START]    = sText_PkmnsItemRestoredHPALittle,
    [STRINGID_ITEMALLOWSONLYYMOVE - BATTLESTRINGS_TABLE_START]           = sText_ItemAllowsOnlyYMove,
    [STRINGID_PKMNHUNGONWITHX - BATTLESTRINGS_TABLE_START]               = sText_PkmnHungOnWithX,
    [STRINGID_EMPTYSTRING3 - BATTLESTRINGS_TABLE_START]                  = gText_EmptyString3,
    [STRINGID_PKMNSXPREVENTSBURNS - BATTLESTRINGS_TABLE_START]           = sText_PkmnsXPreventsBurns,
    [STRINGID_PKMNSXBLOCKSY - BATTLESTRINGS_TABLE_START]                 = sText_PkmnsXBlocksY,
    [STRINGID_PKMNSXRESTOREDHPALITTLE2 - BATTLESTRINGS_TABLE_START]      = sText_PkmnsXRestoredHPALittle2,
    [STRINGID_PKMNSXWHIPPEDUPSANDSTORM - BATTLESTRINGS_TABLE_START]      = sText_PkmnsXWhippedUpSandstorm,
    [STRINGID_PKMNSXPREVENTSYLOSS - BATTLESTRINGS_TABLE_START]           = sText_PkmnsXPreventsYLoss,
    [STRINGID_PKMNSXINFATUATEDY - BATTLESTRINGS_TABLE_START]             = sText_PkmnsXInfatuatedY,
    [STRINGID_PKMNSXMADEYINEFFECTIVE - BATTLESTRINGS_TABLE_START]        = sText_PkmnsXMadeYIneffective,
    [STRINGID_PKMNSXCUREDYPROBLEM - BATTLESTRINGS_TABLE_START]           = sText_PkmnsXCuredYProblem,
    [STRINGID_ITSUCKEDLIQUIDOOZE - BATTLESTRINGS_TABLE_START]            = sText_ItSuckedLiquidOoze,
    [STRINGID_PKMNTRANSFORMED - BATTLESTRINGS_TABLE_START]               = sText_PkmnTransformed,
    [STRINGID_ELECTRICITYWEAKENED - BATTLESTRINGS_TABLE_START]           = sText_ElectricityWeakened,
    [STRINGID_FIREWEAKENED - BATTLESTRINGS_TABLE_START]                  = sText_FireWeakened,
    [STRINGID_PKMNHIDUNDERWATER - BATTLESTRINGS_TABLE_START]             = sText_PkmnHidUnderwater,
    [STRINGID_PKMNSPRANGUP - BATTLESTRINGS_TABLE_START]                  = sText_PkmnSprangUp,
    [STRINGID_HMMOVESCANTBEFORGOTTEN - BATTLESTRINGS_TABLE_START]        = sText_HMMovesCantBeForgotten,
    [STRINGID_XFOUNDONEY - BATTLESTRINGS_TABLE_START]                    = sText_XFoundOneY,
    [STRINGID_PLAYERDEFEATEDTRAINER1 - BATTLESTRINGS_TABLE_START]        = sText_PlayerDefeatedLinkTrainerTrainer1,
    [STRINGID_SOOTHINGAROMA - BATTLESTRINGS_TABLE_START]                 = sText_SoothingAroma,
    [STRINGID_ITEMSCANTBEUSEDNOW - BATTLESTRINGS_TABLE_START]            = sText_ItemsCantBeUsedNow,
    [STRINGID_FORXCOMMAYZ - BATTLESTRINGS_TABLE_START]                   = sText_ForXCommaYZ,
    [STRINGID_USINGITEMSTATOFPKMNROSE - BATTLESTRINGS_TABLE_START]       = sText_UsingItemTheStatOfPkmnRose,
    [STRINGID_PKMNUSEDXTOGETPUMPED - BATTLESTRINGS_TABLE_START]          = sText_PkmnUsedXToGetPumped,
    [STRINGID_PKMNSXMADEYUSELESS - BATTLESTRINGS_TABLE_START]            = sText_PkmnsXMadeYUseless,
    [STRINGID_PKMNTRAPPEDBYSANDTOMB - BATTLESTRINGS_TABLE_START]         = sText_PkmnTrappedBySandTomb,
    [STRINGID_EMPTYSTRING4 - BATTLESTRINGS_TABLE_START]                  = sText_EmptyString4,
    [STRINGID_ABOOSTED - BATTLESTRINGS_TABLE_START]                      = sText_ABoosted,
    [STRINGID_PKMNSXINTENSIFIEDSUN - BATTLESTRINGS_TABLE_START]          = sText_PkmnsXIntensifiedSun,
    [STRINGID_PKMNMAKESGROUNDMISS - BATTLESTRINGS_TABLE_START]           = sText_PkmnMakesGroundMiss,
    [STRINGID_YOUTHROWABALLNOWRIGHT - BATTLESTRINGS_TABLE_START]         = sText_YouThrowABallNowRight,
    [STRINGID_PKMNSXTOOKATTACK - BATTLESTRINGS_TABLE_START]              = sText_PkmnsXTookAttack,
    [STRINGID_PKMNCHOSEXASDESTINY - BATTLESTRINGS_TABLE_START]           = sText_PkmnChoseXAsDestiny,
    [STRINGID_PKMNLOSTFOCUS - BATTLESTRINGS_TABLE_START]                 = sText_PkmnLostFocus,
    [STRINGID_USENEXTPKMN - BATTLESTRINGS_TABLE_START]                   = sText_UseNextPkmn,
    [STRINGID_PKMNFLEDUSINGITS - BATTLESTRINGS_TABLE_START]              = sText_PkmnFledUsingIts,
    [STRINGID_PKMNFLEDUSING - BATTLESTRINGS_TABLE_START]                 = sText_PkmnFledUsing,
    [STRINGID_PKMNWASDRAGGEDOUT - BATTLESTRINGS_TABLE_START]             = sText_PkmnWasDraggedOut,
    [STRINGID_PREVENTEDFROMWORKING - BATTLESTRINGS_TABLE_START]          = sText_PreventedFromWorking,
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS - BATTLESTRINGS_TABLE_START]     = sText_PkmnsItemNormalizedStatus,
    [STRINGID_TRAINER1USEDITEM - BATTLESTRINGS_TABLE_START]              = sText_Trainer1UsedItem,
    [STRINGID_BOXISFULL - BATTLESTRINGS_TABLE_START]                     = sText_BoxIsFull,
    [STRINGID_PKMNAVOIDEDATTACK - BATTLESTRINGS_TABLE_START]             = sText_PkmnAvoidedAttack,
    [STRINGID_PKMNSXMADEITINEFFECTIVE - BATTLESTRINGS_TABLE_START]       = sText_PkmnsXMadeItIneffective,
    [STRINGID_PKMNSXPREVENTSFLINCHING - BATTLESTRINGS_TABLE_START]       = sText_PkmnsXPreventsFlinching,
    [STRINGID_PKMNALREADYHASBURN - BATTLESTRINGS_TABLE_START]            = sText_PkmnAlreadyHasBurn,
    [STRINGID_STATSWONTDECREASE2 - BATTLESTRINGS_TABLE_START]            = sText_StatsWontDecrease2,
    [STRINGID_PKMNSXBLOCKSY2 - BATTLESTRINGS_TABLE_START]                = sText_PkmnsXBlocksY2,
    [STRINGID_PKMNSXWOREOFF - BATTLESTRINGS_TABLE_START]                 = sText_PkmnsXWoreOff,
    [STRINGID_PKMNRAISEDDEFALITTLE - BATTLESTRINGS_TABLE_START]          = sText_PkmnRaisedDefALittle,
    [STRINGID_PKMNRAISEDSPDEFALITTLE - BATTLESTRINGS_TABLE_START]        = sText_PkmnRaisedSpDefALittle,
    [STRINGID_THEWALLSHATTERED - BATTLESTRINGS_TABLE_START]              = sText_TheWallShattered,
    [STRINGID_PKMNSXPREVENTSYSZ - BATTLESTRINGS_TABLE_START]             = sText_PkmnsXPreventsYsZ,
    [STRINGID_PKMNSXCUREDITSYPROBLEM - BATTLESTRINGS_TABLE_START]        = sText_PkmnsXCuredItsYProblem,
    [STRINGID_ATTACKERCANTESCAPE - BATTLESTRINGS_TABLE_START]            = sText_AttackerCantEscape,
    [STRINGID_PKMNOBTAINEDX - BATTLESTRINGS_TABLE_START]                 = sText_PkmnObtainedX,
    [STRINGID_PKMNOBTAINEDX2 - BATTLESTRINGS_TABLE_START]                = sText_PkmnObtainedX2,
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ - BATTLESTRINGS_TABLE_START]       = sText_PkmnObtainedXYObtainedZ,
    [STRINGID_BUTNOEFFECT - BATTLESTRINGS_TABLE_START]                   = sText_ButNoEffect,
    [STRINGID_PKMNSXHADNOEFFECTONY - BATTLESTRINGS_TABLE_START]          = sText_PkmnsXHadNoEffectOnY,
    [STRINGID_OAKPLAYERWON - BATTLESTRINGS_TABLE_START]                  = gText_WinEarnsPrizeMoney,
    [STRINGID_OAKPLAYERLOST - BATTLESTRINGS_TABLE_START]                 = gText_HowDissapointing,
    [STRINGID_PLAYERLOSTAGAINSTENEMYTRAINER - BATTLESTRINGS_TABLE_START] = sText_PlayerWhiteoutAgainstTrainer,
    [STRINGID_PLAYERPAIDPRIZEMONEY - BATTLESTRINGS_TABLE_START]          = sText_PlayerPaidAsPrizeMoney,
    [STRINGID_PKMNTRANSFERREDSOMEONESPC - BATTLESTRINGS_TABLE_START]     = Text_MonSentToBoxInSomeonesPC,
    [STRINGID_PKMNTRANSFERREDBILLSPC - BATTLESTRINGS_TABLE_START]        = Text_MonSentToBoxInBillsPC,
    [STRINGID_PKMNBOXSOMEONESPCFULL - BATTLESTRINGS_TABLE_START]         = Text_MonSentToBoxSomeonesBoxFull,
    [STRINGID_PKMNBOXBILLSPCFULL - BATTLESTRINGS_TABLE_START]            = Text_MonSentToBoxBillsBoxFull,
    [STRINGID_POKEDUDEUSED - BATTLESTRINGS_TABLE_START]                  = sText_PokedudeUsedItem,
    [STRINGID_POKEFLUTECATCHY - BATTLESTRINGS_TABLE_START]               = sText_PlayedFluteCatchyTune,
    [STRINGID_POKEFLUTE - BATTLESTRINGS_TABLE_START]                     = sText_PlayedThe,
    [STRINGID_MONHEARINGFLUTEAWOKE - BATTLESTRINGS_TABLE_START]          = sText_PkmnHearingFluteAwoke,
    [STRINGID_TRAINER2CLASS - BATTLESTRINGS_TABLE_START]                 = sText_Trainer2Class,
    [STRINGID_TRAINER2NAME - BATTLESTRINGS_TABLE_START]                  = sText_Trainer2Name,
    [STRINGID_PLAYERWHITEDOUT - BATTLESTRINGS_TABLE_START]               = sText_PlayerWhiteout2,
    [STRINGID_MONTOOSCAREDTOMOVE - BATTLESTRINGS_TABLE_START]            = sText_TooScaredToMove,
    [STRINGID_GHOSTGETOUTGETOUT - BATTLESTRINGS_TABLE_START]             = sText_GetOutGetOut,
    [STRINGID_SILPHSCOPEUNVEILED - BATTLESTRINGS_TABLE_START]            = sText_SilphScopeUnveil,
    [STRINGID_GHOSTWASMAROWAK - BATTLESTRINGS_TABLE_START]               = sText_TheGhostWas,
    [STRINGID_TRAINER1MON1COMEBACK - BATTLESTRINGS_TABLE_START]          = sText_Trainer1RecallPkmn1,
    [STRINGID_TRAINER1WINTEXT - BATTLESTRINGS_TABLE_START]               = sText_Trainer1WinText,
    [STRINGID_TRAINER1MON2COMEBACK - BATTLESTRINGS_TABLE_START]          = sText_Trainer1RecallPkmn2,
    [STRINGID_TRAINER1MON1AND2COMEBACK - BATTLESTRINGS_TABLE_START]      = sText_Trainer1RecallBoth
};

const u16 gMissStringIds[] = {
    STRINGID_ATTACKMISSED,
    STRINGID_PKMNPROTECTEDITSELF,
    STRINGID_PKMNAVOIDEDATTACK,
    STRINGID_AVOIDEDDAMAGE,
    STRINGID_PKMNMAKESGROUNDMISS
};

const u16 gNoEscapeStringIds[] = {
    STRINGID_CANTESCAPE,
    STRINGID_DONTLEAVEBIRCH,
    STRINGID_PREVENTSESCAPE,
    STRINGID_CANTESCAPE2,
    STRINGID_ATTACKERCANTESCAPE
};

const u16 gMoveWeatherChangeStringIds[] = {
    STRINGID_STARTEDTORAIN,
    STRINGID_DOWNPOURSTARTED,
    STRINGID_BUTITFAILED,
    STRINGID_SANDSTORMBREWED,
    STRINGID_SUNLIGHTGOTBRIGHT,
    STRINGID_STARTEDHAIL
};

const u16 gSandstormHailContinuesStringIds[] = {
    STRINGID_SANDSTORMRAGES,
    STRINGID_HAILCONTINUES
};

const u16 gSandstormHailDmgStringIds[] = {
    STRINGID_PKMNBUFFETEDBYSANDSTORM,
    STRINGID_PKMNPELTEDBYHAIL
};

const u16 gSandstormHailEndStringIds[] = {
    STRINGID_SANDSTORMSUBSIDED,
    STRINGID_HAILSTOPPED
};

const u16 gRainContinuesStringIds[] = {
    STRINGID_RAINCONTINUES,
    STRINGID_DOWNPOURCONTINUES,
    STRINGID_RAINSTOPPED
};

const u16 gProtectLikeUsedStringIds[] = {
    STRINGID_PKMNPROTECTEDITSELF2,
    STRINGID_PKMNBRACEDITSELF,
    STRINGID_BUTITFAILED
};

const u16 gReflectLightScreenSafeguardStringIds[] = {
    STRINGID_BUTITFAILED,
    STRINGID_PKMNRAISEDDEF,
    STRINGID_PKMNRAISEDDEFALITTLE,
    STRINGID_PKMNRAISEDSPDEF,
    STRINGID_PKMNRAISEDSPDEFALITTLE,
    STRINGID_PKMNCOVEREDBYVEIL
};

const u16 gLeechSeedStringIds[] = {
    STRINGID_PKMNSEEDED,
    STRINGID_PKMNEVADEDATTACK,
    STRINGID_ITDOESNTAFFECT,
    STRINGID_PKMNSAPPEDBYLEECHSEED,
    STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gRestUsedStringIds[] = {
    STRINGID_PKMNWENTTOSLEEP,
    STRINGID_PKMNSLEPTHEALTHY
};

const u16 gUproarOverTurnStringIds[] = {
    STRINGID_PKMNMAKINGUPROAR,
    STRINGID_PKMNCALMEDDOWN
};

const u16 gStockpileUsedStringIds[] = {
    STRINGID_PKMNSTOCKPILED,
    STRINGID_PKMNCANTSTOCKPILE
};

const u16 gWokeUpStringIds[] = {
    STRINGID_PKMNWOKEUP,
    STRINGID_PKMNWOKEUPINUPROAR
};

const u16 gSwallowFailStringIds[] = {
    STRINGID_FAILEDTOSWALLOW,
    STRINGID_PKMNHPFULL
};

const u16 gUproarAwakeStringIds[] = {
    STRINGID_PKMNCANTSLEEPINUPROAR2,
    STRINGID_UPROARKEPTPKMNAWAKE,
    STRINGID_PKMNSTAYEDAWAKEUSING
};

const u16 gStatUpStringIds[] =
{
    [B_MSG_ATTACKER_STAT_ROSE] = STRINGID_ATTACKERSSTATROSE,
    [B_MSG_DEFENDER_STAT_ROSE] = STRINGID_DEFENDERSSTATROSE,
    [B_MSG_STAT_WONT_INCREASE] = STRINGID_STATSWONTINCREASE,
    [B_MSG_STAT_ROSE_EMPTY]    = STRINGID_EMPTYSTRING3,
    [B_MSG_STAT_ROSE_ITEM]     = STRINGID_USINGITEMSTATOFPKMNROSE,
    [B_MSG_USED_DIRE_HIT]      = STRINGID_PKMNUSEDXTOGETPUMPED,
};

const u16 gStatDownStringIds[] = {
    STRINGID_ATTACKERSSTATFELL,
    STRINGID_DEFENDERSSTATFELL,
    STRINGID_STATSWONTDECREASE,
    STRINGID_EMPTYSTRING3
};

const u16 gFirstTurnOfTwoStringIds[] = {
    STRINGID_PKMNWHIPPEDWHIRLWIND,
    STRINGID_PKMNTOOKSUNLIGHT,
    STRINGID_PKMNLOWEREDHEAD,
    STRINGID_PKMNISGLOWING,
    STRINGID_PKMNFLEWHIGH,
    STRINGID_PKMNDUGHOLE,
    STRINGID_PKMNHIDUNDERWATER,
    STRINGID_PKMNSPRANGUP
};

const u16 gWrappedStringIds[] = {
    STRINGID_PKMNSQUEEZEDBYBIND,
    STRINGID_PKMNWRAPPEDBY,
    STRINGID_PKMNTRAPPEDINVORTEX,
    STRINGID_PKMNCLAMPED,
    STRINGID_PKMNTRAPPEDINVORTEX,
    STRINGID_PKMNTRAPPEDBYSANDTOMB
};

const u16 gMistUsedStringIds[] = {
    STRINGID_PKMNSHROUDEDINMIST,
    STRINGID_BUTITFAILED
};

const u16 gFocusEnergyUsedStringIds[] = {
    STRINGID_PKMNGETTINGPUMPED,
    STRINGID_BUTITFAILED
};

const u16 gTransformUsedStringIds[] = {
    STRINGID_PKMNTRANSFORMEDINTO,
    STRINGID_BUTITFAILED
};

const u16 gSubstituteUsedStringIds[] = {
    STRINGID_PKMNMADESUBSTITUTE,
    STRINGID_TOOWEAKFORSUBSTITUTE
};

const u16 gGotPoisonedStringIds[] = {
    STRINGID_PKMNWASPOISONED,
    STRINGID_PKMNPOISONEDBY
};

const u16 gGotParalyzedStringIds[] = {
    STRINGID_PKMNWASPARALYZED,
    STRINGID_PKMNWASPARALYZEDBY
};

const u16 gFellAsleepStringIds[] = {
    STRINGID_PKMNFELLASLEEP,
    STRINGID_PKMNMADESLEEP
};

const u16 gGotBurnedStringIds[] = {
    STRINGID_PKMNWASBURNED,
    STRINGID_PKMNBURNEDBY
};

const u16 gGotFrozenStringIds[] = {
    STRINGID_PKMNWASFROZEN,
    STRINGID_PKMNFROZENBY
};

const u16 gGotDefrostedStringIds[] = {
    STRINGID_PKMNWASDEFROSTED2,
    STRINGID_PKMNWASDEFROSTEDBY
};

const u16 gKOFailedStringIds[] = {
    STRINGID_ATTACKMISSED,
    STRINGID_PKMNUNAFFECTED
};

const u16 gAttractUsedStringIds[] = {
    STRINGID_PKMNFELLINLOVE,
    STRINGID_PKMNSXINFATUATEDY
};

const u16 gAbsorbDrainStringIds[] = {
    STRINGID_PKMNENERGYDRAINED,
    STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gSportsUsedStringIds[] = {
    STRINGID_ELECTRICITYWEAKENED,
    STRINGID_FIREWEAKENED
};

const u16 gPartyStatusHealStringIds[] = {
    STRINGID_BELLCHIMED,
    STRINGID_BELLCHIMED,
    STRINGID_BELLCHIMED,
    STRINGID_BELLCHIMED,
    STRINGID_SOOTHINGAROMA
};

const u16 gFutureMoveUsedStringIds[] = {
    STRINGID_PKMNFORESAWATTACK,
    STRINGID_PKMNCHOSEXASDESTINY
};

const u16 gBallEscapeStringIds[] = {
    STRINGID_PKMNBROKEFREE,
    STRINGID_ITAPPEAREDCAUGHT,
    STRINGID_AARGHALMOSTHADIT,
    STRINGID_SHOOTSOCLOSE
};

const u16 gWeatherStartsStringIds[] = {
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING,
    STRINGID_SANDSTORMISRAGING,
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING,
    STRINGID_SUNLIGHTSTRONG,
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING,
    STRINGID_ITISRAINING
};

const u16 gInobedientStringIds[] = {
    STRINGID_PKMNLOAFING,
    STRINGID_PKMNWONTOBEY,
    STRINGID_PKMNTURNEDAWAY,
    STRINGID_PKMNPRETENDNOTNOTICE
};

const u16 gSafariPokeblockResultStringIds[] = {
    STRINGID_PKMNWATCHINGCAREFULLY,
    STRINGID_PKMNANGRY,
    STRINGID_PKMNEATING
};

const u16 gTrainerItemCuredStatusStringIds[] = {
    STRINGID_PKMNSITEMSNAPPEDOUT,
    STRINGID_PKMNSITEMCUREDPARALYSIS,
    STRINGID_PKMNSITEMDEFROSTEDIT,
    STRINGID_PKMNSITEMHEALEDBURN,
    STRINGID_PKMNSITEMCUREDPOISON,
    STRINGID_PKMNSITEMWOKEIT
};

const u16 gBerryEffectStringIds[] = {
    STRINGID_PKMNSITEMCUREDPROBLEM,
    STRINGID_PKMNSITEMNORMALIZEDSTATUS
};

const u16 gBRNPreventionStringIds[] = {
    STRINGID_PKMNSXPREVENTSBURNS,
    STRINGID_PKMNSXPREVENTSYSZ,
    STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gPRLZPreventionStringIds[] = {
    STRINGID_PKMNPREVENTSPARALYSISWITH,
    STRINGID_PKMNSXPREVENTSYSZ,
    STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gPSNPreventionStringIds[] = {
    STRINGID_PKMNPREVENTSPOISONINGWITH,
    STRINGID_PKMNSXPREVENTSYSZ,
    STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gItemSwapStringIds[] = {
    STRINGID_PKMNOBTAINEDX,
    STRINGID_PKMNOBTAINEDX2,
    STRINGID_PKMNOBTAINEDXYOBTAINEDZ
};

const u16 gFlashFireStringIds[] = {
    STRINGID_PKMNRAISEDFIREPOWERWITH,
    STRINGID_PKMNSXMADEYINEFFECTIVE
};

const u16 gCaughtMonStringIds[] = {
    STRINGID_PKMNTRANSFERREDSOMEONESPC,
    STRINGID_PKMNTRANSFERREDBILLSPC,
    STRINGID_PKMNBOXSOMEONESPCFULL,
    STRINGID_PKMNBOXBILLSPCFULL
};

const u16 gDoubleBattleRecallStrings[] = {
    STRINGID_TRAINER1MON1COMEBACK,
    STRINGID_TRAINER1MON1COMEBACK,
    STRINGID_TRAINER1MON2COMEBACK,
    STRINGID_TRAINER1MON1AND2COMEBACK
};

const u16 gTrappingMoves[] = {
    MOVE_BIND,
    MOVE_WRAP,
    MOVE_FIRE_SPIN,
    MOVE_CLAMP,
    MOVE_WHIRLPOOL,
    MOVE_SAND_TOMB,
    0xFFFF
};

const u8 gText_PkmnIsEvolving[] = _("¿Qué?\n¡{STR_VAR_1} esta evolucionando!");
const u8 gText_CongratsPkmnEvolved[] = _("¡Felicidades! tu {STR_VAR_1}\nevoluciono en {STR_VAR_2}!{WAIT_SE}\p");
const u8 gText_PkmnStoppedEvolving[] = _("¿Uh? ¡{STR_VAR_1}\ndejó de evolucionar!\p");
const u8 gText_EllipsisQuestionMark[] = _("¿……?\p");
const u8 gText_WhatWillPkmnDo[] = _("¿Qué hará\n{B_ACTIVE_NAME_WITH_PREFIX}?");
const u8 gText_WhatWillPlayerThrow[] = _("¿Qué lanzará\n{B_PLAYER_NAME}?");
const u8 gText_WhatWillOldManDo[] = _("¿Qué hará el\nseñor mayor?");
const u8 gText_LinkStandby[] = _("{PAUSE 16}Link standby…");
const u8 gText_BattleMenu[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}LUCHAR{CLEAR_TO 56}MOCHI.\nPOKéMON{CLEAR_TO 56}CORRER");
const u8 gUnknown_83FE747[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}BOLA{CLEAR_TO 56}CEBO\nROCA{CLEAR_TO 56}CORRER");
const u8 gText_MoveInterfacePP[] = _("PP ");
const u8 gText_MoveInterfaceType[] = _("TIPO/");
const u8 gUnknown_83FE770[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
const u8 gUnknown_83FE779[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}どの わざを\nわすれさせたい?");
const u8 gText_BattleYesNoChoice[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Sí\nNo");
const u8 gText_BattleSwitchWhich[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Switch\nwhich?");
const u8 gUnknown_83FE7B6[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
const u8 gUnknown_83FE7BF[] = _("{RIGHT_ARROW_2}");
const u8 gUnknown_83FE7C2[] = _("{PLUS}");
const u8 gUnknown_83FE7C5[] = _("-");

const u8 gUnknown_83FE7C7[] = _("{FONT_0}PS{FONT_2} Máx.");
const u8 gUnknown_83FE7D4[] = _("ATAQUE ");
const u8 gUnknown_83FE7DC[] = _("DEFENSA");
const u8 gUnknown_83FE7E4[] = _("ATQ. ESP.");
const u8 gUnknown_83FE7EC[] = _("DEF. ESP");

const u8 *const gUnknown_83FE7F4[] = {
    gUnknown_83FE7C7,
    gUnknown_83FE7E4,
    gUnknown_83FE7D4,
    gUnknown_83FE7EC,
    gUnknown_83FE7DC,
    sText_Speed
};

const u8 gText_SafariBalls[] = _("{HIGHLIGHT 2}SAFARI BALLS"); //
const u8 gText_HighlightRed_Left[] = _("{HIGHLIGHT 2}Left: ");
const u8 gText_HighlightRed[] = _("{HIGHLIGHT 2}");
const u8 gText_Sleep[] = _("sleep");
const u8 gText_Poison[] = _("poison");
const u8 gText_Burn[] = _("burn");
const u8 gText_Paralysis[] = _("paralysis");
const u8 gText_Ice[] = _("ice");
const u8 gText_Confusion[] = _("confusion");
const u8 gText_Love[] = _("love");
const u8 gUnknown_83FE859[] = _("  ");
const u8 gUnknown_83FE85C[] = _("\n");
const u8 gUnknown_83FE85E[] = _("\n");
const u8 gUnknown_83FE860[] = _(" is");
const u8 gUnknown_83FE864[] = _(" is");
const u8 gText_BadEgg[] = _("Bad EGG");
const u8 gUnknown_83FE870[] = _("ミツル");
const u8 gText_Win[] = _("{HIGHLIGHT 0}Win");
const u8 gText_Loss[] = _("{HIGHLIGHT 0}Loss");
const u8 gText_Draw[] = _("{HIGHLIGHT 0}Draw");
static const u8 sText_SpaceIs[] = _(" is");
static const u8 sText_ApostropheS[] = _("'s");
const u8 gUnknown_83FE892[] = _("a NORMAL move");
const u8 gUnknown_83FE8A0[] = _("a FIGHTING move");
const u8 gUnknown_83FE8B0[] = _("a FLYING move");
const u8 gUnknown_83FE8BE[] = _("a POISON move");
const u8 gUnknown_83FE8CC[] = _("a GROUND move");
const u8 gUnknown_83FE8DA[] = _("a ROCK move");
const u8 gUnknown_83FE8E6[] = _("a BUG move");
const u8 gUnknown_83FE8F1[] = _("a GHOST move");
const u8 gUnknown_83FE8FE[] = _("a STEEL move");
const u8 gUnknown_83FE90B[] = _("a ??? move");
const u8 gUnknown_83FE916[] = _("a FIRE move");
const u8 gUnknown_83FE922[] = _("a WATER move");
const u8 gUnknown_83FE92F[] = _("a GRASS move");
const u8 gUnknown_83FE93C[] = _("an ELECTRIC move");
const u8 gUnknown_83FE94D[] = _("a PSYCHIC move");
const u8 gUnknown_83FE95C[] = _("an ICE move");
const u8 gUnknown_83FE968[] = _("a DRAGON move");
const u8 gUnknown_83FE976[] = _("a DARK move");
const u8 gText_TimeBoard[] = _("TIME BOARD");
const u8 gText_ClearTime[] = _("CLEAR TIME"); // Unused
const u8 gText_XMinYZSec[] = _("{STR_VAR_1}MIN. {STR_VAR_2}.{STR_VAR_3}SEC.");
const u8 gUnknown_83FE9A9[] = _("P1");
const u8 gUnknown_83FE9AC[] = _("P2");
const u8 gUnknown_83FE9AF[] = _("P3");
const u8 gUnknown_83FE9B2[] = _("P4");
const u8 gUnknown_83FE9B5[] = _("P5");
const u8 gUnknown_83FE9B8[] = _("P6");
const u8 gUnknown_83FE9BB[] = _("P7");
const u8 gUnknown_83FE9BE[] = _("P8");

const u8 *const gTrainerTowerChallengeTypeTexts[NUM_TOWER_CHALLENGE_TYPES] = {
    gOtherText_Single,
    gOtherText_Double,
    gOtherText_Knockout,
    gOtherText_Mixed
};

const u8 gUnknown_83FE9D4[] = _("{PLAY_SE SE_FLEE}¡{B_TRAINER1_CLASS} {B_TRAINER1_NAME} huyó!"); //
static const u8 sText_PlayerLostAgainstTrainer1[] = _("¡Perdiste contra\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
static const u8 sText_PlayerBattledToDrawTrainer1[] = _("¡Empataste contra\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");

static const u8 *const sATypeMove_Table[] = {
    gUnknown_83FE892,
    gUnknown_83FE8A0,
    gUnknown_83FE8B0,
    gUnknown_83FE8BE,
    gUnknown_83FE8CC,
    gUnknown_83FE8DA,
    gUnknown_83FE8E6,
    gUnknown_83FE8F1,
    gUnknown_83FE8FE,
    gUnknown_83FE90B,
    gUnknown_83FE916,
    gUnknown_83FE922,
    gUnknown_83FE92F,
    gUnknown_83FE93C,
    gUnknown_83FE94D,
    gUnknown_83FE95C,
    gUnknown_83FE968,
    gUnknown_83FE976
};

static const u16 sGrammarMoveUsedTable[] = {
    MOVE_SWORDS_DANCE,
    MOVE_STRENGTH,
    MOVE_GROWTH,
    MOVE_HARDEN,
    MOVE_MINIMIZE,
    MOVE_SMOKESCREEN,
    MOVE_WITHDRAW,
    MOVE_DEFENSE_CURL,
    MOVE_EGG_BOMB,
    MOVE_SMOG,
    MOVE_BONE_CLUB,
    MOVE_FLASH,
    MOVE_SPLASH,
    MOVE_ACID_ARMOR,
    MOVE_BONEMERANG,
    MOVE_REST,
    MOVE_SHARPEN,
    MOVE_SUBSTITUTE,
    MOVE_MIND_READER,
    MOVE_SNORE,
    MOVE_PROTECT,
    MOVE_SPIKES,
    MOVE_ENDURE,
    MOVE_ROLLOUT,
    MOVE_SWAGGER,
    MOVE_SLEEP_TALK,
    MOVE_HIDDEN_POWER,
    MOVE_PSYCH_UP,
    MOVE_EXTREME_SPEED,
    MOVE_FOLLOW_ME,
    MOVE_TRICK,
    MOVE_ASSIST,
    MOVE_INGRAIN,
    MOVE_KNOCK_OFF,
    MOVE_CAMOUFLAGE,
    MOVE_ASTONISH,
    MOVE_ODOR_SLEUTH,
    MOVE_GRASS_WHISTLE,
    MOVE_SHEER_COLD,
    MOVE_MUDDY_WATER,
    MOVE_IRON_DEFENSE,
    MOVE_BOUNCE,
    MOVE_NONE,

    MOVE_TELEPORT,
    MOVE_RECOVER,
    MOVE_BIDE,
    MOVE_AMNESIA,
    MOVE_FLAIL,
    MOVE_TAUNT,
    MOVE_BULK_UP,
    MOVE_NONE,

    MOVE_MEDITATE,
    MOVE_AGILITY,
    MOVE_MIMIC,
    MOVE_DOUBLE_TEAM,
    MOVE_BARRAGE,
    MOVE_TRANSFORM,
    MOVE_STRUGGLE,
    MOVE_SCARY_FACE,
    MOVE_CHARGE,
    MOVE_WISH,
    MOVE_BRICK_BREAK,
    MOVE_YAWN,
    MOVE_FEATHER_DANCE,
    MOVE_TEETER_DANCE,
    MOVE_MUD_SPORT,
    MOVE_FAKE_TEARS,
    MOVE_WATER_SPORT,
    MOVE_CALM_MIND,
    MOVE_NONE,

    MOVE_POUND,
    MOVE_SCRATCH,
    MOVE_VICE_GRIP,
    MOVE_WING_ATTACK,
    MOVE_FLY,
    MOVE_BIND,
    MOVE_SLAM,
    MOVE_HORN_ATTACK,
    MOVE_WRAP,
    MOVE_THRASH,
    MOVE_TAIL_WHIP,
    MOVE_LEER,
    MOVE_BITE,
    MOVE_GROWL,
    MOVE_ROAR,
    MOVE_SING,
    MOVE_PECK,
    MOVE_ABSORB,
    MOVE_STRING_SHOT,
    MOVE_EARTHQUAKE,
    MOVE_FISSURE,
    MOVE_DIG,
    MOVE_TOXIC,
    MOVE_SCREECH,
    MOVE_METRONOME,
    MOVE_LICK,
    MOVE_CLAMP,
    MOVE_CONSTRICT,
    MOVE_POISON_GAS,
    MOVE_BUBBLE,
    MOVE_SLASH,
    MOVE_SPIDER_WEB,
    MOVE_NIGHTMARE,
    MOVE_CURSE,
    MOVE_FORESIGHT,
    MOVE_CHARM,
    MOVE_ATTRACT,
    MOVE_ROCK_SMASH,
    MOVE_UPROAR,
    MOVE_SPIT_UP,
    MOVE_SWALLOW,
    MOVE_TORMENT,
    MOVE_FLATTER,
    MOVE_ROLE_PLAY,
    MOVE_ENDEAVOR,
    MOVE_TICKLE,
    MOVE_COVET,
    MOVE_NONE
};

// code
void BufferStringBattle(u16 stringId)
{
    s32 i;
    const u8 *stringPtr = NULL;

    sBattleMsgDataPtr = (struct BattleMsgData *)(&gBattleBufferA[gActiveBattler][4]);
    gLastUsedItem = sBattleMsgDataPtr->lastItem;
    gLastUsedAbility = sBattleMsgDataPtr->lastAbility;
    gBattleScripting.battler = sBattleMsgDataPtr->scrActive;
    *(&gBattleStruct->scriptPartyIdx) = sBattleMsgDataPtr->bakScriptPartyIdx;
    *(&gBattleStruct->hpScale) = sBattleMsgDataPtr->hpScale;
    gPotentialItemEffectBattler = sBattleMsgDataPtr->itemEffectBattler;
    *(&gBattleStruct->stringMoveType) = sBattleMsgDataPtr->moveType;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        sBattlerAbilities[i] = sBattleMsgDataPtr->abilities[i];
    }
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
    {
        gBattleTextBuff1[i] = sBattleMsgDataPtr->textBuffs[0][i];
        gBattleTextBuff2[i] = sBattleMsgDataPtr->textBuffs[1][i];
        gBattleTextBuff3[i] = sBattleMsgDataPtr->textBuffs[2][i];
    }

    switch (stringId)
    {
    case STRINGID_INTROMSG: // first battle msg
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                {
                    stringPtr = sText_TwoLinkTrainersWantToBattle;
                }
                else
                {
                    if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                        stringPtr = sText_Trainer1WantsToBattle;
                    else
                        stringPtr = sText_LinkTrainerWantsToBattle;
                }
            }
            else
            {
                stringPtr = sText_Trainer1WantsToBattle;
            }
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_GHOST_UNVEILED)
                    stringPtr = sText_TheGhostAppeared;
                else
                    stringPtr = sText_GhostAppearedCantId;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY)
                stringPtr = sText_WildPkmnAppeared2;
            else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) // interesting, looks like they had something planned for wild double battles
                stringPtr = sText_TwoWildPkmnAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
                stringPtr = sText_WildPkmnAppearedPause;
            else
                stringPtr = sText_WildPkmnAppeared;
        }
        break;
    case STRINGID_INTROSENDOUT: // poke first send-out
        if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkPartnerSentOutPkmnGoPkmn;
                else
                    stringPtr = sText_GoTwoPkmn;
            }
            else
            {
                stringPtr = sText_GoPkmn;
            }
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_TwoLinkTrainersSentOutPkmn;
                else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
            }
            else
            {
                if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
                    stringPtr = sText_Trainer1SentOutPkmn;
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                    stringPtr = sText_Trainer1SentOutPkmn;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn;
            }
        }
        break;
    case STRINGID_RETURNMON: // sending poke to ball msg
        if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        {
            if (*(&gBattleStruct->hpScale) == 0)
                stringPtr = sText_PkmnThatsEnough;
            else if (*(&gBattleStruct->hpScale) == 1 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = sText_PkmnComeBack;
            else if (*(&gBattleStruct->hpScale) == 2)
                stringPtr = sText_PkmnOkComeBack;
            else
                stringPtr = sText_PkmnGoodComeBack;
        }
        else
        {
            if (gTrainerBattleOpponent_A == TRAINER_LINK_OPPONENT)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkTrainer2WithdrewPkmn;
                else
                    stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else
            {
                stringPtr = sText_Trainer1WithdrewPkmn;
            }
        }
        break;
    case STRINGID_SWITCHINMON: // switch-in msg
        if (GetBattlerSide(gBattleScripting.battler) == B_SIDE_PLAYER)
        {
            if (*(&gBattleStruct->hpScale) == 0 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = sText_GoPkmn2;
            else if (*(&gBattleStruct->hpScale) == 1)
                stringPtr = sText_DoItPkmn;
            else if (*(&gBattleStruct->hpScale) == 2)
                stringPtr = sText_GoForItPkmn;
            else
                stringPtr = sText_YourFoesWeakGetEmPkmn;
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkTrainerMultiSentOutPkmn;
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                    stringPtr = sText_Trainer1SentOutPkmn2;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn2;
            }
            else
            {
                stringPtr = sText_Trainer1SentOutPkmn2;
            }
        }
        break;
    case STRINGID_USEDMOVE: // pokemon used a move msg
        ChooseMoveUsedParticle(gBattleTextBuff1); // buff1 doesn't appear in the string, leftover from japanese move names

        if (sBattleMsgDataPtr->currentMove >= MOVES_COUNT)
            StringCopy(gBattleTextBuff2, sATypeMove_Table[*(&gBattleStruct->stringMoveType)]);
        else
            StringCopy(gBattleTextBuff2, gLongMoveNames[sBattleMsgDataPtr->currentMove]);

        ChooseTypeOfMoveUsedString(gBattleTextBuff2);
        stringPtr = sText_AttackerUsedX;
        break;
    case STRINGID_BATTLEEND: // battle end
        if (gBattleTextBuff1[0] & B_OUTCOME_LINK_BATTLE_RAN)
        {
            gBattleTextBuff1[0] &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTextBuff1[0] == B_OUTCOME_LOST || gBattleTextBuff1[0] == B_OUTCOME_DREW)
                stringPtr = sText_GotAwaySafely;
            else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                stringPtr = sText_TwoWildFled;
            else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                stringPtr = gUnknown_83FE9D4;
            else
                stringPtr = sText_WildFled;
        }
        else
        {
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_TwoLinkTrainersDefeated;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostToTwo;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawVsTwo;
                    break;
                }
            }
            else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainerTrainer1;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstTrainer1;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawTrainer1;
                    break;
                }
            }
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainer;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstLinkTrainer;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawLinkTrainer;
                    break;
                }
            }
        }
        break;
    default: // load a string from the table
        if (stringId >= BATTLESTRINGS_COUNT + BATTLESTRINGS_TABLE_START)
        {
            gDisplayedStringBattle[0] = EOS;
            return;
        }
        else
        {
            stringPtr = gBattleStringsTable[stringId - BATTLESTRINGS_TABLE_START];
        }
        break;
    }

    BattleStringExpandPlaceholdersToDisplayedString(stringPtr);
}

u32 BattleStringExpandPlaceholdersToDisplayedString(const u8 *src)
{
    BattleStringExpandPlaceholders(src, gDisplayedStringBattle);
}

static const u8 *TryGetStatusString(u8 *src)
{
    u32 i;
    u8 status[] = _("$$$$$$$");
    u32 chars1, chars2;
    u8 *statusPtr;

    statusPtr = status;
    for (i = 0; i < 8; i++)
    {
        if (*src == EOS)
            break;
        *statusPtr = *src;
        src++;
        statusPtr++;
    }

    chars1 = *(u32 *)(&status[0]);
    chars2 = *(u32 *)(&status[4]);

    for (i = 0; i < NELEMS(gStatusConditionStringsTable); i++)
    {
        if (chars1 == *(u32 *)(&gStatusConditionStringsTable[i][0][0])
            && chars2 == *(u32 *)(&gStatusConditionStringsTable[i][0][4]))
            return gStatusConditionStringsTable[i][1];
    }
    return NULL;
}

#define HANDLE_NICKNAME_STRING_CASE(battlerId, monIndex)                \
    if (GetBattlerSide(battlerId) != B_SIDE_PLAYER)                     \
    {                                                                   \
        GetMonData(&gEnemyParty[monIndex], MON_DATA_NICKNAME, text);    \
		StringGetEnd10(text);											\
		toCpy = text;													\
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstId] = *toCpy;                                        \
            dstId++;                                                    \
            toCpy++;                                                    \
        }																\
		 if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                    \
            toCpy = sText_FoePkmnPrefix;                                \
        else                                                            \
            toCpy = sText_WildPkmnPrefix;                               \
    }                                                                   \
    else                                                                \
    {                                                                   \
        GetMonData(&gPlayerParty[monIndex], MON_DATA_NICKNAME, text);   \
    StringGetEnd10(text);                                               \
	toCpy = text;														\
	}

u32 BattleStringExpandPlaceholders(const u8 *src, u8 *dst)
{
    u32 dstId = 0; // if they used dstId, why not use srcId as well?
    const u8 *toCpy = NULL;
    u8 text[30];
    u8 multiplayerId;
    s32 i;

    multiplayerId = GetMultiplayerId();

    while (*src != EOS)
    {
        if (*src == PLACEHOLDER_BEGIN)
        {
            src++;
            switch (*src)
            {
            case B_TXT_BUFF1:
                if (gBattleTextBuff1[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
                    toCpy = gStringVar1;
                }
                else
                {
                    toCpy = TryGetStatusString(gBattleTextBuff1);
                    if (toCpy == NULL)
                        toCpy = gBattleTextBuff1;
                }
                break;
            case B_TXT_BUFF2:
                if (gBattleTextBuff2[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
                    toCpy = gStringVar2;
                }
                else
                    toCpy = gBattleTextBuff2;
                break;
            case B_TXT_BUFF3:
                if (gBattleTextBuff3[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
                    toCpy = gStringVar3;
                }
                else
                    toCpy = gBattleTextBuff3;
                break;
            case B_TXT_COPY_VAR_1:
                toCpy = gStringVar1;
                break;
            case B_TXT_COPY_VAR_2:
                toCpy = gStringVar2;
                break;
            case B_TXT_COPY_VAR_3:
                toCpy = gStringVar3;
                break;
            case B_TXT_PLAYER_MON1_NAME: // first player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON1_NAME: // first enemy poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_PLAYER_MON2_NAME: // second player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON2_NAME: // second enemy poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON1_NAME: // link first player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON1_NAME: // link first opponent poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id ^ 1]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON2_NAME: // link second player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id ^ 2]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON2_NAME: // link second opponent poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id ^ 3]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX_MON1: // attacker name with prefix, only battlerId 0/1
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker,
                                            gBattlerPartyIndexes[GetBattlerAtPosition(GET_BATTLER_SIDE(gBattlerAttacker))])
                break;
            case B_TXT_ATK_PARTNER_NAME: // attacker partner name
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    GetMonData(
                        &gPlayerParty[gBattlerPartyIndexes[GetBattlerAtPosition(GET_BATTLER_SIDE(gBattlerAttacker)) +
                                                           2]], MON_DATA_NICKNAME, text);
                else
                    GetMonData(
                        &gEnemyParty[gBattlerPartyIndexes[GetBattlerAtPosition(GET_BATTLER_SIDE(gBattlerAttacker)) +
                                                          2]], MON_DATA_NICKNAME, text);

                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX: // attacker name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker])
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX: // target name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerTarget, gBattlerPartyIndexes[gBattlerTarget])
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX: // effect battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gEffectBattler, gBattlerPartyIndexes[gEffectBattler])
                break;
            case B_TXT_ACTIVE_NAME_WITH_PREFIX: // active battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gActiveBattler, gBattlerPartyIndexes[gActiveBattler])
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: // scripting active battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler, gBattlerPartyIndexes[gBattleScripting.battler])
                break;
            case B_TXT_CURRENT_MOVE: // current move name
                if (sBattleMsgDataPtr->currentMove >= MOVES_COUNT)
                    toCpy = (const u8 *)&sATypeMove_Table[gBattleStruct->stringMoveType];
                else
                    toCpy = gLongMoveNames[sBattleMsgDataPtr->currentMove];
                break;
            case B_TXT_LAST_MOVE: // originally used move name
                if (sBattleMsgDataPtr->originallyUsedMove >= MOVES_COUNT)
                    toCpy = (const u8 *)&sATypeMove_Table[gBattleStruct->stringMoveType];
                else
                    toCpy = gLongMoveNames[sBattleMsgDataPtr->originallyUsedMove];
                break;
            case B_TXT_LAST_ITEM: // last used item
                if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                {
                    if (gLastUsedItem == ITEM_ENIGMA_BERRY)
                    {
                        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                        {
                            if ((gBattleStruct->multiplayerId != 0 && (gPotentialItemEffectBattler & BIT_SIDE))
                                || (gBattleStruct->multiplayerId == 0 && !(gPotentialItemEffectBattler & BIT_SIDE)))
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                            {
                                toCpy = sText_EnigmaBerry;
                            }
                        }
                        else
                        {
                            if (gLinkPlayers[gBattleStruct->multiplayerId].id == gPotentialItemEffectBattler)
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                                toCpy = sText_EnigmaBerry;
                        }
                    }
                    else
                    {
                        CopyItemName(gLastUsedItem, text);
                        toCpy = text;
                    }
                }
                else
                {
                    CopyItemName(gLastUsedItem, text);
                    toCpy = text;
                }
                break;
            case B_TXT_LAST_ABILITY: // last used ability
                toCpy = gAbilityNames[gLastUsedAbility];
                break;
            case B_TXT_ATK_ABILITY: // attacker ability
                toCpy = gAbilityNames[sBattlerAbilities[gBattlerAttacker]];
                break;
            case B_TXT_DEF_ABILITY: // target ability
                toCpy = gAbilityNames[sBattlerAbilities[gBattlerTarget]];
                break;
            case B_TXT_SCR_ACTIVE_ABILITY: // scripting active ability
                toCpy = gAbilityNames[sBattlerAbilities[gBattleScripting.battler]];
                break;
            case B_TXT_EFF_ABILITY: // effect battlerId ability
                toCpy = gAbilityNames[sBattlerAbilities[gEffectBattler]];
                break;
            case B_TXT_TRAINER1_CLASS: // trainer class name
                if (gTrainerBattleOpponent_A == SECRET_BASE_OPPONENT)
                    toCpy = gTrainerClassNames[GetSecretBaseTrainerNameIndex()];
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                    toCpy = gTrainerClassNames[GetUnionRoomTrainerClass()];
                else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                    toCpy = gTrainerClassNames[GetBattleTowerTrainerClassNameId()];
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                    toCpy = gTrainerClassNames[GetTrainerTowerOpponentClass()];
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
                    toCpy = gTrainerClassNames[GetEreaderTrainerClassId()];
                else
                    toCpy = gTrainerClassNames[gTrainers[gTrainerBattleOpponent_A].trainerClass];
                break;
            case B_TXT_TRAINER1_NAME: // trainer1 name
                if (gTrainerBattleOpponent_A == SECRET_BASE_OPPONENT)
                {
                    for (i = 0; i < (s32)NELEMS(gBattleResources->secretBase->trainerName); i++)
                        text[i] = gBattleResources->secretBase->trainerName[i];
                    text[i] = EOS;
                    toCpy = text;
                }
                if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                {
                    toCpy = gLinkPlayers[multiplayerId ^ BIT_SIDE].name;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                {
                    GetBattleTowerTrainerName(text);
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentName(text);
                    toCpy = text;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
                {
                    CopyEReaderTrainerName5(text);
                    toCpy = text;
                }
                else
                {
                    if (gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_RIVAL_EARLY
                     || gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_RIVAL_LATE
                     || gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_CHAMPION)
                        toCpy = GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
                    else
                        toCpy = gTrainers[gTrainerBattleOpponent_A].trainerName;
                }
                break;
            case B_TXT_LINK_PLAYER_NAME: // link player name
                toCpy = gLinkPlayers[multiplayerId].name;
                break;
            case B_TXT_LINK_PARTNER_NAME: // link partner name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT1_NAME: // link opponent 1 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT2_NAME: // link opponent 2 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(
                    BATTLE_PARTNER(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id)))].name;
                break;
            case B_TXT_LINK_SCR_TRAINER_NAME: // link scripting active name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting.battler)].name;
                break;
            case B_TXT_PLAYER_NAME: // player name
                toCpy = gSaveBlock2Ptr->playerName;
                break;
            case B_TXT_TRAINER1_LOSE_TEXT: // trainerA lose text
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerALoseText();
                }
                break;
            case B_TXT_TRAINER1_WIN_TEXT: // trainerA win text
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerWonSpeech();
                }
                break;
            case B_TXT_TRAINER2_LOSE_TEXT:
                GetTrainerTowerOpponentLoseText(gStringVar4, 1);
                toCpy = gStringVar4;
                break;
            case B_TXT_TRAINER2_WIN_TEXT:
                GetTrainerTowerOpponentWinText(gStringVar4, 1);
                toCpy = gStringVar4;
                break;
            case B_TXT_26: // ?
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler, *(&gBattleStruct->scriptPartyIdx))
                break;
            case B_TXT_PC_CREATOR_NAME: // lanette pc
                if (FlagGet(FLAG_SYS_NOT_SOMEONES_PC))
                    toCpy = sText_Bills;
                else
                    toCpy = sText_Someones;
                break;
            case B_TXT_ATK_PREFIX2:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_DEF_PREFIX2:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_ATK_PREFIX1:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_DEF_PREFIX1:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_ATK_PREFIX3:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_DEF_PREFIX3:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            }

            // missing if (toCpy != NULL) check
            while (*toCpy != EOS)
            {
                dst[dstId] = *toCpy;
                dstId++;
                toCpy++;
            }
            if (*src == B_TXT_TRAINER1_LOSE_TEXT || *src == B_TXT_TRAINER1_WIN_TEXT
             || *src == B_TXT_TRAINER2_LOSE_TEXT || *src == B_TXT_TRAINER2_WIN_TEXT)
            {
                dst[dstId] = EXT_CTRL_CODE_BEGIN;
                dstId++;
                dst[dstId] = 9;
                dstId++;
            }
        }
        else
        {
            dst[dstId] = *src;
            dstId++;
        }
        src++;
    }

    dst[dstId] = *src;
    dstId++;

    return dstId;
}

static void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst)
{
    u32 srcId = 1;
    u32 value = 0;
    u8 text[12];
    u16 hword;

    *dst = EOS;
    while (src[srcId] != B_BUFF_EOS)
    {
        switch (src[srcId])
        {
        case B_BUFF_STRING: // battle string
            hword = T1_READ_16(&src[srcId + 1]);
            StringAppend(dst, gBattleStringsTable[hword - BATTLESTRINGS_TABLE_START]);
            srcId += 3;
            break;
        case B_BUFF_NUMBER: // int to string
            switch (src[srcId + 1])
            {
            case 1:
                value = src[srcId + 3];
                break;
            case 2:
                value = T1_READ_16(&src[srcId + 3]);
                break;
            case 4:
                value = T1_READ_32(&src[srcId + 3]);
                break;
            }
            ConvertIntToDecimalStringN(dst, value, STR_CONV_MODE_LEFT_ALIGN, src[srcId + 2]);
            srcId += src[srcId + 1] + 3;
            break;
        case B_BUFF_MOVE: // move name
            StringAppend(dst, gLongMoveNames[T1_READ_16(&src[srcId + 1])]);
            srcId += 3;
            break;
        case B_BUFF_TYPE: // type name
            StringAppend(dst, gTypeNames[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_MON_NICK_WITH_PREFIX: // poke nick with prefix
            if (GetBattlerSide(src[srcId + 1]) == B_SIDE_PLAYER)
            {
                GetMonData(&gPlayerParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
            }
            else
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                    StringAppend(dst, sText_FoePkmnPrefix);
                else
                    StringAppend(dst, sText_WildPkmnPrefix);

                GetMonData(&gEnemyParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
            }
            StringGetEnd10(text);
            StringAppend(dst, text);
            srcId += 3;
            break;
        case B_BUFF_STAT: // stats
            StringAppend(dst, gStatNamesTable[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_SPECIES: // species name
            GetSpeciesName(dst, T1_READ_16(&src[srcId + 1]));
            srcId += 3;
            break;
        case B_BUFF_MON_NICK: // poke nick without prefix
            if (GetBattlerSide(src[srcId + 1]) == B_SIDE_PLAYER)
                GetMonData(&gPlayerParty[src[srcId + 2]], MON_DATA_NICKNAME, dst);
            else
                GetMonData(&gEnemyParty[src[srcId + 2]], MON_DATA_NICKNAME, dst);
            StringGetEnd10(dst);
            srcId += 3;
            break;
        case B_BUFF_NEGATIVE_FLAVOR: // flavor table
            StringAppend(dst, gPokeblockWasTooXStringTable[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_ABILITY: // ability names
            StringAppend(dst, gAbilityNames[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_ITEM: // item name
            hword = T1_READ_16(&src[srcId + 1]);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (hword == ITEM_ENIGMA_BERRY)
                {
                    if (gLinkPlayers[gBattleStruct->multiplayerId].id == gPotentialItemEffectBattler)
                    {
                        StringCopy(dst, gEnigmaBerries[gPotentialItemEffectBattler].name);
                        StringAppend(dst, sText_BerrySuffix);
                    }
                    else
                    {
                        StringAppend(dst, sText_EnigmaBerry);
                    }
                }
                else
                {
                    CopyItemName(hword, dst);
                }
            }
            else
            {
                CopyItemName(hword, dst);
            }
            srcId += 3;
            break;
        }
    }
}

// Loads one of two text strings into the provided buffer. This is functionally
// unused, since the value loaded into the buffer is not read; it loaded one of
// two particles (either "は" or "の") which works in tandem with ChooseTypeOfMoveUsedString
// below to effect changes in the meaning of the line.
static void ChooseMoveUsedParticle(u8 *textBuff)
{
    s32 counter = 0;
    u32 i = 0;

    while (counter != MAX_MON_MOVES)
    {
        if (sGrammarMoveUsedTable[i] == 0)
            counter++;
        if (sGrammarMoveUsedTable[i++] == sBattleMsgDataPtr->currentMove)
            break;
    }

    if (counter >= 0)
    {
        if (counter <= 2)
            StringCopy(textBuff, sText_SpaceIs); // is
        else if (counter <= MAX_MON_MOVES)
            StringCopy(textBuff, sText_ApostropheS); // 's
    }
}

// Appends "!" to the text buffer `dst`. In the original Japanese this looked
// into the table of moves at sGrammarMoveUsedTable and varied the line accordingly.
//
// sText_ExclamationMark was a plain "!", used for any attack not on the list.
// It resulted in the translation "<NAME>'s <ATTACK>!".
//
// sText_ExclamationMark2 was "を つかった！". This resulted in the translation
// "<NAME> used <ATTACK>!", which was used for all attacks in English.
//
// sText_ExclamationMark3 was "した！". This was used for those moves whose
// names were verbs, such as Recover, and resulted in translations like "<NAME>
// recovered itself!".
//
// sText_ExclamationMark4 was "を した！" This resulted in a translation of
// "<NAME> did an <ATTACK>!".
//
// sText_ExclamationMark5 was " こうげき！" This resulted in a translation of
// "<NAME>'s <ATTACK> attack!".
static void ChooseTypeOfMoveUsedString(u8 *dst)
{
    s32 counter = 0;
    s32 i = 0;

    while (*dst != EOS)
        dst++;

    while (counter != MAX_MON_MOVES)
    {
        if (sGrammarMoveUsedTable[i] == MOVE_NONE)
            counter++;
        if (sGrammarMoveUsedTable[i++] == sBattleMsgDataPtr->currentMove)
            break;
    }

    switch (counter)
    {
    case 0:
        StringCopy(dst, sText_ExclamationMark);
        break;
    case 1:
        StringCopy(dst, sText_ExclamationMark2);
        break;
    case 2:
        StringCopy(dst, sText_ExclamationMark3);
        break;
    case 3:
        StringCopy(dst, sText_ExclamationMark4);
        break;
    case 4:
        StringCopy(dst, sText_ExclamationMark5);
        break;
    }
}

static const struct BattleWindowText sTextOnWindowsInfo_Normal[] = {
    {PIXEL_FILL(0xf), 2, 0x02, 2, 0, 2, 1, 0x1, 0xf, 0x6},
    {PIXEL_FILL(0xf), 2, 0x02, 2, 0, 2, 0, 0x1, 0xf, 0x6},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 2, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 0, 0x00, 1, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 0, 0x00, 1, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 0, 0x00, 1, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 0, 0x00, 1, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 0, 0x00, 2, 0, 0, 0, 0xc, 0xe, 0xb},
    {PIXEL_FILL(0xe), 0, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 1, 0x0a, 2, 0, 2, 0, 0xc, 0xe, 0xb},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 2, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 2, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 0, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0x0), 0, 0x20, 0, 0, 0, 0, 0x1, 0x0, 0x2},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 1, 2, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0x0), 2, 0x00, 2, 0, 0, 0, 0x1, 0x0, 0x6},
    {PIXEL_FILL(0x0), 2, 0x00, 2, 0, 0, 0, 0x1, 0x0, 0x6},
    {PIXEL_FILL(0x0), 2, 0x00, 2, 0, 0, 0, 0x1, 0x0, 0x6},
    {PIXEL_FILL(0x1), 4, 0x00, 1, 0, 1, 1, 0x2, 0x1, 0x3}
};

const u8 gUnknown_83FEC90[] = {0x04, 0x05, 0x02, 0x02};

// windowId: Upper 2 bits are text flags
//   x40: Use NPC context-defined font
//   x80: Inhibit window clear
void BattlePutTextOnWindow(const u8 *text, u8 windowId) {
    bool32 copyToVram;
    struct TextPrinterTemplate printerTemplate;
    u8 speed;
    int x;
    u8 context;

    u8 textFlags = windowId & 0xC0;
    windowId &= 0x3F;
    if (!(textFlags & 0x80))
        FillWindowPixelBuffer(windowId, sTextOnWindowsInfo_Normal[windowId].fillValue);
    if (textFlags & 0x40) {
        context = ContextNpcGetTextColor();
        printerTemplate.fontId = gUnknown_83FEC90[context];
    }
    else {
        printerTemplate.fontId = sTextOnWindowsInfo_Normal[windowId].fontId;
    }
    switch (windowId)
    {
    case 15 ... 20:
        x = (48 - GetStringWidth(sTextOnWindowsInfo_Normal[windowId].fontId, text,
                                 sTextOnWindowsInfo_Normal[windowId].letterSpacing)) / 2;
        break;
    case 21 ... 23:
        x = (64 - GetStringWidth(sTextOnWindowsInfo_Normal[windowId].fontId, text,
                                 sTextOnWindowsInfo_Normal[windowId].letterSpacing)) / 2;
        break;
    default:
        x = sTextOnWindowsInfo_Normal[windowId].x;
        break;
    }
    if (x < 0)
        x = 0;
    printerTemplate.currentChar = text;
    printerTemplate.windowId = windowId;
    printerTemplate.x = x;
    printerTemplate.y = sTextOnWindowsInfo_Normal[windowId].y;
    printerTemplate.currentX = printerTemplate.x;
    printerTemplate.currentY = printerTemplate.y;
    printerTemplate.letterSpacing = sTextOnWindowsInfo_Normal[windowId].letterSpacing;
    printerTemplate.lineSpacing = sTextOnWindowsInfo_Normal[windowId].lineSpacing;
    printerTemplate.unk = 0;
    printerTemplate.fgColor = sTextOnWindowsInfo_Normal[windowId].fgColor;
    printerTemplate.bgColor = sTextOnWindowsInfo_Normal[windowId].bgColor;
    printerTemplate.shadowColor = sTextOnWindowsInfo_Normal[windowId].shadowColor;
    if (windowId == 24)
        gTextFlags.useAlternateDownArrow = FALSE;
    else
        gTextFlags.useAlternateDownArrow = TRUE;

    if ((gBattleTypeFlags & BATTLE_TYPE_LINK) || ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && windowId != 24))
        gTextFlags.autoScroll = TRUE;
    else
        gTextFlags.autoScroll = FALSE;

    if (windowId == 0 || windowId == 24)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            speed = 1;
        else
            speed = GetTextSpeedSetting();
        gTextFlags.canABSpeedUpPrint = TRUE;
    }
    else
    {
        speed = sTextOnWindowsInfo_Normal[windowId].speed;
        gTextFlags.canABSpeedUpPrint = FALSE;
    }

    AddTextPrinter(&printerTemplate, speed, NULL);
    if (!(textFlags & 0x80))
    {
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_FULL);
    }
}

bool8 BattleStringShouldBeColored(u16 stringId)
{
    if (stringId == STRINGID_TRAINER1LOSETEXT || stringId == STRINGID_TRAINER2CLASS || stringId == STRINGID_TRAINER1WINTEXT || stringId == STRINGID_TRAINER2NAME)
        return TRUE;
    return FALSE;
}

void SetPpNumbersPaletteInMoveSelection(void)
{
    struct ChooseMoveStruct *chooseMoveStruct = (struct ChooseMoveStruct *)(&gBattleBufferA[gActiveBattler][4]);
    const u16 *palPtr = gUnknown_8D2FBB4;
    u8 var = GetCurrentPpToMaxPpState(chooseMoveStruct->currentPp[gMoveSelectionCursor[gActiveBattler]],
                                      chooseMoveStruct->maxPp[gMoveSelectionCursor[gActiveBattler]]);

    gPlttBufferUnfaded[92] = palPtr[(var * 2) + 0];
    gPlttBufferUnfaded[91] = palPtr[(var * 2) + 1];

    CpuCopy16(&gPlttBufferUnfaded[92], &gPlttBufferFaded[92], sizeof(u16));
    CpuCopy16(&gPlttBufferUnfaded[91], &gPlttBufferFaded[91], sizeof(u16));
}

u8 GetCurrentPpToMaxPpState(u8 currentPp, u8 maxPp)
{
    if (maxPp == currentPp)
    {
        return 3;
    }
    else if (maxPp <= 2)
    {
        if (currentPp > 1)
            return 3;
        else
            return 2 - currentPp;
    }
    else if (maxPp <= 7)
    {
        if (currentPp > 2)
            return 3;
        else
            return 2 - currentPp;
    }
    else
    {
        if (currentPp == 0)
            return 2;
        if (currentPp <= maxPp / 4)
            return 1;
        if (currentPp > maxPp / 2)
            return 3;
    }

    return 0;
}
