def entry(registry, appstate, delta):
    print('OnUpdate')
    gravity(registry, appstate, delta)


def gravity(registry, appstate, delta):
    if appstate.Take('JumpPower').value >= -0.5:
        appstate.Take('JumpPower').value -= 0.02 * delta

    marioTransform = registry.GetTransform(0)

    marioTransform.Position.y += appstate.Take('JumpPower').value * delta
    registry.SetTransform(0, marioTransform)

    