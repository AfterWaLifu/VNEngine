#include <VNEngine/LuaReaders/Loader.hpp>

#include <VNEngine/Audio/AudioPlayer.hpp>
#include <VNEngine/Controls/InputHandler.hpp>
#include <VNEngine/Core/App.hpp>
#include <VNEngine/Core/Logger.hpp>
#include <VNEngine/Graphic/TextureManager.hpp>
#include <VNEngine/LuaReaders/StoryTeller.hpp>
#include <VNEngine/StateMachine/SaveLoad.hpp>
#include <VNEngine/StateMachine/State.hpp>
#include <VNEngine/Widgets/FontManager.hpp>

#include <VNEngine/LuaReaders/LuaGameFunctions.hpp>

#include <LuaBridge/LuaBridge.h>
#include <tinyxml2.h>

#define lb luabridge

namespace VNEngine {

void Loader::readPresets() {
    if (luaL_dofile(L, "game/presets.lua")) {
        VN_LOGS_WARNING("Cant load Lua presets");
        VN_LOGS_WARNING(lua_tostring(L, -1));
        lua_pop(L, 1);
        return;
    }
    lb::LuaRef t = lb::getGlobal(L, "scripts");
    if (t.isString()) {
        StoryTeller::SetScriptPath(t.tostring());
    }
    t = lb::getGlobal(L, "images");
    if (t.isString()) {
        TextureManager::SetImagesPath(t.tostring());
    }
    t = lb::getGlobal(L, "fonts");
    if (t.isString()) {
        FM_INSTANCE.SetFontPath(t.tostring());
    }
    t = lb::getGlobal(L, "fontDefault");
    if (t.isString()) {
        FM_INSTANCE.SetDefaultFont(t.tostring());
    }
    t = lb::getGlobal(L, "audio");
    if (t.isString()) {
        AP_INSTANCE.setAudiofilePath(t.tostring());
    }
    t = lb::getGlobal(L, "saves");
    if (t.isString()) {
        SaveLoad::setSaveDir(t.tostring());
    }
}

void Loader::readSettings() {
    std::string title = "VNEngine App";
    int width = 1280, height = 720, bwidth = width, bheight = height;
    bool foolscreen = false, resizable = false;
    uint8_t stretching = 2;

    using namespace tinyxml2;
    XMLDocument doc;
    doc.LoadFile((StoryTeller::GetScriptsPath() + "settings.cfg").c_str());
    XMLElement* root = doc.RootElement();

    XMLElement* window = root->FirstChildElement("Graphics");
    if (window) {
        XMLElement* temp;
        XMLElement* nested;
        if ((temp = window->FirstChildElement("Title"))) {
            title = temp->GetText();
        }
        if ((temp = window->FirstChildElement("BaseSize"))) {
            if ((nested = temp->FirstChildElement("Width")))
                bwidth = atoi(nested->GetText());
            if ((nested = temp->FirstChildElement("Height")))
                bheight = atoi(nested->GetText());
        }
        if ((temp = window->FirstChildElement("Size"))) {
            if ((nested = temp->FirstChildElement("Width")))
                width = atoi(nested->GetText());
            if ((nested = temp->FirstChildElement("Height")))
                height = atoi(nested->GetText());
        }
        if ((temp = window->FirstChildElement("Fullscreened"))) {
            if (std::string(temp->GetText()) == "true")
                foolscreen = true;
        }
        if ((temp = window->FirstChildElement("Resizable"))) {
            if (std::string(temp->GetText()) == "true")
                resizable = true;
        }
        if ((temp = window->FirstChildElement("Stretching"))) {
            auto stretchFromXML = (uint8_t)atoi(temp->GetText());
            if (stretchFromXML >= 0 && stretchFromXML <= 2)
                stretching = stretchFromXML;
        }
    }
    m_pApp->drawer_ = std::make_unique<Artist>(title, bwidth, bheight, foolscreen);
    if (width != bwidth || height != bheight)
        m_pApp->drawer_->SetWindowSize({width, height});
    m_pApp->drawer_->SetWindowResizable(resizable);
    m_pApp->drawer_->SetStretchingState((Stretching)stretching);

    XMLElement* audio = root->FirstChildElement("Audio");
    if (audio) {
        XMLElement* temp;
        if ((temp = audio->FirstChildElement("MusicVolume")))
            AP_INSTANCE.setMusicVolume((float)atof(temp->GetText()) / 100.0f);
        if ((temp = audio->FirstChildElement("SoundVolume")))
            AP_INSTANCE.setSoundVolume((float)atof(temp->GetText()) / 100.0f);
        if ((temp = audio->FirstChildElement("Muted"))) {
            if (std::string(temp->GetText()) == "true")
                AP_INSTANCE.mute();
        }
    }
}

void Loader::writeSettings() {
    using namespace tinyxml2;

    XMLDocument doc;
    XMLDeclaration* dec = doc.NewDeclaration();
    doc.InsertFirstChild(dec);
    XMLElement* root = doc.NewElement("Settings");

    XMLElement* window = doc.NewElement("Graphics");
    XMLElement* title = doc.NewElement("Title");
    XMLElement* size = doc.NewElement("Size");
    XMLElement* basesize = doc.NewElement("BaseSize");
    XMLElement* sizex = doc.NewElement("Width");
    XMLElement* sizey = doc.NewElement("Height");
    XMLElement* basesizex = doc.NewElement("Width");
    XMLElement* basesizey = doc.NewElement("Height");
    XMLElement* fullscreen = doc.NewElement("Fullscreened");
    XMLElement* resizable = doc.NewElement("Resizable");
    XMLElement* stretching = doc.NewElement("Stretching");

    title->SetText(m_pApp->drawer_->GetWindowTitle().c_str());
    auto sizevec = m_pApp->drawer_->GetWindowSize();
    sizex->SetText(sizevec.x);
    sizey->SetText(sizevec.y);
    size->InsertFirstChild(sizex);
    size->InsertEndChild(sizey);
    auto basesizevec = m_pApp->drawer_->GetBaseWinSize();
    basesizex->SetText(basesizevec.x);
    basesizey->SetText(basesizevec.y);
    basesize->InsertFirstChild(basesizex);
    basesize->InsertEndChild(basesizey);
    fullscreen->SetText(m_pApp->drawer_->GetWindowFullscreen());
    resizable->SetText(m_pApp->drawer_->GetWindowResizable());
    stretching->SetText((int)m_pApp->drawer_->GetStretchingState());
    window->InsertFirstChild(title);
    window->InsertEndChild(size);
    window->InsertEndChild(basesize);
    window->InsertEndChild(fullscreen);
    window->InsertEndChild(resizable);
    window->InsertEndChild(stretching);

    XMLElement* audio = doc.NewElement("Audio");
    XMLElement* musicv = doc.NewElement("MusicVolume");
    XMLElement* soundv = doc.NewElement("SoundVolume");
    XMLElement* muted = doc.NewElement("Muted");
    musicv->SetText(AP_INSTANCE.musicVolume() * 100.0f);
    soundv->SetText(AP_INSTANCE.soundVolume() * 100.0f);
    muted->SetText(AP_INSTANCE.isMute());
    audio->InsertFirstChild(musicv);
    audio->InsertEndChild(soundv);
    audio->InsertEndChild(muted);

    root->InsertFirstChild(window);
    root->InsertEndChild(audio);
    doc.InsertEndChild(root);

    doc.SaveFile((StoryTeller::GetScriptsPath() + "settings.cfg").c_str());
}

void Loader::preload() {
    if (luaL_dofile(L, ((StoryTeller::GetScriptsPath() + "preload.lua").c_str()))) {
        VN_LOGS_WARNING("Error on preloading resources stage");
        VN_LOGS_WARNING(lua_tostring(L, -1));
        lua_pop(L, 1);
    }
    LG::SetDrawwerToGetWindowSize(m_pApp->drawer_.get());
}

Loader::Loader(App* app) : LuaReader() {
    RegisterSettingsFunctons();
    m_pApp = app;
}

Loader::~Loader() {}

void Loader::Load() {
    readPresets();
    readSettings();
    State::setStatesDrawer(m_pApp->drawer_.get());
    LS::InitDrawingForLua(m_pApp->drawer_.get());
    preload();
}

void Loader::Save() { writeSettings(); }

} // namespace VNEngine
