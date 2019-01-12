#pragma once
#include <IApp.h>
#include "Sprite.h"
#include "data/Item.h"
#include <objects/Scene.h>
#include <functional>

class SpriteApp : public blink::IApp
{
public:
    typedef std::map<int, quicktype::Item> ItemMap;
    typedef std::map<int, int> IntMap;

public:
    RTTI_DEF(SpriteApp, blink::IApp);

    SpriteApp();
    virtual ~SpriteApp();

    virtual bool initialize() override;
    virtual void terminate() override;

    virtual void update(float dt) override;
    virtual void render() override;

private:
    virtual blink::RenderModule* createRenderer() override;
    void drawCombo(const char* label, int selectedId, const IntMap& armorMap, const std::function<void(int)>& selectCb);
    const char* getItemNameFromArmorMap(const IntMap& armorMap, int armorId);
    void drawItemsMap();

    bool parseItems(ItemMap& itemMap, const blink::tstring& filePath);

private:
    blink::Scene* m_rootScene{};
    blink::Camera* m_camera{};

    Sprite* m_sprite{};

    ItemMap m_itemsMap;
    int m_selectedItemId{};

    // armor
    IntMap m_headItemMap;
    IntMap m_bodyItemMap;
    IntMap m_legItemMap;

    IntMap m_handsOffMap;
    IntMap m_handsOnMap;
    IntMap m_backMap;
    IntMap m_frontMap;
    IntMap m_shoeMap;
    IntMap m_waistMap;
    IntMap m_wingMap;
    IntMap m_shieldMap;
    IntMap m_neckMap;
    IntMap m_faceMap;
    IntMap m_balloonMap;

    // weapon
    IntMap m_weaponMap;

};
