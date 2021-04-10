/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: userinfo.proto */

#ifndef PROTOBUF_C_userinfo_2eproto__INCLUDED
#define PROTOBUF_C_userinfo_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _User__UserInformation User__UserInformation;


/* --- enums --- */


/* --- messages --- */

/*
 *User information
 */
struct  _User__UserInformation
{
  ProtobufCMessage base;
  uint32_t age;
  uint32_t height;
  uint32_t weight;
  uint32_t sex;
};
#define USER__USER_INFORMATION__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&user__user_information__descriptor) \
    , 0, 0, 0, 0 }


/* User__UserInformation methods */
void   user__user_information__init
                     (User__UserInformation         *message);
size_t user__user_information__get_packed_size
                     (const User__UserInformation   *message);
size_t user__user_information__pack
                     (const User__UserInformation   *message,
                      uint8_t             *out);
size_t user__user_information__pack_to_buffer
                     (const User__UserInformation   *message,
                      ProtobufCBuffer     *buffer);
User__UserInformation *
       user__user_information__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   user__user_information__free_unpacked
                     (User__UserInformation *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*User__UserInformation_Closure)
                 (const User__UserInformation *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor user__user_information__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_userinfo_2eproto__INCLUDED */