from TritonCore import (EntityRegistry)
from TritonMath import (Vector3)
from TritonComponents import (Transform)
from TritonStorage import GameVariables

def entry(registry : EntityRegistry, delta):
    gravity(registry, delta)

def gravity(registry, delta):
    if GameVariables["JumpPower"] > -3.6:
        GameVariables["JumpPower"] -= 0.1
    
    marioTransform = registry.GetTransform(0)

    marioTransform.Position.y += GameVariables["JumpPower"] * delta * 10
    registry.SetTransform(0, marioTransform)