################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Camera.c \
../Clavier.c \
../Color.c \
../Cube.c \
../ExportWindow.c \
../FenPrincipale.c \
../FenetreAjoutCube.c \
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
../Sphere.c \
../Text.c \
../TransfoTools.c \
../main.c 

OBJS += \
./Camera.o \
./Clavier.o \
./Color.o \
./Cube.o \
./ExportWindow.o \
./FenPrincipale.o \
./FenetreAjoutCube.o \
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
./Sphere.o \
./Text.o \
./TransfoTools.o \
./main.o 

C_DEPS += \
./Camera.d \
./Clavier.d \
./Color.d \
./Cube.d \
./ExportWindow.d \
./FenPrincipale.d \
./FenetreAjoutCube.d \
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
./Sphere.d \
./Text.d \
./TransfoTools.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/cairo -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<" `pkg-config --cflags --libs gtk+-2.0`
	@echo 'Finished building: $<'
	@echo ' '


