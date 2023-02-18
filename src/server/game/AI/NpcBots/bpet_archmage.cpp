#include "bot_ai.h"
#include "botspell.h"
#include "bpet_ai.h"
#include "Creature.h"
#include "ScriptMgr.h"
/*
Archmage NpcBot Pets (by Trickerer onlysuffering@gmail.com)
Complete - 100%
TODO:
*/

enum ArchmagePetBaseSpells
{
    WATERBOLT_1             = SPELL_WATERBOLT
};

enum ArchmagePetSpecial
{
    ELEMENTAL_DURATION      = 60000 //1 min
};

class archmage_pet_bot : public CreatureScript
{
public:
    archmage_pet_bot() : CreatureScript("archmage_pet_bot") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new awater_elemental_botpetAI(creature);
    }

    struct awater_elemental_botpetAI : public bot_pet_ai
    {
        awater_elemental_botpetAI(Creature* creature) : bot_pet_ai(creature) { }

        void JustEngagedWith(Unit* u) override { bot_pet_ai::JustEngagedWith(u); }
        void KilledUnit(Unit* u) override { bot_pet_ai::KilledUnit(u); }
        void EnterEvadeMode(EvadeReason why = EVADE_REASON_OTHER) override { bot_pet_ai::EnterEvadeMode(why); }
        void MoveInLineOfSight(Unit* u) override { bot_pet_ai::MoveInLineOfSight(u); }
        void JustDied(Unit* u) override { bot_pet_ai::JustDied(u); }
        void DoNonCombatActions(uint32 /*diff*/) { }

        void StartAttack(Unit* u, bool force = false)
        {
            if (!bot_pet_ai::StartAttack(u, force))
                return;
            GetInPosition(force, u);
        }

        void UpdateAI(uint32 diff) override
        {
            if ((liveTimer += diff) >= uint32(IAmFree() ? (1 * HOUR * IN_MILLISECONDS) : ELEMENTAL_DURATION))
            {
                canUpdate = false;
                me->setDeathState(JUST_DIED);
                return;
            }

            if (!GlobalUpdate(diff))
                return;

            if (!me->IsInCombat())
                DoNonCombatActions(diff);

            //DoPetActions(diff);
            //CheckDrainMana(diff);

            if (!CheckAttackTarget())
                return;

            if (IsCasting())
                return;

            DoPetAttack(diff);
        }

        void DoPetAttack(uint32 diff)
        {
            StartAttack(opponent, IsPetMelee());

            if (!petOwner->GetBotAI()->HasRole(BOT_ROLE_DPS))
                return;

            if (IsSpellReady(WATERBOLT_1, diff) && me->GetDistance(opponent) < 30)
            {
                me->CastSpell(opponent, GetSpell(WATERBOLT_1), false);
                return;
            }
        }

        void OnPetClassSpellGo(SpellInfo const* /*spellInfo*/) override
        {
        }

        void SpellHit(Unit* wcaster, SpellInfo const* spell) override
        {
            Unit* caster = wcaster->ToUnit();
            if (!caster)
                return;

            OnSpellHit(caster, spell);
        }

        void SpellHitTarget(Unit* /*wtarget*/, SpellInfo const* /*spell*/) override
        {
        }

        void DamageDealt(Unit* victim, uint32& damage, DamageEffectType damageType) override
        {
            bot_pet_ai::DamageDealt(victim, damage, damageType);
        }

        void DamageTaken(Unit* u, uint32& /*damage*/, DamageEffectType /*damageType*/, SpellSchoolMask /*schoolMask*/) override
        {
            if (!u)
                return;
            if (!u->IsInCombat() && !me->IsInCombat())
                return;
            OnOwnerDamagedBy(u);
        }

        void OwnerAttackedBy(Unit* u) override
        {
            if (!u)
                return;
            OnOwnerDamagedBy(u);
        }

        void Reset() override
        {
        }

        void InitPetSpells() override
        {
            InitSpellMap(WATERBOLT_1);
        }

        void ApplyPetPassives() const override
        {
        }

    private:
        uint32 liveTimer;
    };
};

void AddSC_archmage_bot_pets()
{
    new archmage_pet_bot();
}
