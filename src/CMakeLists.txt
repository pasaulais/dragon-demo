set(DEMO_SOURCES
    main.cpp
    SceneViewport.cpp
    RenderState.cpp
    RenderStateGL1.cpp
    RenderStateGL2.cpp
    Mesh.cpp
    Material.cpp
    Vertex.cpp
    Scene.cpp
    Dragon.cpp
    MeshGL1.cpp
    MeshGL2.cpp
    Platform.cpp
)

set(DEMO_HEADERS
    ../include/SceneViewport.h
    ../include/RenderState.h
    ../include/RenderStateGL1.h
    ../include/RenderStateGL2.h
    ../include/Mesh.h
    ../include/Material.h
    ../include/Vertex.h
    ../include/Dragon.h
    ../include/Scene.h
    ../include/MeshGL1.h
    ../include/MeshGL2.h
    ../include/Platform.h
)

set(DEMO_RESOURCES
    ../meshes/meshes.qrc
)

QT4_ADD_RESOURCES(DEMO_RESOURCES_CPP ${DEMO_RESOURCES})

QT4_WRAP_CPP(DEMO_MOC_SOURCES ${DEMO_HEADERS})

source_group(moc FILES ${DEMO_MOC_SOURCES})

add_executable(DragonDemo
    ${DEMO_SOURCES}
    ${DEMO_MOC_SOURCES}
    ${DEMO_RESOURCES}
    ${DEMO_RESOURCES_CPP}
    ${DEMO_HEADERS}
)

target_link_libraries(DragonDemo
    ${QT_LIBRARIES}
    ${GL_LIBRARIES}
    ${SYSTEM_LIBRARIES}
)
