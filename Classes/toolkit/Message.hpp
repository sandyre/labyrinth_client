//
//  Message.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 09/09/2017.
//
//

#ifndef Message_h
#define Message_h

#include <deque>
#include <memory>
#include <vector>

#include <mflatbuffers/flatbuffers.h>

#define CONSTRUCT_MESSAGEBUFFER(builder) std::make_shared<MessageBuffer>(builder.GetBufferPointer(), builder.GetBufferPointer() + builder.GetSize())

using MessageBuffer = std::vector<uint8_t>;
using MessageBufferPtr = std::shared_ptr<MessageBuffer>;

using MessageStorage = std::deque<MessageBufferPtr>;

#endif /* Message_h */
