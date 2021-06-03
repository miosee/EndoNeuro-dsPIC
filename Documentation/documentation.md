# Endoneuro Rpi-dsPIC Specifications

## ADC specifications

- The ADC is 16 bits and has 4 input channels.  It is used to acquire 2 VENG channels and two EEg channels.
- The VENG channels are sampled at 20kHz.
- The EEG channels are sampled at 1kHz.

## dsPIC firmware

### Sampling strategy

The ADC is connected to the SPI2 peripheral.
The sampling period (50µs) is defined by TIMER2.

The acquisition is started in the TIMER2 ISR. It initiates the first SPI transaction. The acquisition is then handled by SPI2 ISR.

The VENG channels are sampled every TIMER2 overflow and EEG channels are sampled every 20 overflows.

### Rpi-dsPIC communication frame

