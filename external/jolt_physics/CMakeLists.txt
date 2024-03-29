cmake_minimum_required(VERSION 3.8)

project(libjolt_physics)

# When turning this option on, the library will be compiled using doubles for positions. This allows for much bigger worlds.
option(DOUBLE_PRECISION "Use double precision math" OFF)

# When turning this option on, the library will be compiled with debug symbols
option(GENERATE_DEBUG_SYMBOLS "Generate debug symbols" ON)

# When turning this option on, the library will be compiled in such a way to attempt to keep the simulation deterministic across platforms
option(CROSS_PLATFORM_DETERMINISTIC "Cross platform deterministic" ON)

# When turning this option on, the library will be compiled for ARM (aarch64-linux-gnu), requires compiling with clang
option(CROSS_COMPILE_ARM "Cross compile to aarch64-linux-gnu" OFF)

# When turning this option on, the library will be compiled with interprocedural optimizations enabled, also known as link-time optimizations or link-time code generation.
# Note that if you turn this on you need to use SET_INTERPROCEDURAL_OPTIMIZATION() or set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON) to enable LTO specificly for your own project as well.
# If you don't do this you may get an error: /usr/bin/ld: libJolt.a: error adding symbols: file format not recognized
#option(INTERPROCEDURAL_OPTIMIZATION "Enable interprocedural optimizations" ON)

# When turning this on, in Debug and Release mode, the library will emit extra code to ensure that the 4th component of a 3-vector is kept the same as the 3rd component
# and will enable floating point exceptions during simulation to detect divisions by zero.
# Note that this currently only works using MSVC. Clang turns Float2 into a SIMD vector sometimes causing floating point exceptions (the option is ignored).
option(FLOATING_POINT_EXCEPTIONS_ENABLED "Enable floating point exceptions" ON)

# Number of bits to use in ObjectLayer. Can be 16 or 32.
option(OBJECT_LAYER_BITS "Number of bits in ObjectLayer" 16)

# Select X86 processor features to use (if everything is off it will be SSE2 compatible)
option(USE_SSE4_1 "Enable SSE4.1" ON)
option(USE_SSE4_2 "Enable SSE4.2" ON)
option(USE_AVX "Enable AVX" ON)
option(USE_AVX2 "Enable AVX2" ON)
option(USE_AVX512 "Enable AVX512" OFF)
option(USE_LZCNT "Enable LZCNT" ON)
option(USE_TZCNT "Enable TZCNT" ON)
option(USE_F16C "Enable F16C" ON)
option(USE_FMADD "Enable FMADD" ON)

# Enable all warnings
option(ENABLE_ALL_WARNINGS "Enable all warnings and warnings as errors" ON)

include(CMakeDependentOption)

# Ability to toggle between the static and DLL versions of the MSVC runtime library
# Windows Store only supports the DLL version
cmake_dependent_option(USE_STATIC_MSVC_RUNTIME_LIBRARY "Use the static MSVC runtime library" ON "MSVC;NOT WINDOWS_STORE" OFF)

# Determine which configurations exist
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release;ReleaseASAN;ReleaseUBSAN;ReleaseCoverage;Distribution")
endif()

if (("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows" OR "${CMAKE_SYSTEM_NAME}" STREQUAL "WindowsStore") AND NOT MINGW)
	# Fill in the path to the asan libraries
	set(CLANG_LIB_PATH "\"$(VSInstallDir)\\VC\\Tools\\Llvm\\x64\\lib\\clang\\${CMAKE_CXX_COMPILER_VERSION}\\lib\\windows\"")

	# 64 bit architecture
	set(CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x64")

	# Set runtime library
	if (USE_STATIC_MSVC_RUNTIME_LIBRARY)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()

	# Set general compiler flags
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /Gm- /MP /nologo /diagnostics:classic /FC /fp:except- /Zc:inline")

	# Enable warnings
	if (ENABLE_ALL_WARNINGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /WX")
	endif()

	# Optionally generate debug symbols
	if (GENERATE_DEBUG_SYMBOLS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
	endif()

	# Remove any existing compiler flag that enables RTTI
	string(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

	# Set compiler flag for disabling RTTI
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")

	if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "ARM")
		# On ARM the exception handling flag is missing which causes warnings
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	endif()

	# Set compiler flags for various configurations
	set(CMAKE_CXX_FLAGS_DEBUG "/GS /Od /Ob0 /RTC1")
	set(CMAKE_CXX_FLAGS_RELEASE "/GS- /Gy /O2 /Oi /Ot")
	set(CMAKE_CXX_FLAGS_DISTRIBUTION "/GS- /Gy /O2 /Oi /Ot")
	set(CMAKE_CXX_FLAGS_RELEASEASAN "-fsanitize=address /Od")
	set(CMAKE_CXX_FLAGS_RELEASEUBSAN "-fsanitize=undefined,implicit-conversion,float-divide-by-zero,local-bounds -fno-sanitize-recover=all")
	set(CMAKE_CXX_FLAGS_RELEASECOVERAGE "-fprofile-instr-generate -fcoverage-mapping")

	# Set linker flags
	set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS /ignore:4221 /DEBUG:FASTLINK")
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		if (CROSS_PLATFORM_DETERMINISTIC)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:precise")
		else()
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast") # Clang doesn't use fast math because it cannot be turned off inside a single compilation unit
		endif()
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /showFilenames")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments") # Clang emits warnings about unused arguments such as /MP and /GL
		set(CMAKE_EXE_LINKER_FLAGS_RELEASEASAN "/SUBSYSTEM:CONSOLE /LIBPATH:${CLANG_LIB_PATH} clang_rt.asan-x86_64.lib -wholearchive:clang_rt.asan-x86_64.lib clang_rt.asan_cxx-x86_64.lib -wholearchive:clang_rt.asan_cxx-x86_64.lib")
		set(CMAKE_EXE_LINKER_FLAGS_RELEASEUBSAN "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LIBPATH:${CLANG_LIB_PATH}")
		set(CMAKE_EXE_LINKER_FLAGS_RELEASECOVERAGE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LIBPATH:${CLANG_LIB_PATH}")
	endif()
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" OR "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" OR "${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR MINGW OR EMSCRIPTEN)
	# Set general compiler flags
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I.")

	# Enable warnings
	if (ENABLE_ALL_WARNINGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
	endif()

	# Optionally generate debug symbols
	if (GENERATE_DEBUG_SYMBOLS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
	endif()

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		# Somehow -Wcomment doesn't want to be turned off from code and we need this because Doxygen MathJax uses it
		# Also disable -Wstringop-overflow or it will generate false positives that can't be disabled from code when link-time optimizations are enabled
		# Also turn off automatic fused multiply add contractions, there doesn't seem to be a way to do this selectively through the macro JPH_PRECISE_MATH_OFF
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-comment -Wno-stringop-overflow -ffp-contract=off")
	else()
		# Do not use -ffast-math since it cannot be turned off in a single compilation unit under clang, see Core.h
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffp-model=precise")

		# On clang 14 and later we can turn off float contraction through a pragma, older versions and deterministic versions need it off always, see Core.h
		if (CMAKE_CXX_COMPILER_VERSION LESS 14 OR CROSS_PLATFORM_DETERMINISTIC)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffp-contract=off")
		endif()
	endif()

	# Cross compiler flags
	if (CROSS_COMPILE_ARM)
		set(CMAKE_CXX_FLAGS "--target=aarch64-linux-gnu ${CMAKE_CXX_FLAGS}")
	endif()

	# Set compiler flags for various configurations
	set(CMAKE_CXX_FLAGS_DEBUG "")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")
	set(CMAKE_CXX_FLAGS_RELEASEASAN "-fsanitize=address")
	set(CMAKE_CXX_FLAGS_RELEASEUBSAN "-fsanitize=undefined,implicit-conversion,float-divide-by-zero,local-bounds -fno-sanitize-recover=all")
	set(CMAKE_CXX_FLAGS_RELEASECOVERAGE "-fprofile-instr-generate -fcoverage-mapping")

	# Set linker flags
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

# Set linker flags
set(CMAKE_EXE_LINKER_FLAGS_DISTRIBUTION "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

## Enable link time optimization in Release and Distribution mode if requested and available
#function(SET_INTERPROCEDURAL_OPTIMIZATION)
#	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE OFF PARENT_SCOPE)
#	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DISTRIBUTION OFF PARENT_SCOPE)
#
#	# On ARM, whole program optimization triggers an internal compiler error during code gen, so we don't turn it on
#	if (INTERPROCEDURAL_OPTIMIZATION AND NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "ARM64") AND NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "ARM"))
#		include(CheckIPOSupported)
#		check_ipo_supported(RESULT IS_IPO_SUPPORTED OUTPUT IPO_CHECK_OUTPUT)
#
#		if (IS_IPO_SUPPORTED)
#			set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON PARENT_SCOPE)
#			set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DISTRIBUTION ON PARENT_SCOPE)
#		else()
#			message(WARNING "Interprocedural optimizations are not supported: ${IPO_CHECK_OUTPUT}")
#		endif()
#	endif()
#endfunction()
#SET_INTERPROCEDURAL_OPTIMIZATION()

# Set repository root
set(PHYSICS_REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Make Jolt Library

# Root
set(JOLT_PHYSICS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Jolt)

# Source files
set(JOLT_PHYSICS_SRC_FILES
	${JOLT_PHYSICS_ROOT}/AABBTree/AABBTreeBuilder.cpp
	${JOLT_PHYSICS_ROOT}/AABBTree/AABBTreeBuilder.h
	${JOLT_PHYSICS_ROOT}/AABBTree/AABBTreeToBuffer.h
	${JOLT_PHYSICS_ROOT}/AABBTree/NodeCodec/NodeCodecQuadTreeHalfFloat.h
	${JOLT_PHYSICS_ROOT}/AABBTree/TriangleCodec/TriangleCodecIndexed8BitPackSOA4Flags.h
	${JOLT_PHYSICS_ROOT}/Core/ARMNeon.h
	${JOLT_PHYSICS_ROOT}/Core/Atomics.h
	${JOLT_PHYSICS_ROOT}/Core/ByteBuffer.h
	${JOLT_PHYSICS_ROOT}/Core/Color.cpp
	${JOLT_PHYSICS_ROOT}/Core/Color.h
	${JOLT_PHYSICS_ROOT}/Core/Core.h
	${JOLT_PHYSICS_ROOT}/Core/Factory.cpp
	${JOLT_PHYSICS_ROOT}/Core/Factory.h
	${JOLT_PHYSICS_ROOT}/Core/FixedSizeFreeList.h
	${JOLT_PHYSICS_ROOT}/Core/FixedSizeFreeList.inl
	${JOLT_PHYSICS_ROOT}/Core/FPControlWord.h
	${JOLT_PHYSICS_ROOT}/Core/FPException.h
	${JOLT_PHYSICS_ROOT}/Core/FPFlushDenormals.h
	${JOLT_PHYSICS_ROOT}/Core/HashCombine.h
	${JOLT_PHYSICS_ROOT}/Core/InsertionSort.h
	${JOLT_PHYSICS_ROOT}/Core/IssueReporting.cpp
	${JOLT_PHYSICS_ROOT}/Core/IssueReporting.h
	${JOLT_PHYSICS_ROOT}/Core/JobSystem.h
	${JOLT_PHYSICS_ROOT}/Core/JobSystem.inl
	${JOLT_PHYSICS_ROOT}/Core/JobSystemThreadPool.cpp
	${JOLT_PHYSICS_ROOT}/Core/JobSystemThreadPool.h
	${JOLT_PHYSICS_ROOT}/Core/JobSystemWithBarrier.cpp
	${JOLT_PHYSICS_ROOT}/Core/JobSystemWithBarrier.h
	${JOLT_PHYSICS_ROOT}/Core/LinearCurve.cpp
	${JOLT_PHYSICS_ROOT}/Core/LinearCurve.h
	${JOLT_PHYSICS_ROOT}/Core/LockFreeHashMap.h
	${JOLT_PHYSICS_ROOT}/Core/LockFreeHashMap.inl
	${JOLT_PHYSICS_ROOT}/Core/Memory.cpp
	${JOLT_PHYSICS_ROOT}/Core/Memory.h
	${JOLT_PHYSICS_ROOT}/Core/Mutex.h
	${JOLT_PHYSICS_ROOT}/Core/MutexArray.h
	${JOLT_PHYSICS_ROOT}/Core/NonCopyable.h
	${JOLT_PHYSICS_ROOT}/Core/Profiler.cpp
	${JOLT_PHYSICS_ROOT}/Core/Profiler.h
	${JOLT_PHYSICS_ROOT}/Core/Profiler.inl
	${JOLT_PHYSICS_ROOT}/Core/QuickSort.h
	${JOLT_PHYSICS_ROOT}/Core/Reference.h
	${JOLT_PHYSICS_ROOT}/Core/Result.h
	${JOLT_PHYSICS_ROOT}/Core/RTTI.cpp
	${JOLT_PHYSICS_ROOT}/Core/RTTI.h
	${JOLT_PHYSICS_ROOT}/Core/Semaphore.cpp
	${JOLT_PHYSICS_ROOT}/Core/Semaphore.h
	${JOLT_PHYSICS_ROOT}/Core/StaticArray.h
	${JOLT_PHYSICS_ROOT}/Core/StreamIn.h
	${JOLT_PHYSICS_ROOT}/Core/StreamOut.h
	${JOLT_PHYSICS_ROOT}/Core/StreamWrapper.h
	${JOLT_PHYSICS_ROOT}/Core/StringTools.cpp
	${JOLT_PHYSICS_ROOT}/Core/StringTools.h
	${JOLT_PHYSICS_ROOT}/Core/STLAlignedAllocator.h
	${JOLT_PHYSICS_ROOT}/Core/STLAllocator.h
	${JOLT_PHYSICS_ROOT}/Core/STLTempAllocator.h
	${JOLT_PHYSICS_ROOT}/Core/TempAllocator.h
	${JOLT_PHYSICS_ROOT}/Core/TickCounter.cpp
	${JOLT_PHYSICS_ROOT}/Core/TickCounter.h
	${JOLT_PHYSICS_ROOT}/Core/UnorderedMap.h
	${JOLT_PHYSICS_ROOT}/Core/UnorderedSet.h
	${JOLT_PHYSICS_ROOT}/Geometry/AABox.h
	${JOLT_PHYSICS_ROOT}/Geometry/AABox4.h
	${JOLT_PHYSICS_ROOT}/Geometry/ClipPoly.h
	${JOLT_PHYSICS_ROOT}/Geometry/ClosestPoint.h
	${JOLT_PHYSICS_ROOT}/Geometry/ConvexHullBuilder.cpp
	${JOLT_PHYSICS_ROOT}/Geometry/ConvexHullBuilder.h
	${JOLT_PHYSICS_ROOT}/Geometry/ConvexHullBuilder2D.cpp
	${JOLT_PHYSICS_ROOT}/Geometry/ConvexHullBuilder2D.h
	${JOLT_PHYSICS_ROOT}/Geometry/ConvexSupport.h
	${JOLT_PHYSICS_ROOT}/Geometry/Ellipse.h
	${JOLT_PHYSICS_ROOT}/Geometry/EPAConvexHullBuilder.h
	${JOLT_PHYSICS_ROOT}/Geometry/EPAPenetrationDepth.h
	${JOLT_PHYSICS_ROOT}/Geometry/GJKClosestPoint.h
	${JOLT_PHYSICS_ROOT}/Geometry/IndexedTriangle.h
	${JOLT_PHYSICS_ROOT}/Geometry/Indexify.cpp
	${JOLT_PHYSICS_ROOT}/Geometry/Indexify.h
	${JOLT_PHYSICS_ROOT}/Geometry/MortonCode.h
	${JOLT_PHYSICS_ROOT}/Geometry/OrientedBox.cpp
	${JOLT_PHYSICS_ROOT}/Geometry/OrientedBox.h
	${JOLT_PHYSICS_ROOT}/Geometry/Plane.h
	${JOLT_PHYSICS_ROOT}/Geometry/RayAABox.h
	${JOLT_PHYSICS_ROOT}/Geometry/RayAABox8.h
	${JOLT_PHYSICS_ROOT}/Geometry/RayCapsule.h
	${JOLT_PHYSICS_ROOT}/Geometry/RayCylinder.h
	${JOLT_PHYSICS_ROOT}/Geometry/RaySphere.h
	${JOLT_PHYSICS_ROOT}/Geometry/RayTriangle.h
	${JOLT_PHYSICS_ROOT}/Geometry/RayTriangle8.h
	${JOLT_PHYSICS_ROOT}/Geometry/Sphere.h
	${JOLT_PHYSICS_ROOT}/Geometry/Triangle.h
	${JOLT_PHYSICS_ROOT}/Jolt.h
	${JOLT_PHYSICS_ROOT}/Math/DMat44.h
	${JOLT_PHYSICS_ROOT}/Math/DMat44.inl
	${JOLT_PHYSICS_ROOT}/Math/Double3.h
	${JOLT_PHYSICS_ROOT}/Math/DVec3.h
	${JOLT_PHYSICS_ROOT}/Math/DVec3.inl
	${JOLT_PHYSICS_ROOT}/Math/DynMatrix.h
	${JOLT_PHYSICS_ROOT}/Math/EigenValueSymmetric.h
	${JOLT_PHYSICS_ROOT}/Math/FindRoot.h
	${JOLT_PHYSICS_ROOT}/Math/Float2.h
	${JOLT_PHYSICS_ROOT}/Math/Float3.h
	${JOLT_PHYSICS_ROOT}/Math/Float4.h
	${JOLT_PHYSICS_ROOT}/Math/GaussianElimination.h
	${JOLT_PHYSICS_ROOT}/Math/HalfFloat.h
	${JOLT_PHYSICS_ROOT}/Math/Mat44.h
	${JOLT_PHYSICS_ROOT}/Math/Mat44.inl
	${JOLT_PHYSICS_ROOT}/Math/Math.h
	${JOLT_PHYSICS_ROOT}/Math/MathTypes.h
	${JOLT_PHYSICS_ROOT}/Math/Matrix.h
	${JOLT_PHYSICS_ROOT}/Math/Quat.h
	${JOLT_PHYSICS_ROOT}/Math/Quat.inl
	${JOLT_PHYSICS_ROOT}/Math/Real.h
	${JOLT_PHYSICS_ROOT}/Math/Swizzle.h
	${JOLT_PHYSICS_ROOT}/Math/Trigonometry.h
	${JOLT_PHYSICS_ROOT}/Math/UVec4.cpp
	${JOLT_PHYSICS_ROOT}/Math/UVec4.h
	${JOLT_PHYSICS_ROOT}/Math/UVec4.inl
	${JOLT_PHYSICS_ROOT}/Math/UVec8.h
	${JOLT_PHYSICS_ROOT}/Math/UVec8.inl
	${JOLT_PHYSICS_ROOT}/Math/Vec3.cpp
	${JOLT_PHYSICS_ROOT}/Math/Vec3.h
	${JOLT_PHYSICS_ROOT}/Math/Vec3.inl
	${JOLT_PHYSICS_ROOT}/Math/Vec4.h
	${JOLT_PHYSICS_ROOT}/Math/Vec4.inl
	${JOLT_PHYSICS_ROOT}/Math/Vec8.h
	${JOLT_PHYSICS_ROOT}/Math/Vec8.inl
	${JOLT_PHYSICS_ROOT}/Math/Vector.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/GetPrimitiveTypeOfType.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStream.cpp
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStream.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamBinaryIn.cpp
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamBinaryIn.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamBinaryOut.cpp
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamBinaryOut.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamIn.cpp
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamIn.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamOut.cpp
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamOut.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamTextIn.cpp
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamTextIn.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamTextOut.cpp
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamTextOut.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/ObjectStreamTypes.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/SerializableAttribute.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/SerializableAttributeEnum.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/SerializableAttributeTyped.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/SerializableObject.cpp
	${JOLT_PHYSICS_ROOT}/ObjectStream/SerializableObject.h
	${JOLT_PHYSICS_ROOT}/ObjectStream/TypeDeclarations.cpp
	${JOLT_PHYSICS_ROOT}/ObjectStream/TypeDeclarations.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/Body.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Body/Body.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/Body.inl
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyAccess.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyAccess.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyActivationListener.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyCreationSettings.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyCreationSettings.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyFilter.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyID.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyInterface.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyInterface.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyLock.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyLockInterface.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyLockMulti.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyManager.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyManager.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/BodyPair.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/MassProperties.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Body/MassProperties.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/MotionProperties.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Body/MotionProperties.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/MotionProperties.inl
	${JOLT_PHYSICS_ROOT}/Physics/Body/MotionQuality.h
	${JOLT_PHYSICS_ROOT}/Physics/Body/MotionType.h
	${JOLT_PHYSICS_ROOT}/Physics/Character/Character.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Character/Character.h
	${JOLT_PHYSICS_ROOT}/Physics/Character/CharacterBase.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Character/CharacterBase.h
	${JOLT_PHYSICS_ROOT}/Physics/Character/CharacterVirtual.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Character/CharacterVirtual.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/AABoxCast.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/ActiveEdgeMode.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/ActiveEdges.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BackFaceMode.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BroadPhase/BroadPhase.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BroadPhase/BroadPhase.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BroadPhase/BroadPhaseBruteForce.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BroadPhase/BroadPhaseBruteForce.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BroadPhase/BroadPhaseLayer.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BroadPhase/BroadPhaseQuadTree.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BroadPhase/BroadPhaseQuadTree.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BroadPhase/BroadPhaseQuery.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BroadPhase/QuadTree.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/BroadPhase/QuadTree.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CastConvexVsTriangles.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CastConvexVsTriangles.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CastSphereVsTriangles.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CastSphereVsTriangles.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CastResult.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollectFacesMode.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollideConvexVsTriangles.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollideConvexVsTriangles.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollidePointResult.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollideShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollideSphereVsTriangles.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollideSphereVsTriangles.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollisionCollector.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollisionCollectorImpl.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollisionDispatch.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollisionDispatch.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollisionGroup.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/CollisionGroup.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/ContactListener.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/EstimateCollisionResponse.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/EstimateCollisionResponse.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/GroupFilter.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/GroupFilter.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/GroupFilterTable.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/GroupFilterTable.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/ManifoldBetweenTwoFaces.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/ManifoldBetweenTwoFaces.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/NarrowPhaseQuery.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/NarrowPhaseQuery.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/NarrowPhaseStats.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/NarrowPhaseStats.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/ObjectLayer.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/PhysicsMaterial.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/PhysicsMaterial.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/PhysicsMaterialSimple.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/PhysicsMaterialSimple.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/RayCast.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/BoxShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/BoxShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/CapsuleShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/CapsuleShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/CompoundShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/CompoundShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/CompoundShapeVisitors.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/ConvexHullShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/ConvexHullShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/ConvexShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/ConvexShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/CylinderShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/CylinderShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/DecoratedShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/DecoratedShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/GetTrianglesContext.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/HeightFieldShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/HeightFieldShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/MeshShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/MeshShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/MutableCompoundShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/MutableCompoundShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/OffsetCenterOfMassShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/OffsetCenterOfMassShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/PolyhedronSubmergedVolumeCalculator.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/RotatedTranslatedShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/RotatedTranslatedShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/ScaledShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/ScaledShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/ScaleHelpers.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/Shape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/Shape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/SphereShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/SphereShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/StaticCompoundShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/StaticCompoundShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/SubShapeID.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/SubShapeIDPair.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/TaperedCapsuleShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/TaperedCapsuleShape.gliffy
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/TaperedCapsuleShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/TriangleShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/Shape/TriangleShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/ShapeCast.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/ShapeFilter.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/SortReverseAndStore.h
	${JOLT_PHYSICS_ROOT}/Physics/Collision/TransformedShape.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Collision/TransformedShape.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConeConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConeConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/Constraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/Constraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintManager.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintManager.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/AngleConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/AxisConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/DualAxisConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/GearConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/HingeRotationConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/IndependentAxisConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/PointConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/RackAndPinionConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/RotationEulerConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/RotationQuatConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/SpringPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ConstraintPart/SwingTwistConstraintPart.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ContactConstraintManager.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/ContactConstraintManager.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/DistanceConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/DistanceConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/FixedConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/FixedConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/GearConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/GearConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/HingeConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/HingeConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/MotorSettings.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/MotorSettings.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/PathConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/PathConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/PathConstraintPath.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/PathConstraintPath.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/PathConstraintPathHermite.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/PathConstraintPathHermite.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/PointConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/PointConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/PulleyConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/PulleyConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/RackAndPinionConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/RackAndPinionConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/SixDOFConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/SixDOFConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/SliderConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/SliderConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/SwingTwistConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/SwingTwistConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/TwoBodyConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Constraints/TwoBodyConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/DeterminismLog.cpp
	${JOLT_PHYSICS_ROOT}/Physics/DeterminismLog.h
	${JOLT_PHYSICS_ROOT}/Physics/EActivation.h
	${JOLT_PHYSICS_ROOT}/Physics/EPhysicsUpdateError.h
	${JOLT_PHYSICS_ROOT}/Physics/IslandBuilder.cpp
	${JOLT_PHYSICS_ROOT}/Physics/IslandBuilder.h
	${JOLT_PHYSICS_ROOT}/Physics/LargeIslandSplitter.cpp
	${JOLT_PHYSICS_ROOT}/Physics/LargeIslandSplitter.h
	${JOLT_PHYSICS_ROOT}/Physics/PhysicsLock.cpp
	${JOLT_PHYSICS_ROOT}/Physics/PhysicsLock.h
	${JOLT_PHYSICS_ROOT}/Physics/PhysicsScene.cpp
	${JOLT_PHYSICS_ROOT}/Physics/PhysicsScene.h
	${JOLT_PHYSICS_ROOT}/Physics/PhysicsSettings.h
	${JOLT_PHYSICS_ROOT}/Physics/PhysicsStepListener.h
	${JOLT_PHYSICS_ROOT}/Physics/PhysicsSystem.cpp
	${JOLT_PHYSICS_ROOT}/Physics/PhysicsSystem.h
	${JOLT_PHYSICS_ROOT}/Physics/PhysicsUpdateContext.cpp
	${JOLT_PHYSICS_ROOT}/Physics/PhysicsUpdateContext.h
	${JOLT_PHYSICS_ROOT}/Physics/Ragdoll/Ragdoll.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Ragdoll/Ragdoll.h
	${JOLT_PHYSICS_ROOT}/Physics/StateRecorder.h
	${JOLT_PHYSICS_ROOT}/Physics/StateRecorderImpl.cpp
	${JOLT_PHYSICS_ROOT}/Physics/StateRecorderImpl.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/MotorcycleController.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/MotorcycleController.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/TrackedVehicleController.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/TrackedVehicleController.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleAntiRollBar.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleAntiRollBar.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleCollisionTester.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleCollisionTester.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleConstraint.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleConstraint.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleController.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleController.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleDifferential.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleDifferential.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleEngine.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleEngine.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleTrack.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleTrack.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleTransmission.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/VehicleTransmission.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/Wheel.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/Wheel.h
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/WheeledVehicleController.cpp
	${JOLT_PHYSICS_ROOT}/Physics/Vehicle/WheeledVehicleController.h
	${JOLT_PHYSICS_ROOT}/RegisterTypes.cpp
	${JOLT_PHYSICS_ROOT}/RegisterTypes.h
	${JOLT_PHYSICS_ROOT}/Renderer/DebugRenderer.cpp
	${JOLT_PHYSICS_ROOT}/Renderer/DebugRenderer.h
	${JOLT_PHYSICS_ROOT}/Renderer/DebugRendererPlayback.cpp
	${JOLT_PHYSICS_ROOT}/Renderer/DebugRendererPlayback.h
	${JOLT_PHYSICS_ROOT}/Renderer/DebugRendererRecorder.cpp
	${JOLT_PHYSICS_ROOT}/Renderer/DebugRendererRecorder.h
	${JOLT_PHYSICS_ROOT}/Skeleton/SkeletalAnimation.cpp
	${JOLT_PHYSICS_ROOT}/Skeleton/SkeletalAnimation.h
	${JOLT_PHYSICS_ROOT}/Skeleton/Skeleton.cpp
	${JOLT_PHYSICS_ROOT}/Skeleton/Skeleton.h
	${JOLT_PHYSICS_ROOT}/Skeleton/SkeletonMapper.cpp
	${JOLT_PHYSICS_ROOT}/Skeleton/SkeletonMapper.h
	${JOLT_PHYSICS_ROOT}/Skeleton/SkeletonPose.cpp
	${JOLT_PHYSICS_ROOT}/Skeleton/SkeletonPose.h
	${JOLT_PHYSICS_ROOT}/TriangleGrouper/TriangleGrouper.h
	${JOLT_PHYSICS_ROOT}/TriangleGrouper/TriangleGrouperClosestCentroid.cpp
	${JOLT_PHYSICS_ROOT}/TriangleGrouper/TriangleGrouperClosestCentroid.h
	${JOLT_PHYSICS_ROOT}/TriangleGrouper/TriangleGrouperMorton.cpp
	${JOLT_PHYSICS_ROOT}/TriangleGrouper/TriangleGrouperMorton.h
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitter.cpp
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitter.h
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitterBinning.cpp
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitterBinning.h
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitterFixedLeafSize.cpp
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitterFixedLeafSize.h
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitterLongestAxis.cpp
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitterLongestAxis.h
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitterMean.cpp
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitterMean.h
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitterMorton.cpp
	${JOLT_PHYSICS_ROOT}/TriangleSplitter/TriangleSplitterMorton.h
	)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	# Add natvis file
	set(JOLT_PHYSICS_SRC_FILES ${JOLT_PHYSICS_SRC_FILES} ${JOLT_PHYSICS_ROOT}/Jolt.natvis)
endif()

# Group source files
source_group(TREE ${JOLT_PHYSICS_ROOT} FILES ${JOLT_PHYSICS_SRC_FILES})

# Create Jolt lib
add_library(${PROJECT_NAME} STATIC ${JOLT_PHYSICS_SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "external")
target_include_directories(${PROJECT_NAME} PUBLIC ${PHYSICS_REPO_ROOT})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${JOLT_PHYSICS_ROOT}/Jolt.h)
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:_DEBUG;JPH_PROFILE_ENABLED;JPH_DEBUG_RENDERER>")
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Release>:NDEBUG;JPH_PROFILE_ENABLED;JPH_DEBUG_RENDERER>")
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Distribution>:NDEBUG>")
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:ReleaseASAN>:NDEBUG;JPH_PROFILE_ENABLED;JPH_DISABLE_TEMP_ALLOCATOR;JPH_DISABLE_CUSTOM_ALLOCATOR;JPH_DEBUG_RENDERER>")
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:ReleaseUBSAN>:NDEBUG;JPH_PROFILE_ENABLED;JPH_DEBUG_RENDERER>")
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:ReleaseCoverage>:NDEBUG>")

# Setting floating point exceptions
if (FLOATING_POINT_EXCEPTIONS_ENABLED AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:JPH_FLOATING_POINT_EXCEPTIONS_ENABLED>")
	target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Release>:JPH_FLOATING_POINT_EXCEPTIONS_ENABLED>")
endif()

# Setting double precision flag
if (DOUBLE_PRECISION)
	target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_DOUBLE_PRECISION)
endif()

# Setting to attempt cross platform determinism
if (CROSS_PLATFORM_DETERMINISTIC)
	target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_CROSS_PLATFORM_DETERMINISTIC)
endif()

# Setting to determine number of bits in ObjectLayer
if (OBJECT_LAYER_BITS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_OBJECT_LAYER_BITS=${OBJECT_LAYER_BITS})
endif()

# Emit the instruction set definitions to ensure that child projects use the same settings even if they override the used instruction sets (a mismatch causes link errors)
function(EMIT_X86_INSTRUCTION_SET_DEFINITIONS)
	if (USE_AVX512)
		target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_USE_AVX512)
	endif()
	if (USE_AVX2)
		target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_USE_AVX2)
	endif()
	if (USE_AVX)
		target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_USE_AVX)
	endif()
	if (USE_SSE4_1)
		target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_USE_SSE4_1)
	endif()
	if (USE_SSE4_2)
		target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_USE_SSE4_2)
	endif()
	if (USE_LZCNT)
		target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_USE_LZCNT)
	endif()
	if (USE_TZCNT)
		target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_USE_TZCNT)
	endif()
	if (USE_F16C)
		target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_USE_F16C)
	endif()
	if (USE_FMADD AND NOT CROSS_PLATFORM_DETERMINISTIC)
		target_compile_definitions(${PROJECT_NAME} PUBLIC JPH_USE_FMADD)
	endif()
endfunction()

# Add the compiler commandline flags to select the right instruction sets
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86" OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
		if (USE_AVX512)
			target_compile_options(${PROJECT_NAME} PUBLIC /arch:AVX512)
		elseif (USE_AVX2)
			target_compile_options(${PROJECT_NAME} PUBLIC /arch:AVX2)
		elseif (USE_AVX)
			target_compile_options(${PROJECT_NAME} PUBLIC /arch:AVX)
		endif()
		EMIT_X86_INSTRUCTION_SET_DEFINITIONS()
	endif()
else()
	if (CROSS_COMPILE_ARM OR CMAKE_OSX_ARCHITECTURES MATCHES "arm64" OR "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
		# ARM64 uses no special commandline flags
	elseif ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" OR "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "AMD64")
		# x64
		if (USE_AVX512)
			target_compile_options(${PROJECT_NAME} PUBLIC -mavx512f -mavx512vl -mavx512dq -mavx2 -mbmi -mpopcnt -mlzcnt -mf16c)
		elseif (USE_AVX2)
			target_compile_options(${PROJECT_NAME} PUBLIC -mavx2 -mbmi -mpopcnt -mlzcnt -mf16c)
		elseif (USE_AVX)
			target_compile_options(${PROJECT_NAME} PUBLIC -mavx -mpopcnt)
		elseif (USE_SSE4_2)
			target_compile_options(${PROJECT_NAME} PUBLIC -msse4.2 -mpopcnt)
		elseif (USE_SSE4_1)
			target_compile_options(${PROJECT_NAME} PUBLIC -msse4.1)
		else()
			target_compile_options(${PROJECT_NAME} PUBLIC -msse2)
		endif()
		if (USE_LZCNT)
			target_compile_options(${PROJECT_NAME} PUBLIC -mlzcnt)
		endif()
		if (USE_TZCNT)
			target_compile_options(${PROJECT_NAME} PUBLIC -mbmi)
		endif()
		if (USE_F16C)
			target_compile_options(${PROJECT_NAME} PUBLIC -mf16c)
		endif()
		if (USE_FMADD AND NOT CROSS_PLATFORM_DETERMINISTIC)
			target_compile_options(${PROJECT_NAME} PUBLIC -mfma)
		endif()
		EMIT_X86_INSTRUCTION_SET_DEFINITIONS()
	endif()
endif()

if (IOS)
	# Ensure that we enable SSE4.2 for the x86_64 build, CMAKE_SYSTEM_PROCESSOR is not set for iOS
	set_property(TARGET ${PROJECT_NAME} PROPERTY XCODE_ATTRIBUTE_OTHER_CPLUSPLUSFLAGS[arch=x86_64] "$(inherited) -msse4.2 -mpopcnt")
endif()
