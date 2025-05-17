################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Dash_bitmaps/back.c \
../Core/Src/Dash_bitmaps/flecha.c \
../Core/Src/Dash_bitmaps/font16.c \
../Core/Src/Dash_bitmaps/font24.c \
../Core/Src/Dash_bitmaps/font32.c \
../Core/Src/Dash_bitmaps/font8.c \
../Core/Src/Dash_bitmaps/logo.c \
../Core/Src/Dash_bitmaps/precharge.c \
../Core/Src/Dash_bitmaps/refri.c 

OBJS += \
./Core/Src/Dash_bitmaps/back.o \
./Core/Src/Dash_bitmaps/flecha.o \
./Core/Src/Dash_bitmaps/font16.o \
./Core/Src/Dash_bitmaps/font24.o \
./Core/Src/Dash_bitmaps/font32.o \
./Core/Src/Dash_bitmaps/font8.o \
./Core/Src/Dash_bitmaps/logo.o \
./Core/Src/Dash_bitmaps/precharge.o \
./Core/Src/Dash_bitmaps/refri.o 

C_DEPS += \
./Core/Src/Dash_bitmaps/back.d \
./Core/Src/Dash_bitmaps/flecha.d \
./Core/Src/Dash_bitmaps/font16.d \
./Core/Src/Dash_bitmaps/font24.d \
./Core/Src/Dash_bitmaps/font32.d \
./Core/Src/Dash_bitmaps/font8.d \
./Core/Src/Dash_bitmaps/logo.d \
./Core/Src/Dash_bitmaps/precharge.d \
./Core/Src/Dash_bitmaps/refri.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Dash_bitmaps/%.o Core/Src/Dash_bitmaps/%.su Core/Src/Dash_bitmaps/%.cyclo: ../Core/Src/Dash_bitmaps/%.c Core/Src/Dash_bitmaps/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Dash_bitmaps

clean-Core-2f-Src-2f-Dash_bitmaps:
	-$(RM) ./Core/Src/Dash_bitmaps/back.cyclo ./Core/Src/Dash_bitmaps/back.d ./Core/Src/Dash_bitmaps/back.o ./Core/Src/Dash_bitmaps/back.su ./Core/Src/Dash_bitmaps/flecha.cyclo ./Core/Src/Dash_bitmaps/flecha.d ./Core/Src/Dash_bitmaps/flecha.o ./Core/Src/Dash_bitmaps/flecha.su ./Core/Src/Dash_bitmaps/font16.cyclo ./Core/Src/Dash_bitmaps/font16.d ./Core/Src/Dash_bitmaps/font16.o ./Core/Src/Dash_bitmaps/font16.su ./Core/Src/Dash_bitmaps/font24.cyclo ./Core/Src/Dash_bitmaps/font24.d ./Core/Src/Dash_bitmaps/font24.o ./Core/Src/Dash_bitmaps/font24.su ./Core/Src/Dash_bitmaps/font32.cyclo ./Core/Src/Dash_bitmaps/font32.d ./Core/Src/Dash_bitmaps/font32.o ./Core/Src/Dash_bitmaps/font32.su ./Core/Src/Dash_bitmaps/font8.cyclo ./Core/Src/Dash_bitmaps/font8.d ./Core/Src/Dash_bitmaps/font8.o ./Core/Src/Dash_bitmaps/font8.su ./Core/Src/Dash_bitmaps/logo.cyclo ./Core/Src/Dash_bitmaps/logo.d ./Core/Src/Dash_bitmaps/logo.o ./Core/Src/Dash_bitmaps/logo.su ./Core/Src/Dash_bitmaps/precharge.cyclo ./Core/Src/Dash_bitmaps/precharge.d ./Core/Src/Dash_bitmaps/precharge.o ./Core/Src/Dash_bitmaps/precharge.su ./Core/Src/Dash_bitmaps/refri.cyclo ./Core/Src/Dash_bitmaps/refri.d ./Core/Src/Dash_bitmaps/refri.o ./Core/Src/Dash_bitmaps/refri.su

.PHONY: clean-Core-2f-Src-2f-Dash_bitmaps

