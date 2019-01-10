//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Item data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

namespace quicktype {
    using nlohmann::json;

    //inline json get_untyped(const json & j, const char * property) {
    //    if (j.find(property) != j.end()) {
    //        return j.at(property).get<json>();
    //    }
    //    return json();
    //}

    //inline json get_untyped(const json & j, std::string property) {
    //    return get_untyped(j, property.data());
    //}

    struct ToolTip {
        int lines;
    };

    struct UseSound {
        int style;
        int variations;
        int soundId;
        bool isTrackable;
        int volume;
        int pitchVariance;
        int type;
    };

    struct Item {
        bool questItem;
        bool flame;
        bool mech;
        int noGrabDelay;
        bool beingGrabbed;
        bool isBeingGrabbed;
        int spawnTime;
        int tileWand;
        bool wornArmor;
        int dye;
        int fishingPole;
        int bait;
        int makeNpc;
        bool expertOnly;
        bool expert;
        int hairDye;
        int paint;
        bool instanced;
        int ownIgnore;
        int ownTime;
        int keepTime;
        int type;
        bool favorited;
        int holdStyle;
        int useStyle;
        bool channel;
        bool accessory;
        int useAnimation;
        int useTime;
        int stack;
        int maxStack;
        int pick;
        int axe;
        int hammer;
        int tileBoost;
        int createTile;
        int createWall;
        int placeStyle;
        int damage;
        int knockBack;
        int healLife;
        int healMana;
        bool potion;
        bool consumable;
        bool autoReuse;
        bool useTurn;
        std::string color;
        int alpha;
        int glowMask;
        int scale;
        UseSound useSound;
        int defense;
        int headSlot;
        int bodySlot;
        int legSlot;
        int handOnSlot;
        int handOffSlot;
        int backSlot;
        int frontSlot;
        int shoeSlot;
        int waistSlot;
        int wingSlot;
        int shieldSlot;
        int neckSlot;
        int faceSlot;
        int balloonSlot;
        int stringColor;
        ToolTip toolTip;
        int owner;
        int rare;
        int shoot;
        int shootSpeed;
        int ammo;
        bool notAmmo;
        int useAmmo;
        int lifeRegen;
        int manaIncrease;
        bool buyOnce;
        int mana;
        bool noUseGraphic;
        bool noMelee;
        int release;
        int value;
        bool buy;
        bool social;
        bool vanity;
        bool material;
        bool noWet;
        int buffType;
        int buffTime;
        int mountType;
        bool cartTrack;
        bool uniqueStack;
        int shopSpecialCurrency;
        //nlohmann::json shopCustomPrice;
        bool dd2Summon;
        int netId;
        int crit;
        int prefix;
        bool melee;
        bool magic;
        bool ranged;
        bool thrown;
        bool summon;
        bool sentry;
        int reuseDelay;
        bool newAndShiny;
        int whoAmI;
        bool active;
        std::string position;
        std::string velocity;
        std::string oldPosition;
        std::string oldVelocity;
        int oldDirection;
        int direction;
        int width;
        int height;
        bool wet;
        bool honeyWet;
        int wetCount;
        bool lavaWet;
        std::string name;
        std::string hoverName;
        bool isAir;
        std::string center;
        std::string left;
        std::string right;
        std::string top;
        std::string topLeft;
        std::string topRight;
        std::string bottom;
        std::string bottomLeft;
        std::string bottomRight;
        std::string size;
        std::string hitbox;
    };

    void from_json(const json & j, quicktype::ToolTip & x);
    void to_json(json & j, const quicktype::ToolTip & x);

    void from_json(const json & j, quicktype::UseSound & x);
    void to_json(json & j, const quicktype::UseSound & x);

    void from_json(const json & j, quicktype::Item & x);
    void to_json(json & j, const quicktype::Item & x);

    inline void from_json(const json & j, quicktype::ToolTip& x) {
        x.lines = j.at("Lines").get<int>();
    }

    inline void to_json(json & j, const quicktype::ToolTip & x) {
        j = json::object();
        j["Lines"] = x.lines;
    }

    inline void from_json(const json & j, quicktype::UseSound& x) {
        if (j.is_null())
        {
            x.style = 0;
            x.variations = 0;
            x.soundId = 0;
            x.isTrackable = false;
            x.volume = 0;
            x.pitchVariance = 0;
            x.type = 0;
        }
        else
        {
            x.style = j.at("Style").get<int>();
            x.variations = j.at("Variations").get<int>();
            x.soundId = j.at("SoundId").get<int>();
            x.isTrackable = j.at("IsTrackable").get<bool>();
            x.volume = j.at("Volume").get<int>();
            x.pitchVariance = j.at("PitchVariance").get<int>();
            x.type = j.at("Type").get<int>();
        }
    }

    inline void to_json(json & j, const quicktype::UseSound & x) {
        j = json::object();
        j["Style"] = x.style;
        j["Variations"] = x.variations;
        j["SoundId"] = x.soundId;
        j["IsTrackable"] = x.isTrackable;
        j["Volume"] = x.volume;
        j["PitchVariance"] = x.pitchVariance;
        j["Type"] = x.type;
    }

    inline void from_json(const json & j, quicktype::Item& x) {
        x.questItem = j.at("questItem").get<bool>();
        x.flame = j.at("flame").get<bool>();
        x.mech = j.at("mech").get<bool>();
        x.noGrabDelay = j.at("noGrabDelay").get<int>();
        x.beingGrabbed = j.at("beingGrabbed").get<bool>();
        x.isBeingGrabbed = j.at("isBeingGrabbed").get<bool>();
        x.spawnTime = j.at("spawnTime").get<int>();
        x.tileWand = j.at("tileWand").get<int>();
        x.wornArmor = j.at("wornArmor").get<bool>();
        x.dye = j.at("dye").get<int>();
        x.fishingPole = j.at("fishingPole").get<int>();
        x.bait = j.at("bait").get<int>();
        x.makeNpc = j.at("makeNPC").get<int>();
        x.expertOnly = j.at("expertOnly").get<bool>();
        x.expert = j.at("expert").get<bool>();
        x.hairDye = j.at("hairDye").get<int>();
        x.paint = j.at("paint").get<int>();
        x.instanced = j.at("instanced").get<bool>();
        x.ownIgnore = j.at("ownIgnore").get<int>();
        x.ownTime = j.at("ownTime").get<int>();
        x.keepTime = j.at("keepTime").get<int>();
        x.type = j.at("type").get<int>();
        x.favorited = j.at("favorited").get<bool>();
        x.holdStyle = j.at("holdStyle").get<int>();
        x.useStyle = j.at("useStyle").get<int>();
        x.channel = j.at("channel").get<bool>();
        x.accessory = j.at("accessory").get<bool>();
        x.useAnimation = j.at("useAnimation").get<int>();
        x.useTime = j.at("useTime").get<int>();
        x.stack = j.at("stack").get<int>();
        x.maxStack = j.at("maxStack").get<int>();
        x.pick = j.at("pick").get<int>();
        x.axe = j.at("axe").get<int>();
        x.hammer = j.at("hammer").get<int>();
        x.tileBoost = j.at("tileBoost").get<int>();
        x.createTile = j.at("createTile").get<int>();
        x.createWall = j.at("createWall").get<int>();
        x.placeStyle = j.at("placeStyle").get<int>();
        x.damage = j.at("damage").get<int>();
        x.knockBack = j.at("knockBack").get<int>();
        x.healLife = j.at("healLife").get<int>();
        x.healMana = j.at("healMana").get<int>();
        x.potion = j.at("potion").get<bool>();
        x.consumable = j.at("consumable").get<bool>();
        x.autoReuse = j.at("autoReuse").get<bool>();
        x.useTurn = j.at("useTurn").get<bool>();
        x.color = j.at("color").get<std::string>();
        x.alpha = j.at("alpha").get<int>();
        x.glowMask = j.at("glowMask").get<int>();
        x.scale = j.at("scale").get<int>();
        x.useSound = j.at("UseSound").get<quicktype::UseSound>();
        x.defense = j.at("defense").get<int>();
        x.headSlot = j.at("headSlot").get<int>();
        x.bodySlot = j.at("bodySlot").get<int>();
        x.legSlot = j.at("legSlot").get<int>();
        x.handOnSlot = j.at("handOnSlot").get<int>();
        x.handOffSlot = j.at("handOffSlot").get<int>();
        x.backSlot = j.at("backSlot").get<int>();
        x.frontSlot = j.at("frontSlot").get<int>();
        x.shoeSlot = j.at("shoeSlot").get<int>();
        x.waistSlot = j.at("waistSlot").get<int>();
        x.wingSlot = j.at("wingSlot").get<int>();
        x.shieldSlot = j.at("shieldSlot").get<int>();
        x.neckSlot = j.at("neckSlot").get<int>();
        x.faceSlot = j.at("faceSlot").get<int>();
        x.balloonSlot = j.at("balloonSlot").get<int>();
        x.stringColor = j.at("stringColor").get<int>();
        x.toolTip = j.at("ToolTip").get<quicktype::ToolTip>();
        x.owner = j.at("owner").get<int>();
        x.rare = j.at("rare").get<int>();
        x.shoot = j.at("shoot").get<int>();
        x.shootSpeed = j.at("shootSpeed").get<int>();
        x.ammo = j.at("ammo").get<int>();
        x.notAmmo = j.at("notAmmo").get<bool>();
        x.useAmmo = j.at("useAmmo").get<int>();
        x.lifeRegen = j.at("lifeRegen").get<int>();
        x.manaIncrease = j.at("manaIncrease").get<int>();
        x.buyOnce = j.at("buyOnce").get<bool>();
        x.mana = j.at("mana").get<int>();
        x.noUseGraphic = j.at("noUseGraphic").get<bool>();
        x.noMelee = j.at("noMelee").get<bool>();
        x.release = j.at("release").get<int>();
        x.value = j.at("value").get<int>();
        x.buy = j.at("buy").get<bool>();
        x.social = j.at("social").get<bool>();
        x.vanity = j.at("vanity").get<bool>();
        x.material = j.at("material").get<bool>();
        x.noWet = j.at("noWet").get<bool>();
        x.buffType = j.at("buffType").get<int>();
        x.buffTime = j.at("buffTime").get<int>();
        x.mountType = j.at("mountType").get<int>();
        x.cartTrack = j.at("cartTrack").get<bool>();
        x.uniqueStack = j.at("uniqueStack").get<bool>();
        x.shopSpecialCurrency = j.at("shopSpecialCurrency").get<int>();
        //x.shopCustomPrice = quicktype::get_untyped(j, "shopCustomPrice");
        x.dd2Summon = j.at("DD2Summon").get<bool>();
        x.netId = j.at("netID").get<int>();
        x.crit = j.at("crit").get<int>();
        x.prefix = j.at("prefix").get<int>();
        x.melee = j.at("melee").get<bool>();
        x.magic = j.at("magic").get<bool>();
        x.ranged = j.at("ranged").get<bool>();
        x.thrown = j.at("thrown").get<bool>();
        x.summon = j.at("summon").get<bool>();
        x.sentry = j.at("sentry").get<bool>();
        x.reuseDelay = j.at("reuseDelay").get<int>();
        x.newAndShiny = j.at("newAndShiny").get<bool>();
        x.whoAmI = j.at("whoAmI").get<int>();
        x.active = j.at("active").get<bool>();
        x.position = j.at("position").get<std::string>();
        x.velocity = j.at("velocity").get<std::string>();
        x.oldPosition = j.at("oldPosition").get<std::string>();
        x.oldVelocity = j.at("oldVelocity").get<std::string>();
        x.oldDirection = j.at("oldDirection").get<int>();
        x.direction = j.at("direction").get<int>();
        x.width = j.at("width").get<int>();
        x.height = j.at("height").get<int>();
        x.wet = j.at("wet").get<bool>();
        x.honeyWet = j.at("honeyWet").get<bool>();
        x.wetCount = j.at("wetCount").get<int>();
        x.lavaWet = j.at("lavaWet").get<bool>();
        x.name = j.at("Name").get<std::string>();
        x.hoverName = j.at("HoverName").get<std::string>();
        x.isAir = j.at("IsAir").get<bool>();
        x.center = j.at("Center").get<std::string>();
        x.left = j.at("Left").get<std::string>();
        x.right = j.at("Right").get<std::string>();
        x.top = j.at("Top").get<std::string>();
        x.topLeft = j.at("TopLeft").get<std::string>();
        x.topRight = j.at("TopRight").get<std::string>();
        x.bottom = j.at("Bottom").get<std::string>();
        x.bottomLeft = j.at("BottomLeft").get<std::string>();
        x.bottomRight = j.at("BottomRight").get<std::string>();
        x.size = j.at("Size").get<std::string>();
        x.hitbox = j.at("Hitbox").get<std::string>();
    }

    inline void to_json(json & j, const quicktype::Item & x) {
        j = json::object();
        j["questItem"] = x.questItem;
        j["flame"] = x.flame;
        j["mech"] = x.mech;
        j["noGrabDelay"] = x.noGrabDelay;
        j["beingGrabbed"] = x.beingGrabbed;
        j["isBeingGrabbed"] = x.isBeingGrabbed;
        j["spawnTime"] = x.spawnTime;
        j["tileWand"] = x.tileWand;
        j["wornArmor"] = x.wornArmor;
        j["dye"] = x.dye;
        j["fishingPole"] = x.fishingPole;
        j["bait"] = x.bait;
        j["makeNPC"] = x.makeNpc;
        j["expertOnly"] = x.expertOnly;
        j["expert"] = x.expert;
        j["hairDye"] = x.hairDye;
        j["paint"] = x.paint;
        j["instanced"] = x.instanced;
        j["ownIgnore"] = x.ownIgnore;
        j["ownTime"] = x.ownTime;
        j["keepTime"] = x.keepTime;
        j["type"] = x.type;
        j["favorited"] = x.favorited;
        j["holdStyle"] = x.holdStyle;
        j["useStyle"] = x.useStyle;
        j["channel"] = x.channel;
        j["accessory"] = x.accessory;
        j["useAnimation"] = x.useAnimation;
        j["useTime"] = x.useTime;
        j["stack"] = x.stack;
        j["maxStack"] = x.maxStack;
        j["pick"] = x.pick;
        j["axe"] = x.axe;
        j["hammer"] = x.hammer;
        j["tileBoost"] = x.tileBoost;
        j["createTile"] = x.createTile;
        j["createWall"] = x.createWall;
        j["placeStyle"] = x.placeStyle;
        j["damage"] = x.damage;
        j["knockBack"] = x.knockBack;
        j["healLife"] = x.healLife;
        j["healMana"] = x.healMana;
        j["potion"] = x.potion;
        j["consumable"] = x.consumable;
        j["autoReuse"] = x.autoReuse;
        j["useTurn"] = x.useTurn;
        j["color"] = x.color;
        j["alpha"] = x.alpha;
        j["glowMask"] = x.glowMask;
        j["scale"] = x.scale;
        j["UseSound"] = x.useSound;
        j["defense"] = x.defense;
        j["headSlot"] = x.headSlot;
        j["bodySlot"] = x.bodySlot;
        j["legSlot"] = x.legSlot;
        j["handOnSlot"] = x.handOnSlot;
        j["handOffSlot"] = x.handOffSlot;
        j["backSlot"] = x.backSlot;
        j["frontSlot"] = x.frontSlot;
        j["shoeSlot"] = x.shoeSlot;
        j["waistSlot"] = x.waistSlot;
        j["wingSlot"] = x.wingSlot;
        j["shieldSlot"] = x.shieldSlot;
        j["neckSlot"] = x.neckSlot;
        j["faceSlot"] = x.faceSlot;
        j["balloonSlot"] = x.balloonSlot;
        j["stringColor"] = x.stringColor;
        j["ToolTip"] = x.toolTip;
        j["owner"] = x.owner;
        j["rare"] = x.rare;
        j["shoot"] = x.shoot;
        j["shootSpeed"] = x.shootSpeed;
        j["ammo"] = x.ammo;
        j["notAmmo"] = x.notAmmo;
        j["useAmmo"] = x.useAmmo;
        j["lifeRegen"] = x.lifeRegen;
        j["manaIncrease"] = x.manaIncrease;
        j["buyOnce"] = x.buyOnce;
        j["mana"] = x.mana;
        j["noUseGraphic"] = x.noUseGraphic;
        j["noMelee"] = x.noMelee;
        j["release"] = x.release;
        j["value"] = x.value;
        j["buy"] = x.buy;
        j["social"] = x.social;
        j["vanity"] = x.vanity;
        j["material"] = x.material;
        j["noWet"] = x.noWet;
        j["buffType"] = x.buffType;
        j["buffTime"] = x.buffTime;
        j["mountType"] = x.mountType;
        j["cartTrack"] = x.cartTrack;
        j["uniqueStack"] = x.uniqueStack;
        j["shopSpecialCurrency"] = x.shopSpecialCurrency;
        //j["shopCustomPrice"] = x.shopCustomPrice;
        j["DD2Summon"] = x.dd2Summon;
        j["netID"] = x.netId;
        j["crit"] = x.crit;
        j["prefix"] = x.prefix;
        j["melee"] = x.melee;
        j["magic"] = x.magic;
        j["ranged"] = x.ranged;
        j["thrown"] = x.thrown;
        j["summon"] = x.summon;
        j["sentry"] = x.sentry;
        j["reuseDelay"] = x.reuseDelay;
        j["newAndShiny"] = x.newAndShiny;
        j["whoAmI"] = x.whoAmI;
        j["active"] = x.active;
        j["position"] = x.position;
        j["velocity"] = x.velocity;
        j["oldPosition"] = x.oldPosition;
        j["oldVelocity"] = x.oldVelocity;
        j["oldDirection"] = x.oldDirection;
        j["direction"] = x.direction;
        j["width"] = x.width;
        j["height"] = x.height;
        j["wet"] = x.wet;
        j["honeyWet"] = x.honeyWet;
        j["wetCount"] = x.wetCount;
        j["lavaWet"] = x.lavaWet;
        j["Name"] = x.name;
        j["HoverName"] = x.hoverName;
        j["IsAir"] = x.isAir;
        j["Center"] = x.center;
        j["Left"] = x.left;
        j["Right"] = x.right;
        j["Top"] = x.top;
        j["TopLeft"] = x.topLeft;
        j["TopRight"] = x.topRight;
        j["Bottom"] = x.bottom;
        j["BottomLeft"] = x.bottomLeft;
        j["BottomRight"] = x.bottomRight;
        j["Size"] = x.size;
        j["Hitbox"] = x.hitbox;
    }
}
