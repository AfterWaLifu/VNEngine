#include <VNEngine/Widgets/TextBox.hpp>

#include <VNEngine/Controls/InputHandler.hpp>

namespace VNEngine {

TextBox::TextBox(vec4 geometry, std::wstring text, uint32_t maxCharNumber, vec4u8 textColor, const std::string& fontKey)
    : Text(geometry, text, textColor, fontKey) {
    m_BackgroundColor = {220, 220, 220, 255};
    SetAlign((Alignment)(ALIGN_VCENTER | ALIGN_LEFT));
    s_ActiveTextBox = this;

    m_DrawBorder = true;
    m_BorderColor = {(uint8_t)(m_BackgroundColor.r < 40 ? 255 - m_BackgroundColor.r : m_BackgroundColor.r - 40),
                     (uint8_t)(m_BackgroundColor.g < 40 ? 255 - m_BackgroundColor.g : m_BackgroundColor.g - 40),
                     (uint8_t)(m_BackgroundColor.b < 40 ? 255 - m_BackgroundColor.b : m_BackgroundColor.b - 40), 255};

    m_LineEnabled = true;
    m_LineTimer = SDL_GetTicks64();
    m_MaxCharNumber = maxCharNumber;

    IH_INSTANCE.setTextInputState(true);

    if (!s_InputString) {
        s_InputString = IH_INSTANCE.textInput();
    }
}

TextBox::TextBox(const textboxState& tbs) : Text(tbs.ts) {
    SetValue(tbs.currentString);
    SetMaxCharNumber(tbs.maxchar);

    m_LineEnabled = true;
    m_LineTimer = SDL_GetTicks64();

    s_ActiveTextBox = this;

    IH_INSTANCE.setTextInputState(true);
    if (!s_InputString) {
        s_InputString = IH_INSTANCE.textInput();
    }
}

TextBox::~TextBox() { IH_INSTANCE.setTextInputState(false); }

void TextBox::Check() {
    if (IH_INSTANCE.mouseButtonClicked(LEFT)) {
        vec2 mousepos = IH_INSTANCE.mousePos();

        if (mousepos.x >= m_Geometry.x && mousepos.y >= m_Geometry.y && mousepos.x <= m_Geometry.x + m_Geometry.w &&
            mousepos.y <= m_Geometry.y + m_Geometry.h) {

            s_ActiveTextBox = this;

            if (IH_INSTANCE.textInputState() == false) {
                IH_INSTANCE.setTextInputState(true);
            }

            if (s_InputString) {
                auto& inputString = s_InputString->get();
                if (m_CurrentString.empty()) {
                    inputString = "";
                } else {
                    inputString = m_CurrentString;
                }
            }
        }
    }

    if (s_InputString) {
        auto& inputString = s_InputString->get();

        if (this == s_ActiveTextBox) {
            inputString.empty() ? m_CurrentString = "" : m_CurrentString = inputString;
            s_ActiveTextBox->SetText(m_CurrentString);
        }

        if (m_CurrentString.length() > m_MaxCharNumber) {
            inputString.pop_back();
            m_CurrentString.pop_back();
        }
    }
}

void TextBox::Draw() {
    Text::Draw();

    if (s_ActiveTextBox != this) {
        SDL_SetRenderDrawColor(Widget::sWD.renderer, m_TextColor.r, m_TextColor.g, m_TextColor.b, m_TextColor.a);
        SDL_RenderDrawLine(Widget::sWD.renderer, m_TextDestination.x + m_TextDestination.w + 1, m_TextDestination.y + 1,
                           m_TextDestination.x + m_TextDestination.w + 1,
                           m_TextDestination.y + m_TextDestination.h + 1);
    }

    uint64_t time = SDL_GetTicks64();
    if ((time - m_LineTimer) > 300) {
        m_LineTimer = time;
        m_LineEnabled = !m_LineEnabled;
    }
    if (m_LineEnabled) {
        SDL_SetRenderDrawColor(Widget::sWD.renderer, m_TextColor.r, m_TextColor.g, m_TextColor.b, m_TextColor.a);
        SDL_RenderDrawLine(Widget::sWD.renderer, m_TextDestination.x + m_TextDestination.w + 1, m_TextDestination.y + 1,
                           m_TextDestination.x + m_TextDestination.w + 1,
                           m_TextDestination.y + m_TextDestination.h + 1);
    }
}

std::string TextBox::Value() { return m_CurrentString; }

void TextBox::SetValue(const std::string& str) {
    m_CurrentString = str;
    if (s_InputString && s_ActiveTextBox == this) {
        (*s_InputString).get() = str;
    }
    SetText(str);
}

void TextBox::SetMaxCharNumber(uint32_t quantity) {
    m_MaxCharNumber = quantity;
    if (m_CurrentString.length() > quantity) {
        m_CurrentString = m_CurrentString.substr(0, quantity);
        SetText(cvt(m_CurrentString));
    }
}

uint32_t TextBox::GetMaxCharNumber() { return m_MaxCharNumber; }

textboxState TextBox::Dump() {
    textboxState tbs = {Text::Dump(), m_CurrentString, m_MaxCharNumber};
    return tbs;
}
} // namespace VNEngine