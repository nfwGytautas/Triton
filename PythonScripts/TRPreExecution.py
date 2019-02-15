from TritonStorage import GameVariables
from TritonStorage import Resources

from TritonMath import (Vector3)
from TritonData import (TextureData, Texture, Material)
from TritonComponents import (Transform, MeshFilter, MeshRenderer)

def entry(registry):
    import User.Setup
    User.Setup.entry(registry)
    return