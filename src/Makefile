#************************************************
# Monitor LPC2378
# Marcos Augusto Stemmer
#*************************************************/

# Targets for PC
HW_SPECIFIC_IMPL = infra_pucrsDev.o io_pucrsDev.o time_pucrsDev.o temperature_pucrsDev.o

#Configuracoes especificas deste projeto
SERIALDEV = /dev/ttyS0
CLOCKFREQ = 12000
TARGET = a01
MODULOS =  sts_cfg.o general.o main.o crt.o $(HW_SPECIFIC_IMPL)
TERMINAL = ltser
BAUDRATE=19200

#Nome do compilador C, assembler e linker
CC      = arm-elf-gcc
LD      = arm-elf-gcc
AS	= arm-elf-as
AFLAGS  = -mapcs-32 -mcpu=arm7tdmi
CFLAGS  = -Wall -O2 -mcpu=arm7tdmi-s -D BAUDRATE=$(BAUDRATE) -I../include
LFLAGS  = -nostartfiles

all: $(TARGET).hex

#Converte arquivo elf para hex
%.hex: %.elf
	arm-elf-objcopy -O ihex $< $@

#Chama o compilador c
%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) -o $@ $<

#Compila os modulos em assembly
%.o: %.S
	$(AS) $(AFLAGS) -o $@ $<

#Chama o linker/loader para juntar os módulos gerando o arquivo elf
$(TARGET).elf: $(MODULOS)
	$(LD) $(LFLAGS) -Tlpc2378_flash.ld -o $(TARGET).elf $(MODULOS)

$(TARGET)r.elf: $(MODULOS)
	$(LD) $(LFLAGS) -Tlpc2378_ram.ld -o $(TARGET)r.elf $(MODULOS)

#Chama o terminal e executa o programa na RAM (necessita do mon23)
tser: $(TARGET)r.hex
	$(TERMINAL) $(SERIALDEV) b=$(BAUDRATE) $(TARGET)r.hex

tseru: $(TARGET)r.hex
	$(TERMINAL) /dev/ttyUSB0 b=$(BAUDRATE) $(TARGET)r.hex

# Use 'make isp' para programar a memoria flash
isp: $(TARGET).hex
	lpc21isp $(TARGET).hex $(SERIALDEV) $(BAUDRATE) $(CLOCKFREQ)
ispu: $(TARGET).hex
	lpc21isp $(TARGET).hex /dev/ttyUSB0 $(BAUDRATE) $(CLOCKFREQ)
isp1: $(TARGET).hex
	lpc21isp $(TARGET).hex com1 $(BAUDRATE) $(CLOCKFREQ)
isp2: $(TARGET).hex
	lpc21isp $(TARGET).hex com2 $(BAUDRATE) $(CLOCKFREQ)
isp3: $(TARGET).hex
	lpc21isp $(TARGET).hex com3 $(BAUDRATE) $(CLOCKFREQ)
isp4: $(TARGET).hex
	lpc21isp $(TARGET).hex com4 $(BAUDRATE) $(CLOCKFREQ)
isp5: $(TARGET).hex
	lpc21isp $(TARGET).hex com5 $(BAUDRATE) $(CLOCKFREQ)

#Limpa, apagando os arquivos gerados pela compilacao
clean:
	-rm -f *.o *.elf *~ *.bin *.map *.hex

