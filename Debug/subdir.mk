################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Camera.c \
../Clavier.c \
../Cube.c \
../FenetreAjoutCube.c \
../FenetreAjoutRectangle.c \
../FenetrePropriete.c \
../Groupe.c \
../MatrixTools.c \
../Modification.c \
../Objet.c \
../Point.c \
../ProjectionTools.c \
../Rectangle.c \
../Scene.c \
../Segment.c \
../Selection.c \
../TransfoTools.c \
../main.c 

OBJS += \
./Camera.o \
./Clavier.o \
./Cube.o \
./FenetreAjoutCube.o \
./FenetreAjoutRectangle.o \
./FenetrePropriete.o \
./Groupe.o \
./MatrixTools.o \
./Modification.o \
./Objet.o \
./Point.o \
./ProjectionTools.o \
./Rectangle.o \
./Scene.o \
./Segment.o \
./Selection.o \
./TransfoTools.o \
./main.o 

C_DEPS += \
./Camera.d \
./Clavier.d \
./Cube.d \
./FenetreAjoutCube.d \
./FenetreAjoutRectangle.d \
./FenetrePropriete.d \
./Groupe.d \
./MatrixTools.d \
./Modification.d \
./Objet.d \
./Point.d \
./ProjectionTools.d \
./Rectangle.d \
./Scene.d \
./Segment.d \
./Selection.d \
./TransfoTools.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/cairo -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<" `pkg-config --libs --cflags gtk+-2.0`
	@echo 'Finished building: $<'
	@echo ' '


