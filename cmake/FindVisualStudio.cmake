set(PROGRAM_FILES
    "$ENV{ProgramFiles\(x86\)}"
    "$ENV{ProgramFiles}"
)

set(VS_BASE_PATHS
    "Microsoft Visual Studio/18"
    "Microsoft Visual Studio/2022"
)

foreach (base_path ${VS_BASE_PATHS})
    foreach (check ${PROGRAM_FILES})
        if (EXISTS "${check}/${base_path}")
            set(VS_PATH "${check}/${base_path}")
            break()
        endif ()
    endforeach ()
    if (VS_PATH)
        break()
    endif ()
endforeach ()
if (NOT VS_PATH)
    message(FATAL_ERROR "Visual Studio not found")
endif ()
if(EXISTS "${VS_PATH}/Community")
    set(VS_EDITION "Community")
elseif(EXISTS "${VS_PATH}/Professional")
    set(VS_EDITION "Professional")
elseif(EXISTS "${VS_PATH}/Enterprise")
    set(VS_EDITION "Enterprise")
elseif(EXISTS "${VS_PATH}/BuildTools")
    set(VS_EDITION "BuildTools")
else()
    message(FATAL_ERROR "Visual Studio edition not found")
endif()
set(VS_TOOLCHAIN_PATH "${VS_PATH}/${VS_EDITION}/VC/Tools/MSVC")

file(GLOB VS_VERSION_DIR "${VS_TOOLCHAIN_PATH}/*")
list(GET VS_VERSION_DIR 0 VS_LATEST_VERSION)
if(NOT EXISTS "${VS_LATEST_VERSION}")
    message(FATAL_ERROR "Visual Studio version not found")
endif ()
string(REGEX REPLACE "\\\\" "/" VS_LATEST_VERSION "${VS_LATEST_VERSION}")

set(VS_TARGET_ARCH x64)
if (DEFINED NTG_TARGET_ARCH AND NTG_TARGET_ARCH STREQUAL "x86")
    set(VS_TARGET_ARCH x86)
endif ()

set(CMAKE_GENERATOR "Ninja")
set(CMAKE_C_COMPILER "${VS_LATEST_VERSION}/bin/Hostx64/${VS_TARGET_ARCH}/cl.exe")
set(CMAKE_CXX_COMPILER "${VS_LATEST_VERSION}/bin/Hostx64/${VS_TARGET_ARCH}/cl.exe")
set(CMAKE_STRIP "${VS_LATEST_VERSION}/bin/Hostx64/${VS_TARGET_ARCH}/editbin.exe")
set(LIB_ASSEMBLER "${VS_LATEST_VERSION}/bin/Hostx64/${VS_TARGET_ARCH}/lib.exe")
