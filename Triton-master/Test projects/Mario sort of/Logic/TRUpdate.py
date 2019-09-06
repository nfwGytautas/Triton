from TritonCore import (EntityRegistry)

def entry(registry : EntityRegistry, delta):
    import User.Update
    User.Update.entry(registry, delta)
    return