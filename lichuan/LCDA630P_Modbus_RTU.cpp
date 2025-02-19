#include "LCDA630P_Modbus_RTU.hpp"

LCDA630P_Modbus_RTU::LCDA630P_Modbus_RTU()
{
    DEBUG_SERIAL_PRINTLN("init");
};
void LCDA630P_Modbus_RTU::scan_devices()
{
    DEBUG_SERIAL_PRINTLN("Scanning devices");
}

std::vector<uint8_t> LCDA630P_Modbus_RTU::read_parameter(uint8_t slave_id, uint8_t group_number, uint8_t parameter_offset, uint16_t function_code_number)
{
    std::vector<uint8_t> frame;
    frame.push_back(slave_id); // ADR
    frame.push_back(0x03);     // Read Holding Register
    frame.push_back(group_number);
    frame.push_back(parameter_offset);
    frame.push_back(function_code_number >> 8);
    frame.push_back(function_code_number & 0xFF);

    // Calculate CRC using uint8_t data
    uint16_t crc = crcValueCalc(frame.data(), frame.size());
    frame.push_back(crc & 0xFF);        // Low byte
    frame.push_back((crc >> 8) & 0xFF); // High byte

#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif
    return frame;
}
std::vector<uint8_t> LCDA630P_Modbus_RTU::write_parameter(uint8_t slave_id, uint8_t group_number, uint8_t parameter_offset, uint16_t value)
{
    std::vector<uint8_t> frame;
    frame.push_back(slave_id); // ADR
    frame.push_back(0x06);     // Write Holding Register
    frame.push_back(group_number);
    frame.push_back(parameter_offset);
    frame.push_back(value >> 8);
    frame.push_back(value & 0xFF);

    // Calculate CRC using uint8_t data
    uint16_t crc = crcValueCalc(frame.data(), frame.size());
    frame.push_back(crc & 0xFF);        // Low byte
    frame.push_back((crc >> 8) & 0xFF); // High byte
#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif
    return frame;
};
std::vector<uint8_t> LCDA630P_Modbus_RTU::write_parameter_32(uint8_t slave_id, uint8_t group_number, uint8_t parameter_offset, uint32_t value)
{
    std::vector<uint8_t> frame;
    frame.push_back(slave_id); // ADR
    frame.push_back(0x10);     // Write Holding Register
    frame.push_back(group_number);
    frame.push_back(parameter_offset);
    frame.push_back(0x00);
    frame.push_back(0x02);
    frame.push_back(0x04);
    frame.push_back((value >> 8) & 0xFF);
    frame.push_back(value & 0xFF);
    frame.push_back((value >> 16) & 0xFF);
    frame.push_back((value >> 24) & 0xFF);
    // Calculate CRC using uint8_t data
    uint16_t crc = crcValueCalc(frame.data(), frame.size());
    frame.push_back(crc & 0xFF);        // Low byte
    frame.push_back((crc >> 8) & 0xFF); // High byte
#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif
    return frame;    
}
std::vector<std::vector<uint8_t>> LCDA630P_Modbus_RTU::read_servo_brief(uint8_t slave_id)
{
    std::vector<std::vector<uint8_t>> list_of_commands;
    list_of_commands.push_back(read_parameter(slave_id, 0, 0, 2));
    list_of_commands.push_back(read_parameter(slave_id, 0, 9, 2));
    list_of_commands.push_back(read_parameter(slave_id, 0, 10, 2));
    list_of_commands.push_back(read_parameter(slave_id, 0, 11, 2));
    list_of_commands.push_back(read_parameter(slave_id, 0, 12, 2));
    list_of_commands.push_back(read_parameter(slave_id, 0, 13, 2));
    list_of_commands.push_back(read_parameter(slave_id, 0, 14, 2));
    list_of_commands.push_back(read_parameter(slave_id, 0, 15, 2));
    list_of_commands.push_back(read_parameter(slave_id, 0, 28, 2));
    return list_of_commands;
}
std::vector<std::vector<uint8_t>> LCDA630P_Modbus_RTU::raw_one_rotation(uint8_t slave_id)
{
    std::vector<std::vector<uint8_t>> list_of_commands;
    std::vector<uint8_t> frame;
    frame.push_back(slave_id);
    frame.push_back(0x06);
    frame.push_back(0x17);
    frame.push_back(0x00);
    frame.push_back(0x00);
    frame.push_back(0x01);
    frame.push_back(0x4D);
    frame.push_back(0xBE);
    list_of_commands.push_back(frame);
#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif    
    frame.clear();
    frame.push_back(slave_id);
    frame.push_back(0x06);
    frame.push_back(0x17);
    frame.push_back(0x02);
    frame.push_back(0x00);
    frame.push_back(0x1C);
    frame.push_back(0x2C);
    frame.push_back(0x77);
    list_of_commands.push_back(frame);
#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif      
    frame.clear();
    frame.push_back(slave_id);
    frame.push_back(0x06);
    frame.push_back(0x02);
    frame.push_back(0x00);
    frame.push_back(0x00);
    frame.push_back(0x01);
    frame.push_back(0x49);
    frame.push_back(0xB2);
    list_of_commands.push_back(frame);   
#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif       
    frame.clear();
    frame.push_back(slave_id);
    frame.push_back(0x06);
    frame.push_back(0x05);
    frame.push_back(0x00);
    frame.push_back(0x00);
    frame.push_back(0x02);
    frame.push_back(0x08);
    frame.push_back(0xC7);
    list_of_commands.push_back(frame); 
#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif      
    frame.clear();
    frame.push_back(slave_id);
    frame.push_back(0x06);
    frame.push_back(0x11);
    frame.push_back(0x00);
    frame.push_back(0x00);
    frame.push_back(0x03);
    frame.push_back(0xCC);
    frame.push_back(0xF7);
    list_of_commands.push_back(frame);  
#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif         
    frame.clear();
    frame.push_back(slave_id);
    frame.push_back(0x10);
    frame.push_back(0x11);
    frame.push_back(0x0C);
    frame.push_back(0x00);
    frame.push_back(0x02);
    frame.push_back(0x04);
    frame.push_back(0x27);
    frame.push_back(0x10);
    frame.push_back(0x00);
    frame.push_back(0x00);
    frame.push_back(0x38);
    frame.push_back(0xDB);
    list_of_commands.push_back(frame); 
#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif           
    frame.clear();
    frame.push_back(slave_id);
    frame.push_back(0x06);
    frame.push_back(0x31);
    frame.push_back(0x00);
    frame.push_back(0x00);
    frame.push_back(0x01);
    frame.push_back(0x46);
    frame.push_back(0xF6);
    list_of_commands.push_back(frame);  
#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif      
    frame.clear();
    frame.push_back(slave_id);
    frame.push_back(0x06);
    frame.push_back(0x31);
    frame.push_back(0x00);
    frame.push_back(0x00);
    frame.push_back(0x03);
    frame.push_back(0xC7);
    frame.push_back(0x37);
    list_of_commands.push_back(frame);  
#if DEBUG_SERIAL
    for (int i = 0; i < frame.size(); i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(frame[i]) << " ";
        DEBUG_SERIAL_PRINT(ss.str());
    }
    DEBUG_SERIAL_PRINTLN("");
#endif              
    return list_of_commands ;
}
std::vector<std::vector<uint8_t>> LCDA630P_Modbus_RTU::move_to_position(uint8_t slave_id, u_int32_t position)
{
    std::vector<std::vector<uint8_t>> list_of_commands ;
    list_of_commands.push_back(write_parameter_32(1,0x11,0xC,position));//Multi-segment location operation mode Sequential operation (P11-01 for selection of segment number)
    list_of_commands.push_back(write_parameter(1,0x31,0,1));//Communication given VDI virtual level 0～65535
    list_of_commands.push_back(write_parameter(1,0x31,0,3));//Communication given VDI virtual level 0～65535
    return list_of_commands;
}
std::vector<std::vector<uint8_t>> LCDA630P_Modbus_RTU::config_for_modbus_control(uint8_t slave_id)
{
    std::vector<std::vector<uint8_t>> list_of_commands ;
    list_of_commands.push_back(write_parameter(1,0x17,0,1));//VDI1 Terminal function selection
    list_of_commands.push_back(write_parameter(1,0x17,2,28));//VDI2 Terminal function selection
    list_of_commands.push_back(write_parameter(1,0x2,0,1));//Control Mode Selectio 1: position mod
    list_of_commands.push_back(write_parameter(1,0x5,0,2));//Location instruction source multi-segment position instruction give
    list_of_commands.push_back(write_parameter(1,0x11,0,3));//Multi-segment location operation mode Sequential operation (P11-01 for selection of segment number)
    return list_of_commands;
}
std::string LCDA630P_Modbus_RTU::vector_to_string(std::vector<uint8_t> frame)
{
    // Convert uint16_t array to string
    std::string request_string = "";
    int size_of_frame = 8;
    if (frame[1] == 0x06)
        size_of_frame = 8;
    else if (frame[1] == 0x10)
        size_of_frame = 13;

    for (int i = 0; i < size_of_frame ; i++)
    {
        request_string += static_cast<char>(frame[i]);        // Get low byte
    }
    return request_string ;
}
uint32_t LCDA630P_Modbus_RTU::parseModbusResponse(const std::vector<uint8_t> &response)
{
    if (response.size() < 7) {
        throw std::runtime_error("Invalid Modbus response: too short");
    }
    // Extract first word (0x2d15)
    uint32_t value = 0;
    if (response.size() > 8 )
        value = (response[8] << 0) | (response[7] << 8) | (response[9] << 16) | (response[10] << 24 );
    else 
        value = (response[5] << 8) | response[4];
#if DEBUG_SERIAL or true
    std::stringstream ss ;
    ss << std::hex << std::setfill('0') << std::setw(2) << "adr: " << static_cast<int>(response[0]) << "\tf :" << 
        static_cast<int>(response[1]) << "\tp" << static_cast<int>(response[2]) << "-" << 
        static_cast<int>(response[3]) << "\tsize: " << std::dec << std::setw(2) << response.size() << 
        "\tvalue : " << value << "\t hex: " << std::hex << std::setfill('0') << std::setw(2) << "0x" << 
        static_cast<int>(value) << std::endl;
    DEBUG_SERIAL_PRINT(ss.str());
#endif
    return value ;
};
uint16_t LCDA630P_Modbus_RTU::crcValueCalc(const uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (uint16_t j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
};