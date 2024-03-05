install(
    TARGETS FluidSim
    RUNTIME COMPONENT FluidSim_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif()
