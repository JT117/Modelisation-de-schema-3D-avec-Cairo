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
../FenSeg.c \
../FenText.c \
../FenTrans.c \
../FenetreAjout.c \
../FenetrePropriete.c \
../Groupe.c \
../MatrixTools.c \
../Modification.c \
../MouseInterface.c \
../Objet.c \
../Point.c \
../ProjectionTools.c \
../Pyramid.c \
../Quadrilateral.c \
../Rectangle.c \
../Scene.c \
../Segment.c \
../Selection.c \
../Sphere.c \
../Text.c \
../Transformation.c \
../Triangle.c \
../main.c 

OBJS += \
./Camera.o \
./Clavier.o \
./Color.o \
./Cube.o \
./ExportWindow.o \
./FenPrincipale.o \
./FenSeg.o \
./FenText.o \
./FenTrans.o \
./FenetreAjout.o \
./FenetrePropriete.o \
./Groupe.o \
./MatrixTools.o \
./Modification.o \
./MouseInterface.o \
./Objet.o \
./Point.o \
./ProjectionTools.o \
./Pyramid.o \
./Quadrilateral.o \
./Rectangle.o \
./Scene.o \
./Segment.o \
./Selection.o \
./Sphere.o \
./Text.o \
./Transformation.o \
./Triangle.o \
./main.o 

C_DEPS += \
./Camera.d \
./Clavier.d \
./Color.d \
./Cube.d \
./ExportWindow.d \
./FenPrincipale.d \
./FenSeg.d \
./FenText.d \
./FenTrans.d \
./FenetreAjout.d \
./FenetrePropriete.d \
./Groupe.d \
./MatrixTools.d \
./Modification.d \
./MouseInterface.d \
./Objet.d \
./Point.d \
./ProjectionTools.d \
./Pyramid.d \
./Quadrilateral.d \
./Rectangle.d \
./Scene.d \
./Segment.d \
./Selection.d \
./Sphere.d \
./Text.d \
./Transformation.d \
./Triangle.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/cairo -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<" `pkg-config --cflags --libs gtk+-2.0`
	@echo 'Finished building: $<'
	@echo ' '


