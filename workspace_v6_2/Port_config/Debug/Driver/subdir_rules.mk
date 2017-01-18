################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Driver/driver_aktorik.obj: ../Driver/driver_aktorik.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"" -vmspx --data_model=restricted -Ooff --opt_for_speed=0 --use_hw_mpy=F5 --include_path="/msp430/include" --include_path="C:/Users/Sebo/Documents/Crazy_car/workspace_v6_2/Port_config" --include_path="/include" --advice:power=all -g --define=__MSP430F5335__ --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Driver/driver_aktorik.d" --obj_directory="Driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Driver/driver_general.obj: ../Driver/driver_general.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"" -vmspx --data_model=restricted -Ooff --opt_for_speed=0 --use_hw_mpy=F5 --include_path="/msp430/include" --include_path="C:/Users/Sebo/Documents/Crazy_car/workspace_v6_2/Port_config" --include_path="/include" --advice:power=all -g --define=__MSP430F5335__ --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Driver/driver_general.d" --obj_directory="Driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Driver/driver_lcd.obj: ../Driver/driver_lcd.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"" -vmspx --data_model=restricted -Ooff --opt_for_speed=0 --use_hw_mpy=F5 --include_path="/msp430/include" --include_path="C:/Users/Sebo/Documents/Crazy_car/workspace_v6_2/Port_config" --include_path="/include" --advice:power=all -g --define=__MSP430F5335__ --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Driver/driver_lcd.d" --obj_directory="Driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


