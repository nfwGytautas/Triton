def entry(registry, appstate):
    print('PreExec')
    createMaterials(registry, appstate)
    createEntities(registry, appstate)

def createMaterials(registry, appstate):
    import TritonData
    import TritonMath
    import TritonCore
    mTData = TritonData.TextureData()
    marioTData = TritonData.TextureData()
    marioTData.Fill('D:\\Programming\\Test files\\mario\\Mario\\marioTest.png')

    TestTexture = TritonData.Texture.Create(mTData)
    TestMaterial = TritonData.Material(TestTexture)
    TestMaterial.SetDiffuse(TritonMath.Vector3(1.0, 0.0, 0.0))
    appstate.Store('TestTexture', TritonCore.Texture_Storable(TestTexture))
    appstate.Store('TestMaterial', TritonCore.Material_Storable(TestMaterial))

    MarioTexture = TritonData.Texture.Create(marioTData)
    MarioMaterial = TritonData.Material(MarioTexture)
    MarioMaterial.SetDiffuse(TritonMath.Vector3(0.0, 0.0, 0.0))
    appstate.Store('MarioTexture', TritonCore.Texture_Storable(MarioTexture))
    appstate.Store('MarioMaterial', TritonCore.Material_Storable(MarioMaterial))

    EnviromentTexture = TritonData.Texture.Create(mTData)
    EnviromentMaterial = TritonData.Material(EnviromentTexture)
    EnviromentMaterial.SetDiffuse(TritonMath.Vector3(0.0, 1.0, 1.0))
    appstate.Store('EnviromentTexture', TritonCore.Texture_Storable(EnviromentTexture))
    appstate.Store('EnviromentMaterial', TritonCore.Material_Storable(EnviromentMaterial))

    PipeTexture = TritonData.Texture.Create(mTData)
    PipeMaterial = TritonData.Material(PipeTexture)
    PipeMaterial.SetDiffuse(TritonMath.Vector3(0.0, 1.0, 0.0))
    appstate.Store('PipeTexture', TritonCore.Texture_Storable(PipeTexture))
    appstate.Store('PipeMaterial', TritonCore.Material_Storable(PipeMaterial))

def createEntities(registry, appstate):
    import TritonData
    import TritonMath
    import TritonCore
    import TritonComponents

    mesh = appstate.Take("CubeMesh").value

    createMario(registry, appstate, mesh)
    createGround(registry, appstate, mesh)
    createEnviroment(registry, appstate, mesh)
    createPipes(registry, appstate, mesh)

def createMario(registry, appstate, mesh):
    import TritonComponents
    import TritonMath
    import TritonCore

    #Create Mario Entity
    marioMaterial = appstate.Take("MarioMaterial").value #Get material from appstate
    Mario = registry.Create() #Create in registry

    mTransform = TritonComponents.Transform() #Transform component
    mTransform.Position = TritonMath.Vector3(0.0, 0.0, 1.0)
    registry.AssignTransform(Mario)
    registry.SetTransform(Mario, mTransform)

    registry.AssignMeshFilter(Mario) #MeshFilter
    registry.SetMeshFilter(Mario, TritonComponents.MeshFilter(mesh))
    
    registry.AssignMeshRenderer(Mario) #MeshRenderer
    registry.SetMeshRenderer(Mario, TritonComponents.MeshRenderer(marioMaterial))

    appstate.Store("Mario", TritonCore.Entity_Storable(Mario)) #Store Mario entity in appstate

def createGround(registry, appstate, mesh):
    import TritonComponents
    import TritonMath
    import TritonCore

    #Create ground tiles
    groundMaterial = appstate.Take("TestMaterial").value #Get material from appstate

    ground = list() #Ground list

    for i in range(0, 212):
        groundEntity = registry.Create()
        ground.append(groundEntity)

        transform = TritonComponents.Transform() #Transform component
        transform.Position = TritonMath.Vector3(-20.0 + (2.0 * i), -2.0, 1.0)

        registry.AssignTransform(groundEntity)
        registry.SetTransform(groundEntity, transform)

        registry.AssignMeshFilter(groundEntity) #MeshFilter
        registry.SetMeshFilter(groundEntity, TritonComponents.MeshFilter(mesh))

        registry.AssignMeshRenderer(groundEntity) #MeshRenderer
        registry.SetMeshRenderer(groundEntity, TritonComponents.MeshRenderer(groundMaterial))

    appstate.Store("Ground", TritonCore.List_Storable(ground)) #Store ground entities in appstate
        
def createEnviroment(registry, appstate, mesh):
    import TritonComponents
    import TritonMath
    import TritonCore
    
    #Create other enviroment blocks
    enviromentMaterial = appstate.Take("EnviromentMaterial").value #Get material from appstate

    enviroment = list() #enviroment list

    for i in range(0, 7):
        enviromentEntity = registry.Create()
        enviroment.append(enviromentEntity)

        transform = TritonComponents.Transform() #Transform component
        transform.Position = TritonMath.Vector3(0.0, 0.0, 0.0)

        registry.AssignTransform(enviromentEntity)
        registry.SetTransform(enviromentEntity, transform)

        registry.AssignMeshFilter(enviromentEntity) #MeshFilter
        registry.SetMeshFilter(enviromentEntity, TritonComponents.MeshFilter(mesh))

        registry.AssignMeshRenderer(enviromentEntity) #MeshRenderer
        registry.SetMeshRenderer(enviromentEntity, TritonComponents.MeshRenderer(enviromentMaterial))

    #Position enviroment blocks
    setPosition(registry, enviroment[0], TritonMath.Vector3(10.0, 8.0, 1.0))

    setPosition(registry, enviroment[1], TritonMath.Vector3(20.0, 8.0, 1.0))
    setPosition(registry, enviroment[2], TritonMath.Vector3(22.0, 8.0, 1.0))
    setPosition(registry, enviroment[3], TritonMath.Vector3(24.0, 8.0, 1.0))
    setPosition(registry, enviroment[4], TritonMath.Vector3(26.0, 8.0, 1.0))
    setPosition(registry, enviroment[5], TritonMath.Vector3(28.0, 8.0, 1.0))

    setPosition(registry, enviroment[6], TritonMath.Vector3(24.0, 16.0, 1.0))
    
    appstate.Store("Enviroment", TritonCore.List_Storable(enviroment)) #Store enviroment entities in appstate

def createPipes(registry, appstate, mesh):
    import TritonComponents
    import TritonMath
    import TritonCore

    #Create pipes
    pipeMaterial = appstate.Take("PipeMaterial").value #Get material from appstate

    pipes = list() #enviroment list

    for i in range(0, 4):
        pipeEntity = registry.Create()
        pipes.append(pipeEntity)

        transform = TritonComponents.Transform() #Transform component
        transform.Position = TritonMath.Vector3(0.0, 0.0, 0.0)

        registry.AssignTransform(pipeEntity)
        registry.SetTransform(pipeEntity, transform)

        registry.AssignMeshFilter(pipeEntity) #MeshFilter
        registry.SetMeshFilter(pipeEntity, TritonComponents.MeshFilter(mesh))

        registry.AssignMeshRenderer(pipeEntity) #MeshRenderer
        registry.SetMeshRenderer(pipeEntity, TritonComponents.MeshRenderer(pipeMaterial))

    #Position enviroment blocks
    setPosition(registry, pipes[0], TritonMath.Vector3(38.0, 0.0, 1.0))
    setPosition(registry, pipes[1], TritonMath.Vector3(40.0, 0.0, 1.0))
    setPosition(registry, pipes[2], TritonMath.Vector3(38.0, 2.0, 1.0))
    setPosition(registry, pipes[3], TritonMath.Vector3(40.0, 2.0, 1.0))
    
    appstate.Store("Pipes", TritonCore.List_Storable(pipes)) #Store pipe entities in appstate

def setPosition(registry, entity, value):
    import TritonMath

    #Set position of an entity
    marioTransform = registry.GetTransform(entity)
    marioTransform.Position = value
    registry.SetTransform(entity, marioTransform)
