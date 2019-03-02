local util = require("Chuckle.Utilities")

local ws = WindowSettings()
ws:setResizeable(false)
ws:setSize(1280, 720)
ws:setSampleCount(1)
ws:setTitle("Test")

local window = Window(ws)

local rd = createRenderDevice():ensure()

local rb = rd:createRenderBuffer({width=80, height=45, sampleCount=1, renderTargets={
            {TextureFormat.Depth24Stencil8},
            {TextureFormat.RGBA8},
        }
    }).ensure();

local vb = rd:createVertexBuffer(BufferUsageFlags.Default):ensure()

-- triangle strip data for fullscreen quad
local stripData = {-1.0, -1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 1.0, 1.0, -1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0}
vb:loadData(stripData)

local vl = {
    { DataType.Float32, 2 }, -- vertex
    { DataType.Float32, 2 }  -- texture coords
}
local mesh = rd:createMesh({{vb, vl}}):ensure()

local vertexShaderCodeTex = [[#version 410 core
    layout(location = 0) in vec2 vertex;
    layout(location = 1) in vec2 textureCoordinates;
    out vec2 tc;
    void main()
    {
        tc = textureCoordinates;
        gl_Position = vec4(vertex, 0.0, 1.0);
    }]]

local fragmentShaderCodeTex = [[#version 410 core
    uniform sampler2D tex;
    in vec2 tc;
    out vec4 fragColor;
    void main()
    {
        fragColor = texture(tex, tc);
    }]]

local prog = rd:createProgram(vertexShaderCodeTex, fragmentShaderCodeTex):ensure()
local pipe = rd:createPipeline({program=prog}):ensure()
print(pipe)

local sampler = rd:createSampler({filtering = TextureFiltering.Nearest}):ensure()
print(sampler)

local tb = pipe:texture("tex")
print(tb, rb:colorTargets()[1], #rb:colorTargets())
tb:set(rb:colorTargets()[1], sampler)

local doc = Document("Test Document")
local renderer = TarpRenderer(doc)
doc:setSize(rb:width(), rb:height())
renderer:setViewport(0, 0, rb:width(), rb:height())
renderer:setProjection(Mat4.ortho(0, rb:width(), rb:height(), 0, -1, 1))

local c = doc:createCircle(Vec2(45, 20), 20)
c:setFill("red")
util.path.applyNoise(c, random(0, 255), 200, 3, 1)

print(rb:width(), rb:height())

while not window:shouldClose() do
    pollEvents()

    rd:beginFrame()
    local pass = rd:beginPass({renderBuffer=rb, clear={color={0, 1, 0, 1}}})
    pass:drawCustom(function() return renderer:draw() end)
    rd:endPass(pass)

    pass = rd:beginPass({clear={color={0, 0, 0, 1}}})
    pass:setViewport(0, 0, window:widthInPixels(), window:heightInPixels())
    pass:drawMesh(mesh, pipe, 0, 4, VertexDrawMode.TriangleStrip)
    rd:endPass(pass)

    local err = rd:endFrame()
    if err then print("ERROR") end

    window:swapBuffers()
end

-- destroyRenderDevice(rd)

-- local tr = testResult():ensure()
-- print(tr)
-- local tr2 = testResultFail():ensure()
-- print(tr2)
