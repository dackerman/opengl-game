################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DackGLImporter.cpp \
../Shader.cpp \
../ShaderProgram.cpp \
../main.cpp 

OBJS += \
./DackGLImporter.o \
./Shader.o \
./ShaderProgram.o \
./main.o 

CPP_DEPS += \
./DackGLImporter.d \
./Shader.d \
./ShaderProgram.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


