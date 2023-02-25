#ifndef __ENUM_HPP_
#define __ENUM_HPP_

#include <map>
#include <string>

namespace ttr{
    enum class BlockType{
        I,
        J,
        L,
        O,
        S,
        T,
        Z,

        Empty,
        Yellow = O,
        Light_blue = I,
        Purple = T,
        Orange = L,
        Dark_blue = J,
        Green = S,
        Red = Z,
        Ghost,
    };

    static const ttr::BlockType All_types[] = {
        ttr::BlockType::Empty,
        ttr::BlockType::Yellow,
        ttr::BlockType::Light_blue,
        ttr::BlockType::Purple,
        ttr::BlockType::Orange,
        ttr::BlockType::Dark_blue,
        ttr::BlockType::Green,
        ttr::BlockType::Red,
        ttr::BlockType::Ghost
    };

    static const std::map<ttr::BlockType, std::string> string_type = {
        {ttr::BlockType::Empty, "Empty"},
        {ttr::BlockType::Yellow, "Yellow"},
        {ttr::BlockType::Light_blue, "Light_blue"},
        {ttr::BlockType::Purple, "Purple"},
        {ttr::BlockType::Orange, "Orange"},
        {ttr::BlockType::Dark_blue, "Dark_blue"},
        {ttr::BlockType::Green, "Green"},
        {ttr::BlockType::Red, "Red"},
        {ttr::BlockType::Ghost, "Ghost"}
    };

    static const ttr::BlockType All_blocks[] = {
        ttr::BlockType::I,
        ttr::BlockType::J,
        ttr::BlockType::L,
        ttr::BlockType::O,
        ttr::BlockType::S,
        ttr::BlockType::T,
        ttr::BlockType::Z
    };
}

#endif