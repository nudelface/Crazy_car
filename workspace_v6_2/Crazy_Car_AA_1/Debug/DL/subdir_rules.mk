################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
DL/driver_aktorik.obj: ../DL/driver_aktorik.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/msp430/include" --include_path="/include" --advice:power=all -g --define=__MSP430F5335__ --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="DL/driver_aktorik.d" --obj_directory="DL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

DL/driver_general.obj: ../DL/driver_general.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/msp430/include" --include_path="/include" --advice:power=all -g --define=__MSP430F5335__ --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="DL/driver_general.d" --obj_directory="DL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

DL/driver_lcd.obj: ../DL/driver_lcd.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/msp430/include" --include_path="/include" --advice:power=all -g --define=__MSP430F5335__ --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="DL/driver_lcd.d" --obj_directory="DL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


