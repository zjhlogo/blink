#include "SpriteApp.h"
#include "SpriteRenderModule.h"
#include <Framework.h>
#include <cameras/FixCamera.h>
#include <ui/UiModule.h>
#include <imgui.h>
#include <fmt/format.h>

#include <fstream>

SpriteApp::SpriteApp()
    :IApp(1280, 720, "Sprite")
{

}

SpriteApp::~SpriteApp()
{
}

bool SpriteApp::initialize()
{
    m_rootScene = new blink::Scene();

    m_sprite = new Sprite();
    m_rootScene->add(m_sprite);

    auto camera = new blink::FixCamera();
    camera->useDefaultProjection(blink::Camera::DefaultProjection::Orthographic);
    camera->lookAt({ 0.0f, 0.0f, 3.0f }, blink::VEC3_ZERO, blink::VEC3_PY);
    m_camera = camera;

    // read json from file
    parseItems(m_itemsMap, "resource/items.json");

    return true;
}

void SpriteApp::terminate()
{
    SAFE_DELETE(m_camera);
    SAFE_DELETE(m_rootScene);
}

void SpriteApp::update(float dt)
{
    m_sprite->update(dt);
    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void SpriteApp::render()
{
    renderObject(m_rootScene, m_camera);

    auto pUiModule = blink::Framework::getInstance().findComponent<blink::UiModule>();
    if (!pUiModule) return;

    pUiModule->begin();

    ImGui::Checkbox("male", &m_sprite->m_male);
    drawCombo("head", m_sprite->m_headId, m_headItemMap, [&](int id) { m_sprite->m_headId = id; });
    drawCombo("body", m_sprite->m_bodyId, m_bodyItemMap, [&](int id) { m_sprite->m_bodyId = id; m_sprite->m_armId = id; });
    drawCombo("leg", m_sprite->m_legId, m_legItemMap, [&](int id) { m_sprite->m_legId = id; });

    drawCombo("hands off id", m_sprite->m_handsOffId, m_handsOffMap, [&](int id) { m_sprite->m_handsOffId = id; });
    drawCombo("hands on id", m_sprite->m_handsOnId, m_handsOnMap, [&](int id) { m_sprite->m_handsOnId = id; });

    drawCombo("back", m_sprite->m_backId, m_backMap, [&](int id) { m_sprite->m_backId = id; });
    drawCombo("front", m_sprite->m_frontId, m_frontMap, [&](int id) { m_sprite->m_frontId = id; });
    drawCombo("shoe", m_sprite->m_shoeId, m_shoeMap, [&](int id) { m_sprite->m_shoeId = id; });
    drawCombo("waist", m_sprite->m_waistId, m_waistMap, [&](int id) { m_sprite->m_waistId = id; });
    drawCombo("wing", m_sprite->m_wingId, m_wingMap, [&](int id) { m_sprite->m_wingId = id; });
    drawCombo("shield", m_sprite->m_shieldId, m_shieldMap, [&](int id) { m_sprite->m_shieldId = id; });
    drawCombo("neck", m_sprite->m_neckId, m_neckMap, [&](int id) { m_sprite->m_neckId = id; });
    drawCombo("face", m_sprite->m_faceId, m_faceMap, [&](int id) { m_sprite->m_faceId = id; });
    drawCombo("balloon", m_sprite->m_balloonId, m_balloonMap, [&](int id) { m_sprite->m_balloonId = id; });

    drawCombo("weapon", m_sprite->m_weaponId, m_weaponMap, [&](int id) { m_sprite->m_weaponId = id; });
    //drawItemsMap();

    pUiModule->end();
}

blink::RenderModule * SpriteApp::createRenderer()
{
    return new SpriteRenderModule();
}

void SpriteApp::drawCombo(const char * label, int selectedId, const IntMap& armorMap, const std::function<void(int)>& selectCb)
{
    if (ImGui::BeginCombo(label, getItemNameFromArmorMap(armorMap, selectedId)))
    {
        for (auto it : armorMap)
        {
            bool isSelected = (selectedId == it.first);
            if (ImGui::Selectable(getItemNameFromArmorMap(armorMap, it.first), isSelected))
            {
                // selection changed
                selectCb(it.first);
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
        }

        ImGui::EndCombo();
    }
}

const char * SpriteApp::getItemNameFromArmorMap(const IntMap & armorMap, int armorId)
{
    if (armorId <= 0) return "Empty";

    auto it = armorMap.find(armorId);
    if (it == armorMap.end()) return "Empty";

    auto itItem = m_itemsMap.find(it->second);
    if (itItem == m_itemsMap.end()) return "Empty";

    return itItem->second.name.c_str();
}

void SpriteApp::drawItemsMap()
{
    auto itSelected = m_itemsMap.find(m_selectedItemId);
    if (ImGui::BeginCombo("items", itSelected->second.name.c_str()))
    {
        for (const auto& it : m_itemsMap)
        {
            bool isSelected = (m_selectedItemId == it.first);
            if (ImGui::Selectable(it.second.name.c_str(), isSelected))
            {
                // selection changed
                m_selectedItemId = it.first;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
        }

        ImGui::EndCombo();
    }
}

bool SpriteApp::parseItems(ItemMap& itemMap, const blink::tstring & filePath)
{
    nlohmann::json j;
    {
        std::ifstream file(filePath);
        file >> j;
    }

    m_handsOffMap.insert(std::make_pair(0, 0));
    m_handsOnMap.insert(std::make_pair(0, 0));
    m_backMap.insert(std::make_pair(0, 0));
    m_frontMap.insert(std::make_pair(0, 0));
    m_shoeMap.insert(std::make_pair(0, 0));
    m_waistMap.insert(std::make_pair(0, 0));
    m_wingMap.insert(std::make_pair(0, 0));
    m_shieldMap.insert(std::make_pair(0, 0));
    m_neckMap.insert(std::make_pair(0, 0));
    m_faceMap.insert(std::make_pair(0, 0));
    m_balloonMap.insert(std::make_pair(0, 0));

    for (auto it = j.begin(); it != j.end(); ++it)
    {
        quicktype::Item item = (*it).get<quicktype::Item>();

        if (item.headSlot != -1) m_headItemMap.insert(std::make_pair(item.headSlot, item.type));
        if (item.bodySlot != -1) m_bodyItemMap.insert(std::make_pair(item.bodySlot, item.type));
        if (item.legSlot != -1) m_legItemMap.insert(std::make_pair(item.legSlot, item.type));
        if (item.handOffSlot != -1) m_handsOffMap.insert(std::make_pair(item.handOffSlot, item.type));
        if (item.handOnSlot != -1) m_handsOnMap.insert(std::make_pair(item.handOnSlot, item.type));
        if (item.backSlot != -1) m_backMap.insert(std::make_pair(item.backSlot, item.type));
        if (item.frontSlot != -1) m_frontMap.insert(std::make_pair(item.frontSlot, item.type));
        if (item.shoeSlot != -1) m_shoeMap.insert(std::make_pair(item.shoeSlot, item.type));
        if (item.waistSlot != -1) m_waistMap.insert(std::make_pair(item.waistSlot, item.type));
        if (item.wingSlot != -1) m_wingMap.insert(std::make_pair(item.wingSlot, item.type));
        if (item.shieldSlot != -1) m_shieldMap.insert(std::make_pair(item.shieldSlot, item.type));
        if (item.neckSlot != -1) m_neckMap.insert(std::make_pair(item.neckSlot, item.type));
        if (item.faceSlot != -1) m_faceMap.insert(std::make_pair(item.faceSlot, item.type));
        if (item.balloonSlot != -1) m_balloonMap.insert(std::make_pair(item.balloonSlot, item.type));

        if (item.damage != -1) m_weaponMap.insert(std::make_pair(item.type, item.type));

        m_itemsMap.insert(std::make_pair(item.type, item));
    }

    return true;
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new SpriteApp());
}
