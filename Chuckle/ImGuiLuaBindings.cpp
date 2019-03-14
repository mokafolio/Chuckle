#include <Chuckle/ImGuiLuaBindings.hpp>
#include <Stick/DynamicArray.hpp>
#include <Stick/String.hpp>

#include "imgui.h"
#include "imgui_internal.h"

namespace chuckle
{
    using namespace stick;

void registerImGuiBindings(sol::state_view _lua)
{
    // imgui bindings
    sol::table globals = _lua.globals();
    sol::table imgui = _lua.create_table();
    globals["gui"] = imgui;

    imgui["createContext"] = []() { return ImGui::CreateContext(); };
    imgui["destroyContext"] =
        sol::overload(ImGui::DestroyContext, []() { ImGui::DestroyContext(); });
    imgui["getCurrentContext"] = ImGui::GetCurrentContext;
    imgui["setCurrentContext"] = ImGui::SetCurrentContext;

    imgui["getIO"] = ImGui::GetIO;
    imgui["getStyle"] = ImGui::GetStyle;
    imgui["newFrame"] = ImGui::NewFrame;
    imgui["endFrame"] = ImGui::EndFrame;
    imgui["render"] = ImGui::Render;
    imgui["getDrawData"] = ImGui::GetDrawData;

    imgui["showDemoWindow"] = []() {
        bool bShow;
        ImGui::ShowDemoWindow(&bShow);
        return bShow;
    };
    imgui["showAboutWindow"] = []() {
        bool bShow;
        ImGui::ShowAboutWindow(&bShow);
        return bShow;
    };
    imgui["showMetricsWindow"] = []() {
        bool bShow;
        ImGui::ShowMetricsWindow(&bShow);
        return bShow;
    };
    imgui["showStyleEditor"] =
        sol::overload(ImGui::ShowStyleEditor, []() { ImGui::ShowStyleEditor(); });
    imgui["showStyleEditor"] = ImGui::ShowStyleSelector;
    imgui["showFontSelector"] = ImGui::ShowFontSelector;
    imgui["showUserGuid"] = ImGui::ShowUserGuide;
    imgui["getVersion"] = ImGui::GetVersion;

    imgui["styleColorsDark"] =
        sol::overload(ImGui::StyleColorsDark, []() { ImGui::StyleColorsDark(); });
    imgui["styleColorsClassic"] =
        sol::overload(ImGui::StyleColorsClassic, []() { ImGui::StyleColorsClassic(); });
    imgui["styleColorsLight"] =
        sol::overload(ImGui::StyleColorsLight, []() { ImGui::StyleColorsLight(); });

    imgui["beginWindow"] = sol::overload([](const char * _str) { return ImGui::Begin(_str); },
                                   [](const char * _str, ImGuiWindowFlags _flags) {
                                       return ImGui::Begin(_str, NULL, _flags);
                                   });
    imgui["beginWindowClosable"] = sol::overload(
        [](const char * _str, sol::function _cb) {
            bool bOpen = true;
            if (ImGui::Begin(_str, &bOpen, 0))
                _cb(bOpen);
        },
        [](const char * _str, ImGuiWindowFlags _flags, sol::function _cb) {
            bool bOpen = true;
            if (ImGui::Begin(_str, &bOpen, _flags))
                _cb(bOpen);
        });

    imgui["endWindow"] = ImGui::End;
    imgui["beginChild"] = sol::overload(
        (bool (*)(const char *, const ImVec2 &, bool, ImGuiWindowFlags))ImGui::BeginChild,
        [](const char * _str) { return ImGui::BeginChild(_str); },
        [](const char * _str, const ImVec2 & _size) { return ImGui::BeginChild(_str, _size); },
        [](const char * _str, const ImVec2 & _size, bool _border) {
            return ImGui::BeginChild(_str, _size, _border);
        });
    imgui["endChild"] = ImGui::EndChild;

    imgui["isWindowAppearing"] = ImGui::IsWindowAppearing;
    imgui["isWindowCollapsed"] = ImGui::IsWindowCollapsed;
    imgui["isWindowFocused"] =
        sol::overload(ImGui::IsWindowFocused, []() { return ImGui::IsWindowFocused(); });
    imgui["isWindowHovered"] =
        sol::overload(ImGui::IsWindowHovered, []() { return ImGui::IsWindowHovered(); });

    imgui["getWindowPos"] = ImGui::GetWindowPos;
    imgui["getWindowSize"] = ImGui::GetWindowSize;
    imgui["getWindowWidth"] = ImGui::GetWindowWidth;
    imgui["getWindowHeight"] = ImGui::GetWindowHeight;
    imgui["getContentRegionMax"] = ImGui::GetContentRegionMax;
    imgui["getContentRegionAvail"] = ImGui::GetContentRegionAvail;
    imgui["getContentRegionAvailWidth"] = ImGui::GetContentRegionAvailWidth;
    imgui["getWindowContentRegionMin"] = ImGui::GetWindowContentRegionMin;
    imgui["getWindowContentRegionMax"] = ImGui::GetWindowContentRegionMax;
    imgui["getWindowContentRegionWidth"] = ImGui::GetWindowContentRegionWidth;

    imgui["setNextWindowPos"] = sol::overload(
        ImGui::SetNextWindowPos,
        [](const ImVec2 & _pos) { ImGui::SetNextWindowPos(_pos); },
        [](const ImVec2 & _pos, ImGuiCond _cond) { ImGui::SetNextWindowPos(_pos, _cond); });
    imgui["setNextWindowSize"] = sol::overload(
        ImGui::SetNextWindowSize, [](const ImVec2 & _size) { ImGui::SetNextWindowSize(_size); });
    // imgui["setNextWindowSizeConstraints"] =
    //     sol::overload(ImGui::SetNextWindowSizeConstraints,
    //                   [](const ImVec2 & _size_min, const ImVec2 & _size_max) {
    //                       ImGui::SetNextWindowSizeConstraints(_size_min, _size_max);
    //                   });
    imgui["setNextWindowContentSize"] = ImGui::SetNextWindowContentSize;
    imgui["setNextWindowCollapsed"] =
        sol::overload(ImGui::SetNextWindowCollapsed,
                      [](bool _collapsed) { ImGui::SetNextWindowCollapsed(_collapsed); });
    imgui["setNextWindowFocus"] = ImGui::SetNextWindowFocus;
    imgui["setNextWindowBgAlpha"] = ImGui::SetNextWindowBgAlpha;

    imgui["setWindowCollapsed"] = sol::overload(
        (void (*)(bool, ImGuiCond))ImGui::SetWindowCollapsed,
        [](bool _collapsed) { ImGui::SetWindowCollapsed(_collapsed); },
        (void (*)(const char *, bool, ImGuiCond))ImGui::SetWindowCollapsed,
        [](const char * _name, bool _collapsed) { ImGui::SetWindowCollapsed(_name, _collapsed); });
    imgui["setWindowFocus"] = sol::overload((void (*)(const char *))ImGui::SetWindowFocus,
                                            (void (*)())ImGui::SetWindowFocus);
    imgui["setWindowFontScale"] = ImGui::SetWindowFontScale;
    imgui["setWindowPos"] = sol::overload(
        (void (*)(const ImVec2 &, ImGuiCond))ImGui::SetWindowPos,
        [](const ImVec2 & _pos) { ImGui::SetWindowPos(_pos); },
        (void (*)(const char *, const ImVec2 &, ImGuiCond))ImGui::SetWindowPos,
        [](const char * _name, const ImVec2 & _pos) { ImGui::SetWindowPos(_name, _pos); });
    imgui["setWindowSize"] = sol::overload(
        (void (*)(const ImVec2 &, ImGuiCond))ImGui::SetWindowSize,
        [](const ImVec2 & _size) { ImGui::SetWindowSize(_size); },
        (void (*)(const char *, const ImVec2 &, ImGuiCond))ImGui::SetWindowSize,
        [](const char * _name, const ImVec2 & _size) { ImGui::SetWindowSize(_name, _size); });
    imgui["setWindowCollapsed"] = sol::overload(
        (void (*)(bool, ImGuiCond))ImGui::SetWindowCollapsed,
        [](bool _collapsed) { ImGui::SetWindowCollapsed(_collapsed); },
        (void (*)(const char *, bool, ImGuiCond))ImGui::SetWindowCollapsed,
        [](const char * _name, bool _collapsed) { ImGui::SetWindowCollapsed(_name, _collapsed); });
    imgui["setWindowFocus"] = sol::overload((void (*)())ImGui::SetWindowFocus,
                                            (void (*)(const char *))ImGui::SetWindowFocus);

    imgui["getScrollX"] = ImGui::GetScrollX;
    imgui["getScrollY"] = ImGui::GetScrollY;
    imgui["getScrollMaxX"] = ImGui::GetScrollMaxX;
    imgui["getScrollMaxY"] = ImGui::GetScrollMaxY;
    imgui["getScrollX"] = ImGui::SetScrollX;
    imgui["getScrollY"] = ImGui::SetScrollY;
    imgui["setScrollHereY"] = ImGui::SetScrollHereY;
    imgui["setScrollFromPosY"] = ImGui::SetScrollFromPosY;

    imgui["pushFont"] = ImGui::PushFont;
    imgui["popFont"] = ImGui::PopFont;
    imgui["pushStyleColor"] =
        sol::overload((void (*)(ImGuiCol, ImU32))ImGui::PushStyleColor,
                      (void (*)(ImGuiCol, const ImVec4 &))ImGui::PushStyleColor);
    imgui["popStyleColor"] = sol::overload(ImGui::PopStyleColor, []() { ImGui::PopStyleColor(); });
    imgui["pushStyleVar"] =
        sol::overload((void (*)(ImGuiStyleVar, float))ImGui::PushStyleVar,
                      (void (*)(ImGuiStyleVar, const ImVec2 &))ImGui::PushStyleVar),
    imgui["popStyleVar"] = sol::overload(ImGui::PopStyleVar, []() { ImGui::PopStyleVar(); });
    imgui["getStyleColorVec4"] = ImGui::GetStyleColorVec4;
    imgui["getFont"] = ImGui::GetFont;
    imgui["getFontSize"] = ImGui::GetFontSize;
    imgui["getFontTexUvWhitePixel"] = ImGui::GetFontTexUvWhitePixel;

    imgui["pushItemWidth"] = ImGui::PushItemWidth;
    imgui["popItemWidth"] = ImGui::PopItemWidth;
    imgui["calcItemWidth"] = ImGui::CalcItemWidth;
    imgui["pushTextWrapPos"] =
        sol::overload(ImGui::PushTextWrapPos, []() { ImGui::PushTextWrapPos(); });
    imgui["popTextWrapPos"] = ImGui::PopTextWrapPos;
    imgui["pushAllowKeyboardFocus"] = ImGui::PushAllowKeyboardFocus;
    imgui["popAllowKeyboardFocus"] = ImGui::PopAllowKeyboardFocus;
    imgui["pushButtonRepeat"] = ImGui::PushButtonRepeat;
    imgui["popButtonRepeat"] = ImGui::PopButtonRepeat;

    imgui["separator"] = ImGui::Separator;
    imgui["sameLine"] = sol::overload(ImGui::SameLine,
                                      []() { ImGui::SameLine(); },
                                      [](float _localPos) { ImGui::SameLine(_localPos); });
    imgui["newLine"] = ImGui::NewLine;
    imgui["spacing"] = ImGui::Spacing;
    imgui["dummy"] = ImGui::Dummy;
    imgui["indent"] = sol::overload(ImGui::Indent, []() { ImGui::Indent(); });
    imgui["unindent"] = sol::overload(ImGui::Unindent, []() { ImGui::Unindent(); });
    imgui["beginGroup"] = ImGui::BeginGroup;
    imgui["endGroup"] = ImGui::EndGroup;
    imgui["getCursorPos"] = ImGui::GetCursorPos;
    imgui["getCursorPosX"] = ImGui::GetCursorPosX;
    imgui["getCursorPosY"] = ImGui::GetCursorPosY;
    imgui["setCursorPos"] = ImGui::SetCursorPos;
    imgui["setCursorPosX"] = ImGui::SetCursorPosX;
    imgui["setCursorPosY"] = ImGui::SetCursorPosY;
    imgui["getCursorStartPos"] = ImGui::GetCursorStartPos;
    imgui["getCursorScreenPos"] = ImGui::GetCursorScreenPos;
    imgui["setCursorScreenPos"] = ImGui::SetCursorScreenPos;
    imgui["alignTextToFramePadding"] = ImGui::AlignTextToFramePadding;
    imgui["getTextLineHeight"] = ImGui::GetTextLineHeight;
    imgui["getTextLineHeightWithSpacing"] = ImGui::GetTextLineHeightWithSpacing;
    imgui["getFrameHeight"] = ImGui::GetFrameHeight;
    imgui["getFrameHeightWithSpacing"] = ImGui::GetFrameHeightWithSpacing;

    imgui["pushID"] =
        sol::overload((void (*)(const char *))ImGui::PushID, (void (*)(int))ImGui::PushID);
    imgui["popID"] = ImGui::PopID;
    imgui["getID"] = (ImGuiID(*)(const char *))ImGui::GetID;

    imgui["text"] = [](const char * _text) { ImGui::TextUnformatted(_text); };
    imgui["textColored"] = [](const ImVec4 & col, const char * _text) {
        ImGui::TextColored(col, "%s", _text);
    };
    imgui["textDisabled"] = [](const char * _text) { ImGui::TextDisabled("%s", _text); };
    imgui["textWrapped"] = [](const char * _text) { ImGui::TextWrapped("%s", _text); };
    imgui["labelText"] = [](const char * _label, const char * _text) {
        ImGui::LabelText(_label, "%s", _text);
    };
    imgui["bulletText"] = [](const char * _text) { ImGui::BulletText("%s", _text); };

    imgui["button"] =
        sol::overload(ImGui::Button, [](const char * _label) { ImGui::Button(_label); });
    imgui["smallButton"] = ImGui::SmallButton;
    imgui["invisibleButton"] = ImGui::InvisibleButton;
    imgui["arrowButton"] = ImGui::ArrowButton;
    // Image
    // ImageButton
    imgui["checkbox"] = ImGui::Checkbox;
    imgui["radioButton"] = (bool (*)(const char *, bool))ImGui::RadioButton;
    imgui["progressBar"] = sol::overload(ImGui::ProgressBar,
                                         [](float _fraction) { ImGui::ProgressBar(_fraction); },
                                         [](float _fraction, const ImVec2 & _size_arg) {
                                             ImGui::ProgressBar(_fraction, _size_arg);
                                         });
    imgui["bullet"] = ImGui::Bullet;

    imgui["beginCombo"] =
        sol::overload(ImGui::BeginCombo, [](const char * _label, const char * _preview_value) {
            ImGui::BeginCombo(_label, _preview_value);
        });
    imgui["endCombo"] = ImGui::EndCombo;

    imgui["dragFloat"] = sol::overload(
        [](const char * _label, float _currentValue, sol::function _cb) {
            if (ImGui::DragFloat(_label, &_currentValue))
                _cb(_currentValue);
        },
        [](const char * _label, float _currentValue, float _v_speed, sol::function _cb) {
            if (ImGui::DragFloat(_label, &_currentValue, _v_speed))
                _cb(_currentValue);
        },
        [](const char * _label,
           float _currentValue,
           float _v_speed,
           float _v_min,
           sol::function _cb) {
            if (ImGui::DragFloat(_label, &_currentValue, _v_speed, _v_min))
                _cb(_currentValue);
        },
        [](const char * _label,
           float _currentValue,
           float _v_speed,
           float _v_min,
           float _v_max,
           sol::function _cb) {
            if (ImGui::DragFloat(_label, &_currentValue, _v_speed, _v_min, _v_max))
                _cb(_currentValue);
        },
        [](const char * _label,
           float _currentValue,
           float _v_speed,
           float _v_min,
           float _v_max,
           const char * _fmt,
           sol::function _cb) {
            if (ImGui::DragFloat(_label, &_currentValue, _v_speed, _v_min, _v_max, _fmt))
                _cb(_currentValue);
        },
        [](const char * _label,
           float _currentValue,
           float _v_speed,
           float _v_min,
           float _v_max,
           const char * _fmt,
           float _power,
           sol::function _cb) {
            if (ImGui::DragFloat(_label, &_currentValue, _v_speed, _v_min, _v_max, _fmt, _power))
                _cb(_currentValue);
        });

    imgui["dragInt"] = sol::overload(
        [](const char * _label, int _currentValue, sol::function _cb) {
            if (ImGui::DragInt(_label, &_currentValue))
                _cb(_currentValue);
        },
        [](const char * _label, int _currentValue, float _v_speed, sol::function _cb) {
            if (ImGui::DragInt(_label, &_currentValue, _v_speed))
                _cb(_currentValue);
        },
        [](const char * _label, int _currentValue, float _v_speed, int _v_min, sol::function _cb) {
            if (ImGui::DragInt(_label, &_currentValue, _v_speed, _v_min))
                _cb(_currentValue);
        },
        [](const char * _label,
           int _currentValue,
           float _v_speed,
           int _v_min,
           int _v_max,
           sol::function _cb) {
            if (ImGui::DragInt(_label, &_currentValue, _v_speed, _v_min, _v_max))
                _cb(_currentValue);
        },
        [](const char * _label,
           int _currentValue,
           float _v_speed,
           int _v_min,
           int _v_max,
           const char * _fmt,
           sol::function _cb) {
            if (ImGui::DragInt(_label, &_currentValue, _v_speed, _v_min, _v_max, _fmt))
                _cb(_currentValue);
        });

    imgui["sliderFloat"] = sol::overload(
        [](const char * _label,
           float _currentValue,
           float _v_min,
           float _v_max,
           sol::function _cb) {
            if (ImGui::SliderFloat(_label, &_currentValue, _v_min, _v_max))
                _cb(_currentValue);
        },
        [](const char * _label,
           float _currentValue,
           float _v_min,
           float _v_max,
           const char * _format,
           sol::function _cb) {
            if (ImGui::SliderFloat(_label, &_currentValue, _v_min, _v_max, _format))
                _cb(_currentValue);
        });

    imgui["sliderAngle"] = sol::overload(
        [](const char * _label, float _currentValue, sol::function _cb) {
            if (ImGui::SliderAngle(_label, &_currentValue))
                _cb(_currentValue);
        },
        [](const char * _label, float _currentValue, float _v_degrees_min, sol::function _cb) {
            if (ImGui::SliderAngle(_label, &_currentValue, _v_degrees_min))
                _cb(_currentValue);
        },
        [](const char * _label,
           float _currentValue,
           float _v_degrees_min,
           float _v_degrees_max,
           sol::function _cb) {
            if (ImGui::SliderAngle(_label, &_currentValue, _v_degrees_min, _v_degrees_max))
                _cb(_currentValue);
        },
        [](const char * _label,
           float _currentValue,
           float _v_degrees_min,
           float _v_degrees_max,
           const char * _format,
           sol::function _cb) {
            if (ImGui::SliderAngle(_label, &_currentValue, _v_degrees_min, _v_degrees_max, _format))
                _cb(_currentValue);
        });

    imgui["sliderInt"] = sol::overload(
        [](const char * _label, int _currentValue, int _v_min, int _v_max, sol::function _cb) {
            if (ImGui::SliderInt(_label, &_currentValue, _v_min, _v_max))
                _cb(_currentValue);
        },
        [](const char * _label,
           int _currentValue,
           int _v_min,
           int _v_max,
           const char * _format,
           sol::function _cb) {
            if (ImGui::SliderInt(_label, &_currentValue, _v_min, _v_max, _format))
                _cb(_currentValue);
        });

    imgui["vSliderFloat"] = sol::overload(
        [](const char * _label,
           const ImVec2 & _size,
           float _currentValue,
           float _v_min,
           float _v_max,
           sol::function _cb) {
            if (ImGui::VSliderFloat(_label, _size, &_currentValue, _v_min, _v_max))
                _cb(_currentValue);
        },
        [](const char * _label,
           const ImVec2 & _size,
           float _currentValue,
           float _v_min,
           float _v_max,
           const char * _format,
           sol::function _cb) {
            if (ImGui::VSliderFloat(_label, _size, &_currentValue, _v_min, _v_max, _format))
                _cb(_currentValue);
        });

    imgui["vSliderInt"] = sol::overload(
        [](const char * _label,
           const ImVec2 & _size,
           int _currentValue,
           int _v_min,
           int _v_max,
           sol::function _cb) {
            if (ImGui::VSliderInt(_label, _size, &_currentValue, _v_min, _v_max))
                _cb(_currentValue);
        },
        [](const char * _label,
           const ImVec2 & _size,
           int _currentValue,
           int _v_min,
           int _v_max,
           const char * _format,
           sol::function _cb) {
            if (ImGui::VSliderInt(_label, _size, &_currentValue, _v_min, _v_max, _format))
                _cb(_currentValue);
        });

    //@TODO: add overloads
    imgui["inputText"] = [](const char * _label, int _capacity, sol::function _cb) {
        static stick::String s_buffer;
        s_buffer.reserve(_capacity);
        if (ImGui::InputText(_label, &s_buffer[0], s_buffer.capacity()))
            _cb(s_buffer.cString());
    };
    imgui["inputTextMultiline"] = [](const char * _label, int _capacity, sol::function _cb) {
        static stick::String s_buffer;
        s_buffer.reserve(_capacity);
        if (ImGui::InputTextMultiline(_label, &s_buffer[0], s_buffer.capacity()))
            _cb(s_buffer.cString());
    };

    imgui["inputFloat"] = sol::overload(
        [](const char * _label, float _current, sol::function _cb) {
            if (ImGui::InputFloat(_label, &_current))
                _cb(_current);
        },
        [](const char * _label, float _current, float _step, sol::function _cb) {
            if (ImGui::InputFloat(_label, &_current, _step))
                _cb(_current);
        },
        [](const char * _label, float _current, float _step, float _stepFast, sol::function _cb) {
            if (ImGui::InputFloat(_label, &_current, _step, _stepFast))
                _cb(_current);
        },
        [](const char * _label,
           float _current,
           float _step,
           float _stepFast,
           const char * _fmt,
           sol::function _cb) {
            if (ImGui::InputFloat(_label, &_current, _step, _stepFast, _fmt))
                _cb(_current);
        },
        [](const char * _label,
           float _current,
           float _step,
           float _stepFast,
           const char * _fmt,
           ImGuiInputTextFlags _flags,
           sol::function _cb) {
            if (ImGui::InputFloat(_label, &_current, _step, _stepFast, _fmt, _flags))
                _cb(_current);
        });

    imgui["inputInt"] = sol::overload(
        [](const char * _label, int _current, sol::function _cb) {
            if (ImGui::InputInt(_label, &_current))
                _cb(_current);
        },
        [](const char * _label, int _current, int _step, sol::function _cb) {
            if (ImGui::InputInt(_label, &_current, _step))
                _cb(_current);
        },
        [](const char * _label, int _current, int _step, int _stepFast, sol::function _cb) {
            if (ImGui::InputInt(_label, &_current, _step, _stepFast))
                _cb(_current);
        },
        [](const char * _label,
           int _current,
           int _step,
           int _stepFast,
           ImGuiInputTextFlags _flags,
           sol::function _cb) {
            if (ImGui::InputInt(_label, &_current, _step, _stepFast, _flags))
                _cb(_current);
        });

    //@TODO: Overload
    imgui["colorEdit3"] = [](const char * _label, float _r, float _g, float _b, sol::function _cb) {
        float tmp[3] = { _r, _g, _b };
        if (ImGui::ColorEdit3(_label, tmp))
            _cb(tmp[0], tmp[1], tmp[2]);
    };
    //@TODO: Overload
    imgui["colorEdit4"] =
        [](const char * _label, float _r, float _g, float _b, float _a, sol::function _cb) {
            float tmp[4] = { _r, _g, _b, _a };
            if (ImGui::ColorEdit4(_label, tmp))
                _cb(tmp[0], tmp[1], tmp[2], tmp[3]);
        };
    //@TODO: Overload
    imgui["colorPicker3"] =
        [](const char * _label, float _r, float _g, float _b, sol::function _cb) {
            float tmp[3] = { _r, _g, _b };
            if (ImGui::ColorPicker3(_label, tmp))
                _cb(tmp[0], tmp[1], tmp[2]);
        };
    //@TODO: Overload
    imgui["colorPicker3"] =
        [](const char * _label, float _r, float _g, float _b, float _a, sol::function _cb) {
            float tmp[4] = { _r, _g, _b, _a };
            if (ImGui::ColorPicker4(_label, tmp))
                _cb(tmp[0], tmp[1], tmp[2], tmp[3]);
        };
    //@TODO: Overload
    imgui["colorButton"] =
        [](const char * _label, float _r, float _g, float _b, float _a, sol::function _cb) {
            ImVec4 tmp = { _r, _g, _b, _a };
            if (ImGui::ColorButton(_label, tmp))
                _cb(tmp.x, tmp.y, tmp.z, tmp.y);
        };

    imgui["setColorEditOptions"] = ImGui::SetColorEditOptions;

    imgui["treeNode"] = (bool (*)(const char *))ImGui::TreeNode;
    imgui["treeNodeEx"] =
        sol::overload((bool (*)(const char *, ImGuiTreeNodeFlags))ImGui::TreeNodeEx,
                      [](const char * _label) { ImGui::TreeNodeEx(_label); });
    imgui["treePush"] = (void (*)(const char *))ImGui::TreePush;
    imgui["treePop"] = ImGui::TreePop;
    imgui["treeAdvanceToLabelPos"] = ImGui::TreeAdvanceToLabelPos;
    imgui["getTreeNodeToLabelSpacing"] = ImGui::GetTreeNodeToLabelSpacing;
    imgui["setNextTreeNodeOpen"] = sol::overload(
        ImGui::SetNextTreeNodeOpen, [](bool _is_open) { ImGui::SetNextTreeNodeOpen(_is_open); });
    imgui["collapsingHeader"] =
        sol::overload((bool (*)(const char *, ImGuiTreeNodeFlags))ImGui::CollapsingHeader,
                      [](const char * _label) { ImGui::CollapsingHeader(_label); });
    //@TODO: Overloads
    imgui["collapsingHeaderClosable"] = sol::overload([](const char * _label, sol::function _cb) {
        bool bOpen = true;
        ImGui::CollapsingHeader(_label, &bOpen);
        _cb(bOpen);
    });

    imgui["selectable"] = sol::overload(
        [](const char * _label, bool _bSelected, sol::function _cb) {
            if (ImGui::Selectable(_label, _bSelected))
                _cb();
        },
        [](const char * _label, bool _bSelected, ImGuiSelectableFlags _flags, sol::function _cb) {
            if (ImGui::Selectable(_label, _bSelected, _flags))
                _cb();
        });

    imgui["ListBox"] =
        [](const char * _label, int _current_item, sol::table _items, sol::function _cb) {
            STICK_ASSERT(_current_item <= _items.size());

            //@TODO: proper allocator?
            static DynamicArray<const char *> s_buffer;
            s_buffer.resize(_items.size());
            for (Size i = 0; i < s_buffer.count(); ++i)
                s_buffer[i] = _items.get<const char *>(i + 1);

            --_current_item;
            if (ImGui::ListBox(_label, &_current_item, &s_buffer[0], s_buffer.count()))
                _cb(_current_item + 1, s_buffer[_current_item]);
        };

    // PlotLines

    imgui["value"] =
        sol::overload((void (*)(const char *, bool))ImGui::Value,
                      (void (*)(const char *, int))ImGui::Value,
                      (void (*)(const char *, float, const char *))ImGui::Value,
                      [](const char * _pref, float _val) { ImGui::Value(_pref, _val); });

    imgui["beginMainMenuBar"] = ImGui::BeginMainMenuBar;
    imgui["endMainMenuBar"] = ImGui::EndMainMenuBar;
    imgui["beginMenuBar"] = ImGui::BeginMenuBar;
    imgui["endMenuBar"] = ImGui::EndMenuBar;
    imgui["beginMenu"] = sol::overload(
        ImGui::BeginMenu, [](const char * _label) { return ImGui::BeginMenu(_label); });
    imgui["endMenu"] = ImGui::EndMenu;
    imgui["menuItem"] =
        sol::overload((bool (*)(const char *, const char *, bool, bool))ImGui::MenuItem,
                      [](const char * _label) { return ImGui::MenuItem(_label); },
                      [](const char * _label, const char * _shortcut) {
                          return ImGui::MenuItem(_label, _shortcut);
                      },
                      [](const char * _label, const char * _shortcut, bool _selected) {
                          return ImGui::MenuItem(_label, _shortcut, _selected);
                      });
}
} // namespace chuckle
