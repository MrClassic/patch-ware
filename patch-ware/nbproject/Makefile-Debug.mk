#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/BiquadFilter.o \
	${OBJECTDIR}/Compressor.o \
	${OBJECTDIR}/Delay.o \
	${OBJECTDIR}/Distortion.o \
	${OBJECTDIR}/FIRFilter.o \
	${OBJECTDIR}/Filter.o \
	${OBJECTDIR}/Gain.o \
	${OBJECTDIR}/IIRFilter.o \
	${OBJECTDIR}/InputDevice.o \
	${OBJECTDIR}/OutputDevice.o \
	${OBJECTDIR}/Parameter.o \
	${OBJECTDIR}/Patch.o \
	${OBJECTDIR}/QuadraticWaveGenerator.o \
	${OBJECTDIR}/SawtoothWaveGenerator.o \
	${OBJECTDIR}/SineWaveGenerator.o \
	${OBJECTDIR}/SquareWaveGenerator.o \
	${OBJECTDIR}/TriangleWaveGenerator.o \
	${OBJECTDIR}/driver.o \
	${OBJECTDIR}/sineTable.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/audio

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/audio: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/audio ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/BiquadFilter.o: BiquadFilter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BiquadFilter.o BiquadFilter.cpp

${OBJECTDIR}/Compressor.o: Compressor.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Compressor.o Compressor.cpp

${OBJECTDIR}/Delay.o: Delay.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Delay.o Delay.cpp

${OBJECTDIR}/Distortion.o: Distortion.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Distortion.o Distortion.cpp

${OBJECTDIR}/FIRFilter.o: FIRFilter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FIRFilter.o FIRFilter.cpp

${OBJECTDIR}/Filter.o: Filter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Filter.o Filter.cpp

${OBJECTDIR}/Gain.o: Gain.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Gain.o Gain.cpp

${OBJECTDIR}/IIRFilter.o: IIRFilter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IIRFilter.o IIRFilter.cpp

${OBJECTDIR}/InputDevice.o: InputDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/InputDevice.o InputDevice.cpp

${OBJECTDIR}/OutputDevice.o: OutputDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OutputDevice.o OutputDevice.cpp

${OBJECTDIR}/Parameter.o: Parameter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Parameter.o Parameter.cpp

${OBJECTDIR}/Patch.o: Patch.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Patch.o Patch.cpp

${OBJECTDIR}/QuadraticWaveGenerator.o: QuadraticWaveGenerator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/QuadraticWaveGenerator.o QuadraticWaveGenerator.cpp

${OBJECTDIR}/SawtoothWaveGenerator.o: SawtoothWaveGenerator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SawtoothWaveGenerator.o SawtoothWaveGenerator.cpp

${OBJECTDIR}/SineWaveGenerator.o: SineWaveGenerator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SineWaveGenerator.o SineWaveGenerator.cpp

${OBJECTDIR}/SquareWaveGenerator.o: SquareWaveGenerator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SquareWaveGenerator.o SquareWaveGenerator.cpp

${OBJECTDIR}/TriangleWaveGenerator.o: TriangleWaveGenerator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TriangleWaveGenerator.o TriangleWaveGenerator.cpp

${OBJECTDIR}/driver.o: driver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/driver.o driver.cpp

${OBJECTDIR}/sineTable.o: sineTable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sineTable.o sineTable.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/audio

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
