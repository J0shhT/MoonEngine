print("TestGame.lua loaded, hello from " .. _VERSION .. "!")

time.startTimer("test")

print("Current Moon Engine version:", version())
print("Current thread identity:", identity())

print("Seconds since Unix Epoch:", time.epoch())
print("Seconds since program start:", time.elapsed())
print("Frame delta:", time.frameDelta())

local elapsed = time.stopTimer("test")
print("Elapsed timer:", elapsed)

print("Current line number:", debug.line())
print(debug.traceback())

print("Vector2:", Vector2.new(5, 5))

warn("This is a warning!")
error("This is an error!")