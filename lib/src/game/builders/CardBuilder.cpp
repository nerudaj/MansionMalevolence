#include "game/builders/CardBuilder.hpp"
#include "misc/Compatibility.hpp"

const std::map<CardType, Card> CARD_DEFS = {
    { CardType::Empty, Card { .image = CardImage::Max, .name = "--empty--" } },
    // Weapons
    { CardType::Pistol,
      Card { .image = CardImage::Pistol,
             .traits = CardTrait::Pickable | CardTrait::Weapon,
             .special = CardSpecial::Combines,
             .specialSound = SoundId::PistolFire,
             .quantity = 3,
             .power = 1,
             .link = SPECIAL_SILENCER,
             .name = "pistol" } },
    { CardType::SilencedPistol,
      Card { .image = CardImage::SilencedPistol,
             .traits = CardTrait::Pickable | CardTrait::Weapon,
             .special = CardSpecial::NegatesEvasive | CardSpecial::BoostsEvasion,
             .quantity = 3,
             .power = 1,
             .name = "silenced pistol",
             .texts = { "boosts your", "dodge chance,", "negates evasive", "monster ability." },} },
    { CardType::Shotgun,
      Card { .image = CardImage::Shotgun,
             .traits = CardTrait::Pickable | CardTrait::Weapon,
             .special = CardSpecial::NegatesEvasive | CardSpecial::CritChance,
             .specialSound = SoundId::ShotgunFire,
             .quantity = 2,
             .power = 2,
             .name = "shotgun", .texts={"chance to deal", "extra damage,", "negates evasive", "moster ability"}, } },
    { CardType::Crossbow,
      Card { .image = CardImage::Crossbow,
             .traits = CardTrait::Pickable | CardTrait::Weapon,
             .special = CardSpecial::RefillAmmoOnKill,
             .quantity = 2,
             .power = 2,
             .name = "crossbow", .texts = {"refills 1 ammo", "when moster is", "killed."}, } },
    { CardType::RocketLauncher,
      Card { .image = CardImage::RocketLauncher,
             .traits = CardTrait::Pickable | CardTrait::Weapon,
             .quantity = 1,
             .power = 3,
             .name = "rocket launcher" } },
    { CardType::Ammo,
      Card { .image = CardImage::Ammo,
             .traits = CardTrait::Pickable | CardTrait::Ammo,
             .specialSound = SoundId::WeaponReload,
             .quantity = 3,
             .name = "ammo",
             .texts = {
                "restores ammo",
                "for a chosen",
                "weapon."} } },
    { CardType::PistolParts,
      Card { .image = CardImage::PistolParts,
             .traits = CardTrait::Pickable,
             .special = CardSpecial::Combines,
             .link = SPECIAL_SILENCER,
             .name = "ammo",
             .texts = {
                "upgrade your", "pistol with", "these parts"}, } },

    // Healing
    { CardType::GreenHerb,
      Card { .image = CardImage::GreenHerb,
             .traits = CardTrait::Pickable | CardTrait::Healing,
             .special = CardSpecial::Combines,
             .specialSound = SoundId::Heal,
             .power = 2,
             .link = SPECIAL_HERBS,
             .name = "green herb",
             .texts = {"restores portion","of your health.","try mixing with","red herb."} } },
    { CardType::RedHerb,
      Card { .image = CardImage::RedHerb,
             .traits = CardTrait::Pickable | CardTrait::Healing,
             .special = CardSpecial::Combines,
             .specialSound = SoundId::Heal,
             .power = 1,
             .link = SPECIAL_HERBS,
             .name = "red herb",
             .texts = {"restores portion","of your health.","try mixing with","green herb."} } },
    { CardType::MixedHerbs,
      Card { .image = CardImage::MixedHerbs,
             .traits = CardTrait::Pickable | CardTrait::Healing,
             .specialSound = SoundId::Heal,
             .power = 5,
             .name = "mixed herbs",
             .texts = {"restores to full","health."} } },
    { CardType::FirstAid,
      Card { .image = CardImage::FirstAid,
             .traits = CardTrait::Pickable | CardTrait::Healing,
             .specialSound = SoundId::Heal,
             .power = 5,
             .name = "first-aid spray",
             .texts = {"restores to full","health."} } },
    { CardType::Vaccine,
      Card {
          .image = CardImage::Vaccine,
          .traits = CardTrait::Pickable | CardTrait::Healing,
          .special = CardSpecial::WinGame,
          .specialSound = SoundId::Heal,
          .link = SPECIAL_YELLOW_JEWEL,
          .name = "vaccine",.texts = { "use this to cure", "the infection." } } },

    // Items
    { CardType::ShieldDoor,
      Card { .image = CardImage::ShieldDoor,
             .traits = CardTrait::KeyTarget,
             .specialSound = SoundId::DoorUnlock,
             .link = SPECIAL_SHIELD_KEYDOOR,
             .name = "door",
             .texts = {"there's a spade","symbol above","the keyhole."} } },
    { CardType::ShieldKey,
      Card { .image = CardImage::ShieldKey,
             .traits = CardTrait::Pickable | CardTrait::KeyItem,
             .link = SPECIAL_SHIELD_KEYDOOR,
             .name = "spade key",
             .texts = {"key with a","spade symbol."} } },
    { CardType::DiamondDoor,
      Card { .image = CardImage::DiamondDoor,
             .traits = CardTrait::KeyTarget,
             .specialSound = SoundId::DoorUnlock,
             .link = SPECIAL_DIAMOND_KEYDOOR,
             .name = "door",
             .texts = {"there is a","diamond symbol","above the","keyhole."} } },
    { CardType::DiamondKey,
      Card { .image = CardImage::DiamondKey,
             .traits = CardTrait::Pickable | CardTrait::KeyItem,
             .link = SPECIAL_DIAMOND_KEYDOOR,
             .name = "diamond key",
             .texts = {"key with a","diamond symbol."} } },
    { CardType::MoonCrestLeft,
      Card { .image = CardImage::MoonCrestLeft,
             .traits = CardTrait::Pickable | CardTrait::KeyItemPart,
             .special = CardSpecial::Combines,
             .link = SPECIAL_MOON_CREST_PART,
             .name = "moon crest (l)",
             .texts = {"combine with the", "other half."} } },
    { CardType::MoonCrestRight,
      Card { .image = CardImage::MoonCrestRight,
             .traits = CardTrait::Pickable | CardTrait::KeyItemPart,
             .special = CardSpecial::Combines,
             .link = SPECIAL_MOON_CREST_PART,
             .name = "moon crest (r)",
             .texts = {"combine with the", "other half."} } },
    { CardType::MoonCrest,
      Card { .image = CardImage::MoonCrest,
             .traits = CardTrait::Pickable | CardTrait::KeyItem,
             .link = SPECIAL_CREST_DOOR,
             .name = "moon crest",
             .texts = {
                "hexagonal crest",
                "with an image of",
                "a half moon." } } },
    { CardType::SunCrest,
      Card { .image = CardImage::SunCrest,
             .traits = CardTrait::Pickable | CardTrait::KeyItem,
             .link = SPECIAL_CREST_DOOR,
             .name = "sun crest",
             .texts = {
                "hexagonal crest",
                "with an image of",
                "a sun." } } },
    { CardType::CrestDoorEmpty,
      Card { .image = CardImage::CrestDoorEmpty,
             .traits = CardTrait::KeyTarget,
             .link = SPECIAL_CREST_DOOR,
             .name = "sun/moon door",
             .texts = {
                 "there are slots",
                 "for two hex-",
                 "shaped crests."
             } } },
    { CardType::CrestDoorWithOneCrest,
      Card {
          .image = CardImage::CrestDoorWithOneCrest,
          .traits = CardTrait::KeyTarget,
          .specialSound = SoundId::DoorUnlock,
          .link = SPECIAL_CREST_DOOR,
          .name = "sun/moon door",
          .texts = {
              "there is a slot",
              "for one more",
              "crest.",
          } } },
    { CardType::RedJewel,
      Card {
          .image = CardImage::RedJewel,
          .traits = CardTrait::KeyItem | CardTrait::Pickable,
          .link = SPECIAL_RED_JEWEL,
          .name = "red jewel",
      } },
    { CardType::RedJewelBox,
      Card {
          .image = CardImage::RedJewelBox,
          .traits = CardTrait::KeyTarget,
          .link = SPECIAL_RED_JEWEL,
          .name = "red jewel box",
          .texts = {
              "there is a slot",
              "for a big jewel.",
          } } },
    { CardType::YellowJewel,
      Card {
          .image = CardImage::YellowJewel,
          .traits = CardTrait::KeyItem | CardTrait::Pickable,
          .link = SPECIAL_YELLOW_JEWEL,
          .name = "yellow jewel",
      } },
    { CardType::YellowJewelBox,
      Card {
          .image = CardImage::YellowJewelBox,
          .traits = CardTrait::KeyTarget,
          .link = SPECIAL_YELLOW_JEWEL,
          .name = "yellow jewel box",
          .texts = {
              "there is a slot",
              "for a big jewel.",
          } } },
    { CardType::WeaponLockerKey,
      Card {
          .image = CardImage::WeaponLockerKey,
          .traits = CardTrait::KeyItem | CardTrait::Pickable,
          .link = SPECIAL_LOCKER_KEY,
          .name = "weapon box key",
      } },
    { CardType::LockedWeaponLocker,
      Card {
          .image = CardImage::LockedWeaponLocker,
          .traits = CardTrait::KeyTarget,
          .link = SPECIAL_LOCKER_KEY,
          .name = "locked wpn. box",
      } },

    // Enemies
    { CardType::Zombie,
      Card { .image = CardImage::Zombie,
             .traits = CardTrait::Enemy,
             .special = CardSpecial::SpawnCrimsonHead,
             .specialSound = SoundId::ZombieGrowl,
             .power = 1,
             .name = "zombie",
             .texts = {
                 "on killed:",
                 "chance to spawn",
                 "a crimson head."
             } } },
    { CardType::CrimsonHead,
      Card { .image = CardImage::CrimsonHead,
             .traits = CardTrait::Enemy,
             .special = CardSpecial::Vigilant,
             .specialSound = SoundId::CrimsonHeadGrowl,
             .power = 2,
             .name = "crimson head",
             .texts = {
                 "vigilant:",
                 "decreases your",
                 "chance to evade."
             } } },
    { CardType::Licker,
      Card { .image = CardImage::Licker,
             .traits = CardTrait::Enemy,
             .special = CardSpecial::Blind,
             .specialSound = SoundId::LickerGrowl,
             .power = 3,
             .name = "licker",
             .texts = {
                 "blind:",
                 "increases your",
                 "chance to evade."
             } } },
    { CardType::Cerberus,
      Card { .image = CardImage::Cerberus,
             .traits = CardTrait::Enemy,
             .special = CardSpecial::Evasive | CardSpecial::Vigilant,
             .specialSound = SoundId::DogGrowl,
             .power = 1,
             .name = "cerberus",
             .texts = {
                 "evasive,vigilant",
                 "decreases your",
                 "chance to hit",
                 "and evade."
             } } },
    { CardType::Tyrant,
      Card { .image = CardImage::Tyrant,
             .traits = CardTrait::Enemy,
             .special = CardSpecial::Retaliate | CardSpecial::Vigilant,
             .specialSound = SoundId::TyrantGrowl,
             .power = 5,
             .name = "tyrant t-002",
             .texts = {
                 "retaliate",
                 "",
                 "vigilant",
                 "",
             } } },

    // Misc
    { CardType::Crate,
      Card { .image = CardImage::Crate,
             .special = CardSpecial::BoosterPack,
             .specialSound = SoundId::CrateOpen,
             .name = "crate",
             .texts = {
                 "take this card",
                 "so you can pick",
                 "one out of three",
                 "support cards.",
             } } },
    { CardType::UnlockedWeaponLocker,
      Card { .image = CardImage::WeaponLocker,
             .special = CardSpecial::BoosterPack,
             .specialSound = SoundId::CrateOpen,
             .name = "weapon box",
             .texts = {
                 "take this card",
                 "so you can pick",
                 "one out of three",
                 "weapon cards.",
             } } },
};

Card CardBuilder::createCard(CardType type)
{
    return CARD_DEFS.at(type);
}

Card CardBuilder::combineCards(const Card& a, const Card& b)
{
    const bool isValid = a.special & CardSpecial::Combines
                         && b.special & CardSpecial::Combines
                         && a.link == b.link;
    if (!isValid)
    {
        throw std::runtime_error(uni::format(
            "Trying to combine invalid cards! {}, {}",
            std::to_underlying(a.image),
            std::to_underlying(b.image)));
    }

    if (a.link == SPECIAL_HERBS)
        return createCard(CardType::MixedHerbs);
    else if (a.link == SPECIAL_MOON_CREST_PART)
        return createCard(CardType::MoonCrest);
    else if (a.link == SPECIAL_SILENCER)
    {
        assert(a.traits & CardTrait::Weapon);
        auto card = createCard(CardType::SilencedPistol);
        card.quantity = a.quantity;
        return card;
    }

    return createCard(CardType::Empty);
}
