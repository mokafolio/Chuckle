local util = require("Chuckle.Utilities")

local ws = {
    resizeable = false,
    size = {800, 600},
    sampleCount = 1,
    title = "Drag And Drop"
}

local window = RenderWindow(ws)
util.misc.exitOnError(window)
util.misc.exitOnError(window:enableDefaultUI())
local rd = window:renderDevice()

local buttons = {"uno", "dos", "tres"}
window:setDrawFunction(function(_dt)

    gui.beginWindowClosable("Document", function(_bOpen)

        for i=1,#buttons do
            gui.pushID(i)
            
            gui.button(buttons[i], {60,60})
            if i < #buttons then
                gui.sameLine()
            end

            if gui.beginDragDropSource(gui.DragDropFlags.None) then
                print("SETTING PAYLOAD", i)
                gui.setDragDropPayload("DragSource", i)
                gui.endDragDropSource()
            end

            if  gui.beginDragDropTarget() then
                gui.acceptDragDropPayload("DragSource", function(_payload)
                    print("I ACCEPT YOU MY MAAAJN", _payload)
                    buttons[i], buttons[_payload] = buttons[_payload], buttons[i]
                end)
                gui.endDragDropTarget()
            end

            gui.popID()
        end

        gui.endWindow()
    end)

    gui.showDemoWindow()

    local pass = rd:beginPass({renderBuffer=rb, clear={color={0, 0, 0, 1}}})
    rd:endPass(pass)
end)

window:run()
