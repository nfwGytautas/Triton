from TritonCore import (EntityRegistry)
from TritonMath import (Vector3)
from TritonComponents import (Transform, MeshFilter, MeshRenderer)
from TritonData import (TextureData, Texture, Material, MeshData, Mesh)

from TritonStorage import (GameVariables, Resources)

def entry(registry : EntityRegistry):
    addVariables()
    createMaterials(registry)
    createEntities(registry)

def addVariables():
    GameVariables["JumpPower"] = -0.5
    GameVariables["NotFalling"] = True
    GameVariables["Jump"] = False
    GameVariables["MoveLeft"] = False
    GameVariables["MoveRight"] = False
    GameVariables["Collision"] = False

def createMaterials(registry):
    mTData = TextureData()
    marioTData = TextureData()
    marioTData.Fill('D:\\Programming\\Test files\\mario\\Mario\\marioTest.png')

    TestTexture = Texture.Create(mTData)
    TestMaterial = Material(TestTexture)
    TestMaterial.SetDiffuse(Vector3(1.0, 0.0, 0.0))
    Resources["TestTexture"] = TestTexture
    Resources["TestMaterial"] = TestMaterial

    MarioTexture = Texture.Create(marioTData)
    MarioMaterial = Material(MarioTexture)
    MarioMaterial.SetDiffuse(Vector3(0.0, 0.0, 0.0))
    Resources["MarioTexture"] = MarioTexture
    Resources["MarioMaterial"] = MarioMaterial

    EnviromentTexture = Texture.Create(mTData)
    EnviromentMaterial = Material(EnviromentTexture)
    EnviromentMaterial.SetDiffuse(Vector3(0.0, 1.0, 1.0))
    Resources["EnviromentTexture"] = EnviromentTexture
    Resources["EnviromentMaterial"] = EnviromentMaterial

    PipeTexture = Texture.Create(mTData)
    PipeMaterial = Material(PipeTexture)
    PipeMaterial.SetDiffuse(Vector3(0.0, 1.0, 0.0))
    Resources["PipeTexture"] = PipeTexture
    Resources["PipeMaterial"] = PipeMaterial

def createEntities(registry):
    mesh = Resources["CubeMesh"]

    createMario(registry, mesh)
    createGround(registry, mesh)
    createEnviroment(registry, mesh)
    createPipes(registry, mesh)

def createMario(registry, mesh):
    #Create Mario Entity
    marioMaterial = Resources["MarioMaterial"] #Get material from appstate
    Mario = registry.Create() #Create in registry

    mTransform = Transform() #Transform component
    mTransform.Position = Vector3(0.0, 0.1, 1.0)
    registry.AssignTransform(Mario)
    registry.SetTransform(Mario, mTransform)

    registry.AssignMeshFilter(Mario) #MeshFilter
    registry.SetMeshFilter(Mario, MeshFilter(mesh))
    
    registry.AssignMeshRenderer(Mario) #MeshRenderer
    registry.SetMeshRenderer(Mario, MeshRenderer(marioMaterial))

def createGround(registry, mesh):
    #Create ground tiles
    groundMaterial = Resources["TestMaterial"] #Get material from appstate

    ground = list() #Ground list

    for i in range(0, 212):
        groundEntity = registry.Create()
        ground.append(groundEntity)

        transform = Transform() #Transform component
        transform.Position = Vector3(-20.0 + (2.0 * i), -2.0, 1.0)

        registry.AssignTransform(groundEntity)
        registry.SetTransform(groundEntity, transform)

        registry.AssignMeshFilter(groundEntity) #MeshFilter
        registry.SetMeshFilter(groundEntity, MeshFilter(mesh))

        registry.AssignMeshRenderer(groundEntity) #MeshRenderer
        registry.SetMeshRenderer(groundEntity, MeshRenderer(groundMaterial))
       
     #Position enviroment blocks
    import User.WOne
    User.WOne.positionGround(registry, ground)

def createEnviroment(registry, mesh):
    #Create other enviroment blocks
    enviromentMaterial = Resources["EnviromentMaterial"] #Get material from appstate

    enviroment = list() #enviroment list
    
    for i in range(0, 86):
        enviromentEntity = registry.Create()
        enviroment.append(enviromentEntity)

        transform = Transform() #Transform component
        transform.Position = Vector3(0.0, 100.0, 0.0)

        registry.AssignTransform(enviromentEntity)
        registry.SetTransform(enviromentEntity, transform)

        registry.AssignMeshFilter(enviromentEntity) #MeshFilter
        registry.SetMeshFilter(enviromentEntity, MeshFilter(mesh))

        registry.AssignMeshRenderer(enviromentEntity) #MeshRenderer
        registry.SetMeshRenderer(enviromentEntity, MeshRenderer(enviromentMaterial))

    #Position enviroment blocks
    import User.WOne
    User.WOne.positionEnviroment(registry, enviroment)

def createPipes(registry, mesh):
    #Create pipes
    pipeMaterial = Resources["PipeMaterial"] #Get material from appstate

    pipes = list() #enviroment list

    for i in range(0, 34):
        pipeEntity = registry.Create()
        pipes.append(pipeEntity)

        transform = Transform() #Transform component
        transform.Position = Vector3(0.0, 0.0, 0.0)

        registry.AssignTransform(pipeEntity)
        registry.SetTransform(pipeEntity, transform)

        registry.AssignMeshFilter(pipeEntity) #MeshFilter
        registry.SetMeshFilter(pipeEntity, MeshFilter(mesh))

        registry.AssignMeshRenderer(pipeEntity) #MeshRenderer
        registry.SetMeshRenderer(pipeEntity, MeshRenderer(pipeMaterial))

    #Position enviroment blocks
    import User.WOne
    User.WOne.positionPipes(registry, pipes)

def setPosition(registry, entity, value):
    #Set position of an entity
    marioTransform = registry.GetTransform(entity)
    marioTransform.Position = value
    registry.SetTransform(entity, marioTransform)
