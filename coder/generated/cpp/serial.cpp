#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msg.h>
#include <codec.h>

FrameEncoder frame_encoder(256);
const uint32_t HOVERBOARD_ID = FNV("hoverboard");
Log logger(256);

Result<Void> add_header(FrameEncoder& encoder, MsgHeader& header) {
    return header.encode(encoder);
}



uint32_t get_txd_info(uint8_t** buffer, uint32_t idx) {
    MsgHeader header;
    header.src = Option<uint32_t>::Some(FNV_ID);
    header.msg_type = MsgType::Info;

    frame_encoder.clear();
    if (header.encode(frame_encoder).is_err()) {
        return 0;
    }
    {
        frame_encoder.begin_map();
        frame_encoder.add_map(InfoPropertyId::PROP_ID, props[idx].id);
        frame_encoder.encode_uint32(InfoPropertyId::NAME);
        frame_encoder.encode_str(props[idx].name);
        frame_encoder.encode_uint32(InfoPropertyId::DESCRIPTION);
        frame_encoder.encode_str(props[idx].description);
        frame_encoder.add_map(InfoPropertyId::TYPE, props[idx].ValueType);
        frame_encoder.add_map(InfoPropertyId::MODE, props[idx].ValueMode);
        frame_encoder.end_map();
    }
    if (frame_encoder.add_crc().is_err()) {
        return 0;
    }
    if (frame_encoder.add_cobs().is_err()) {
        return 0;
    }

    *buffer = frame_encoder.data();
    return frame_encoder.size();
}

static bool send_info = false;
static uint32_t  property_index = 0;

bool toggle() {
    send_info = !send_info;
    return send_info;
}

uint32_t get_prop_or_object(uint8_t** buffer) {
    if (property_index < PROP_COUNT) {
        return get_txd_info(buffer, property_index++);
    }
    else {
        property_index = 0;
        return get_info_object(buffer);
    }
}


extern "C" uint32_t get_txd(uint8_t** buffer) {
    if (toggle()) {
        return get_prop_or_object(buffer);
    }
    MsgHeader header;
    header.src = Option<uint32_t>::Some(FNV_ID);
    header.msg_type = MsgType::Pub;

    frame_encoder.clear();

    if (header.encode(frame_encoder).is_err()) {
        return 0;
    }

    if (encode_vars(frame_encoder).is_err()) {
        return 0;
    }
    if (frame_encoder.add_crc().is_err()) {
        return 0;
    }
    if (frame_encoder.add_cobs().is_err()) {
        return 0;
    }
    *buffer = frame_encoder.data();

    return frame_encoder.size();
}





/*

Values are being translated to external<>internal format automatically.

Type	Name	Description	Can be Set	Can be saved to EEPROM
Parameter	CTRL_MOD	Ctrl mode 1:Voltage 2:Speed 3:Torque	Yes	No
Parameter	CTRL_TYP	Ctrl type 0:Commutation 1:Sinusoidal 2:FOC	Yes	No
Parameter	I_MOT_MAX	Max phase current A	Yes	Yes
Parameter	N_MOT_MAX	Max motor RPM	Yes	Yes
Parameter	FI_WEAK_ENA	Enable field weak 0:OFF 1:ON	Yes	No
Parameter	FI_WEAK_HI	Field weak high RPM	Yes	No
Parameter	FI_WEAK_LO	Field weak low RPM	Yes	No
Parameter	FI_WEAK_MAX	Field weak max current A(FOC only)	Yes	No
Parameter	PHA_ADV_MAX	Max Phase Adv angle Deg(SIN only)	Yes	No
Variable	IN1_RAW	Input1 raw value	No	No
Parameter	IN1_TYP	Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect	Yes	Yes
Parameter	IN1_MIN	Input1 minimum value	Yes	Yes
Parameter	IN1_MID	Input1 middle value	Yes	Yes
Parameter	IN1_MAX	Input1 maximum value	Yes	Yes
Variable	IN1_CMD	Input1 command value	No	No
Variable	IN2_RAW	Input2 raw value	No	No
Parameter	IN2_TYP	Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect	Yes	Yes
Parameter	IN2_MIN	Input2 minimum value	Yes	Yes
Parameter	IN2_MID	Input2 middle value	Yes	Yes
Parameter	IN2_MAX	Input2 maximum value	Yes	Yes
Variable	IN2_CMD	Input2 command value	No	No
Variable	DC_CURR	Total DC Link current A *100	No	No
Variable	LDC_CURR	Left DC Link current A *100	No	No
Variable	RDC_CURR	Right DC Link current A *100	No	No
Variable	CMDL	Left Motor Command RPM	No	No
Variable	CMDR	Right Motor Command RPM	No	No
Variable	SPD_AVG	Motor Measured Avg RPM	No	No
Variable	SPDL	Left Motor Measured RPM	No	No
Variable	SPDR	Right Motor Measured RPM	No	No
Variable	RATE	Rate *10	No	No
Variable	SPD_COEF	Speed Coefficient *10	No	No
Variable	STR_COEF	Steer Coefficient *10	No	No
Variable	BATV	Calibrated Battery Voltage *100	No	No
Variable	TEMP	Calibrated Temperature °C *10	No	No
🧰 Troubleshooting:
Make sure the baud rate is 115200
Make sure you are using the sensor cable(left=DEBUG_SERIAL_USART2 or right=DEBUG_SERIAL_USART3) selected in config.h
TX can be defective on your mainboard, you can switch to other sensor cable if not used already in config.h
On some boards the wire colors might differ, try switching the green wire
Make sure the RX on your FTDI is working. You can connect the RX and TX on the FTDI and check if you received the commands you send in the Web Tool tool or any serial monitor
 Add a custom footer
Pages 28
Buying a used hoverboard
Firmware compatibility
Hoverboard Wheels
How to Unlock MCU Flash
Variants
ADC
USART
NUNCHUK
PPM
PWM
IBUS
HOVERCAR
TRANSPOTTER
SKATEBOARD
HOVERBOARD
BBCAR
Dual Inputs
Compiling and flashing the firmware
Input Calibration
Parameters
Setup current and speed limits
Battery
Diagnostics
Troubleshooting
Debug Serial
Sideboards
Clone this wiki locally
*/