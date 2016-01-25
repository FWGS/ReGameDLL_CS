/*
*
*   This program is free software; you can redistribute it and/or modify it
*   under the terms of the GNU General Public License as published by the
*   Free Software Foundation; either version 2 of the License, or (at
*   your option) any later version.
*
*   This program is distributed in the hope that it will be useful, but
*   WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*   General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software Foundation,
*   Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*   In addition, as a special exception, the author gives permission to
*   link the code of this program with the Half-Life Game Engine ("HL
*   Engine") and Modified Game Libraries ("MODs") developed by Valve,
*   L.L.C ("Valve").  You must obey the GNU General Public License in all
*   respects for all of the code used other than the HL Engine and MODs
*   from Valve.  If you modify this file, you may extend this exception
*   to your version of the file, but you are not obligated to do so.  If
*   you do not wish to do so, delete this exception statement from your
*   version.
*
*/

#ifndef CS_BOT_H
#define CS_BOT_H
#ifdef _WIN32
#pragma once
#endif

#include "bot/cs_gamestate.h"
#include "bot/cs_bot_manager.h"
#include "bot/cs_bot_chatter.h"

#define CSBOT_VERSION_MAJOR		1
#define CSBOT_VERSION_MINOR		50

#define PRIMARY_WEAPON_BUY_COUNT	13
#define SECONDARY_WEAPON_BUY_COUNT	3

#define FLAG_PROGRESS_DRAW		0x0	// draw status bar progress
#define FLAG_PROGRESS_START		0x1	// init status bar progress
#define FLAG_PROGRESS_HIDE		0x2	// hide status bar progress

#define HI_X				0x01
#define LO_X				0x02
#define HI_Y				0x04
#define LO_Y				0x08
#define HI_Z				0x10
#define LO_Z				0x20

#ifdef HOOK_GAMEDLL

#define _navAreaCount (*pnavAreaCount)
#define _currentIndex (*pcurrentIndex)

#define primaryWeaponBuyInfoCT (*pprimaryWeaponBuyInfoCT)
#define secondaryWeaponBuyInfoCT (*psecondaryWeaponBuyInfoCT)

#define primaryWeaponBuyInfoT (*pprimaryWeaponBuyInfoT)
#define secondaryWeaponBuyInfoT (*psecondaryWeaponBuyInfoT)

#endif // HOOK_GAMEDLL

extern int _navAreaCount;
extern int _currentIndex;

extern struct BuyInfo primaryWeaponBuyInfoCT[ PRIMARY_WEAPON_BUY_COUNT ];
extern struct BuyInfo secondaryWeaponBuyInfoCT[ SECONDARY_WEAPON_BUY_COUNT ];

extern struct BuyInfo primaryWeaponBuyInfoT[ PRIMARY_WEAPON_BUY_COUNT ];
extern struct BuyInfo secondaryWeaponBuyInfoT[ SECONDARY_WEAPON_BUY_COUNT ];

class CCSBot;
class BotChatterInterface;

/* <3327a8> ../cstrike/dlls/bot/cs_bot.h:44 */
class BotState
{
public:
	virtual void OnEnter(CCSBot *me) {}
	virtual void OnUpdate(CCSBot *me) {}
	virtual void OnExit(CCSBot *me) {}
	virtual const char *GetName(void) const = 0;

};/* size: 4, cachelines: 1, members: 1 */

/* <3328b8> ../cstrike/dlls/bot/cs_bot.h:53 */
class IdleState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual const char *GetName(void) const { return "Idle"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);

#endif // HOOK_GAMEDLL

};/* size: 4, cachelines: 1, members: 1 */

/* <3328d9> ../cstrike/dlls/bot/cs_bot.h:61 */
class HuntState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "Hunt"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

	void ClearHuntArea(void) { m_huntArea = NULL; }
private:
	CNavArea *m_huntArea;

};/* size: 8, cachelines: 1, members: 2 */

/* <3328fb> ../cstrike/dlls/bot/cs_bot.h:75 */
class AttackState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "Attack"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

	void SetCrouchAndHold(bool crouch) { m_crouchAndHold = crouch; }
	void StopAttacking(CCSBot *me);

protected:
	enum DodgeStateType
	{
		STEADY_ON,
		SLIDE_LEFT,
		SLIDE_RIGHT,
		JUMP,
		NUM_ATTACK_STATES
	} m_dodgeState;

	float m_nextDodgeStateTimestamp;
	CountdownTimer m_repathTimer;
	float m_scopeTimestamp;
	bool m_haveSeenEnemy;
	bool m_isEnemyHidden;
	float m_reacquireTimestamp;
	float m_shieldToggleTimestamp;
	bool m_shieldForceOpen;
	float m_pinnedDownTimestamp;
	bool m_crouchAndHold;
	bool m_didAmbushCheck;
	bool m_dodge;
	bool m_firstDodge;
	bool m_isCoward;
	CountdownTimer m_retreatTimer;

};/* size: 60, cachelines: 1, members: 17 */

/* <332922> ../cstrike/dlls/bot/cs_bot.h:120 */
class InvestigateNoiseState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "InvestigateNoise"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

private:
	void AttendCurrentNoise(CCSBot *me);
	Vector m_checkNoisePosition;

};/* size: 16, cachelines: 1, members: 2 */

/* <332949> ../cstrike/dlls/bot/cs_bot.h:133 */
class BuyState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "Buy"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

private:
	bool m_isInitialDelay;
	int m_prefRetries;
	int m_prefIndex;
	int m_retries;
	bool m_doneBuying;
	bool m_buyDefuseKit;
	bool m_buyGrenade;
	bool m_buyShield;
	bool m_buyPistol;

};/* size: 28, cachelines: 1, members: 10 */

/* <332970> ../cstrike/dlls/bot/cs_bot.h:154 */
class MoveToState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "MoveTo"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

	void SetGoalPosition(const Vector &pos) { m_goalPosition = pos; }
	void SetRouteType(RouteType route) { m_routeType = route; }

private:
	Vector m_goalPosition;
	RouteType m_routeType;
	bool m_radioedPlan;
	bool m_askedForCover;

};/* size: 24, cachelines: 1, members: 5 */

/* <332997> ../cstrike/dlls/bot/cs_bot.h:171 */
class FetchBombState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual const char *GetName(void) const { return "FetchBomb"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);

#endif // HOOK_GAMEDLL

};/* size: 4, cachelines: 1, members: 1 */

/* <3329be> ../cstrike/dlls/bot/cs_bot.h:179 */
class PlantBombState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "PlantBomb"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

};/* size: 4, cachelines: 1, members: 1 */

/* <3329e5> ../cstrike/dlls/bot/cs_bot.h:188 */
class DefuseBombState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "DefuseBomb"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

};/* size: 4, cachelines: 1, members: 1 */

/* <332a0c> ../cstrike/dlls/bot/cs_bot.h:197 */
class HideState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "Hide"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

public:
	void SetHidingSpot(const Vector &pos)	{ m_hidingSpot = pos; }
	const Vector &GetHidingSpot(void) const	{ return m_hidingSpot; }

	void SetSearchArea(CNavArea *area) { m_searchFromArea = area; }
	void SetSearchRange(float range) { m_range = range; }

	void SetDuration(float time) { m_duration = time; }
	void SetHoldPosition(bool hold) { m_isHoldingPosition = hold; }

	bool IsAtSpot(void) const { return m_isAtSpot; }

private:
	CNavArea *m_searchFromArea;
	float m_range;

	Vector m_hidingSpot;
	bool m_isAtSpot;
	float m_duration;
	bool m_isHoldingPosition;
	float m_holdPositionTime;
	bool m_heardEnemy;

	float m_firstHeardEnemyTime;
	int m_retry;
	Vector m_leaderAnchorPos;

};/* size: 64, cachelines: 1, members: 12 */

/* <332a33> ../cstrike/dlls/bot/cs_bot.h:234 */
class EscapeFromBombState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "EscapeFromBomb"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

};/* size: 4, cachelines: 1, members: 1 */

/* <332a5a> ../cstrike/dlls/bot/cs_bot.h:243 */
class FollowState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "Follow"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

	void SetLeader(CBaseEntity *leader) { m_leader = leader; }

private:
	void ComputeLeaderMotionState(float leaderSpeed);

	EHANDLE m_leader;
	Vector m_lastLeaderPos;
	bool m_isStopped;
	float m_stoppedTimestamp;

	enum LeaderMotionStateType
	{
		INVALID,
		STOPPED,
		WALKING,
		RUNNING

	} m_leaderMotionState;

	IntervalTimer m_leaderMotionStateTime;

	bool m_isSneaking;
	float m_lastSawLeaderTime;
	CountdownTimer m_repathInterval;

	IntervalTimer m_walkTime;
	bool m_isAtWalkSpeed;

	float m_waitTime;
	CountdownTimer m_idleTimer;

};/* size: 76, cachelines: 2, members: 14 */

/* <332a81> ../cstrike/dlls/bot/cs_bot.h:282 */
class UseEntityState: public BotState
{
public:
	virtual void OnEnter(CCSBot *me);
	virtual void OnUpdate(CCSBot *me);
	virtual void OnExit(CCSBot *me);
	virtual const char *GetName(void) const { return "UseEntity"; }

#ifdef HOOK_GAMEDLL

	void OnEnter_(CCSBot *me);
	void OnUpdate_(CCSBot *me);
	void OnExit_(CCSBot *me);

#endif // HOOK_GAMEDLL

	void SetEntity(CBaseEntity *entity) { m_entity = entity; }

private:
	EHANDLE m_entity;

};/* size: 12, cachelines: 1, members: 2 */

// The Counter-strike Bot

/* <32b267> ../cstrike/dlls/bot/cs_bot.h:300 */
class CCSBot: public CBot
{
public:
	CCSBot(void);														// constructor initializes all values to zero

public:
	virtual int TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType);		// invoked when injured by something (EXTEND) - returns the amount of damage inflicted
	virtual void Killed(entvars_t *pevAttacker, int iGib);									// invoked when killed (EXTEND)
	virtual void RoundRespawn(void);
	virtual void Blind(float duration, float holdTime, float fadeTime, int alpha = 255);					// player blinded by a flashbang
	virtual void OnTouchingWeapon(CWeaponBox *box);										// invoked when in contact with a CWeaponBox

	virtual bool Initialize(const BotProfile *profile);									// (EXTEND) prepare bot for action
	virtual void SpawnBot(void);												// (EXTEND) spawn the bot into the game

	virtual void Upkeep(void);												// lightweight maintenance, invoked frequently
	virtual void Update(void);												// heavyweight algorithms, invoked less often

	virtual void Walk(void);
	virtual bool Jump(bool mustJump = false);										// returns true if jump was started

	virtual void OnEvent(GameEventType event, CBaseEntity *entity = NULL, CBaseEntity *other = NULL);			// invoked when event occurs in the game (some events have NULL entity)

	#define CHECK_FOV true
	virtual bool IsVisible(const Vector *pos, bool testFOV = false) const;							// return true if we can see the point
	virtual bool IsVisible(CBasePlayer *player, bool testFOV = false, unsigned char *visParts = NULL) const;		// return true if we can see any part of the player

	virtual bool IsEnemyPartVisible(VisiblePartType part) const;								// if enemy is visible, return the part we see for our current enemy

#ifdef HOOK_GAMEDLL

	bool Initialize_(const BotProfile *profile);
	void SpawnBot_(void);
	void Upkeep_(void);
	void Update_(void);
	void Walk_(void);
	bool Jump_(bool mustJump);
	void Blind_(float duration, float holdTime, float fadeTime, int alpha);
	int TakeDamage_(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType);
	void Killed_(entvars_t *pevAttacker, int iGib);
	void OnTouchingWeapon_(CWeaponBox *box);
	void OnEvent_(GameEventType event, CBaseEntity *entity, CBaseEntity *other);
	bool IsVisible_(const Vector *pos, bool testFOV) const;
	bool IsVisible_(CBasePlayer *player, bool testFOV, unsigned char *visParts) const;
	bool IsEnemyPartVisible_(VisiblePartType part) const;
	void RoundRespawn_(void);

#endif // HOOK_GAMEDLL

public:
	void Disconnect(void);

	// behavior properties
	float GetCombatRange(void) const;
	bool IsRogue(void) const;				// return true if we dont listen to teammates or pursue scenario goals
	void SetRogue(bool rogue);
	bool IsHurrying(void) const;				// return true if we are in a hurry
	void Hurry(float duration);				// force bot to hurry
	bool IsSafe(void) const;				// return true if we are in a safe region
	bool IsWellPastSafe(void) const;			// return true if it is well past the early, "safe", part of the round
	bool IsEndOfSafeTime(void) const;			// return true if we were in the safe time last update, but not now
	float GetSafeTimeRemaining(void) const;			// return the amount of "safe time" we have left
	float GetSafeTime(void) const;				// return what we think the total "safe time" for this map is
	NOXREF bool IsUnhealthy(void) const;			// returns true if bot is low on health

	// behaviors
	void Idle(void);
	void Hide(CNavArea *searchFromArea = NULL, float duration = -1.0f, float hideRange = 750.0f, bool holdPosition = false);						// DEPRECATED: Use TryToHide() instead
	#define USE_NEAREST true
	bool TryToHide(CNavArea *searchFromArea = NULL, float duration = -1.0f, float hideRange = 750.0f, bool holdPosition = false, bool useNearest = false);			// try to hide nearby, return false if cannot

	void Hide(const Vector *hidingSpot, float duration = -1.0f, bool holdPosition = false);											// move to the given hiding place
	bool IsHiding(void) const;																		// returns true if bot is currently hiding
	bool IsAtHidingSpot(void) const;																	// return true if we are hiding and at our hiding spot
	bool TryToRetreat(void);																		// retreat to a nearby hiding spot, away from enemies

	void Hunt(void);
	bool IsHunting(void) const;								// returns true if bot is currently hunting

	void Attack(CBasePlayer *victim);
	void FireWeaponAtEnemy(void);								// fire our active weapon towards our current enemy
	void StopAttacking(void);
	bool IsAttacking(void) const;								// returns true if bot is currently engaging a target

	void MoveTo(const Vector *pos, RouteType route = SAFEST_ROUTE);				// move to potentially distant position
	bool IsMovingTo(void) const;								// return true if we are in the MoveTo state

	void PlantBomb(void);

	void FetchBomb(void);									// bomb has been dropped - go get it
	bool NoticeLooseBomb(void) const;							// return true if we noticed the bomb on the ground or on radar
	bool CanSeeLooseBomb(void) const;							// return true if we directly see the loose bomb
	bool IsCarryingBomb(void) const;

	void DefuseBomb(void);
	bool IsDefusingBomb(void) const;							// returns true if bot is currently defusing the bomb
	bool CanSeePlantedBomb(void) const;							// return true if we directly see the planted bomb

	void EscapeFromBomb(void);
	bool IsEscapingFromBomb(void) const;							// return true if we are escaping from the bomb

	void RescueHostages(void);
	void UseEntity(CBaseEntity *entity);							// use the entity

	bool IsBuying(void) const;

	void Panic(CBasePlayer *enemy);								// look around in panic
	void Follow(CBasePlayer *player);							// begin following given Player
	void ContinueFollowing(void);								// continue following our leader after finishing what we were doing
	void StopFollowing(void);								// stop following
	bool IsFollowing(void) const;								// return true if we are following someone (not necessarily in the follow state)
	CBasePlayer *GetFollowLeader(void);							// return the leader we are following
	float GetFollowDuration(void) const;							// return how long we've been following our leader
	bool CanAutoFollow(void) const;								// return true if we can auto-follow

	bool IsNotMoving(void) const;								// return true if we are currently standing still

	void AimAtEnemy(void);									// point our weapon towards our enemy
	void StopAiming(void);									// stop aiming at enemy
	bool IsAimingAtEnemy(void) const;							// returns true if we are trying to aim at an enemy

	bool IsSurprised(void) const;								// return true if we are "surprised"
	float GetSurpriseDelay(void) const;
	void ClearSurpriseDelay(void);

	float GetStateTimestamp(void) const;							// get time current state was entered

	bool IsDoingScenario(void) const;							// return true if we will do scenario-related tasks

	// scenario / gamestate
	CSGameState *GetGameState(void);							// return an interface to this bot's gamestate
	const CSGameState *GetGameState(void) const;						// return an interface to this bot's gamestate

	bool IsAtBombsite(void);								// return true if we are in a bomb planting zone
	bool GuardRandomZone(float range = 500.0f);						// pick a random zone and hide near it

	bool IsBusy(void) const;								// return true if we are busy doing something important

	// high-level tasks
	enum TaskType
	{
		SEEK_AND_DESTROY,
		PLANT_BOMB,
		FIND_TICKING_BOMB,
		DEFUSE_BOMB,
		GUARD_TICKING_BOMB,
		GUARD_BOMB_DEFUSER,
		GUARD_LOOSE_BOMB,
		GUARD_BOMB_ZONE,
		ESCAPE_FROM_BOMB,
		HOLD_POSITION,
		FOLLOW,
		VIP_ESCAPE,
		GUARD_VIP_ESCAPE_ZONE,
		COLLECT_HOSTAGES,
		RESCUE_HOSTAGES,
		GUARD_HOSTAGES,
		GUARD_HOSTAGE_RESCUE_ZONE,
		MOVE_TO_LAST_KNOWN_ENEMY_POSITION,
		MOVE_TO_SNIPER_SPOT,
		SNIPING,

		NUM_TASKS
	};

	void SetTask(TaskType task, CBaseEntity *entity = NULL);				// set our current "task"
	TaskType GetTask(void) const;
	CBaseEntity *GetTaskEntity(void);

	// behavior modifiers
	enum DispositionType
	{
		ENGAGE_AND_INVESTIGATE,		// engage enemies on sight and investigate enemy noises
		OPPORTUNITY_FIRE,		// engage enemies on sight, but only look towards enemy noises, dont investigate
		SELF_DEFENSE,			// only engage if fired on, or very close to enemy
		IGNORE_ENEMIES,			// ignore all enemies - useful for ducking around corners, running away, etc

		NUM_DISPOSITIONS
	};

	void SetDisposition(DispositionType disposition);					// define how we react to enemies
	DispositionType GetDisposition(void) const;						// return enum describing current disposition

	void IgnoreEnemies(float duration);							// ignore enemies for a short duration

	enum MoraleType
	{
		TERRIBLE = -3,
		BAD = -2,
		NEGATIVE = -1,
		NEUTRAL = 0,
		POSITIVE = 1,
		GOOD = 2,
		EXCELLENT = 3,
	};

	MoraleType GetMorale(void) const;							// return enum describing current morale
	void IncreaseMorale(void);
	void DecreaseMorale(void);

	// listening for noises
	bool IsNoiseHeard(void) const;								// return true if we have heard a noise
	bool ShouldInvestigateNoise(float *retNoiseDist = NULL);
	void InvestigateNoise(void);								// investigate recent enemy noise
	const Vector *GetNoisePosition(void) const;						// return position of last heard noise, or NULL if none heard
	CNavArea *GetNoiseArea(void) const;							// return area where noise was heard
	void ForgetNoise(void);									// clear the last heard noise
	bool CanSeeNoisePosition(void) const;							// return true if we directly see where we think the noise came from
	NOXREF float GetNoiseRange(void) const;							// return approximate distance to last noise heard

	bool CanHearNearbyEnemyGunfire(float range = -1.0f) const;				// return true if we hear nearby threatening enemy gunfire within given range (-1 == infinite)
	PriorityType GetNoisePriority(void) const;						// return priority of last heard noise

	// radio and chatter
	void SendRadioMessage(GameEventType event);							// send voice chatter
	BotChatterInterface *GetChatter(void);								// return an interface to this bot's chatter system
	bool RespondToHelpRequest(CBasePlayer *them, Place place, float maxRange = -1.0f);		// decide if we should move to help the player, return true if we will
	void StartVoiceFeedback(float duration);
	bool IsUsingVoice(void) const;									// new-style "voice" chatter gets voice feedback

	// enemies
	// BOTPORT: GetEnemy() collides with GetEnemy() in CBaseEntity - need to use different nomenclature

	void SetEnemy(CBasePlayer *enemy);							// set given player as our current enemy
	CBasePlayer *GetEnemy(void);
	int GetNearbyEnemyCount(void) const;							// return max number of nearby enemies we've seen recently
	unsigned int GetEnemyPlace(void) const;							// return location where we see the majority of our enemies
	bool CanSeeBomber(void) const;								// return true if we can see the bomb carrier
	CBasePlayer *GetBomber(void) const;

	int GetNearbyFriendCount(void) const;							// return number of nearby teammates
	CBasePlayer *GetClosestVisibleFriend(void) const;					// return the closest friend that we can see
	CBasePlayer *GetClosestVisibleHumanFriend(void) const;					// return the closest human friend that we can see

	bool IsOutnumbered(void) const;								// return true if we are outnumbered by enemies
	int OutnumberedCount(void) const;							// return number of enemies we are outnumbered by

	#define ONLY_VISIBLE_ENEMIES true
	CBasePlayer *GetImportantEnemy(bool checkVisibility = false) const;			// return the closest "important" enemy for the given scenario (bomb carrier, VIP, hostage escorter)

	void UpdateReactionQueue(void);								// update our reaction time queue
	CBasePlayer *GetRecognizedEnemy(void);							// return the most dangerous threat we are "conscious" of
	bool IsRecognizedEnemyReloading(void);							// return true if the enemy we are "conscious" of is reloading
	bool IsRecognizedEnemyProtectedByShield(void);						// return true if the enemy we are "conscious" of is hiding behind a shield
	float GetRangeToNearestRecognizedEnemy(void);						// return distance to closest enemy we are "conscious" of

	CBasePlayer *GetAttacker(void) const;							// return last enemy that hurt us
	float GetTimeSinceAttacked(void) const;							// return duration since we were last injured by an attacker
	float GetFirstSawEnemyTimestamp(void) const;						// time since we saw any enemies
	float GetLastSawEnemyTimestamp(void) const;
	float GetTimeSinceLastSawEnemy(void) const;
	float GetTimeSinceAcquiredCurrentEnemy(void) const;
	bool HasNotSeenEnemyForLongTime(void) const;						// return true if we haven't seen an enemy for "a long time"
	const Vector &GetLastKnownEnemyPosition(void) const;
	bool IsEnemyVisible(void) const;							// is our current enemy visible
	float GetEnemyDeathTimestamp(void) const;
	bool IsFriendInLineOfFire(void);							// return true if a friend is in our weapon's way
	bool IsAwareOfEnemyDeath(void) const;							// return true if we *noticed* that our enemy died
	int GetLastVictimID(void) const;							// return the ID (entindex) of the last victim we killed, or zero

	// navigation
	bool HasPath(void) const;
	void DestroyPath(void);

	float GetFeetZ(void) const;								// return Z of bottom of feet

	enum PathResult
	{
		PROGRESSING,	// we are moving along the path
		END_OF_PATH,	// we reached the end of the path
		PATH_FAILURE,	// we failed to reach the end of the path
	};
	#define NO_SPEED_CHANGE false
	PathResult UpdatePathMovement(bool allowSpeedChange = true);					// move along our computed path - if allowSpeedChange is true, bot will walk when near goal to ensure accuracy

	NOXREF bool AStarSearch(CNavArea *startArea, CNavArea *goalArea);				// find shortest path from startArea to goalArea - don't actually buid the path
	bool ComputePath(CNavArea *goalArea, const Vector *goal, RouteType route);			// compute path to goal position
	bool StayOnNavMesh(void);
	CNavArea *GetLastKnownArea(void) const;								// return the last area we know we were inside of
	const Vector &GetPathEndpoint(void) const;							// return final position of our current path
	float GetPathDistanceRemaining(void) const;							// eturn estimated distance left to travel along path
	void ResetStuckMonitor(void);
	NOXREF bool IsAreaVisible(CNavArea *area) const;						// is any portion of the area visible to this bot
	const Vector &GetPathPosition(int numpath) const;
	bool GetSimpleGroundHeightWithFloor(const Vector *pos, float *height, Vector *normal = NULL);	// find "simple" ground height, treating current nav area as part of the floor

	Place GetPlace(void) const;									// get our current radio chatter place

	bool IsUsingLadder(void) const;									// returns true if we are in the process of negotiating a ladder
	void GetOffLadder(void);

	void SetGoalEntity(CBaseEntity *entity);
	CBaseEntity *GetGoalEntity(void);

	bool IsNearJump(void) const;									// return true if nearing a jump in the path
	float GetApproximateFallDamage(float height) const;						// return how much damage will will take from the given fall height

	void ForceRun(float duration);									// force the bot to run if it moves for the given duration
	void Wiggle(void);										// random movement, for getting un-stuck

	bool IsFriendInTheWay(const Vector *goalPos) const;						// return true if a friend is between us and the given position
	void FeelerReflexAdjustment(Vector *goalPosition);						// do reflex avoidance movements if our "feelers" are touched

	// looking around
	void SetLookAngles(float yaw, float pitch);			// set our desired look angles
	void UpdateLookAngles(void);					// move actual view angles towards desired ones
	void UpdateLookAround(bool updateNow = false);			// update "looking around" mechanism
	void InhibitLookAround(float duration);				// block all "look at" and "looking around" behavior for given duration - just look ahead

	// TODO: Clean up notion of "forward angle" and "look ahead angle"
	void SetForwardAngle(float angle);				// define our forward facing
	void SetLookAheadAngle(float angle);				// define default look ahead angle

	// look at the given point in space for the given duration (-1 means forever)
	void SetLookAt(const char *desc, const Vector *pos, PriorityType pri, float duration = -1.0f, bool clearIfClose = false, float angleTolerance = 5.0f);
	void ClearLookAt(void);						// stop looking at a point in space and just look ahead
	bool IsLookingAtSpot(PriorityType pri = PRIORITY_LOW) const;	// return true if we are looking at spot with equal or higher priority
	bool IsViewMoving(float angleVelThreshold = 1.0f) const;	// returns true if bot's view angles are rotating (not still)

	const Vector &GetEyePosition(void) const
	{
		m_eyePos = pev->origin + pev->view_ofs;
		return m_eyePos;
	}
	float ComputeWeaponSightRange(void);				// return line-of-sight distance to obstacle along weapon fire ray

	// approach points
	void ComputeApproachPoints(void);					// determine the set of "approach points" representing where the enemy can enter this region
	NOXREF void UpdateApproachPoints(void);					// recompute the approach point set if we have moved far enough to invalidate the current ones
	void ClearApproachPoints(void);
	void DrawApproachPoints(void);						// for debugging
	float GetHidingSpotCheckTimestamp(HidingSpot *spot) const;		// return time when given spot was last checked
	void SetHidingSpotCheckTimestamp(HidingSpot *spot);			// set the timestamp of the given spot to now

	// weapon query and equip
	#define MUST_EQUIP true
	void EquipBestWeapon(bool mustEquip = false);				// equip the best weapon we are carrying that has ammo
	void EquipPistol(void);							// equip our pistol
	void EquipKnife(void);							// equip our knife

	#define DONT_USE_SMOKE_GRENADE true
	bool EquipGrenade(bool noSmoke = false);				// equip a grenade, return false if we cant

	bool IsUsingKnife(void) const;						// returns true if we have knife equipped
	bool IsUsingPistol(void) const;						// returns true if we have pistol equipped
	bool IsUsingGrenade(void) const;					// returns true if we have grenade equipped
	bool IsUsingSniperRifle(void) const;					// returns true if using a "sniper" rifle
	bool IsUsingAWP(void) const;						// returns true if we have AWP equipped

	bool IsSniper(void) const;						// return true if we have a sniper rifle in our inventory
	bool IsSniping(void) const;						// return true if we are actively sniping (moving to sniper spot or settled in)
	bool IsUsingShotgun(void) const;					// returns true if using a shotgun
	bool IsUsingMachinegun(void) const;					// returns true if using the big 'ol machinegun
	void ThrowGrenade(const Vector *target);				// begin the process of throwing the grenade
	bool IsThrowingGrenade(void) const;					// return true if we are in the process of throwing a grenade
	bool HasGrenade(void) const;						// return true if we have a grenade in our inventory

	bool DoesActiveWeaponHaveSilencer(void) const;
	bool IsUsingHEGrenade(void) const;
	void StartRapidFire(void);
	void StopRapidFire(void);
	bool IsRapidFiring(void) const;

	enum ZoomType { NO_ZOOM, LOW_ZOOM, HIGH_ZOOM };
	ZoomType GetZoomLevel(void) const;					// return the current zoom level of our weapon

	bool AdjustZoom(float range);						// change our zoom level to be appropriate for the given range

	bool IsPrimaryWeaponEmpty(void) const;					// return true if primary weapon doesn't exist or is totally out of ammo
	bool IsPistolEmpty(void) const;						// return true if secondary weapon doesn't exist or is totally out of ammo

	int GetHostageEscortCount(void) const;
	void IncreaseHostageEscortCount(void);
	float GetRangeToFarthestEscortedHostage(void) const;
	void ResetWaitForHostagePatience(void);
	void ResetValues(void);							// reset internal data to initial state
	void BotDeathThink(void);
	CBasePlayer *FindNearbyPlayer(void);
	void AdjustSafeTime(void);						// called when enemy seen to adjust safe time for this round
	void EXPORT BotTouch(CBaseEntity *other);
	bool HasAnyAmmo(CBasePlayerWeapon *weapon) const;

#ifndef HOOK_GAMEDLL
private:
#endif // HOOK_GAMEDLL
	friend class CCSBotManager;

	// TODO: Get rid of these
	friend class AttackState;
	friend class BuyState;

	char m_name[64];					// copied from STRING(pev->netname) for debugging

	// behavior properties
	float m_combatRange;					// desired distance between us and them during gunplay
	mutable bool m_isRogue;					// if true, the bot is a "rogue" and listens to no-one
	mutable CountdownTimer m_rogueTimer;
	MoraleType m_morale;					// our current morale, based on our win/loss history
	bool m_diedLastRound;					// true if we died last round
	float m_safeTime;					// duration at the beginning of the round where we feel "safe"
	bool m_wasSafe;						// true if we were in the safe time last update
	NavRelativeDirType m_blindMoveDir;			// which way to move when we're blind
	bool m_blindFire;					// if true, fire weapon while blinded

	// TODO: implement through CountdownTimer
	float m_surpriseDelay;					// when we were surprised
	float m_surpriseTimestamp;

	bool m_isFollowing;					// true if we are following someone
	EHANDLE m_leader;					// the ID of who we are following
	float m_followTimestamp;				// when we started following
	float m_allowAutoFollowTime;				// time when we can auto follow

	CountdownTimer m_hurryTimer;				// if valid, bot is in a hurry

	// instances of each possible behavior state, to avoid dynamic memory allocation during runtime
	IdleState m_idleState;
	HuntState m_huntState;
	AttackState m_attackState;
	InvestigateNoiseState m_investigateNoiseState;
	BuyState m_buyState;
	MoveToState m_moveToState;
	FetchBombState m_fetchBombState;
	PlantBombState m_plantBombState;
	DefuseBombState m_defuseBombState;
	HideState m_hideState;
	EscapeFromBombState m_escapeFromBombState;
	FollowState m_followState;
	UseEntityState m_useEntityState;

	// TODO: Allow multiple simultaneous state machines (look around, etc)
	void SetState(BotState *state);						// set the current behavior state
	BotState *m_state;							// current behavior state
	float m_stateTimestamp;							// time state was entered
	bool m_isAttacking;							// if true, special Attack state is overriding the state machine

	TaskType m_task;							// our current task
	EHANDLE m_taskEntity;							// an entity used for our task

	// navigation
	Vector m_goalPosition;
	EHANDLE m_goalEntity;
	void MoveTowardsPosition(const Vector *pos);				// move towards position, independant of view angle
	NOXREF void MoveAwayFromPosition(const Vector *pos);			// move away from position, independant of view angle
	void StrafeAwayFromPosition(const Vector *pos);				// strafe (sidestep) away from position, independant of view angle
	void StuckCheck(void);							// check if we have become stuck

	CNavArea *m_currentArea;						// the nav area we are standing on
	CNavArea *m_lastKnownArea;						// the last area we were in
	EHANDLE m_avoid;							// higher priority player we need to make way for
	float m_avoidTimestamp;
	bool m_isJumpCrouching;
	bool m_isJumpCrouched;
	float m_jumpCrouchTimestamp;

	// path navigation data
	enum { 	MAX_PATH_LENGTH = 256 };
	struct ConnectInfo
	{
		CNavArea *area;			// the area along the path
		NavTraverseType how;		// how to enter this area from the previous one
		Vector pos;			// our movement goal position at this point in the path
		const CNavLadder *ladder;	// if "how" refers to a ladder, this is it
	}
	m_path[ MAX_PATH_LENGTH ];
	int m_pathLength;
	int m_pathIndex;
	float m_areaEnteredTimestamp;
	void BuildTrivialPath(const Vector *goal);										// build trivial path to goal, assuming we are already in the same area
	bool FindGrenadeTossPathTarget(Vector *pos);

	CountdownTimer m_repathTimer;												// must have elapsed before bot can pathfind again

	bool ComputePathPositions(void);											// determine actual path positions bot will move between along the path
	void SetupLadderMovement(void);
	void SetPathIndex(int newIndex);											// set the current index along the path
	void DrawPath(void);
	int FindOurPositionOnPath(Vector *close, bool local = false) const;							// compute the closest point to our current position on our path
	int FindPathPoint(float aheadRange, Vector *point, int *prevIndex = NULL);						// compute a point a fixed distance ahead along our path.
	bool FindClosestPointOnPath(const Vector *worldPos, int startIndex, int endIndex, Vector *close) const;			// compute closest point on path to given point
	bool IsStraightLinePathWalkable(const Vector *goal) const;								// test for un-jumpable height change, or unrecoverable fall

	mutable CountdownTimer m_avoidFriendTimer;		// used to throttle how often we check for friends in our path
	mutable bool m_isFriendInTheWay;			// true if a friend is blocking our path
	CountdownTimer m_politeTimer;				// we'll wait for friend to move until this runs out
	bool m_isWaitingBehindFriend;				// true if we are waiting for a friend to move

	#define ONLY_JUMP_DOWN true
	bool DiscontinuityJump(float ground, bool onlyJumpDown = false, bool mustJump = false);					// check if we need to jump due to height change

	enum LadderNavState
	{
		APPROACH_ASCENDING_LADDER,		// prepare to scale a ladder
		APPROACH_DESCENDING_LADDER,		// prepare to go down ladder
		FACE_ASCENDING_LADDER,
		FACE_DESCENDING_LADDER,
		MOUNT_ASCENDING_LADDER,			// move toward ladder until "on" it
		MOUNT_DESCENDING_LADDER,		// move toward ladder until "on" it
		ASCEND_LADDER,				// go up the ladder
		DESCEND_LADDER,				// go down the ladder
		DISMOUNT_ASCENDING_LADDER,		// get off of the ladder
		DISMOUNT_DESCENDING_LADDER,		// get off of the ladder
		MOVE_TO_DESTINATION,			// dismount ladder and move to destination area
	}
	m_pathLadderState;
	bool m_pathLadderFaceIn;			// if true, face towards ladder, otherwise face away
	const CNavLadder *m_pathLadder;			// the ladder we need to use to reach the next area
	bool UpdateLadderMovement(void);		// called by UpdatePathMovement()
	NavRelativeDirType m_pathLadderDismountDir;	// which way to dismount
	float m_pathLadderDismountTimestamp;		// time when dismount started
	float m_pathLadderEnd;				// if ascending, z of top, if descending z of bottom
	void ComputeLadderEndpoint(bool isAscending);
	float m_pathLadderTimestamp;			// time when we started using ladder - for timeout check

	CountdownTimer m_mustRunTimer;			// if nonzero, bot cannot walk

	// game scenario mechanisms
	CSGameState m_gameState;

	// hostages mechanism
	byte m_hostageEscortCount;
	void UpdateHostageEscortCount(void);
	float m_hostageEscortCountTimestamp;
	bool m_isWaitingForHostage;
	CountdownTimer m_inhibitWaitingForHostageTimer;
	CountdownTimer m_waitForHostageTimer;

	// listening mechanism
	Vector m_noisePosition;				// position we last heard non-friendly noise
	float m_noiseTimestamp;				// when we heard it (can get zeroed)
	CNavArea *m_noiseArea;				// the nav area containing the noise
	float m_noiseCheckTimestamp;
	PriorityType m_noisePriority;			// priority of currently heard noise
	bool UpdateLookAtNoise(void);			// return true if we decided to look towards the most recent noise source
	bool m_isNoiseTravelRangeChecked;

	// "looking around" mechanism
	float m_lookAroundStateTimestamp;		// time of next state change
	float m_lookAheadAngle;				// our desired forward look angle
	float m_forwardAngle;				// our current forward facing direction
	float m_inhibitLookAroundTimestamp;		// time when we can look around again

	enum LookAtSpotState
	{
		NOT_LOOKING_AT_SPOT,		// not currently looking at a point in space
		LOOK_TOWARDS_SPOT,		// in the process of aiming at m_lookAtSpot
		LOOK_AT_SPOT,			// looking at m_lookAtSpot
		NUM_LOOK_AT_SPOT_STATES
	}
	m_lookAtSpotState;
	Vector m_lookAtSpot;					// the spot we're currently looking at
	PriorityType m_lookAtSpotPriority;
	float m_lookAtSpotDuration;				// how long we need to look at the spot
	float m_lookAtSpotTimestamp;				// when we actually began looking at the spot
	float m_lookAtSpotAngleTolerance;			// how exactly we must look at the spot
	bool m_lookAtSpotClearIfClose;				// if true, the look at spot is cleared if it gets close to us
	const char *m_lookAtDesc;				// for debugging
	void UpdateLookAt(void);
	void UpdatePeripheralVision(void);			// update enounter spot timestamps, etc
	float m_peripheralTimestamp;

	enum { MAX_APPROACH_POINTS = 16 };
	Vector m_approachPoint[ MAX_APPROACH_POINTS ];
	unsigned char m_approachPointCount;
	Vector m_approachPointViewPosition;			// the position used when computing current approachPoint set

	bool BendLineOfSight(const Vector *eye, const Vector *point, Vector *bend) const;		// "bend" our line of sight until we can see the target point. Return bend point, false if cant bend.
	NOXREF bool FindApproachPointNearestPath(Vector *pos);						// find the approach point that is nearest to our current path, ahead of us

	bool m_isWaitingToTossGrenade;				// lining up throw
	CountdownTimer m_tossGrenadeTimer;			// timeout timer for grenade tossing

	SpotEncounter *m_spotEncounter;				// the spots we will encounter as we move thru our current area
	float m_spotCheckTimestamp;				// when to check next encounter spot

	// TODO: Add timestamp for each possible client to hiding spots
	enum { MAX_CHECKED_SPOTS = 64 };
	struct HidingSpotCheckInfo
	{
		HidingSpot *spot;
		float timestamp;
	}
	m_checkedHidingSpot[ MAX_CHECKED_SPOTS ];
	int m_checkedHidingSpotCount;

	// view angle mechanism
	float m_lookPitch;					// our desired look pitch angle
	float m_lookPitchVel;
	float m_lookYaw;					// our desired look yaw angle
	float m_lookYawVel;

	// aim angle mechanism
	mutable Vector m_eyePos;
	Vector m_aimOffset;					// current error added to victim's position to get actual aim spot
	Vector m_aimOffsetGoal;					// desired aim offset
	float m_aimOffsetTimestamp;				// time of next offset adjustment
	float m_aimSpreadTimestamp;				// time used to determine max spread as it begins to tighten up
	void SetAimOffset(float accuracy);			// set the current aim offset
	void UpdateAimOffset(void);				// wiggle aim error based on m_accuracy
	Vector m_aimSpot;					// the spot we are currently aiming to fire at

	// attack state data
	DispositionType m_disposition;				// how we will react to enemies
	CountdownTimer m_ignoreEnemiesTimer;			// how long will we ignore enemies
	mutable EHANDLE m_enemy;				// our current enemy
	bool m_isEnemyVisible;					// result of last visibility test on enemy
	unsigned char m_visibleEnemyParts;			// which parts of the visible enemy do we see
	Vector m_lastEnemyPosition;				// last place we saw the enemy
	float m_lastSawEnemyTimestamp;
	float m_firstSawEnemyTimestamp;
	float m_currentEnemyAcquireTimestamp;
	float m_enemyDeathTimestamp;				// if m_enemy is dead, this is when he died
	bool m_isLastEnemyDead;					// true if we killed or saw our last enemy die
	int m_nearbyEnemyCount;					// max number of enemies we've seen recently
	unsigned int m_enemyPlace;				// the location where we saw most of our enemies

	struct WatchInfo
	{
		float timestamp;
		bool isEnemy;
	}
	m_watchInfo[ MAX_CLIENTS ];
	mutable EHANDLE m_bomber;				// points to bomber if we can see him

	int m_nearbyFriendCount;				// number of nearby teammates
	mutable EHANDLE m_closestVisibleFriend;			// the closest friend we can see
	mutable EHANDLE m_closestVisibleHumanFriend;		// the closest human friend we can see

	CBasePlayer *m_attacker;				// last enemy that hurt us (may not be same as m_enemy)
	float m_attackedTimestamp;				// when we were hurt by the m_attacker

	int m_lastVictimID;					// the entindex of the last victim we killed, or zero
	bool m_isAimingAtEnemy;					// if true, we are trying to aim at our enemy
	bool m_isRapidFiring;					// if true, RunUpkeep() will toggle our primary attack as fast as it can
	IntervalTimer m_equipTimer;				// how long have we had our current weapon equipped
	bool DoEquip(CBasePlayerWeapon *gun);			// equip the given item

	void ReloadCheck(void);					// reload our weapon if we must
	void SilencerCheck(void);				// use silencer

	float m_fireWeaponTimestamp;

	// reaction time system
	enum { MAX_ENEMY_QUEUE = 20 };
	struct ReactionState
	{
		// NOTE: player position & orientation is not currently stored separately
		EHANDLE player;
		bool isReloading;
		bool isProtectedByShield;
	}
	m_enemyQueue[ MAX_ENEMY_QUEUE ];			// round-robin queue for simulating reaction times

	byte m_enemyQueueIndex;
	byte m_enemyQueueCount;
	byte m_enemyQueueAttendIndex;				// index of the timeframe we are "conscious" of

	CBasePlayer *FindMostDangerousThreat(void);		// return most dangerous threat in my field of view (feeds into reaction time queue)

	// stuck detection
	bool m_isStuck;
	float m_stuckTimestamp;					// time when we got stuck
	Vector m_stuckSpot;					// the location where we became stuck
	NavRelativeDirType m_wiggleDirection;
	float m_wiggleTimestamp;
	float m_stuckJumpTimestamp;				// time for next jump when stuck

	enum { MAX_VEL_SAMPLES = 5 };
	float m_avgVel[ MAX_VEL_SAMPLES ];
	int m_avgVelIndex;
	int m_avgVelCount;
	Vector m_lastOrigin;

	// chatter mechanism
	GameEventType m_lastRadioCommand;			// last radio command we recieved
	void RespondToRadioCommands(void);
	bool IsRadioCommand(GameEventType event) const;		// returns true if the radio message is an order to do something

	#define NO_FORCE false
	void EndVoiceFeedback(bool force = true);
	float m_lastRadioRecievedTimestamp;			// time we recieved a radio message
	float m_lastRadioSentTimestamp;				// time when we send a radio message
	EHANDLE m_radioSubject;					// who issued the radio message
	Vector m_radioPosition;					// position referred to in radio message
	float m_voiceFeedbackStartTimestamp;
	float m_voiceFeedbackEndTimestamp;			// new-style "voice" chatter gets voice feedback
	BotChatterInterface m_chatter;

	// learn map mechanism
	const CNavNode *m_navNodeList;
	CNavNode *m_currentNode;
	NavDirType m_generationDir;
	NavAreaList::iterator m_analyzeIter;
	
	enum ProcessType
	{
		PROCESS_NORMAL,
		PROCESS_LEARN,
		PROCESS_ANALYZE_ALPHA,
		PROCESS_ANALYZE_BETA,
		PROCESS_SAVE,
	}
	m_processMode;
	CountdownTimer m_mumbleTimer;
	CountdownTimer m_booTimer;
	CountdownTimer m_relocateTimer;

	CNavNode *AddNode(const Vector *destPos, const Vector *normal, NavDirType dir, CNavNode *source);
	void StartLearnProcess(void);
	void UpdateLearnProcess(void);
	bool LearnStep(void);
	void StartAnalyzeAlphaProcess(void);
	void UpdateAnalyzeAlphaProcess(void);
	bool AnalyzeAlphaStep(void);
	void StartAnalyzeBetaProcess(void);
	void UpdateAnalyzeBetaProcess(void);
	bool AnalyzeBetaStep(void);
	void StartSaveProcess(void);
	void UpdateSaveProcess(void);
	void StartNormalProcess(void);

};/* size: 11424, cachelines: 179, members: 161 */

// Inlines

/* <51934c> ../cstrike/dlls/bot/cs_bot.h:316 */
inline float CCSBot::GetCombatRange(void) const
{
	return m_combatRange;
}

/* <58de9b> ../cstrike/dlls/bot/cs_bot.h:318 */
inline void CCSBot::SetRogue(bool rogue)
{
	m_isRogue = rogue;
}

/* <519617> ../cstrike/dlls/bot/cs_bot.h:320 */
inline void CCSBot::Hurry(float duration)
{
	m_hurryTimer.Start(duration);
}

/* <5a0773> ../cstrike/dlls/bot/cs_bot.h:326 */
inline float CCSBot::GetSafeTime(void) const
{
	return m_safeTime;
}

/* <5c4b10> ../cstrike/dlls/bot/cs_bot.h:355 */
inline bool CCSBot::IsCarryingBomb(void) const
{
	return m_bHasC4;
}

/* <5a07aa> ../cstrike/dlls/bot/cs_bot.h:369 */
inline bool CCSBot::IsFollowing(void) const
{
	return m_isFollowing;
}

/* <57bf57> ../cstrike/dlls/bot/cs_bot.h:370 */
inline CBasePlayer *CCSBot::GetFollowLeader(void)
{
	return m_leader;
}

/* <3c5a20> ../cstrike/dlls/bot/cs_bot.h:375 */
inline float CCSBot::GetFollowDuration(void) const
{
	return gpGlobals->time - m_followTimestamp;
}

/* <3c5a39> ../cstrike/dlls/bot/cs_bot.h:376 */
inline bool CCSBot::CanAutoFollow(void) const
{
	return (gpGlobals->time > m_allowAutoFollowTime);
}

/* <3c5a52> ../cstrike/dlls/bot/cs_bot.h:381 */
inline void CCSBot::AimAtEnemy(void)
{
	m_isAimingAtEnemy = true;
}

/* <3c5a70> ../cstrike/dlls/bot/cs_bot.h:382 */
inline void CCSBot::StopAiming(void)
{
	m_isAimingAtEnemy = false;
}

/* <3c5a89> ../cstrike/dlls/bot/cs_bot.h:383 */
inline bool CCSBot::IsAimingAtEnemy(void) const
{
	return m_isAimingAtEnemy;
}

/* <3e9c5e> ../cstrike/dlls/bot/cs_bot.h:385 */
inline bool CCSBot::IsSurprised(void) const
{
	return gpGlobals->time - m_surpriseTimestamp < 5.0f;
}

/* <3ea00d> ../cstrike/dlls/bot/cs_bot.h:386 */
inline float CCSBot::GetSurpriseDelay(void) const
{
	return gpGlobals->time - IsSurprised() ? m_surpriseDelay : 0.0f;
}

/* <51938e> ../cstrike/dlls/bot/cs_bot.h:387 */
inline void CCSBot::ClearSurpriseDelay(void)
{
	m_surpriseDelay = 0.0f;
	m_surpriseTimestamp = 0.0f;
}

/* <5e2f12> ../cstrike/dlls/bot/cs_bot.h:389 */
inline float CCSBot::GetStateTimestamp(void) const
{
	return m_stateTimestamp;
}

/* <5c4c60> ../cstrike/dlls/bot/cs_bot.h:394 */
inline CSGameState *CCSBot::GetGameState(void)
{
	return &m_gameState;
}

/* <2e7d45> ../cstrike/dlls/bot/cs_bot.h:395 */
inline const CSGameState *CCSBot::GetGameState(void) const
{
	return &m_gameState;
}

/* <5c4ce8> ../cstrike/dlls/bot/cs_bot.h:397 */
inline bool CCSBot::IsAtBombsite(void)
{
	return (m_signals.GetState() & SIGNAL_BOMB) == SIGNAL_BOMB;
}

/* <5a07c3> ../cstrike/dlls/bot/cs_bot.h:453 */
inline CCSBot::MoraleType CCSBot::GetMorale(void) const
{
	return m_morale;
}

/* <5b2dba> ../cstrike/dlls/bot/cs_bot.h:459 */
inline bool CCSBot::IsNoiseHeard(void) const
{
	if (m_noiseTimestamp <= 0.0f)
		return false;

	// primitive reaction time simulation - cannot "hear" noise until reaction time has elapsed
	if (gpGlobals->time - m_noiseTimestamp >= GetProfile()->GetReactionTime())
		return true;

	return false;
}

/* <5d3f92> ../cstrike/dlls/bot/cs_bot.h:426 */
inline void CCSBot::SetTask(TaskType task, CBaseEntity *entity)
{
	m_task = task;
	m_taskEntity = entity;
}

/* <5e2f30> ../cstrike/dlls/bot/cs_bot.h:427 */
inline CCSBot::TaskType CCSBot::GetTask(void) const
{
	return m_task;
}

/* <5c4c42> ../cstrike/dlls/bot/cs_bot.h:428 */
inline CBaseEntity *CCSBot::GetTaskEntity(void)
{
	return m_taskEntity;
}

/* <5b2d33> ../cstrike/dlls/bot/cs_bot.h:474 */
inline CNavArea *CCSBot::GetNoiseArea(void) const
{
	return m_noiseArea;
}

/* <5b2d51> ../cstrike/dlls/bot/cs_bot.h:475 */
inline void CCSBot::ForgetNoise(void)
{
	m_noiseTimestamp = 0.0f;
}

/* <5b2d6f> ../cstrike/dlls/bot/cs_bot.h:487 */
inline PriorityType CCSBot::GetNoisePriority(void) const
{
	return m_noisePriority;
}

/* <5c4ccf> ../cstrike/dlls/bot/cs_bot.h:491 */
inline BotChatterInterface *CCSBot::GetChatter(void)
{
	return &m_chatter;
}

/* <111238> ../cstrike/dlls/bot/cs_bot.h:508 */
inline bool CCSBot::IsUsingVoice(void) const
{
	return (m_voiceFeedbackEndTimestamp != 0.0f);
}

/* <519493> ../cstrike/dlls/bot/cs_bot.h:513 */
inline CBasePlayer *CCSBot::GetEnemy(void)
{
	return m_enemy;
}

/* <57bfbd> ../cstrike/dlls/bot/cs_bot.h:514 */
inline int CCSBot::GetNearbyEnemyCount(void) const
{
	return Q_min(GetEnemiesRemaining(), m_nearbyEnemyCount);
}

/* <2fecad> ../cstrike/dlls/bot/cs_bot.h:515 */
inline unsigned int CCSBot::GetEnemyPlace(void) const
{
	return m_enemyPlace;
}

/* <2eff13> ../cstrike/dlls/bot/cs_bot.h:516 */
inline bool CCSBot::CanSeeBomber(void) const
{
	return (m_bomber == NULL) ? false : true;
}

/* <3c6110> ../cstrike/dlls/bot/cs_bot.h:517 */
inline CBasePlayer *CCSBot::GetBomber(void) const
{
	return m_bomber;
}

/* <2fecc8> ../cstrike/dlls/bot/cs_bot.h:519 */
inline int CCSBot::GetNearbyFriendCount(void) const
{
	return Q_min(GetFriendsRemaining(), m_nearbyFriendCount);
}

/* <3a138c> ../cstrike/dlls/bot/cs_bot.h:521 */
inline CBasePlayer *CCSBot::GetClosestVisibleFriend(void) const
{
	return m_closestVisibleFriend;
}

/* <3c615b> ../cstrike/dlls/bot/cs_bot.h:521 */
inline CBasePlayer *CCSBot::GetClosestVisibleHumanFriend(void) const
{
	return m_closestVisibleHumanFriend;
}

/* <57bfd6> ../cstrike/dlls/bot/cs_bot.h:536 */
inline float CCSBot::GetTimeSinceAttacked(void) const
{
	return gpGlobals->time - m_attackedTimestamp;
}

/* <56ae2c> ../cstrike/dlls/bot/cs_bot.h:537 */
inline float CCSBot::GetFirstSawEnemyTimestamp(void) const
{
	return m_firstSawEnemyTimestamp;
}

/* <58df07> ../cstrike/dlls/bot/cs_bot.h:538 */
inline float CCSBot::GetLastSawEnemyTimestamp(void) const
{
	return m_lastSawEnemyTimestamp;
}

/* <3e9ccc> ../cstrike/dlls/bot/cs_bot.h:539 */
inline float CCSBot::GetTimeSinceLastSawEnemy(void) const
{
	return gpGlobals->time - m_lastSawEnemyTimestamp;
}

/* <3e9ce5> ../cstrike/dlls/bot/cs_bot.h:540 */
inline float CCSBot::GetTimeSinceAcquiredCurrentEnemy(void) const
{
	return gpGlobals->time - m_currentEnemyAcquireTimestamp;
}

/* <5196d7> ../cstrike/dlls/bot/cs_bot.h:542 */
inline const Vector &CCSBot::GetLastKnownEnemyPosition(void) const
{
	return m_lastEnemyPosition;
}

/* <5e2f49> ../cstrike/dlls/bot/cs_bot.h:543 */
inline bool CCSBot::IsEnemyVisible(void) const
{
	return m_isEnemyVisible;
}

/* <519429> ../cstrike/dlls/bot/cs_bot.h:544 */
inline float CCSBot::GetEnemyDeathTimestamp(void) const
{
	return m_enemyDeathTimestamp;
}

/* <519442> ../cstrike/dlls/bot/cs_bot.h:547 */
inline int CCSBot::GetLastVictimID(void) const
{
	return m_lastVictimID;
}

/* <5b2da1> ../cstrike/dlls/bot/cs_bot.h:550 */
inline bool CCSBot::HasPath(void) const
{
	return m_pathLength != 0;
}

/* <5a07f5> ../cstrike/dlls/bot/cs_bot.h:551 */
inline void CCSBot::DestroyPath(void)
{
	m_pathLength = 0;
	m_pathLadder = NULL;
}

/* <5a0813> ../cstrike/dlls/bot/cs_bot.h:567 */
inline CNavArea *CCSBot::GetLastKnownArea(void) const
{
	return m_lastKnownArea;
}

/* <51963c> ../cstrike/dlls/bot/cs_bot.h:568 */
inline const Vector &CCSBot::GetPathEndpoint(void) const
{
	return m_path[ m_pathLength - 1 ].pos;
}

/* <5c1be3> ../cstrike/dlls/bot/cs_bot.h:571 */
inline const Vector &CCSBot::GetPathPosition(int numpath) const
{
	return m_path[ numpath ].pos;
}

/* <5c4b79> ../cstrike/dlls/bot/cs_bot.h:577 */
inline bool CCSBot::IsUsingLadder(void) const
{
	return m_pathLadder != NULL;
}

/* <5a0c27> ../cstrike/dlls/bot/cs_bot.h:580 */
inline void CCSBot::SetGoalEntity(CBaseEntity *entity)
{
	m_goalEntity = entity;
}

/* <5c4d01> ../cstrike/dlls/bot/cs_bot.h:581 */
inline CBaseEntity *CCSBot::GetGoalEntity(void)
{
	return m_goalEntity;
}

/* <5195f2> ../cstrike/dlls/bot/cs_bot.h:586 */
inline void CCSBot::ForceRun(float duration)
{
	Run();
	m_mustRunTimer.Start(duration);
}

/* <3d8238> ../cstrike/dlls/bot/cs_bot.h:598 */
inline void CCSBot::SetLookAngles(float yaw, float pitch)
{
	m_lookYaw = yaw;
	m_lookPitch = pitch;
}

inline void CCSBot::SetForwardAngle(float angle)
{
	m_forwardAngle = angle;
}

/* <57c008> ../cstrike/dlls/bot/cs_bot.h:609 */
inline void CCSBot::SetLookAheadAngle(float angle)
{
	m_lookAheadAngle = angle;
}

/* <5e2f62> ../cstrike/dlls/bot/cs_bot.h:612 */
inline void CCSBot::ClearLookAt(void)
{
	//PrintIfWatched("ClearLookAt()\n");
	m_lookAtSpotState = NOT_LOOKING_AT_SPOT;
	m_lookAtDesc = NULL;
}

/* <3d8287> ../cstrike/dlls/bot/cs_bot.h:618 */
inline bool CCSBot::IsLookingAtSpot(PriorityType pri) const
{
	if (m_lookAtSpotState != NOT_LOOKING_AT_SPOT && m_lookAtSpotPriority >= pri)
		return true;

	return false;
}

/* <3e9d59> ../cstrike/dlls/bot/cs_bot.h:625 */
inline bool CCSBot::IsViewMoving(float angleVelThreshold) const
{
	if (m_lookYawVel < angleVelThreshold && m_lookYawVel > -angleVelThreshold &&
		m_lookPitchVel < angleVelThreshold && m_lookPitchVel > -angleVelThreshold)
	{
		return false;
	}
	return true;
}

/* <57c047> ../cstrike/dlls/bot/cs_bot.h:651 */
inline void CCSBot::ClearApproachPoints(void)
{
	m_approachPointCount = 0;
}

/* <3e9d7e> ../cstrike/dlls/bot/cs_bot.h:674 */
inline bool CCSBot::IsThrowingGrenade(void) const
{
	return m_isWaitingToTossGrenade;
}

/* <3e9d97> ../cstrike/dlls/bot/cs_bot.h:679 */
inline void CCSBot::StartRapidFire(void)
{
	m_isRapidFiring = true;
}

/* <3e9db0> ../cstrike/dlls/bot/cs_bot.h:680 */
inline void CCSBot::StopRapidFire(void)
{
	m_isRapidFiring = false;
}

/* <51954d> ../cstrike/dlls/bot/cs_bot.h:684 */
inline CCSBot::ZoomType CCSBot::GetZoomLevel(void) const
{
	if (m_iFOV > 60.0f)
		return NO_ZOOM;

	if (m_iFOV > 25.0f)
		return LOW_ZOOM;

	return HIGH_ZOOM;
}

/* <5c4b92> ../cstrike/dlls/bot/cs_bot.h:698 */
inline int CCSBot::GetHostageEscortCount(void) const
{
	return m_hostageEscortCount;
}

/* <5e2f80> ../cstrike/dlls/bot/cs_bot.h:699 */
inline void CCSBot::IncreaseHostageEscortCount(void)
{
	++m_hostageEscortCount;
}

/* <5a0c4d> ../cstrike/dlls/bot/cs_bot.h:701 */
inline void CCSBot::ResetWaitForHostagePatience(void)
{
	m_isWaitingForHostage = false;
	m_inhibitWaitingForHostageTimer.Invalidate();
}

/* <5d402a> ../cstrike/dlls/bot/cs_bot.h:1055 */
inline float CCSBot::GetFeetZ(void) const
{
	if (IsCrouching())
	{
		const Vector crouch = Vector(0, 0, -StepHeight);
		return (pev->origin + crouch).z;
	}
	else
	{
		const Vector stand = Vector(0, 0, -HalfHumanHeight);
		return (pev->origin + stand).z;
	}
}

/* <5b2dd3> ../cstrike/dlls/bot/cs_bot.h:1063 */
inline const Vector *CCSBot::GetNoisePosition(void) const
{
	if (m_noiseTimestamp > 0.0f)
		return &m_noisePosition;

	return NULL;
}

/* <519655> ../cstrike/dlls/bot/cs_bot.h:1071 */
inline bool CCSBot::IsAwareOfEnemyDeath(void) const
{
	if (GetEnemyDeathTimestamp() == 0.0f)
		return false;

	if (m_enemy == NULL)
		return true;

	if (!m_enemy->IsAlive() && gpGlobals->time - GetEnemyDeathTimestamp() > (1.0f - GetProfile()->GetSkill()))
		return true;

	return false;
}

/* <568b12> ../cstrike/dlls/bot/cs_bot.h:1085 */
inline bool CCSBot::IsNotMoving(void) const
{
	const float stillSpeed = 10.0f;
	return pev->velocity.IsLengthLessThan(stillSpeed);
}

inline bool CCSBot::HasAnyAmmo(CBasePlayerWeapon *weapon) const
{
	return (weapon->m_iClip != 0 || m_rgAmmo[weapon->m_iPrimaryAmmoType] > 0);
}

/* <2e8465> ../cstrike/dlls/bot/cs_bot.cpp:1004 */
class CollectRetreatSpotsFunctor
{
public:
	CollectRetreatSpotsFunctor(CCSBot *me, float range)
	{
		m_me = me;
		m_count = 0;
		m_range = range;
	}

	enum { MAX_SPOTS = 256 };

	bool operator()(CNavArea *area)
	{
		// collect all the hiding spots in this area
		const HidingSpotList *list = area->GetHidingSpotList();

		for (HidingSpotList::const_iterator iter = list->begin(); iter != list->end(); ++iter)
		{
			const HidingSpot *spot = (*iter);

			if (m_count >= MAX_SPOTS)
				break;

			// make sure hiding spot is in range
			if (m_range > 0.0f)
			{
				if ((*spot->GetPosition() - m_me->pev->origin).IsLengthGreaterThan(m_range))
					continue;
			}

			// if a Player is using this hiding spot, don't consider it
			if (IsSpotOccupied(m_me, spot->GetPosition()))
			{
				// player is in hiding spot
				// TODO: Check if player is moving or sitting still
				continue;
			}

			// don't select spot if an enemy can see it
			if (UTIL_IsVisibleToTeam(*spot->GetPosition() + Vector(0, 0, HalfHumanHeight), OtherTeam(m_me->m_iTeam)))
				continue;

			// don't select spot if it is closest to an enemy
			CBasePlayer *owner = UTIL_GetClosestPlayer(spot->GetPosition());
			if (owner != NULL && m_me->m_iTeam != owner->m_iTeam)
				continue;

			m_spot[ m_count++ ] = spot->GetPosition();
		}

		// if we've filled up, stop searching
		if (m_count == MAX_SPOTS)
			return false;

		return true;
	}

	CCSBot *m_me;
	float m_range;

	const Vector *m_spot[ MAX_SPOTS ];
	int m_count;

};/* size: 1036, cachelines: 17, members: 4 */

/* <2e7f9f> ../cstrike/dlls/bot/cs_bot.cpp:1088 */
class FarthestHostage
{
public:
	FarthestHostage(const CCSBot *me)
	{
		m_me = me;
		m_farRange = -1.0f;
	}
	bool operator()(CHostage *hostage)
	{
		if (hostage->IsFollowing(m_me))
		{
			float range = (hostage->Center() - m_me->pev->origin).Length();

			if (range > m_farRange)
			{
				m_farRange = range;
			}
		}

		return true;

		/*if (hostage->pev->takedamage != DAMAGE_YES)
			return true;

		if (hostage->m_improv != NULL)
		{
			if (!hostage->IsFollowingSomeone() || m_me != hostage->GetLeader())
				return true;
		}
		else if (!hostage->IsFollowing(m_me))
			return true;

		float range = (hostage->Center() - m_me->pev->origin).Length();

		if (range > m_farRange)
		{
			m_farRange = range;
		}

		return true;*/
	}

	const CCSBot *m_me;
	float m_farRange;

};/* size: 8, cachelines: 1, members: 2 */

// Functor used with NavAreaBuildPath()

/* <5a0af3> ../cstrike/dlls/bot/cs_bot.h:1114 */
class PathCost
{
public:
	PathCost(CCSBot *bot, RouteType route = SAFEST_ROUTE)
	{
		m_bot = bot;
		m_route = route;
	}
	float operator()(CNavArea *area, CNavArea *fromArea, const CNavLadder *ladder)
	{
		const float baseDangerFactor = 100.0f;

		// respond to the danger modulated by our aggression (even super-aggressives pay SOME attention to danger)
		float dangerFactor = (1.0f - (0.95f * m_bot->GetProfile()->GetAggression())) * baseDangerFactor;

		if (fromArea == NULL)
		{
			if (m_route == FASTEST_ROUTE)
				return 0.0f;

			// first area in path, cost is just danger
			return dangerFactor * area->GetDanger(m_bot->m_iTeam - 1);
		}
		else if ((fromArea->GetAttributes() & NAV_JUMP) && (area->GetAttributes() & NAV_JUMP))
		{
			// cannot actually walk in jump areas - disallow moving from jump area to jump area
			return -1.0f;
		}
		else
		{
			// compute distance from previous area to this area
			float dist;
			if (ladder)
			{
				// ladders are slow to use
				const float ladderPenalty = 1.0f;
				dist = ladderPenalty * ladder->m_length;

				// if we are currently escorting hostages, avoid ladders (hostages are confused by them)
				//if (m_bot->GetHostageEscortCount())
				//	dist *= 100.0f;
			}
			else
			{
				dist = (*area->GetCenter() - *fromArea->GetCenter()).Length();
			}

			// compute distance travelled along path so far
			float cost = dist + fromArea->GetCostSoFar();

			// zombies ignore all path penalties
			if (cv_bot_zombie.value > 0.0f)
				return cost;

			// add cost of "jump down" pain unless we're jumping into water
			if (!area->IsConnected(fromArea, NUM_DIRECTIONS))
			{
				// this is a "jump down" (one way drop) transition - estimate damage we will take to traverse it
				float fallDistance = -fromArea->ComputeHeightChange(area);

				// if it's a drop-down ladder, estimate height from the bottom of the ladder to the lower area
				//if (ladder && ladder->m_bottom.z < fromArea->GetCenter()->z && ladder->m_bottom.z > area->GetCenter()->z)
				//{
				//	fallDistance = ladder->m_bottom.z - area->GetCenter()->z;
				//}
				
				float fallDamage = m_bot->GetApproximateFallDamage(fallDistance);

				if (fallDamage > 0.0f)
				{
					// if the fall would kill us, don't use it
					const float deathFallMargin = 10.0f;
					if (fallDamage + deathFallMargin >= m_bot->pev->health)
						return -1.0f;

					// if we need to get there in a hurry, ignore minor pain
					const float painTolerance = 15.0f * m_bot->GetProfile()->GetAggression() + 10.0f;
					if (m_route != FASTEST_ROUTE || fallDamage > painTolerance)
					{
						// cost is proportional to how much it hurts when we fall
						// 10 points - not a big deal, 50 points - ouch!
						cost += 100.0f * fallDamage * fallDamage;
					}
				}
			}

			// if this is a "crouch" area, add penalty
			if (area->GetAttributes() & NAV_CROUCH)
			{
				// these areas are very slow to move through
				float_precision crouchPenalty = (m_route == FASTEST_ROUTE) ? 20.0f : 5.0f;

				// avoid crouch areas if we are rescuing hostages
				if (m_bot->GetHostageEscortCount())
				{
					crouchPenalty *= 3.0f;
				}

				cost += crouchPenalty * dist;
			}

			// if this is a "jump" area, add penalty
			if (area->GetAttributes() & NAV_JUMP)
			{
				// jumping can slow you down
				//const float jumpPenalty = (m_route == FASTEST_ROUTE) ? 100.0f : 0.5f;
				const float jumpPenalty = 1.0f;
				cost += jumpPenalty * dist;
			}

			if (m_route == SAFEST_ROUTE)
			{
				// add in the danger of this path - danger is per unit length travelled
				cost += dist * dangerFactor * area->GetDanger(m_bot->m_iTeam - 1);
			}

			if (!m_bot->IsAttacking())
			{
				// add in cost of teammates in the way
				// approximate density of teammates based on area
				float size = (area->GetSizeX() + area->GetSizeY()) / 2.0f;

				// degenerate check
				if (size >= 1.0f)
				{
					// cost is proportional to the density of teammates in this area
					const float costPerFriendPerUnit = 50000.0f;
					cost += costPerFriendPerUnit * (float)area->GetPlayerCount(m_bot->m_iTeam, m_bot) / size;
				}
			}

			return cost;
		}

		return 0.0f;
	}

private:
	CCSBot *m_bot;
	RouteType m_route;

};/* size: 8, cachelines: 1, members: 2 */

/* <568fae> ../cstrike/dlls/bot/states/cs_bot_follow.cpp:95 */
class FollowTargetCollector
{
public:
	FollowTargetCollector(CBasePlayer *player)
	{
		m_player = player;
		m_forward.x = player->pev->velocity.x;
		m_forward.y = player->pev->velocity.y;

		float speed = m_forward.NormalizeInPlace();

		const float walkSpeed = 100.0f;
		if (speed < walkSpeed)
		{
			m_cutoff.x = player->pev->origin.x;
			m_cutoff.y = player->pev->origin.y;

			m_forward.x = 0.0f;
			m_forward.y = 0.0f;
		}
		else
		{
			const float k = 1.5f;
			float_precision trimSpeed = (speed < 200.0f) ? speed : 200.0f;

			m_cutoff.x = player->pev->origin.x + k * trimSpeed * m_forward.x;
			m_cutoff.y = player->pev->origin.y + k * trimSpeed * m_forward.y;
		}

		m_targetAreaCount = 0;
	}

	enum { MAX_TARGET_AREAS = 128 };

	/* <568dc3> ../cstrike/dlls/bot/states/cs_bot_follow.cpp:124 */
	bool operator()(CNavArea *area)
	{
		if (m_targetAreaCount >= MAX_TARGET_AREAS)
			return false;

		// only use two-way connections
		if (!area->GetParent() || area->IsConnected(area->GetParent(), NUM_DIRECTIONS))
		{
			if (m_forward.IsZero())
			{
				m_targetArea[ m_targetAreaCount++ ] = area;
			}
			else
			{
				// collect areas in the direction of the player's forward motion
				Vector2D to(((*area->GetCenter()).x - m_cutoff.x), (*area->GetCenter()).y - m_cutoff.y);
				to.NormalizeInPlace();

				//if (DotProduct(to, m_forward) > 0.7071f)
				if ((to.x * m_forward.x + to.y * m_forward.y) > 0.7071f)
					m_targetArea[ m_targetAreaCount++ ] = area;
			}
		}

		return (m_targetAreaCount < MAX_TARGET_AREAS);
	}

	CBasePlayer *m_player;
	Vector2D m_forward;
	Vector2D m_cutoff;
	CNavArea *m_targetArea[ MAX_TARGET_AREAS ];
	int m_targetAreaCount;

};/* size: 536, cachelines: 9, members: 5 */

#ifdef HOOK_GAMEDLL

typedef bool (CCSBot::*CS_IS_VISIBLE_VECTOR)(const Vector *, bool) const;
typedef bool (CCSBot::*CS_IS_VISIBLE_CBASEPLAYER)(CBasePlayer *, bool, unsigned char *) const;

typedef const Vector *(FIND_SPOT_CSSBOT)(CCSBot *, float);
typedef void (CCSBot::*HIDE_NAV_AREA)(CNavArea *, float, float, bool);
typedef void (CCSBot::*HIDE_VECTOR)(const Vector *, float, bool);

typedef const CSGameState *(CCSBot::*GETGAMESTATE_CONST)(void) const;
typedef CSGameState *(CCSBot::*GETGAMESTATE_NOTCONST)(void);

// NavAreaBuildPath<PathCost> hook
inline bool NavAreaBuildPath__PathCost__wrapper(CNavArea *startArea, CNavArea *goalArea, const Vector *goalPos, PathCost &costFunc, CNavArea **closestArea)
{
	return NavAreaBuildPath(startArea, goalArea, goalPos, costFunc, closestArea);
}

// NavAreaTravelDistance<PathCost> hook
inline float_precision NavAreaTravelDistance__PathCost__wrapper(CNavArea *startArea, CNavArea *endArea, PathCost &costFunc)
{
	return NavAreaTravelDistance(startArea, endArea, costFunc);
}

// refs
extern void (*pCCSBot__UpdateLookAngles)(void);

#endif // HOOK_GAMEDLL

void InstallBotControl(void);
void Bot_ServerCommand(void);
void Bot_RegisterCvars(void);
int GetBotFollowCount(CBasePlayer *leader);
const Vector *FindNearbyRetreatSpot(CCSBot *me, float maxRange);

void drawProgressMeter(float progress, char *title);
void startProgressMeter(const char *title);
void hideProgressMeter(void);

bool isSniperRifle(CBasePlayerItem *item);
float StayOnLadderLine(CCSBot *me, const CNavLadder *ladder);

#endif // CS_BOT_H
