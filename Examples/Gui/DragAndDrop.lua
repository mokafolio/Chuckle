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

local buttons = {"eins", "zwei", "drei", "vier", "fünf", "sechs", "sieben", "acht", "neun"}
window:setDrawFunction(function(_dt)

    gui.beginWindowClosable("Document", function(_bOpen)

        for i=1,#buttons do
            gui.pushID(i)
            
            gui.button(buttons[i], {60,60})

            if (((i-1) % 3) < 2) then gui.sameLine() end

            if gui.beginDragDropSource(gui.DragDropFlags.None) then
                gui.setDragDropPayload("DragSource", i, gui.Cond.Once)
                gui.text(buttons[i]);
                gui.endDragDropSource()
            end

            if  gui.beginDragDropTarget() then
                gui.acceptDragDropPayload("DragSource", function(_payload)
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
