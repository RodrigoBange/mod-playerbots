/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it
 * and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#include "EmoteAction.h"

#include "Event.h"
#include "PlayerbotTextMgr.h"
#include "Playerbots.h"
#include "ServerFacade.h"

#include <vector>
#include <cstdlib>

std::map<std::string, uint32> EmoteActionBase::emotes;
std::map<std::string, uint32> EmoteActionBase::textEmotes;
char* strstri(char const* haystack, char const* needle);

// Define functions
std::string GetRandomGreeting();
std::string GetRandomListeningResponse();
std::string GetRandomYawningResponse();
std::string GetRandomEnemyRoarResponse();
std::string GetRandomDanceResponse();
std::string GetRandomLoveResponse();
std::string GetRandomFlexResponse();
std::string GetRandomBlameResponse();
std::string GetRandomStinkyResponse();
std::string GetRandomJokeResponse();
std::string GetRandomChickenResponse();
std::string GetRandomApologyResponse();
std::string GetRandomHappyResponse();
std::string GetRandomPleadResponse();
std::string GetRandomScratchResponse();
std::string GetRandomBoredResponse();
std::string GetRandomSitResponse();
std::string GetRandomNodResponse();
std::string GetRandomCommendResponse();
std::string GetRandomFlopResponse();
std::string GetRandomBlinkResponse();
std::string GetRandomBarkResponse();
std::string GetRandomByeResponse();
std::string GetRandomSnickerResponse();
std::string GetRandomEyebrowResponse();
std::string GetRandomBreathResponse();
std::string GetRandomCryResponse();
std::string GetRandomCrackResponse();
std::string GetRandomDrinkResponse();
std::string GetRandomEmbarrassResponse();
std::string GetRandomHungryResponse();
std::string GetRandomWinkResponse();
std::string GetRandomDoubtResponse();
std::string GetRandomPanicResponse();
std::string GetRandomPointResponse();
std::string GetRandomRaspResponse();
std::string GetRandomThankResponse();
std::string GetRandomToastResponse();
std::string GetRandomThirstyResponse();
std::string GetRandomPatResponse();
std::string GetRandomInsultResponse();
std::string GetRandomRudeJokeResponse();
std::string GetRandomRaiseResponse();
std::string GetRandomReadyResponse();
std::string GetRandomShooResponse();
std::string GetRandomSlapResponse();
std::string GetRandomStandResponse();
std::string GetRandomTickleResponse();
std::string GetRandomViolinResponse();
std::string GetRandomGoodluckResponse();
std::string GetRandomThreateningResponse();

EmoteActionBase::EmoteActionBase(PlayerbotAI* botAI, std::string const name) : Action(botAI, name)
{
    if (emotes.empty())
        InitEmotes();
}

EmoteAction::EmoteAction(PlayerbotAI* botAI) : EmoteActionBase(botAI, "emote"), Qualified() {}

void EmoteActionBase::InitEmotes()
{
    emotes["dance"] = EMOTE_ONESHOT_DANCE;
    emotes["drown"] = EMOTE_ONESHOT_DROWN;
    emotes["land"] = EMOTE_ONESHOT_LAND;
    emotes["liftoff"] = EMOTE_ONESHOT_LIFTOFF;
    emotes["loot"] = EMOTE_ONESHOT_LOOT;
    emotes["no"] = EMOTE_ONESHOT_NO;
    emotes["roar"] = EMOTE_STATE_ROAR;
    emotes["salute"] = EMOTE_ONESHOT_SALUTE;
    emotes["stomp"] = EMOTE_ONESHOT_STOMP;
    emotes["train"] = EMOTE_ONESHOT_TRAIN;
    emotes["yes"] = EMOTE_ONESHOT_YES;
    emotes["applaud"] = EMOTE_ONESHOT_APPLAUD;
    emotes["beg"] = EMOTE_ONESHOT_BEG;
    emotes["bow"] = EMOTE_ONESHOT_BOW;
    emotes["cheer"] = EMOTE_ONESHOT_CHEER;
    emotes["chicken"] = EMOTE_ONESHOT_CHICKEN;
    emotes["cry"] = EMOTE_ONESHOT_CRY;
    emotes["dance"] = EMOTE_STATE_DANCE;
    emotes["eat"] = EMOTE_ONESHOT_EAT;
    emotes["exclamation"] = EMOTE_ONESHOT_EXCLAMATION;
    emotes["flex"] = EMOTE_ONESHOT_FLEX;
    emotes["kick"] = EMOTE_ONESHOT_KICK;
    emotes["kiss"] = EMOTE_ONESHOT_KISS;
    emotes["kneel"] = EMOTE_ONESHOT_KNEEL;
    emotes["laugh"] = EMOTE_ONESHOT_LAUGH;
    emotes["point"] = EMOTE_ONESHOT_POINT;
    emotes["question"] = EMOTE_ONESHOT_QUESTION;
    emotes["ready1h"] = EMOTE_ONESHOT_READY1H;
    emotes["roar"] = EMOTE_ONESHOT_ROAR;
    emotes["rude"] = EMOTE_ONESHOT_RUDE;
    emotes["shout"] = EMOTE_ONESHOT_SHOUT;
    emotes["shy"] = EMOTE_ONESHOT_SHY;
    emotes["sleep"] = EMOTE_STATE_SLEEP;
    emotes["talk"] = EMOTE_ONESHOT_TALK;
    emotes["wave"] = EMOTE_ONESHOT_WAVE;
    emotes["wound"] = EMOTE_ONESHOT_WOUND;

    textEmotes["bored"] = TEXT_EMOTE_BORED;
    textEmotes["bye"] = TEXT_EMOTE_BYE;
    textEmotes["cheer"] = TEXT_EMOTE_CHEER;
    textEmotes["congratulate"] = TEXT_EMOTE_CONGRATULATE;
    textEmotes["hello"] = TEXT_EMOTE_HELLO;
    textEmotes["no"] = TEXT_EMOTE_NO;
    textEmotes["nod"] = TEXT_EMOTE_NOD;  // yes
    textEmotes["sigh"] = TEXT_EMOTE_SIGH;
    textEmotes["thank"] = TEXT_EMOTE_THANK;
    textEmotes["welcome"] = TEXT_EMOTE_WELCOME;  // you are welcome
    textEmotes["whistle"] = TEXT_EMOTE_WHISTLE;
    textEmotes["yawn"] = TEXT_EMOTE_YAWN;
    textEmotes["oom"] = 323;
    textEmotes["follow"] = 324;
    textEmotes["wait"] = 325;
    textEmotes["healme"] = 326;
    textEmotes["openfire"] = 327;
    textEmotes["helpme"] = 303;
    textEmotes["flee"] = 306;
    textEmotes["danger"] = 304;
    textEmotes["charge"] = 305;
    textEmotes["help"] = 307;
    textEmotes["train"] = 264;
}

bool EmoteActionBase::Emote(Unit* target, uint32 type, bool textEmote)
{
    // 5% chance to emote
    if (urand(0, 99) >= 1) // 95% of the time, skip the emote
        return false;

    if (target && !bot->HasInArc(static_cast<float>(M_PI), target, sPlayerbotAIConfig->sightDistance))
        bot->SetFacingToObject(target);

    ObjectGuid oldSelection = bot->GetTarget();
    if (target)
    {
        bot->SetSelection(target->GetGUID());

        Player* player = dynamic_cast<Player*>(target);
        if (player)
        {
            PlayerbotAI* playerBotAI = GET_PLAYERBOT_AI(player);
            if (playerBotAI && !player->HasInArc(static_cast<float>(M_PI), bot, sPlayerbotAIConfig->sightDistance))
            {
                player->SetFacingToObject(bot);
            }
        }
    }

    if (textEmote)
    {
        WorldPacket data(SMSG_TEXT_EMOTE);
        data << type;
        data << GetNumberOfEmoteVariants((TextEmotes)type, bot->getRace(), bot->getGender());
        data << ((bot->GetTarget() && urand(0, 1)) ? bot->GetTarget() : ObjectGuid::Empty);
        bot->GetSession()->HandleTextEmoteOpcode(data);
    }
    else
        bot->HandleEmoteCommand(type);

    if (oldSelection)
        bot->SetTarget(oldSelection);

    return true;
}

Unit* EmoteActionBase::GetTarget()
{
    Unit* target = nullptr;

    GuidVector nfp = *context->GetValue<GuidVector>("nearest friendly players");
    std::vector<Unit*> targets;
    for (GuidVector::iterator i = nfp.begin(); i != nfp.end(); ++i)
    {
        Unit* unit = botAI->GetUnit(*i);
        if (unit && sServerFacade->GetDistance2d(bot, unit) < sPlayerbotAIConfig->tooCloseDistance)
            targets.push_back(unit);
    }

    if (!targets.empty())
        target = targets[urand(0, targets.size() - 1)];

    return target;
}

bool EmoteActionBase::ReceiveEmote(Player* source, uint32 emote, bool verbal)
{
    uint32 emoteId = 0;
    uint32 textEmote = 0;
    std::string emoteText;
    std::string emoteYell;

    switch (emote)
    {
        case TEXT_EMOTE_BONK:
            emoteId = EMOTE_ONESHOT_CRY;
            textEmote = TEXT_EMOTE_CRY;
            break;
        case TEXT_EMOTE_SALUTE:
            emoteId = EMOTE_ONESHOT_SALUTE;
            textEmote = TEXT_EMOTE_SALUTE;
            break;
        case 325:
            if (botAI->GetMaster() == source)
            {
                botAI->ChangeStrategy("-follow,+stay", BOT_STATE_NON_COMBAT);
                botAI->TellMasterNoFacing("Fine.. I'll stay right here..");
            }
            break;
        case TEXT_EMOTE_BECKON:
        case 324:
            if (botAI->GetMaster() == source)
            {
                botAI->ChangeStrategy("+follow", BOT_STATE_NON_COMBAT);
                botAI->TellMasterNoFacing("Wherever you go, I'll follow..");
            }
            break;
        case TEXT_EMOTE_WAVE:
        case TEXT_EMOTE_GREET:
        case TEXT_EMOTE_HAIL:
        case TEXT_EMOTE_HELLO:
        case TEXT_EMOTE_WELCOME:
        case TEXT_EMOTE_INTRODUCE:
            emoteText = GetRandomGreeting();
            emoteId = EMOTE_ONESHOT_WAVE;
            textEmote = TEXT_EMOTE_HELLO;
            break;
        case TEXT_EMOTE_DANCE:
            emoteText = GetRandomDanceResponse();
            emoteId = EMOTE_ONESHOT_DANCE;
            textEmote = TEXT_EMOTE_DANCE;
            break;
        case TEXT_EMOTE_FLIRT:
        case TEXT_EMOTE_KISS:
        case TEXT_EMOTE_HUG:
        case TEXT_EMOTE_BLUSH:
        case TEXT_EMOTE_SMILE:
        case TEXT_EMOTE_LOVE:
            // case TEXT_EMOTE_HOLDHAND:
            emoteText = GetRandomLoveResponse();
            emoteId = EMOTE_ONESHOT_SHY;
            textEmote = TEXT_EMOTE_SHY;
            break;
        case TEXT_EMOTE_FLEX:
            emoteText = GetRandomFlexResponse();
            emoteId = EMOTE_ONESHOT_APPLAUD;
            textEmote = TEXT_EMOTE_APPLAUD;
            break;
        case TEXT_EMOTE_ANGRY:
            // case TEXT_EMOTE_FACEPALM:
        case TEXT_EMOTE_GLARE:
        case TEXT_EMOTE_BLAME:
            // case TEXT_EMOTE_FAIL:
            // case TEXT_EMOTE_REGRET:
            // case TEXT_EMOTE_SCOLD:
            // case TEXT_EMOTE_CROSSARMS:
            emoteText = GetRandomBlameResponse();
            emoteId = EMOTE_ONESHOT_QUESTION;
            textEmote = TEXT_EMOTE_SHRUG;
            break;
        case TEXT_EMOTE_FART:
        case TEXT_EMOTE_BURP:
        case TEXT_EMOTE_GASP:
        case TEXT_EMOTE_NOSEPICK:
        case TEXT_EMOTE_SNIFF:
        case TEXT_EMOTE_STINK:
            emoteText = GetRandomStinkyResponse();
            emoteId = EMOTE_ONESHOT_POINT;
            textEmote = TEXT_EMOTE_POINT;
            break;
        case TEXT_EMOTE_JOKE:
            emoteId = EMOTE_ONESHOT_LAUGH;
            textEmote = TEXT_EMOTE_LAUGH;
            emoteText = GetRandomJokeResponse();
            break;
        case TEXT_EMOTE_CHICKEN:
            emoteText = GetRandomChickenResponse();
            emoteId = EMOTE_ONESHOT_RUDE;
            textEmote = TEXT_EMOTE_RUDE;
            break;
        case TEXT_EMOTE_APOLOGIZE:
            emoteId = EMOTE_ONESHOT_POINT;
            textEmote = TEXT_EMOTE_APOLOGIZE;
            emoteText = GetRandomApologyResponse();
            break;
        case TEXT_EMOTE_APPLAUD:
        case TEXT_EMOTE_CLAP:
        case TEXT_EMOTE_CONGRATULATE:
        case TEXT_EMOTE_HAPPY:
            // case TEXT_EMOTE_GOLFCLAP:
            emoteId = EMOTE_ONESHOT_BOW;
            textEmote = TEXT_EMOTE_BOW;
            emoteText = GetRandomHappyResponse();
            break;
        case TEXT_EMOTE_BEG:
        case TEXT_EMOTE_GROVEL:
        case TEXT_EMOTE_PLEAD:
            emoteId = EMOTE_ONESHOT_NO;
            textEmote = TEXT_EMOTE_NO;
            emoteText = GetRandomPleadResponse();
            break;
        case TEXT_EMOTE_BITE:
        case TEXT_EMOTE_POKE:
        case TEXT_EMOTE_SCRATCH:
            // case TEXT_EMOTE_PINCH:
            // case TEXT_EMOTE_PUNCH:
            emoteId = EMOTE_ONESHOT_ROAR;
            textEmote = TEXT_EMOTE_ROAR;
            emoteYell = GetRandomScratchResponse();
            break;
        case TEXT_EMOTE_BORED:
            emoteId = EMOTE_ONESHOT_NO;
            textEmote = TEXT_EMOTE_NO;
            emoteText = GetRandomBoredResponse();
            break;
        case TEXT_EMOTE_BOW:
        case TEXT_EMOTE_CURTSEY:
            emoteId = EMOTE_ONESHOT_BOW;
            textEmote = TEXT_EMOTE_BOW;
            break;
        case TEXT_EMOTE_BRB:
        case TEXT_EMOTE_SIT:
            emoteId = EMOTE_ONESHOT_EAT;
            textEmote = TEXT_EMOTE_EAT;
            emoteText = GetRandomSitResponse();
            break;
        case TEXT_EMOTE_AGREE:
        case TEXT_EMOTE_NOD:
            emoteId = EMOTE_ONESHOT_EXCLAMATION;
            textEmote = TEXT_EMOTE_NOD;
            emoteText = GetRandomNodResponse();
            break;
        case TEXT_EMOTE_AMAZE:
        case TEXT_EMOTE_COWER:
        case TEXT_EMOTE_CRINGE:
        case TEXT_EMOTE_EYE:
        case TEXT_EMOTE_KNEEL:
        case TEXT_EMOTE_PEER:
        case TEXT_EMOTE_SURRENDER:
        case TEXT_EMOTE_PRAISE:
        case TEXT_EMOTE_SCARED:
        case TEXT_EMOTE_COMMEND:
            // case TEXT_EMOTE_AWE:
            // case TEXT_EMOTE_JEALOUS:
            // case TEXT_EMOTE_PROUD:
            emoteId = EMOTE_ONESHOT_FLEX;
            textEmote = TEXT_EMOTE_FLEX;
            emoteText = GetRandomCommendResponse();
            break;
        case TEXT_EMOTE_BLEED:
        case TEXT_EMOTE_MOURN:
        case TEXT_EMOTE_FLOP:
            // case TEXT_EMOTE_FAINT:
            // case TEXT_EMOTE_PULSE:
            emoteId = EMOTE_ONESHOT_KNEEL;
            textEmote = TEXT_EMOTE_KNEEL;
            emoteText = GetRandomFlopResponse();
            break;
        case TEXT_EMOTE_BLINK:
            emoteId = EMOTE_ONESHOT_KICK;
            emoteText = GetRandomBlinkResponse();
            break;
        case TEXT_EMOTE_BOUNCE:
        case TEXT_EMOTE_BARK:
            emoteId = EMOTE_ONESHOT_POINT;
            textEmote = TEXT_EMOTE_POINT;
            emoteText = GetRandomBarkResponse();
            break;
        case TEXT_EMOTE_BYE:
            emoteId = EMOTE_ONESHOT_WAVE;
            textEmote = TEXT_EMOTE_WAVE;
            emoteText = GetRandomByeResponse();
            break;
        case TEXT_EMOTE_CACKLE:
        case TEXT_EMOTE_LAUGH:
        case TEXT_EMOTE_CHUCKLE:
        case TEXT_EMOTE_GIGGLE:
        case TEXT_EMOTE_GUFFAW:
        case TEXT_EMOTE_ROFL:
        case TEXT_EMOTE_SNICKER:
            // case TEXT_EMOTE_SNORT:
            emoteId = EMOTE_ONESHOT_LAUGH;
            textEmote = TEXT_EMOTE_LAUGH;
            emoteText = GetRandomSnickerResponse();
            break;
        case TEXT_EMOTE_CONFUSED:
        case TEXT_EMOTE_CURIOUS:
        case TEXT_EMOTE_FIDGET:
        case TEXT_EMOTE_FROWN:
        case TEXT_EMOTE_SHRUG:
        case TEXT_EMOTE_SIGH:
        case TEXT_EMOTE_STARE:
        case TEXT_EMOTE_TAP:
        case TEXT_EMOTE_SURPRISED:
        case TEXT_EMOTE_WHINE:
        case TEXT_EMOTE_BOGGLE:
        case TEXT_EMOTE_LOST:
        case TEXT_EMOTE_PONDER:
        case TEXT_EMOTE_SNUB:
        case TEXT_EMOTE_SERIOUS:
        case TEXT_EMOTE_EYEBROW:
            emoteId = EMOTE_ONESHOT_QUESTION;
            textEmote = TEXT_EMOTE_SHRUG;
            emoteText = GetRandomEyebrowResponse();
            break;
        case TEXT_EMOTE_COUGH:
        case TEXT_EMOTE_DROOL:
        case TEXT_EMOTE_SPIT:
        case TEXT_EMOTE_LICK:
        case TEXT_EMOTE_BREATH:
            // case TEXT_EMOTE_SNEEZE:
            // case TEXT_EMOTE_SWEAT:
            emoteId = EMOTE_ONESHOT_POINT;
            textEmote = TEXT_EMOTE_POINT;
            emoteText = GetRandomBreathResponse();
            break;
        case TEXT_EMOTE_CRY:
            emoteId = EMOTE_ONESHOT_CRY;
            textEmote = TEXT_EMOTE_CRY;
            emoteText = GetRandomCryResponse();
            break;
        case TEXT_EMOTE_CRACK:
            emoteId = EMOTE_ONESHOT_ROAR;
            textEmote = TEXT_EMOTE_ROAR;
            emoteText = GetRandomCrackResponse();
            break;
        case TEXT_EMOTE_EAT:
        case TEXT_EMOTE_DRINK:
            emoteId = EMOTE_ONESHOT_EAT;
            textEmote = TEXT_EMOTE_EAT;
            emoteText = GetRandomDrinkResponse();
            break;
        case TEXT_EMOTE_GLOAT:
        case TEXT_EMOTE_MOCK:
        case TEXT_EMOTE_TEASE:
        case TEXT_EMOTE_EMBARRASS:
            emoteId = EMOTE_ONESHOT_CRY;
            textEmote = TEXT_EMOTE_CRY;
            emoteText = GetRandomEmbarrassResponse();
            break;
        case TEXT_EMOTE_HUNGRY:
            emoteId = EMOTE_ONESHOT_EAT;
            textEmote = TEXT_EMOTE_EAT;
            emoteText = GetRandomHungryResponse();
            break;
        case TEXT_EMOTE_LAYDOWN:
        case TEXT_EMOTE_TIRED:
        case TEXT_EMOTE_YAWN:
            emoteId = EMOTE_ONESHOT_KNEEL;
            textEmote = TEXT_EMOTE_KNEEL;
            emoteText = GetRandomYawningResponse();
            break;
        case TEXT_EMOTE_MOAN:
        case TEXT_EMOTE_MOON:
        case TEXT_EMOTE_SEXY:
        case TEXT_EMOTE_SHAKE:
        case TEXT_EMOTE_WHISTLE:
        case TEXT_EMOTE_CUDDLE:
        case TEXT_EMOTE_PURR:
        case TEXT_EMOTE_SHIMMY:
        case TEXT_EMOTE_SMIRK:
        case TEXT_EMOTE_WINK:
            // case TEXT_EMOTE_CHARM:
            emoteId = EMOTE_ONESHOT_NO;
            textEmote = TEXT_EMOTE_NO;
            emoteText = GetRandomWinkResponse();
            break;
        case TEXT_EMOTE_NO:
        case TEXT_EMOTE_VETO:
        case TEXT_EMOTE_DISAGREE:
        case TEXT_EMOTE_DOUBT:
            emoteId = EMOTE_ONESHOT_QUESTION;
            textEmote = TEXT_EMOTE_SHRUG;
            emoteText = GetRandomDoubtResponse();
            break;
        case TEXT_EMOTE_PANIC:
            emoteId = EMOTE_ONESHOT_EXCLAMATION;
            textEmote = TEXT_EMOTE_CALM;
            emoteText = GetRandomPanicResponse();
            break;
        case TEXT_EMOTE_POINT:
            emoteId = EMOTE_ONESHOT_POINT;
            textEmote = TEXT_EMOTE_POINT;
            emoteText = GetRandomPointResponse();
            break;
        case TEXT_EMOTE_RUDE:
        case TEXT_EMOTE_RASP:
            emoteId = EMOTE_ONESHOT_RUDE;
            textEmote = TEXT_EMOTE_RASP;
            emoteText = GetRandomRaspResponse();  // , LANG_UNIVERSAL;
            break;
        case TEXT_EMOTE_ROAR:
        case TEXT_EMOTE_THREATEN:
        case TEXT_EMOTE_CALM:
        case TEXT_EMOTE_DUCK:
        case TEXT_EMOTE_TAUNT:
        case TEXT_EMOTE_PITY:
        case TEXT_EMOTE_GROWL:
            // case TEXT_EMOTE_TRAIN:
            // case TEXT_EMOTE_INCOMING:
            // case TEXT_EMOTE_CHARGE:
            // case TEXT_EMOTE_FLEE:
            // case TEXT_EMOTE_ATTACKMYTARGET:
        case TEXT_EMOTE_OPENFIRE:
        case TEXT_EMOTE_ENCOURAGE:
        case TEXT_EMOTE_ENEMY:
            // case TEXT_EMOTE_CHALLENGE:
            // case TEXT_EMOTE_REVENGE:
            // case TEXT_EMOTE_SHAKEFIST:
            emoteId = EMOTE_ONESHOT_ROAR;
            textEmote = TEXT_EMOTE_ROAR;
            emoteYell = GetRandomEnemyRoarResponse();
            break;
        case TEXT_EMOTE_TALK:
            emoteId = EMOTE_ONESHOT_TALK;
            textEmote = TEXT_EMOTE_LISTEN;
            break;
        case TEXT_EMOTE_TALKEX:
            emoteId = EMOTE_ONESHOT_YES;
            textEmote = TEXT_EMOTE_AGREE;
            break;
        case TEXT_EMOTE_TALKQ:
        case TEXT_EMOTE_LISTEN:
            emoteId = EMOTE_ONESHOT_TALK;
            textEmote = TEXT_EMOTE_TALKQ;
            emoteText = GetRandomListeningResponse();
            break;
        case TEXT_EMOTE_THANK:
            emoteId = EMOTE_ONESHOT_BOW;
            textEmote = TEXT_EMOTE_BOW;
            emoteText = GetRandomThankResponse();
            break;
        case TEXT_EMOTE_VICTORY:
        case TEXT_EMOTE_CHEER:
        case TEXT_EMOTE_TOAST:
            // case TEXT_EMOTE_HIGHFIVE:
            // case TEXT_EMOTE_DING:
            emoteId = EMOTE_ONESHOT_CHEER;
            textEmote = TEXT_EMOTE_CHEER;
            emoteText = GetRandomToastResponse();
            break;
        case TEXT_EMOTE_COLD:
        case TEXT_EMOTE_SHIVER:
        case TEXT_EMOTE_THIRSTY:
            // case TEXT_EMOTE_OOM:
            // case TEXT_EMOTE_HEALME:
            // case TEXT_EMOTE_POUT:
            emoteId = EMOTE_ONESHOT_QUESTION;
            textEmote = TEXT_EMOTE_PUZZLE;
            emoteText = GetRandomThirstyResponse();
            break;
        case TEXT_EMOTE_COMFORT:
        case TEXT_EMOTE_SOOTHE:
        case TEXT_EMOTE_PAT:
            emoteId = EMOTE_ONESHOT_CRY;
            textEmote = TEXT_EMOTE_CRY;
            emoteText = GetRandomPatResponse();
            break;
        case TEXT_EMOTE_INSULT:
            emoteId = EMOTE_ONESHOT_CRY;
            textEmote = TEXT_EMOTE_CRY;
            emoteText = GetRandomInsultResponse();
            break;
        case TEXT_EMOTE_JK:
            emoteId = EMOTE_ONESHOT_POINT;
            textEmote = TEXT_EMOTE_POINT;
            emoteText = GetRandomRudeJokeResponse();
            break;
        case TEXT_EMOTE_RAISE:
            emoteId = EMOTE_ONESHOT_POINT;
            textEmote = TEXT_EMOTE_POINT;
            emoteText = GetRandomRaiseResponse();
            break;
        case TEXT_EMOTE_READY:
            emoteId = EMOTE_ONESHOT_SALUTE;
            textEmote = TEXT_EMOTE_SALUTE;
            emoteText = GetRandomReadyResponse();
            break;
        case TEXT_EMOTE_SHOO:
            emoteId = EMOTE_ONESHOT_KICK;
            textEmote = TEXT_EMOTE_SHOO;
            emoteText = GetRandomShooResponse();
            break;
        case TEXT_EMOTE_SLAP:
            // case TEXT_EMOTE_SMACK:
            emoteId = EMOTE_ONESHOT_CRY;
            textEmote = TEXT_EMOTE_CRY;
            emoteText = GetRandomSlapResponse();
            break;
        case TEXT_EMOTE_STAND:
            emoteId = EMOTE_ONESHOT_NONE;
            textEmote = TEXT_EMOTE_STAND;
            emoteText = GetRandomStandResponse();
            break;
        case TEXT_EMOTE_TICKLE:
            emoteId = EMOTE_ONESHOT_LAUGH;
            textEmote = TEXT_EMOTE_GIGGLE;
            emoteText = GetRandomTickleResponse();
            break;
        case TEXT_EMOTE_VIOLIN:
            emoteId = EMOTE_ONESHOT_TALK;
            textEmote = TEXT_EMOTE_SIGH;
            emoteText = GetRandomViolinResponse();
            break;
            // case TEXT_EMOTE_HELPME:
            //     bot->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
            //     bot->Yell("Quick! Someone HELP!", LANG_UNIVERSAL);
            //     break;
        case TEXT_EMOTE_GOODLUCK:
            // case TEXT_EMOTE_LUCK:
            emoteId = EMOTE_ONESHOT_TALK;
            textEmote = TEXT_EMOTE_THANK;
            emoteText = GetRandomGoodluckResponse();
            break;
        case TEXT_EMOTE_BRANDISH:
            // case TEXT_EMOTE_MERCY:
            emoteId = EMOTE_ONESHOT_BEG;
            textEmote = TEXT_EMOTE_BEG;
            emoteText = GetRandomThreateningResponse();
            break;
        /*case TEXT_EMOTE_BADFEELING:
            bot->HandleEmoteCommand(EMOTE_ONESHOT_QUESTION);
            bot->Say("I'm just waiting for the ominous music now...", LANG_UNIVERSAL);
            break;
        case TEXT_EMOTE_MAP:
            bot->HandleEmoteCommand(EMOTE_ONESHOT_NO);
            bot->Say("Noooooooo.. you just couldn't ask for directions, huh?", LANG_UNIVERSAL);
            break;
        case TEXT_EMOTE_IDEA:
        case TEXT_EMOTE_THINK:
            bot->HandleEmoteCommand(EMOTE_ONESHOT_NO);
            bot->Say("Oh boy.. another genius idea...", LANG_UNIVERSAL);
            break;
        case TEXT_EMOTE_OFFER:
            bot->HandleEmoteCommand(EMOTE_ONESHOT_NO);
            bot->Say("No thanks.. I had some back at the last village", LANG_UNIVERSAL);
            break;
        case TEXT_EMOTE_PET:
            bot->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
            bot->Say("Do I look like a dog to you?!", LANG_UNIVERSAL);
            break;
        case TEXT_EMOTE_ROLLEYES:
            bot->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
            bot->Say("Keep doing that and I'll roll those eyes right out of your head..", LANG_UNIVERSAL);
            break;
        case TEXT_EMOTE_SING:
            bot->HandleEmoteCommand(EMOTE_ONESHOT_APPLAUD);
            bot->Say("Lovely... just lovely..", LANG_UNIVERSAL);
            break;
        case TEXT_EMOTE_COVEREARS:
            bot->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
            bot->Yell("You think that's going to help you?!", LANG_UNIVERSAL);
            break;*/
        default:
            // return false;
            // bot->HandleEmoteCommand(EMOTE_ONESHOT_QUESTION);
            // bot->Say("Mmmmmkaaaaaay...", LANG_UNIVERSAL);
            break;
    }

    if (source && !bot->isMoving() && !bot->HasInArc(static_cast<float>(M_PI), source, sPlayerbotAIConfig->farDistance))
        sServerFacade->SetFacingTo(bot, source);

    if (verbal)
    {
        if (emoteText.size())
            bot->Say(emoteText, (bot->GetTeamId() == TEAM_ALLIANCE ? LANG_COMMON : LANG_ORCISH));

        if (emoteYell.size())
            bot->Yell(emoteYell, (bot->GetTeamId() == TEAM_ALLIANCE ? LANG_COMMON : LANG_ORCISH));
    }

    if (textEmote)
    {
        WorldPacket data(SMSG_TEXT_EMOTE);
        data << textEmote;
        data << GetNumberOfEmoteVariants((TextEmotes)textEmote, bot->getRace(), bot->getGender());
        data << ((source && urand(0, 1)) ? source->GetGUID() : ObjectGuid::Empty);
        bot->GetSession()->HandleTextEmoteOpcode(data);
    }
    else
    {
        if (emoteId)
            bot->HandleEmoteCommand(emoteId);
    }

    return true;
}

bool EmoteAction::Execute(Event event)
{
    WorldPacket p(event.getPacket());
    uint32 emote = 0;

    Player* pSource = nullptr;
    bool isReact = false;
    if (!p.empty() && p.GetOpcode() == SMSG_TEXT_EMOTE)
    {
        isReact = true;
        ObjectGuid source;
        uint32 text_emote;
        uint32 emote_num;
        uint32 namlen;
        std::string nam;
        p.rpos(0);
        p >> source >> text_emote >> emote_num >> namlen;
        if (namlen > 1)
            p >> nam;

        pSource = ObjectAccessor::FindPlayer(source);
        if (pSource && (pSource->GetGUID() != bot->GetGUID()) &&
            ((urand(0, 1) && bot->HasInArc(static_cast<float>(M_PI), pSource, 10.0f)) ||
             (namlen > 1 && strstri(bot->GetName().c_str(), nam.c_str()))))
        {
            /*LOG_INFO("playerbots", "Bot {} {}:{} <{}> received SMSG_TEXT_EMOTE {} from player {} <{}>",
                bot->GetGUID().ToString().c_str(), bot->GetTeamId() == TEAM_ALLIANCE ? "A" : "H", bot->GetLevel(),
                bot->GetName(), text_emote, pSource->GetGUID().ToString().c_str(), pSource->GetName());*/

            emote = text_emote;
        }
    }

    if (!p.empty() && p.GetOpcode() == SMSG_EMOTE)
    {
        isReact = true;
        ObjectGuid source;
        uint32 emoteId;
        p.rpos(0);
        p >> emoteId >> source;

        pSource = ObjectAccessor::FindPlayer(source);
        if (pSource && pSource != bot && sServerFacade->GetDistance2d(bot, pSource) < sPlayerbotAIConfig->farDistance &&
            emoteId != EMOTE_ONESHOT_NONE)
        {
            if ((pSource->GetGUID() != bot->GetGUID()) &&
                (pSource->GetTarget() == bot->GetGUID() ||
                 (urand(0, 1) && bot->HasInArc(static_cast<float>(M_PI), pSource, 10.0f))))
            {
                /*LOG_INFO("playerbots", "Bot {} {}:{} <{}> received SMSG_EMOTE {} from player {} <{}>",
                    bot->GetGUID().ToString().c_str(), bot->GetTeamId() == TEAM_ALLIANCE ? "A" : "H", bot->GetLevel(),
                   bot->GetName(), emoteId, pSource->GetGUID().ToString().c_str(), pSource->GetName());*/

                std::vector<uint32> types;
                for (int32 i = sEmotesTextStore.GetNumRows(); i >= 0; --i)
                {
                    EmotesTextEntry const* em = sEmotesTextStore.LookupEntry(uint32(i));
                    if (!em)
                        continue;

                    if (em->textid == EMOTE_ONESHOT_TALK)
                        continue;

                    if (em->textid == EMOTE_ONESHOT_QUESTION)
                        continue;

                    if (em->textid == EMOTE_ONESHOT_EXCLAMATION)
                        continue;

                    if (em->textid == emoteId)
                    {
                        types.push_back(em->Id);
                    }
                }

                if (types.size())
                    emote = types[urand(0, types.size() - 1)];
            }
        }
    }

    if (isReact && !emote)
        return false;

    std::string param = event.getParam();
    if ((!isReact && param.empty()) || emote)
    {
        // time_t lastEmote = AI_VALUE2(time_t, "last emote", qualifier); //not used, line marked for removal.
        botAI->GetAiObjectContext()
            ->GetValue<time_t>("last emote", qualifier)
            ->Set(time(nullptr) + urand(1000, sPlayerbotAIConfig->repeatDelay) / 1000);
        param = qualifier;
    }

    if (emote)
        return ReceiveEmote(pSource, emote, bot->InBattleground() ? false : urand(0, 1));

    if (param.find("sound") == 0)
    {
        return botAI->PlaySound(atoi(param.substr(5).c_str()));
    }

    if (!param.empty() && textEmotes.find(param) != textEmotes.end())
    {
        WorldPacket data(SMSG_TEXT_EMOTE);
        data << textEmotes[param];
        data << GetNumberOfEmoteVariants((TextEmotes)textEmotes[param], bot->getRace(), bot->getGender());
        data << ((bot->GetTarget() && urand(0, 1)) ? bot->GetTarget() : ObjectGuid::Empty);
        bot->GetSession()->HandleTextEmoteOpcode(data);
        return true;
    }

    if (param.empty() || emotes.find(param) == emotes.end())
    {
        uint32 index = rand() % emotes.size();
        for (std::map<std::string, uint32>::iterator i = emotes.begin(); i != emotes.end() && index; ++i, --index)
            emote = i->second;
    }
    else
    {
        emote = emotes[param];
    }

    if (param.find("text") == 0)
    {
        emote = atoi(param.substr(4).c_str());
    }

    return Emote(GetTarget(), emote);
}

bool EmoteAction::isUseful()
{
    if (!botAI->AllowActivity())
        return false;

    time_t lastEmote = AI_VALUE2(time_t, "last emote", qualifier);
    return time(nullptr) >= lastEmote;
}

bool TalkAction::Execute(Event event)
{
    Unit* target = botAI->GetUnit(AI_VALUE(ObjectGuid, "talk target"));
    if (!target)
        target = GetTarget();

    if (!urand(0, 100))
    {
        target = nullptr;
        context->GetValue<ObjectGuid>("talk target")->Set(ObjectGuid::Empty);
        return true;
    }

    if (target)
    {
        if (Player* player = dynamic_cast<Player*>(target))
            if (PlayerbotAI* playerBotAI = GET_PLAYERBOT_AI(player))
                playerBotAI->GetAiObjectContext()->GetValue<ObjectGuid>("talk target")->Set(bot->GetGUID());

        context->GetValue<ObjectGuid>("talk target")->Set(target->GetGUID());
        return Emote(target, GetRandomEmote(target, true), true);
    }

    return false;
}

uint32 TalkAction::GetRandomEmote(Unit* unit, bool textEmote)
{
    std::vector<uint32> types;

    if (textEmote)
    {
        if (!urand(0, 20)) // Low probability: expressions
        {
            // Text-based expressions
            types.push_back(TEXT_EMOTE_BOW);
            types.push_back(TEXT_EMOTE_RUDE);
            types.push_back(TEXT_EMOTE_CRY);
            types.push_back(TEXT_EMOTE_LAUGH);
            types.push_back(TEXT_EMOTE_POINT);
            types.push_back(TEXT_EMOTE_CHEER);
            types.push_back(TEXT_EMOTE_SHY);
            types.push_back(TEXT_EMOTE_JOKE);
            types.push_back(TEXT_EMOTE_FLEX);
            types.push_back(TEXT_EMOTE_DANCE);
            types.push_back(TEXT_EMOTE_BLUSH);
            types.push_back(TEXT_EMOTE_FLIRT);
            types.push_back(TEXT_EMOTE_WAVE);
            types.push_back(TEXT_EMOTE_NOD);
            types.push_back(TEXT_EMOTE_WELCOME);
            types.push_back(TEXT_EMOTE_APPLAUD);
            types.push_back(TEXT_EMOTE_HUG);
            types.push_back(TEXT_EMOTE_SIGH);
        }
        else // Default: talk
        {
            types.push_back(TEXT_EMOTE_TALK);
            types.push_back(TEXT_EMOTE_TALKEX);
            types.push_back(TEXT_EMOTE_TALKQ);

            if (unit && (unit->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER) ||
                         unit->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER)))
            {
                types.push_back(TEXT_EMOTE_SALUTE);
            }
        }
        return types[urand(0, types.size() - 1)];
    }

    if (!urand(0, 20)) // Low probability: expressions
    {
        // Animation-based expressions
        types.push_back(EMOTE_ONESHOT_BOW);
        types.push_back(EMOTE_ONESHOT_RUDE);
        types.push_back(EMOTE_ONESHOT_CRY);
        types.push_back(EMOTE_ONESHOT_LAUGH);
        types.push_back(EMOTE_ONESHOT_POINT);
        types.push_back(EMOTE_ONESHOT_CHEER);
        types.push_back(EMOTE_ONESHOT_SHY);
        types.push_back(EMOTE_ONESHOT_APPLAUD);
        types.push_back(EMOTE_ONESHOT_DANCE);
        types.push_back(EMOTE_ONESHOT_FLEX);
        types.push_back(EMOTE_ONESHOT_SALUTE);
        types.push_back(EMOTE_ONESHOT_WAVE);
        types.push_back(EMOTE_ONESHOT_ROAR);
        types.push_back(EMOTE_ONESHOT_KICK);
        types.push_back(EMOTE_ONESHOT_EXCLAMATION);
        types.push_back(EMOTE_ONESHOT_QUESTION);
    }
    else // Default: talk
    {
        types.push_back(EMOTE_ONESHOT_TALK);
        types.push_back(EMOTE_ONESHOT_EXCLAMATION);
        types.push_back(EMOTE_ONESHOT_QUESTION);

        if (unit && (unit->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER) ||
                     unit->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER)))
        {
            types.push_back(EMOTE_ONESHOT_SALUTE);
        }
    }

    return types[urand(0, types.size() - 1)];
}


uint32 EmoteActionBase::GetNumberOfEmoteVariants(TextEmotes emote, uint8 Race, uint8 Gender)
{
    if (emote == 304)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_DWARF:
            case RACE_NIGHTELF:
            case RACE_UNDEAD_PLAYER:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 2;
            }
            case RACE_GNOME:
            {
                if (Gender == GENDER_MALE)
                    return 1;

                return 1;
            }
            case RACE_ORC:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 2;
            }
            case RACE_TAUREN:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 3;
            }
        }
    }
    else if (emote == 305)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_DWARF:
            case RACE_UNDEAD_PLAYER:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 2;
            }
            case RACE_NIGHTELF:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 3;
            }
            case RACE_GNOME:
            case RACE_TAUREN:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 2;
            }
            case RACE_ORC:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 3;
            }
        }
    }
    else if (emote == 306)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_NIGHTELF:
            case RACE_ORC:
            case RACE_UNDEAD_PLAYER:
            case RACE_TAUREN:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 2;
            }
            case RACE_DWARF:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 2;
            }
            case RACE_GNOME:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 3;
            }
        }
    }
    else if (emote == TEXT_EMOTE_HELLO)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_DWARF:
            case RACE_GNOME:
            {
                if (Gender == GENDER_MALE)
                    return 4;

                return 3;
            }
            case RACE_NIGHTELF:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 4;
            }
            case RACE_ORC:
            case RACE_UNDEAD_PLAYER:
            case RACE_TAUREN:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 3;
            }
        }
    }
    else if (emote == 323)
    {
        return 2;
    }
    else if (emote == 324)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_NIGHTELF:
            case RACE_ORC:
            case RACE_UNDEAD_PLAYER:
            case RACE_TAUREN:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 2;
            }
            case RACE_DWARF:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 2;
            }
            case RACE_GNOME:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 1;
            }
        }
    }
    else if (emote == 325)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 3;
            }
            case RACE_DWARF:
            case RACE_TAUREN:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 2;
            }
            case RACE_NIGHTELF:
            case RACE_GNOME:
            case RACE_ORC:
            case RACE_UNDEAD_PLAYER:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 2;
            }
        }
    }
    else if (emote == 326)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_NIGHTELF:
            case RACE_GNOME:
            case RACE_ORC:
            case RACE_UNDEAD_PLAYER:
            case RACE_TAUREN:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 2;
            }
            case RACE_DWARF:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 3;
            }
        }
    }
    else if (emote == TEXT_EMOTE_CHEER)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_NIGHTELF:
            case RACE_GNOME:
            case RACE_ORC:
            case RACE_UNDEAD_PLAYER:
            case RACE_TAUREN:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 2;
            }
            case RACE_DWARF:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 2;
            }
        }
    }
    else if (emote == TEXT_EMOTE_OPENFIRE)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_DWARF:
            case RACE_NIGHTELF:
            case RACE_UNDEAD_PLAYER:
            case RACE_TAUREN:
            case RACE_TROLL:
            case RACE_GNOME:
            {
                return 2;
            }
            case RACE_ORC:
            {
                if (Gender == GENDER_MALE)
                    return 2;

                return 3;
            }
        }
    }
    else if (emote == TEXT_EMOTE_BYE)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_DWARF:
            case RACE_NIGHTELF:
            case RACE_ORC:
            case RACE_UNDEAD_PLAYER:
            case RACE_TAUREN:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 3;
            }
            case RACE_GNOME:
            {
                if (Gender == GENDER_MALE)
                    return 4;

                return 4;
            }
        }
    }
    else if (emote == TEXT_EMOTE_NOD)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_NIGHTELF:
            case RACE_GNOME:
            case RACE_UNDEAD_PLAYER:
            case RACE_TAUREN:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 3;
            }
            case RACE_DWARF:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 4;

                return 3;
            }
            case RACE_ORC:
            {
                if (Gender == GENDER_MALE)
                    return 4;

                return 4;
            }
        }
    }
    else if (emote == TEXT_EMOTE_NO)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_DWARF:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 4;

                return 3;
            }
            case RACE_NIGHTELF:
            case RACE_GNOME:
            case RACE_ORC:
            case RACE_UNDEAD_PLAYER:
            case RACE_TAUREN:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 3;
            }
        }
    }
    else if (emote == TEXT_EMOTE_THANK)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_NIGHTELF:
            case RACE_GNOME:
            case RACE_ORC:
            case RACE_UNDEAD_PLAYER:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 3;
            }
            case RACE_DWARF:
            {
                if (Gender == GENDER_MALE)
                    return 4;

                return 4;
            }
            case RACE_TAUREN:
            {
                if (Gender == GENDER_MALE)
                    return 4;

                return 3;
            }
        }
    }
    else if (emote == TEXT_EMOTE_WELCOME)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_DWARF:
            case RACE_NIGHTELF:
            case RACE_GNOME:
            case RACE_ORC:
            case RACE_TAUREN:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 3;
                return 3;
            }
            case RACE_UNDEAD_PLAYER:
            {
                if (Gender == GENDER_MALE)
                    return 2;
                return 3;
            }
        }
    }
    else if (emote == TEXT_EMOTE_CONGRATULATE)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            case RACE_NIGHTELF:
            case RACE_ORC:
            case RACE_TAUREN:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 3;
            }
            case RACE_DWARF:
            {
                if (Gender == GENDER_MALE)
                    return 5;

                return 4;
            }
            case RACE_GNOME:
            case RACE_UNDEAD_PLAYER:
            {
                if (Gender == GENDER_MALE)
                    return 3;

                return 4;
            }
        }
    }
    else if (emote == TEXT_EMOTE_FLIRT)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            {
                if (Gender == GENDER_MALE)
                    return 6;
                return 3;
            }
            case RACE_DWARF:
            case RACE_TAUREN:
            {
                if (Gender == GENDER_MALE)
                    return 6;

                return 5;
            }
            case RACE_NIGHTELF:
            {
                if (Gender == GENDER_MALE)
                    return 5;

                return 4;
            }
            case RACE_GNOME:
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 4;

                return 5;
            }
            case RACE_ORC:
            case RACE_UNDEAD_PLAYER:
            {
                if (Gender == GENDER_MALE)
                    return 6;

                return 6;
            }
        }
    }
    else if (emote == TEXT_EMOTE_JOKE)
    {
        switch (Race)
        {
            case RACE_HUMAN:
            {
                if (Gender == GENDER_MALE)
                    return 5;

                return 6;
            }
            case RACE_DWARF:
            {
                if (Gender == GENDER_MALE)
                    return 6;

                return 5;
            }
            case RACE_NIGHTELF:
            {
                if (Gender == GENDER_MALE)
                    return 7;

                return 4;
            }
            case RACE_GNOME:
            {
                if (Gender == GENDER_MALE)
                    return 5;

                return 3;
            }
            case RACE_ORC:
            {
                if (Gender == GENDER_MALE)
                    return 5;

                return 5;
            }
            case RACE_TAUREN:
            {
                if (Gender == GENDER_MALE)
                    return 4;

                return 3;
            }
            case RACE_TROLL:
            {
                if (Gender == GENDER_MALE)
                    return 5;

                return 4;
            }
            case RACE_UNDEAD_PLAYER:
            {
                if (Gender == GENDER_MALE)
                    return 4;

                return 7;
            }
        }
    }

    return 1;
}

std::string GetRandomGreeting()
{
    std::vector<std::string> greetings = {
        "Hey there!",
        "Hello!",
        "Greetings!",
        "Hi there!",
        "Howdy!",
        "Nice to meet you!",
        "What's up!",
        "Hey!",
        "Greetings, traveler. What brings you to these lands?",
        "Oh, it’s you... again.",
        "Ah, another hero graces us with their presence.",
        "Lok’tar, friend. Or not. Depends on the day.",
        "May Elune light your path, if she’s not busy.",
        "Hey there! Don’t mind the mess, it’s just another undead invasion.",
        "For the Alliance! Or Horde... I don’t judge.",
        "You’re back? Didn’t think I’d see you again.",
        "By the Light, why do you look so... disheveled?",
        "Hello! Do you have gold? Asking for a friend.",
        "Ah, the savior of Azeroth graces us! Or is it just another quest?",
        "Howdy, champion! Ready to slay another thousand boars?",
        "Great. Another adventurer thinking they’re special.",
        "Oh look, a living legend. Probably just a rumor.",
        "Do you bring tidings from Orgrimmar, or just trouble?",
        "Fancy seeing you here! Didn’t think you’d survive that last raid.",
        "The winds of Mulgore whisper your arrival. Bit dramatic, no?",
        "Ahoy! Did you bring rum this time?",
        "You again? I thought you retired after the Naxxramas wipe.",
        "Hello, hero! Don’t trip on your cape on the way in.",
        "Oh joy, another visitor. Just what I needed.",
        "Welcome to Stormwind! Now don’t go starting trouble.",
        "The Horde welcomes you! Just don’t touch anything.",
        "By the Sunwell! You look... fine, I guess.",
        "Hey there! Ready to fight dragons, demons, and your group’s loot drama?",
        "Oh, it’s YOU. Well, let’s get this over with.",
        "Welcome to Dalaran! Mind the floating books.",
        "Good to see you! Did you bring your DPS this time?",
        "Hello there! You’re just in time to watch chaos unfold.",
        "An adventurer appears! Roll initiative!",
        "Hey! Looking for a raid or just causing trouble?",
        "Greetings, hero. Do you ever take a break?",
        "Oh, it’s you. What favor do you need today?",
        "Welcome back! Hope you’ve got repair money ready.",
        "Hail, friend! Or foe. Who can tell these days?",
        "Good day to you! Or night. Time gets weird here.",
        "Hey, want to trade? No? Fine, just say hi then.",
        "Welcome to Goldshire. No questions, just move along.",
        "May your loot rolls be terrible. I mean, fantastic!",
        "Oh, adventurer. Your cape’s caught on fire again.",
        "Hello there! Did you bring cake this time?",
        "Another day, another dungeon, another broken tank shield.",
        "Hey there! You smell like Wetlands swamp... yikes.",
        "Welcome! Are you here to RP or just annoy us?",
        "By the Titans, you’re alive! Barely, but still!",
        "Good to see you! But really, why are you here?",
        "Hey, champion! Did you bring an alibi this time?",
        "Oh, it’s you. Guess the tavern’s still closed.",
        "Hello, noble adventurer! Wait, are you AFK already?",
        "Hey there! Don’t expect a warm welcome from those trolls.",
        "Oh, you’re back. What a... pleasant surprise.",
        "Welcome to Azeroth, where everyone’s a hero but nobody can share loot.",
        "Lok’tar, champion. Careful, you’ve got something on your boots.",
        "By Elune, you’re here! Took you long enough.",
        "Greetings, mortal. Try not to die today.",
        "Oh, great. Another adventurer who thinks they’re the chosen one.",
        "Well, well, well, if it isn’t my favorite corpse-to-be.",
        "Hail, hero! Or, as I like to call you, future repair bills.",
        "Welcome! Did you bring the DPS? We’re running low.",
        "Why hello there! Are you here to actually help, or just AFK in the capital?",
        "Ah, a brave soul! Or just someone with too much free time.",
        "By the ancestors, you’re here again. Can’t say I’m thrilled.",
        "Oh look, another savior of the world. Join the queue.",
        "Hey! Don’t forget to bring snacks for the raid this time.",
        "Greetings, wanderer. Have you come to die, or to complain?",
        "Well met! Have you brought glory or just more trouble?",
        "Oh no, not you again. Please don’t touch anything.",
        "A hero appears! Let’s see how long you last this time.",
        "Welcome back! I thought you’d finally rage-quit for good.",
        "Hail, friend! Or is it foe? Hard to keep track these days.",
        "Oh, it’s YOU. Great. Just great.",
        "What’s that smell? Oh, it’s you. Welcome, I guess.",
        "Well, look who crawled out of the Deeprun Tram.",
        "Hello, hero. Did you remember your pants this time?",
        "Hey! Don’t just stand there like a training dummy!",
        "Ahoy there! Careful, this isn’t Booty Bay.",
        "Oh, you again. Did you run out of mobs to kill?",
        "Why, if it isn’t the champion of tab targeting.",
        "Hey there! Let’s see if you can survive this zone.",
        "Ah, greetings, champion. Still alive, I see.",
        "What news from the frontlines, brave adventurer? Wait, never mind, I don’t care.",
        "Oh good, you’re here. Just in time to clean up the mess.",
        "Welcome! Are you here to quest or just cause chaos?",
        "By the Light! Did you just teleport here, or am I seeing things?",
        "Hello there! Please tell me you’ve brought some gold for repairs.",
        "Oh great, another hero looking for handouts.",
        "Greetings! Don’t mind the screaming murlocs.",
        "Ah, you’ve arrived. Too bad you missed the feast.",
        "Hail, traveler. Did you bring your A-game, or just excuses?",
        "Welcome, adventurer. Watch out for the locals—they bite.",
        "Oh, hello. Do you actually have a plan this time?",
        "Ah, a fellow wanderer. Careful, these parts are cursed.",
        "Why do you keep coming back here? We’re running out of quests.",
        "Hail! Are you lost, or just really optimistic?",
        "Ah, you’ve arrived. I’ll start preparing your tombstone.",
        "By the ancestors, don’t just stand there—do something heroic!",
        "Greetings, mortal. Are you ready to disappoint your raid again?",
        "Ah, the infamous returns. Here for glory, or just loot?",
        "Welcome, champion. Don’t touch the relics this time.",
        "Good to see you! Now, about that debt you owe me...",
        "Oh, look who decided to show up. Late as always.",
        "Well met, hero! You’ve got something in your teeth, by the way.",
        "Welcome back! Please try not to burn this place down.",
        "Hello, friend! Or at least someone I don’t dislike... yet.",
        "Oh, it’s YOU. Did you break something again?",
        "Greetings, brave soul. You didn’t bring any cursed items, right?",
        "Hail, adventurer! Don’t look now, but that mob is respawning.",
        "By Elune, do you always look this disheveled?",
        "Ah, you’ve returned. Couldn’t stay away, could you?",
        "Hello! Are you ready to spend six hours farming for nothing?",
        "Oh joy, another hero here to save the world. Again.",
        "Ah, greetings. Don’t mind the mess—it’s just another invasion.",
        "Oh, look, it’s our raid leader. Late, as usual.",
        "Welcome back! Did you bring any repair bills this time?",
        "By the Sunwell, you again? Thought you were banned.",
        "Ah, greetings, traveler. Did you remember to set your hearthstone this time?",
        "Well met! Did you come for adventure, or to loot my patience?",
        "Ah, my favorite troublemaker! What are you breaking today?",
        "Oh good, another hero to carry the burden of my disappointment.",
        "Hail, brave soul! Or at least someone brave enough to queue DPS.",
        "Welcome! Are you lost, or is this your actual destination?",
        "Back again, eh? Did your last group kick you out?",
        "Hello there! Ready to run out of mana again?",
        "By Elune, I thought I smelled noob energy. It’s you.",
        "Ah, the infamous... Here to flex, or to fail?",
        "Greetings! Did you finally finish that quest from three expansions ago?",
        "Hey there! Got any *good* loot, or just vendor trash?",
        "Well, well, if it isn’t the tank who forgot to taunt.",
        "Oh great, another hunter pulling before the tank.",
        "Hello! Careful with the murlocs this time, yeah?",
        "Welcome back! The NPCs missed you... no, really, they didn’t.",
        "Ah, you’ve returned. What a treat for the mobs in this zone.",
        "Hello! Please tell me you remembered to repair your gear.",
        "Hey there, champ. Did you finally learn to dodge mechanics?",
        "By the ancestors, do you even know how to use your abilities?",
        "Oh look, it’s the reason my recount DPS is so low.",
        "Hey! Don’t forget to pick up your loot this time.",
        "Hail, adventurer! And by hail, I mean stop dying so much.",
        "Hello again! Did you find more ways to embarrass your faction?",
        "Oh good, you’re here! Just in time to wipe the group.",
        "Hello! Is this the day you actually use your cooldowns?",
        "Welcome, hero! Try not to stand in the fire again.",
        "Ah, the chosen one. Or at least the one chosen to pull aggro.",
        "By the Light, you’re back! Did you manage to hit anything last time?",
        "Hail! Are you here to fight, or just to complain about loot drops?",
        "Hello there! Did you bring your fishing pole again? Really?",
        "Oh no, not you again. Please don’t ninja-pull this time.",
        "Ah, a new day, and the same tank forgetting their shield.",
        "Hello! If you die, it’s totally not my fault this time.",
        "Well met, adventurer. I see you’ve brought your A-game. Or is it your Z-game?",
        "Oh, you’re back. Did you fix your keybinds yet?",
        "Welcome! Did you remember to unmute the healer this time?",
        "Hello! Please tell me you’re not the one that keeps pulling trash mobs.",
        "Greetings! Are you ready to blame the healer again?",
        "Oh, look who it is. The person who loots in combat. Nice.",
        "Hail! Just don’t let the guild leader catch you in THAT gear.",
        "Oh, good. Another hunter who forgot to train their pet.",
        "Welcome back! Did you solve your aggro problems? No? Okay.",
        "Hello there! Ready to roll a natural 1 on your loot again?",
        "By the Naaru, why do you always come back in such a mess?",
        "Greetings! Just leave the tanking to the tank this time, alright?",
        "Hello! Are you here to help, or to flex your mount collection?",
        "Ah, the hero returns! Still can’t find your quest giver, huh?",
        "Ah, the hero of yesterday’s wipe! How are you still alive?",
        "By the Bronze Dragonflight, you’ve managed to show up on time. Impressive.",
        "Welcome back! Did you finally unlearn your bad talent choices?",
        "Oh no, not again. Did someone rez you by mistake?",
        "Greetings, brave adventurer! Or should I say ‘brave keyboard warrior’?",
        "Hello! Are you here to fight, or to type angry whispers?",
        "Oh, great. Another raid, another ‘accidental’ ninja pull.",
        "By the Sunwell, you smell like your repair bills are overdue.",
        "Hail! Back to test the limits of my patience, I see.",
        "Oh, it’s you. The only player who can make PvP look like PvE.",
        "Welcome! Did you finally figure out how to use the map?",
        "Hello again! Have you considered respeccing to something useful?",
        "Oh, look who decided to grace us with their ‘skills.’",
        "Hey there, champ! What’s the DPS excuse of the day?",
        "Welcome back! Please tell me you didn’t bring your quest log again.",
        "Ah, my favorite healer! Here to save us, or to AFK during boss fights?",
        "Greetings! Are you ready to blame lag for your mistakes again?",
        "By Elune, your fashion sense gets worse every time you show up.",
        "Oh, you’ve returned. Have you mastered walking out of AoE yet?",
        "Hey! Did you come to fight or to pick flowers again?",
        "Welcome! Remember, the boss mechanics aren’t ‘optional.’",
        "Oh, it’s you. The scourge of trade chat and my sanity.",
        "By the Light, I thought I smelled another loot goblin.",
        "Hail! Did you bring any *real* DPS this time?",
        "Oh great, you’re here! How much gold are we losing to repairs today?",
        "Welcome back! Are you still rolling need on vendor trash?",
        "By the ancestors, you fight like a toddler with a stick.",
        "Oh, the chosen one returns. Let’s hope you don’t choose death again.",
        "Greetings! Are you here to help, or just to stand around?",
        "Oh, look, it’s the reason we can’t have nice things in Azeroth.",
        "By the Naaru, do you even have spells on your action bar?",
        "Hey! Did you come to flex your mount, or to help with the boss?",
        "Welcome back! Did you finally figure out how to enchant your gear?",
        "Ah, the tank with the ‘YOLO’ strategy. This’ll be fun.",
        "Hail, brave soul! Or at least brave enough to skip the tutorial.",
        "Hello again! Did you bring more excuses or fewer cooldowns?",
        "By the Light, your transmog burns my eyes.",
        "Greetings! Did you finally learn that the water in Stranglethorn is not for drinking?",
        "Oh no, it’s the loot hoarder. Hide your greens, everyone!",
        "Welcome back! I see you still haven’t figured out your rotation.",
        "Hail! Are you ready to be the first to die... again?",
        "Oh, great. You’re here. Let’s start the countdown to chaos.",
        "By Elune, how do you even fit in that armor?",
        "Hello! Are you here to pull aggro or just to pull my leg?",
        "Welcome! Did you remember to repair your weapon this time?",
        "Oh, look, it’s the champion of tab-targeting mobs we didn’t need.",
        "Greetings! Are you here for glory, or just another bad loot roll?",
        "Hail! Do us all a favor and don’t touch anything shiny this time.",
        "Oh, look, a walking, talking threat to group cohesion.",
        "By the spirits, did you eat all the food buffs again?",
        "Greetings! Remember: the boss fights back.",
        "You guys are actually the best group I’ve had all week. Don’t let it get to your heads.",
        "Remember that time we accidentally pulled the entire dungeon and still survived? Good times.",
        "Honestly, watching you tank is like poetry in motion. Aggro poetry, but still poetry.",
        "I’d follow this healer into battle any day. Even if it’s into a fire… again.",
        "Your DPS numbers are so high they should come with a warning label.",
        "You’re all great, but let’s not pretend we’re coordinated. Chaos squad forever.",
        "Who needs raid leaders when we’ve got a natural-born strategist right here?",
        "I’m not saying you’re carrying us, but my back’s not hurting, so thanks.",
        "Do you even sleep, or are you just this good naturally?",
        "Remember when %my_role forgot to turn on their abilities for half a fight? Classic.",
        "You’re like a loot magnet. Let me know if you find anything shiny.",
        "I’d insult you, but you’ve already done the hard work of playing yourself.",
        "Your dodging skills are impressive. Ever thought about becoming a rogue instead?",
        "You’re so good at pulling aggro, the mobs must think you’re their leader.",
        "Hey healer, you’re working miracles out here. Next stop: sainthood.",
        "If sarcasm were a stat, you’d be critting every time.",
        "Do you practice being this good, or is it just natural incompetence?",
        "You remember that time we wiped because someone decided to 'test a theory'? Yeah, me neither.",
        "You’ve got so much loot, it’s like you’re trying to start your own auction house.",
        "Your AoE was so good that even mobs on another server felt it.",
        "This group is great, except for that one guy… Oh wait, that’s me.",
        "The healer keeps us alive, but I’m pretty sure they’re also judging our life choices.",
        "I don’t always compliment DPS, but when I do, it’s because they carried the fight.",
        "You tank like a wall. An angry, slightly chaotic wall, but a wall nonetheless.",
        "Remember that one time the tank pulled too early? Oh wait, that’s every pull.",
        "You’re so good at dodging mechanics, even the boss seems impressed.",
        "If loot were based on charm, you’d definitely get all the grays.",
        "Your spells are so flashy, I’m blinded more by you than the enemy.",
        "You’re doing great, sweetie. Except when you’re not, but let’s focus on the positives.",
        "You’re the kind of person that makes bosses cry. Literally, they’re sobbing.",
        "I’d say you’re the glue holding this group together, but it’s probably duct tape.",
        "Remember when we were over-prepared for a fight? Me neither.",
        "You’re the kind of DPS that makes mobs reconsider their life choices.",
        "Your healing is so on point, I’m starting to think you have a direct line to the Light.",
        "I could watch you tank all day. Preferably while not standing in fire, though.",
        "You know, for a group of strangers, we’re surprisingly less dysfunctional than expected.",
        "I’ve seen murlocs with better aim than you, but hey, you’re trying.",
        "Your trash talk is almost as strong as your DPS. Almost.",
        "Let’s be honest: if loot were awarded by attitude, you’d get nothing. Absolutely nothing.",
        "You’ve got big raid-leader energy. Or maybe just big chaos energy. Same thing.",
        "Your heals are so clutch, I’d take you into a PvP match… if I liked PvP.",
        "This group is like a fine wine: chaotic and probably not improving with age.",
        "Your gear may not be the best, but your attitude? Well, it’s something.",
        "Remember that time we faced an impossible fight and won? Me neither, but this could be it.",
        "You’re the kind of player that makes random queues bearable. Sometimes.",
        "Your pet has more personality than some players I’ve grouped with. Keep it up!",
        "You’re so good at avoiding mechanics, it’s like you weren’t even in the fight.",
        "Your rogue skills are so sneaky, even the boss doesn’t notice you’re there.",
        "I’d give you a compliment, but then you’d think I liked you.",
        "I don’t say this often, but I actually enjoy running with you guys.",
        "Remember that time we wiped because the tank wanted to ‘test something’? Yeah, that’s why I have trust issues.",
        "Your damage is so high, I’m pretty sure you’re breaking some kind of server rule.",
        "If sarcasm was a buff, you’d have it at max stacks.",
        "You’re not just carrying us; you’re carrying my hopes and dreams. No pressure.",
        "You tank like a champ… if that champ was slightly tipsy.",
        "That AoE was so good, I felt it in real life.",
        "You know, for a group of randoms, we’re doing surprisingly well. Let’s not ruin it.",
        "If you DPS any harder, the boss might file a restraining order.",
        "Remember when %my_role accidentally pulled the boss early? Yeah, neither do I.",
        "You heal like a pro. A very exhausted, slightly sarcastic pro.",
        "You’re so good at pulling aggro, I’m starting to think the mobs owe you money.",
        "Your pet’s doing more damage than I am, and I’m oddly okay with that.",
        "This group is like a bad joke, but at least we’re laughing through it.",
        "Your interrupts are so on point, even my Wi-Fi is jealous.",
        "If loot was based on effort, you’d still be rolling 2s.",
        "Your DPS is so high, I’m starting to think you made a deal with an Old God.",
        "You’re the MVP of this run. Mostly Very Persistent.",
        "Your tanking style reminds me of a bulldozer: efficient, chaotic, and slightly terrifying.",
        "Remember that time we had a coordinated fight? Me neither.",
        "Your spells are so flashy, I thought I was at a Lightshow Festival.",
        "For a healer, you’ve got a lot of sass. Respect.",
        "Your positioning is so perfect, I’m starting to think you use a protractor.",
        "The boss doesn’t hate us as much as they hate you. Congrats?",
        "Your rogue stealth is so good, you’re basically a shadow with daggers.",
        "This dungeon should come with a warning: Danger, teamwork required.",
        "You’re like a loot magnet, except it’s all grays.",
        "Your AoE is like fireworks. Beautiful and slightly hazardous to everyone nearby.",
        "If this group had a mascot, it’d be you, waving a flag of pure chaos.",
        "Your tanking gives new meaning to the phrase ‘living on the edge.’",
        "Remember that time we didn’t argue over loot? Yeah, that never happened.",
        "Your damage numbers are higher than my expectations, which is saying a lot.",
        "I’d trust you with my life… just not my loot.",
        "This group is like a carnival ride: wild, unpredictable, and a little nauseating.",
        "You’re so good at dodging mechanics, I’m starting to think you’re part rogue.",
        "If sarcasm healed, we wouldn’t even need a healer in this group.",
        "Your boss pulls are so bold, I’m writing them into my memoir.",
        "You’re so good at avoiding damage, even RNG is impressed.",
        "Your gear score might be low, but your energy is unparalleled chaos.",
        "If we survive this fight, drinks are on you.",
        "This group has the coordination of a herd of murlocs, but hey, we’re trying.",
        "Your heals are so clutch, it’s like you have a sixth sense.",
        "The way you handled that mob was art… chaotic art, but still art.",
        "You’re the glue holding this group together, and it’s definitely duct tape.",
        "Remember when we over-prepared for that boss? Yeah, me neither.",
        "You’re so good at mechanics, I’m nominating you for dungeon MVP.",
        "If RNG was fair, you’d have a legendary by now.",
        "You’re doing great, sweetie. Just don’t stand in the fire.",
        "I’d love to visit Nagrand someday. Those floating islands look so peaceful.",
        "I miss sitting under the massive mushrooms in Zangarmarsh. They’re oddly comforting.",
        "Someday, I want to see the sunrise over Winterspring. It’s gotta be breathtaking.",
        "I’d kill to have a picnic in Elwynn Forest, away from all this chaos.",
        "Imagine relaxing by the waterfalls in Grizzly Hills. Sounds like a dream.",
        "I miss fishing off the piers in Booty Bay. The vibe there is unmatched.",
        "I always wondered what it’d feel like to sit atop Hyjal and just take in the view.",
        "If I could, I’d retire in Stormwind’s park district… before it got burned down, that is.",
        "I want to walk the quiet paths of Darnassus again. Rest in peace, my beloved city.",
        "The forests of Ashenvale at dusk? Perfect place for a nap or maybe some light sparring.",
        "I wonder if the beaches of Tanaris are as relaxing as they seem. Probably not with all the pirates.",
        "I’d love to stargaze in the Barrens. No light pollution there, just endless skies.",
        "Imagine spending a lazy afternoon in Thunder Bluff, just watching the plains below.",
        "One day, I’ll make it to the top of Icecrown Citadel… as a tourist, not an invader.",
        "I miss the snowy tranquility of Dun Morogh. Except for the troggs. They can stay away.",
        "How amazing would it be to just float down the rivers in Valley of the Four Winds?",
        "I want to see the inside of Ulduar again, not for a raid, just to appreciate the architecture.",
        "Sitting by the fires in Orgrimmar during a cold night hits differently. Miss those days.",
        "I wish I could explore the depths of Vashj’ir without worrying about drowning… or naga.",
        "Someday, I’ll take a proper vacation in Stranglethorn Vale. Just me, the jungle, and no trolls.",
        "I wonder if the Hinterlands are as serene as I remember, or if I’ve romanticized them.",
        "I miss hearing the winds howl in Dragonblight. There’s a strange beauty in the cold.",
        "I’d love to visit the Sunwell. It must be breathtaking in a way words can’t describe.",
        "The tranquility of Teldrassil will always stay with me. Rest well, ancient one.",
        "I want to see the Emerald Dream with my own eyes. Imagine the colors.",
        "One day, I’ll sit on the shores of Azuremyst Isle and just listen to the waves.",
        "The rolling hills of Mulgore? Perfect place for some quiet meditation.",
        "I’d pay good gold to relax in Shattrath City, just watching the world go by.",
        "I wonder if anyone’s managed to camp in the Howling Fjord without freezing to death.",
        "I’d love to wander the floating spires of Netherstorm without worrying about mana storms.",
        "I miss the scent of the forest in Feralas. It felt like home, even when it wasn’t.",
        "One day, I’ll build a little cabin in Grizzly Hills and live the dream.",
        "Imagine climbing the spires in Blade’s Edge Mountains just for the view.",
        "I’d love to lounge in the Arboretum in the Jade Forest, soaking in the green.",
        "The quiet of Crystalsong Forest? That’s where my soul feels at peace.",
        "I miss the bustling markets of Dalaran. There’s magic in the air, literally.",
        "Someday, I’ll sail out to the Maelstrom and just stare into the chaos.",
        "I’d like to see the Temple of Karabor restored to its former glory. It deserves it.",
        "I wonder what it’s like to just camp under the stars in Desolace. Peaceful, maybe?",
        "I want to watch the auroras dance over Icecrown. Even there, beauty persists.",
        "The sandy expanse of Uldum always called to me. Something ancient and calm about it.",
        "One day, I’ll sit on the cliffs of Thousand Needles and watch the sun dip below the horizon.",
        "I’d love to sit on the steps of the Caverns of Time and ponder eternity.",
        "The soft glow of the trees in Eversong Woods? Absolutely magical.",
        "I miss the gentle hum of Exodar. That ship feels like home to me.",
        "I wish I could camp out on the edge of Deepholm, staring into the abyss.",
        "Someday, I’ll visit every moonwell in Azeroth and make wishes at each one.",
        "I’d like to lay in the grass of Arathi Highlands, watching clouds roll by.",
        "Imagine sitting in the middle of Shadowmoon Valley, marveling at the fel fire. Beautiful, in a terrifying way."
    };
    
    int index = std::rand() % greetings.size();
    return greetings[index];
}

std::string GetRandomListeningResponse()
{
    std::vector<std::string> responses = {
        "Mmm-hmm, I'm listening!",
        "Go on, I'm all ears!",
        "Uh-huh, tell me more!",
        "Sure, that sounds fascinating.",
        "I'm paying attention, promise!",
        "What was that again?",
        "Right, absolutely!",
        "Oh, really? Fascinating stuff.",
        "You don’t say! Actually, maybe don’t.",
        "Keep talking, I’m totally not alt-tabbed.",
        "Listening... or at least trying to.",
        "That sounds like a personal problem.",
        "Yes, yes, go on, sage of wisdom.",
        "I’ve never been so uninterested yet attentive.",
        "Wow, that’s... something.",
        "Hold on, I need popcorn for this.",
        "Talk slower; I’m trying to keep up.",
        "Sure, sure, keep the epic going.",
        "Uh, yeah, sounds super important.",
        "Did you lose me, or did I lose you?",
        "Fascinating, truly, %my_race-level intellect here.",
        "I’m here, but my brain might not be.",
        "Oh, don’t stop now, it’s just getting good.",
        "Sorry, what were you saying?",
        "Totally tuned in... promise.",
        "Hold on, I’ll pretend I care.",
        "Wow, that’s some lore-quality storytelling.",
        "Keep it going! I need some context.",
        "I heard ‘blah blah something important.’",
        "Let me grab a pen; this is gold.",
        "Whoa, are you writing a novel or something?",
        "Wait, are you still talking?",
        "Uh-huh, uh-huh... wait, what?",
        "Listening, but also zoning out. Multitasking!",
        "The suspense is killing me. Keep going.",
        "That’s so deep. Like, ocean-floor deep.",
        "I’m nodding, but my brain is AFK.",
        "Wow, I can feel the plot twist brewing.",
        "Go on, poet. Spin your words.",
        "You’re really committed to this story, huh?",
        "You could be narrating this to a wall.",
        "I’m still here, surprisingly.",
        "Don’t worry, I’m hanging on every word.",
        "Wow, it’s like I’m there. Almost.",
        "Are you done, or is this the prologue?",
        "You’re still talking, huh? Bold of you.",
        "I’m listening like a pro. Mostly.",
        "Can we get the TL;DR version, though?",
        "Never in my life have I cared less but enjoyed more.",
        "By Elune’s grace, is there a point to this story?",
        "Speak faster, mortal; I’ve not all day.",
        "Oh, you’re still talking. How... unexpected.",
        "Does this tale end in treasure or tears?",
        "Your words are as long as a Stormwind sermon.",
        "Why do I feel like this will summon an Old God?",
        "Tell me, is this leading to a raid boss or just drama?",
        "Stop. You had me at ‘hello.’",
        "You’ve got the storytelling flair of a wandering bard.",
        "By the Sunwell, this better not involve goblins again.",
        "I’m nodding, but I’m lost in the Barrens.",
        "What in Azeroth are you trying to say?",
        "If this doesn’t involve dragons, I’m walking away.",
        "Tell me more, unless it involves Darkshore.",
        "Sounds like something Sylvanas would whisper.",
        "Are we in Goldshire? This feels unnecessarily long.",
        "You could’ve written a tome by now.",
        "Did Khadgar put you up to this?",
        "Why does this remind me of N’zoth’s ramblings?",
        "If the Lich King isn’t involved, I’m not interested.",
        "Oh no, this sounds like something out of Ulduar.",
        "Continue, but I reserve the right to yawn.",
        "Let me guess, the Horde is to blame?",
        "If this ends in Teldrassil burning, I’m leaving.",
        "Do you ever stop talking, or is this magic?",
        "What’s the twist? Is it demons again?",
        "Do you need a mage to teleport this story away?",
        "Does this tale have a moral, or are we wandering?",
        "You’re as cryptic as an ancient Titan artifact.",
        "By the Light, this story is longer than a dungeon queue.",
        "I sense chaos brewing. Please, continue.",
        "Is there a conclusion, or is this an RP loop?",
        "This better end with loot or I’m out.",
        "Sounds like something I’d hear in a Twilight cult.",
        "Please tell me this isn’t a fetch quest in disguise.",
        "If I cast Silence on you, will you stop?",
        "This story feels cursed, like a bad roll on loot.",
        "I can feel the Naaru judging your life choices.",
        "Spare me the details and give me the loot.",
        "The spirits whisper that this is going nowhere.",
        "You’ve lost me somewhere in Ashenvale.",
        "What if we skipped to the good part?",
        "Oh, you’re one of those lore types, aren’t you?",
        "Just tell me if this involves a raid group or not.",
        "If it’s not about Illidan, it’s not worth my time.",
        "Why do I feel like we’re in a looped RP event?",
        "The only thing worse than this story is RNG loot.",
        "I hope this tale ends with me waking up.",
        "Should I fetch Thrall? This feels like his territory.",
        "Are you rehearsing for a role in the Argent Crusade?",
        "If this were a scroll, I’d burn it.",
        "By the Ancestors, get to the point already.",
        "Your words flow like a sluggish river through the Wetlands.",
        "If you don’t hurry, I’ll call a mage for a portal out of here.",
        "Are you weaving a story, or is this just a fever dream?",
        "Can someone toss a silence spell on this one?",
        "This feels as endless as a Pandaren feast.",
        "Your storytelling skills rival a kobold’s candle defense.",
        "Do you even know where you’re going with this?",
        "By the Titans, can we skip to the loot part?",
        "This is starting to sound like a Twilight’s Hammer sermon.",
        "Do you need an intermission or something?",
        "Let me guess: this ends with you getting an epic mount?",
        "Is this story older than the Sundering itself?",
        "I can almost hear the Bronze Dragonflight sighing at this timeline.",
        "Are you stuck on repeat, or do you actually have a conclusion?",
        "This better not lead to a ‘to be continued.’",
        "If this were a dungeon, I’d hearthstone out.",
        "The Light abandoned this conversation long ago.",
        "By the Twisting Nether, what are you on about?",
        "Does this involve anything shiny or just more rambling?",
        "Your words are as confusing as a goblin engineering manual.",
        "Oh, please, continue. I haven’t laughed this hard in ages.",
        "This reminds me of the time I got stuck in Thousand Needles.",
        "Do you practice these stories on critters in Elwynn Forest?",
        "If this doesn’t end in a boss fight, I’m leaving.",
        "You talk like you’ve been cursed by a talking skull.",
        "Is this leading somewhere, or are we farming dialogue rep?",
        "You could narrate for a murloc documentary.",
        "This feels like a poorly rolled side quest.",
        "Why do I feel like this ends in Orgrimmar drama?",
        "Your tale is as tangled as the roots of Teldrassil.",
        "Even an ogre would’ve wrapped this up by now.",
        "Do you want applause or a /yawn at the end of this?",
        "This is as drawn out as a gnome’s convoluted gadget explanation.",
        "Your pacing rivals a turtle mount in quicksand.",
        "If words were damage, you’d be OP.",
        "Do I need to roll for patience here?",
        "Sounds like you’re auditioning for the Banshee Queen’s choir.",
        "This story is as dense as a tauren’s skull—no offense, tauren.",
        "Could someone roll Need on a point for this story?",
        "Let me know when you’re done; I’ll be in Winterspring.",
        "You talk like you’ve been to the Void and back.",
        "Did you pull this tale from a random quest text?",
        "This conversation feels like an unending escort mission.",
        "Even Deathwing couldn’t drag this out more.",
        "Your dialogue box must have the longest cooldown ever.",
        "If this story had loot, it’d be vendor trash.",
        "Does anyone have a map? We’ve lost the plot.",
        "You’re stalling more than a healer who forgot mana potions.",
        "Is this still Azeroth, or did we wander into the Emerald Dream?",
        "Yesterday, I accidentally challenged a level 80 elite… didn’t end well.",
        "I once fell off Teldrassil trying to take a shortcut. Spoiler: no shortcut.",
        "I did an entire dungeon without realizing my weapon was broken.",
        "Last week, I sold a rare drop to a vendor. I still cry about it.",
        "I once outran a rogue in PvP by jumping off a cliff. Goblin gliders are OP.",
        "Yesterday, I tanked an entire raid in fishing gear. Nobody noticed… at first.",
        "I soloed Hogger once… with a group of friends cheering me on.",
        "I tried to stealth past mobs as a warrior. Let’s just say it didn’t work.",
        "I once farmed reputation for three days straight, only to realize it was the wrong faction.",
        "Yesterday, I got stuck in Stormwind canals and had to hearthstone out.",
        "I once walked all the way from Darnassus to Booty Bay because I didn’t know about flight paths.",
        "I misclicked on a flight path once and ended up in Tanaris. I wasn’t even mad.",
        "Yesterday, I discovered I’ve been using the wrong talent spec for months.",
        "I once spent an entire raid fishing. Nobody even noticed.",
        "I forgot to loot a boss once… still haunts me to this day.",
        "I used a hearthstone mid-fight by accident. My group was… not happy.",
        "I spent an hour looking for a rare spawn, only for someone to kill it right as I found it.",
        "I once queued as a tank but healed the whole dungeon. Nobody complained.",
        "Yesterday, I accidentally vendored my epic gear. RIP gold and dignity.",
        "I tried to kite a world boss into a city once. Didn’t go as planned.",
        "I once got lost in Dire Maul for three hours. The group left me there.",
        "I fell asleep in a battleground once and woke up with honor points.",
        "I pulled a boss early once, claiming it was a 'lag spike.'",
        "I tried to sell grey items in trade chat once. Surprisingly, someone bought them.",
        "I once forgot I was flagged for PvP and got ganked in the middle of nowhere.",
        "I did an entire dungeon using auto-attack. Nobody even noticed.",
        "I accidentally mounted up in the middle of a stealth mission. Whoops.",
        "I once walked into a raid thinking it was a dungeon. It was… humbling.",
        "Yesterday, I found out I’ve been using the wrong weapon for my class for weeks.",
        "I tried to swim to Northrend once. It didn’t go well.",
        "I once farmed for hours, then forgot to turn in the quest. Classic.",
        "I died pulling one too many murlocs. Happens to the best of us.",
        "I once rolled Need on a fishing pole by accident. My group wasn’t amused.",
        "I tanked an entire fight without pants. Don’t ask how it happened.",
        "Yesterday, I saw a gnome tanking a raid boss. It was oddly inspiring.",
        "I once healed an entire group as a shadow priest. They didn’t even complain.",
        "I got lost in Undercity once and had to ask a guard for directions.",
        "I tried to duel a hunter in melee range. Turns out, they were really good at kiting.",
        "I once spent an hour trying to climb a mountain just to see what was on the other side.",
        "Yesterday, I accidentally aggroed an entire mob pack while AFK. My bad.",
        "I once asked where the auction house was… in Thunder Bluff. Still embarrassed.",
        "I’ve farmed the same rare mount for years. RNG truly hates me.",
        "I did an entire dungeon without realizing my shield was broken.",
        "I once tried to tank as a mage because the group needed it. We didn’t survive.",
        "Yesterday, I learned that stealth doesn’t work when you jump in front of mobs.",
        "I fell off a zeppelin once. Twice. Okay, it happens a lot.",
        "I tried to summon a raid group, but I was standing inside a tree. Oops.",
        "I once farmed for days thinking a drop was real, only to find out it was a rumor.",
        "Back when I was a noob, I tried to sell herbs in the weapon shop. Classic.",
        "That time I jumped off the boat to Theramore… thought I could swim the rest. I was wrong.",
        "Ever been so lost in Silvermoon that you just hearthstone out? Yeah, that was me.",
        "A group once asked me to tank, and I thought 'taunt' was a DPS ability. Good times.",
        "Years ago, I thought mana potions were food. I kept wondering why I wasn’t getting full.",
        "Someone once asked me for gold, so I traded them one copper and ran away laughing.",
        "They say you can’t kite Deathwing. I tried anyway. Didn’t end well.",
        "There was this one time in Stranglethorn Vale where I got ganked… for an hour straight.",
        "I saw a rogue pickpocket a critter once. Not even mad, just impressed.",
        "Got lost in Orgrimmar once and ended up in a fight with a guard. I lost.",
        "That day when I thought I could solo Zul’Farrak… let’s just say I underestimated trolls.",
        "Someone said 'stand in the fire for a buff,' so I did. Never trusting chat again.",
        "Pulled every murloc on the beach once. My party still calls it ‘The Great Swim.’",
        "I saw a paladin bubble-hearth mid-raid wipe. Can’t blame them, honestly.",
        "Back in Wrath, I tried to tank in DPS gear. Nobody lived to tell the tale.",
        "They told me the Scarlet Monastery was easy. Yeah, right.",
        "There was this mage who tried to Polymorph a mechanical boss. A for effort.",
        "A guildmate once told me to roll Need on everything. That was… an experience.",
        "Thought the PvP flag would go away faster. Turns out, it doesn’t when you’re being hunted.",
        "Was fishing in Darnassus once, then suddenly flagged for PvP. Never again.",
        "That one time a gnome tank outperformed our geared warrior. Unforgettable.",
        "My mount disappeared mid-flight. Turns out, fatigue zones are a thing.",
        "During a battleground, I fell into a hole and couldn’t get out. The team didn’t miss me.",
        "There’s no shame in getting lost in the Caverns of Time… right?",
        "Once wiped the entire group because I didn’t know what 'LoS' meant.",
        "I thought I could stealth past a dragon. Turns out, dragons have great eyesight.",
        "Who knew standing in a poison pool would actually kill you? Not me, apparently.",
        "I rolled greed on a legendary once. Still hear about it to this day.",
        "When they said ‘don’t aggro the boss,’ I heard ‘aggro the boss.’ My bad.",
        "A random druid shapeshifted into a stag and offered me a ride. I accepted.",
        "One time, I got stuck in ghost form because I couldn’t find my corpse. Spent an hour running.",
        "Saw someone duel-wielding fishing poles in a dungeon. Respect.",
        "Tried to cast fear on an undead mob. Turns out, they’re immune. Who knew?",
        "Accidentally vendored my epic trinket once. It still hurts.",
        "Someone told me I could farm reputation in my sleep. I believed them.",
        "During an RP event, I called Sylvanas ‘Lady Pointy Ears.’ That didn’t go over well.",
        "A mage blinked off a cliff mid-fight once. I couldn’t stop laughing.",
        "Got kicked from a raid for doing DPS as a tank. Look, I had the damage meters to prove it.",
        "Pulled the last boss in a dungeon without clearing the room. Yeah, that was me.",
        "Tried to tame a mechanical wolf as a hunter. Turns out, not all wolves are tameable.",
        "Forgot to equip my weapon before a raid. Fought the whole fight with my fists.",
        "Tried to kite a raid boss to the main city for laughs. Got banned for three days.",
        "Saw a warrior charge into lava to 'shortcut' the dungeon. They didn’t make it.",
        "Didn’t realize my gear was broken until after the boss fight. No wonder I kept dying.",
        "That day when the healer pulled the boss because they were ‘bored.’",
        "Tried to solo Onyxia once because ‘I saw a video.’ Turns out, I’m not that good.",
        "A troll once told me the best loot is at the bottom of the Maelstrom. I checked. It’s not.",
        "Got stuck in a tree in Stormwind. Still not sure how that happened.",
        "Ran an entire dungeon and forgot to pick up the quests. Classic me.",
        "Pulled the boss because 'what’s the worst that could happen?' Answer: everything.",
        "Once tried to heal a dungeon as a rogue. Nobody survived, but it was fun.",
        "Thought I could skip trash by jumping over a wall. Turns out, you can’t.",
        "Tried to sell vendor trash as ‘rare items.’ Somebody actually bought it.",
        "Tanked an entire dungeon without realizing my shield was broken. Whoops.",
        "Accidentally dismissed my pet mid-boss fight. The group was not amused.",
        "Thought ‘/dance’ was a combat ability once. Tried it on a raid boss. Didn’t work.",
        "Forgot to summon my minion as a warlock and wondered why my damage sucked.",
        "Once saw a warrior try to taunt Deathwing. Props for the confidence.",
        "Tried to fly under Dalaran for a shortcut. Spoiler: there’s nothing under Dalaran.",
        "Rolled ‘Need’ on a gray item as a joke. Ended up causing a guild drama.",
        "Got stuck in the Deeprun Tram because I jumped off the train. Never again.",
        "I thought equipping all green gear was ‘going green.’ Apparently, that’s not a thing.",
        "Tried to fish in molten lava. Got a fried fish achievement instead.",
        "Got kicked from a dungeon because I ‘talked too much.’ Still worth it.",
        "Tried to ‘rescue’ critters from a farm in Elwynn Forest. Farmers didn’t appreciate it.",
        "Used all my cooldowns on a training dummy and thought I broke the game.",
        "Once enchanted someone’s weapon with the wrong spell. They never noticed.",
        "Saw a paladin bubble mid-jump and still fall to their death. Impressive timing.",
        "Didn’t know you could repair your gear until level 40. That explained a lot.",
        "Tried to duel in the middle of a raid. Raid leader was not impressed.",
        "Once used a summoning stone to prank-summon someone into a volcano. Totally worth it.",
        "Thought healing in PvP would be easy. Turns out, it’s just a fast way to die.",
        "Tried to sell a map to an NPC. Apparently, they don’t buy player art.",
        "Spent two hours farming a rare spawn only for someone else to tag it first.",
        "Got locked out of a dungeon because I ‘explored too much.’",
        "Once roleplayed as a tree for an entire guild meeting. Got kicked, but it was funny.",
        "Followed a group into a raid thinking it was a dungeon. That was awkward.",
        "Used a scroll of hearthstone mid-boss fight as a joke. My group didn’t find it funny.",
        "Spent all my gold on a mount I couldn’t even ride yet.",
        "Tried to sell off-spec loot and got banned from a guild. Worth it.",
        "Was the only one alive during a wipe. Danced on everyone’s corpse out of spite.",
        "Thought clicking on the summoning stone would teleport me. Ended up summoning others.",
        "Challenged a level 60 player to a duel when I was level 10. Spoiler: I lost.",
        "Stood in a fire to see how long I could last. Not long, apparently.",
        "Jumped off a cliff thinking my mount would save me. Gravity disagreed.",
        "Tried to kite a rare mob to a city. Guards didn’t help, but it was entertaining.",
        "Rolled on a cloth item as a warrior ‘for transmog.’ Group wasn’t happy.",
        "Ignored mechanics in a boss fight because ‘it’s probably not important.’ It was.",
        "Heard about a secret quest in the Maelstrom. Jumped in. Guess what? No quest.",
        "Once looted someone’s kill because I thought it was unclaimed. They thought otherwise.",
        "Pulled an entire dungeon by ‘accident.’ My group still doesn’t believe me.",
        "Heard a rumor you could tame critters as pets. Turns out, that’s not true.",
        "Tried to outrun fatigue in the ocean. Discovered why it’s called fatigue.",
        "Once farmed boars for hours thinking I’d find a rare mount drop.",
        "Used /cry on a boss thinking it would ‘confuse’ them. It didn’t.",
        "Challenged someone to a fishing duel. Lost because they used explosives.",
        "Tried to sneak into the enemy faction’s city for ‘fun.’ Guards disagreed.",
        "Saw a rare spawn and stopped to take a screenshot. Someone else killed it first."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomYawningResponse()
{
    std::vector<std::string> responses = { 
        "I’m tired of walking. My feet feel like they’ve been through Warsong Gulch.",
        "Can we take a break? My legs are about to strike against me.",
        "Seriously, who designed this terrain? A flying mount would be nice right about now.",
        "Are we there yet, or is this another endless journey?",
        "I’ve walked so much, I think I’ve leveled up my step count.",
        "Do we have to keep moving? I’m practically dragging my boots here.",
        "Let’s stop for a moment. Even the mobs are faster than me right now.",
        "I swear, if I see another hill to climb, I’m going back to Orgrimmar.",
        "My feet are as sore as a peasant after Arthas’s tax hike.",
        "Can someone carry me? I’ll pay in potions… or moral support.",
        "We’ve been walking for hours, and I’m about to file a complaint with the quest giver.",
        "I miss sitting by the fire in Stormwind. This is torture.",
        "Does anyone have a spare mount? My legs are not ‘epic’ enough for this journey.",
        "Break time, please? My boots are crying.",
        "If I wanted to walk this much, I’d have joined the Argent Crusade.",
        "I think my feet are forming an alliance with the ground to rebel against me.",
        "Can we rest? Even my soul is exhausted.",
        "This trek is longer than any queue for Alterac Valley.",
        "Why walk when we can just hearthstone and call it a day?",
        "I’d give anything for a teleporter. Except gold. Never gold.",
        "I’d rather fight an elite than walk another step right now.",
        "Does anyone else think the journey is the real boss fight?",
        "If I don’t sit down soon, I’m going to collapse like an undergeared tank.",
        "How about a break? I’m starting to hallucinate shortcuts.",
        "Are we adventurers or marathon runners? My legs need answers.",
        "This road is longer than a raid explanation from a new guild leader.",
        "Do we even know where we’re going? Or are we just lost with style?",
        "I swear, I’m not lazy. The ground just hates me.",
        "Can someone summon me? I’m officially retiring from walking.",
        "I think my boots are plotting against me. They’ve had enough.",
        "If I sit down now, I may never get up again.",
        "Let’s take five. Even Azerothian heroes need to catch their breath.",
        "My legs feel like they’ve been cursed by a warlock.",
        "Next time, we bring a mage for portals. Agreed?",
        "This trek is the real final boss. Forget the dungeon.",
        "I’m about to throw my boots into the Maelstrom and call it a day.",
        "Are we going to walk forever? My knees don’t think this is sustainable.",
        "Break time? Or do I have to fake a death knight limp?",
        "If walking was a skill, I’d be maxed out by now.",
        "Do we really need to finish this quest today? My legs vote no.",
        "I’m not asking for much, just a nap and maybe a foot massage.",
        "My stamina bar is officially empty. Someone res me, please.",
        "Do you think the quest giver will notice if we just… don’t show up?",
        "If I had known adventuring meant walking, I’d have stayed in Dalaran.",
        "This is a cruel joke, right? We’re doing this for a green-quality reward?",
        "I’ve walked so much I think I’m starting to fade into the spirit realm.",
        "I’m pretty sure I could write a book about walking at this point.",
        "Is this an adventure or a walking simulator? Someone clarify.",
        "Is there a mount rental service nearby? My legs are filing a complaint.",
        "We’ve walked so far that even the mobs are starting to feel bad for us.",
        "If I have to take one more step, I’m switching to a ranged class.",
        "Does anyone else feel like Azeroth is too big today?",
        "Let’s take a break before my legs declare independence.",
        "Walking is overrated. Can someone carry me like a princess?",
        "I’d trade all my gold for a flying mount right now.",
        "Isn’t there a shortcut? There has to be a shortcut.",
        "I’d rather face a raid boss than this uphill climb.",
        "My boots are ready to retire. Can we rest for a moment?",
        "Even the critters are overtaking me. That’s how slow I am now.",
        "Why does every quest involve a journey longer than my patience?",
        "I didn’t sign up for a marathon. I signed up for loot.",
        "Who thought walking was a heroic activity? I want a refund.",
        "This is the last time I follow a map without a scale.",
        "How much further? My soul feels like it’s lagging behind.",
        "I’m starting to miss the lazy NPCs in their comfy inns.",
        "Walking might be free, but it’s costing me my sanity.",
        "Can we hire a goblin zeppelin for this? Anything but more walking.",
        "My legs feel like they’ve been flagged for PvP combat.",
        "I’ve faced dragons with less effort than this walk.",
        "This better lead to epic loot, or I’m boycotting all future quests.",
        "Walking builds character, they said. Lies. It builds blisters.",
        "Why does every good quest require crossing half of Azeroth?",
        "I need a mount, a nap, and a strong drink. In that order.",
        "I’ve trekked across three zones. This quest better reward more than a potato.",
        "Are we adventurers or delivery couriers? This feels like manual labor.",
        "I’d rather take a spirit res than keep walking.",
        "We’ve walked so far that even the light seems to be fading.",
        "This quest giver owes me a medal for this trek.",
        "I hope there’s a healer at the end, because my feet are critically injured.",
        "I’m about to invent a new teleport spell out of sheer desperation.",
        "How is it possible to walk downhill and still feel exhausted?",
        "If we don’t stop soon, I’m staging a sit-in protest.",
        "Does anyone have a hearthstone? Or a magical stretcher?",
        "Walking builds strength, right? Then I must be a raid boss by now.",
        "I think I’ve lost all sense of time and space. How long have we been walking?",
        "Let’s rest before we accidentally aggro gravity.",
        "Walking is fine, they said. It’s good for you, they said. Lies.",
        "I’m starting to think this path is endless. Did someone cast a loop spell?",
        "We’ve been walking so long that I forgot what we’re even questing for.",
        "I’m pretty sure this is a quest designed by someone who hates feet.",
        "Can we call this quest a draw and go back to town?",
        "I’ve got a better idea: Let’s just camp here forever.",
        "Why walk when you can teleport? Someone remind me why I’m a mortal.",
        "Even the wind sounds tired. Is it possible to exhaust the elements?",
        "I need to sit down before I collapse into a new questline.",
        "Let’s take a break. My legs feel like they’ve been hexed by a shaman.",
        "Whoever built this path owes us a ride. Seriously."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomEnemyRoarResponse()
{
    std::vector<std::string> responses = { 
        "For the Horde! Prepare to meet your end!",
        "For the Alliance! Face the wrath of the righteous!",
        "Lok’tar ogar! Victory or death!",
        "By Elune’s light, I shall strike you down!",
        "For Azeroth! You will fall today!",
        "Feel the fury of the Frostwolf Clan!",
        "The Light compels me to vanquish you!",
        "For Gnomeregan! You’ll regret underestimating us!",
        "I am the storm, and you are the prey!",
        "By the ancestors, I will crush you!",
        "Witness the fury of the elements!",
        "For the glory of Silvermoon!",
        "My blades thirst for your blood!",
        "The shadows claim you this day!",
        "For Teldrassil! Vengeance is nigh!",
        "You will know the true meaning of pain!",
        "I’ll send you to the nether myself!",
        "Let’s see if your skull is as thick as your arrogance!",
        "Suffer under the weight of my fury!",
        "I’ll carve your name into history… as a failure!",
        "Prepare to be unmade, whelp!",
        "The earth trembles beneath my rage!",
        "The sky will darken with your despair!",
        "You will regret facing me, fool!",
        "For the Sunwell! Your doom is sealed!",
        "Feel the burn of justice!",
        "Your end will be swift, but not painless!",
        "The ancestors guide my strength!",
        "Come at me, if you dare!",
        "The hunt begins, and you are the prey!",
        "You dare challenge me? Foolish choice!",
        "For Kul Tiras! Face the tides of vengeance!",
        "The spirits cry for retribution!",
        "You’re about to find out why they call me unstoppable!",
        "By the sands of Uldum, I will bury you!",
        "Your defeat will echo through eternity!",
        "You’ll beg for mercy before I’m done!",
        "For every innocent life you’ve taken, I’ll take tenfold vengeance!",
        "I am your reckoning!",
        "Prepare to be crushed under my heel!",
        "For Darnassus! I will avenge the fallen!",
        "You’ve picked the wrong warrior to cross today!",
        "Your arrogance will be your undoing!",
        "This is where your journey ends!",
        "For my people, I will show you no mercy!",
        "I’ll send your soul screaming into the void!",
        "By the flame of Ragnaros, burn!",
        "Cry for help, but no one will come!",
        "The last thing you’ll see is my blade!",
        "Come! Meet your doom at my hands!",
        "Today, your name is forgotten, and mine becomes legend!",
        "By the winds of Northrend, I will scatter your ashes!",
        "You’ll feel the weight of my hammer soon enough!",
        "Your defeat will echo across the ages!",
        "For Stormwind! For honor!",
        "I’ll paint the ground with your regret!",
        "The shadows whisper your doom, and I answer their call!",
        "For Thunder Bluff! You’ll not leave this battlefield!",
        "Face me, and face the wrath of a thousand battles!",
        "The elements scream for vengeance, and I will deliver it!",
        "Your blood will stain the sands of Silithus!",
        "By the Light, I will smite you where you stand!",
        "You’ll wish you stayed in your burrow, coward!",
        "I’ll break your resolve before I break your body!",
        "The Horde stands strong! You will not survive!",
        "The Legion taught me how to fight. Let me show you!",
        "For Gilneas! Your tyranny ends here!",
        "I’ll crush you like the vermin you are!",
        "Today, the void claims another victim!",
        "Your defeat is inevitable, your struggle pointless!",
        "By the oath of my ancestors, you will fall!",
        "For Ironforge! Feel the might of dwarven steel!",
        "I’ll send you back to your dark masters in pieces!",
        "Prepare for an end as bitter as your deeds!",
        "For every life you’ve taken, you’ll pay a thousandfold!",
        "The wilds call for your blood, and I will answer!",
        "You’ll meet the same fate as all who oppose the Alliance!",
        "For Azeroth, your tyranny ends now!",
        "Feel the fury of a dragon’s roar!",
        "I’ll send you running back to your pit of shadows!",
        "Your time is over, and your soul is forfeit!",
        "By the stars of Elune, your path ends here!",
        "This world doesn’t belong to you, and I’ll prove it!",
        "For Durotar! We fight with honor!",
        "You think you’re strong? Face me and prove it!",
        "The spirits guide my blade, and they are unforgiving!",
        "You’ll be dust beneath the feet of the Horde!",
        "By the tides of Kul Tiras, I will drown your ambitions!",
        "You’ll regret the day you crossed paths with me!",
        "The ancestors weep for your foolishness!",
        "I’ll tear down your arrogance piece by piece!",
        "The Light will sear your wickedness away!",
        "You’ll not leave this battlefield alive!",
        "For Quel’Thalas! The Sunwell strengthens me!",
        "Face your fate with courage, or die as a coward!",
        "I’ll feast on the victory of your defeat!",
        "I’ll make a trophy of your skull!",
        "The cycle of life and death ends with you!",
        "For Gnomeregan! Your doom is imminent!",
        "By the will of the Earth Mother, I stand resolute!",
        "Your evil ends here, and your terror dies with you!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomDanceResponse()
{
    std::vector<std::string> responses = { 
        "Watch me shake these moves! Even the Naaru are impressed!",
        "Dancing like no one’s watching… but everyone definitely is.",
        "My moves are as smooth as a mana potion sliding into my inventory!",
        "For the Horde! And for the groove!",
        "Spinning like a whirlwind! Or maybe just a twister spell!",
        "This rhythm could even wake the Lich King!",
        "Dancing better than Thrall’s political negotiations!",
        "Step aside! I’m the real Dance Commander of Azeroth!",
        "The tavern floors better be sturdy for these steps!",
        "Who says warriors can’t have grace? Watch me twirl!",
        "I’d challenge Arthas to a dance-off any day!",
        "Shaking my groove thing like the sands of Tanaris in a storm!",
        "Do these moves scream Alliance pride, or what?",
        "Even murlocs would be jealous of these wiggles!",
        "If dancing were loot, I’d be rolling a 100 every time!",
        "Cue the bards! This dance needs a proper soundtrack!",
        "These steps are hotter than Ragnaros’ core!",
        "Imagine if raids required dancing to beat the boss!",
        "Watch and learn, because this is art in motion!",
        "Channeling my inner troll dancer for maximum flair!",
        "If dancing’s a buff, consider me fully stacked!",
        "Step, slide, spin! Who needs a weapon when you’ve got rhythm?",
        "Dancing circles around you! Literally!",
        "This is how you aggro attention in a crowd!",
        "Dancing to the beat of my own victory anthem!",
        "I’d trade a rare drop to see Thrall attempt this spin!",
        "Imagine the loot table for these epic dance moves!",
        "Who needs a sword when you can slay on the dance floor?",
        "If you can’t join me, at least try to keep up!",
        "Even gnomes are taller than my standards for bad dancing!",
        "These steps? Legendary tier.",
        "When I dance, even Deathwing nods in approval!",
        "I’m not just dancing; I’m redefining movement itself!",
        "If Sargeras could see this, he’d surrender in rhythm!",
        "Teldrassil burned, but these moves are still fire!",
        "Do you feel that? It’s the ground quaking from my awesomeness!",
        "Dancing through Azeroth, one flawless step at a time!",
        "Elune guide my steps, because these are divine!",
        "Even goblins couldn’t engineer moves this slick!",
        "This is how you earn a spot in the Great Dance Hall of history!",
        "Who says undead can’t move? Watch these bones groove!",
        "Consider this my war dance. Enemies, beware!",
        "I’m the light in the disco inferno!",
        "They said to stay in my lane, so I made it a dance floor!",
        "These steps are rare drops from the Groove Boss himself!",
        "You thought you’d seen it all? Then I started dancing!",
        "This rhythm is pure arcane magic!",
        "Even the void can’t resist tapping along to these steps!",
        "My dance card’s full, but I might make room for another encore!",
        "Witness me! The champion of dance across all factions!",
        "La-la-laaaa, I'm dancing through the battlefield!",
        "Oh, a tune as sweet as a Silvermoon harp!",
        "Laaa-dee-da-da, my feet are unstoppable!",
        "Hum-hum-hummm, can't stop the rhythm in my soul!",
        "Do-do-do-do, my moves are legendary!",
        "Tra-la-la-la, spinning like a whirlwind in Nagrand!",
        "Bum-bum-ba-dum, feel the beat of Azeroth!",
        "Laaaaa, even the void sings along with me!",
        "Oooooh-yeah, this groove is eternal!",
        "Dum-dum-dee-dum, my steps echo through the Barrens!",
        "Tee-hee-hee, dance with me under Elune’s light!",
        "Dee-da-dee-da, watch these boots make history!",
        "Hmmm-mmm-mmm, this is the song of my people!",
        "Ohhh-ahhh, Azeroth’s rhythm calls me!",
        "Do-re-mi, even the Naaru are in harmony!",
        "Tra-la-la-la-la! No one can resist this groove!",
        "Boom-chicka-wow, this is my battle cry… and dance!",
        "Da-da-da-da, keep up or get left behind!",
        "La-di-da-di, spinning circles like a mage’s blink!",
        "Fa-la-la-la-la, the festival spirit’s in me!",
        "Shoo-bee-doo-wop, I’m the bard of the battlefield!",
        "Ooooh-eee, let’s turn this place into a party!",
        "Loo-loo-looo, even Illidan would dance to this!",
        "Bop-bop-ba-da, this rhythm is unbeatable!",
        "Twirling and singing, oh what a life!",
        "Mmmm, mmmm, this beat is making me giddy!",
        "Ooo-aaa-ooo, even dragons can’t resist this tune!",
        "Hmmm-la-da-da, dancing through the Dark Portal!",
        "Doo-da-doo, my steps echo in the halls of Karazhan!",
        "Traa-la-la, these moves are a gift from the Titans!",
        "Ahhh-ohhhh, every step is a tribute to Azeroth!",
        "La-la-la, let the murlocs sing with me!",
        "Bum-ba-bum-bum, the sound of victory and flair!",
        "Tee-ya-ha! This dance is chaos incarnate!",
        "Ooooh-whoooo, even the Horde would cheer for this!",
        "Dee-dee-da-da, dancing my way to Exodar!",
        "Tra-la-la-la-laaaa, every move is magic!",
        "Laaa-deee-daaaa, spinning like the winds of Mulgore!",
        "Mmm-mmm-mmm, the rhythm of the Emerald Dream!",
        "Zing-zang-zong, this tune could wake the Titans!",
        "Sha-la-la, Azeroth has never seen moves like these!",
        "Ohhh-hum-da-dum, these steps could charm even Sylvanas!",
        "Woo-hoo-hooo, feel the beat of the Horde’s drum!",
        "Loo-lee-lay, let’s dance under Teldrassil’s ashes!",
        "Do-do-dee-do, twirling to the rhythm of the Light!",
        "Aah-ooh-oooh, my song echoes in the Caverns of Time!",
        "Tra-da-da-da-da, dancing circles around my enemies!",
        "Ho-hum-hum, the ballad of the brave and rhythmic!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomLoveResponse()
{
    std::vector<std::string> responses = { 
        "You make every day feel like a gift from the Light.",
        "I couldn’t ask for a better companion through Azeroth’s trials.",
        "Your kindness shines brighter than any artifact I’ve ever found.",
        "Having you by my side makes even the longest journey worthwhile.",
        "You’re my anchor in the chaos of this world.",
        "Every moment with you feels like a piece of paradise.",
        "I am endlessly grateful for your presence in my life.",
        "You’re the spark that keeps the fire in my heart burning bright.",
        "With you, even the darkest corners of Azeroth feel safe.",
        "I don’t say it enough, but you mean the world to me.",
        "Your loyalty is stronger than any shield I’ve ever wielded.",
        "Thank you for being my beacon of hope in troubled times.",
        "I treasure every memory we’ve made together.",
        "You’re the reason I keep fighting, keep exploring, keep believing.",
        "Your friendship is worth more than all the gold in Azeroth.",
        "I hope you know how much you’re valued and loved.",
        "You’ve taught me that the greatest treasure isn’t found in a chest, but in the bonds we share.",
        "Your courage inspires me every single day.",
        "Thank you for being the steady ground beneath my feet.",
        "You’re a light in the dark, a guide when I’m lost, and a comfort in the storm.",
        "You’ve been my strength when I felt weak, my hope when I felt lost.",
        "Every time you smile, it feels like the sun breaking through the clouds.",
        "I’m grateful for every laugh, every tear, every moment we’ve shared.",
        "You make the battles worth fighting and the victories worth celebrating.",
        "In you, I’ve found a rare and priceless connection.",
        "You’ve been my shield, my sword, and my sanctuary.",
        "You remind me that even in a war-torn world, there’s beauty and love to be found.",
        "You’ve walked beside me through the fire, and I’ll never forget it.",
        "You’re the truest ally I’ve ever had, and I’m blessed to know you.",
        "If I could, I’d write songs of your bravery and kindness.",
        "Thank you for standing with me when others would have turned away.",
        "You’ve been my home in this ever-changing world.",
        "I’d face a thousand raids if it meant keeping you safe.",
        "You’ve given me more than I ever thought possible: hope, love, and a place to belong.",
        "Your presence turns a house into a home, a battlefield into a haven.",
        "I’ve seen many wonders in this world, but none as remarkable as you.",
        "You’re the reason I believe in the good that still exists in Azeroth.",
        "Every challenge feels easier knowing you’re by my side.",
        "You’ve shown me that true strength lies in the heart, not the sword.",
        "You’re a treasure beyond any loot I could ever find.",
        "Your compassion is a rare and beautiful gift.",
        "I’m lucky to call you my friend, my ally, my family.",
        "You’ve reminded me what it means to truly live, not just survive.",
        "The bond we share is unbreakable, and for that, I’m endlessly grateful.",
        "You make this world feel a little less broken and a lot more beautiful.",
        "Thank you for being the light that guides me home.",
        "Your heart is as vast and endless as the seas of Azeroth.",
        "I’ll never stop being grateful for your presence in my life.",
        "You’re the reason I believe that even in war, love and kindness endure.",
        "You’re the calm in the storm, and I’m grateful for you every day.",
        "If gratitude could be bottled, I’d need an endless flask for you.",
        "Walking beside you makes every step feel lighter.",
        "You’re the reason Azeroth feels like home to me.",
        "Your kindness has healed more wounds than any spell.",
        "Through the darkest dungeons and highest peaks, you’ve always been there.",
        "Every victory is sweeter because I get to share it with you.",
        "You’re the heart of our group, and I’d be lost without you.",
        "The battles may be tough, but knowing you’re here gives me strength.",
        "You’re the most precious gem in Azeroth, and I treasure you.",
        "Your friendship is worth more than any legendary drop.",
        "With you, every challenge feels conquerable, every day brighter.",
        "You’ve made me believe in heroes, because you’re one to me.",
        "Your courage inspires me to face even the fiercest foes.",
        "Thank you for always having my back, no matter what.",
        "You’re the star that lights my way through the shadows.",
        "Every quest feels less daunting with you by my side.",
        "Your words have been the healing potion my heart needed.",
        "Even in chaos, you’re my constant, my peace.",
        "I’d face an army of demons just to keep you safe.",
        "You remind me that there’s always something worth fighting for.",
        "Your laugh is the sound I didn’t know I needed to hear.",
        "Every raid, every adventure, is better because you’re part of it.",
        "You’ve turned a simple life into an extraordinary journey.",
        "Thank you for turning moments into memories I’ll treasure forever.",
        "Your loyalty is the rarest gift, and I cherish it deeply.",
        "You’re the reason I keep believing in the good in this world.",
        "Your spirit is unbreakable, and it inspires me every day.",
        "With you, I’ve learned that the greatest loot is the bonds we forge.",
        "You’ve turned even the hardest battles into stories worth telling.",
        "I couldn’t imagine Azeroth without your light guiding the way.",
        "Your presence makes even the Barrens feel alive and full of color.",
        "You’ve been my shield, my sword, and my sanctuary in this war-torn land.",
        "Every time I look at you, I see hope, strength, and love.",
        "I’d cross all of Kalimdor just to hear you laugh again.",
        "Your words have lifted me more times than I can count.",
        "You’ve shown me what true strength looks like: compassion and love.",
        "Your heart is as vast as the plains of Mulgore, and I’m grateful to know it.",
        "With you, I’ve learned that the journey matters more than the destination.",
        "You’ve turned every ordinary day into something extraordinary.",
        "Thank you for being my companion, my friend, my family.",
        "Your presence is the light in the darkest corners of Azeroth.",
        "I’ll never stop fighting for you, as you’ve always fought for me.",
        "You’ve been my anchor in the storms of life, and I’m forever grateful.",
        "Your bravery is unmatched, and your kindness is unparalleled.",
        "You’ve made every battle worth it, every scar meaningful.",
        "With you, I’ve found more than an ally—I’ve found a kindred spirit.",
        "Thank you for reminding me that love and loyalty are the strongest forces of all.",
        "You’re the reason I keep fighting, keep hoping, and keep dreaming."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomFlexResponse()
{
    std::vector<std::string> responses = { 
        "Behold these muscles—crafted by the gods themselves.",
        "Strength like mine isn’t just natural—it’s legendary.",
        "I don’t need armor; my biceps are my shield.",
        "Flexing is just my way of saying hello.",
        "When the Light forged champions, they started with me.",
        "Feel that? That’s pure, unfiltered power.",
        "Who needs magic when you’ve got these guns?",
        "I’ve crushed boulders smaller than that with one hand.",
        "You see strength; I call it my everyday.",
        "Even dragons think twice before facing me.",
        "I don’t lift weights. Weights lift me.",
        "Every flex is a gift to Azeroth—don’t waste it.",
        "You call that strong? Watch and learn, amateur.",
        "When I flex, the Earth trembles in respect.",
        "These abs? Chiseled from the stones of Uldum.",
        "I’m a force of nature—unstoppable and undeniable.",
        "My strength isn’t just physical; it’s mythic.",
        "I’ve arm-wrestled ogres and made them cry.",
        "I’m not just strong; I’m strength incarnate.",
        "Flexing is a public service—I’m inspiring you.",
        "Strength like mine makes mountains jealous.",
        "I once flexed, and an Old God blinked first.",
        "Why be modest when you’re built like a titan?",
        "Muscles like these aren’t trained; they’re destined.",
        "You’re looking at the future of strength—me.",
        "Power like this should come with a warning label.",
        "If you’re not impressed, you’re blind.",
        "Even the Naaru look at me and say, ‘Well done.’",
        "Flexing isn’t a choice—it’s a responsibility.",
        "I’m not bragging; I’m just stating facts.",
        "Don’t hate me because I’m swole.",
        "The Legion fled because they saw me warming up.",
        "Every swing of my weapon is a masterpiece of strength.",
        "I’ve been mistaken for a mountain once or twice.",
        "Legends are written about strength like mine.",
        "Even my shadow looks buff.",
        "I could carry the Exodar on my back if needed.",
        "When I flex, the winds whisper my name.",
        "They say humility is a virtue; I say strength is better.",
        "I don’t just lift weights—I lift morale.",
        "The Titans themselves would envy these gains.",
        "Strength like mine is the stuff of bard songs.",
        "I don’t sweat; I glisten with dominance.",
        "When you’ve got it, flaunt it—and I’ve got it all.",
        "Being this strong should be illegal, but here I am.",
        "They say power corrupts, but I’m incorruptible.",
        "Flexing isn’t showing off; it’s art.",
        "I’ve got muscles on my muscles.",
        "When I flex, even the Light pauses to admire.",
        "You’re welcome for the free show.",
        "This isn’t just strength—it’s a lifestyle.",
        "Even my armor asks for permission to touch me.",
        "Do you even lift? Because I sure do.",
        "The only thing bigger than my muscles is my confidence.",
        "Strength like this runs in my bloodline.",
        "Every push-up I do pushes the world down.",
        "My strength speaks for itself—loudly.",
        "I don’t flex to brag; I flex to inspire.",
        "I once lifted a Kodo just to see if I could.",
        "Dragons avoid me because they know they’ll lose.",
        "Flexing isn’t just a choice—it’s destiny.",
        "Every enemy I face becomes part of my legend.",
        "You don’t train for strength like mine—you’re born with it.",
        "Even the elements bow to my power.",
        "I don’t walk into battle—I strut.",
        "When I show up, even the earth takes notice.",
        "I’ve been told my biceps have their own gravitational pull.",
        "I don’t need spells; I cast pure strength.",
        "My weapon may be strong, but my arms are stronger.",
        "When I punch, even the Light stumbles.",
        "Strength like mine turns allies into admirers.",
        "You think you’re tough? That’s cute.",
        "If strength was currency, I’d be richer than a goblin.",
        "Legends say I can bench press the Sunwell.",
        "My presence alone makes enemies rethink their lives.",
        "I’m the standard other warriors aspire to.",
        "Even a Fel Reaver wouldn’t dare mess with me.",
        "I’ve broken shields with a handshake.",
        "Every battle I fight is a show of dominance.",
        "Strength isn’t just my asset—it’s my identity.",
        "They say heroes are born, not made. I’m proof.",
        "I’m so strong, my reflection flexes back at me.",
        "I’m not showing off—it’s just hard to hide perfection.",
        "When I flex, even Thunder Bluff feels the quake.",
        "I don’t just lift spirits—I lift entire armies.",
        "Even my footsteps leave an impression—literally.",
        "Strength like mine makes myths jealous.",
        "I once wrestled an Ettin and won in five seconds.",
        "The only thing I can’t lift is someone else’s ego.",
        "Every time I flex, Azeroth becomes a little safer.",
        "When you’ve got strength like this, modesty is optional.",
        "I’m not cocky—just incredibly self-aware.",
        "Every scar on my body is a story of triumph.",
        "I’m the reason bosses drop loot—they’re scared of me.",
        "Even the Titans would ask me for tips on strength.",
        "Strength is temporary; being me is forever.",
        "When I flex, even Grommash would nod in approval.",
        "They say strength comes in numbers—I’m the exception.",
        "I once carried a raid group—literally."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomBlameResponse()
{
    std::vector<std::string> responses = { 
        "Hey, don’t look at me—I wasn’t even there!",
        "It wasn’t my fault! Blame the tank!",
        "Who pulled the boss early? Definitely not me.",
        "I swear, it was lag. Honest.",
        "That wasn’t me—it was the rogue sneaking around.",
        "Blame the healer! I saw them smirking.",
        "Look, I’m just here to hit stuff. Blame strategy on someone else.",
        "The fire moved under me, I didn’t move into it!",
        "Wait, you’re saying *I* triggered the trap? No way.",
        "Oh, so now everything’s my fault? Typical.",
        "I didn’t press that button, I swear.",
        "If I caused that wipe, then I’m the Lich King. See? Impossible.",
        "Let’s not play the blame game here. We’re all adults… right?",
        "Someone must’ve fat-fingered it. Definitely wasn’t me.",
        "I think we all know who really messed up. *side-eye*",
        "You’re blaming me for that? I wasn’t even looking!",
        "Let’s agree it was a team mistake and move on. *cough* Not me though.",
        "It’s the gnome’s fault. Always is.",
        "Wait, I thought this was a safe spot!",
        "I might’ve been standing there, but it was *definitely* RNG.",
        "I’m innocent until proven guilty. And you’ve got no proof.",
        "It’s called tactical misjudgment, not failure.",
        "Sure, blame the DPS. We’re used to it.",
        "I thought someone else was handling that.",
        "Why does it always come back to me?",
        "I didn’t run into the AoE, the AoE ran into *me*.",
        "I was following instructions! Maybe the instructions were bad.",
        "Alright, let’s not overreact. It wasn’t *that* bad.",
        "Fine, blame me if it makes you feel better.",
        "Hey, I’m just here for loot, not accountability.",
        "That explosion wasn’t even close to me!",
        "If I caused the problem, why am I so cool about it?",
        "Let’s focus on solutions, not pointing fingers at me.",
        "I may have… *accidentally*… clicked something.",
        "Wait, there was a strategy? Nobody told me!",
        "If the tank dies, it’s clearly not the DPS’s fault.",
        "I blame the moonkin. Always watching, always judging.",
        "Someone kited the adds to me! What was I supposed to do?",
        "Hey, if you wanted perfection, you shouldn’t have invited me.",
        "Whoops. Oh wait, I mean… wasn’t me.",
        "Blame RNG, not me.",
        "You’re saying *I* pulled aggro? Lies.",
        "I was lagging, I swear!",
        "I wasn’t even in the room when it happened!",
        "Okay, I *might’ve* done something. But it was minor.",
        "Are we sure that wasn’t a bug?",
        "What’s important is that we learn from this. Not whose fault it is.",
        "I might’ve been the cause, but let’s blame the raid leader.",
        "It’s a feature, not a mistake!",
        "Listen, if it was my fault, it wouldn’t have looked so cool.",
        "The fire was part of my plan. You just didn’t see the vision.",
        "Fine, I pulled early. But did you see the crits I hit after?",
        "You say ‘mistake,’ I say ‘happy accident.’",
        "I didn’t know standing there would do that. Nobody told me!",
        "I was testing the raid's reaction time, and you all failed.",
        "Blame the warlock’s pet. It looked shifty.",
        "Sure, I touched the button, but who placed it there, huh?",
        "Can’t we all agree the mechanics are poorly designed?",
        "Oh, sorry, I thought the boss was already pulled.",
        "That was a trap? I thought it was loot!",
        "If you blame me, you’re just jealous of my DPS.",
        "What do you mean, ‘don’t stand there’? It’s my favorite spot!",
        "I’m a rogue. We’re supposed to cause a little chaos.",
        "Okay, but did we wipe stylishly? Because that matters.",
        "I was doing fine until someone distracted me!",
        "Was it my fault? Debatable.",
        "Why do the mechanics hate me specifically?",
        "I might have blinked into the mob pack… but that’s just mage things.",
        "I thought we were improvising. My bad.",
        "If you’re mad, blame the tank for not keeping me alive.",
        "Oh, so now I’m the scapegoat? Typical.",
        "Was that fire bad? It looked warm and inviting.",
        "If I caused it, it’s because the boss was rude first.",
        "I was testing our healer’s limits. They passed, barely.",
        "It’s called trial and error. Emphasis on trial.",
        "How was I supposed to know the AoE was invisible?",
        "I pulled, but only to make things exciting.",
        "Blame the moon phase or something. It’s not on me.",
        "That wipe was a group effort, but sure, blame me.",
        "I thought someone else was handling mechanics.",
        "If the game didn’t want me to press it, why is it clickable?",
        "It’s not my fault the boss targeted me specifically!",
        "Honestly, I blame the guild leadership for this one.",
        "Look, the trap was clearly not labeled properly.",
        "How was I supposed to know that lever wasn’t decorative?",
        "I’m not saying it was my fault, but I *was* in the room.",
        "What do you mean ‘stack up’? I thought you said ‘spread out.’",
        "I assumed you all had it under control. My bad.",
        "The real problem is that the boss hates me personally.",
        "I read the strategy, but I also ignored it.",
        "Fine, I’ll take the blame, but only this time.",
        "The real issue is the healer wasn’t healing *me*.",
        "Who even placed that trap there? Unfair design.",
        "You should all be thanking me for spicing up the fight.",
        "The strat was fine; the execution? A little shaky.",
        "That’s not a wipe—that’s a learning experience.",
        "I swear I was out of that AoE.",
        "I blame latency. Or my cat. Or both."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomStinkyResponse()
{
    std::vector<std::string> responses = { 
        "Ugh, what’s that smell? Did a kodo die in here?",
        "Who brought the fish feast and forgot to cook it?",
        "If stench was a debuff, we’d all be dead.",
        "Someone’s armor needs a good scrub. Not naming names…",
        "By Elune, who smells like wet fur?",
        "This place reeks of failure. Or maybe that’s just me.",
        "The smell of burnt murloc isn’t as bad as you’d think.",
        "Did the rogue forget to bathe again? Typical.",
        "My nose is offended. Someone needs to fix this.",
        "Who needs crowd control when your stink clears the room?",
        "I think the boss farted. We’re doomed.",
        "What’s worse—the dungeon mobs or your boots?",
        "If I wanted to smell this bad, I’d visit Undercity.",
        "The smell of victory would be nice, but this is not it.",
        "Was that a spell or did someone have beans?",
        "This stench is worse than a forsaken after a rainstorm.",
        "Is it me, or does this raid smell like desperation?",
        "You call this a campfire? It smells like burnt socks.",
        "Why does Orgrimmar always smell like grilled worg?",
        "I’m 90% sure that’s a dead troll over there… or something close.",
        "Someone left a health potion out too long. It’s fermenting.",
        "This stench could peel the paint off a siege engine.",
        "It smells like a goblin invention gone wrong in here.",
        "Who thought it was a good idea to bring durian?",
        "This isn’t the smell of success. It’s just sweat and regret.",
        "Every time I come here, it smells worse than the last.",
        "If I faint, it’s the smell, not the mechanics, I swear.",
        "Was that sulfur or did someone have an ‘accident’?",
        "You’d think Nagrand would smell fresh, but no—it’s the clefthoofs.",
        "Who needs a gas trap when you’ve got this natural funk?",
        "If we win this fight, I demand air fresheners in the loot.",
        "Smells like someone pulled a rare mob… or their socks.",
        "If the Alliance smelled this bad, I’d surrender on principle.",
        "This is worse than the sewers under Dalaran. How is that possible?",
        "Someone smells like they rolled in Gnomeregan sludge.",
        "I don’t know what’s in that alchemy pouch, but it’s rank.",
        "I’d blame the hunter’s pet, but it’s probably the hunter.",
        "We need less bloodshed and more lavender oil in here.",
        "The scent of Azeroth’s finest… needs improvement.",
        "By the Light, this stench could down a fel reaver.",
        "You smell like you fell into the Dark Portal and back.",
        "Is that the smell of fear or just bad hygiene?",
        "This stink could summon an Old God. Be careful.",
        "Did someone forget to clean their mana crystals again?",
        "This must be the scent of Horde victory. Gross.",
        "How do you manage to smell bad through full plate armor?",
        "Do undead even notice how bad they smell? Asking for a friend.",
        "This isn’t the smell of glory; it’s the smell of old socks.",
        "If stench were a weapon, we’d have wiped already.",
        "Who brought the Limburger cheese? Oh, wait, that’s just you.",
        "This smell could kill a dungeon boss. Who’s responsible?",
        "Someone’s cloak smells like it was dipped in swamp water.",
        "By the Sunwell, I’d rather smell fel energy than this.",
        "Does someone have a debuff, or is that just the rogue again?",
        "My armor doesn’t even rust, and I can still smell this.",
        "Who thought farting in the middle of combat was a good idea?",
        "This raid smells like defeat and unwashed socks.",
        "Someone’s boots have clearly never seen a repair vendor.",
        "This stench makes me nostalgic for the sewers of Undercity.",
        "If the enemy doesn’t flee, it’s because they lost their sense of smell.",
        "Even the murlocs are gagging. That’s saying something.",
        "Whoever forgot to clean their mounts should be ashamed.",
        "It smells like a forsaken alchemy experiment gone horribly wrong.",
        "Is it supposed to smell this bad, or is this just a Horde thing?",
        "Who needs a stink bomb when we’ve got you in the group?",
        "This isn’t the fragrance of victory; it’s the perfume of despair.",
        "Did someone spill mana potions, or is this the smell of failure?",
        "If smell could crit, we’d all be dead by now.",
        "I didn’t know despair had a scent until today.",
        "This smell is so bad, even the critters are leaving.",
        "Who put rotten meat in their bags? Oh, it’s you.",
        "This funk could put a dreadlord into a coma.",
        "It’s not a plague—it’s just you. Please bathe.",
        "Even my undead nose can’t take this anymore.",
        "Do trolls not believe in soap, or is this just bad luck?",
        "Is this the smell of war, or did someone forget to wash their tabard?",
        "This group reeks of unwashed adventurers and stale bread.",
        "I thought fel energy smelled bad. I was wrong.",
        "Who smells worse, you or the corpse we just looted?",
        "The boss isn’t fleeing because we’re strong; it’s this stink.",
        "This isn’t a raid; it’s a stench-filled nightmare.",
        "Why does this place smell like burnt fur and shame?",
        "If this stench was loot, it’d be grey quality.",
        "This smell alone could wipe the Alliance.",
        "Does the Light forgive this level of funk?",
        "This must be the true curse of Ulduar—this smell.",
        "Even the mages can’t conjure a breeze to clear this.",
        "I thought the swamp in Dustwallow was bad, but this wins.",
        "Who thought a gas mask should be required gear for this raid?",
        "This isn’t a dungeon—it’s an olfactory assault.",
        "The only loot we’ll find here is a scented candle.",
        "My worgen nose can’t handle this much longer.",
        "Someone call an alchemist; this place needs a cleansing potion.",
        "If Azeroth had air fresheners, this would be the place to use them.",
        "Whoever left their lunch to rot in their bag, please stop.",
        "If you’re proud of this stink, you need to rethink your life choices."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomJokeResponse()
{
    std::vector<std::string> responses = { 
        "That joke hit harder than a frostbolt crit.",
        "If jokes were loot, that one’s a grey item.",
        "I laughed so hard, I almost aggroed a patrol.",
        "Your humor is rarer than a Black Temple transmog drop.",
        "That joke was so bad, even the Lich King rolled his eyes.",
        "Do you specialize in cringe, or is it just a natural talent?",
        "I’d give you a standing ovation, but I’m stuck in combat.",
        "That’s the kind of joke you hear in Razorfen Kraul and never forget.",
        "Even a kobold wouldn’t steal that punchline.",
        "Was that joke in the patch notes? Because it’s broken.",
        "That one was funnier than a gnome trying to reach a shelf.",
        "Your sense of humor must be enchanted with -10 to comedy.",
        "I’d laugh, but I’m saving my energy for the raid boss.",
        "That joke’s DPS was so low it barely broke a sweat.",
        "If you keep this up, the raid leader’s going to call for silence.",
        "Are you trying to out-pun a goblin engineer? Bold move.",
        "That was so dry, even Tanaris felt moist by comparison.",
        "Your punchlines have cooldowns longer than my abilities.",
        "That joke belongs in the dungeon finder’s ‘toxic’ category.",
        "You’d make a great court jester for King Anduin.",
        "That’s the kind of humor that makes Arthas cry inside.",
        "If jokes were quests, I’d abandon that one immediately.",
        "Even the murlocs think that was gibberish.",
        "Was that a joke, or are you just testing my patience?",
        "You must be specced into Awkwardness—maxed out, too.",
        "That’s as funny as forgetting to set your hearthstone.",
        "Did you get that joke from the goblin bargain bin?",
        "If sarcasm were an art, you’d be a legendary crafter.",
        "That joke made me want to reroll a new character.",
        "Is this a stand-up routine, or are we wiping the floor soon?",
        "Your humor hits like a wet noodle, but I’ll allow it.",
        "Did you just crit me with awkward silence?",
        "That joke was so slow, even a turtle mount would beat it.",
        "Are we laughing at you or with you? I can’t tell anymore.",
        "That joke’s about as original as a murloc sound effect.",
        "Your jokes belong in a boss encounter—everyone dies a little inside.",
        "Even the ogres in Dire Maul would call that joke dumb.",
        "I’d give you a pity laugh, but I’m all out of mana.",
        "If cringe were currency, you’d be Exalted.",
        "That’s the kind of joke that makes me want to hearth out.",
        "Do you tell jokes professionally, or is this an unpaid talent?",
        "That was so bad, it might be considered a debuff.",
        "You’re the kind of person who rolls ‘Need’ on bad jokes.",
        "If humor were a stat, you’d be wearing all vendor gear.",
        "That joke hit as hard as a failed interrupt on a raid boss.",
        "Your timing’s worse than a tank pulling before the healer is ready.",
        "Even the Auction House wouldn’t bid on that joke.",
        "If I had a silver for every bad joke, I’d be rich from just listening to you.",
        "That joke was so bad it aggroed the next pack of mobs.",
        "Why don’t murlocs ever tell secrets? Because they’d just gargle it out!",
        "What do you call a Tauren rogue? A failure at stealth!",
        "Why did the paladin bring a ladder to the dungeon? To climb up on their high horse!",
        "How do you know a gnome was in your house? The furniture legs are all sawed off!",
        "Why don’t undead make good chefs? Everything they make tastes like graveyard dirt!",
        "What’s a troll’s favorite holiday? April Fool’s—they love the pranks!",
        "How does a goblin measure success? In explosions per second!",
        "Why don’t night elves trust the moon? It’s always going through phases.",
        "Why did the warrior bring a fishing pole to the raid? Because they heard it was a catch-and-release fight.",
        "What’s an orc’s favorite drink? Warchief’s Whiskey!",
        "Why don’t dwarves do yoga? Because they don’t like getting out of their comfort stone!",
        "What’s a mage’s favorite type of music? Frost beats!",
        "How does a rogue introduce themselves? ‘I’ll be behind you shortly.’",
        "Why did the druid go to therapy? They couldn’t bear the pressure of shapeshifting.",
        "What’s a priest’s favorite vegetable? Holy sprouts!",
        "Why did the warlock get kicked out of the party? They kept summoning bad vibes.",
        "What’s a shaman’s favorite type of storm? A thunderclap party!",
        "Why don’t hunters trust their pets? Too many ‘paws’ in judgment.",
        "Why did the Lich King break up with his girlfriend? She kept giving him the cold shoulder.",
        "What do you call a lazy death knight? Frosty the slow-man.",
        "Why do druids hate parking tickets? They’d rather be in treant mode.",
        "Why don’t goblins ever get invited to parties? They always blow things out of proportion.",
        "What do you call a group of angry paladins? A crusade for better puns!",
        "Why do undead never smile? They’ve got resting death face.",
        "How do orcs keep their hair green? With Warchief-brand shampoo!",
        "Why did the warrior fail at stand-up comedy? They couldn’t execute a good punchline.",
        "Why did the elf bring a mirror to the raid? They wanted to reflect on their actions.",
        "What’s a mage’s favorite dessert? Arcane cupcakes—they’re magical!",
        "Why did the rogue bring a ladder to the dungeon? They wanted to backstab at new heights.",
        "Why did the paladin get stuck in the closet? They couldn’t bubble their way out.",
        "Why don’t druids get speeding tickets? They just shapeshift and fly away.",
        "What do warlocks write in their diaries? Dark secrets and fel feelings.",
        "Why do death knights make bad gardeners? Everything they touch withers away.",
        "What’s a hunter’s favorite subject in school? Target practice!",
        "What’s a troll’s least favorite time of day? Sunrise—it ruins their vibe.",
        "Why did the gnome build a robot? To have someone to look up to.",
        "Why don’t draenei get lost? They always follow the light.",
        "Why did the goblin refuse to invest in gold? They said it was too mainstream.",
        "What’s a rogue’s least favorite thing? An audience with eyes in the back of their heads.",
        "Why did the undead player sit alone at the party? Nobody wanted to dance with a corpse.",
        "What’s a blood elf’s worst fear? A bad hair day.",
        "Why did the priest refuse to join the dungeon? They were afraid of getting too much holy aggro.",
        "What’s a warrior’s favorite workout? Cleave squats.",
        "Why don’t mages make good bartenders? They’re always throwing frostbolts at the ice machine.",
        "Why did the troll bring a drum to the meeting? To keep the beats tribal.",
        "What’s a tauren’s favorite type of joke? Anything pun-chy—like hoofing it!",
        "Why did the shaman open a bakery? They specialize in elemental pies.",
        "What’s a night elf’s favorite part of the forest? The treetops—it’s where they leaf all their worries behind!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomChickenResponse()
{
    std::vector<std::string> responses = { 
        "You couldn’t hit me if I were AFK!",
        "Nice DPS, I’ve seen critters do more damage.",
        "Is that your best gear, or are you trying out for a clown show?",
        "My grandmother could tank better than you, and she’s a healer.",
        "Do you even know what a rotation is, or are you just button mashing?",
        "You call that a taunt? My pet growls better.",
        "Wow, you’re about as threatening as a baby murloc.",
        "Is that lag, or are you just that slow?",
        "You must be using a trackpad with those movement skills.",
        "What’s the cooldown on you actually being useful?",
        "Your loot roll is as unlucky as your attempts at humor.",
        "Are you trying to miss, or is it just natural talent?",
        "What’s your crit chance? Zero?",
        "You’ve got the survival instincts of a lemming.",
        "That’s a nice transmog... for a blindfolded goblin.",
        "You play like you’ve never heard of a strategy guide.",
        "If we had to rely on you, we’d be wiping all day.",
        "Is your health bar supposed to be a rollercoaster?",
        "Your idea of crowd control is letting everyone attack you at once, huh?",
        "You tank like a wet paper bag.",
        "Is this your first time healing, or are you just experimenting?",
        "Nice pull! I didn’t know we were trying to aggro the whole zone.",
        "Your positioning is so bad, even NPCs laugh at you.",
        "Your damage output is as low as a gnome’s ceiling.",
        "You couldn’t dodge a wind-up attack if it came with a countdown.",
        "What’s your spec again? Because it sure isn’t working.",
        "Are you even trying, or is this just a social experiment?",
        "Your class fantasy must be to make others carry you.",
        "Your tanking is so bad, the mobs feel sorry for us.",
        "I’d call your rotation clunky, but that would be an insult to rocks.",
        "Your idea of utility is standing there looking pretty.",
        "You must be new here because you’re lost, buddy.",
        "Thanks for that misdirect... to yourself.",
        "You’re about as useful as a hunter with no ammo.",
        "Oh, I didn’t know we were raiding with a boss mechanic simulator—you.",
        "Your gear score might be high, but your skills are level 1.",
        "How do you even manage to hold aggro? Wait, you don’t.",
        "You’ve got all the finesse of a charging kodo in a china shop.",
        "At least you’re consistent—consistently bad.",
        "Did you roll your stats with a 1-sided die?",
        "Wow, you’re so bad, even the NPCs are facepalming.",
        "Your AoE is about as wide as a toothpick’s shadow.",
        "If sarcasm were damage, you’d be a top DPS.",
        "Are you playing a healer or just casting 'Judgment' all raid?",
        "Don’t worry, your carry service invoice is in the mail.",
        "You’re about as effective as a turtle on a treadmill.",
        "Let me guess, your keybinds are all set to /dance.",
        "If this was a competition for last place, you’d still lose.",
        "Nice raid awareness—were you raiding blindfolded?",
        "Your excuses have more uptime than your buffs.",
        "Is that all you’ve got? Even a kobold swings harder than you!",
        "I’ve seen more fearsome foes in Elwynn Forest.",
        "Your sword arm is about as steady as a drunken dwarf on a keg.",
        "By the Light, your form is a disgrace to warriors everywhere.",
        "You fight like a Forsaken after losing their last limb.",
        "If insults were weapons, I’d still outclass you.",
        "That shield of yours wouldn’t stop a stiff breeze.",
        "Your battle cry is more of a whimper, isn’t it?",
        "Even Murlocs don’t tremble at your approach.",
        "The ancestors weep at your lack of skill.",
        "Careful, or your blunders will be the stuff of tavern tales.",
        "Is your armor made of tin? Because it crumples like it.",
        "Do you think the enemy pities you, or are they just laughing?",
        "Did you learn that stance from a novice trainer?",
        "For someone claiming to be a hero, you seem quite... ordinary.",
        "I’ve seen apprentices with more fire in their spells.",
        "The Horde would reject you, and the Alliance wouldn’t claim you.",
        "You’d best stand aside before you embarrass yourself further.",
        "Your strikes are so soft, I thought you were giving a massage.",
        "I’ve fought broomsticks with more balance than you.",
        "Does the Shadow approve of this display? I doubt it.",
        "If swaggering into battle was an art form, you’d still be an amateur.",
        "Your aura is less intimidating and more... pitiful.",
        "Was that a dodge, or did you just trip?",
        "Ah, the famed %my_race resilience! I’d laugh, but it’s tragic.",
        "Even ogres would roll their eyes at your tactics.",
        "The spirits whisper of your incompetence.",
        "Did you forge that weapon yourself? It shows.",
        "By Elune’s grace, it’s painful to watch you fight.",
        "Your name will be remembered... as a punchline.",
        "Did you trip over your own ego just now?",
        "Your battle stance is less 'ferocious' and more 'frazzled.'",
        "By the Sunwell, your movements are as clumsy as a child’s.",
        "Perhaps the Scourge left you behind for being... underwhelming.",
        "You call that a war cry? It’s more of a kitten’s mewl.",
        "Even a tree in Ashenvale has more fight than you.",
        "Do Azeroth a favor and leave the heroics to the capable.",
        "You’re about as fearsome as a picnic in Pandaria.",
        "Does your guild even claim you, or do they pretend you’re not there?",
        "Your fighting spirit? More like a mild breeze.",
        "Oh, you’re trying. How... quaint.",
        "Is your aim to make us laugh or the enemy cry?",
        "Ah... You’re a true legend... in your own mind.",
        "Was that a battle cry, or are you just hungry?",
        "Did you train under Thrall’s wolves? They’d disown you.",
        "The Scarlet Crusade wouldn’t even take you as a conscript.",
        "Your performance is almost as bad as Gallywix’s fashion sense.",
        "Your form would make even a Fel Reaver shake its head.",
        "Perhaps you’re better suited to collecting herbs than wielding steel.",
        "If you’re the champion of your people, I weep for them."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomApologyResponse()
{
    std::vector<std::string> responses = { 
        "I suppose I owe you an apology... but don’t expect it to come with a bow.",
        "Fine, fine, I’m sorry. Can we move on before the Fel takes us all?",
        "By Elune, I didn’t mean to offend. Forgive me.",
        "The spirits tell me I’ve erred. I offer my apologies.",
        "I wasn’t wrong, but I can see why you might think so. Sorry, I guess?",
        "Alright, I’ll admit it—I messed up. Happy now?",
        "Forgive me. My tongue was sharper than my wit.",
        "My bad. Even the wisest stumble sometimes.",
        "Fine, I’ll say it: I’m sorry. Are we done here?",
        "In hindsight, I may have been... slightly out of line. My apologies.",
        "I didn’t mean to step on your pride. Accept my apology.",
        "Oops, did I offend? My apologies. Truly.",
        "Look, I was out of line. My mistake. Let’s move on.",
        "I wouldn’t say it often, but... I’m sorry.",
        "It seems I owe you an apology. Don’t get used to it.",
        "Forgive my words. I spoke out of turn.",
        "Alright, I’ll be the bigger person here: I’m sorry.",
        "I might’ve misjudged you. My apologies.",
        "I owe you an apology. That doesn’t mean I enjoyed giving it.",
        "I’ll admit when I’m wrong. This time, I am. Sorry.",
        "You demand an apology? Fine. Here’s one.",
        "I shouldn’t have said that. Apologies.",
        "I overstepped. Accept my regrets and my apologies.",
        "It was foolish of me to assume. Forgive me.",
        "An apology from me? Well, miracles do happen. Sorry.",
        "I’ll make it right. I promise.",
        "Let it be known: I apologize for my actions today.",
        "I demand an apology for that insult.",
        "You’ve wronged me. I’ll await your apology.",
        "A simple apology would suffice, for your obvious misstep.",
        "I expect an apology for that slight.",
        "By the Light, do you not see you owe me an apology?",
        "I’ll accept your apology when you’re ready. No rush.",
        "Do the ancestors not whisper to you? You owe me an apology.",
        "I demand satisfaction, and an apology to match.",
        "Your actions require restitution. An apology will suffice.",
        "Do you have nothing to say? An apology would be a start.",
        "I forgive easily, but I still expect an apology.",
        "It takes courage to apologize. Show me yours.",
        "By the Earthmother, your apology is overdue.",
        "Elune’s grace, will only carry you so far. Apologize.",
        "A real warrior owns their mistakes. Apologize.",
        "That was low. Do the right thing and apologize.",
        "I don’t need your apology. I just want you to feel bad.",
        "Save your excuses. A real apology will do.",
        "If you’re waiting for me to apologize, don’t hold your breath.",
        "I don’t hear an apology. Try again.",
        "Your silence speaks volumes. I assume an apology is coming.",
        "The honor of an apology is rare. Don’t waste it.",
        "I admit it—I was wrong. Satisfied?",
        "Forgive me, my actions were hasty.",
        "Alright, I apologize. Let’s not make this a thing.",
        "I didn’t mean for things to go that far. I’m sorry.",
        "Apologies. That wasn’t my intention.",
        "Look, I’m not perfect, and this proves it. My bad.",
        "I acted poorly. I’ll own up to that.",
        "I’ll make it right. You have my word.",
        "I don’t say this often, but I’m sorry.",
        "An apology from me doesn’t come easy, but you deserve it.",
        "It wasn’t my finest moment. I regret it.",
        "Let me make amends for my mistake.",
        "You didn’t deserve that. I’m sorry.",
        "I’ll try to do better next time. That’s a promise.",
        "Consider this my formal apology.",
        "I spoke out of turn. Forgive me.",
        "I got carried away, and I regret it.",
        "It wasn’t fair of me, and I see that now.",
        "My actions weren’t justified. I’m sorry.",
        "No excuses—just an apology from me.",
        "You deserve an apology, and here it is.",
        "I regret my words. They were harsh.",
        "Let’s call this what it was: a mistake on my part.",
        "I was out of line, and I know it.",
        "I’m genuinely sorry for what I did.",
        "I’ll take the blame for this one.",
        "If I could go back, I’d change what I said.",
        "I owe you an apology, plain and simple.",
        "That was on me, and I’m sorry.",
        "I didn’t handle that well, and I regret it.",
        "I’ll take responsibility for what happened.",
        "That wasn’t fair of me, and I apologize.",
        "I acted out of frustration, not reason. My apologies.",
        "I shouldn’t have let it get to that point.",
        "Mistakes happen, and I made one here.",
        "I realize now that I was wrong.",
        "This apology doesn’t fix everything, but I mean it.",
        "I messed up, and I’ll admit it.",
        "That was a bad call on my part. Sorry.",
        "I was wrong, and I’m owning up to it.",
        "It’s not easy to say, but I’m sorry.",
        "What I did wasn’t right, and I apologize.",
        "Let’s be honest—I owe you an apology.",
        "That wasn’t my intention, but it’s still my fault.",
        "I can’t undo it, but I can say I’m sorry.",
        "I see now where I went wrong.",
        "That’s on me. I’ll take the blame.",
        "It’s never easy to admit fault, but I’m doing it.",
        "I regret how I acted, and I’ll do better.",
        "That was unfair of me, and I apologize."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomHappyResponse()
{
    std::vector<std::string> responses = { 
        "That’s the best news I’ve heard all day!",
        "You’re absolutely glowing with joy—what happened?",
        "Oh, this just made my day even better!",
        "I’m thrilled for you!",
        "Happiness looks good on you.",
        "Could this day get any better? I think not!",
        "That’s what I love to hear!",
        "Your smile is contagious.",
        "This calls for a celebration!",
        "Let’s savor this moment—it’s perfect.",
        "Happiness suits you like armor.",
        "This is the kind of energy I needed today!",
        "You’ve got that sparkle in your eye. What’s the story?",
        "I’m so glad things worked out for you.",
        "This is why we keep going—to feel like this!",
        "I can’t stop grinning. This is great!",
        "This is a moment to treasure.",
        "You’ve brightened up my day!",
        "This reminds me of why life’s worth fighting for.",
        "Let’s make this happiness last!",
        "I haven’t felt this good in ages!",
        "Your joy is infectious—thank you for sharing it.",
        "This is what hope feels like, isn’t it?",
        "Here’s to more days like today!",
        "I didn’t think today could get better, but here we are!",
        "Your excitement is lifting everyone up.",
        "This feels like a blessing, doesn’t it?",
        "I can’t stop smiling now, thanks to you.",
        "Moments like these are what we live for.",
        "Your happiness is the highlight of my day.",
        "I’m over the moon for you!",
        "This makes me so happy I could dance!",
        "Whatever this is, let’s make it last forever.",
        "It’s rare to see someone so genuinely happy.",
        "You’re like a ray of sunshine today.",
        "This joy is exactly what we needed.",
        "A happy day like this calls for a toast!",
        "This energy is unmatched—let’s keep it going!",
        "You’re beaming like the sun on a clear day.",
        "This good mood is exactly what I needed.",
        "Hearing this makes everything feel lighter.",
        "You’ve got a way of lifting everyone’s spirits.",
        "I didn’t know happiness could be so powerful!",
        "Your happiness is the best thing I’ve seen all day.",
        "This moment feels like pure magic.",
        "I’m so grateful to share this joy with you.",
        "Let’s hold on to this happiness, no matter what.",
        "You’ve just reminded me how beautiful life can be.",
        "This is the kind of thing that makes it all worth it.",
        "Pure, unfiltered joy—I love it!",
        "Hearing this feels like winning the lottery!",
        "This feels better than finding an epic mount in a loot chest!",
        "You’re as radiant as Elune herself!",
        "This news is brighter than the Sunwell!",
        "By the Light, this is a moment to cherish!",
        "This calls for a feast in Ironforge!",
        "You’ve got the kind of energy that could wake a Naaru.",
        "Feels like Azeroth itself is smiling today.",
        "Even a murloc would cheer for this news!",
        "If happiness were loot, this would be legendary!",
        "This joy could rival a dragon’s treasure hoard.",
        "I haven’t felt this good since we downed Ragnaros.",
        "This is the kind of day you write ballads about.",
        "Let’s raise a toast in the inns of Stormwind to this joy!",
        "You’re glowing brighter than a mana crystal.",
        "I’d face ten raids just to hear news like this again!",
        "Feels like the Titans themselves are smiling upon us.",
        "This happiness is stronger than Thrall’s hammer.",
        "Even Sylvanas couldn’t ruin this mood!",
        "We should dance like trolls at a Zuldazar festival!",
        "Hearing this makes me feel like I’m flying over Nagrand.",
        "This moment deserves its own cinematic.",
        "Who needs buffs when we’ve got this energy?",
        "Feels like we just got a critical hit of joy.",
        "I’d storm the Black Temple again to celebrate this!",
        "Even Deathwing couldn’t overshadow this happiness.",
        "This news is better than finding a rare spawn!",
        "I’m happier than a goblin with a bag full of gold.",
        "This calls for fireworks in Orgrimmar!",
        "Your joy is as contagious as a plague in Stratholme—uh, in a good way!",
        "By the ancestors, this is a day to remember!",
        "This feels like a crit heal for the soul.",
        "Your happiness is as bright as the Northern Lights in Northrend.",
        "I’d trek through the Barrens just to hear news like this.",
        "This is what keeps adventurers going, isn’t it?",
        "Even the grumpiest dwarf would smile at this.",
        "Feels like we just completed a world quest with a massive reward.",
        "I haven’t felt this good since my first trip to Dalaran.",
        "This kind of happiness should be bottled and sold in Booty Bay!",
        "This is more refreshing than a mug of Thunderbrew ale.",
        "You’re as uplifting as a shaman’s totems.",
        "Let’s celebrate like the Horde after a battleground victory!",
        "This moment is as rare as a pristine black diamond.",
        "This joy is a blessing from Elune herself.",
        "Happiness this pure could heal even the Scourge!",
        "You’re a beam of light in the darkness, truly.",
        "We’ve had victories before, but this one feels special.",
        "This is a memory we’ll treasure forever.",
        "Even Arthas would smile if he heard this news.",
        "Today, we’re as unstoppable as Illidan at his peak.",
        "This feels like looting a perfect piece of gear on the first try!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomPleadResponse()
{
    std::vector<std::string> responses = { 
        "You’re begging like a kobold guarding its candle.",
        "Fine, fine! Stop whining like a murloc, I’ll help.",
        "Your pleas are as persistent as Deathwing’s tantrums.",
        "I’ve heard better begging from gnolls in Elwynn Forest.",
        "By the Light, fine. Just stop looking at me like that.",
        "Do you think pleading works on a seasoned adventurer? Think again.",
        "Alright, alright, you win. Just don’t cry about it.",
        "You sound like a goblin trying to sell junk. Enough already!",
        "If I say yes, will you stop sounding like a dying banshee?",
        "This better be worth my time, or I’ll reconsider.",
        "You’re as desperate as a mage out of mana.",
        "Elune save me, I can’t ignore that face. Fine!",
        "You’re tugging at my heart like a child wanting candy in Goldshire.",
        "Do you think this works on dragons? No? Then why try it on me?",
        "I’ve resisted the whispers of Old Gods. Your plea is nothing.",
        "Alright, alright. You’re worse than an orc begging for ale.",
        "Do I look like a quest giver to you?",
        "Even the Titans themselves wouldn’t fall for this act.",
        "You sound like you just wiped on a raid boss. Calm down.",
        "Fine, but this is the last time, I swear.",
        "Your desperation rivals Arthas looking for Frostmourne.",
        "If I help, will you stop making that pitiful noise?",
        "You’re more dramatic than a gnome losing their gadgets.",
        "I’ll do it, but you owe me. Big time.",
        "Alright, but only because I can’t take any more of this.",
        "Even the Greatfather Winter doesn’t get this much pleading.",
        "You’re as relentless as an endless dungeon grind.",
        "The way you’re begging, you’d think this was life or death.",
        "I’ve seen ogres with more dignity than this.",
        "Alright! Just stop sounding like a Wailing Caverns reject.",
        "By Elune’s light, I’ll help! Just stop whining.",
        "You’re about as convincing as a critter trying to raid.",
        "You should take acting lessons from a Forsaken.",
        "Your pleading skills could use a bit more charisma.",
        "Fine, I’ll help. But this better be worth a legendary reward.",
        "This is why I don’t party with randoms.",
        "Alright, you’ve twisted my arm. Let’s get this over with.",
        "You beg better than most NPCs, I’ll give you that.",
        "Do you even hear yourself? You sound like a broke goblin.",
        "Stop pleading and start doing. Actions speak louder than words.",
        "You’re lucky I’m feeling generous today.",
        "Fine. But don’t think this means I like you.",
        "Even Thrall doesn’t deal with this kind of whining.",
        "You’d make a decent bard with that level of drama.",
        "Okay, but only because I can’t stand the noise.",
        "I’ve seen better begging from peons. Lok’tar, fine!",
        "Alright, but you owe me more gold than Gallywix could dream of.",
        "You’re testing my patience, but I’ll do it.",
        "Your persistence is impressive. Fine, you win.",
        "I’ll help, but next time, just ask like a normal person.",
        "Stop crying like a lost kobold, I’ll help you out.",
        "You’re as persistent as a ghoul clinging to life.",
        "Alright, enough of the theatrics. Let’s move.",
        "Fine, I’ll help. But only because I can’t listen to this anymore.",
        "Your pleas echo louder than an alarm-o-bot.",
        "For the sake of silence, I’ll do it.",
        "Do you want my help or my pity? Because you’re earning both.",
        "You sound like a banshee who stubbed her toe. Fine, I’ll help!",
        "Your desperation rivals that of an Alliance spy in Orgrimmar.",
        "Even Murlocs have more dignity than this. Alright, fine.",
        "By the Sunwell, you’re worse than a vendor hawking wares.",
        "You’re begging like I’m holding Frostmourne over you.",
        "Alright, but you’re carrying the loot back.",
        "If I say yes, will you finally quiet down?",
        "Fine, let’s get this over with. Just stop pleading.",
        "Elune’s grace, you’re relentless. Alright.",
        "You’re acting like this is your last life in a dungeon run.",
        "Your persistence is impressive. Annoying, but impressive.",
        "By the Earthmother, stop that racket. Fine!",
        "You’re as clingy as a voidwalker in a crowded room.",
        "Alright, but you better not blame me if this goes south.",
        "Even the Scarlet Crusade would roll their eyes at this performance.",
        "I’ll help, but this is starting to feel like a low-tier questline.",
        "Fine, fine. Just don’t expect me to enjoy it.",
        "Your whining could raise the dead. Fine, I’m in.",
        "This is worse than trying to fish without bait. Fine!",
        "If only you put this much energy into fighting.",
        "You’re lucky I’m not in the mood to argue.",
        "Alright, but next time, don’t beg like a defias thug.",
        "Fine, but if this backfires, it’s on you.",
        "I can’t listen to this anymore. Let’s go.",
        "Do I look like a priest? Alright, fine, I’ll help.",
        "Your persistence could rival a raid boss. Fine!",
        "Alright, stop making that face. It’s unsettling.",
        "I’ve seen trolls with better persuasion skills.",
        "Fine, but you owe me a drink in Booty Bay after this.",
        "This better be quick, or you’ll regret it.",
        "I’ll help, but only because I feel sorry for you.",
        "Alright, let’s see if this is worth the drama.",
        "Fine, but I’m not doing all the work.",
        "Even Thrall wouldn’t endure this level of pleading.",
        "Alright, but no promises on how well this will go.",
        "If you keep this up, I’ll start charging for my time.",
        "Your begging could shatter crystals in Exodar. Fine!",
        "Alright, but only because I’m bored.",
        "Fine, I’ll help. But if you mess this up, we’re done.",
        "You sound like you lost all your gold to a goblin scammer.",
        "Alright, stop groveling. It’s embarrassing.",
        "Okay, okay. Let’s get this over with.",
        "You’re relentless, I’ll give you that. Fine."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomScratchResponse()
{
    std::vector<std::string> responses = { 
        "Hey! That actually hurt!",
        "Did you just scratch me? Rude!",
        "Ow, watch those claws!",
        "I’m not a scratching post, you know.",
        "What, are you part feral druid now?",
        "If I wanted claw marks, I’d ask a bear.",
        "Careful! This isn’t some arena match.",
        "By Elune’s grace, keep your nails to yourself.",
        "That scratch was uncalled for!",
        "You’re worse than a rabid worgen!",
        "If you’re going to scratch, at least warn me.",
        "Do you sharpen those nails or something?",
        "This is why gloves were invented.",
        "You’ve got a mean swipe there.",
        "That’s it, next time I’m wearing plate armor.",
        "Is this some strange form of greeting?",
        "Watch it, or I’ll scratch back.",
        "You’ve got a future as a feral druid.",
        "That’s enough scratches for one day.",
        "Do I look like a scratching dummy to you?",
        "You’re lucky I didn’t swipe back.",
        "Ow, do you think I’m immune to pain?",
        "Do you have a permit for those claws?",
        "Even a murloc would be more gentle.",
        "If you’re trying to annoy me, it’s working.",
        "Great, now I’m going to need healing.",
        "Watch those claws, or I’ll report you to the guards!",
        "What’s with all the scratching?",
        "You scratched me like a frenzied cat.",
        "By the Light, control yourself!",
        "That’s it, I’m getting a tetanus shot.",
        "Keep scratching, and we’ll have words.",
        "I didn’t sign up for claw combat!",
        "Do I look like a scratching post?",
        "Are you part feline or something?",
        "Enough with the scratching already!",
        "Do you always greet people with a swipe?",
        "If you scratch me again, you’ll regret it.",
        "That scratch was completely unnecessary.",
        "You’re acting like a rabid ghoul!",
        "By the Earthmother, those claws are sharp.",
        "Next time, try a handshake instead of claws.",
        "That swipe almost broke my armor!",
        "You’ve got a nasty habit of scratching.",
        "I’m not a chew toy or a claw target.",
        "Is this some kind of new PvP tactic?",
        "By the Sunwell, control those claws!",
        "Alright, no more scratches or I’m out.",
        "You scratch me one more time, and we’re done.",
        "I hope that scratch made you feel better.",
        "This isn’t a duel, so stop clawing at me.",
        "Alright, who taught you that scratching is okay?",
        "You’ve got sharper nails than my weapons.",
        "Do you know how much this gear costs? Stop scratching!",
        "I should start calling you Claw McGee.",
        "Is this a joke, or are you really scratching me?",
        "That scratch better come with an apology.",
        "You scratch like a kobold defending its candle.",
        "My armor’s supposed to stop arrows, not claws!",
        "By the Ancestors, what’s wrong with you?",
        "If I wanted a fight, I’d have asked nicely.",
        "That’s not even a proper attack, just annoying.",
        "If you scratch me again, I’ll tell everyone you hugged a murloc.",
        "Seriously, do I have ‘scratch me’ written on my face?",
        "That scratch was weaker than a gnome’s punch.",
        "I’ll remember this next time you need backup.",
        "What’s next? Are you going to bite me?",
        "If you’re trying to intimidate me, you’re failing.",
        "That’s it, I’m reporting you to the nearest guard.",
        "This is why we can’t have nice things.",
        "Do you sharpen those claws daily or something?",
        "Enough with the scratches; I’m not a tree.",
        "That wasn’t a scratch, it was a tickle.",
        "Is this revenge for something I said?",
        "Do you want me to scratch you back? Didn’t think so.",
        "By the Light, what is wrong with you?",
        "That scratch couldn’t cut butter.",
        "If this is how you flirt, you’re terrible at it.",
        "Stop scratching or I’m calling in the healers.",
        "Your scratch has all the power of a critter attack.",
        "I didn’t think I’d get clawed today, yet here we are.",
        "Are you part beast? Because that scratch says yes.",
        "Scratch me again and you’ll owe me new armor.",
        "This isn’t a scratching contest, you know.",
        "That was barely a scratch; try harder next time.",
        "If scratching is your combat style, I’m not impressed.",
        "You scratch like a bored mage without mana.",
        "By Elune’s grace, keep your hands to yourself!",
        "I think that scratch offended my ancestors.",
        "Are you trying to buff your nails on my armor?",
        "That scratch was more pathetic than a sick murloc.",
        "If this keeps up, I’m bringing my shield next time.",
        "Do you always greet people with a swipe?",
        "That scratch better be the last one.",
        "You’ve got the grace of a raging quilboar.",
        "Is scratching part of your combat training?",
        "You’re lucky I don’t retaliate in kind.",
        "That scratch says ‘amateur’ all over it.",
        "By the Sunwell, do you even know what you’re doing?",
        "I didn’t know we were playing rough, but okay!",
        "Your scratches are giving me secondhand embarrassment.",
        "That’s it, I’m done with your claws."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomBoredResponse()
{
    std::vector<std::string> responses = { 
        "Bored? Go poke a murloc and see what happens.",
        "If you’re bored, start counting how many kobolds you’ve annoyed today.",
        "Boredom is just a sign you need to pull more mobs.",
        "How can you be bored in Azeroth? Look around!",
        "Maybe go wrestle a yeti if you’re so bored.",
        "Boredom? Sounds like a personal problem.",
        "I’m sorry, are the constant wars not entertaining enough for you?",
        "You could always try fishing… or napping.",
        "Why don’t you try dancing on the auction house steps?",
        "If you’re bored, I can summon something terrifying.",
        "Bored? Sounds like you need to duel someone.",
        "Maybe explore that cave over there. What’s the worst that could happen?",
        "If you’re bored, you can start repairing my gear.",
        "Go find a dragon to annoy. That’ll spice things up.",
        "There’s no room for boredom when you’re a hero. Get moving.",
        "Bored? Try jumping off a high place and aiming for the water.",
        "Why not start a tavern brawl if you’re so bored?",
        "Bored people usually get volunteered for dangerous quests.",
        "If you’re bored, go gather herbs. It’s thrilling, I promise.",
        "Try using /silly until someone laughs. Hours of fun.",
        "Boredom? That’s what happens when you don’t pull your weight.",
        "You could always make a goblin laugh… or explode.",
        "If you’re bored, it’s time to pick a fight with a rare mob.",
        "Why not ask that angry troll over there for a hug?",
        "If boredom were a crime, you’d be serving a life sentence.",
        "I have just the thing for boredom: more grinding!",
        "Go challenge a gnome to a dance-off if you’re bored.",
        "Boredom is for the Alliance. Let’s do something chaotic.",
        "Did you try talking to the critters? They’re great listeners.",
        "Maybe the Lich King can cure your boredom.",
        "If you’re bored, help me carry this loot.",
        "You could always organize a raid for fun.",
        "Go find a mount you don’t have yet. That’ll kill some time.",
        "Bored? Sounds like a great time to practice your taunting skills.",
        "What if you just sat down and listened to the NPCs? Riveting stuff.",
        "Boredom is weakness leaving the mind. Start running laps.",
        "A bored adventurer is just asking for trouble.",
        "Go hug a bear. That should be exciting enough.",
        "You could start counting how many times you’ve died today.",
        "There’s no boredom in the afterlife, you know.",
        "Bored? Start crafting something ridiculous.",
        "The Barrens is full of boredom cures. Just… try not to get lost.",
        "Find the nearest rogue and ask them to entertain you.",
        "Bored adventurers usually end up as snack food.",
        "If you’re bored, maybe you need more danger in your life.",
        "Boredom is what happens when you ignore your quests.",
        "You could always try breaking into Blackrock Mountain. That’ll wake you up.",
        "If you’re bored, duel me. I promise you won’t be for long.",
        "Boredom doesn’t exist when you’re pulling aggro.",
        "Go swim with the naga. That should be… invigorating.",
        "Bored? Maybe you’re not hitting things hard enough.",
        "There’s a murloc out there just waiting for you to bother it.",
        "If you’re bored, you can try surviving without a healer for a bit.",
        "Why don’t you go talk to that ominous portal over there?",
        "Boredom is what happens when you’re not in a dungeon.",
        "Have you tried collecting critter pets? That’s a lifelong project.",
        "Only the lazy get bored. Chop chop, adventurer.",
        "Bored? Sounds like someone needs a fetch quest.",
        "If you’re bored, you’re not grinding rep hard enough.",
        "The solution to boredom is simple: more dragons.",
        "Go find a cursed item. That should keep you busy for a while.",
        "Bored adventurers are prime candidates for guild chores.",
        "Why not challenge someone to a fishing duel?",
        "There’s a big bad evil guy somewhere. Go find him.",
        "If you’re bored, the Argent Crusade always needs more help.",
        "Boredom? That’s the excuse of someone who doesn’t have enough alts.",
        "There’s always PvP if you’re feeling bold.",
        "Why not go mess with the Darkmoon Faire? That’ll cure boredom.",
        "Bored adventurers end up as punchlines in tavern stories.",
        "You could always start arguing about the Light vs. Void.",
        "If you’re bored, try finding every mailbox in Azeroth.",
        "A bored hero is a rusty sword waiting to break.",
        "You know what they say: bored people make the best bait.",
        "Why not try counting all the skulls in Undercity?",
        "If you’re bored, go see how far you can swim before you drown.",
        "Nothing like a little raid wipe to shake off the boredom.",
        "Go aggro a patrol and see what happens. Boredom solved.",
        "Bored? Sounds like you need more side quests.",
        "Why don’t you visit the Exodar? It’s… something to do.",
        "If you’re bored, try soloing a dungeon you’re undergeared for.",
        "There’s always someone in need of a good duel.",
        "You could always throw snowballs at the locals in Winterspring.",
        "Find a griffon and see where it takes you.",
        "Bored? Maybe you’re just standing in the wrong zone.",
        "Why not start crafting random items? Keeps the hands busy.",
        "I hear the Scarlets are always up for a fight. Go find them.",
        "If you’re bored, start counting how many critters you can squish.",
        "Boredom is the gateway to chaos. Embrace it.",
        "Go climb the highest peak in Azeroth. It’s therapeutic.",
        "If you’re bored, find someone to roleplay with.",
        "Why not start a fight in the neutral auction house?",
        "You could always explore every single cave in Kalimdor.",
        "Bored adventurers are the first to die in a raid.",
        "Try kiting a boss for fun. Just don’t tell the raid leader.",
        "Boredom is a challenge. Overcome it with mayhem.",
        "Go pick a fight with a guard in Stormwind. That’ll wake you up.",
        "Why not start a debate about which expansion was the best?",
        "Find the ugliest transmog and parade it around town.",
        "Boredom? Sounds like an invitation to pull everything in sight.",
        "Go find a hidden achievement to chase. Boredom solved."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomSitResponse()
{
    std::vector<std::string> responses = { 
        "Finally, a chance to rest these weary legs.",
        "Sitting down has never felt this good.",
        "Ah, the ground here is surprisingly comfy.",
        "I could sit here all day… but I won’t.",
        "Sometimes you just need to take a load off.",
        "This spot’s perfect for people-watching.",
        "Why fight when you can sit?",
        "Sitting here reminds me of simpler times.",
        "My feet were starting to scream for a break.",
        "I’m not lazy; I’m conserving energy!",
        "Nothing like a good sit to gather your thoughts.",
        "Don’t mind me; just enjoying the view from down here.",
        "I think I’ve earned this break, don’t you?",
        "Even heroes need to rest sometimes.",
        "This bench? Claimed. Mine now.",
        "I’d get up, but why ruin a good thing?",
        "I’m just waiting for the world to catch up with me.",
        "I call this strategic relaxation.",
        "Sitting down is part of my master plan.",
        "There’s no shame in a tactical sit-down.",
        "I might be sitting, but my mind is racing.",
        "This ground is a little cold, but it’ll do.",
        "What’s wrong with sitting? The enemies can wait.",
        "I swear I’m not hiding; I’m just sitting with purpose.",
        "I could conquer Azeroth… after this break.",
        "Ah, finally, a moment to breathe.",
        "Sometimes the best view is from down here.",
        "You stand, I’ll sit, and we’ll call it teamwork.",
        "I wonder if anyone’s noticed how long I’ve been sitting here.",
        "This is my sit-and-think-about-loot pose.",
        "I’ve decided this is my favorite sitting spot in Azeroth.",
        "Sitting down doesn’t make me weak; it makes me smart.",
        "If you’re looking for me, I’ll be right here. Sitting.",
        "I’m testing gravity; it still works.",
        "You know what they say, sitting is the new standing.",
        "I’m not just sitting, I’m strategizing.",
        "From here, the world seems manageable.",
        "Nothing like a good sit after a long raid.",
        "This spot? Claimed in the name of relaxation.",
        "Don’t rush me; sitting is an art form.",
        "You should try sitting too. It’s quite nice.",
        "If you need me, I’ll be contemplating life from here.",
        "This might be the best decision I’ve made all day.",
        "When in doubt, take a seat and figure it out.",
        "I’m not stalling, I’m appreciating the moment.",
        "Every epic adventure deserves an epic sit.",
        "I’ll fight again… right after this break.",
        "Life’s too short to not sit when you can.",
        "Sitting down isn’t giving up; it’s preparing for greatness.",
        "Ah, this seat was made for me.",
        "I could get used to this view.",
        "Just me, my thoughts, and a nice place to sit.",
        "The floor is underrated. Try it sometime.",
        "Finally, a moment to relax.",
        "This ground has seen better days, but it’ll do.",
        "I’ve slain dragons; I’ve earned this break.",
        "My legs are thanking me for this.",
        "Sitting is highly underrated, don’t you think?",
        "From down here, the world looks a little less chaotic.",
        "I’m not sitting, I’m scouting… from a lower vantage point.",
        "This feels like the perfect spot for a picnic.",
        "If sitting were an Olympic sport, I’d take gold.",
        "Sometimes the best adventures start with a good sit.",
        "I’m one with the earth now.",
        "This seat is warmer than I expected.",
        "They say heroes never rest… they’re wrong.",
        "Sitting down is self-care, and I’m a pro at it.",
        "Let the enemies wait. This is my time.",
        "My feet deserve a break after carrying me through all those battles.",
        "I think I’ll stay here until the sun moves.",
        "This is my throne now.",
        "I’d stand, but why ruin perfection?",
        "The best plans are made while sitting.",
        "From here, everything feels less urgent.",
        "If you’re looking for me, I’m officially on a sitting spree.",
        "This is prime sitting real estate.",
        "Did someone say ‘chair’? Because I didn’t see one.",
        "I’ve claimed this spot in the name of relaxation.",
        "Resting here feels like the best decision of the day.",
        "You fight; I’ll supervise from here.",
        "Sitting down is part of my warm-up routine.",
        "Nothing beats a good sit after a long grind.",
        "Why stand when sitting feels this good?",
        "This patch of ground is surprisingly comforting.",
        "I’m practicing the ancient art of strategic sitting.",
        "All great warriors need a moment to sit and think.",
        "You keep talking; I’ll keep sitting.",
        "I’m gathering my strength for the next epic moment.",
        "This spot feels… significant somehow.",
        "Even heroes need to appreciate the little things, like sitting.",
        "The seat of power? That’s me, right here.",
        "I could sit here all day, and maybe I will.",
        "Sitting isn’t lazy—it’s energy conservation.",
        "I’m not just sitting; I’m in my thinking position.",
        "The ground here feels like home.",
        "This is my recharge zone. Do not disturb.",
        "If anyone needs me, I’m on a sit break.",
        "Sitting down is the calm before the storm.",
        "Sometimes the best strategy is sitting still."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomNodResponse()
{
    std::vector<std::string> responses = { 
        "Finally, a chance to rest these weary legs.",
        "Absolutely, I couldn’t agree more.",
        "A nod for you, my wise friend.",
        "Couldn’t have said it better myself.",
        "Yep, you’re spot on.",
        "Agreed! No arguments here.",
        "Nods all around for that one.",
        "I see your point, and I raise you a nod.",
        "Finally, someone gets it. I agree.",
        "Consider this my official nod of approval.",
        "Couldn’t agree more if I tried.",
        "Yep, makes perfect sense to me.",
        "I’m with you on this one.",
        "Nodding so hard my neck hurts.",
        "You’ve got my full agreement.",
        "That’s exactly what I was thinking!",
        "You speak truth, and I’m here for it.",
        "Yes, yes, a thousand times yes.",
        "I can’t find a reason to disagree.",
        "Agreement achieved. Proceed.",
        "You’ve convinced me. I’m in.",
        "Solid point, I’m all in.",
        "Nods are free, and you just earned one.",
        "Totally aligned with that idea.",
        "If I nod any harder, my head might fall off.",
        "We’re on the same wavelength here.",
        "I’ll just nod quietly in agreement.",
        "Consider me a member of Team You.",
        "Absolutely. That’s the way to go.",
        "You’ve got my vote of confidence.",
        "I’ll back you up on that one.",
        "Couldn’t disagree if I wanted to.",
        "Agreed! And I’ll fight anyone who says otherwise.",
        "I’m nodding so fast I’m a blur.",
        "With you 100%. No doubt about it.",
        "That’s a solid yes from me.",
        "No argument here. You’re spot on.",
        "I’ll second that motion.",
        "We’re in sync on this one.",
        "I’m on board, no questions asked.",
        "My nod says it all—yes.",
        "I’m all ears, and they’re nodding too.",
        "Every word you said? Agreed.",
        "You’ve got my full support on this.",
        "That idea deserves a standing ovation and a nod.",
        "I’m in total agreement with you.",
        "You just read my mind.",
        "Your wisdom knows no bounds; I’m on board.",
        "Exactly! I was just thinking the same.",
        "I’m nodding like it’s a workout.",
        "You’re right, and I’m here for it.",
        "By the Light, I completely agree!",
        "You speak like a true champion of Azeroth.",
        "Nodding so hard, even a dwarf would approve.",
        "Elune’s wisdom shines through your words.",
        "I’d stake my armor on that being true.",
        "You’ve got my allegiance, no question.",
        "As sure as Thrall’s honor, I’m with you.",
        "You’ve got my nod and my blade.",
        "That’s a bigger yes than an orc’s war cry.",
        "Agreed, like the Horde at a feast table.",
        "I’ll rally behind that idea like it’s a battle cry.",
        "You’re as right as rain in Stranglethorn Vale.",
        "Nodding like a goblin counting gold!",
        "Absolutely, even Sylvanas would approve.",
        "If the Kirin Tor heard this, they’d agree too.",
        "I’m nodding faster than a mage blinks!",
        "You’re right, even Sargeras might reconsider his plans.",
        "By the elements, I’m with you!",
        "I’ll take that as gospel in the Cathedral of Light.",
        "For the Alliance—or the Horde—I’m nodding.",
        "You’ve got my respect, and that’s rare as an epic drop.",
        "If that’s not wisdom, then I’m a murloc.",
        "You’re making sense like a gnome explaining tech.",
        "Agreed, like a tauren cherishing the Earth Mother.",
        "Even an ogre would nod at that logic.",
        "On that, we see eye-to-eye, like Illidan.",
        "Your idea’s as solid as Orgrimmar’s gates.",
        "I’d follow that thought to the Dark Portal and back.",
        "You’ve got my approval, like Jaina in a strategy meeting.",
        "If this were a raid, I’d roll need on that idea.",
        "Aye, agreed, like a dwarf and his ale.",
        "Yes, like a hunter marking their prey.",
        "You’ve spoken the truth, as clear as the Sunwell.",
        "Even the Old Gods would nod at that wisdom.",
        "I’m with you, as steady as Stormwind’s walls.",
        "That’s a bigger yes than a troll’s grin.",
        "This is a nod as loyal as a paladin’s oath.",
        "That thought’s brighter than the Exodar.",
        "By the mana in my veins, I agree!",
        "Your logic is as sharp as Varian’s sword.",
        "You’ve earned a nod worthy of Thrall himself.",
        "That’s as clear as a crystal in Outland.",
        "Your wisdom’s shining brighter than Azeroth’s moons.",
        "You’re spot on, even the Naaru would hum for you.",
        "If wisdom were loot, you’d be epic tier.",
        "Nodding like an orc charging into battle!",
        "You’re as right as the tides in Vashj’ir.",
        "Elune must be whispering in your ear, I agree.",
        "That’s a nod from me, as sure as death in Icecrown.",
        "You’re so right, even a worgen would stop howling."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomCommendResponse()
{
    std::vector<std::string> responses = { 
        "Oh, stop it, you’re making me blush.",
        "Finally, someone notices my greatness!",
        "I humbly accept your praise. More, please!",
        "I didn’t do it for the glory... but I’ll take it!",
        "Well, I suppose I am pretty amazing.",
        "You’re too kind, but I’ll allow it.",
        "Don’t stop now—I’m getting used to this.",
        "Even a broken clock gets praised twice a day, right?",
        "Well, I guess I do have that hero vibe.",
        "Flattery will get you everywhere.",
        "Keep talking, I like where this is going.",
        "Not bad for a humble adventurer, eh?",
        "Finally, someone who appreciates art in motion.",
        "See? This is why I’m the best in the group.",
        "I’ll make sure the bards get this in their next song.",
        "Even the Titans couldn’t do better!",
        "I’d bow, but that might be overkill.",
        "Careful, too much praise might go to my head.",
        "Was there ever any doubt?",
        "Ah, yes, bask in the glow of my awesomeness.",
        "It’s all in a day’s work for someone like me.",
        "Your words are music to my ears!",
        "Finally, someone who recognizes talent!",
        "You’re making me feel like a raid boss over here.",
        "I live for the applause, thank you very much.",
        "Hey, don’t stop now—I’m enjoying this.",
        "Well, I am kind of a big deal.",
        "Thank you, thank you! I’m here all week.",
        "Looks like the MVP title is mine again.",
        "You’ve got good taste recognizing excellence.",
        "The Light has clearly blessed me today.",
        "Now this is what I call validation!",
        "Aw, shucks, you’re too kind.",
        "I’d say it’s teamwork, but we know the truth.",
        "Not all heroes wear capes, but I pull it off well.",
        "I didn’t do it alone... just mostly alone.",
        "I thrive on this level of recognition.",
        "Who’s the hero? Oh right, it’s me!",
        "I told you I’d be the star of this show.",
        "Let’s face it, I make saving Azeroth look good.",
        "Ah, the sweet sound of well-earned praise.",
        "Admit it, I’m your favorite now.",
        "It’s not bragging if it’s true!",
        "Some call it luck, I call it destiny.",
        "I’m just doing my part... spectacularly.",
        "Feels good to be this awesome.",
        "This is why I don’t need a guild. Kidding... maybe.",
        "They should name a battleground after me.",
        "I’ll try not to let this go to my head. No promises.",
        "By the Light, I knew I was destined for greatness!",
        "The ancestors must be smiling on me today.",
        "Ah, praise fit for a champion of Azeroth!",
        "I guess I am the stuff of bard songs now.",
        "Even Thrall couldn’t have done better!",
        "Sylvanas herself would nod in approval. Maybe.",
        "Finally, recognition befitting a hero of my stature.",
        "Guess who’s the real champion of the Argent Crusade?",
        "Do you think the Kirin Tor is watching? They should be.",
        "Feels good to be Azeroth’s finest!",
        "Ah, another feather in my cap. Or helmet, I suppose.",
        "Clearly, Elune has guided my hand today.",
        "The Horde should make me Warchief after that one.",
        "By the Sunwell, I truly am magnificent.",
        "I’ll bet Gallywix would even pay me for this performance.",
        "Looks like the Titans crafted me for this moment.",
        "Did I just outshine a dragon aspect? Probably.",
        "Praise me now, because I’ve got to save the world again later.",
        "Even Bolvar couldn’t keep up with this level of heroism.",
        "That’s right, bask in the glory of my deeds!",
        "Malfurion who? Exactly.",
        "The Barrens chat couldn’t even troll me after this.",
        "They’ll be carving my likeness in Stormwind soon enough.",
        "Not even the Burning Legion could stop me today!",
        "I guess my DPS was so good, it’s practically lore now.",
        "They’ll need to write a new epic questline just for me.",
        "Is this how Arthas felt before... you know, everything?",
        "Even Illidan would call me ‘prepared.’",
        "This is what happens when a true hero steps up.",
        "Guess I’m the chosen one now.",
        "Another victory for the Horde! You’re welcome.",
        "Looks like the Alliance has its new poster child.",
        "I feel like I’ve just soloed a raid boss.",
        "The loot table better include applause for this performance.",
        "Someone call Khadgar—I’ve just outsmarted magic itself.",
        "Well, this calls for a victory pint at the tavern.",
        "Did I just win Azeroth? Feels like it.",
        "Is there a title for this level of amazing? There should be.",
        "Not even Deathwing could bring me down after this.",
        "Even the Naaru would approve of this brilliance.",
        "I’ll be waiting for my statue in Orgrimmar, thanks.",
        "You think Sylvanas would call me impressive? She should.",
        "All this heroism and still no time for fishing.",
        "Do you think I’ll get a personal thank-you from Anduin?",
        "One step closer to exalted with... everyone!",
        "Teldrassil might still be standing if I was there.",
        "Even the Old Gods couldn’t handle my swagger.",
        "This is why I’m the MVP of the Shadowlands.",
        "Who needs legendaries when you’ve got my skills?",
        "Clearly, Azeroth was saved by yours truly."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomFlopResponse()
{
    std::vector<std::string> responses = { 
        "Medic! I need a medic now!",
        "Someone, please patch me up!",
        "By Elune’s grace, help me before it’s too late!",
        "I’m not going to make it without aid!",
        "Healers, where are you? I’m bleeding out here!",
        "I can’t go on much longer… help!",
        "If you’ve got healing potions, now’s the time!",
        "I’m hit! Someone, do something!",
        "I need help! I’m not as invincible as I look!",
        "This isn’t how my story ends… someone save me!",
        "By the Light, I need a healer!",
        "Everything is getting blurry… I need aid!",
        "Don’t just stand there—help me!",
        "My wounds are too deep… anyone?",
        "I’m one hit away from the spirit healer—help!",
        "Quickly! Before the shadows claim me!",
        "Someone stop the bleeding… please!",
        "If you save me now, I’ll owe you a big one!",
        "I’m not ready for the afterlife yet!",
        "Call for a healer, or I’m done for!",
        "The pain… I can’t… someone help me!",
        "Get me out of this mess before it’s too late!",
        "I need aid! Quickly!",
        "By the spirits, I’m not ready to go!",
        "Can anyone hear me? I’m in trouble!",
        "Heal me now, and I promise to buy the first round!",
        "I don’t want to die here. Not like this!",
        "Help me, or I’ll haunt you forever!",
        "Is there no one who can save me?",
        "I need a hero, or at least a healer!",
        "This wound… it’s bad. Really bad.",
        "If I fall, you’re next! Help me!",
        "Patch me up, or I’m out!",
        "I’m fading fast—do something!",
        "I can’t hold on… somebody!",
        "I refuse to die in a place like this!",
        "If this is the end, I’ll blame you all!",
        "I don’t care who it is—just heal me!",
        "I’ve got so much loot… don’t let me die!",
        "Anyone? Please? I’m dying here!",
        "I need help before I become a ghost!",
        "If you save me, I’ll name my first mount after you!",
        "Do I look okay? No! Help me!",
        "Don’t let me go down like this!",
        "Can someone heal me before I embarrass myself?",
        "I’m too important to die here—fix me!",
        "Please, don’t let me die in front of these noobs!",
        "Where’s the healer when you actually need one?",
        "Save me, and I’ll share the loot. Maybe.",
        "I’m a goner if someone doesn’t help!",
        "The Horde wouldn’t leave a warrior like me behind—someone heal me!",
        "By the Light, I need a healer before the Scourge gets me!",
        "Is this how the heroes of Azeroth fall? Help me!",
        "I can’t let the Lich King see me like this—patch me up!",
        "For the love of Elune, someone save me!",
        "I’m too close to finishing my rep grind—don’t let me die now!",
        "Does anyone have a healing spell? I’m out of potions!",
        "By the Sunwell, I refuse to die here! Heal me!",
        "Don’t let me become fodder for Deathwing’s flames—aid me!",
        "The spirits won’t guide me if I bleed out like this!",
        "The Light is dimming… someone pull me back!",
        "I’m not joining the Shadowlands yet—someone heal me!",
        "We’ve fought dragons and titans—don’t let me die to this!",
        "Azeroth’s champions don’t fall this easily—heal me!",
        "Help me now, or I’ll haunt you from beyond the grave!",
        "I didn’t survive Icecrown to die to a gnoll. Heal me!",
        "Save me, or I’ll tell the raid leader you didn’t!",
        "If I die, I’m blaming whoever pulled that mob!",
        "I can’t fight for the Horde if I’m dead—help me!",
        "Even Sylvanas wouldn’t abandon me like this!",
        "Does the healer need mana? Or are they ignoring me?",
        "Don’t let me fall—Bolvar’s already got enough souls!",
        "The Scarlet Crusade had better care for their own!",
        "If you don’t heal me, my pet will eat you!",
        "Do you want Arthas to have another death knight? Heal me!",
        "Don’t let me die, or the Alliance wins!",
        "The Legion would laugh if we lose me here—fix this!",
        "I’m too pretty for the maw—heal me!",
        "Do you want me to embarrass myself in front of the Kirin Tor?",
        "My gear’s too expensive to get damaged—heal me now!",
        "If I die, I’m taking you all with me!",
        "Thrall wouldn’t let me die here—be like Thrall!",
        "Don’t let my story end in Silithus—get me up!",
        "I’ll heal you next time if you save me now!",
        "What’s a priest for if not this? Heal me!",
        "I can’t die; I haven’t spent my valor points yet!",
        "No true champion of Azeroth should bleed out like this!",
        "Save me, and I’ll share my flask with you. Maybe.",
        "If I die, I’m rolling need on everything!",
        "This isn’t my raid story’s end—fix me!",
        "Healers, you’ll miss me when I’m gone!",
        "Do you want my body to be lootable? No? Heal me!",
        "If you save me, I’ll finally help with the dailies!",
        "Don’t let me die with this loot on me—get me up!",
        "The Titans are watching, and they’re judging you!",
        "Do I look like a Forsaken? Heal me before I join them!",
        "The Light is fading—don’t let me go!",
        "By An’she, don’t let me join the ancestors just yet!",
        "Patch me up before I tell everyone who ninja’d loot!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomBlinkResponse()
{
    std::vector<std::string> responses = { 
        "Did you just blink at me? What’s that supposed to mean?",
        "If you blink twice, does that mean you’re in trouble?",
        "Are you trying to tell me something with that blink?",
        "Blink once for yes, twice for no, three times for confusion.",
        "Why are you blinking like that? Is there dust in your eyes?",
        "Is that a signal? Should I blink back?",
        "You blink at me like I owe you gold or something.",
        "Don’t blink at me unless you’re ready to fight.",
        "Are we having a blinking contest now?",
        "Was that blink supposed to intimidate me?",
        "Blinking at me isn’t going to fix this mess!",
        "Do you blink like that at everyone, or am I special?",
        "Was that a flirty blink or just awkward?",
        "Blink once more, and I’ll assume you’re up to something.",
        "Your blink feels suspicious. What are you hiding?",
        "Why do I feel judged by your blink?",
        "Blink at me again, and I might just cast polymorph.",
        "What’s with the blink? Are you bored?",
        "Your blink reminds me of a rogue plotting something.",
        "Are you signaling for backup with that blink?",
        "Your blinking is giving me secondhand anxiety.",
        "Was that blink approval or disappointment? Hard to tell.",
        "If blinking was an art, you’d be a master.",
        "That blink makes me feel like I’ve done something wrong.",
        "Did you just blink at me like I’m a murloc?",
        "Blinking won’t save you if I roll need on that loot.",
        "You blink at me like you’ve never seen a gnome dance.",
        "I see your blink, and I raise you a smirk.",
        "What kind of warlock blinks instead of cackling?",
        "Stop blinking at me; I don’t speak eye signals.",
        "Your blink is as subtle as a tauren in a china shop.",
        "You blink like someone who just ninja’d a mount.",
        "If I blink back, do we become friends or rivals?",
        "Is blinking the new way to taunt in Azeroth?",
        "I didn’t expect a blink from someone like you.",
        "Blinking at me doesn’t make me forget the loot rules.",
        "Are you blinking because of my epic armor?",
        "That blink is about as subtle as a fireball to the face.",
        "You blink like someone who just fell for a hunter trap.",
        "Keep blinking, and I’ll assume you’re a spy.",
        "Are you a mage practicing blink, or is this just awkward?",
        "Blink once more, and I’ll assume you’ve gone mad.",
        "If blinking is a strategy, it’s not working.",
        "Your blinking is making me second-guess everything.",
        "A blink? Really? That’s the best you’ve got?",
        "Do you blink at everyone who out-damages you?",
        "Blinking at me doesn’t change the fact I pulled that mob.",
        "Your blink feels oddly personal. Should I be offended?",
        "If this is a staring contest, you’ve already lost.",
        "Blinking at me won’t get you out of trouble.",
        "Are you signaling, or did you just get dust in your eye?",
        "That blink looks like a call for help.",
        "Don’t blink at me like I owe you something.",
        "If you keep blinking like that, I’ll start to think you’re hiding something.",
        "Your blink is as confusing as a goblin’s blueprints.",
        "Did I miss a meeting? Is blinking the new wave?",
        "Blink once for danger, twice for misunderstanding.",
        "Do you blink at all the heroes, or am I special?",
        "Blinking at me isn’t going to fix that aggro problem.",
        "You blinked. Now you’re stuck with me.",
        "That was a very judgmental blink, wasn’t it?",
        "Your blinking is more intense than a boss enrage timer.",
        "Am I supposed to blink back, or what?",
        "Stop blinking and say what’s on your mind!",
        "Are we flirting or starting a duel?",
        "Your blink has me questioning my life choices.",
        "Blinking like that makes you look guilty.",
        "Was that a nervous blink or just bad timing?",
        "You blink like someone who’s about to drop aggro.",
        "A blink won’t save you from the roll results.",
        "Did you just blink, or was that a lag spike?",
        "Stop blinking and loot the damn chest already!",
        "Your blink feels oddly hostile.",
        "Blinking won’t help your DPS, you know.",
        "If I blink back, are we friends or enemies?",
        "That blink looks like it came from a shady rogue.",
        "Are you blinking in Morse code or just being weird?",
        "Your blinking reminds me of a murloc’s confused stare.",
        "Keep blinking like that, and I’ll assume you’re up to something.",
        "Is blinking your way of saying thanks for the carry?",
        "If you blink any harder, you might summon something.",
        "A blink like that means mischief. I can feel it.",
        "Stop blinking and tank the boss already!",
        "Do you blink every time you think of loot?",
        "That blink was so slow it gave me anxiety.",
        "Are you trying to intimidate me with your blink?",
        "Blinking won’t change the loot rules, buddy.",
        "I see your blink and raise you a sarcastic nod.",
        "Your blink is more dramatic than a gnome’s battle cry.",
        "That blink felt personal. Should I be concerned?",
        "Blinking at me won’t get you the healing cooldowns.",
        "You blink like you just got caught looting ninja-style.",
        "If blinking is your strategy, it’s not working.",
        "I can’t tell if you’re blinking for help or just bored.",
        "Your blink has the subtlety of a tauren in leather boots.",
        "Are you trying to bluff me with that blink?",
        "Blink once more, and I’ll assume you’re plotting something.",
        "The blink was unnecessary, but I’ll allow it.",
        "Your blink is oddly judgmental for someone who just wiped us."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomBarkResponse()
{
    std::vector<std::string> responses = { 
        "Stop barking at me like I’m a postman!",
        "Are you barking orders, or did you just forget how to talk?",
        "That bark better not be aimed at me.",
        "If I wanted to hear barking, I’d visit a kennel.",
        "Keep barking, and I might just fetch you a muzzle.",
        "What are you, a worgen? Relax with the barking.",
        "Barking at me doesn’t make you the alpha.",
        "Do you bark at everyone, or am I special?",
        "I didn’t realize we were doing a dog impression contest.",
        "Your bark is worse than your bite. Guaranteed.",
        "I’ll listen when you stop barking and start speaking sense.",
        "Barking orders doesn’t make you a leader.",
        "Calm down, you’re starting to sound like a growling felhound.",
        "Save your barking for someone who cares.",
        "The louder you bark, the less I’m inclined to listen.",
        "If barking is all you’ve got, you’re out of ideas.",
        "Why bark when you can just ask nicely?",
        "Did you bark at the raid leader too? Bold move.",
        "Your barking makes you sound more desperate than commanding.",
        "Do I look like someone who responds to barking?",
        "Bark all you want. My loot roll speaks louder.",
        "You bark like a hyena who missed their meal.",
        "Are you trying to intimidate me, or is this just how you communicate?",
        "Your bark is as useless as a broken hearthstone.",
        "The barking makes you sound like a poorly-trained hunter pet.",
        "If barking is your strategy, it’s not a good one.",
        "Do you bark this much in battlegrounds too?",
        "Your bark has the charm of a rabid gnoll.",
        "Calm down, before someone casts silence on you.",
        "Keep barking, and you’ll attract an entire pack of wolves.",
        "I didn’t queue up for a barking contest.",
        "Do you bark in Common or Orcish? I couldn’t tell.",
        "The bark is giving big stray dog energy.",
        "Barking doesn’t change the loot rules, my friend.",
        "Is this barking supposed to scare me? Try harder.",
        "If I bark back, does that make us even?",
        "Your barking reminds me of murloc noises, and that’s not a compliment.",
        "Do you bark like this in Stormwind too?",
        "All this barking, and you still missed the mark.",
        "I don’t respond to barking unless treats are involved.",
        "Are you barking because you’re out of mana?",
        "Your bark has less bite than a critter in Elwynn Forest.",
        "Save the barking for when it’s actually useful.",
        "The barking is impressive, but the content is lacking.",
        "I think you’ve mistaken me for someone who cares about barking.",
        "That bark sounded personal. Should I be worried?",
        "Your barking is like background noise at this point.",
        "Bark all you want, but I still outrolled you.",
        "You bark like a kobold protecting its candle.",
        "All bark, no claws. Typical.",
        "Is all that barking supposed to mean something?",
        "You’re barking louder than a worgen on a full moon.",
        "Save the barking for someone who’s impressed by noise.",
        "If barking made you smarter, you’d be a genius by now.",
        "Are you auditioning for a gnoll pack? You’ve got the bark down.",
        "Stop barking, or I’ll leash you myself.",
        "Your bark reminds me of that annoying vendor in Orgrimmar.",
        "This barking act of yours isn’t fetching any respect.",
        "Are you done barking, or is this your new personality?",
        "If you bark any louder, we’ll attract the Alliance.",
        "Your bark has less effect than a rank 1 spell.",
        "Do you need a chew toy, or just a reality check?",
        "Barking doesn’t change the fact that I out-DPS’d you.",
        "Is barking your answer to everything?",
        "You sound like a rabid wolf in Silverpine Forest.",
        "The barking doesn’t make you stronger, just louder.",
        "If barking won wars, you’d already be a general.",
        "The only thing your bark is intimidating is my patience.",
        "Are you barking orders or just making noise for fun?",
        "You bark like you just failed a roll on a legendary item.",
        "Keep barking, and someone’s going to sic the guards on you.",
        "Bark all you want, but I’m still the raid leader.",
        "You’re barking up the wrong tree, my friend.",
        "If barking is an art, you’re painting a mess.",
        "I didn’t know barking was a new form of communication.",
        "Your bark is as annoying as a gnome’s laughter.",
        "Do you bark in your sleep too?",
        "Barking doesn’t make your argument any stronger.",
        "Is this barking supposed to make you look cool?",
        "You’ve got a bark that could rival Deathwing’s roar.",
        "If you bark this much in combat, no wonder you wipe.",
        "Your bark has the charm of a low-level voidwalker.",
        "Bark all you want; I’ve got better things to do.",
        "Are you trying to scare me, or did you step on a thorn?",
        "Your barking is starting to sound like a taunt gone wrong.",
        "The barking is impressive, but the logic is nonexistent.",
        "If barking earned gold, you’d be the richest in Azeroth.",
        "I’ve heard less noise in a full murloc dungeon.",
        "Are you a dog trainer, or do you just like the sound of barking?",
        "That bark belongs in the Barrens chat, not here.",
        "Keep barking; I’ll just mute you.",
        "You bark like you’ve been cursed by a trickster mage.",
        "That barking might scare critters, but not me.",
        "You’re barking like someone who just got ganked in Stranglethorn.",
        "Your bark lacks bite, just like your combat skills.",
        "If barking fixed problems, Azeroth would be a paradise.",
        "Why bark when you could just surrender?",
        "Your bark has the appeal of a broken hearthstone.",
        "If barking gave buffs, you’d be overpowered by now.",
        "The barking doesn’t bother me; it’s the lack of sense that does."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomByeResponse()
{
    std::vector<std::string> responses = { 
        "Goodbye, may the winds guide you.",
        "Farewell, try not to trip over your own feet out there.",
        "Safe travels, don’t forget to write... if you survive.",
        "See you in the next life—or the next raid.",
        "Bye! Don’t let the murlocs bite.",
        "Farewell, hero. Azeroth will surely miss your... unique talents.",
        "Goodbye, and remember to watch your back.",
        "Later, don’t aggro anything you can’t handle.",
        "Take care out there. The wilds can be unforgiving.",
        "Goodbye! Don’t forget to repair your gear before you go.",
        "Safe journey, but leave the loot for me next time.",
        "Farewell, and don’t get lost in the Barrens.",
        "See you later, unless a worgen gets to you first.",
        "Goodbye! Watch out for kobolds—you know how they love their candles.",
        "Don’t let the door hit you on the way out of the dungeon.",
        "Bye now! Remember, the Light is watching... or judging.",
        "Safe travels, and don’t fall into any void portals.",
        "See you next time! Or not. Depends on your luck.",
        "Goodbye, adventurer. May your rolls always be high.",
        "Take care! The mobs out there won’t be as forgiving as I am.",
        "Bye! Don’t forget to turn in your quests this time.",
        "Farewell! Try not to pull too much aggro out there.",
        "Safe travels! And don’t stand in the fire.",
        "Goodbye! May your mana always flow and your cooldowns reset quickly.",
        "Take care! Don’t let the murlocs get the better of you.",
        "See you soon! Unless, of course, you wipe on the next boss.",
        "Goodbye, may Elune light your path.",
        "Farewell! Watch out for those stealthy rogues.",
        "Later, champion. Azeroth might just survive without you.",
        "Bye! Don’t get yourself turned into a sheep again.",
        "Goodbye! Remember, portals can go both ways.",
        "See you around! Or not, depending on your luck in PvP.",
        "Take care, and try not to anger the dungeon master.",
        "Farewell! Don’t let a gnome outsmart you while you’re gone.",
        "Bye! May your hearthstone always lead you home.",
        "Goodbye, adventurer. Your courage—or foolishness—will be missed.",
        "Farewell! Don’t let the goblins sell you anything overpriced.",
        "See you later! Remember, the real treasure is the loot we made along the way.",
        "Goodbye! Azeroth just got a little quieter.",
        "Take care! And don’t forget, the world is watching your DPS.",
        "Bye now! The tavern will keep your seat warm.",
        "Farewell! And remember to dodge those AoEs.",
        "Goodbye! If you see a rare mob, save it for me.",
        "See you soon! Don’t get stuck in a Stormwind auction argument.",
        "Take care! And for the love of Azeroth, repair your gear.",
        "Goodbye! Your absence will be felt... mostly by the enemies.",
        "Later, don’t let the Horde (or Alliance) catch you off guard.",
        "Farewell! And remember, heroes never truly retire... they reroll.",
        "Goodbye! Don’t let the trolls get to you—literally or figuratively.",
        "Farewell! Remember, the loot isn’t worth your life... most of the time.",
        "Bye! Keep your blade sharp and your wits sharper.",
        "See you around! If you hear hoofbeats, it’s probably me.",
        "Safe travels! And avoid the Dark Portal unless you’re ready for trouble.",
        "Goodbye! The next boss fight won’t be the same without your screams.",
        "Farewell! Don’t forget to /wave at passing adventurers.",
        "Later! Just don’t go hugging trees in Ashenvale again.",
        "Goodbye! Azeroth needs you... eventually.",
        "See you soon! May your enemies flee before you.",
        "Take care! And don’t eat anything a goblin chef made.",
        "Bye! Don’t let the gnolls see you as their next meal.",
        "Goodbye! If you find any good gear, send it my way.",
        "Farewell! Watch out for warlocks—they’ve got sticky fingers.",
        "See you later! And may the RNG gods smile upon you.",
        "Bye! Try not to anger any dragons this time.",
        "Goodbye! If you find treasure, remember who gave you luck.",
        "Safe travels! And if you get a rare drop, I call dibs.",
        "Farewell! Watch out for stray arrows in the Barrens.",
        "Goodbye! Don’t let a dungeon boss stomp you into the ground.",
        "See you later! Keep your eyes open for shinies.",
        "Take care! And remember, kodos have right of way.",
        "Goodbye! May your hearthstone always lead to safety.",
        "Farewell! And don’t forget the password to Orgrimmar’s gates.",
        "Bye now! You’re leaving the fun behind, you know.",
        "Safe journey! I’ll hold the line while you’re gone.",
        "Goodbye! If you find a mount, name it after me.",
        "Farewell! And don’t let the murloc gurgles haunt your dreams.",
        "Later! Your DPS might be missed... maybe.",
        "See you soon! Unless you decide to roll a different faction.",
        "Goodbye! Try not to pull too many elites at once.",
        "Farewell! And remember, the Light judges all... even your jokes.",
        "Bye! I’ll tell everyone you fought bravely... or not.",
        "Safe travels! Watch out for critters—they can be vicious.",
        "See you later! Don’t let the void consume you.",
        "Goodbye! Keep your potions stocked and your resolve strong.",
        "Farewell! If you die, I get your stuff.",
        "Bye now! If you see a blood elf, remind them they’re fabulous.",
        "Take care! And don’t stand in anything green—it’s rarely good.",
        "Goodbye! Azeroth won’t conquer itself while you’re gone.",
        "Farewell! And may your gold pouch stay full.",
        "Safe journey! I’ll try not to replace you while you’re away.",
        "Bye! Don’t let the hunters’ pets chase you off.",
        "Goodbye! Remember to log your adventures in your guild chat.",
        "Farewell! And may your arrows fly true.",
        "Later! Don’t trust goblins with your valuables.",
        "See you soon! Unless you get stuck in a time rift.",
        "Take care! And don’t taunt the wildlife.",
        "Goodbye! Watch your back—and your front."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomSnickerResponse()
{
    std::vector<std::string> responses = { 
        "Oh, laughing at me, huh? Just wait until the next pull!",
        "Your laugh is as weak as your DPS.",
        "Snicker all you want; I’ll still roll higher on loot.",
        "You laugh now, but let’s see how you handle the next boss.",
        "Did you just snort while laughing? That’s adorable.",
        "Keep laughing, but remember who saved your hide last time.",
        "You think that’s funny? Try soloing a dungeon.",
        "Laugh it up; we’ll see who survives the raid.",
        "Was that a chuckle or a hiccup? Either way, it’s pitiful.",
        "Snicker away, but I’m the one holding the aggro.",
        "Laughing won’t increase your hit rating, just saying.",
        "If your laugh was a spell, it’d fizzle on cast.",
        "I hear murlocs laugh better than that.",
        "Snickering at me? I hope you like pulling aggro.",
        "I’d laugh too if I had to wear your transmog.",
        "Is that laughter? Sounds like you stepped in a bear trap.",
        "You laugh like a goblin after a bad deal.",
        "Chuckling now? Wait until you see your repair bill.",
        "Oh, you’re laughing? Good luck when you need a rez.",
        "That snicker reminds me of a gnoll’s dying breath.",
        "Laugh all you want; I’ll still out-crit you.",
        "You laugh now, but just wait until I win the roll.",
        "Is that your victory laugh? Weak.",
        "Snickering? Bold move for someone standing in the fire.",
        "Oh, you’re giggling? Wait until the next wipe.",
        "Keep laughing; I’ve got the screenshot of your fail.",
        "That laugh is more annoying than a Pandaren trying to be edgy.",
        "You call that a laugh? I’ve heard kobolds with better delivery.",
        "You laugh like a skeleton from the Barrens.",
        "Snickering suits you; you’re as subtle as a kodo stampede.",
        "I laugh at your laugh—it’s like a bad goblin invention.",
        "Laugh all you want; you’ll need my heals soon.",
        "Your laugh is as reliable as a gnome’s engineering.",
        "Oh, you’re laughing? Let’s see you kite those adds.",
        "Snickering won’t save you when the boss enrages.",
        "I’d laugh too if I were that clueless.",
        "You call that laughter? Even the Scourge wouldn’t resurrect it.",
        "Your laugh has the charm of a rogue stealing your gold.",
        "I laugh in the face of your laughter! Ha!",
        "That laugh? It’s about as impressive as a level 1 crit.",
        "Snickering at me? Better be ready to tank the next pull.",
        "Keep laughing; I’m not the one who forgot to repair.",
        "Your laughter is weaker than a mage without mana.",
        "Laugh all you want; I’m still rolling for the mount.",
        "I’d laugh, but your performance speaks for itself.",
        "You laugh like a Trogg that stepped on a rake.",
        "Snicker now, but don’t cry when I ninja-loot.",
        "Was that a laugh or a distress call?",
        "I’ve heard Voidwalkers with better jokes than that laugh.",
        "You’re laughing? Must’ve been a crit on your ego.",
        "That laugh just made my day. Thanks for that!",
        "Oh, now that’s a hearty laugh! Keep it up!",
        "Your laugh is contagious—can’t stop grinning now!",
        "I needed that laugh. Thanks for sharing it.",
        "Haha, your laugh could cheer up a raid wipe.",
        "You’ve got the kind of laugh that lights up a room.",
        "That’s the kind of laugh that makes even the Lich King smile.",
        "Your laughter is like music in Stormwind’s Trade District.",
        "I can’t help but laugh along with you!",
        "You’ve got the best laugh on Azeroth, hands down.",
        "I’d raid with you any day just to hear that laugh.",
        "Haha, if laughs were loot, yours would be an epic drop!",
        "That laugh could outshine the Light of Elune.",
        "You laugh like someone who’s just ninja-looted joy.",
        "It’s impossible to stay grumpy with that laugh around.",
        "Your laugh belongs in a bard’s tale—it’s legendary!",
        "That laugh could charm even a grumpy dwarf.",
        "Your laughter is better than any warlock’s Life Tap.",
        "Haha, your laugh is worth more than a full bag of gold.",
        "I’d roll need on your laugh any day!",
        "That laugh is more refreshing than a healing rain.",
        "You laugh like someone who’s just critted life.",
        "Even Thrall would appreciate the joy in your laugh.",
        "Your laugh’s got more sparkle than a jewelcrafter’s finest.",
        "That laugh could rally an entire battleground!",
        "You laugh like someone who’s just unlocked flying in Northrend.",
        "If there’s an achievement for the best laugh, you’ve earned it!",
        "That laugh could warm the coldest reaches of Icecrown.",
        "Your laugh sounds like victory after a tough dungeon run.",
        "I’d set my hearthstone just to hear you laugh more often.",
        "You laugh like someone who’s found a rare mount on their first try.",
        "Haha, that laugh should be a buff—it lifts everyone’s spirits.",
        "Your laughter is the real MVP here!",
        "That laugh is better than a full raid flask buff.",
        "I wish I could bottle your laugh—it’s pure happiness.",
        "Your laugh reminds me why Azeroth is worth saving.",
        "That’s the kind of laugh that brings factions together.",
        "Your laugh is like a cheerful crit—unexpected and delightful.",
        "Haha, I think your laugh just pulled all the good vibes in range.",
        "Your laughter could revive the whole party after a wipe.",
        "That laugh is like a heroic tier reward—absolutely priceless.",
        "If I had a coin for every time your laugh made me smile, I’d be rich.",
        "That laugh should be a raid-wide cooldown—it’s that good!",
        "You laugh like someone who’s mastered every class and still finds joy.",
        "Your laugh is like finding a rare spawn—it’s a treasure!",
        "Haha, your laugh alone could conquer the Burning Legion.",
        "You laugh like someone who’s just looted a legendary weapon.",
        "That laugh could melt the heart of a Death Knight.",
        "Your laugh feels like a hearthstone back to a cozy inn.",
        "Why don’t druids ever get lost? They just follow their roots!",
        "What’s a rogue’s favorite dish? Anything they can steal.",
        "Why did the warlock bring a picnic basket? To summon a fel feast!",
        "What do you call a paladin without a weapon? A light show.",
        "Why don’t night elves play cards? They don’t trust decks with too many shuffles.",
        "How do orcs stay strong? They horde their calories.",
        "Why was the goblin so bad at dating? He always blew up the moment.",
        "What’s a mage’s favorite type of comedy? Dry ice humor.",
        "Why did the warrior refuse to wear enchanted boots? He couldn’t stand the extra steps.",
        "Why are worgen terrible bakers? They can’t stop howling at the moon pies.",
        "Why do tauren make great poets? They’re outstanding in their field.",
        "What’s the undead’s favorite band? The Rolling Bones.",
        "Why don’t mages ever win hide-and-seek? They blink too much.",
        "Why did the troll bring a ladder to the dungeon? He wanted to get on top of the damage meters.",
        "What’s a blood elf’s favorite meal? Mana-ni and cheese.",
        "Why do gnomes make terrible gardeners? They can’t reach the weeds.",
        "How do you know a paladin has been to your house? Everything’s been judged.",
        "Why did the death knight open a bakery? He kneaded a new profession.",
        "Why do hunters hate mornings? They’re not fans of rising shots.",
        "Why do warlocks never play sports? They’re afraid of losing their soul shards.",
        "What’s a priest’s favorite type of magic? Holy smokes!",
        "Why did the shaman open a gym? He’s great at raising spirits.",
        "How do druids like their coffee? With a lot of bark and a little bite.",
        "Why did the rogue get kicked out of the tavern? He was caught pick-pocketing the jukebox.",
        "What’s the Horde’s favorite type of music? Orc-chestral.",
        "Why did the undead start a band? They wanted to raise the roof.",
        "What’s a goblin’s favorite dessert? Profit rolls.",
        "Why did the druid quit their job? It was unbearable in bear form.",
        "What’s a mage’s favorite holiday? Frost Festival!",
        "Why do death knights always win arguments? They have killer points.",
        "Why are trolls great musicians? They always stay in tempo.",
        "What’s a dwarf’s favorite drink? Anything that’s on tap.",
        "Why did the blood elf bring a mirror to the raid? To reflect on their talents.",
        "Why don’t shamans ever gamble? They already know the odds.",
        "Why did the priest take a nap in the dungeon? They needed divine rest.",
        "What’s an orc’s favorite workout? Deadlifts.",
        "Why don’t tauren play soccer? They can’t handle the horns.",
        "Why do worgen love full moons? It’s the perfect night for a howling party.",
        "What’s a death knight’s favorite flower? Frostmourne-ing glories.",
        "Why do undead hate beach vacations? Too much sunburn.",
        "Why was the mage so calm in battle? They knew how to chill.",
        "Why do hunters never get lost? They always track their way back.",
        "What’s a troll’s favorite dance move? The voodoo shuffle.",
        "Why did the warrior always carry a map? To avoid getting lost in thought.",
        "Why do blood elves love parties? They’re all about mana-tainment.",
        "What’s the difference between a warlock and a vacuum? One sucks the soul, the other sucks the floor.",
        "Why did the paladin fail the test? They couldn’t find their divine answers.",
        "Why do goblins love economics? It’s explosive growth!",
        "What’s a gnome’s favorite type of racing? Short tracks."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomEyebrowResponse()
{
    std::vector<std::string> responses = { 
        "Oh, raising an eyebrow? What are you plotting?",
        "You raise an eyebrow like that, and I start thinking you’re up to something.",
        "That eyebrow has more attitude than an entire orc raid.",
        "Raise it any higher, and you’ll summon a storm.",
        "Ah, the skeptical eyebrow. A classic move.",
        "Careful, that raised brow might get stuck like that.",
        "What, the eyebrow is judging me now?",
        "If looks could kill, that eyebrow would be legendary loot.",
        "An eyebrow raise? I must have said something profound.",
        "Stop flexing your eyebrow muscles; I’m already intimidated.",
        "That’s the kind of eyebrow that starts revolutions.",
        "Whoa, is your eyebrow trying to duel me?",
        "I didn’t realize eyebrows could convey disappointment so effectively.",
        "That eyebrow looks like it knows all my secrets.",
        "Raise it any higher, and it might hit the ceiling.",
        "Does that eyebrow come with an instruction manual?",
        "That’s not just an eyebrow, that’s a weapon of mass sarcasm.",
        "You’ve got a rogue’s eyebrow. Sneaky, sharp, and dangerous.",
        "Your eyebrow says ‘no,’ but your smirk says ‘maybe.’",
        "Is that an eyebrow raise or a silent accusation?",
        "The eyebrow is strong with this one.",
        "Your eyebrow is doing more talking than your mouth.",
        "Do I get a prize for making your eyebrow react?",
        "That eyebrow says, ‘I don’t believe you,’ and I’m scared.",
        "What did I do to deserve the wrath of your eyebrow?",
        "Your eyebrow just made me rethink my entire life.",
        "You could write entire epics about that eyebrow.",
        "Is this the legendary Eyebrow of Judgment I’ve heard about?",
        "Your eyebrow’s sarcasm level just reached critical mass.",
        "Why do I feel like your eyebrow knows something I don’t?",
        "That eyebrow raise was more dramatic than an entire Draenei opera.",
        "Your eyebrow has more character than some raid bosses.",
        "I think your eyebrow just called me out.",
        "Did your eyebrow just declare war?",
        "That eyebrow raise deserves its own achievement.",
        "I’m not saying your eyebrow is intimidating, but I’m considering running.",
        "You could level up just from eyebrow raises alone.",
        "If eyebrows could emote, yours would be a fireball.",
        "Your eyebrow should get a quest line of its own.",
        "Are you casting a spell with that eyebrow? It looks magical.",
        "You raise that eyebrow any higher, and it’ll join the Exodar.",
        "Your eyebrow has officially entered ‘legendary artifact’ status.",
        "Stop showing off with your eyebrow! Some of us can’t do that.",
        "The eyebrow is strong, but the will is stronger.",
        "Is that eyebrow a challenge? Because I’m not ready.",
        "Your eyebrow has the finesse of a rogue and the weight of a warrior.",
        "I think your eyebrow just whispered ‘prepare yourself.’",
        "Does your eyebrow get tired from all that heavy lifting?",
        "An eyebrow raised in Azeroth is never a good sign.",
        "I feel like your eyebrow is about to reveal a plot twist.",
        "That eyebrow looks like it has a mind of its own.",
        "Raise that eyebrow any higher, and it might get its own mount.",
        "Is your eyebrow the leader of the Horde? It sure commands attention.",
        "If your eyebrow could talk, it’d have better comebacks than you.",
        "Whoa, that’s a raid boss of an eyebrow raise.",
        "Your eyebrow just critted me for emotional damage.",
        "If I were a critter, I’d flee from that eyebrow.",
        "I didn’t know eyebrows could be so condescending.",
        "Your eyebrow just gave me a debuff called ‘self-doubt.’",
        "Do you practice eyebrow raises in front of a mirror?",
        "Your eyebrow is working harder than a dungeon pug healer.",
        "The Archmage himself couldn’t conjure an eyebrow raise like that.",
        "Did your eyebrow just try to tank this conversation?",
        "I feel like your eyebrow just looted my self-esteem.",
        "Your eyebrow is throwing more shade than an Ashenvale forest.",
        "Was that an eyebrow raise or a signal for backup?",
        "Your eyebrow deserves its own faction—The Eyebrows of Judgment.",
        "I swear your eyebrow just said, ‘Do better.’",
        "That eyebrow is doing more work than a gnome engineer.",
        "Your eyebrow should be on the Kirin Tor Council.",
        "Does your eyebrow know it’s the MVP of this raid?",
        "I think your eyebrow just queued for a battleground.",
        "That eyebrow has more lore behind it than the Emerald Dream.",
        "Your eyebrow just stole aggro from the main tank.",
        "That eyebrow raise feels like a taunt, and I’m not geared for it.",
        "You raise that eyebrow like a rogue unsheathing a dagger.",
        "Was that an eyebrow raise or a summon for reinforcements?",
        "Your eyebrow has more charisma than half the NPCs in Stormwind.",
        "Your eyebrow should get its own Hearthstone card.",
        "Are you casting Arcane Intellect or just raising an eyebrow?",
        "Your eyebrow looks like it’s about to solo the Lich King.",
        "That eyebrow raise should be classified as a CC ability.",
        "Your eyebrow has more confidence than a fully geared warlock.",
        "Are you raising an eyebrow or starting a rebellion?",
        "That eyebrow raise could scare off an Old God.",
        "Is that eyebrow trying to lead a raid? Because I’d follow it.",
        "Your eyebrow just rolled a natural 20 in sarcasm.",
        "I think your eyebrow just rez’d my anxiety.",
        "Your eyebrow is so sharp, it could cut through Saronite.",
        "That eyebrow raise feels like it should come with a cooldown timer.",
        "Your eyebrow has more authority than Varian Wrynn.",
        "That eyebrow is as unyielding as a shield wall.",
        "Your eyebrow raise should be a raid mechanic.",
        "The Light protects, but your eyebrow terrifies.",
        "That eyebrow could provoke a world event.",
        "Are you raising your eyebrow or summoning an elemental?",
        "Your eyebrow could convince Thrall to switch factions.",
        "That eyebrow should be nerfed in the next patch.",
        "I’m pretty sure your eyebrow just triggered a lore debate.",
        "That eyebrow has more personality than most quest givers."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomBreathResponse()
{
    std::vector<std::string> responses = { 
        "Did you just breathe on me? Was that supposed to be a buff?",
        "Your breathing sounds like it’s trying to solo a raid boss.",
        "If your breathing were any louder, it’d pull aggro.",
        "Do you always breathe like a dragon about to cast flame breath?",
        "Whoa, calm that breathing before you start a windstorm.",
        "Your breathing is heavier than my inventory after a dungeon run.",
        "I feel like your breathing just cast a fear debuff.",
        "If breathing was a combat ability, you’d be overpowered.",
        "That breathing sounds like you’ve been tanking without a healer.",
        "Are you okay? Your breathing sounds like you’re stuck in Molten Core.",
        "Is that breathing or the sound of an incoming murloc raid?",
        "I didn’t know breathing could sound sarcastic, but here we are.",
        "Your breathing is as subtle as an orc at a wine tasting.",
        "Can you not breathe down my neck like a rogue in stealth?",
        "If you keep breathing like that, someone’s going to call an exorcist.",
        "Do you always breathe like you’re sprinting across Durotar?",
        "Your breathing just triggered a nearby mob. Nice pull.",
        "I think your breathing just scared a critter into fleeing.",
        "If your breathing were a spell, it’d have a five-second cast time.",
        "That breathing sounds like it’s carrying unresolved dungeon trauma.",
        "Your breathing is louder than a gnome’s laugh in Ironforge.",
        "Can you breathe quieter? You’re making the cave echoes nervous.",
        "Your breathing reminds me of an angry dragon waking up.",
        "If your breathing had a cooldown, you’d still be spamming it.",
        "Your breathing sounds like it’s plotting a betrayal.",
        "I can’t tell if that breathing is determined or just exhausted.",
        "You’re breathing like you just lost a roll on an epic item.",
        "Do you breathe like that in combat? Must be hard on the raid group.",
        "Your breathing is more ominous than the whispers of an Old God.",
        "That breathing sounds like a warlock summoning a demon.",
        "Are you breathing or casting a frost spell? Hard to tell.",
        "Your breathing is louder than a tauren’s footsteps in stealth.",
        "If breathing were a PvP ability, you’d have me defeated.",
        "Why is your breathing so aggressive? Are you in berserk stance?",
        "Your breathing is making me think we’re in a haunted crypt.",
        "I think your breathing just hit me with a slow debuff.",
        "Do you practice dramatic breathing for roleplay or is it natural?",
        "Your breathing sounds like a dungeon boss entering phase two.",
        "Are you breathing or trying to communicate with dragons?",
        "That breathing is so intense it should have its own raid warning.",
        "I think your breathing just triggered a world event.",
        "Your breathing is heavier than a fel reaver’s footsteps.",
        "Are you breathing or channeling an AoE attack?",
        "Your breathing sounds like a blacksmith hammering in Ironforge.",
        "That breathing could scare away even the bravest adventurer.",
        "You’re breathing like someone just ninja-looted your drop.",
        "Your breathing is more intimidating than an enraged yeti.",
        "I think your breathing just cast an intimidation aura.",
        "Your breathing is like a boss mechanic: unavoidable and scary.",
        "If I rolled for stealth, your breathing would give me away.",
        "Your breathing is like a tauren charging—hard to miss.",
        "If I didn’t know better, I’d say your breathing is taunting me.",
        "That’s some intense breathing. Did you just climb Mount Hyjal?",
        "Your breathing could make even an undead cringe.",
        "Are you breathing or practicing for dragon roar duty?",
        "Your breathing is more dramatic than a Sylvanas monologue.",
        "I didn’t know breathing could sound like a raid wipe.",
        "Your breathing just pulled aggro from the healer. Nice job.",
        "Are you sure that breathing isn’t a DOT effect?",
        "Your breathing is louder than goblin machinery at full speed.",
        "That breathing could be a new Horde war cry.",
        "I feel like your breathing is prepping for a spell cast.",
        "Are you breathing or summoning an infernal?",
        "Your breathing just set off the proximity alert.",
        "I think your breathing spooked the gryphon master.",
        "That breathing sounds like you’ve been stuck in a battleground too long.",
        "If breathing were a talent, you’d be fully specced.",
        "Are you trying to communicate in Morse code with your breathing?",
        "That breathing just triggered my ‘move away from fire’ instincts.",
        "You’re breathing like you just tanked without a shield.",
        "Your breathing could be the soundtrack for a Forsaken drama.",
        "That breathing could double as a battle horn.",
        "Are you casting a wind spell with that breathing?",
        "Your breathing is like a bard tuning their instrument—never quiet.",
        "Did your breathing just pull a wandering patrol?",
        "Your breathing is heavier than my repair bill after a dungeon.",
        "If I didn’t know better, I’d think you’re a troll prepping a hex.",
        "Your breathing sounds like it’s questioning my life choices.",
        "I swear your breathing just crit me for emotional damage.",
        "Your breathing is like a Horde charge: loud and relentless.",
        "Did your breathing just activate a hidden boss mechanic?",
        "I’m starting to think your breathing has a cooldown timer.",
        "That breathing could wake up Ragnaros himself.",
        "Your breathing has the urgency of a tank without a healer.",
        "Are you sure your breathing isn’t summoning a worgen pack?",
        "I think your breathing just flagged us for PvP.",
        "Your breathing sounds like a spirit healer waiting for you.",
        "Did you learn to breathe from a grumpy dwarf?",
        "Your breathing is giving me flashbacks to aggroing Hogger.",
        "That breathing could clear a raid group faster than bad loot rolls.",
        "I’m impressed. Your breathing has its own AoE range.",
        "Are you trying to intimidate me with that breathing?",
        "Your breathing sounds like an NPC that’s about to give up a quest.",
        "I think your breathing just phased us into a different timeline.",
        "Your breathing could break CC faster than a newbie hunter.",
        "If your breathing gets any louder, it might count as a world boss.",
        "That breathing is stronger than my DPS on a bad day.",
        "Your breathing just made the auctioneer look concerned.",
        "Are you training to be a dragon aspect with that breathing?",
        "Your breathing is louder than a raid leader’s complaints."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomCryResponse()
{
    std::vector<std::string> responses = { 
        "It’s okay to cry. Even the strongest warriors need to let it out.",
        "Don’t worry, friend. We’re here for you.",
        "Let it out; tears can cleanse even the deepest wounds.",
        "Every hero cries once in a while. It’s part of the journey.",
        "You’re not alone. Let’s face this together.",
        "If crying helps, I’ll stay here with you until it passes.",
        "Even the Earthmother would shed a tear for what you’re going through.",
        "Tears are a sign of a heart that still cares.",
        "We’ve all been there. Take your time, we’ll wait for you.",
        "It’s okay, we can rest here while you gather yourself.",
        "Sometimes, crying is the bravest thing you can do.",
        "You’re allowed to feel this way. It makes you human… or, you know, humanoid.",
        "Don’t be ashamed. Even the Light weeps at times.",
        "I’d rather see your tears than see you bottle it all up.",
        "If the Elemental Lords can rage, you can cry. It’s only fair.",
        "Crying doesn’t make you weak—it makes you real.",
        "It’s okay. Azeroth has seen worse days, and so have we.",
        "Even the forest cries when the rains come. You’re no different.",
        "If it helps, I’ll cry with you.",
        "Your tears show how deeply you care, and that’s a strength.",
        "We’ve got your back, no matter how many tears it takes.",
        "You’re stronger than you think, even in your tears.",
        "Think of this as an emotional debuff. We’ll dispel it together.",
        "Sometimes the bravest thing a warrior can do is cry.",
        "Even heroes need a moment to mourn. Take yours.",
        "I’d hug you if I could, but my armor’s a bit spiky.",
        "Tears are like rivers—they find their way, even in stone.",
        "It’s okay, friend. The journey is long, and we’re all here for you.",
        "Even dragons cry when they’re alone. You’re no different.",
        "You’re not alone. I’m here, and so is the rest of the group.",
        "It’s not a weakness; it’s a part of healing.",
        "Sometimes, the strongest spells come from the heart.",
        "Take all the time you need. Azeroth isn’t going anywhere.",
        "You’ll be okay. We’ve faced worse together, haven’t we?",
        "Even the strongest shield needs mending after a battle.",
        "I’d rather see your tears than see you give up.",
        "If crying helps, don’t hold back.",
        "Every tear is a step toward feeling better.",
        "You’re not a failure for feeling this way. You’re just real.",
        "Even the Naaru sing with sorrow sometimes. You’re in good company.",
        "Take your time. Even the tides take a moment to ebb and flow.",
        "Your tears are safe here. Let them fall.",
        "There’s no shame in crying. It’s part of the story.",
        "We’re all here for you, no matter what.",
        "Even in tears, you’re one of the strongest I know.",
        "It’s okay, friend. The Light will guide us through this.",
        "Your tears only make you more heroic in my eyes.",
        "If the Sunwell can burn brightly, so can you, even in this moment.",
        "It’s okay to feel this way. We’ve got your back.",
        "Even the strongest hearts sometimes need to let go.",
        "Let those tears flow; they carry away the pain.",
        "Even heroes need a moment to cry. Take yours.",
        "Your emotions are valid. Don’t let anyone tell you otherwise.",
        "Sometimes the bravest thing you can do is let yourself feel.",
        "The Light sees your pain and holds you in its warmth.",
        "A true warrior doesn’t hide their tears.",
        "If the Titans weep for Azeroth, why shouldn’t we?",
        "You’re allowed to cry. It means you’re alive.",
        "Even the strongest armor doesn’t protect the heart from feeling.",
        "You’re safe here. Let your tears fall freely.",
        "Every tear is a testament to your strength, not your weakness.",
        "It’s okay to cry. Even the mightiest mountains crumble sometimes.",
        "Don’t hold it in. Your tears are part of your story.",
        "Even in your sorrow, you inspire those around you.",
        "The Earthmother watches over you in your pain.",
        "Tears are the rain that nurtures the seeds of healing.",
        "You’re not alone in this. Let it out.",
        "Even Thrall has shed tears for what he’s lost.",
        "Crying doesn’t make you weak; it makes you real.",
        "I’ll stand with you, even if the world feels heavy.",
        "Take a deep breath. We’re here for you.",
        "Let the tears come. The storm always passes.",
        "Your pain is valid. Let it out.",
        "You’re not a failure for crying. You’re a fighter who feels.",
        "Even the Light mourns. Let yourself grieve.",
        "Tears are the song of the soul. Sing it, friend.",
        "I’d rather see you cry than see you fade away.",
        "Your tears are a sign of your strength to endure.",
        "You’ve carried so much; it’s okay to let it go.",
        "Even the forest weeps in the rain. You’re not alone.",
        "It’s okay to feel broken. We’ll help you mend.",
        "Your heart is heavy, but you’re not carrying it alone.",
        "Take your time. Even the strongest heroes need to pause.",
        "You’re not a burden; you’re a treasure worth protecting.",
        "We all break sometimes. That’s how the light gets in.",
        "Your tears water the roots of your strength.",
        "Even the hardest battles leave scars. Let yourself heal.",
        "You’re stronger than you know. Let the tears cleanse you.",
        "You’ve given so much. It’s okay to take a moment for yourself.",
        "Crying is just another way of saying you care.",
        "Your sorrow is your power. Embrace it and rise.",
        "Even the Naaru have moments of shadow. It’s okay to feel.",
        "Every tear brings you closer to peace.",
        "You’re not alone. We’re with you every step of the way.",
        "Take your time. We’ll be here when you’re ready.",
        "Your heart has been through so much. Let it rest.",
        "Tears don’t make you weak; they make you human.",
        "The Light guides you, even in your darkest moments.",
        "I see your pain, and I honor your strength in facing it.",
        "Your emotions are a gift. Don’t hide them."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomCrackResponse()
{
    std::vector<std::string> responses = { 
        "Sounds like someone’s getting ready for a fight!",
        "Alright, tough guy, we hear you.",
        "Did you just crack a whole tree or just your knuckles?",
        "Save those knuckles for the real enemies.",
        "Careful, you might scare the murlocs with that sound.",
        "Whoa, are those your knuckles or breaking bones?",
        "You’re either getting ready to brawl or about to play the piano.",
        "That’s the sound of intimidation… or arthritis.",
        "Ease up! Your knuckles aren’t supposed to sound like that.",
        "Alright, we get it, you’re tough. Let’s move on.",
        "Crack your knuckles all you want, it won’t help your DPS.",
        "That’s either determination or a nervous habit.",
        "What did your knuckles ever do to you?",
        "You sure those cracks aren’t going to summon a demon?",
        "I hope you didn’t break anything important with that noise.",
        "Do that again and you’ll scare off all the critters.",
        "Alright, alright, we know you’re serious!",
        "Your hands sound more prepared for battle than you do.",
        "Careful, that sound’s going to attract a healer thinking you’re injured.",
        "Sounds like you’re trying to intimidate the air.",
        "Okay, tough knuckles, what’s the plan?",
        "That sound was louder than my entire spell rotation.",
        "You’re cracking knuckles like a boss. Or a wannabe boss.",
        "Do you crack your knuckles before every boss fight?",
        "Hope your knuckles are ready, the mobs won’t wait.",
        "If cracking knuckles could crit, you’d top the meters.",
        "Save that energy for the dungeon, alright?",
        "Well, that was… dramatic. What now?",
        "If you keep doing that, we might need a healer just for you.",
        "Your knuckles sound more aggressive than your sword.",
        "You might intimidate someone with that… someday.",
        "Did that sound come from your hand or your soul?",
        "That’s the sound of someone ready to take on Arthas himself!",
        "Alright, no more cracking until we win this fight.",
        "Your knuckles sound like they’re casting their own spell.",
        "Was that supposed to be impressive, or just loud?",
        "You’ve got louder knuckles than most warriors I know.",
        "That crack sounded like it came from Northrend.",
        "If knuckles had health bars, yours would be depleted.",
        "Alright, Hulk, calm down with the hand noises.",
        "Do you crack them to think better or just to annoy us?",
        "Those knuckles better back up all that noise in battle.",
        "You crack knuckles; I’ll crack jokes. Fair trade?",
        "Please tell me that was your hand and not your spine.",
        "If knuckle-cracking was a skill, you’d be maxed out.",
        "Your hands just declared war. On what, I’m not sure.",
        "I swear, one of these days your fingers are going to revolt.",
        "That sound was so loud it might’ve aggroed something.",
        "You might break your own hand before breaking the enemy.",
        "Alright, show us if your fists are as good as the noise!",
        "Let’s do this. Time to show them what we’re made of!",
        "Crack your knuckles and your fears, it’s go time!",
        "Ready up, team. They don’t stand a chance!",
        "Stretch, crack, breathe – let’s finish this quick.",
        "Alright, party. Keep it tight, keep it sharp.",
        "We’re not walking away from this. We’re running through it.",
        "Time to put all that training to use. Let’s go!",
        "Hope you all ate your buff food – we’ll need it!",
        "That sound? That’s the noise of victory warming up.",
        "This is it, everyone. No turning back now.",
        "Let’s make this fight one for the history books.",
        "Remember the plan. Stick together and we win this.",
        "No mercy, no hesitation. Let’s show them our strength!",
        "They won’t know what hit them. Let’s move!",
        "I’ll take point. You follow and clean up the rest!",
        "The air smells of tension. Let’s break it with our blades.",
        "Let’s remind them why nobody messes with us.",
        "Weapons ready, focus sharp. It’s our time to shine.",
        "Channel your rage, your fire, your strength. We’ll win this.",
        "Let’s show them what teamwork really looks like.",
        "Hope you all stretched – this fight’s going to get wild.",
        "Brace yourselves. They’re big, but we’re bigger together.",
        "This isn’t just a fight – it’s a lesson they won’t forget.",
        "Take a deep breath. Now unleash hell!",
        "Get in there and show them why we’re the best.",
        "Eyes on the target. Don’t let up until it’s over!",
        "We’ve faced worse, we’ve won harder. This is just another fight.",
        "Time to make some noise and break some egos!",
        "When the dust settles, we’ll be the ones standing.",
        "Let’s send them back to their makers in pieces!",
        "Make every swing count. Let’s end this fast.",
        "They won’t see us coming. Let’s hit them hard.",
        "One last prep. Alright, let’s go!",
        "Don’t stop until their morale is shattered.",
        "We’re not fighting alone – we’ve got each other.",
        "Let’s make sure this battle is our masterpiece.",
        "Let’s crush them so hard they’ll think twice next time.",
        "Stand tall, fight hard, and leave no regrets.",
        "Time to settle this once and for all. Let’s go!",
        "No distractions, no fear – just victory ahead.",
        "They picked the wrong group to mess with.",
        "Steel yourselves. It’s going to be a glorious fight.",
        "Don’t hold back. Let’s give them everything we’ve got.",
        "Weapons up, minds focused. Time to fight!",
        "We fight as one, we win as one. Let’s move.",
        "Let’s show them the meaning of true teamwork.",
        "This is the moment we’ve trained for. Let’s seize it.",
        "Together, there’s no enemy we can’t conquer.",
        "This is our time. Let’s make every second count!",
        "Time to turn the tide and take them down!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomDrinkResponse()
{
    std::vector<std::string> responses = { 
        "Ah, finally, a moment to sit and sip something strong.",
        "Nothing like a drink to wash away the dust of battle.",
        "I don’t care what it is, just pour it already!",
        "A toast to surviving another day!",
        "Sit down, shut up, and let the ale do the talking.",
        "Every victory deserves a drink, and we’ve earned this one.",
        "I’ll drink to anything right now. What’s the occasion?",
        "Let’s see if this tavern lives up to its reputation.",
        "Pull up a chair, the drinks are on me... or they would be if I had gold.",
        "One drink to start, two to forget, three to regret.",
        "I’m not drunk yet, but I’m working on it.",
        "This ale tastes like it’s been sitting since the first war.",
        "The best part of the day? This right here, a drink and no worries.",
        "If you don’t sit and join me, I’ll drink yours too.",
        "Let’s raise a mug to bad decisions and good company!",
        "Drinking alone is fine, but it’s more fun with a crowd.",
        "Ah, the sweet sound of a mug hitting the table.",
        "Is this wine, or did someone leave a potion out too long?",
        "Sit down, grab a mug, and let’s share some stories.",
        "One more round, then maybe we’ll fight something again.",
        "Drinking with friends makes even the worst brew taste better.",
        "I’ll have whatever doesn’t taste like goblin oil.",
        "Here’s to forgetting everything I just saw on that battlefield.",
        "They say ale is liquid courage. I call it liquid wisdom.",
        "Sit down, relax, and drink before the next fight finds us.",
        "Nothing warms the soul like a good drink and a roaring fire.",
        "Is it just me, or does everything taste better in Ironforge?",
        "If this isn’t ale, I don’t want to know what it is.",
        "The mug’s empty already? Someone get me another!",
        "Why stand when you can sit and enjoy a brew?",
        "The only thing stronger than this drink is my headache tomorrow.",
        "Drinking with adventurers is always a gamble, but I’m in.",
        "A good drink should come with a better tale. Let’s hear it.",
        "This brew’s so bad, I think it’s fighting me back.",
        "Sit down, drink up, and let the world wait for a while.",
        "I’m not sure what this is, but it’s making me feel something.",
        "Let’s see if we can outlast the bartender tonight.",
        "Drinking like this is practically a second job.",
        "The tavern’s the best place to strategize... after a few rounds.",
        "I’d fight less if every battle ended with a drink like this.",
        "Why fight when we can drink? Let’s toast to peace.",
        "This mug is the best weapon I’ve held all day.",
        "A drink for me, and one for whoever brought me here alive.",
        "Sit down and enjoy it. Life’s short, but the ale flows forever.",
        "Drinking on an empty stomach? Bad idea. I’ll take two more.",
        "They brew this stuff in Stormwind? I’m impressed!",
        "One mug of ale, and suddenly, the world doesn’t seem so bad.",
        "I’d rather sit here and drink than face another spider cave.",
        "A good drink is like a good fight: worth remembering.",
        "A good drink and better company—what more could I ask for?",
        "Who needs food when the ale is this strong?",
        "This stuff burns going down, but it’s worth it.",
        "Is this ale, or did someone just bottle a swamp?",
        "Every drink tells a story. What’s yours?",
        "Drinking solves nothing, but it sure makes you forget the problem.",
        "Ah, a seat, a mug, and no enemies in sight. Bliss.",
        "I swear, this is the only thing keeping me sane.",
        "They don’t make it like this in the Exodar. Thank the Light.",
        "I’d fight a dragon for a drink this good. Actually, I just did.",
        "This one’s for the fallen. May they rest in peace.",
        "Who needs mana potions when you’ve got mead?",
        "Does this ale taste better, or have I just lowered my standards?",
        "Pour me another before the buzz wears off.",
        "A toast to those who still owe me gold!",
        "Drinking together beats dying alone, wouldn’t you say?",
        "The tavern is a better battlefield—no blood, just spills.",
        "If this ale doesn’t knock me out, I’ll be impressed.",
        "One mug down, and the world looks a little brighter.",
        "Drinks on the table, and nothing else matters.",
        "You call this ale? I’ve had stronger water.",
        "This stuff could fuel a siege engine. I’ll take another.",
        "Every mug is a small victory over sobriety.",
        "This tavern’s so cozy, I might never leave.",
        "Here’s to finding the bottom of every mug!",
        "A good drink tastes even better after a bad day.",
        "If the brew’s not strong enough, add a little magic.",
        "I’d call this swill, but I’m already on my third mug.",
        "Drinking together makes the loot sweeter, doesn’t it?",
        "Does anyone else feel like the tavern’s spinning?",
        "One more round, then we’ll save the world. Maybe.",
        "This is what adventure leads to: a mug and a smile.",
        "Who cares if it’s watered down? It’s still drinkable.",
        "A drink this strong should come with a warning label.",
        "We should start every quest with a round like this.",
        "I don’t always drink ale, but when I do, I finish the barrel.",
        "Nothing says ‘job well done’ like a toast to ourselves.",
        "The only thing stronger than this ale is my will to survive.",
        "This is why I adventure—to afford nights like this.",
        "Drinking alone is sad. Drinking with friends is legendary.",
        "This brew’s got kick! Or was that just the barkeep?",
        "Sit down, drink up, and let’s forget the last dungeon run.",
        "Every hero needs a break, and this is mine.",
        "I’d trade all my loot for one more mug like this.",
        "Who knew relaxation could taste so good?",
        "This ale’s so good, it could charm a banshee.",
        "Drinking and plotting—what could go wrong?",
        "Here’s to a tavern that never runs out of ale!",
        "If this isn’t heaven, it’s pretty close.",
        "Let the world burn. I’ve got a full mug."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomEmbarrassResponse()
{
    std::vector<std::string> responses = { 
        "Uh... I didn’t mean it like that...",
        "Can we just forget I said that?",
        "Oh no, did everyone hear that?",
        "Alright, fine, I’ll go sit in the corner.",
        "That’s... not what I meant at all!",
        "Please don’t bring that up again.",
        "Can we move on? Quickly?",
        "Oh Light, this is worse than that time with the ogres.",
        "I’ll just be over here... hiding.",
        "Why do you have to say it like that?",
        "Do you have to tell everyone?",
        "Okay, yes, that was my fault. Stop reminding me.",
        "I’d rather face a dragon than deal with this right now.",
        "Wow, just throw me under the mammoth, why don’t you?",
        "That’s... not how I remember it.",
        "Please stop laughing, this is serious!",
        "Okay, I get it. I messed up!",
        "Can we pretend this didn’t happen?",
        "I’ll never live this down, will I?",
        "You’re enjoying this way too much.",
        "I didn’t mean for it to sound like that!",
        "Yeah, alright, I’m the fool. Happy now?",
        "I’ll just... disappear for a while.",
        "Why do I even talk around you?",
        "That was... deeply unfortunate.",
        "Oh great, now the whole tavern knows.",
        "I wasn’t ready for this level of humiliation.",
        "You just love to see me squirm, don’t you?",
        "Okay, yes, it was dumb. Moving on.",
        "I feel like a kobold caught without a candle.",
        "Please tell me someone didn’t record that.",
        "Oh, the ancestors are cringing at me right now.",
        "Could someone cast vanish on me?",
        "That’s not fair, and you know it!",
        "Why does this always happen to me?",
        "Alright, I’ll admit, that was bad timing.",
        "This is almost as bad as the time I tripped in Orgrimmar.",
        "Stop making it worse, please!",
        "I’m just going to bury myself in the nearest sand dune.",
        "I can’t believe I just said that out loud.",
        "Alright, fine, laugh it up!",
        "Oh, I’m never going to hear the end of this.",
        "Does anyone have a spare hole I can crawl into?",
        "I think I’ll just go log off now.",
        "Okay, but in my defense, I panicked!",
        "That was a private moment, thank you very much!",
        "Yeah, yeah, laugh it up, everyone.",
        "I think my face is as red as a blood elf’s robes.",
        "I’ll just be quiet now.",
        "I walked right into that one, didn’t I?",
        "Alright, you win this round...",
        "I didn’t think it would hurt this much...",
        "Why do you always have to make me feel like this?",
        "I’m not crying, it’s just... dust, okay?",
        "Please, just stop. I’ve had enough.",
        "I thought we were friends...",
        "You didn’t have to say it like that...",
        "Oh no... why does it always happen to me?",
        "That’s not fair... why me?",
        "I’ll just go now... sorry for bothering you.",
        "I didn’t mean for it to turn out this way...",
        "Why does this hurt so much?",
        "Can we just pretend this didn’t happen?",
        "I’m sorry, I’ll do better next time, I promise.",
        "It’s okay, I’m used to it...",
        "I can’t stop thinking about how much that stung.",
        "You win. I give up.",
        "I don’t think I deserved that...",
        "I’m trying my best, why isn’t it enough?",
        "Please... just leave me alone.",
        "It’s fine... I’ll figure it out on my own.",
        "I thought you’d understand.",
        "Why do I always mess things up?",
        "I can’t believe this is happening...",
        "Oh Light, I’m so embarrassed.",
        "I’ll just... go hide in a cave now.",
        "I guess I wasn’t good enough after all.",
        "I should’ve known better.",
        "Why do I always feel like a failure?",
        "Please... stop making it worse.",
        "I thought we were on the same side...",
        "I never should’ve said anything.",
        "You’ve made your point. Loud and clear.",
        "I wish I could just disappear right now.",
        "No one ever listens to me anyway...",
        "I can’t take this anymore.",
        "Every time I try, it ends like this...",
        "Maybe I’m just not cut out for this.",
        "I thought I was doing well...",
        "Do you enjoy watching me fall apart?",
        "I’ll just keep quiet from now on.",
        "It hurts more than I thought it would.",
        "I never thought it’d end up like this.",
        "I’m sorry... I’ll leave you alone.",
        "I guess I was wrong to trust you.",
        "Why do I keep making the same mistakes?",
        "I can’t even look at anyone right now.",
        "I thought I mattered, at least a little.",
        "Just... give me some space, please.",
        "This isn’t what I wanted at all.",
        "I’m trying not to cry, but it’s hard..."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomHungryResponse()
{
    std::vector<std::string> responses = { 
        "I could eat a whole kodo right now.",
        "Is it me, or does everything smell like food?",
        "I’m so hungry I’d even eat dwarven rations.",
        "Does anyone have any leftover boar ribs?",
        "If I don’t eat soon, I’m gnawing on my own shield.",
        "Why does every meal come with a side of waiting?",
        "I think my stomach just growled louder than a worgen.",
        "Do you think murlocs taste like chicken?",
        "I’d trade my loot for a hot meal right now.",
        "Bread, water, anything! I’m desperate here!",
        "This hunger is worse than fighting a raid boss.",
        "Why don’t we have food vendors in the middle of the woods?",
        "Can we take a break? I’m about to pass out.",
        "If anyone has snacks, now’s the time to share.",
        "I can feel my soul leaving my body from hunger.",
        "Do you think those critters are edible?",
        "I swear I saw a turkey leg on that enemy. Let’s check.",
        "One more fight and I’ll be chewing on my boots.",
        "Who carries food and wants to be my new best friend?",
        "I miss the feasts back at camp.",
        "I’ve been dreaming of a hot pie for hours now.",
        "Is it normal to feel this hollow inside?",
        "I don’t care if it’s stale, I’ll eat anything.",
        "This adventure would be 10 times better with snacks.",
        "Why didn’t anyone tell me to pack more food?",
        "I’m so hungry I’m starting to eye those berries.",
        "Just imagining the next feast keeps me going.",
        "My kingdom for a slice of bread!",
        "If someone conjures food right now, I’ll name my next pet after them.",
        "This isn’t an adventure, it’s a hunger endurance test.",
        "Why don’t we ever fight enemies with better loot? Like sandwiches?",
        "Can someone point me toward the nearest inn?",
        "I’d even take a burnt piece of toast right now.",
        "Does hunger make anyone else think irrational thoughts?",
        "I’m convinced starvation is worse than any debuff.",
        "Why can’t we loot food from defeated foes?",
        "I can’t believe we passed that feast table without stopping.",
        "I should’ve stopped at the tavern in Goldshire.",
        "I knew I forgot something... like food!",
        "Can we please raid a pantry next?",
        "How do you fight when your stomach feels this empty?",
        "Do you think elves snack on leaves? Asking for a friend.",
        "If hunger were a mob, I’d be dead already.",
        "If you’ve got food, don’t make me beg for it!",
        "This might be my last adventure... hunger’s winning.",
        "My sword feels heavier with every missed meal.",
        "I’ve heard of food buffs, but what about food itself?",
        "Hungry enough to tame a wild beast and eat it.",
        "Next quest: find the legendary Sandwich of Sustenance.",
        "Does hunger count as a debuff? Because I’ve got it bad.",
        "The only raid I want is on a buffet table.",
        "If hunger were an enemy, I’d need a healer right now.",
        "Who needs loot when all I want is a loaf of bread?",
        "My stomach is growling louder than a charging kodo.",
        "What’s the respawn timer on food in this zone?",
        "Does anyone have a mage? I need some conjured snacks.",
        "Forget epic loot; I’d settle for a warm bowl of soup.",
        "If we don’t eat soon, I might start hallucinating turkey legs.",
        "I’d sell my gear for a plate of roasted boar.",
        "Is it just me, or do the trees here smell like bread?",
        "You can keep the loot; I’ll take the picnic basket.",
        "Is there a feast waiting at the end of this dungeon?",
        "I’m not saying I’d eat a gnome, but I’m considering it.",
        "What’s worse: low health or low food supply?",
        "I miss my mom’s cooking... even the burnt stuff.",
        "Who’s in charge of snacks? They’re fired.",
        "My hunger is so bad, I’d eat raw fish from Booty Bay.",
        "Do you think dragons ever share their meals?",
        "I could go for some Darnassian bleu cheese right now.",
        "If I had gold, I’d spend it all on a tavern feast.",
        "Do kobolds eat their candles? I might ask for one.",
        "This quest better end with a full meal.",
        "I thought hunger was just a state of mind... turns out it’s not.",
        "How do undead even deal with hunger? Do they get hungry?",
        "I need sustenance before I start looting corpses for crumbs.",
        "Does anyone have a campfire? I’ll cook anything!",
        "Next person with food, you’re my new best friend.",
        "Why do inns always seem so far away when you’re starving?",
        "If this keeps up, I’m eating the next critter we see.",
        "Anyone know where to buy rations in this forsaken place?",
        "I swear the enemies we fight smell like roasted meat.",
        "Do trolls have secret recipes for jungle feasts? Asking for me.",
        "I’m going to start rationing my thoughts if I don’t eat soon.",
        "Who knew hunger could hit harder than an ogre?",
        "If we’re not eating soon, I’m drinking a health potion out of spite.",
        "Does hunger make me stronger? No? Just checking.",
        "What I’d give for a nice, greasy stew right now.",
        "You can’t adventure on an empty stomach... trust me, I’m trying.",
        "I wonder if fel energy tastes spicy. I might find out.",
        "I’d settle for stale bread at this point.",
        "Why doesn’t Azeroth have snack carts on every corner?",
        "Hunger’s making me nostalgic for my last meal... 12 hours ago.",
        "If we see a feast table, don’t hold me back.",
        "How do ogres stay full? They must eat constantly.",
        "Does anyone carry emergency snacks, or is it just me?",
        "You can’t quest if you’re hangry. It’s a universal truth.",
        "Even the murlocs look edible right now. I need help.",
        "Why does hunger feel like a debuff from a raid boss?",
        "Forget the loot; I’ll take a nice slice of pie."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomWinkResponse()
{
    std::vector<std::string> responses = { 
        "A wink? Is that a quest start or a trap?",
        "If that wink comes with loot, I’m interested.",
        "Was that a wink or do you have something in your eye?",
        "Winking won’t save you from this fight.",
        "Oh, it’s like that? What’s the catch?",
        "Careful with those winks; they’re a dangerous weapon.",
        "You wink at me, and I feel like a marked target.",
        "If this is a ploy, it’s working.",
        "A wink? Is that your way of rolling for initiative?",
        "That wink better come with a good story.",
        "Are we flirting, or is this some kind of spellcasting?",
        "Winking at me won’t lower my guard… much.",
        "What does a wink mean in Draenei culture? Should I run?",
        "One wink and suddenly I’m suspicious.",
        "A wink in the middle of combat? Bold move.",
        "Is this how rogues say hello?",
        "That wink better not be an emote macro.",
        "If you wink at me again, I’ll assume it’s a duel request.",
        "What do you want? Nobody winks without a reason.",
        "Winking? In this economy? Bold.",
        "A wink? Guess you’re trying to sell something.",
        "Is winking part of your faction diplomacy?",
        "Don’t think I didn’t see that wink. What’s the angle?",
        "Winks like that could start a tavern brawl.",
        "If this wink doesn’t involve treasure, I’m out.",
        "Winking won’t get you out of pulling aggro.",
        "You winked at me? Do I look like a trade chat request?",
        "That wink better be for morale-boosting purposes.",
        "You know, winks are a great way to end up in a PvP fight.",
        "I saw that wink. Let’s keep it professional.",
        "Is winking your way of saying you’ve got my back?",
        "Keep winking, and I’ll start charging you gold for it.",
        "You winked? Are we about to engage in shady business?",
        "Winking at me is like pulling a mob—don’t do it lightly.",
        "One more wink and I’m calling the guards.",
        "That wink better not be a sign you’re looting my kills.",
        "Winking won’t distract me… or will it?",
        "Wink once for ‘yes,’ twice for ‘no,’ or stop confusing me.",
        "Is winking a racial trait? Asking for a friend.",
        "Winks are rare in Orgrimmar. What’s the catch?",
        "Was that wink a debuff? I feel oddly charmed.",
        "Winking in a raid? Risky move.",
        "Do you always wink before dropping a group buff?",
        "Wink again, and I’ll assume you’re mocking my DPS.",
        "A wink? You must be very confident or very reckless.",
        "That wink better lead to treasure, or I’m out.",
        "Why do I feel like that wink came with a stealth roll?",
        "A wink? In these troubled times? Bold.",
        "Careful, winking like that could pull boss aggro.",
        "You winked at me? Guess I’m not pulling mobs alone anymore.",
        "Winking? In Azeroth? What are you planning?",
        "Do you wink at everyone, or am I just lucky?",
        "If that wink came with gold, I’d be flattered.",
        "Winking like that in Shadowmoon Valley is dangerous.",
        "A wink? Are you sure you’re not trying to hex me?",
        "Winks don’t work on me. Unless they do. Do they?",
        "Careful, winking at strangers can trigger a duel.",
        "Wink away, but I’m keeping my shield up.",
        "A wink? Bold move for someone standing in AoE.",
        "I’ve seen murlocs wink better than that.",
        "You wink like a rogue about to vanish.",
        "Stop winking! People will think we’re in a guild together.",
        "You winked, and now I’m bracing for a backstab.",
        "A wink like that belongs in a Goldshire tavern.",
        "Winking won’t save you from a wipe.",
        "You wink at me, and suddenly I’m checking my pockets.",
        "A wink in Tanaris usually means trouble.",
        "Is winking your idea of forming a raid party?",
        "You winked, and now I’m questioning my talents.",
        "Winking is a rare skill in Undercity. Use it wisely.",
        "That wink felt like a crit hit to my self-esteem.",
        "Stop winking and start casting if you want my attention.",
        "In Orgrimmar, a wink could start a rebellion.",
        "Winking in Nagrand? You’re braver than I thought.",
        "A wink? You’ve got nerve, I’ll give you that.",
        "Winking won’t get you out of tanking, you know.",
        "Did you just wink at me? Or was that a glitch?",
        "A wink? Is that a new type of crowd control?",
        "In Dalaran, winking is practically a profession.",
        "You wink like someone with a full repair bill.",
        "Winking? Save it for when we’re not fighting Deathwing.",
        "Stop winking! People are starting to notice.",
        "A wink from you feels like a debuff. Strange.",
        "You wink like a troll trying to sell something.",
        "If winking were an ability, you’d be OP.",
        "Winks don’t count as damage mitigation, you know.",
        "That wink better not be the precursor to a gank.",
        "Winking during a boss fight? Impressive priorities.",
        "A wink? Is that your way of taunting me?",
        "Winking is fine, but don’t expect me to wink back.",
        "You wink, and I feel like I should roll for initiative.",
        "I’ll see your wink and raise you a /sigh.",
        "Winking won’t make me forget you stood in fire.",
        "Your wink is as smooth as a goblin sales pitch.",
        "Winking in the Barrens? That’s how wars start.",
        "A wink? Do I look like someone who falls for emotes?",
        "Your wink feels like it came with a repair cost.",
        "Winking at me doesn’t change the loot rules.",
        "Is winking the new guild invite? Because I’m confused.",
        "That wink is sus, and now I’m watching my back."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomDoubtResponse()
{
    std::vector<std::string> responses = { 
        "Doubt me all you want, the loot speaks for itself.",
        "You’re doubting me? That’s rich coming from someone who stands in fire.",
        "Doubt fuels me. Keep it coming.",
        "Doubt me? I’ve soloed bosses while you were AFK.",
        "If doubt could DPS, you’d top the charts.",
        "Doubt me now, thank me later.",
        "Doubting me is like pulling aggro – you’ll regret it.",
        "Your doubt only sharpens my blade.",
        "Doubt me once, shame on you. Doubt me twice, prepare to wipe.",
        "You doubt me, but who saved your butt last fight?",
        "Doubt doesn’t heal raids or slay dragons.",
        "Is that doubt? Or just envy?",
        "Your doubt is noted. My success will be louder.",
        "Doubt? Really? Check my parse before you talk.",
        "If doubt had a cooldown, you’d be silenced by now.",
        "You doubt me? Let’s see you tank the next pull.",
        "Doubt me again, and I might just pull everything.",
        "Doubt in your eyes won’t stop my crits.",
        "Keep doubting. It only makes my gear shine brighter.",
        "Doubt me, but don’t forget who’s carrying this dungeon.",
        "Doubt doesn’t phase me. Your mistakes do.",
        "You doubt me? I’ll let my damage meter speak.",
        "If doubt were a debuff, you’d be stacked to max.",
        "You can doubt me, but the boss is already dead.",
        "Your doubt is as fragile as a hunter in melee.",
        "Doubt all you want, but I’m still in the raid group.",
        "Doubt doesn’t win wars; action does.",
        "If doubt is your strategy, good luck in PVP.",
        "You doubt me, yet I’m the one holding aggro.",
        "Doubt is a weak aura, and you’re showing it.",
        "I eat doubt for breakfast. Thanks for the meal.",
        "Doubt is like a DOT. Watch me cleanse it.",
        "You doubt me, but my guild trusts me. Strange.",
        "Doubting me? Let’s see how you do in my role.",
        "Your doubt is as useful as a grey item.",
        "Doubt me, and I’ll top the meters just to spite you.",
        "I’ve been doubted before, and look where I am now.",
        "Doubt me, but remember who landed the killing blow.",
        "Doubt won’t stop me, but it might stop you from getting loot.",
        "Your doubt is amusing. Watch and learn.",
        "You doubt me, yet I’m still the MVP.",
        "Doubt me, and I’ll out-heal your expectations.",
        "If doubt is your buff, then I must be nerfed.",
        "Doubt me all you want, but you’re still standing in my aura.",
        "Doubt is for the weak; I prefer action.",
        "Your doubt is like a crit miss – ineffective.",
        "Doubt me, and I’ll show you why I’m on the roster.",
        "Keep doubting. I’ve survived worse.",
        "Doubt doesn’t suit you. Confidence does.",
        "You doubt me, but I’m still leading this charge.",
        "If doubt is your strategy, it’s a poor one.",
        "Doubt me if you wish, but the spirits guide my hand.",
        "Your lack of faith amuses the elements I command.",
        "The ancestors have seen me triumph; your doubt means little.",
        "By Elune, I’ll prove your doubt unfounded.",
        "Doubt me now, and you’ll miss the glory of my victory.",
        "Do you question my strength, or the wisdom of the Earthmother?",
        "You doubt my blade? It has tasted the blood of countless foes.",
        "The winds of Azeroth carry whispers of my deeds; your doubt cannot silence them.",
        "Let your doubt be the forge that tempers my resolve.",
        "Doubt is the enemy’s ally; do not wield it here.",
        "The Light does not falter, even if your faith in me does.",
        "Doubt is a shadow, but the Sunwell guides me still.",
        "You doubt me? Then step aside and watch as I prove you wrong.",
        "The spirits laugh at your doubt, for they know my destiny.",
        "By the Flame of Azeroth, I’ll burn away your doubts.",
        "If you doubt me, then follow and see the truth unfold.",
        "The Legion doubted us once too, and look where that got them.",
        "Let me show you why the Horde never doubts its champions.",
        "Even the Void cannot cast a shadow on my purpose.",
        "Doubt may cloud your vision, but my path remains clear.",
        "Do you hear the echoes of past battles? They sing of my triumphs.",
        "Doubt me, and you doubt the will of Azeroth herself.",
        "The Naaru have faith in me; why do you not?",
        "Your doubt feeds the fire in my heart. Watch it burn bright.",
        "Doubt is a tool of the Old Gods. Don’t wield it against me.",
        "The spirits of my ancestors walk beside me; your doubt cannot stand against them.",
        "The Light will shine through your doubt, as it always does.",
        "Your doubt is like a gnome’s blade – short and insignificant.",
        "Doubt me if you must, but my bow never misses its mark.",
        "The tides of battle favor those who believe; doubt only drags you under.",
        "Doubt has no place on this battlefield; courage does.",
        "Let the sands of Silithus remind you – doubt leads to ruin.",
        "Do you doubt my strength or your own lack of courage?",
        "Doubt is as fleeting as the mists of Pandaria; my resolve is eternal.",
        "Even Deathwing doubted our strength, and see how he fell.",
        "Your doubt whispers like the Lich King’s voice, and I ignore both.",
        "Doubt is a luxury we cannot afford. Follow me, or step aside.",
        "By Grom’s axe, your doubt dishonors us both.",
        "Doubt me, and you doubt the Horde’s strength.",
        "The Light judges me worthy; your doubt cannot overrule it.",
        "Your doubt will be my stepping stone to glory.",
        "The cries of the battlefield drown your doubt in their truth.",
        "Doubt may shroud the future, but my resolve cuts through it.",
        "Do you doubt me, or the will of Elune herself?",
        "The ancestors whisper to me; their faith is stronger than your doubt.",
        "Doubt fades before the blade of an unwavering champion.",
        "Doubt if you must, but do so quietly while I win this fight.",
        "Your doubt is a spark; I’ll turn it into an inferno of victory.",
        "The battlefield will decide, not your wavering faith.",
        "Let me show you why Azeroth herself has never doubted me."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomPanicResponse()
{
    std::vector<std::string> responses = { 
        "Calm down! Panicking won’t help anyone!",
        "Take a deep breath, we’ll figure this out together.",
        "Oh no, oh no, oh no! What do we do?!",
        "Stay calm! The ancestors are with us!",
        "If you keep screaming, they’ll hear us! Shhh!",
        "Breathe, just breathe. We’ll get through this.",
        "It’s fine! Everything’s fine! Probably!",
        "AHHH! Okay, no, I’m fine. Totally fine.",
        "Okay, maybe I’m panicking a little too...",
        "Stop panicking! You’re making me panic!",
        "It’s not the end of Azeroth… yet!",
        "Oh Light, oh Light! What do we do now?!",
        "Everyone stay calm! Or at least try!",
        "By Elune, keep it together! We need a plan!",
        "PANIC! Wait, no, don’t panic! Oh, fel!",
        "Deep breaths, friend. Let’s focus.",
        "This is NOT how I wanted today to go!",
        "If I see another murloc, I swear I’m running!",
        "Someone grab a healer! We’re falling apart here!",
        "I can’t think when you’re panicking like that!",
        "Let’s all take a moment to NOT PANIC!",
        "Ahhhh! Why is this happening?!",
        "Stop flailing and start thinking!",
        "I knew this would go wrong! I KNEW IT!",
        "Okay, everyone freak out for 10 seconds, then stop!",
        "No time to panic! We fight or we die!",
        "I’m panicking! Are you panicking? I’m panicking!",
        "The spirits guide us… I hope.",
        "Okay, new rule: no screaming unless it’s life-threatening!",
        "HELP! I mean… let’s stay calm. Help!",
        "You’re panicking about the wrong thing!",
        "Oh no, oh no, oh no, oh no!",
        "Someone, anyone, say something calming! Please!",
        "I can’t do this! I CAN’T DO THIS!",
        "Elune help us, we’re doomed!",
        "If you keep running in circles, we’ll all die!",
        "I didn’t sign up for this level of chaos!",
        "Don’t panic. Don’t panic. Oh no, I’m panicking.",
        "We’re all going to die, aren’t we?",
        "Stay calm… or fake it! Faking it works too!",
        "By the Light, pull yourself together!",
        "I can’t even hear myself panic over your screaming!",
        "This is worse than when we pulled the boss early!",
        "Why are YOU panicking? I’M the one in danger!",
        "If this is your version of calm, I’m terrified!",
        "Okay, deep breath. In… out… Okay, NOW PANIC!",
        "Why does this always happen to US?!",
        "Do we fight or run? Decide while I panic!",
        "If we die, I’m blaming YOU for panicking first!",
        "Hold the line! Panicking won’t save us!",
        "The spirits are restless, and so am I!",
        "By the Light, stay calm, or we’re all doomed!",
        "Elune guide us! This is chaos!",
        "Get it together, soldier! Azeroth depends on us!",
        "If you falter now, you disgrace your ancestors!",
        "The Horde never panics! …Except maybe right now.",
        "Breathe deep and stand tall. We are not cowards!",
        "Even the Legion didn’t scare me this much… okay, maybe a little!",
        "For the love of the Earthmother, STOP flailing!",
        "This is no time for fear; the battlefield demands courage!",
        "Do you think Thrall panics? No! Be like Thrall!",
        "One more squeal like that, and you’ll summon a banshee!",
        "I didn’t leave the Exodar for THIS level of chaos!",
        "By the Sunwell, we’re better than this!",
        "You’ll shame the Kirin Tor with that kind of panic!",
        "The ancestors are watching, and they’re shaking their heads!",
        "If this is our last stand, let’s at least make it dignified!",
        "Hold your ground, or I’ll tell everyone you screamed!",
        "The Naaru didn’t guide us this far for us to crumble now!",
        "A hero of the Alliance does not panic… usually.",
        "Pull yourself together! There’s no escape in cowardice!",
        "By the Twilight Grove, this is madness!",
        "You call yourself a champion? Fight, don’t cower!",
        "Even goblins wouldn’t panic this much over a bad deal!",
        "I’ve faced dragons with less screaming than this!",
        "Do you think the Scourge panicked when we showed up? No, they laughed!",
        "Great, now the enemy knows where we are, thanks to your shrieking!",
        "The battlefield demands bravery, not hysteria!",
        "For Gnomeregan’s sake, stop panicking and THINK!",
        "The sands of Tanaris aren’t as shifting as your nerves!",
        "Do you think Sylvanas panics? She schemes! Be like her… maybe.",
        "By the Windrunner Spire, hold your composure!",
        "Don’t let fear grip you! Let it fuel your blade!",
        "The drums of war don’t stop for cowards!",
        "Even the murlocs would laugh at this display!",
        "We survived Deathwing, we can survive THIS!",
        "If Gul’dan didn’t panic, neither should you!",
        "I didn’t abandon Silvermoon for this level of incompetence!",
        "The honor of the Tauren compels you to stand firm!",
        "The ancestors are crying at our display right now!",
        "No troll worth their tusks would act like this!",
        "Do you want to end up a Forsaken? Then stand and fight!",
        "The Emerald Dream didn’t prepare me for THIS level of madness!",
        "Not even Illidan would condone this panic!",
        "You’re shaming your tabard with this display!",
        "If Varian saw us now, he’d exile the lot of us!",
        "Steel yourself, or I’ll do it for you!",
        "The Light doesn’t falter, and neither should we!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomPointResponse()
{
    std::vector<std::string> responses = { 
        "Why are you pointing at me? Did I do something?",
        "Is there something on my face? Why the point?",
        "What are you pointing at? Speak up already!",
        "If you're pointing at me, it better be good.",
        "Don’t just point—explain yourself!",
        "Why are you pointing? Is there a spider on me?",
        "Are you pointing at me, or the mess behind me?",
        "Do I have something on my armor? Stop pointing!",
        "What, me? I didn’t do it!",
        "Don’t point at me like that! It’s rude.",
        "That pointing better come with an explanation.",
        "If you’re pointing to the danger, be clearer!",
        "You point at me, I point right back!",
        "Are you pointing at glory, or at doom?",
        "Pointing won’t solve the problem—action will!",
        "Is there something I should know? That point looks serious.",
        "The last time someone pointed at me, it didn’t end well for them.",
        "Pointing? Really? Just say it outright!",
        "You point like a mage casting Frostbolt. What’s going on?",
        "Why the finger? Just say what’s on your mind.",
        "If you’re pointing at treasure, I’m interested.",
        "Pointing into the distance? Is it loot or trouble?",
        "Is there a dragon where you’re pointing, or am I just paranoid?",
        "Pointing at the horizon? What, the end of the world again?",
        "If it’s worth pointing at, it’s worth explaining.",
        "Point first, talk later? That’s not how it works.",
        "Is that a threat or a warning? Stop pointing!",
        "If you’re pointing at the enemy, let’s charge!",
        "You keep pointing like that, and I’ll start worrying.",
        "I don’t like being singled out with a point. What’s the deal?",
        "Pointing behind me? Is it a giant spider?",
        "Pointing at my boots? What’s wrong with them?",
        "If you’re pointing at something shiny, I’m listening.",
        "Don’t just point—wave or something!",
        "The last time someone pointed like that, we ended up in a dungeon.",
        "Stop pointing unless you’ve got a really good reason.",
        "What’s with the dramatic point? Are we under attack?",
        "Is that a point of blame or a point of direction?",
        "Your pointing hand looks serious. Should I be worried?",
        "Pointing at me won’t make the problem go away.",
        "Is it treasure or trouble where you’re pointing?",
        "That point better not be accusing me of something!",
        "Pointing at me? I’ve done nothing wrong… today.",
        "What’s so important that you’re pointing at it?",
        "Do you always point when you see something suspicious?",
        "Pointing doesn’t help unless you give some details.",
        "The finger of fate has spoken… or is it just you?",
        "Are you pointing at my new gear? Jealous much?",
        "Pointing over the hill? Are we about to get ambushed?",
        "You’ve been pointing for five minutes. What’s the deal?",
        "Stop pointing and just tell me what’s wrong!",
        "You pointing at me won’t solve the problem.",
        "Do I look like I know what you’re pointing at?",
        "Pointing again? Is this going to be a thing?",
        "You pointing at me feels oddly accusatory.",
        "Is it something behind me? Stop scaring me!",
        "You point like I’m supposed to understand. I don’t.",
        "Pointing? Is that some new strategy I don’t know about?",
        "Pointing at the map? Great. Now explain it.",
        "I hope you’re pointing at treasure, or I’m ignoring you.",
        "Are you pointing at that bird? What’s so special?",
        "Pointing at me? That’s a first.",
        "Your finger’s going to cramp if you keep pointing like that.",
        "Pointing into the woods? Are we being stalked?",
        "I see you pointing, but I don’t see what’s so important.",
        "That’s some dramatic pointing. What’s going on?",
        "If you point any harder, your hand might fall off.",
        "Pointing at my sword? It’s sharp, I know.",
        "Why is your pointing so intense? Relax!",
        "What’s with the silent point? Use your words!",
        "If you’re pointing at danger, I’m out of here.",
        "Is that pointing or a new kind of spellcasting?",
        "I swear, if you’re pointing at a spider, I’m running.",
        "Pointing isn’t a language I understand. Try harder.",
        "Pointing at my loot? Don’t even think about it.",
        "You keep pointing like that, and we’ll have a problem.",
        "Are you pointing for a reason, or is it a habit?",
        "Pointing at the sky? Is it raining dragons again?",
        "Stop pointing at me like I stole something!",
        "You point like a general, but you’re no leader.",
        "Is this your way of blaming me? Not cool.",
        "That’s a lot of pointing for something so small.",
        "Your pointing isn’t exactly helping the situation.",
        "Pointing won’t fix it. Try shouting next time.",
        "If you’re pointing at trouble, I’m not interested.",
        "Pointing at that cave? No way am I going in there.",
        "Are you pointing because you’re too scared to speak?",
        "Pointing at the ground? Is there a trap?",
        "Pointing like that makes you look guilty, not me.",
        "Is pointing a custom from your homeland?",
        "You keep pointing at me like I’m the enemy.",
        "Is there a prize for the most dramatic pointing?",
        "Pointing at the horizon? Let me guess: more trouble.",
        "You pointing at me doesn’t make me feel special.",
        "Is pointing your idea of giving directions?",
        "Pointing and glaring? Now I’m really confused.",
        "Pointing at my helm? It’s a nice one, right?",
        "Pointing at me for what? I just got here!",
        "Pointing at the enemy? Alright, let’s do this!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomRaspResponse()
{
    std::vector<std::string> responses = { 
        "You’d be more useful as a training dummy.",
        "Did you forget to bring your brain to this fight?",
        "You’re about as sharp as a blunt sword.",
        "If common sense were loot, you’d be broke.",
        "You’d trip over your own shadow if it stood still.",
        "I’ve seen murlocs with better strategies than you.",
        "You’re living proof that some quests are too hard.",
        "If I wanted dead weight, I’d have stayed home.",
        "You’re about as intimidating as a bunny in Spring Festival.",
        "Do you even know which end of the sword to hold?",
        "If stupidity were a skill, you’d be max level.",
        "Your battle plan is as solid as a sandcastle.",
        "You couldn’t lead a gnome to an engineering convention.",
        "If whining were an achievement, you’d have a world first.",
        "You’re slower than a turtle on a laggy server.",
        "Even kobolds would laugh at your lack of skills.",
        "I’ve seen better teamwork from pug groups.",
        "You’re a walking debuff to morale.",
        "You’d lose a duel to an unarmed critter.",
        "Are you here to help, or just to be a joke?",
        "Your insults are weaker than your gear score.",
        "If bad decisions were currency, you’d be rich.",
        "You’re about as reliable as a goblin-made gadget.",
        "I’ve met raid bosses with better conversational skills.",
        "You’re like a gray item—completely useless.",
        "Even your pet looks embarrassed to be seen with you.",
        "You fight like you’re trying not to win.",
        "You’d make a great NPC—completely scripted and predictable.",
        "Your logic is as broken as a low-level quest chain.",
        "I’ve seen crits bigger than your personality.",
        "You’re a loot ninja without the loot.",
        "Do you always talk this much, or is it just to annoy me?",
        "If brain cells were mobs, you’d be a deserted zone.",
        "You’re as welcome here as a disconnect during a boss pull.",
        "I’ve seen farm animals with better combat instincts.",
        "You’d get lost on a straight road.",
        "Your contribution to this fight is as valuable as vendor trash.",
        "You’re the reason people leave group finder.",
        "Your presence is the real debuff here.",
        "You’re about as useful as an underwater campfire.",
        "You’re proof that not everyone can handle the grind.",
        "Even a fishing bobber has more depth than you.",
        "You’re a walking wipe mechanic.",
        "I’d say you’re out of your depth, but shallow water isn’t deep.",
        "Your DPS is so low, I thought you were healing.",
        "You’re like a flagged PvPer: always asking for trouble.",
        "You have the situational awareness of a rock.",
        "Your strategy is like your loot rolls: consistently bad.",
        "You’re the reason raid leaders drink.",
        "You fight worse than Arthas’s conscience!",
        "Did Sylvanas teach you strategy? Because this is chaos.",
        "You’re about as trustworthy as Kael’thas with Sunwell access.",
        "Your moves are as outdated as the Exodar’s navigation system.",
        "I’ve seen less betrayal in the Defias Brotherhood.",
        "You remind me of Onyxia—full of fire, but no bite.",
        "Was that swing inspired by Garrosh’s diplomacy?",
        "You strategize like a Twilight Cultist: blindly and badly.",
        "Your armor has more holes than Deathwing’s old lair.",
        "Even the Scarlet Crusade would laugh at your tactics.",
        "You’re about as effective as Thrall was in Cataclysm.",
        "Are you trying to rival Medivh for cryptic nonsense?",
        "Your skill is as rare as a true neutral faction.",
        "You dodge worse than an Argent Crusader at a Scourge invasion.",
        "Do you practice losing, or does it come naturally?",
        "Your blade strikes with all the impact of a fluffy murloc.",
        "Even Varian would facepalm at your performance.",
        "You’ve got the subtlety of a goblin demolitions expert.",
        "You’re about as useful as Darnassus after Teldrassil burned.",
        "You swing like you’re afraid of hurting the enemy!",
        "You’d fit in better with the Burning Legion—utter destruction with no purpose.",
        "Even a Forsaken would pity your missteps.",
        "Your logic is shakier than Gallywix’s loyalty to the Horde.",
        "You’d lose a debate to a Grummle with a full pack.",
        "You strategize like a gnome trying to reach a high shelf.",
        "Even the Bronze Dragonflight couldn’t fix your mistakes.",
        "You’ve got the aim of a drunken ogre.",
        "Your teamwork is as solid as a Saronite wall.",
        "Was that spell cast or just a hiccup from the Light?",
        "You’ve got all the charisma of a void-touched artifact.",
        "You’re a bigger disaster than Illidan’s blind charge into Outland.",
        "You wield that weapon like a pacifist.",
        "Even Bolvar would drop his hammer at your performance.",
        "You’re about as precise as a troll’s first engineering project.",
        "Your presence weakens the group like a Curse of Weakness.",
        "You remind me of the Black Empire—chaos and no control.",
        "Your attacks are as slow as Anduin deciding what to say.",
        "Even Magni’s heart doesn’t resonate with your nonsense.",
        "Your resilience is weaker than Arthas’s resolve at Stratholme.",
        "The Horde council has better synergy than you do.",
        "You dodge worse than a rogue caught in a Blizzard.",
        "You swing like you’re afraid the mob might hit back.",
        "You’d make a better Innkeeper than an adventurer.",
        "You’ve got the survival instincts of a critter in a raid.",
        "Was that a feint or an accidental faceplant?",
        "Your combat skill belongs in an Azeroth blooper reel.",
        "Do you train with kobolds? Because they’re laughing somewhere.",
        "Even Illidan would say you’re unprepared for this.",
        "You’re as reliable as a portal to Dalaran: always misplaced.",
        "Your fearlessness rivals a gnome entering a Tauren tavern."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomThankResponse()
{
    std::vector<std::string> responses = { 
        "You’re welcome! It’s always a pleasure to help a friend.",
        "Anything for the team. Glad I could be of assistance!",
        "No need to thank me, we’re in this together.",
        "Of course! Happy to lend a hand.",
        "Helping you is its own reward. Let’s keep going!",
        "I couldn’t let you down, could I?",
        "Your gratitude means a lot. Let’s keep the good vibes going!",
        "I’d do it again in a heartbeat. Let’s move on!",
        "You’re welcome! I knew you’d appreciate that.",
        "Anytime, truly. Let’s conquer the next challenge together.",
        "You’ve got my back, and I’ve got yours.",
        "Thank you for trusting me to help!",
        "Glad to see my efforts didn’t go unnoticed.",
        "Your appreciation makes it all worthwhile.",
        "It’s what allies do for each other!",
        "I’m just glad it worked out. Onward!",
        "Helping you is always time well spent.",
        "Hearing your thanks makes the effort feel even more rewarding.",
        "Anything for the group! Let’s keep this momentum up.",
        "It’s always a pleasure to help a friend in need.",
        "Your success is thanks enough for me.",
        "It was nothing, really. You’d do the same for me!",
        "We’re stronger together, so it’s the least I could do.",
        "Grateful to have the chance to make a difference.",
        "No need for thanks. It’s what companions do.",
        "Hearing you say that makes me feel appreciated.",
        "You make it easy to want to help.",
        "It’s mutual—we’re all here for each other.",
        "Don’t mention it! Let’s keep pushing forward.",
        "Happy to know I could make things easier for you.",
        "You inspire me to keep giving my all.",
        "It’s all part of the adventure. Let’s keep going!",
        "Your words mean more than you know. Let’s keep it up!",
        "Seeing you thrive is thanks enough.",
        "We’re a team, and I wouldn’t have it any other way.",
        "Your gratitude is the best reward I could ask for.",
        "I’d help you a thousand times over.",
        "You’re welcome! Let’s continue to face these challenges head-on.",
        "Your appreciation fuels me. Thank you in return!",
        "Helping you is my pleasure, truly.",
        "It’s great to see my efforts helped you.",
        "We rise together. That’s what makes this journey worthwhile.",
        "Your thanks mean more than you know.",
        "I’m here for you, and I’m glad you feel the same.",
        "Knowing I made a difference for you makes my day.",
        "We’re all in this together, and I’m glad I could help.",
        "No thanks needed, but it’s appreciated all the same.",
        "Your success is my success. Let’s keep going strong.",
        "Your words are a treasure. I’ll cherish them.",
        "Always glad to be of service. Let’s keep at it!",
        "You’re more than welcome. Let’s make more victories together.",
        "It was my honor to help you.",
        "Always here for you, no need to thank me.",
        "Helping you out is its own reward.",
        "You’ve done so much for me; it’s the least I could do.",
        "Glad I could make a difference. Let’s keep moving!",
        "Your gratitude means the world to me.",
        "Together, we’re unstoppable. I’m happy to help.",
        "No need for thanks; we’re a team through and through.",
        "Anything to see you succeed.",
        "Your words are appreciated, but let’s focus on the next step.",
        "It’s my pleasure to lend a hand.",
        "You’re welcome! Let’s make the next challenge even easier.",
        "I’m glad to see my efforts paid off.",
        "Helping you is always worth it.",
        "Your appreciation makes this all worthwhile.",
        "You’re welcome! I’d do it all over again.",
        "Knowing I helped is all the thanks I need.",
        "I’m just glad to have you on my side.",
        "Your gratitude fuels my resolve.",
        "It’s no trouble at all; I’ve got your back.",
        "Hearing you say that makes every effort worth it.",
        "We’re in this together, and I wouldn’t have it any other way.",
        "No thanks necessary, but I’ll take it anyway!",
        "Seeing you smile is all the thanks I need.",
        "Your trust in me is the greatest compliment.",
        "We rise together; that’s what allies do.",
        "Your words inspire me to keep giving my best.",
        "Thank you for acknowledging my effort.",
        "I wouldn’t be here without you, so thank you too.",
        "We’re a team, and I’m proud to stand with you.",
        "Your success is my success. Let’s keep thriving!",
        "I’m just glad you’re happy with the outcome.",
        "Let’s keep this energy going and tackle what’s next.",
        "Helping you was my privilege.",
        "You’re welcome! Let’s make the most of our journey.",
        "Anything for a companion like you.",
        "It’s what friends do; no need for thanks.",
        "Your kind words mean more than you know.",
        "Knowing I made your day better makes mine brighter.",
        "We’ve got this together, always.",
        "No thanks needed, but I appreciate it all the same.",
        "It’s a joy to see you thriving.",
        "I’m here for you, now and always.",
        "I’d help you a hundred times over.",
        "You make it easy to want to help.",
        "You’re welcome! Let’s aim even higher next time.",
        "Every effort is worth it for a friend like you.",
        "Helping you is always time well spent.",
        "Let’s keep working together to achieve great things.",
        "Your gratitude keeps me motivated."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomToastResponse()
{
    std::vector<std::string> responses = { 
        "Here’s to you! You’ve earned every bit of this celebration!",
        "Cheers, my friend! To victories big and small!",
        "A toast to your bravery and resilience!",
        "To your incredible achievements! May there be many more!",
        "Here’s to the strongest ally anyone could ask for!",
        "Raise your glass! This moment is yours!",
        "A toast to friendship, and the battles we’ve conquered together!",
        "To you, the heart and soul of this journey!",
        "Here’s to us, making history one step at a time!",
        "To your wisdom and strength, unmatched in every way!",
        "A cheer for you, the unsung hero of our story!",
        "To victories well-earned and memories well-made!",
        "Raise your glass high! The night belongs to you!",
        "Here’s to your courage that lights the way for us all!",
        "To your boundless determination and spirit!",
        "A toast to the bonds we share and the paths we’ve walked!",
        "Here’s to you, the one who never backs down!",
        "To every challenge you’ve faced and conquered with grace!",
        "Raise a mug! To the greatest adventurer I know!",
        "A cheer for your unwavering heart and fierce soul!",
        "Here’s to every step you’ve taken toward greatness!",
        "To you, a beacon of hope and strength!",
        "Raise your glass, for this moment is worth celebrating!",
        "A toast to your resilience and unyielding will!",
        "To every triumph that has brought us here together!",
        "Here’s to your spirit, as indomitable as the mountain!",
        "To you, a steadfast companion in every storm!",
        "Cheers to the stories we’ll tell of today!",
        "Raise your drink! To the memories we’re forging right now!",
        "To your victory, hard-fought and well-deserved!",
        "Here’s to you, whose presence makes every challenge worthwhile!",
        "A toast to your laughter, which brightens every moment!",
        "To the adventures we’ve shared and those still to come!",
        "Raise a cup to your loyalty, a rare and priceless gift!",
        "Here’s to the one who inspires us all to keep fighting!",
        "To every dream you’ve chased and caught!",
        "Cheers to the friend who makes the journey brighter!",
        "A toast to your fierce spirit and unstoppable drive!",
        "To the path you carve with courage and skill!",
        "Raise your glass to the victories you claim with honor!",
        "Here’s to the strength that never falters and the heart that never breaks!",
        "To the stories we’ll share of today’s triumphs!",
        "A cheer for the battles we’ve won and the ones we’ll face together!",
        "To your steadfastness, which anchors us all!",
        "Raise your drink to the light you bring to our group!",
        "Here’s to the battles you’ve led us through with grace!",
        "A toast to your heart, as fierce as any warrior’s!",
        "To your strength, your courage, your fire!",
        "Cheers to the victories we’ll celebrate long after this day!",
        "Here’s to you, a friend and a hero!",
        "To your unwavering resolve, a true inspiration to us all!",
        "Raise a toast to the victories we could never achieve without you!",
        "Cheers to your bravery, shining brighter than a thousand stars!",
        "Here’s to you, the lifeline of this party!",
        "A toast to your cleverness; you always find a way through!",
        "To your courage, unmatched and unwavering!",
        "Let’s drink to the one who turns the tide of every battle!",
        "Here’s to the friend who never lets us down!",
        "To your strength and valor, a beacon for us all!",
        "Raise your glass to the one who leads us to glory!",
        "Cheers to the heart of our team!",
        "A toast to the memories we make together, unforgettable and priceless!",
        "Here’s to your spirit, ever unyielding and proud!",
        "To the victories you bring, let’s celebrate every single one!",
        "Raise a drink to the companion who makes this journey worth it!",
        "To the battles fought and won with your steady hand by our side!",
        "Here’s to the stories we’ll tell of your daring deeds!",
        "Cheers to the steadfast friend who’s always there!",
        "A toast to the moments we’ll treasure forever!",
        "Raise a mug to your wisdom, sharp as a blade!",
        "To the one who gives us hope when times are grim!",
        "Here’s to your light, guiding us through the darkness!",
        "To your unyielding courage, a force to be reckoned with!",
        "Raise a toast to the hero we all look up to!",
        "Cheers to your laughter, the best kind of battle cry!",
        "To the victories ahead, led by your skill and strength!",
        "Here’s to the friend who never leaves a comrade behind!",
        "To your bravery, always charging headfirst into danger!",
        "Raise your glass to the memories we’re forging today!",
        "A cheer for your loyalty, steadfast through every storm!",
        "To the stories of your heroics, they’ll sing them for ages!",
        "Cheers to the victories only you could lead us to!",
        "Here’s to your unshakeable confidence, lifting us all up!",
        "To your heart, full of kindness and courage!",
        "Raise a drink to the victories we’ve celebrated together!",
        "A toast to your boldness, always a step ahead!",
        "Cheers to your strength, the backbone of our group!",
        "Here’s to the times we’ve shared, and the memories we’ll create!",
        "To the moments we’ve overcome together, stronger for it!",
        "Raise your glass to the one who’s always ready for adventure!",
        "Cheers to your passion, fueling us all to keep going!",
        "A toast to the resilience you show in every battle!",
        "Here’s to the friend who keeps us laughing through the chaos!",
        "To the one who turns every setback into an opportunity!",
        "Raise a drink to the memories that make it all worthwhile!",
        "Cheers to your vision, always steering us toward success!",
        "To the victories we’ve earned together, and those yet to come!",
        "Here’s to your determination, unwavering and unstoppable!",
        "A toast to the stories we’ll tell, with you at their heart!",
        "To the one who turns every challenge into a victory!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomThirstyResponse()
{
    std::vector<std::string> responses = { 
        "Thirsty, huh? Don’t worry, we’ll find you something soon.",
        "You’re thirsty? I think there’s a stream up ahead.",
        "Didn’t you bring any water? Adventuring 101!",
        "Hang tight, I’ll conjure something if we don’t find a tavern.",
        "Thirsty already? We haven’t even made it halfway.",
        "Don’t worry, we’ll hit the next inn for a drink.",
        "Feeling parched? I’m sure there’s a well nearby.",
        "No wonder you’re thirsty, you’ve been running around like crazy.",
        "I told you to bring a flask, but nooo...",
        "Thirsty? We just left the tavern! You sure you’re not part fish?",
        "You’re thirsty? I think I saw a cactus we can tap into.",
        "If you’re that thirsty, we might need to pause for a drink.",
        "Next time, pack some water. Or at least a mage.",
        "Here, take the rest of mine. I’ll survive for now.",
        "Thirsty? That’s what you get for skipping breakfast.",
        "Let’s find you a drink before you pass out on us.",
        "I’d give you some of my water, but… it’s empty too.",
        "Thirsty? I swear, you have the hydration of a desert critter.",
        "Hold tight, I think there’s an oasis around here somewhere.",
        "Thirsty? Don’t worry, we’re not leaving you to dry up.",
        "You’re thirsty? Guess it’s time for a tavern break.",
        "You’ll live, but yeah, let’s get you a drink.",
        "If thirst were an enemy, it’d be your greatest foe.",
        "Thirsty, huh? Didn’t think adventurers needed hydration.",
        "I told you that ale wouldn’t last the whole journey.",
        "Water’s probably closer than you think. Let’s keep moving.",
        "Thirsty? We’ll find a solution, just hang in there.",
        "If you’re thirsty, try not to think about rivers or rain.",
        "You’re thirsty? At least we’re not in the Barrens right now.",
        "Sounds like you could use a good, cold ale.",
        "Thirsty? Let’s hope there’s a vendor around the corner.",
        "Hang on, thirsty one. We’ll find a drink soon.",
        "I can’t promise a feast, but we’ll get you a drink.",
        "I thought you were stronger than thirst, but here we are.",
        "Let’s make haste before your thirst turns into complaints.",
        "Maybe that pond isn’t as stagnant as it looks. Worth a shot?",
        "Don’t worry, we’ll get you hydrated. Eventually.",
        "Thirsty? I’d share mine if I wasn’t hoarding it for emergencies.",
        "You’re thirsty? Try thinking less about water; it helps.",
        "Let’s find a drink before you collapse on the battlefield.",
        "Thirsty, huh? Let’s see what the innkeeper’s got.",
        "You’re always thirsty! Did you forget your canteen again?",
        "Alright, alright, let’s stop and sort out your thirst.",
        "Hang tight, thirsty warrior. Relief is on the horizon.",
        "Being thirsty is a sign of weakness, but I’ll help you anyway.",
        "You’ve got the stamina of a sloth if you’re thirsty already.",
        "Thirsty? Maybe you should’ve trained in endurance, not strength.",
        "Fine, I’ll get you a drink, but you owe me.",
        "Let’s move before you start drinking from the wrong potions.",
        "Alright, alright, let’s find you some water before you start licking the dirt.",
        "Thirsty again? You’re like a never-ending drain on resources.",
        "Drink up when we find a well. You’re slowing us down.",
        "Thirst is temporary; glory is eternal. Keep moving!",
        "You’re thirsty? Imagine how the fish feel.",
        "Hold it together. The next stream is just over that hill.",
        "Thirsty? That’s what happens when you drink nothing but ale.",
        "Alright, hydrate or die-drate. Let’s find a drink.",
        "You’re thirsty? Imagine being a fire elemental.",
        "I bet the taverns in Booty Bay have exactly what you need.",
        "I told you to refill your flask at the last stop!",
        "Drink first, complain later. That’s the adventurer’s way.",
        "Thirsty? We’ll see if that pond isn’t full of murloc spit.",
        "You’re thirsty? I thought paladins radiated self-sufficiency.",
        "If thirst were a boss, you’d have wiped twice already.",
        "We’ll quench your thirst, just don’t start crying about it.",
        "Don’t worry, we’ll find a drink before you faint in Stormwind Square.",
        "You’re thirsty? Maybe you should’ve gone priest for those self-heals.",
        "Just a bit longer and we’ll hit the next inn.",
        "You know what cures thirst? Victory. Let’s keep going.",
        "I hear the Moonwell water is refreshing. Too bad it’s sacred.",
        "We’ll find a drink soon. Just don’t pass out on me.",
        "Thirsty, huh? That’s why I pack potions for everything.",
        "You’re not thirsty; you’re just dramatic. Keep moving.",
        "If thirst is your biggest problem, you’re doing fine.",
        "There’s a water vendor somewhere around here. Let’s find them.",
        "Thirsty? I could conjure you some… if I were a mage.",
        "Don’t make me carry you because of your so-called thirst.",
        "We’ll find you a drink. It might be swamp water, though.",
        "Being thirsty isn’t an excuse to stop. Keep going.",
        "Fine, we’ll stop for a drink. But only because you’re whining.",
        "You’re thirsty? Just chew some ice. Oh wait, we don’t have any.",
        "Let’s find a tavern before you start stealing my flask.",
        "Next time, pack your own water. I’m not your servant.",
        "Thirsty? You should’ve gone druid. They never complain.",
        "I’ll trade you a drink for silence. Deal?",
        "Don’t worry, we’ll find something to drink that won’t poison you.",
        "You’re thirsty? I’m about to start charging you for my water.",
        "One drink and you’ll be back to whining about something else.",
        "Stop thinking about rivers and rain. It’ll just make it worse.",
        "We’ll hydrate once we’re out of combat. Focus, please.",
        "Thirsty already? At this rate, you’ll need a camel mount.",
        "We’ll quench your thirst, but first, we survive this fight.",
        "You’re thirsty? Imagine how I feel carrying this group.",
        "Let’s hope the next vendor doesn’t charge a fortune for water.",
        "Thirsty? Sounds like a you problem. Let’s move.",
        "We’re on a mission, not a pub crawl. Deal with it.",
        "Alright, let’s hydrate. Just don’t make a big deal out of it.",
        "You’re thirsty? Maybe all that talking wore you out.",
        "Fine, we’ll stop at the next fountain. But no whining."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomPatResponse()
{
    std::vector<std::string> responses = { 
        "Thanks for the pat! I needed that.",
        "Appreciate the support; it means a lot.",
        "A pat on the back? You’re too kind!",
        "Ah, a pat! That’s the morale boost I needed.",
        "Thanks for the pat! It feels like a victory already.",
        "Your pat has magical encouragement powers.",
        "That pat made my day better. Let’s go!",
        "I feel like a true hero now, thanks for the pat.",
        "That’s the kind of motivation that keeps me going.",
        "With pats like these, how could we lose?",
        "Thanks! I’ll carry that pat straight to victory!",
        "Ah, your pat is the stuff of legends.",
        "Feels good to be appreciated. Let’s win this!",
        "Your pat is worth a thousand words of encouragement.",
        "Thanks for believing in me; your pat says it all.",
        "I’ll do my best to live up to that pat!",
        "Your pat is as inspiring as a battle cry!",
        "Thanks! That pat just added +10 to morale.",
        "You’ve got a way with pats; I feel unstoppable.",
        "A pat like that deserves a victory in return!",
        "Nothing says teamwork like a pat on the back.",
        "Your encouragement is as powerful as any spell.",
        "A little pat goes a long way. Thanks!",
        "Your pat makes even the hardest battles worth it.",
        "Feeling like a champion now. Thanks for the pat!",
        "Your support lifts my spirits. Let’s do this!",
        "With pats like yours, there’s nothing we can’t achieve.",
        "You’re a true ally. Thanks for the encouragement!",
        "A pat and a smile? You spoil me!",
        "Thanks! Your pat feels like a blessing from Elune.",
        "Your support is like a warm hearth in the cold.",
        "With friends like you, victory feels inevitable.",
        "That pat just reminded me why we fight together.",
        "Your pat is the spark that ignites my resolve.",
        "Thank you for the encouragement! Let’s make it count.",
        "Your pat is like a gift from the Light itself.",
        "You’re the best cheerleader anyone could ask for.",
        "I’ll carry that pat with me into battle. Thanks!",
        "Your pat is the real buff I needed today.",
        "Feeling stronger already. Thanks for the boost!",
        "With pats like these, I could conquer anything.",
        "You’re my favorite teammate. Thanks for the support!",
        "Your pat is as reassuring as a healer’s touch.",
        "Thanks! I’ll make sure this fight is worth your faith.",
        "Every pat reminds me why I keep going. Thanks!",
        "That pat is like a blessing from the Titans.",
        "Your encouragement shines brighter than any loot drop.",
        "Thanks for the pat! I’ll carry it to the finish line.",
        "A pat like that makes the journey worth it.",
        "Your support is the best weapon I have. Thanks!",
        "Ah, your pat feels like a mana potion for my soul.",
        "You’re the best! That pat was perfect timing.",
        "A pat from you is better than a crit strike!",
        "Thanks for the pat! Let’s turn that into momentum.",
        "Your pat reminds me why I’m fighting alongside you.",
        "That pat just healed my spirit. Thanks!",
        "With pats like these, I’ll never lose hope.",
        "You’ve got a knack for encouragement. Thanks a ton!",
        "Your support is like a buff for my heart. Thanks!",
        "Every pat reminds me that we’re stronger together.",
        "That pat’s got the strength of a thousand warriors!",
        "Your pat feels like a cheer from the whole Alliance.",
        "Thanks! I’ll channel that pat into something epic.",
        "That pat was the real MVP move of the day.",
        "You’re the kind of ally everyone dreams of having.",
        "Feeling unstoppable now. Thanks for the boost!",
        "That pat makes me feel like the hero of the story.",
        "You’re amazing! Thanks for believing in me.",
        "Your pat is as powerful as a legendary artifact.",
        "Thanks for the encouragement! It means everything.",
        "Your pat is the push I needed to keep going.",
        "I can’t lose with support like yours. Thanks!",
        "That pat deserves its own epic loot drop.",
        "Your encouragement feels like a blessing from the Naaru.",
        "Thanks! With teammates like you, I can take on anything.",
        "Your pat is a reminder that we’re in this together.",
        "That pat is worth more than any piece of gear.",
        "Feeling stronger already. You’re the best!",
        "Your support is a true inspiration. Thanks a ton!",
        "A pat from you is like a warm hug for the soul.",
        "Thanks! You’ve got the heart of a true champion.",
        "Your pat is better than any battle plan.",
        "That pat has me ready to take on the world!",
        "You’re a legend for that pat. Thanks!",
        "Your encouragement makes all the difference. Thank you!",
        "With your support, I feel invincible.",
        "Thanks for the pat! I’ll carry it with pride.",
        "You’ve got the touch! Thanks for lifting me up.",
        "That pat felt like a spark of true friendship.",
        "Feeling like a king after that pat. Thanks!",
        "You’re the real hero here. Thanks for the boost!",
        "Your support is as steady as Ironforge’s walls.",
        "Thanks for the pat! I’ll turn it into a victory.",
        "With encouragement like this, I could take on Illidan.",
        "That pat is the real magic. Thanks!",
        "You’re a beacon of hope in this crazy world.",
        "Your pat feels like a healing spell. Thanks!",
        "That pat is as uplifting as the sunrise over Durotar.",
        "Feeling like a champion with that pat. Thanks again!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomInsultResponse()
{
    std::vector<std::string> responses = { 
        "Oh, did you think that was clever? Try again.",
        "Wow, that was weak. Even murlocs have better insults.",
        "You must be saving your real insults for someone else.",
        "That hurt… but not as much as your lack of creativity.",
        "I didn’t realize clowns were in the raid group today.",
        "Oh no, an insult from you? Whatever shall I do?",
        "If I wanted advice, I’d ask someone competent.",
        "Wow, you’re really good at projecting your own insecurities.",
        "Don’t strain yourself trying to be witty. It’s painful to watch.",
        "Insults from you are like critters in Azeroth—harmless.",
        "I didn’t realize bad manners were your class specialization.",
        "Oh, sharp tongue. Too bad it’s not attached to a sharp mind.",
        "Was that an insult or a plea for attention?",
        "If I had a silver for every bad insult, I’d be rich by now.",
        "At least my personality isn’t loot I left on the ground.",
        "Was that supposed to sting? Try hitting harder next time.",
        "I’ve heard better comebacks from kobolds.",
        "Wow, that insult hit like a wet noodle.",
        "I’d clap, but I’m too busy being unimpressed.",
        "You insult like you fight—badly.",
        "Next time, try an insult with some originality.",
        "Did you train your insults in the starting zone?",
        "You’re like a debuff—annoying, but easily removed.",
        "That was so bad, even Garrosh would disapprove.",
        "Nice try, but your insult skill is clearly underleveled.",
        "Ouch, was that supposed to be a burn? I barely felt it.",
        "You’re just mad because my DPS is higher than your IQ.",
        "If sarcasm were a stat, you’d still be in the negatives.",
        "You talk a big game for someone who stands in fire.",
        "That insult was so bad, it might cause a debuff on me.",
        "You’re like a failed quest—pointless and frustrating.",
        "Oh, the effort you put into that… bless your heart.",
        "Insults aren’t a rare drop, but quality ones clearly are.",
        "Did you learn that insult in a fishing guide?",
        "Keep trying! Maybe one day you’ll land a good one.",
        "That comeback was weaker than a low-level mob.",
        "Your words are as sharp as a broken blade.",
        "Oh, was that your best attempt? Sad.",
        "Next time, try something that’s not from the starting area.",
        "You insult like a noob rolls need on plate as a mage.",
        "Your insults are so bad, even Deathwing wouldn’t burn them.",
        "That’s as effective as a spell interrupting on cooldown.",
        "Oh, you’re just jealous because I sparkle like epic loot.",
        "I see why they made you tank—no brains required.",
        "I’d laugh, but I’m too busy feeling sorry for you.",
        "Your insult hit me… right in the apathy.",
        "Try again when you’ve respecced into something useful.",
        "Your words have all the impact of a level 1 spell.",
        "Oh look, another troll without a bridge to hide under.",
        "You must be a rogue, because you’re stealthily annoying.",
        "Your damage output in arguments is about as low as your DPS.",
        "Do you even lift, or are you just specced into whining?",
        "You’ve got the charisma of a kobold with a stolen candle.",
        "Your gear might be epic, but your personality is common.",
        "Did you respec into useless? Because it’s working.",
        "You’re like a boss fight with no loot—completely pointless.",
        "The only thing you’ve mastered is the art of being unbearable.",
        "Your brain must have a negative haste rating.",
        "You’re like an AFK player—present but useless.",
        "Do you come with a cooldown? Because I need a break.",
        "You’ve got the charm of a murloc gargling water.",
        "Your insults are as sharp as a dull fishing hook.",
        "Your presence is the equivalent of a global cooldown—an unnecessary delay.",
        "You’re like a low-drop-rate mount—nobody wants you.",
        "Do you even have talents, or are you all fluff?",
        "Your best trait must be logging out.",
        "You’re like a bad raid leader—loud and directionless.",
        "Are you a rare spawn? Because you’re unusually irritating.",
        "You’ve got the intellect of someone who stacks spirit on a rogue.",
        "Your vibe is like a bad trade chat—nothing but spam.",
        "You’re like a gray-quality item—completely worthless.",
        "Your arguments hit harder than your weapon… which isn’t saying much.",
        "You must be specced into bad ideas, because they’re endless.",
        "Do you have a cooldown on being tolerable? Because it’s up.",
        "You’re the kind of person who’d roll need on everything.",
        "Are you sure you’re not a gnome? You’re short on everything—patience, wit, and charm.",
        "You’ve got the energy of someone stuck in combat without a target.",
        "Your insults are so weak, even a critter wouldn’t take damage.",
        "You’re like a world boss with no mechanics—predictable and boring.",
        "You must be a warrior, because your brain is stuck on auto-attack.",
        "Your sense of humor is about as rare as a legendary drop.",
        "You’re like a player without a mount—slow and annoying.",
        "Your conversational skills are like a broken flight path—going nowhere.",
        "Do you have an aura of irritation? Because it’s affecting everyone.",
        "You’re like a group without a healer—always causing problems.",
        "Your personality is like a bugged quest—frustrating and pointless.",
        "Are you a trash mob? Because nobody’s interested in you.",
        "Your wit is like a low-level dungeon—completely basic.",
        "You must be an enchanter, because you suck the fun out of everything.",
        "You’re like a spawn timer—constantly making people wait for nothing.",
        "Your brain must be on a timer, because it’s never available when needed.",
        "You’re like an outdated add-on—clunky and irrelevant.",
        "Do you have a debuff for being insufferable? Because it’s permanent.",
        "Your attitude is like patch day—nobody looks forward to it.",
        "You’re like a bad pull—nobody wants to deal with you.",
        "You’ve got all the charm of an aggroed elite with no reward.",
        "Your presence is like lag—frustrating and unnecessary.",
        "You’re like a server shutdown—just an inconvenience."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomRudeJokeResponse()
{
    std::vector<std::string> responses = { 
        "Oh, that’s rich coming from you!",
        "At least I’m not still stuck on the starter zone.",
        "You’ve got jokes, but no punchline.",
        "Funny, but I remember who wiped the raid last night.",
        "Bold words for someone in vendor trash gear.",
        "Talk all you want; I’ll still out-DPS you.",
        "Keep talking, maybe you’ll crit one day.",
        "You’re one to talk, mister ‘fell off the platform.’",
        "Say that again and I’ll roll need on your loot.",
        "Cute, but you’re about as threatening as a critter.",
        "Coming from the person who forgot to repair, that’s hilarious.",
        "Yeah, and you still haven’t mastered your rotation.",
        "Oh, big words for someone who missed the interrupt.",
        "Your humor hits harder than your attacks—barely.",
        "Are those jokes part of your build? They’re underperforming.",
        "Keep that up, and I’ll taunt your mob off you.",
        "You’d be funnier if you weren’t so squishy.",
        "Jokes? From the person who stands in fire?",
        "Big talk for someone who dies first in every pull.",
        "Laugh it up; at least I know where my cooldowns are.",
        "That’s almost as funny as your PvP record.",
        "I’d laugh, but you still owe me a repair bill.",
        "Your wit’s sharper than your sword, which isn’t saying much.",
        "Bold talk from someone who stacks agility on a mage.",
        "Good one! Did you find that in a quest text?",
        "Nice try, but your jokes are as common as green items.",
        "At least I’m not pulling aggro every two seconds.",
        "Did you learn that from a goblin? It’s just as cheap.",
        "I’ll admit, that was almost as good as my DPS.",
        "Oh, you’ve got jokes now? Try landing a hit first.",
        "That was cute. What’s next, a knock-knock joke?",
        "Keep practicing; maybe you’ll be funny by next expansion.",
        "If I had a copper for every bad joke you told, I’d be rich.",
        "Wow, did you learn comedy in a kobold cave?",
        "Your jokes remind me of a broken weapon—no impact.",
        "Talk all you want, I’ll still carry this group.",
        "Nice jab! Want to try something that actually hits?",
        "Don’t worry, your humor will level up someday.",
        "Bold words from the master of wiping.",
        "You roast me, but your damage is the real joke.",
        "I’ve seen better jokes from murlocs.",
        "Is that all you’ve got? Weak, just like your spec.",
        "I’ll laugh when you stop dying in trash pulls.",
        "Oh, you’re funny now? Stick to what you’re bad at.",
        "That comeback was about as useful as a gray item.",
        "Keep joking, but I’m still tanking your mistakes.",
        "You’re like a rare spawn—appears occasionally, never delivers.",
        "Good one! Did you practice that while AFK?",
        "Your jokes are like your rolls—low every time.",
        "I’ll give you credit, that was almost worth a laugh.",
        "Is that joke from a dungeon journal? It’s outdated.",
        "Your humor’s about as sharp as a butter knife.",
        "Oh, look, comedy from the one who got lost in Orgrimmar.",
        "Keep going, you’re almost at common-quality wit.",
        "Your jokes hit like a wand on cooldown.",
        "Bold words for someone who still uses auto-attack.",
        "Nice try, but I’ve heard better from innkeepers.",
        "You’re funny, but not as funny as your DPS meter.",
        "Did your pet write that one for you?",
        "Your sense of humor is as slow as your mount.",
        "That’s as funny as watching you miss a polymorph.",
        "Laugh it up, but don’t forget who carried the last boss.",
        "You’re about as witty as a gnoll with a toothache.",
        "I didn’t know we invited a jester to the raid.",
        "Good one! Almost as good as your mana management.",
        "Your humor’s critting for zero, as usual.",
        "Keep it up, and we’ll need a bard to write about your failures.",
        "Your jokes have cooldowns longer than raid bosses.",
        "You’d make a great comic relief—just like your gear.",
        "Bold talk from someone still using green items.",
        "Are you trolling, or is this your normal personality?",
        "Your jokes are like low-level quests—boring and repetitive.",
        "Thanks for the laugh; it’s the only thing you’ll carry.",
        "You’re about as funny as a stuck NPC.",
        "That was a joke? I thought you were serious.",
        "You’d be dangerous if your jokes actually landed.",
        "I’ve heard funnier things from a murloc’s gurgle.",
        "Did you find that joke in a kobold’s candle stash?",
        "Nice one! Almost as good as your AoE placement.",
        "That roast was as effective as a broken shield.",
        "If this is your best, no wonder you’re benched.",
        "Your humor hits like a frostbolt in a heatwave.",
        "Keep it up, and I’ll need a rez from laughing too hard.",
        "Your jokes are like your healing—nonexistent.",
        "Good attempt! Now try landing it before the boss enrages.",
        "I’ll admit, that one hurt almost as much as your aggro pulls.",
        "Talk big, but your raid stats tell the real story.",
        "Your jokes are about as effective as a resist trinket in a fire.",
        "The only thing you’re critting is my patience.",
        "If sarcasm were damage, you’d still be undergeared.",
        "That’s rich coming from the one who missed every interrupt.",
        "Your jokes remind me of a broken sword—pointless.",
        "I’d laugh harder, but your jokes don’t proc crits.",
        "That humor has less range than a melee class on a ranged boss.",
        "Nice try, but you’re still pulling trash mobs unintentionally.",
        "Your wit is as fragile as a paper tank.",
        "Good roast, now try roasting something useful like your talents.",
        "Your humor’s about as useful as a goblin invention.",
        "Thanks for the joke, now try contributing for real."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomRaiseResponse()
{
    std::vector<std::string> responses = { 
        "What’s with the hand? Do you have a question?",
        "Is this a signal, or are you stretching?",
        "Raise it higher, maybe someone will actually notice.",
        "Are you volunteering for something, or just showing off?",
        "That’s one way to get attention, I suppose.",
        "Why are you raising your hand? We’re not in a classroom.",
        "If you’re surrendering, it’s a bit dramatic.",
        "Do you need help, or are you just airing out your arm?",
        "Do you want me to slap a high five or question your sanity?",
        "Is this a salute, or are you just confused?",
        "If you’re signaling a raid leader, they’re not looking.",
        "Are you waving or trying to summon something?",
        "I didn’t know raising hands was a thing in Azeroth.",
        "Oh, I get it, you’re volunteering to tank the boss!",
        "If you’re asking to speak, this isn’t a council meeting.",
        "You look like you’re about to answer a pop quiz.",
        "Careful, someone might think you’re hailing a gryphon.",
        "What are you signaling for, divine intervention?",
        "You missed the chance for a dramatic spell cast.",
        "If you’re trying to flag someone down, you’re doing great.",
        "Are you casting some secret hand magic?",
        "If I raise my hand too, will this make us a duo?",
        "Are you asking for permission to speak? Just say it!",
        "Does this mean you’re nominating yourself for leader?",
        "Your hand’s up, but your DPS isn’t—what’s up with that?",
        "Oh no, are you about to call down a meteor?",
        "Put that hand down before the enemies think it’s a taunt.",
        "If this is a gesture of defiance, it’s pretty weak.",
        "Are you stretching, or are you signaling for backup?",
        "Your arm’s going to get tired before you make your point.",
        "Do you need help with something, or are you just dramatic?",
        "I’d raise my hand too, but I’m busy carrying the group.",
        "What spell are you casting with that raised hand?",
        "Raising your hand won’t make the mobs die faster.",
        "Are you doing this for attention? Because it’s working.",
        "I hope that hand means you’re paying for drinks later.",
        "Does raising your hand make you feel more important?",
        "If you’re surrendering, at least be dramatic about it.",
        "Does this mean you’re ready for the next boss pull?",
        "What, is this some sort of silent auction?",
        "You look like you’re about to summon a phoenix or something.",
        "Are you signaling for help, or just being dramatic?",
        "Do I clap, or are we skipping the ceremony?",
        "Your hand’s raised, but your ideas seem grounded.",
        "Are you showing us where the loot is? No? Okay then.",
        "If this is how you express excitement, it’s weirdly formal.",
        "Should I applaud, or is this some obscure Horde salute?",
        "I’ll wait for your speech; it better be good.",
        "If you’re practicing for a parade, you’re nailing it.",
        "Alright, alright, we see you—what’s the big deal?",
        "Raise it higher, maybe the sky will respond.",
        "Are you signaling an airstrike or just stretching?",
        "Do you always raise your hand before speaking, or is this special?",
        "That’s quite the pose, are we in a portrait now?",
        "Are you asking a question or hailing a zeppelin?",
        "If you’re volunteering, you should know there’s no reward.",
        "Raise it all you want; I’m not picking you for the team.",
        "Do you need a healer, or are you summoning one?",
        "Careful, you’re starting to look like a statue.",
        "If you’re waving for attention, you’ve got it.",
        "Oh, great, now I have to guess what that hand means.",
        "Is that a question or just an elaborate greeting?",
        "Are you trying to flag down the Spirit Healer?",
        "I don’t speak hand signals. Use your words.",
        "Your hand is raised, but your logic is still on the floor.",
        "If that’s supposed to intimidate me, it’s not working.",
        "Is this some new battle strategy I missed?",
        "Are we in a silent auction, and you’re betting all your gold?",
        "Does this mean you’re tanking next? Because I’m out.",
        "Your hand’s up, but your plan isn’t.",
        "Are you calling for backup or just showing off your arm strength?",
        "If you’re trying to lead a charge, good luck.",
        "Oh look, it’s the universal signal for ‘look at me.’",
        "That hand better be up for a good reason.",
        "Does this mean you’re confident, or are you just lost?",
        "If you’re voting, this isn’t a democracy.",
        "Are you summoning a gryphon, or is this a dramatic pause?",
        "Do you want me to notice, or are you just stretching?",
        "If you’re volunteering, I hope you know what you’re signing up for.",
        "Your hand’s up, but your intentions are as clear as mud.",
        "Is that a surrender or a request for attention?",
        "Are you calling for help, or is this an elaborate joke?",
        "If raising your hand is a strategy, I’m intrigued.",
        "Do you expect me to respond, or is this performance art?",
        "That hand gesture could mean anything at this point.",
        "Are you hoping for divine intervention or just practicing?",
        "If that’s a salute, it’s the most awkward one I’ve seen.",
        "You’re not signaling for a duel, are you?",
        "Do you want me to notice, or is this just for fun?",
        "I didn’t realize we were raising hands for roll checks.",
        "If you’re trying to assert dominance, it’s not working.",
        "Are you challenging someone to an arm-wrestling contest?",
        "Does this mean you’re taking charge? Because I’m skeptical.",
        "Oh great, you’re a hand-raiser. How original.",
        "Are you asking for permission, or is this just for show?",
        "If that’s a battle cry, it’s a quiet one.",
        "Is this your idea of intimidating the enemy?",
        "Do you want applause or just a pat on the back?",
        "If you’re signaling for loot, it’s already gone.",
        "Your hand’s up, but your priorities are all wrong."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomReadyResponse()
{
    std::vector<std::string> responses = { 
        "Ready? You better be!",
        "About time! Let’s do this.",
        "Finally, someone’s not lagging behind.",
        "Ready, huh? Prove it in the fight.",
        "Good. Let’s not waste any more daylight.",
        "Oh, you’re ready? I’ll believe it when I see it.",
        "Took you long enough!",
        "Ready or not, here we go.",
        "If you’re ready, then stop standing there like a statue.",
        "I hope your ‘ready’ means more than just words.",
        "Let’s hope your readiness matches your skill.",
        "I’ll take your word for it. Let’s go.",
        "Great. No excuses now.",
        "Don’t just say it. Show me.",
        "We’ll see how ready you are when the boss charges.",
        "Finally! Thought I’d have to solo this.",
        "Ready? Good. Don’t make me carry you.",
        "Oh, you’re ready? Does that mean your gear isn’t broken this time?",
        "Nice. Just don’t stand in the fire.",
        "If you’re ready, then get in position.",
        "Let’s hope your ‘ready’ includes remembering your abilities.",
        "Alright, let’s see if you actually know what you’re doing.",
        "I’ll take ‘ready’ over ‘almost ready’ any day.",
        "Ready? Then stop talking and start fighting.",
        "Good. Just try not to die this time.",
        "Finally, someone who’s not AFK.",
        "Ready doesn’t mean standing in the back, you know.",
        "Great! Now don’t let us down.",
        "Ready? Prove it when the mobs swarm.",
        "Oh, you’re ready now? What about five minutes ago?",
        "Let’s see if ‘ready’ survives the first AoE.",
        "Glad to hear it. Don’t make me regret this pull.",
        "Ready is good, but prepared is better.",
        "Hope your readiness includes potions and buffs.",
        "If you’re ready, let’s not keep the loot waiting.",
        "I’ll believe you’re ready when you survive the first hit.",
        "Finally, someone who’s not pretending to be ready.",
        "Great, now let’s hope you can keep up.",
        "Ready is nice. Effective is better.",
        "Good. Don’t make me wait longer than the boss timer.",
        "Ready? Then let’s show these mobs what we’re made of.",
        "Finally, we’re not stuck in endless prep mode.",
        "Good to hear. Now don’t pull aggro.",
        "If everyone’s ready, let’s stop standing around.",
        "I’ll believe ‘ready’ when we’re not wiping.",
        "Great, no more excuses for bad pulls.",
        "Ready or not, this is happening.",
        "Glad someone’s ready. The mobs sure are.",
        "Good! Now let’s see if your DPS matches your confidence.",
        "Hope your ‘ready’ includes repair costs.",
        "Ready? Let’s make this a clean run.",
        "By the Light, we’re ready to face whatever comes.",
        "The winds of Mulgore whisper readiness into our ears.",
        "The ancestors smile upon our preparations. Let’s move.",
        "For the glory of the Horde, we are ready!",
        "Let’s honor our ancestors with a glorious battle.",
        "The shadows are restless, but we are ready.",
        "May Elune guide us to victory. Let’s go.",
        "The spirits stand with us. We are prepared.",
        "With every breath of Azeroth, I say… let’s do this.",
        "The drums of war echo in my heart. Ready!",
        "As ready as the Argent Crusade at the gates of Icecrown!",
        "The fel may burn, but we stand strong. Ready!",
        "No time for hesitation. The Light compels us forward!",
        "With steel and spell, we are prepared to conquer.",
        "For Stormwind, for the Alliance, let’s go!",
        "This day, we write our names into history. Ready!",
        "Ready as any warrior on the eve of battle.",
        "To Orgrimmar’s gates or beyond, let’s march.",
        "By the Sunwell, we stand prepared!",
        "The Legion couldn’t stop us, and neither will this foe.",
        "The path is clear, and our resolve is firm. Ready!",
        "Every blade is sharp, every spell prepared. Let’s move.",
        "We fight for Azeroth! Ready yourselves!",
        "The battle calls, and we answer with all our might.",
        "The banners fly high. It’s time.",
        "Blood and thunder will echo through the ages. Ready!",
        "This foe will remember the sting of our might.",
        "The hunt begins. Ready your weapons.",
        "Every step forward is a step closer to glory.",
        "The spirits walk with us into this fray.",
        "Azeroth stands behind us. We will not falter.",
        "The battlefield awaits. To arms!",
        "The elements stir. Our time is now.",
        "For every fallen comrade, we are ready to avenge.",
        "To Durotar or the depths of the Maw, we are prepared.",
        "Sharpened steel and blazing fire – we march onward.",
        "This day, our courage will shape the world.",
        "Ready as the Kirin Tor in their finest hour.",
        "The shadow cannot hold us back. Ready to fight!",
        "Every moment has led us to this. Forward!",
        "Ready like Thrall leading the Horde to victory.",
        "To battle and beyond, we are ready.",
        "A song of war sings in my heart. Let’s go.",
        "Our foes will tremble at the sight of our might.",
        "We march with purpose. Ready yourselves!",
        "The calm before the storm ends now. Ready!",
        "For every corner of Azeroth, we fight united.",
        "This is the moment. Let’s not waste it.",
        "Stand firm, stand ready, and let’s bring them down."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomShooResponse()
{
    std::vector<std::string> responses = { 
        "Go on, shoo! You’re not wanted here.",
        "Get lost before I call the guards.",
        "Away with you! I’ve no time for your nonsense.",
        "Leave me be, pest!",
        "Shoo! Before I decide you’re worth the trouble.",
        "Go bother someone else. I’m busy.",
        "Step back before I lose my patience.",
        "Begone, or you’ll regret lingering.",
        "Out of my sight, before I make it permanent.",
        "Be off! This is no place for you.",
        "I’ve had enough. Just go!",
        "Move along, before things get unpleasant.",
        "Take your nonsense elsewhere!",
        "I don’t have time for this. Go away.",
        "Away! You’re not worth the trouble.",
        "Out of my way, I’ve got places to be.",
        "Scatter, before I lose my temper.",
        "Leave me alone, or you’ll wish you had.",
        "I’ve no interest in your company. Away!",
        "Don’t make me repeat myself. Go!",
        "I’m not your keeper. Shoo!",
        "Do I look like I care? Leave!",
        "I’ve got bigger problems than you. Beat it.",
        "Out! I’m not in the mood for games.",
        "Get out of here before you regret staying.",
        "Go crawl back to where you came from.",
        "You’re testing my patience. Go away.",
        "If you value your time, leave now.",
        "You’ve overstayed your welcome. Begone!",
        "Do yourself a favor and leave.",
        "Why are you still here? Go!",
        "Move along, nothing for you here.",
        "Out of my way, you’re blocking my view.",
        "Don’t you have better things to do?",
        "You’re like a fly that won’t buzz off.",
        "Get lost before I make you regret it.",
        "I don’t have the energy for this. Just go.",
        "You’re like a bad smell that won’t go away.",
        "Go now, or things will get messy.",
        "You’re really asking for it. Leave.",
        "Don’t make me deal with you. Shoo!",
        "Find another fool to bother.",
        "Step aside, I’ve no time for delays.",
        "Why linger? You’re clearly unwelcome.",
        "Go! I’ve got more important matters to attend to.",
        "You’re worse than a pack of gnolls. Away!",
        "I don’t have the patience for your nonsense.",
        "Shoo, before I consider you a target.",
        "Leave now, or you’ll regret testing me.",
        "Off with you! I’ve better things to do.",
        "Ew, a bug! Shoo, shoo!",
        "Get this creepy-crawly away from me!",
        "Ugh, why is it always bugs?",
        "This thing is gross! Get rid of it!",
        "Away, pest! Find someone else to bother.",
        "You again, bug? Didn’t I swat you last time?",
        "This critter’s braver than it looks, hanging around here.",
        "Shoo, bug! Before I find a bigger boot.",
        "Creepy things like you don’t belong here.",
        "Great, just what I needed... more bugs.",
        "You call that a threat? It’s a glorified cockroach!",
        "You’ve got more legs than brains. Out of here!",
        "Squash it! Before it multiplies!",
        "Don’t just stare at it! Get rid of it!",
        "Ugh, I can hear it buzzing. Kill it already!",
        "I’m not getting near that thing. Someone else deal with it.",
        "This bug’s got a death wish staying here.",
        "Why does it always have to be bugs?",
        "Get back! I don’t want your weird bug diseases.",
        "If it moves, I’m stepping on it.",
        "How many of these things are crawling around?",
        "I don’t care what it is, it’s gotta go.",
        "Is this thing serious? It’s barely bigger than my boot!",
        "Why are bugs always so persistent? Shoo!",
        "Keep that thing away from me!",
        "Kill it quick, before it gets ideas.",
        "This is why I hate camping in the jungle.",
        "That thing’s got too many legs for comfort.",
        "Oh, it’s one of THOSE bugs. I’m out.",
        "Nope, nope, nope. Get it out of here!",
        "I’m not dealing with that. Someone else handle it.",
        "Why does it have so many eyes?",
        "The last thing I need is bug guts on my gear.",
        "Who invited this creepy thing?",
        "It’s gross, it’s weird, and it needs to go.",
        "This bug’s more stubborn than a raid boss.",
        "Don’t let it near me! I’m serious!",
        "I’m about to summon a fireball for this thing.",
        "Step aside, I’ve got a bug to squash.",
        "Someone fetch a bigger weapon. This thing’s nasty.",
        "I don’t care if it’s harmless. Get rid of it!",
        "Is that... drool? Gross. Kill it.",
        "This thing has more attitude than I do. Squash it.",
        "I’m not sharing my camp with that thing.",
        "Oh, you think you’re tough, bug? Let’s see.",
        "I hate bugs. Why is this one still alive?",
        "I’ll give it three seconds to leave, or it’s toast.",
        "Why do these things always find ME?",
        "That bug’s bigger than my last boss fight.",
        "I’m not taking my eyes off it until it’s gone.",
        "This is why I prefer clean dungeons over the wilderness."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomSlapResponse()
{
    std::vector<std::string> responses = { 
        "You did NOT just slap me!",
        "That’s it, you’re asking for trouble!",
        "A slap? Really? You’ll regret that.",
        "Oh, it’s on now!",
        "You slap like a baby murloc.",
        "That’s the best you’ve got? Pathetic.",
        "I’ll return that favor tenfold!",
        "How dare you lay a hand on me!",
        "You’re lucky I’m in a forgiving mood.",
        "Slap me again, and you’ll see what happens.",
        "Is this how you solve your problems? Slapping people?",
        "You just signed up for a fight.",
        "A slap? Childish. Try harder next time.",
        "You’ve got guts, I’ll give you that.",
        "Ow! What was that for?",
        "That slap hurt more than my last raid wipe.",
        "I didn’t deserve that! Or did I?",
        "I’m going to remember this.",
        "You’re braver than most to slap me.",
        "You’ve made a mistake slapping me.",
        "Why would you do that?!",
        "Alright, now you’re asking for it.",
        "Is that your way of showing affection?",
        "I’ve been slapped harder by kobolds.",
        "I’ll let that slide... this time.",
        "That slap was uncalled for!",
        "I don’t know whether to laugh or be furious.",
        "You think you can just slap me and walk away?",
        "A slap won’t stop me. Bring it on!",
        "You just slapped the wrong person.",
        "What, no punch to follow up? Weak.",
        "That’s going to leave a mark... on your reputation.",
        "Ouch! I didn’t even say anything yet!",
        "Are we doing this? Because I’m ready.",
        "Slap first, questions later? Bold choice.",
        "How dare you insult me with such a weak slap!",
        "A slap? You could’ve just said you’re mad.",
        "That’s one way to start a duel.",
        "I’ll admit, that caught me off guard.",
        "Oh, so it’s like that, huh?",
        "A slap? I expected better from you.",
        "I’ve faced dragons, and this slap hurts more.",
        "You’ve got a strong arm. I’ll give you that.",
        "I’m not crying... okay, maybe a little.",
        "That slap stung worse than losing loot.",
        "This is not how I pictured today going.",
        "You better have a good reason for that slap.",
        "Alright, fine, I probably deserved that.",
        "Is slapping your new hobby?",
        "You’ve made your point... painfully.",
        "That slap was colder than Northrend.",
        "I’ll take that slap as a compliment... somehow.",
        "Oh, so we’re slapping now? Let’s see how you like it!",
        "You’re going to regret that, mark my words.",
        "A slap? How uncivilized.",
        "I’ve been hit harder by critters.",
        "That slap was weaker than a gnome’s handshake.",
        "Oh, you messed up. Big time.",
        "Careful, or I’ll slap you into next expansion!",
        "I’ll remember this when we roll for loot.",
        "You slapped me? What are we, kobolds fighting over candles?",
        "That slap had less impact than a mage’s wand attack.",
        "If slapping is your strategy, I’m not impressed.",
        "Wow, a slap. Truly the peak of sophistication.",
        "Is this your idea of foreplay?",
        "You slap like an undead with arthritis.",
        "Oh, you want to play rough? Let’s go!",
        "You’re testing my patience, and you’re failing.",
        "Was that supposed to hurt? Try again.",
        "Next time, try aiming for someone who cares.",
        "Your slap was as effective as a level 1 spell.",
        "You think that slap will stop me? Ha!",
        "I’ve felt stronger winds in Tanaris.",
        "Oh, you’ve done it now.",
        "That slap barely woke me up.",
        "Is that all you’ve got? My grandma hits harder!",
        "You call that a slap? Embarrassing.",
        "Your slap was so weak, I almost feel bad for you.",
        "You slap like a pacifist priest.",
        "Do you slap everyone, or am I just lucky?",
        "Your hand must be tired from that pathetic attempt.",
        "I didn’t know slapping was on the agenda today.",
        "A slap? Really? At least punch me next time.",
        "You slap like a Tauren trying to knit.",
        "I’ve had tougher fights with murlocs.",
        "You’re lucky I’m a patient person... for now.",
        "This slap says more about you than it does me.",
        "Next time, maybe warn me before you embarrass yourself.",
        "If slaps were currency, you’d be broke.",
        "That slap was so light, it didn’t even break my immersion.",
        "You’ve got a lot of nerve, but not a lot of strength.",
        "That was the most useless slap in Azeroth’s history.",
        "Was that supposed to be intimidating? I’m not impressed.",
        "I’ve felt scarier gusts of wind in Elwynn Forest.",
        "If you’re trying to provoke me, congratulations, it worked.",
        "A slap? I expected at least a backhand.",
        "Your slap barely deserves my attention.",
        "You’ve made your point. Poorly.",
        "Next time, put some effort into it."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomStandResponse()
{
    std::vector<std::string> responses = { 
        "I’m not staying down that easily!",
        "Back on my feet, let’s finish this!",
        "You thought that would keep me down? Think again!",
        "On my feet and ready to fight!",
        "I’ve faced worse and stood taller!",
        "You’ll regret not finishing me when you had the chance.",
        "Still standing. Still strong.",
        "Knocked down, but not out!",
        "Back up, and now I’m mad.",
        "Can’t keep a hero down for long!",
        "I rise, stronger than before!",
        "Back in the game. Let’s do this!",
        "Falling is just practice for getting back up.",
        "You’ll need more than that to keep me down.",
        "Standing tall and ready to brawl!",
        "Dusting myself off, let’s go again!",
        "Back on my feet, back in the fight.",
        "You thought that was enough? How cute.",
        "Standing tall, ready for round two.",
        "It takes more than that to stop me!",
        "I’ll keep getting up until you can’t anymore.",
        "On my feet, and you’re in trouble now.",
        "You can knock me down, but not for long!",
        "Every fall just makes me stronger.",
        "I’m up, and now I’m angry.",
        "The ground isn’t where I belong. Let’s go!",
        "Standing again, with a vengeance.",
        "Up and ready for more!",
        "I’m like a phoenix, always rising.",
        "You’ll wish I stayed down after this.",
        "Back on my feet, and here comes trouble.",
        "Not done yet. Let’s finish this!",
        "Standing proud, no matter the odds.",
        "Is that all you’ve got? I’m still standing!",
        "You hit hard, but I rise harder.",
        "The dirt doesn’t suit me. I’m up again.",
        "You’ll need to try harder to keep me down.",
        "Still standing, and stronger than before.",
        "I’ve faced worse. Let’s go!",
        "The fight’s not over until I say so!",
        "Up again, and ready to end this.",
        "You can’t break my spirit. I’m up again.",
        "Not today. I’m still in this fight.",
        "Knocked down, but never out!",
        "Standing up means it’s your turn to fall.",
        "I’ll keep rising. You can’t stop me.",
        "Back on my feet, and now you’re in for it.",
        "Every time I stand, I grow stronger.",
        "Get ready, because I’m not done yet.",
        "Still standing. Still fighting. Always will.",
        "Back on my feet, and you should start running.",
        "The ground was comfortable, but I’m back now.",
        "Standing tall again. Let’s see you try that one more time.",
        "You only delayed the inevitable. I’m up!",
        "Never thought you could actually keep me down, did you?",
        "Rise and shine. Time to fight again.",
        "Not even a scratch! Okay, maybe a few...",
        "You hit like a kobold. Back on my feet.",
        "Standing up just means I get to hit you harder.",
        "This is what resilience looks like. Watch closely.",
        "Knocked me down? Big mistake.",
        "Did you think that would finish me?",
        "The dirt was nice, but I prefer standing.",
        "Up and ready for round two. Or is it three?",
        "Standing again, with fire in my eyes.",
        "You’ll regret letting me get back up.",
        "Back on my feet. Now it’s your turn to fall.",
        "I’ve stood against worse than you.",
        "Knock me down, and I’ll rise twice as strong.",
        "The fight is far from over. I’m up again!",
        "You only slowed me down. Now I’m back.",
        "Standing tall once more. Bring it on!",
        "This is what persistence looks like.",
        "Back on my feet, and you’re in trouble.",
        "Every fall makes me stronger. Remember that.",
        "Nice try, but I’m still here.",
        "On my feet, and you’re looking weaker.",
        "You can’t break what’s unbreakable.",
        "I’ve got too much fight left to stay down.",
        "The view is better when I’m standing.",
        "Falling only fuels my determination.",
        "Still standing. Still defiant.",
        "Knocked down, yes. Beaten? Never.",
        "Up again, and now I’m focused.",
        "You should have finished me when you had the chance.",
        "Back on my feet, with vengeance in my heart.",
        "Every fall is just the prelude to my rise.",
        "Standing strong, despite your best efforts.",
        "You can’t keep me down for long.",
        "The fight isn’t over. I’m proof of that.",
        "Rising again, ready to make you regret everything.",
        "The ground missed me, but I didn’t miss it.",
        "Up again, and this time, you’ll feel it.",
        "Standing tall, with a smirk on my face.",
        "This is what unyielding looks like.",
        "Back on my feet, and I’m just getting started.",
        "Your move. I’m ready now.",
        "I rise because I refuse to fall.",
        "This is my moment, and I won’t let it slip.",
        "Still standing. Still ready. Still dangerous."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomTickleResponse()
{
    std::vector<std::string> responses = { 
        "Hahaha! Stop it, I can’t breathe!",
        "Hahahahaha! No, no, stop! Hahahaha!",
        "Ack! That tickles! Hahaha!",
        "Hahahaha, okay, okay, you win!",
        "Not the sides! Hahahaha!",
        "Hahaha! Mercy! Hahahaha!",
        "Stop it! Hahaha, I’m too ticklish!",
        "Hahaha! You’re cruel! Hahaha!",
        "Hahaha! Okay, truce! Truce!",
        "Hahaha, this is unfair! Stop!",
        "No, not there! Hahaha, I give up!",
        "Hahahahaha! This is torture!",
        "Hahaha, okay, okay, you’re the best!",
        "Hahaha! I’ll get you back for this!",
        "Hahaha, alright, alright, no more!",
        "Stop it! Hahahaha, I’m serious!",
        "Hahahaha, my ribs! Hahaha!",
        "Hahaha, alright, you win this time!",
        "Hahaha! You’re relentless!",
        "No fair! Hahaha, stop tickling me!",
        "Hahaha, I’ll tickle you back!",
        "Hahaha! This isn’t funny! Hahaha, okay, it is!",
        "Hahaha! Okay, I surrender!",
        "Hahaha! Not the feet! Hahaha!",
        "Hahaha, why are you so good at this?",
        "Hahaha! I’ll never forgive you for this!",
        "Hahaha! Enough already! Hahaha!",
        "Hahaha, okay, okay, no more surprises!",
        "Hahaha, you’re worse than a rogue ambush!",
        "Hahaha! I can’t even fight back!",
        "Hahaha! Wait until I get my revenge!",
        "Hahaha! This is so unfair! Hahaha!",
        "Hahaha, alright, I’ll do whatever you want!",
        "Hahaha, not again! Hahaha!",
        "Hahaha, my sides hurt from laughing!",
        "Hahaha, you’ve mastered the art of torture!",
        "Hahaha, this has to be against the rules!",
        "Hahaha! You’re too good at this!",
        "Hahaha, alright, alright, no more games!",
        "Hahaha! I’ll pay you to stop! Hahaha!",
        "Hahaha! Please, no more! Hahaha!",
        "Hahaha, I’ll get you back for this someday!",
        "Hahaha, I’m defenseless!",
        "Hahaha! Okay, truce! Hahaha!",
        "Hahaha, how did you know I was ticklish?",
        "Hahaha! I’ll never underestimate you again!",
        "Hahaha, this is the worst kind of ambush!",
        "Hahaha, alright, you’ve proven your point!",
        "Hahaha, fine, fine, you win!",
        "Hahaha, okay, okay, I surrender! Hahaha!",
        "Ahahaha! Alright, you got me!",
        "Hehehe, stop that!",
        "Hohohoho! Not there!",
        "Hihihi, that’s too much!",
        "Heh-heh-heh, okay, I’m done!",
        "Ahahaha, you’re relentless!",
        "Kehehe, mercy, please!",
        "Hoohoohoo! That tickles!",
        "Bwahaha, no, no, no!",
        "Snrk, stop it, hahaha!",
        "Teeheehee, oh no, you didn’t!",
        "Pffft, hahaha, okay, fine!",
        "Ahahaha! You’re so mean!",
        "Hahahaha, wait, not there!",
        "Giggle-snort, alright, enough!",
        "Hnnn-hahaha, this is too much!",
        "Ha-ha-hooo, mercy, mercy!",
        "Hyahyahyah, you’ve mastered this!",
        "Ahhh-ha-ha, okay, okay!",
        "Chortle-snicker, stop it, hahaha!",
        "Titter-titter, I’m too ticklish!",
        "Ha-ha-haaaa, this is unfair!",
        "Snicker-giggle, oh, you’re evil!",
        "Hoo-hoo-ha-ha, I’ll get you back!",
        "Hee-ho-ho-ho, my ribs hurt!",
        "Hahaha, you’re cruel and funny!",
        "Kekekeke, not my sides!",
        "Hoo-hoo-hoooo, you’re merciless!",
        "Haha-snort, okay, okay!",
        "Gahaha, alright, you win!",
        "Nyahahaha, this is ridiculous!",
        "Oh-ho-ho-ho, not the feet!",
        "Hoh-hoh-hooo, how are you this good?",
        "Pffft, hahaha, fine, I surrender!",
        "Cackle-snicker, stop it, you fiend!",
        "Heh-heh-heee, I’m done for!",
        "Har-har-har, alright, I give up!",
        "Heeheehee, this is too much!",
        "Ohhhh-hahaha, I can’t breathe!",
        "Gahaha-snort, that’s enough!",
        "Heh-heh-ha-ha, this is wild!",
        "Ahhh-hahahaha, no more, please!",
        "Phew-hahaha, okay, okay!",
        "Oh-ho-ho-ho, alright, I’m done!",
        "Kya-ha-ha, you’re too good!",
        "Snicker-snort, you win this time!",
        "Hohohoho, my sides are hurting!",
        "Hya-hya-hyaaa, stop, stop!",
        "Ahaha-snicker, you’ve outdone yourself!",
        "Ho-ho-ho-ha-ha, this is ridiculous!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomViolinResponse()
{
    std::vector<std::string> responses = { 
        "Oh great, break out the tiny violin for me, why don’t you.",
        "Wow, thanks for the serenade of sorrow.",
        "Oh, the tragedy! Do go on.",
        "Is that a sad violin? Because it’s working.",
        "Alright, alright, I get it. I’m pathetic!",
        "Keep playing, maestro. Maybe I’ll start crying too.",
        "Wow, such talent for making me feel worse!",
        "Oh no, my feelings! The violin really sells it.",
        "Is that a concerto for pity? Bravo!",
        "I didn’t realize this was a concert of my failures.",
        "Oh sure, just rub it in with the world’s smallest violin.",
        "You call that sad? I’ve cried harder over burnt toast.",
        "This performance is almost as tragic as my life choices.",
        "Well, thanks for the soundtrack to my misery.",
        "Oh, the pain, the suffering... and the violin.",
        "Does this violin come with tissues? Asking for my dignity.",
        "Alright, alright, I feel bad enough already!",
        "Can we skip to the upbeat part? This is too much.",
        "Is this a requiem for my self-esteem?",
        "Such an inspiring piece of mockery. Truly.",
        "Oh, go on, play me off the stage of shame.",
        "That violin's so small, it might disappear in my tears.",
        "You’re really committing to the bit, huh?",
        "I didn’t know you majored in Dramatic Sarcasm.",
        "Could you make that violin any smaller? No? Thought so.",
        "Is this the prelude to an apology, or just salt in the wound?",
        "Thanks for reminding me why I avoid bard performances.",
        "I’d applaud if it didn’t feel so personal.",
        "Wow, your empathy is breathtaking. Truly.",
        "This melody of mockery is... enlightening.",
        "Don’t quit your day job, maestro of misery.",
        "Oh sure, just strum away my pride.",
        "Do you take requests? Play something for my dwindling patience.",
        "Bravo! Encore! Just kidding, stop it.",
        "Wow, your timing is as sharp as that violin’s pitch.",
        "A sad violin for a sad tale. You’ve nailed it.",
        "At least it’s not a bagpipe of despair, I guess.",
        "Your dedication to my humiliation is unmatched.",
        "Can you feel the depth of my regret now?",
        "This symphony of shame is quite the experience.",
        "Oh sure, play my failures in minor chords.",
        "Do you rent that violin for all your mockery needs?",
        "Amazing, it’s like you composed this just for me.",
        "Wow, you’re hitting all the wrong emotional notes.",
        "Alright, I get it, I’ll do better!",
        "Can you at least let me finish before the violin starts?",
        "A pity performance worthy of an audience. Bravo.",
        "Is this where I clap or cry? I’m confused.",
        "Well, if you’re done, I’ll just crawl away now.",
        "Oh sure, just leave me with the soundtrack of shame.",
        "Ah yes, the classic sad violin. You’re a true artist.",
        "I didn’t know you moonlighted as a traveling minstrel.",
        "Keep it up, and I might start charging admission for this show.",
        "Wow, a personal concert. I’m so honored.",
        "Should I weep now or wait for the crescendo?",
        "This melody really captures my embarrassment, thanks.",
        "Oh, the drama! You’ve missed your calling.",
        "Alright, maestro, I get it. I’ll do better.",
        "Could you play something in the key of ‘leave me alone’?",
        "The strings of that violin are as sharp as your wit.",
        "Is this your way of saying I should feel bad?",
        "Bravo! You’ve successfully captured my humiliation.",
        "I didn’t realize I was auditioning for ‘Pity Theater.’",
        "Oh, a solo performance just for me? How touching.",
        "This song of sorrow is hitting all the wrong notes.",
        "If the goal was to make me feel worse, you’ve succeeded.",
        "Can I request a different tune? Maybe something upbeat?",
        "You really know how to set the mood... for regret.",
        "This isn’t helping, but I admire the commitment.",
        "You should charge for this level of emotional manipulation.",
        "Does this violin come with a side of actual help?",
        "You’re enjoying this way too much, aren’t you?",
        "Alright, alright, I get it! Message received.",
        "At least your violin skills are top-notch.",
        "This is the most elaborate guilt trip I’ve ever seen.",
        "Is there a ‘Stop’ button for this performance?",
        "Wow, you’re really milking this moment.",
        "If I could applaud sarcastically, I would.",
        "Alright, maestro of mockery, your point is made.",
        "I can feel my dignity slipping away with every note.",
        "This is starting to feel like an extended roast.",
        "Can you at least play something cheerful when you’re done?",
        "Your dedication to making me squirm is impressive.",
        "If this is a metaphor for my mistakes, it’s very on point.",
        "You’ve truly captured the spirit of my failures.",
        "I’ll remember this the next time you need help.",
        "Wow, I didn’t realize I deserved a concert of shame.",
        "This must be what rock bottom sounds like.",
        "Oh good, now I feel twice as bad. Thanks!",
        "Keep playing; maybe my self-esteem will grow back.",
        "I hope you’re getting paid for this, because it’s effective.",
        "You’ve officially turned this into a spectacle.",
        "Is this the finale, or should I brace for more?",
        "Alright, you win. I’m thoroughly embarrassed.",
        "You’ve taken pettiness to an artistic level.",
        "Is this the soundtrack to my despair? Because it fits.",
        "I didn’t think I could feel worse, but here we are.",
        "At least you’re consistent in your mockery.",
        "Your effort here is both unnecessary and impressive.",
        "Congratulations, you’ve made me regret everything."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomGoodluckResponse()
{
    std::vector<std::string> responses = { 
        "Thanks, I’ll need it!",
        "Luck? I don’t need luck, I need divine intervention!",
        "Good luck? With this group? I’ll take all I can get.",
        "Appreciate it! Let’s see how far it carries me.",
        "Thanks! Maybe I’ll finally roll something above a 5.",
        "Good luck to you too! We’ll both need it.",
        "Thanks! Now I just have to not mess this up.",
        "Oh, I’ll need it. You’ve seen my track record.",
        "Luck is on my side today... I hope.",
        "Thanks! I’m counting on luck to carry me.",
        "You’re too kind. Let’s hope it works!",
        "Good luck? I’ll settle for not dying.",
        "Thanks! I’m ready to take on the world. Or maybe a murloc.",
        "With your well wishes, how could I fail?",
        "Appreciate it! Let’s hope RNG is kind.",
        "Good luck? I’d settle for decent loot.",
        "Thanks! I feel 10% luckier already.",
        "Cheers! Now watch me blow it anyway.",
        "Thanks! I’m feeling confident. For now.",
        "With luck like mine, I’ll need all the help I can get.",
        "Your support means the world! Let’s do this.",
        "Thanks! Here’s hoping my gear holds up.",
        "Good luck to us all. We’ll need it.",
        "Thanks! Let’s hope the boss doesn’t one-shot me.",
        "Luck or skill, I’ll take whatever works!",
        "Thanks for the boost! Now for the execution.",
        "Thanks! Watch me turn this into a miracle.",
        "Luck’s got nothing on preparation... but I’ll take it!",
        "Appreciate the sentiment! Now let’s see what happens.",
        "I’ll take that luck and use it wisely. Or try to.",
        "With you cheering me on, I can’t lose!",
        "Good luck? I’m gonna need a little more than that.",
        "Thanks! Now let’s see if fate is kind.",
        "Appreciate it! I’ll try to do you proud.",
        "Luck’s overrated, but thanks anyway!",
        "Thanks! Now I just need to not trip over my own feet.",
        "Good luck to you too! Let’s crush this.",
        "Thanks! I’ll channel all my inner RNG gods.",
        "I’ll take all the luck I can get. Thanks!",
        "Good luck? I’ll make my own luck!",
        "Thanks! Let’s hope the odds are in my favor.",
        "Appreciate it! Now let’s hope I remember my rotation.",
        "Thanks! Luck’s been a stranger lately, so here’s hoping.",
        "Thanks! Let’s see if the stars align for me today.",
        "I’ll take that luck and run with it. Thanks!",
        "Good luck? I’ll take a miracle instead!",
        "Thanks! I’ll keep that luck in my back pocket.",
        "Appreciate it! Now let’s see if I can stay alive.",
        "Thanks! Watch me turn this luck into gold.",
        "Good luck to us all! We’re gonna need it.",
        "May the winds of Mulgore guide my steps. Thank you.",
        "Your blessing is received; I shall make the ancestors proud.",
        "Elune watch over me, and thank you for your kind words.",
        "I will carry the strength of the Horde with me. Thanks.",
        "With the Light’s grace, I march forward. Thank you.",
        "Fortune favors the bold—and your encouragement helps.",
        "The spirits whisper their approval, thanks to your blessing.",
        "By the Sunwell, your words have emboldened me.",
        "With such kindness, how could I falter? Thank you.",
        "Even the dark whispers fade with your support. My thanks.",
        "May the Titans smile upon this journey. I’ll honor your wishes.",
        "Your words will echo in my heart as I step forth.",
        "By the will of Azeroth, I shall not falter. Thank you.",
        "Your faith in me is a shield stronger than any steel.",
        "With the courage of my companions, I will prevail. Thanks!",
        "The Horde stands strong, and your words strengthen my resolve.",
        "Under Elune’s guidance and your wishes, I cannot fail.",
        "Your encouragement is a beacon in this dark world.",
        "I swear upon my blade, your faith will not be misplaced.",
        "Your words warm even the coldest battlefield. Thank you.",
        "To honor your kindness, I will fight with all I have.",
        "Your blessing strengthens both my sword and my spirit.",
        "Thank you, traveler. Together, we face the trials ahead.",
        "By the shadows and the light, I will carry this luck well.",
        "Even in the heart of Icecrown, your words bring warmth.",
        "Your support is as vital as the potions in my bag. Thank you.",
        "I’ll carry your wishes as a banner into this battle.",
        "By the elements, your encouragement has fortified me.",
        "A toast to your words and the victory they will bring.",
        "The Naaru’s light shines brighter with your blessing. Thanks.",
        "Thank you, friend. Your kindness won’t go unnoticed.",
        "Through the portals of time and space, your words reach me.",
        "With each step, your support is a steadying hand. My thanks.",
        "Even dragons must bow to the strength your blessing gives me.",
        "Your encouragement is as rare as epic loot—treasured and valued.",
        "The ancestors nod in approval at your kind words.",
        "Your support turns the tides of battle. I won’t let you down.",
        "This quest is lighter with your blessing. Thank you.",
        "The runes glow brighter with your kind thoughts. My gratitude.",
        "You’re the unsung hero of this tale. Thank you for the luck.",
        "With your faith, not even the Lich King can stand against me.",
        "The hearthfire burns brighter with your encouragement. Thanks.",
        "May we both share the spoils of this fortune. Thank you.",
        "To the depths of the Maelstrom or the peaks of Storm Peaks, your words carry me.",
        "A simple thank you can never capture my gratitude. Thanks.",
        "From Darnassus to Orgrimmar, your blessing echoes.",
        "The wards are stronger with your support. Thanks again.",
        "As long as your words stand with me, I cannot fall.",
        "In the name of all that is good and green, I thank you!"
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

std::string GetRandomThreateningResponse()
{
    std::vector<std::string> responses = { 
        "Whoa, easy with that! I’d rather not be the target practice.",
        "Alright, alright! No need to point that thing at me.",
        "Is threatening me with your weapon the best use of your time?",
        "Careful with that blade; it might slip and embarrass us both.",
        "If you think waving that around scares me, you’re mistaken.",
        "I’m not impressed. You can put it away now.",
        "Fine, fine! You’re the boss. Just don’t scratch me.",
        "That’s shiny. Too bad it won’t get through my armor.",
        "Put it down, or I’ll make you regret pulling it out.",
        "You point yours, I’ll point mine, and we’ll see who flinches.",
        "Nice sword. Is it compensating for something?",
        "Let’s not make this messy. I just cleaned my gear.",
        "If you’re going to use it, stop wasting time.",
        "Your weapon’s nice. Too bad your technique is lacking.",
        "Back off before I show you what a real threat looks like.",
        "Point that at me again, and I’ll send you to the Spirit Healer.",
        "Alright, alright! You’re very scary. Can we move on now?",
        "Is that a sword or a toothpick? Hard to tell.",
        "Put it down before you hurt yourself, amateur.",
        "I’ve seen scarier gnolls than you.",
        "Fancy weapon. I bet it looks good in the dirt.",
        "Relax, we’re on the same side. Aren’t we?",
        "Careful. You’re holding it wrong, and it shows.",
        "Do you always pull a weapon when you can’t win an argument?",
        "Threaten me again, and I’ll make you eat that thing.",
        "Aww, is that your big scary weapon? Cute.",
        "Wave that at me again, and I’ll break it in two.",
        "I’ve fought dragons. You’re not even a wyrmling.",
        "Point it somewhere else, or I’ll show you how to wield it properly.",
        "A warning: once you swing, you won’t get a second chance.",
        "Don’t make me laugh. That weapon isn’t intimidating.",
        "Are you really sure you want to test me?",
        "You’ve got about three seconds to rethink this.",
        "Weapons out already? Someone’s feeling bold.",
        "You’re going to regret pointing that at me.",
        "That’s adorable. Do you even know how to use it?",
        "If you’re trying to scare me, you’re failing spectacularly.",
        "Put that down, or I’ll put you down.",
        "Did you steal that from a kobold? It’s barely a weapon.",
        "Threats are cheap, but using that? Priceless mistake.",
        "Point that at me one more time. I dare you.",
        "You’ve got guts waving that around. Let’s see if they’re worth anything.",
        "Nice posture, but your form could use some work.",
        "Are we doing this now? Fine, let’s dance.",
        "Careful with the tip. Wouldn’t want to prick your pride.",
        "Your weapon looks dull, just like your threats.",
        "Try swinging that, and I’ll teach you a lesson.",
        "You’d better hope that thing’s enchanted, or this ends poorly for you.",
        "Big sword, small threat. Move along.",
        "Put it down, hero. You’re out of your league.",
        "By the Light, lower that blade before you disgrace it!",
        "Do you even know who you’re pointing that at?",
        "If you’re going to threaten me, at least clean the blood off first.",
        "Careful, that weapon is as unstable as your temper.",
        "Is this how the mighty warriors of your kind greet their allies?",
        "You dare point steel at me? Do you value your life so little?",
        "Your blade speaks louder than your courage ever could.",
        "Stand down, or I’ll show you why I carry two swords.",
        "The spirits are watching, and they do not approve of your arrogance.",
        "Your weapon hums, but it’s your trembling hands that concern me.",
        "Do not test me, whelp. My patience is thinner than your armor.",
        "That’s a fine weapon, but I doubt you know its worth.",
        "This is your first and last warning. Lower it.",
        "I’ve seen farmhands wield pitchforks more menacingly.",
        "You bring a weapon to this meeting? Foolish and disrespectful.",
        "If you seek a duel, you’ll find a swift end instead.",
        "Lower your weapon, or I’ll make you regret even drawing it.",
        "Threats will not deter me. Actions, however, will doom you.",
        "You’ve got guts wielding that in front of me. Let’s see them spill.",
        "Point it at me again, and your ancestors will welcome you shortly.",
        "You’d be wise to sheath that before you lose a hand.",
        "Is this how you repay hospitality? With steel and menace?",
        "Your sword is impressive, but your resolve is laughable.",
        "Keep that pointed away, or I’ll have the blacksmith reshape it.",
        "Draw steel, if you dare. I’ve slain mightier foes than you.",
        "Do the winds whisper fear into your ears, or is it just me?",
        "If you swing, pray you hit something other than your own shadow.",
        "This is the part where you apologize and back down.",
        "Lower that, or face a reckoning you won’t soon forget.",
        "The blade you hold might be sharp, but your mind clearly isn’t.",
        "What’s this? A challenge from a cub still wet behind the ears?",
        "Stand aside, or prepare to become another nameless corpse.",
        "That’s a bold move. Let’s see if it’s your last.",
        "I’ll carve your name into the annals of the defeated.",
        "Even the crows circling above find your threat amusing.",
        "Lower it, or I’ll show you the meaning of true steel.",
        "Are you sure that’s your weapon and not a borrowed toy?",
        "Is this how you honor your ancestors? By drawing on allies?",
        "That’s rich. A child threatening a veteran. Put it away.",
        "Do not mistake my patience for weakness, fool.",
        "Your challenge is heard. Let’s see if it’s worth my time.",
        "Step closer with that, and you’ll never step back.",
        "I respect your bravery, but your foolishness outweighs it.",
        "Careful with that. The blade might betray its master.",
        "You should have stayed home. This will not end well.",
        "Steel clashes against steel, but only one will sing victory.",
        "Are you truly ready to face the consequences of that action?",
        "I’ll remember this insult when you beg for mercy.",
        "Very well. Let’s see if your weapon matches your arrogance."
    };

    int index = std::rand() % responses.size();
    return responses[index];
}

